/**
 * @file FaceManager.hpp
 * @brief FaceManager クラス
 * @details 面を表す ID および状態の配列を更新するクラス
 * @date 2024/07/19 最終更新
 * @version 1.0
 * @author Takumi SHIOTA
 */

#pragma once
#include "IDManager.hpp"
#include "UnfoldingDimensions.hpp"
#include "Correspondence.hpp"
#include <vector>
#include <utility>
#include "FillingBorderFaces.hpp"
#include "ArraySize.hpp"

using namespace std;

/**
 * @class FaceManager
 * @brief 面を表す ID および状態の配列を更新するクラス
 */
class FaceManager {
private:
	IDManager& idManager;
	void updateBorderStatus(FillingBorderFaces borderFace);
	void updateBorderStatus_012_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	void updateBorderStatus_0_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	void updateBorderStatus_012_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	void updateBorderStatus_4_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	void updateBorderStatus_5_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	void updateBorderStatus_4_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	void updateBorderStatus_5_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	void updateBorderStatus_435_right(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);

public:
	/**
	 * @brief FaceManager クラスのコンストラクタ
	 * @param idManager 面の ID および状態を管理するための IDManager オブジェクト
	 */
	FaceManager(IDManager& idManager);
	/**
	 * @brief 展開図を格子グリッド上に配置したときに線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void getBorderFaces(UnfoldingDimensions unfoldDim);
	void getInnnerFaces(UnfoldingDimensions unfoldDim);
};