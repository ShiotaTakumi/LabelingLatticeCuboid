/**
 * @file FaceManager.hpp
 * @brief FaceManager クラス
 * @details 面を表す ID および状態の配列を更新するクラス
 * @date 2024/07/26 最終更新
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
	//! 面の状態を管理するためのオブジェクト
	IDManager& idManager;
	/**
	 * @brief 通過する順序を管理するベクトルを元に，面の状態を更新するメンバ関数
	 * @param borderFace 面の状態を更新する際に必要な情報
	 */
	void updateBorderStatus(FillingBorderFaces borderFace);
	/**
	 * @brief 面 0 1 2 の上を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
	void updateBorderStatus_012_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	/**
	 * @brief 面 0 の左を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
	void updateBorderStatus_0_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	/**
	 * @brief 面 0 1 2 の下を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
	void updateBorderStatus_012_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	/**
	 * @brief 面 4 の左を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
	void updateBorderStatus_4_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	/**
	 * @brief 面 5 の左を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
	void updateBorderStatus_5_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	/**
	 * @brief 面 4 の上を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
	void updateBorderStatus_4_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	/**
	 * @brief 面 5 の下を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
	void updateBorderStatus_5_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope);
	/**
	 * @brief 面 4 3 5 の右を通る線分上に位置する面を把握するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 * @param direction 通過する順序を管理するベクトル
	 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
	 */
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
	/**
	 * @brief 格子展開図に内包されている面の状態を 9 に更新するメンバ関数
	 * @param unfoldDim 盤面のサイズおよび余白
	 */
	void getInnnerFaces(UnfoldingDimensions unfoldDim);
};