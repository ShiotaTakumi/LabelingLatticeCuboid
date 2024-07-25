/**
 * @file FaceManager.cpp
 * @brief FaceManager クラスの実装
 * @date 2024/07/26 最終更新
 * @version 1.0
 * @author Takumi SHIOTA
 */

#include "FaceManager.hpp"
#include <iostream>
#include <math.h>
#include "FillingBorderFaces.hpp"
#include "Labeling.hpp"
#include "IDManager.hpp"
#include "ArraySize.hpp"

using namespace std;

/**
 * @brief FaceManager クラスのコンストラクタ
 * @param idManager 面の ID および状態を管理するための IDManager オブジェクト
 */
FaceManager::FaceManager(IDManager& idManager) : idManager(idManager) {}

/**
 * @brief 通過する順序を管理するベクトルを元に，面の状態を更新するメンバ関数
 * @param borderFace 面の状態を更新する際に必要な情報
 */
void FaceManager::updateBorderStatus(FillingBorderFaces borderFace) {
	//! (x, y) 座標の初期値の面を id に更新
	idManager.setFacesStatus(borderFace.y, borderFace.x, borderFace.id);
	for(int i = 0; i < borderFace.roop; ++i) {
		for(int j = 0; j < borderFace.direction.size(); ++j) {
			//! Direction の文字に基づき，注目している (x, y) 座標を移動させる
			if(borderFace.direction[j] == 'r') {
				borderFace.x++; // x 軸 右方向
			} else if(borderFace.direction[j] == 'b') {
				borderFace.y++; // y 軸 下方向
			} else if(borderFace.direction[j] == 'l') {
				borderFace.x--; // x 軸 左方向
			} else if(borderFace.direction[j] == 't') {
				borderFace.y--; // y 軸 上方向			
			}
			//! (x, y) 座標における面の状態を更新する
			idManager.setFacesStatus(borderFace.y, borderFace.x, borderFace.id);
		}
		//! 最後のループ以外の時は，注目している (x, y) 座標を，右上，右下，左上，左下のいずれかに移動させる
		if(i != borderFace.roop - 1) {
			//! LT_to_RD（負の傾き）/ LD_to_RT（正の傾き）で場合分け
			if(!borderFace.slope) {
				borderFace.x++; 
				if(borderFace.nonZero) { borderFace.y++; }
			} else {
				borderFace.y--;
				if(borderFace.nonZero) { borderFace.x++; }
			}
			//! (x, y) 座標における面の状態を更新する
			idManager.setFacesStatus(borderFace.y, borderFace.x, borderFace.id);
		}
	}
}

/**
 * @brief 面 0 1 2 の上を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_012_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 1;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left		// 左の余白
				 + unfoldDim.a * unfoldDim.y;	// 面 0 の左を通る線分の幅
	borderFace.y = unfoldDim.margin_top 		// 上の余白
				 + unfoldDim.top_shift;			// 上の余白が足りない際にシフトする値
	borderFace.roop = unfoldDim.x 				// 面 0 の横幅
					+ unfoldDim.z 				// 面 1 の横幅
					+ unfoldDim.x;				// 面 2 の横幅
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 面 0 の左を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_0_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 2;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left;		// 左の余白
	borderFace.y = unfoldDim.margin_top 		// 上の余白 
				 + unfoldDim.top_shift 			// 上の余白が足りない際にシフトする値
				 + unfoldDim.b * unfoldDim.y	// 面 0 の左を通る線分の高さ
				 - 1;							// 調整値
	borderFace.roop = unfoldDim.y;				// 面 0 の高さ
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 面 0 1 2 の下を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_012_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 3;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left;		// 左の余白
	borderFace.y = unfoldDim.margin_top			// 上の余白
				 + unfoldDim.top_shift 			// 上の余白が足りない際にシフトする値
				 + unfoldDim.b * unfoldDim.y;	// 面 0 の左を通る線分の高さ
	if(unfoldDim.a == 0) borderFace.y -= 1;		// a = 0 の時の調整
	borderFace.roop = unfoldDim.x 				// 面 0 の横幅
					+ unfoldDim.z 				// 面 1 の横幅
					+ unfoldDim.x;				// 面 2 の横幅
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 面 4 の左を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_4_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 4;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left		// 左の余白
				 + unfoldDim.a * unfoldDim.y	// 面 0 の左を通る線分の幅
				 + unfoldDim.b * unfoldDim.x	// 面 0 の上を通る線分の幅
				 + unfoldDim.b * unfoldDim.z	// 面 1 の上を通る線分の幅
				 + unfoldDim.b * unfoldDim.x;	// 面 2 の上を通る線分の幅
	borderFace.y = unfoldDim.margin_top			// 上の余白
				 + unfoldDim.top_shift 			// 上の余白が足りない際にシフトする値
				 + unfoldDim.a * unfoldDim.x 	// 面 0 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.z	// 面 1 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.x 	// 面 2 の上を通る線分の高さ
				 - 1;							// 調整値
	borderFace.roop = unfoldDim.x;				// 面 4 高さ
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 面 5 の左を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_5_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 5;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left		// 左の余白
				 + unfoldDim.b * unfoldDim.x 	// 面 0 の下を通る線分の幅
				 + unfoldDim.b * unfoldDim.z 	// 面 1 の下を通る線分の幅
				 + unfoldDim.b * unfoldDim.x 	// 面 2 の下を通る線分の幅
				 - unfoldDim.a * unfoldDim.x;	// 面 5 の左を通る線分の幅
	borderFace.y = unfoldDim.margin_top			// 上の余白
				 + unfoldDim.top_shift 			// 上の余白が足りない際にシフトする値
				 + unfoldDim.a * unfoldDim.x 	// 面 0 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.z	// 面 1 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.x	// 面 2 の上を通る線分の高さ
				 + unfoldDim.b * unfoldDim.y 	// 面 3 の左を通る線分の高さ
				 + unfoldDim.b * unfoldDim.x 	// 面 5 の左を通る線分の高さ
				 - 1;							// 調整値
	borderFace.roop = unfoldDim.x;				// 面 5 の高さ
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 面 4 の上を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_4_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 6;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left 		// 左の余白
				 + unfoldDim.a * unfoldDim.y	// 面 0 の左を通る線分の幅
				 + unfoldDim.b * unfoldDim.x	// 面 0 の上を通る線分の幅
				 + unfoldDim.b * unfoldDim.z	// 面 1 の上を通る線分の幅
				 + unfoldDim.b * unfoldDim.x	// 面 2 の上を通る線分の幅
				 + unfoldDim.a * unfoldDim.x;	// 面 4 の左を通る線分の幅
	borderFace.y = unfoldDim.margin_top 		// 上の余白
				 + unfoldDim.top_shift			// 上の余白が足りない際にシフトする値
				 + unfoldDim.a * unfoldDim.x	// 面 0 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.z	// 面 1 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.x	// 面 2 の上を通る線分の高さ
				 - unfoldDim.b * unfoldDim.x;	// 面 4 の左を通る線分の高さ
	borderFace.roop = unfoldDim.z;				// 面 4 の横幅
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 面 5 の下を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_5_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 7;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left 		// 左の余白
				 + unfoldDim.b * unfoldDim.x	// 面 0 の下を通る線分の幅
				 + unfoldDim.b * unfoldDim.z	// 面 1 の下を通る線分の幅
				 + unfoldDim.b * unfoldDim.x	// 面 2 の下を通る線分の幅
				 - unfoldDim.a * unfoldDim.x;	// 面 5 の左を通る線分の幅
	borderFace.y = unfoldDim.margin_top 		// 上の余白
				 + unfoldDim.top_shift			// 上の余白が足りない際にシフトする値
				 + unfoldDim.a * unfoldDim.x	// 面 0 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.z	// 面 1 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.x	// 面 2 の上を通る線分の高さ
				 + unfoldDim.b * unfoldDim.y	// 面 3 の左を通る線分の高さ
				 + unfoldDim.b * unfoldDim.x;	// 面 5 の左を通る線分の高さ
	if(unfoldDim.a == 0) borderFace.y -= 1; 	// a = 0 の時の調整
	borderFace.roop = unfoldDim.z;				// 面 5 の横幅
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 面 4 3 5 の右を通る線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 * @param direction 通過する順序を管理するベクトル
 * @param slope ベクトルが正の傾きか負の傾きかを表す bool 値
 */
void FaceManager::updateBorderStatus_435_right(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	//! 境界に位置する面を管理する構造体のインスタンス
	FillingBorderFaces borderFace;
	borderFace.id = 8;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left		// 左の余白
				 + unfoldDim.b * unfoldDim.x	// 面 0 の下を通る線分の幅
				 + unfoldDim.b * unfoldDim.z	// 面 1 の下を通る線分の幅
				 + unfoldDim.b * unfoldDim.x 	// 面 2 の下を通る線分の幅
				 + unfoldDim.b * unfoldDim.z 	// 面 3 の下を通る線分の幅
				 - unfoldDim.a * unfoldDim.x;	// 面 5 の右を通る線分の幅
	borderFace.y = unfoldDim.margin_top			// 上の余白
				 + unfoldDim.top_shift 			// 上の余白が足りない際にシフトする値
				 + unfoldDim.a * unfoldDim.x 	// 面 0 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.z 	// 面 1 の上を通る線分の高さ
				 + unfoldDim.a * unfoldDim.x 	// 面 2 の上を通る線分の高さ
				 + unfoldDim.b * unfoldDim.y 	// 面 3 の左を通る線分の高さ
				 + unfoldDim.b * unfoldDim.x 	// 面 5 の左を通る線分の高さ
				 + unfoldDim.a * unfoldDim.z 	// 面 5 の下を通る線分の高さ
				 - 1;							// 調整値
	if(unfoldDim.a == 0) borderFace.x -= 1; 	// a = 0 の時の調整
	borderFace.roop = unfoldDim.x 				// 面 5 の高さ
					+ unfoldDim.y 				// 面 3 の高さ
					+ unfoldDim.x;				// 面 4 の高さ
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 展開図を格子グリッド上に配置したときに線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 */
void FaceManager::getBorderFaces(UnfoldingDimensions unfoldDim) {
	//! 左上 (LT) から右下 (RB) へと通過する順序を管理するベクトル
	vector<char> LT_to_RD = idManager.getLineSegment_LT_to_RD(unfoldDim);
	//! 左下 (LD) から右上 (RT) へと通過する順序を管理するベクトル
	vector<char> LD_to_RT = idManager.getLineSegment_LD_to_RT(unfoldDim);
	/*************************************************
	 * 格子展開図の周囲を囲む線分は下記の 8 種類に分類できる．
	 * 
	 * (1) 面 0 1 2 の上を通る線分
	 * (2) 面 0 の左を通る線分
	 * (3) 面 0 1 2 の下を通る線分
	 * (4) 面 4 の左を通る線分
	 * (5) 面 5 の左を通る線分
	 * (6) 面 4 の上を通る線分
	 * (7) 面 5 の下を通る線分
	 * (8) 面 4 3 5 の右を通る線分
	 * 
	 * 各線分に対して，位置する面を求める
	 * 
	 * 線分の傾きが必要となるため，bool 値を引数として与える
	 * - LT_to_RD の時：負の傾きの線分なので false
	 * - LD_to_RT の時：正の傾きの線分なので true
	 *************************************************/
	updateBorderStatus_012_top(unfoldDim, LT_to_RD, false);
	updateBorderStatus_0_left(unfoldDim, LD_to_RT, true);
	updateBorderStatus_012_bottom(unfoldDim, LT_to_RD, false);
	updateBorderStatus_4_left(unfoldDim, LD_to_RT, true);
	updateBorderStatus_5_left(unfoldDim, LD_to_RT, true);
	updateBorderStatus_4_top(unfoldDim, LT_to_RD, false);
	updateBorderStatus_5_bottom(unfoldDim, LT_to_RD, false);
	updateBorderStatus_435_right(unfoldDim, LD_to_RT, true);
}

/**
 * @brief 格子展開図に内包されている面の状態を 9 に更新するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 */
void FaceManager::getInnnerFaces(UnfoldingDimensions unfoldDim) {
	//! 面の状態を表す配列の行数
	for(int i = 0; i < idManager.getFacesIDsize().row; ++i) {
		//! 面が内包されているか／内包されていないかを管理するフラグ
		bool flg = false;
		//! 状態を記憶するための変数
		int num = 0;
		//! 面の状態を表す配列の列数
		for(int j = 0; j < idManager.getFacesIDsize().column; ++j) {
			//! a の値で場合分け
			if(unfoldDim.a != 0) {
				//! 面の状態が 0 か否かで場合分け
				if(idManager.getFacesStatus(i, j) != 0) {
					//! 境界線上を横断している場合はスキップ
					if(idManager.getFacesStatus(i, j) == num) continue;
					//! 内包されている／されていないが切り替わる場合は flg を反転
					if(idManager.getFacesStatus(i, j) != num) flg = !flg;
					//! 一つ右の座標の状態を見るために，num の値を更新
					num = idManager.getFacesStatus(i, j);
				} else {
					//! 状態が 0 かつ内包の状態であるならば，状態を 9 に更新
					if(flg) idManager.setFacesStatus(i, j, 9);
				}
			} else {
				//! ループの最後はスキップ（j+1 で配列外参照を起こすため）
				if(j == idManager.getFacesIDsize().column - 1) continue;
				//! 面の状態が 0 か否かで場合分け
				if(idManager.getFacesStatus(i, j) != 0) {
					//! a = 0 の場合は，交差している面の状態の区別は不要なため，上書き
					idManager.setFacesStatus(i, j, 9);
					//! 一つ右の座標の状態が 0 で無いならば，flg の反転は不要なためスキップ
					if(idManager.getFacesStatus(i, j+1) != 0) continue;
					//! 内包されている／されていないを切り替えるため flg を反転
					flg = !flg;
				} else {
					//! 状態が 0 かつ内包の状態であるならば，状態を 9 に更新
					if(flg) idManager.setFacesStatus(i, j, 9);
				}
			}
		}
	}
}
