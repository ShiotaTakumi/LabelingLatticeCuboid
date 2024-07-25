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
 * @brief 線分が交差している面のうち，
 * 貼り合わされる側の面を更新する際に必要となる情報を管理する構造体
*/
struct FillingBorderFaces {
	//! 面と面の対応付けをするために管理するための番号
	int id;
	//! 通過する順序を管理するベクトル
	vector<char> direction;
	//! 左上もしくは左下の x 座標
	int x;
	//! 左上もしくは左下の y 座標
	int y;
	//! direction を繰り返す回数
	int roop;
	//! a が 0 かどうかを管理するフラグ
	bool nonZero;
	//! ベクトルが正の傾きか負の傾きかを表す bool 値
	bool slope;
};
