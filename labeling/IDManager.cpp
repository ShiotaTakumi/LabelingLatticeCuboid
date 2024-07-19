/**
 * @file IDManager.cpp
 * @brief IDManager クラスの実装
 * @date 2024/07/14 最終更新
 * @version 1.0
 * @author Takumi SHIOTA
 */

#include "IDManager.hpp"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <math.h>
#include "ArraySize.hpp"
#include "Coordinates.hpp"

using namespace std;

/*************************************************
 * 各種配列の初期化および出力
 *************************************************/

/**
 * @brief ID および状態を管理する配列の
 * サイズの決定および全ての値を 0 に初期化するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 */
void IDManager::initializeVecs(UnfoldingDimensions unfoldDim) {
	/*************************************************
	 * ID および状態を表す配列の縦横のサイズを決定
	 *************************************************/
	//! 配列の縦のサイズ
	int vecHeight = unfoldDim.height + unfoldDim.top_shift;
	//! 配列の横のサイズ
	int vecWidth = unfoldDim.width;

	/*************************************************
	 * 面・辺・頂点の ID を管理する配列のサイズを割り当て
	 *************************************************/
	faces_id.assign(vecHeight, vector<int>(vecWidth));
	edges_id.assign(2 * vecHeight, vector<int>(vecWidth));
	vertices_id.assign(vecHeight, vector<int>(vecWidth));

	/*************************************************
	 * 面・辺・頂点の ID の初期値を設定
	 *************************************************/
	initializeIDArrays(faces_id, 'f');
	initializeIDArrays(edges_id, 'e');
	initializeIDArrays(vertices_id, 'v');

	/*************************************************
	 * 面・辺・頂点の状態を管理する配列のサイズを割り当て
	 * 状態の初期値は全て 0 に初期化
	 *************************************************/
	faces_status.assign(vecHeight, vector<int>(vecWidth, 0));
	edges_status.assign(2 * vecHeight, vector<int>(vecWidth, 0));
	vertices_status.assign(vecHeight, vector<int>(vecWidth, 0));
}

/**
 * @brief 指定した ID を管理する配列の値を初期化するメンバ関数
 * @param id 指定する ID の配列（参照型）
 * @param c 配列の種類を管理するラベル
 * （f: faces_id，e: edges_id，v: vertices_id）
 */	
void IDManager::initializeIDArrays(std::vector<std::vector<int>>& id, char c) const {
	//! 割り当てる ID の値をカウントする変数
	int cnt = 0;
	if(c == 'f' || c == 'v') {
		for (int i = 0; i < id.size(); ++i) {
			for (int j = 0; j < id[i].size(); ++j) {
				id[i][j] = cnt++;
			}
		}
	} else {
		for (int i = 0; i < id.size(); i += 2) {
			for (int j = 0; j < id[i].size(); ++j) {
				id[i][j] = cnt++;
				id[i+1][j] = cnt++;
			}
		}		
	}
}

/**
 * @brief 指定した配列を出力するメンバ関数
 * @param array 指定する配列
 * @param name 指定する配列の名前
 * @param fillZero ゼロ埋めをするかを管理するラベル（True: ゼロ埋め，False: 何もしない）
 */
void IDManager::printVec(const std::vector<std::vector<int>> array, const std::string name, bool fillZero) {
	cout << name << ": " << endl;
	//! ゼロ埋めのために，指定する配列における右下の値（最大値）を格納する変数
	int max_val = array.size() * array[0].size() - 1;
	
	for (const auto& row : array) {
		for (int col : row) {
			if(fillZero){
				cout << setw(static_cast<int>(log10(max_val)) + 1) << setfill('0') << col << ' ';
			} else {
				cout << col << ' ';
			}
		}
		cout << endl;
	}	
}

/**
 * @brief 指定した配列のうち，状態が 0 でない値のみを出力するメンバ関数
 * @param id 指定する ID の配列
 * @param status 指定する状態の配列
 * @param name 指定する配列の名前
 */
void IDManager::printNonZeroVec(const std::vector<std::vector<int>> id, const vector<vector<int>> status, const std::string name) {
	cout << name << ": " << endl;
	//! ゼロ埋めのために，指定する配列における右下の値（最大値）を格納する変数
	int max_val = id.size() * id[0].size() - 1;

		for (int i = 0; i < id.size(); ++i) {
			for (int j = 0; j < id[i].size(); ++j) {
			if(status[i][j] != 0) cout << setw(static_cast<int>(log10(max_val)) + 1) << setfill('0') << id[i][j] << ' ';
			else cout << setw(static_cast<int>(log10(max_val)) + 1) << setfill('0') << 0 << ' ';
		}
		cout << endl;
	}	
}

/*************************************************
 * 面を表す配列に対する操作
 *************************************************/

/**
 * @brief 面の ID 配列のサイズ取得するメンバ関数
 * @return 面の ID の配列のサイズ（ArraySize 型）
 */
ArraySize IDManager::getFacesIDsize() {
	ArraySize array;
	array.row = faces_id.size();
	array.column = faces_id[0].size();
	return array;
}

/**
 * @brief 指定した座標における面の ID を取得するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @return 面の ID
 */
int IDManager::getFacesID(int x, int y) {
	return faces_id[x][y];
}

/**
 * @brief 指定した座標における面の状態を取得するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @return 面の状態
 */
int IDManager::getFacesStatus(int x, int y) {
	return faces_status[x][y];
}

/**
 * @brief 指定した面の ID から (x, y) 座標を取得するメンバ関数
 * @param id 指定した面の ID
 * @return (x, y) 座標（Coordinates 型）
 */
Coordinates IDManager::getFacesCoordintes(int id) {
	Coordinates coordinates;
	for(int i = 0; i < getFacesIDsize().row; ++i) {
		for(int j = 0; j < getFacesIDsize().column; ++j) {
			if(getFacesID(i, j) == id) {
				coordinates.x = i;
				coordinates.y = j;
			}
		}
	}
	return coordinates;
}

/**
 * @brief 指定した座標における面の ID を更新するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @param id 更新後の面の ID
 */
void IDManager::setFacesID(int x, int y, int id) {
	faces_id[x][y] = id;
}

/**
 * @brief 指定した座標における面の状態を更新するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @param id 更新後の面の状態
 */
void IDManager::setFacesStatus(int x, int y, int status) {
	faces_status[x][y] = status;
}

/*************************************************
 * 頂点を表す配列に対する操作
 *************************************************/

/**
 * @brief 頂点の ID 配列のサイズ取得するメンバ関数
 * @return 頂点の ID の配列のサイズ（ArraySize 型）
 */
ArraySize IDManager::getVerticesIDsize() {
	ArraySize array;
	array.row = vertices_id.size();
	array.column = vertices_id[0].size();
	return array;
}

/**
 * @brief 指定した座標における頂点の ID を取得するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @return 頂点の ID
 */
int IDManager::getVerticesID(int x, int y) {
	return vertices_id[x][y];
}

/**
 * @brief 指定した座標における頂点の状態を取得するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @return 頂点の状態
 */
int IDManager::getVerticesStatus(int x, int y) {
	return vertices_status[x][y];
}

/**
 * @brief 指定した座標における頂点の ID を更新するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @param id 更新後の頂点の ID
 */
void IDManager::setVerticesID(int x, int y, int id) {
	vertices_id[x][y] = id;
}

/**
 * @brief 指定した座標における頂点の状態を更新するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @param id 更新後の頂点の状態
 */
void IDManager::setVerticesStatus(int x, int y, int status) {
	vertices_status[y][x] = status;
}

/*************************************************
 * 辺を表す配列に対する操作
 *************************************************/

/**
 * @brief 指定した座標における辺の ID を取得するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @return 辺の ID
 */
int IDManager::getEdgesID(int x, int y) {
	return edges_id[x][y];
}

/**
 * @brief 指定した座標における辺の状態を取得するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @return 辺の状態
 */
int IDManager::getEdgesStatus(int x, int y) {
	return edges_status[x][y];
}

/**
 * @brief 指定した座標における辺の ID を更新するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @param id 更新後の辺の ID
 */
void IDManager::setEdgesID(int x, int y, int id) {
	edges_id[x][y] = id;
}

/**
 * @brief 指定した座標における辺の状態を更新するメンバ関数
 * @param x 指定した x 座標
 * @param y 指定した y 座標
 * @param id 更新後の辺の状態
 */
void IDManager::setEdgesStatus(int x, int y, int status) {
	edges_status[x][y] = status;
}

/*************************************************
 * 格子グリッドと展開図における辺の交差の管理
 *************************************************/

/**
 * @brief 一辺（a^2+b^2）の正方形と交わっている格子の個数を返すメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @return 交わっている格子の個数
*/
int IDManager::getCrossingNumber(UnfoldingDimensions unfoldDim) {
	int crossingNumber;
	if(unfoldDim.a != 0) crossingNumber = unfoldDim.a + unfoldDim.b - 1;
	else crossingNumber = unfoldDim.a + unfoldDim.b;
	return crossingNumber; 
}

/**
 * @brief 負の傾きを持つ線分が正方格子を横断・縦断する際に
 * 左上 (LT) から右下 (RB) へと通過する順序を取得するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @return 横断・縦断する際に通過する方向を表す char 型の文字が格納された配列
 * @details 左から右に横断する時に通過する辺は (unfoldDim.b - 1) 本
 * 上から下に横断する時に通過する辺は (unfoldDim.a - 1) 本となる
 * なお，各 char 型の文字は，r と b であり，それぞれの意味は下記の通りである
 * r: 左から右に格子の辺を横断する (left → right)
 * b: 上から下に格子の辺を縦断する (top → bottom)
 */
vector<char> IDManager::getLineSegment_LT_to_RD(UnfoldingDimensions unfoldDim) {
	//! 横断していく方向を管理する変数
	vector<char> direction; 
	//! 縦断する場所を管理する変数（true: 縦断する，false: 縦断しない）
	vector<bool> cross(unfoldDim.b - 1, false);
	/*************************************************
	 * 縦断する場所を計算する
	 * y = -(a/b)x + a と y = k (1 <= k <= (a-1)) の
	 * 交点で縦断は生じるため，x について解くと
	 * x = -(b * k - a * b)/a となる
	 * x は整数なので，天井関数で切り上げする
	 *************************************************/
	for (int k = 1; k <= unfoldDim.a - 1; ++k) {
		double x = -(unfoldDim.b * k - unfoldDim.a * unfoldDim.b) / (double) unfoldDim.a;
		cross[ceil(x)-1] = true;	// 0 インデックスの配列なので -1 が必要 
	}
	/*************************************************
	 * 横断・縦断していく方向を direction に格納する
	 *************************************************/
	for (int i = 0; i < unfoldDim.b - 1; ++i) {
		if(cross[i]) direction.push_back('b');
		direction.push_back('r');
	}
	return direction;
}

/**
 * @brief 正の傾きを持つ線分が正方格子を横断・縦断する際に
 * 左下 (LB) から右上 (RT) へと通過する順序を取得するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @return 横断・縦断する際に通過する方向を表す char 型の文字が格納された配列
 * @details 左から右に横断する時に通過する辺は (unfoldDim.b - 1) 本
 * 下から上に横断する時に通過する辺は (unfoldDim.a - 1) 本となる
 * なお，各 char 型の文字は，r と t であり，それぞれの意味は下記の通りである
 * r: 左から右に格子の辺を横断する (left → right)
 * t: 下から上に格子の辺を縦断する (bottom → top)
 */
vector<char> IDManager::getLineSegment_LD_to_RT(UnfoldingDimensions unfoldDim) {
	//! 横断していく方向を管理する変数
	vector<char> direction; 
	//! 縦断する場所を管理する変数（true: 縦断する，false: 縦断しない）
	vector<bool> cross(unfoldDim.b - 1, false);
	/*************************************************
	 * 縦断する場所を計算する
	 * y = -(a/b)x + a と y = k (1 <= k <= (a-1)) の
	 * 交点で縦断は生じるため，x について解くと
	 * x = -(b * k - a * b)/a となる
	 * x は整数なので，天井関数で切り上げする
	 *************************************************/
	for (int k = 1; k <= unfoldDim.a - 1; ++k) {
		double x = (unfoldDim.b * k) / (double) unfoldDim.a;
		cross[ceil(x)-1] = true; // 配列なので，x の値が1ズレる
	}
	/*************************************************
	 * 横断・縦断していく方向を direction に格納する
	 *************************************************/
	for (int i = 0; i < unfoldDim.b - 1; ++i) {
		if(cross[i]) direction.push_back('r');
		direction.push_back('t');
	}
	return direction;
}

/**
 * @brief 入力として与えられた横断・縦断する際に通過する方向を表す配列を逆順にするメンバ関数
 * @param direction 横断・縦断する際に通過する方向を表す char 型の文字が格納された配列
 * @return 通過する方向を表す配列を逆順にした配列
 * @details 各 char 型の文字は，r, l, t, b であり，それぞれの意味は下記の通りである
 * r: 左から右に格子の辺を横断する (left → right)
 * l: 右から左に格子の辺を横断する (right → left)
 * t: 下から上に格子の辺を縦断する (bottom → top)
 * b: 上から下に格子の辺を縦断する (top → bottom)
 */
vector<char> IDManager::getLineSegment_Reverse(vector<char> direction) {
	/*************************************************
	 * r ならば l，l ならば r，t ならば b，b ならば t と
	 * 文字を変換する
	 *************************************************/
	for(int i = 0; i < direction.size(); ++i) {
		if(direction[i] == 'r') direction[i] = 'l';
		else if(direction[i] == 'l') direction[i] = 'r';
		else if(direction[i] == 't') direction[i] = 'b';
		else direction[i] = 't';
	}
	/*************************************************
	 * 配列を逆順にする
	 *************************************************/	
	reverse(direction.begin(), direction.end());
	return direction;
}
