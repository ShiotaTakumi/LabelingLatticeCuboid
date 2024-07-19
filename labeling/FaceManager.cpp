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

/*************************************************
 * borderFace.direction の方向に応じて
 * 境界の面の状態を borderFace.id に書き換えるメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus(FillingBorderFaces borderFace) {
	idManager.faces_status[borderFace.y][borderFace.x] = borderFace.id;
	for(int i = 0; i < borderFace.roop; ++i) {
		for(int j = 0; j < borderFace.direction.size(); ++j) {
			if(borderFace.direction[j] == 'r') {
				borderFace.x++; // x 軸 右方向
			} else if(borderFace.direction[j] == 'b') {
				borderFace.y++; // y 軸 下方向
			} else if(borderFace.direction[j] == 'l') {
				borderFace.x--; // x 軸 左方向
			} else if(borderFace.direction[j] == 't') {
				borderFace.y--; // y 軸 上方向			
			}
			idManager.setFacesStatus(borderFace.y, borderFace.x, borderFace.id);
		}
		if(i != borderFace.roop - 1) {
			if(!borderFace.slope) {
				borderFace.x++; 
				if(borderFace.nonZero) { borderFace.y++; }
			} else {
				borderFace.y--;
				if(borderFace.nonZero) { borderFace.x++; }
			}
			idManager.setFacesStatus(borderFace.y, borderFace.x, borderFace.id);
		}
	}
}

/*************************************************
 * 0 1 2 面の上部の境界（負の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_012_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 1;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left 
				 + unfoldDim.a * unfoldDim.y;
	borderFace.y = unfoldDim.margin_top 
				 + unfoldDim.top_shift;
	borderFace.roop = unfoldDim.x + unfoldDim.z + unfoldDim.x;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}


/*************************************************
 * 0 面の左部の境界（正の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_0_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 2;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left;
	borderFace.y = unfoldDim.margin_top 
				 + unfoldDim.top_shift 
				 + unfoldDim.b * unfoldDim.y
				 - 1;
	borderFace.roop = unfoldDim.y;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/*************************************************
 * 0 1 2 面の下部の境界（負の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_012_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 3;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left;
	borderFace.y = unfoldDim.margin_top
				 + unfoldDim.top_shift 
				 + unfoldDim.b * unfoldDim.y;
	if(unfoldDim.a == 0) borderFace.y -= 1;	
	borderFace.roop = unfoldDim.x + unfoldDim.z + unfoldDim.x;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/*************************************************
 * 4 面の左部の境界（正の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_4_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 4;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left
				 + unfoldDim.a * unfoldDim.y
				 + unfoldDim.b * unfoldDim.x
				 + unfoldDim.b * unfoldDim.z
				 + unfoldDim.b * unfoldDim.x;
	borderFace.y = unfoldDim.margin_top
				 + unfoldDim.top_shift 
				 + unfoldDim.a * unfoldDim.x 
				 + unfoldDim.a * unfoldDim.z
				 + unfoldDim.a * unfoldDim.x 
				 - 1;
	borderFace.roop = unfoldDim.x;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/*************************************************
 * 5 面の左部の境界（正の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_5_left(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 5;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left
				 + unfoldDim.b * unfoldDim.x 
				 + unfoldDim.b * unfoldDim.z
				 + unfoldDim.b * unfoldDim.x
				 - unfoldDim.a * unfoldDim.x;
	borderFace.y = unfoldDim.margin_top
				 + unfoldDim.top_shift 
				 + unfoldDim.a * unfoldDim.x 
				 + unfoldDim.a * unfoldDim.z
				 + unfoldDim.a * unfoldDim.x
				 + unfoldDim.b * unfoldDim.y 
				 + unfoldDim.b * unfoldDim.x 
				 - 1;
	borderFace.roop = unfoldDim.x;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/*************************************************
 * 4 面の上部の境界（負の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_4_top(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 6;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left 
				 + unfoldDim.a * unfoldDim.y
				 + unfoldDim.b * unfoldDim.x
				 + unfoldDim.b * unfoldDim.z
				 + unfoldDim.b * unfoldDim.x
				 + unfoldDim.a * unfoldDim.x;
	borderFace.y = unfoldDim.margin_top 
				 + unfoldDim.top_shift
				 + unfoldDim.a * unfoldDim.x
				 + unfoldDim.a * unfoldDim.z
				 + unfoldDim.a * unfoldDim.x
				 - unfoldDim.b * unfoldDim.x;
	borderFace.roop = unfoldDim.z;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/*************************************************
 * 5 面の下部の境界（負の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_5_bottom(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 7;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left 
				 + unfoldDim.b * unfoldDim.x
				 + unfoldDim.b * unfoldDim.z
				 + unfoldDim.b * unfoldDim.x
				 - unfoldDim.a * unfoldDim.x;
	borderFace.y = unfoldDim.margin_top 
				 + unfoldDim.top_shift
				 + unfoldDim.a * unfoldDim.x
				 + unfoldDim.a * unfoldDim.z
				 + unfoldDim.a * unfoldDim.x
				 + unfoldDim.b * unfoldDim.y
				 + unfoldDim.b * unfoldDim.x;
	if(unfoldDim.a == 0) borderFace.y -= 1; 
	borderFace.roop = unfoldDim.z;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/*************************************************
 * 435 面の右部の境界（正の傾きの線分）における
 * 面の更新をするメンバ関数
 *************************************************/
void FaceManager::updateBorderStatus_435_right(UnfoldingDimensions unfoldDim, vector<char> direction, bool slope) {
	FillingBorderFaces borderFace;
	borderFace.id = 8;
	borderFace.direction = direction;
	borderFace.x = unfoldDim.margin_left
				 + unfoldDim.b * unfoldDim.x
				 + unfoldDim.b * unfoldDim.z
				 + unfoldDim.b * unfoldDim.x 
				 + unfoldDim.b * unfoldDim.z 
				 - unfoldDim.a * unfoldDim.x;
	borderFace.y = unfoldDim.margin_top
				 + unfoldDim.top_shift 
				 + unfoldDim.a * unfoldDim.x 
				 + unfoldDim.a * unfoldDim.z 
				 + unfoldDim.a * unfoldDim.x 
				 + unfoldDim.b * unfoldDim.y 
				 + unfoldDim.b * unfoldDim.x 
				 + unfoldDim.a * unfoldDim.z 
				 - 1;
	if(unfoldDim.a == 0) borderFace.x -= 1; 
	borderFace.roop = unfoldDim.x + unfoldDim.y + unfoldDim.x;
	borderFace.nonZero = (unfoldDim.a != 0) ? true : false;
	borderFace.slope = (slope) ? true : false;
	updateBorderStatus(borderFace);
}

/**
 * @brief 展開図を格子グリッド上に配置したときに線分上に位置する面を把握するメンバ関数
 * @param unfoldDim 盤面のサイズおよび余白
 */
/*************************************************
 * 展開図を格子グリッド上に配置したときに
 * 境界線に位置する面がどの位置にあるかを管理するメンバ関数
 * LT_to_RD は，負の傾きの線分なので false
 * LD_to_RT は，正の傾きの線分なので true
 *************************************************/
void FaceManager::getBorderFaces(UnfoldingDimensions unfoldDim) {
	//! 左上 (LT) から右下 (RB) へと通過する順序を管理する変数
	vector<char> LT_to_RD = idManager.getLineSegment_LT_to_RD(unfoldDim);
	//! 左下 (LD) から右上 (RT) へと通過する順序を管理する変数
	vector<char> LD_to_RT = idManager.getLineSegment_LD_to_RT(unfoldDim);
	/*************************************************
	 * 
	 * なお，線分の傾きに
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

/*************************************************
 * 格子展開図の境界の内部にある面を把握するメンバ関数
 * 境界の内側にある面を 9 で埋める
 * 
 * a != 0 の場合の内部かどうかの判定方法は下記の通り：
 * 右隣の面の状態の ID を見たとき，num の値が
 * - flg = true かつ 0 -> xn に変わる：
 * 					内部に入った（flg を true に）
 * - flg = true かつ 0 -> xm に変わる：
 * 					内部から出た（flg を false に）
 * -  xn -> xn のまま：辺の上を横断中
 * 
 * a == 0 の場合の内部かどうかの判定方法は下記の通り：
 * 右隣の面の状態の ID を見たとき，num の値が
 * - flg = false かつ 0 以外のとき：
 * 					値を 9 にする（flg を true に）
 * - flg = true かつ 0 のとき：値を 9 にする
 * - flg = true かつ 0 以外のとき：
 * 					値を 9 にする（flg を false に）
 *************************************************/
void FaceManager::getInnnerFaces(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < idManager.getFacesIDsize().row; ++i) {
		bool flg = false;
		int num = 0;
		for(int j = 0; j < idManager.getFacesIDsize().column; ++j) {
			if(unfoldDim.a != 0) {
				if(idManager.getFacesStatus(i, j) != 0) {
					if(idManager.getFacesStatus(i, j) == num) continue;
					if(idManager.getFacesStatus(i, j) != num) flg = !flg;
					num = idManager.getFacesStatus(i, j);
				} else {
					if(flg) idManager.setFacesStatus(i, j, 9);
				}
			} else {
				if(j == idManager.getFacesIDsize().column - 1) continue;
				if(idManager.getFacesStatus(i, j) != 0) {
					idManager.setFacesStatus(i, j, 9);
					if(idManager.getFacesStatus(i, j+1) != 0) continue;
					flg = !flg;
				} else {
					if(flg) idManager.setFacesStatus(i, j, 9);
				}
			}
		}
	}
}
