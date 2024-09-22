/**
 * @file main.cpp
 * @brief メインプログラム
 * @details 格子立方体または格子直方体の寸法に基づいて，その格子展開図を生成し，
 * 各面，辺，頂点のIDや状態を更新したのちに，隣接リストを出力する
 * @date 2024/06/24 最終更新
 * @version 1.0
 * @author Takumi SHIOTA
 */

/*************************************************
 * 本プログラム全体での用語の定義
 * 
 * 直方体を大域的に見た時の各面（長方形）の
 * 呼び方を以下のように定義する
 * 
 * α√(a^2+b^2) * β√(a^2+b^2) のサイズの長方形の
 * 面の番号は下記の通り：
 * 
 *          4
 *    0 1 2 3
 *          5
 * 
 * 以降，直方体を格子関係無しに（大域的に）見た時の辺を
 * 「面 X の D を通る線分」と呼ぶ
 * 
 * 格子展開図の面，辺，頂点に対して，
 * 各格子グリッド（各マス目）は，大きく以下の3つに分類される：
 * - 内包される
 * - 交差している（頂点に対しては存在しない）
 * - 内包されない
 * 
 * 以降，これらの 3 つの分類のことを「状態」と呼ぶ
 *************************************************/

#include <iostream>
#include <set>
#include <vector>
#include "UnfoldingDimensions.hpp"
#include "Correspondence.hpp"
#include "AdjacentList.hpp"
#include "Labeling.hpp"

using namespace std;

int main() {
	UnfoldingDimensions unfoldDim;
	Labeling label;

	/*************************************************
	 * 格子立方体の一辺の長さを決める値を入力
	 * 入力は a ≤ b となるように調整
	 *************************************************/
	// cout << "a: "; 
	cin >> unfoldDim.a; 
	// cout << "b: "; 
	cin >> unfoldDim.b;
	if(unfoldDim.a > unfoldDim.b) {
		swap(unfoldDim.a, unfoldDim.b);
	}

	/*************************************************
	 * 格子直方体の縦の長さ・横の長さ・高さを入力
	 * 入力は x ≤ y ≤ z となるように調整
	 *************************************************/
	// cout << "x: "; 
	cin >> unfoldDim.x; 
	// cout << "y: "; 
	cin >> unfoldDim.y; 
	// cout << "z: "; 
	cin >> unfoldDim.z; 
	if (unfoldDim.x > unfoldDim.y) swap(unfoldDim.x, unfoldDim.y);
	if (unfoldDim.x > unfoldDim.z) swap(unfoldDim.x, unfoldDim.z);
	if (unfoldDim.y > unfoldDim.z) swap(unfoldDim.y, unfoldDim.z);

	/*************************************************
	 * 盤面の上下左右の余白の値を決定
	 *************************************************/
	unfoldDim.margin_top = 1;
	unfoldDim.margin_bottom = 1;
	unfoldDim.margin_left = 1;
	unfoldDim.margin_right = 1;

	/*************************************************
	 * 盤面の縦幅と横幅のサイズを計算
	 *************************************************/
	unfoldDim.width = unfoldDim.margin_left			// 左の余白
					+ unfoldDim.a * unfoldDim.y		// 面 0 の左を通る線分の幅
					+ unfoldDim.b * unfoldDim.x		// 面 0 の上を通る線分の幅
					+ unfoldDim.b * unfoldDim.z		// 面 1 の上を通る線分の幅
					+ unfoldDim.b * unfoldDim.x		// 面 2 の上を通る線分の幅
					+ unfoldDim.a * unfoldDim.x		// 面 4 の左を通る線分の幅
					+ unfoldDim.b * unfoldDim.z		// 面 4 の上を通る線分の幅
					+ unfoldDim.margin_right;		// 右の余白
	unfoldDim.height = unfoldDim.margin_top			// 上の余白
					 + unfoldDim.b * unfoldDim.y	// 面 0 の左を通る線分の高さ
					 + unfoldDim.a * unfoldDim.x	// 面 0 の下を通る線分の高さ
					 + unfoldDim.a * unfoldDim.z	// 面 1 の下を通る線分の高さ
					 + unfoldDim.a * unfoldDim.x	// 面 2 の下を通る線分の高さ
					 + unfoldDim.b * unfoldDim.x	// 面 5 の左を通る線分の高さ
					 + unfoldDim.a * unfoldDim.z	// 面 5 の下を通る線分の高さ
					 + unfoldDim.margin_bottom;		// 下の余白

	/*************************************************
	 * 面 4 の左を通過する線分の高さが
	 * 面 0, 1, 2 の上を通過する線分の高さより高い時，
	 * 面 4 が盤面の外に出てしまう．
	 * その際は，上余白が足りない際にシフトする値を計算
	 *************************************************/
	unfoldDim.top_shift = 0;
	int diff = unfoldDim.b * unfoldDim.x	// 面 4 の左を通る線分の高さ
			 - unfoldDim.a * unfoldDim.x	// 面 1 の上を通る線分の高さ
			 - unfoldDim.a * unfoldDim.z 	// 面 2 の上を通る線分の高さ
			 - unfoldDim.a * unfoldDim.x;	// 面 3 の上を通る線分の高さ
	if(diff > 0) unfoldDim.top_shift = diff;

	/*************************************************
	 * 面・辺・頂点の ID および状態を管理する配列の生成
	 *************************************************/
	label.initialize(unfoldDim);

	/*************************************************
	 * 格子展開図を盤面上に配置するした際の
	 * 展開図が配置されている面・辺・頂点のID および状態を計算
	 *************************************************/
	label.getFaces(unfoldDim);
	label.getVertices(unfoldDim);
	label.getEdges(unfoldDim);

	/*************************************************
	 * 隣接リストを作成・出力
	 *************************************************/
	vector<AdjacentList> adjLists;
	label.generateAdjacentList(adjLists, unfoldDim);

	/*************************************************
	 * 各種 ID および状態の配列を出力（デバッグ用）
	 *************************************************/
	// label.printAllIDs();
	// label.printAllStatuses();
	// label.printNonZeroStatuses();
}
