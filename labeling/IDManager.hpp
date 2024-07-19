/**
 * @file IDManager.hpp
 * @brief IDManager クラス
 * @details 面・辺・頂点を表す ID および状態の配列を管理するクラス
 * @date 2024/07/12 最終更新
 * @version 1.0
 * @author Takumi SHIOTA
 */

#pragma once
#include <vector>
#include <string>
#include "UnfoldingDimensions.hpp"
#include "ArraySize.hpp"
#include "Coordinates.hpp"

using namespace std;

/**
 * @class IDManager
 * @brief 面・辺・頂点を表す ID および状態の配列を管理するクラス
 */
class IDManager {
public:
	//! 面の ID を管理する配列 
	vector<vector<int>> faces_id;
	//! 辺の ID を管理する配列 
	vector<vector<int>> edges_id;
	//! 頂点の ID を管理する配列 
	vector<vector<int>> vertices_id;

	//! 面の状態を管理する配列 
	vector<vector<int>> faces_status;
	//! 辺の状態を管理する配列 
	vector<vector<int>> edges_status;
	//! 頂点の状態を管理する配列 
	vector<vector<int>> vertices_status;

	/*************************************************
	 * 各種配列の初期化および出力
	 *************************************************/
	/**
	 * @brief ID および状態を管理する配列の
	 * サイズの決定および全ての値を 0 に初期化するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void initializeVecs(UnfoldingDimensions unfoldDim);
	/**
	 * @brief 指定した ID を管理する配列の値を初期化するメンバ関数
	 * @param id 指定する ID の配列（参照型）
	 * @param c 配列の種類を管理するラベル
	 * （f: faces_id，e: edges_id，v: vertices_id）
	 */	
	void initializeIDArrays(std::vector<std::vector<int>>& id, char c) const;
	/**
	 * @brief 指定した配列を出力するメンバ関数
	 * @param array 指定する配列
	 * @param name 指定する配列の名前
	 * @param fillZero ゼロ埋めをするかを管理するラベル（True: ゼロ埋め，False: 何もしない）
	 */
	void printVec(const std::vector<std::vector<int>> array, const std::string name, bool fillZero);
	/**
	 * @brief 指定した配列のうち，状態が 0 でない値のみを出力するメンバ関数
	 * @param id 指定する ID の配列
	 * @param status 指定する状態の配列
	 * @param name 指定する配列の名前
	 */
	void printNonZeroVec(const std::vector<std::vector<int>> id, const vector<vector<int>> status, const std::string name);

	/*************************************************
	 * 面を表す配列に対する操作
	 *************************************************/
	/**
	 * @brief 面の ID 配列のサイズ取得するメンバ関数
	 * @return 面の ID の配列のサイズ（ArraySize 型）
	 */
	ArraySize getFacesIDsize();
	/**
	 * @brief 指定した座標における面の ID を取得するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @return 面の ID
	 */
	int getFacesID(int x, int y);
	/**
	 * @brief 指定した座標における面の状態を取得するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @return 面の状態
	 */
	int getFacesStatus(int x, int y);
	/**
	 * @brief 指定した面の ID から (x, y) 座標を取得するメンバ関数
	 * @param id 指定した面の ID
	 * @return (x, y) 座標（Coordinates 型）
	 */
	Coordinates getFacesCoordintes(int id);
	/**
	 * @brief 指定した座標における面の ID を更新するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @param id 更新後の面の ID
	 */
	void setFacesID(int x, int y, int id);
	/**
	 * @brief 指定した座標における面の状態を更新するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @param id 更新後の面の状態
	 */
	void setFacesStatus(int x, int y, int status);

	/*************************************************
	 * 頂点を表す配列に対する操作
	 *************************************************/
	/**
	 * @brief 頂点の ID 配列のサイズ取得するメンバ関数
	 * @return 頂点の ID の配列のサイズ（ArraySize 型）
	 */
	ArraySize getVerticesIDsize();
	/**
	 * @brief 指定した座標における頂点の ID を取得するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @return 頂点の ID
	 */
	int getVerticesID(int x, int y);
	/**
	 * @brief 指定した座標における頂点の状態を取得するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @return 頂点の状態
	 */
	int getVerticesStatus(int x, int y);
	/**
	 * @brief 指定した座標における頂点の ID を更新するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @param id 更新後の頂点の ID
	 */
	void setVerticesID(int x, int y, int id);
	/**
	 * @brief 指定した座標における頂点の状態を更新するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @param id 更新後の頂点の状態
	 */
	void setVerticesStatus(int x, int y, int status);


	/*************************************************
	 * 辺を表す配列に対する操作
	 *************************************************/
	/**
	 * @brief 指定した座標における辺の ID を取得するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @return 辺の ID
	 */
	int getEdgesID(int x, int y);
	/**
	 * @brief 指定した座標における辺の状態を取得するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @return 辺の状態
	 */
	int getEdgesStatus(int x, int y);
	/**
	 * @brief 指定した座標における辺の ID を更新するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @param id 更新後の辺の ID
	 */
	void setEdgesID(int x, int y, int id);
	/**
	 * @brief 指定した座標における辺の状態を更新するメンバ関数
	 * @param x 指定した x 座標
	 * @param y 指定した y 座標
	 * @param id 更新後の辺の状態
	 */
	void setEdgesStatus(int x, int y, int status);

	/*************************************************
	 * 格子グリッドと展開図における辺の交差の管理
	 *************************************************/
	/**
	 * @brief 一辺（a^2+b^2）の正方形と交わっている格子の個数を返すメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @return 交わっている格子の個数
	*/
	int getCrossingNumber(UnfoldingDimensions unfoldDim);
	/**
	 * @brief 負の傾きを持つ線分が正方格子を横断・縦断する際に
	 * 左上 (LT) から右下 (RB) へと通過する順序を取得するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @return 横断・縦断する際に通過する方向を表す char 型の文字が格納された配列
	 */
	vector<char> getLineSegment_LT_to_RD(UnfoldingDimensions unfoldDim);
	/**
	 * @brief 正の傾きを持つ線分が正方格子を横断・縦断する際に
	 * 左下 (LB) から右上 (RT) へと通過する順序を取得するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @return 横断・縦断する際に通過する方向を表す char 型の文字が格納された配列
	 */
	vector<char> getLineSegment_LD_to_RT(UnfoldingDimensions unfoldDim);
	/**
	 * @brief 入力として与えられた横断・縦断する際に通過する方向を表す配列を逆順にするメンバ関数
	 * @param direction 横断・縦断する際に通過する方向を表す char 型の文字が格納された配列
	 * @return 通過する方向を表す配列を逆順にした配列
	 */
	vector<char> getLineSegment_Reverse(vector<char> direction);
};