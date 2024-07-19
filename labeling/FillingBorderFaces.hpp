/**
 * @file FillingBorderFaces.hpp
 * @brief FillingBorderFaces 構造体
 * @author Takumi SHIOTA
 * @date 2024/07/14 最終更新
 * @version 1.0
 */

#pragma once

#include<vector>
using namespace std;

/**
 * @struct FillingBorderFaces
 * @brief 線分が交差している面のうち内，
 * 貼り合わされる側の面を更新する際に必要となる情報を管理する構造体
 * @note 要：コメント修正
*/
struct FillingBorderFaces {
	//! 上書きする状態の管理用番号
	int id;
	//! 線分が通過していく面の順番を保持する配列
	vector<char> direction;
	//! 現在の x 座標の位置
	int x;
	//! 現在の y 座標の位置
	int y;
	//! direction をループする回数
	int roop;
	//! a が 0 かどうかを管理するフラグ
	bool nonZero;
	//! 線分の傾きが正(true)か負(false)か管理するフラグ
	bool slope;
};
