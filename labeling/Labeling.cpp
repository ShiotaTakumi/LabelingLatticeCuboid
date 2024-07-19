/**
 * @file Labeling.cpp
 * @brief Labeling クラスの実装
 * @date 2024/06/25 最終更新
 * @version 1.0
 * @author Takumi SHIOTA
 */

#include "Labeling.hpp"

/**
 * @brief コンストラクタ
 * @details 管理しているオブジェクトの初期化をする
 */
Labeling::Labeling()
	: faceManager(idManager),
	  edgeManager(idManager),
	  vertexManager(idManager),
	  updatID(idManager) {}


/**
 * @brief 初期化メソッド
 * @param unfoldDim 盤面のサイズおよび余白
 * @details 盤面の寸法と余白に基づき，ID および状態の配列の
 * サイズの指定および初期化を行う
 */
void Labeling::initialize(UnfoldingDimensions unfoldDim) {
	idManager.initializeVecs(unfoldDim);
}

/**
 * @brief 面・辺・頂点の ID の配列を全て出力するメンバ関数
 */
void Labeling::printAllIDs() {
	idManager.printVec(idManager.faces_id, "Face ID", true);
	idManager.printVec(idManager.edges_id, "Edge ID", true);
	idManager.printVec(idManager.vertices_id, "Vertex ID", true);
}

/**
 * @brief 面・辺・頂点の状態の配列を全て出力するメンバ関数
 */
void Labeling::printAllStatuses() {
	idManager.printVec(idManager.faces_status, "Face ID", false);
	idManager.printVec(idManager.edges_status, "Edge ID", false);
	idManager.printVec(idManager.vertices_status, "Vertex ID", false);
}

/**
 * @brief 状態の値が 0 ではない ID の配列を出力するメンバ関数
 */
void Labeling::printNonZeroStatuses() {
	idManager.printNonZeroVec(idManager.faces_id, idManager.faces_status, "Face ID");
	idManager.printNonZeroVec(idManager.edges_id, idManager.edges_status, "Edge ID");
	idManager.printNonZeroVec(idManager.vertices_id, idManager.vertices_status, "Vertex ID");
}

/**
 * @brief 面の状態を取得するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @details 展開図を格子グリッド上に配置したときに
 * 面がどの位置にあるかを管理するメンバ関数
 */
void Labeling::getFaces(UnfoldingDimensions unfoldDim) {
	faceManager.getBorderFaces(unfoldDim);
	faceManager.getInnnerFaces(unfoldDim);
}

/**
 * @brief 頂点の状態を取得するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @details 展開図を格子グリッド上に配置したときに
 * 頂点がどの位置にあるかを管理するメンバ関数
 */
void Labeling::getVertices(UnfoldingDimensions unfoldDim) {
	vertexManager.getInnnerVertices();
	vertexManager.getConerVertices(unfoldDim);
}

/**
 * @brief 辺の状態を取得するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @details 展開図を格子グリッド上に配置したときに
 * 辺がどの位置にあるかを管理するメンバ関数
 */
void Labeling::getEdges(UnfoldingDimensions unfoldDim) {
	edgeManager.getHorizonEdges(unfoldDim);
	edgeManager.getVerticalEdges(unfoldDim);
}

/**
 * @brief 隣接リストを取得・出力するメンバ関数
 * @param adjLists 隣接リストを管理するベクトル
 * @param unfoldDim 盤面のサイズおよび余白
 */
void Labeling::generateAdjacentList(vector<AdjacentList>& adjLists, UnfoldingDimensions unfoldDim) {
	updatID.getAdjacentList(adjLists, unfoldDim);
	// updatID.printOriginalAdjacentList(adjLists);
	updatID.printAdjacentList(adjLists);
}
