/**
 * @file Labeling.hpp
 * @brief Labeling クラス
 * @details 格子立方体または格子直方体の格子展開図を生成し，
 * 各面・辺・頂点のIDや状態を管理するクラス
 * @date 2024/06/25 最終更新
 * @version 1.0
 * @author Takumi SHIOTA
 */

#pragma once

#include "UnfoldingDimensions.hpp"
#include "Correspondence.hpp"
#include "AdjacentList.hpp"
#include "IDManager.hpp"
#include "FaceManager.hpp"
#include "EdgeManager.hpp"
#include "VertexManager.hpp"
#include "UpdateID.hpp"
#include <vector>

/**
 * @class Labeling
 * @brief 展開図の面，辺，頂点の ID および状態を管理するクラス
 */
class Labeling {
private:
	//! 面，辺，頂点の ID を管理するためのオブジェクト
	IDManager idManager;
	//! 面の状態を管理するためのオブジェクト
	FaceManager faceManager;
	//! 辺の状態を管理するためのオブジェクト
	EdgeManager edgeManager;
	//! 頂点の状態を管理するためのオブジェクト
	VertexManager vertexManager;
	//! 面，辺，頂点の ID の更新を管理するためのオブジェクト
	UpdateID updatID;

public:
	/**
	 * @brief コンストラクタ
	 */
	Labeling();
	/**
	 * @brief 初期化メソッド
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void initialize(UnfoldingDimensions unfoldDim);
	/**
	 * @brief 面・辺・頂点の ID の配列を全て出力するメンバ関数
	 */
	void printAllIDs();
	/**
	 * @brief 面・辺・頂点の状態の配列を全て出力するメンバ関数
	 */
	void printAllStatuses();
	/**
	 * @brief 状態の値が 0 ではない ID の配列を出力するメンバ関数
	 */
	void printNonZeroStatuses();

	/*************************************************
	 * 面 → 頂点 → 辺 の順番で状態を取得していく必要がある
	 * （頂点は面，辺は頂点の状態を使用する）
	 *************************************************/
	/**
	 * @brief 面の状態を取得するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void getFaces(UnfoldingDimensions unfoldDim);
	/**
	 * @brief 頂点の状態を取得するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void getVertices(UnfoldingDimensions unfoldDim);
	/**
	 * @brief 辺の状態を取得するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void getEdges(UnfoldingDimensions unfoldDim);

	/**
	 * @brief 隣接リストを取得・出力するメンバ関数
	 * @param adjLists 隣接リストを管理するベクトル
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void generateAdjacentList(vector<AdjacentList>& adjLists, UnfoldingDimensions unfoldDim);
};
