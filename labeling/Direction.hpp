/**
 * @file Direction.hpp
 * @brief Direction 構造体
 * @author Takumi SHIOTA
 * @date 2024/06/25 最終更新
 * @version 1.0
 */

#pragma once

#include <vector>
using namespace std;

/**
 * @struct Direction
 * @brief 正もしくは負の傾きを持つ線分が
 * 通過していく面の順番を管理するための構造体
 */
struct Direction {
	//! 負の傾きの線分が通過していく面の順番を，左上 (LT) から右下 (RB) の順で保持していくベクトル
	vector<char> LT_to_RD;
	//! 正の傾きの線分が通過していく面の順番を，左下 (LB) から右上 (RT) の順で保持していくベクトル
	vector<char> LD_to_RT;
	//! 負の傾きの線分が通過していく面の順番を，右下 (RB) から左上 (LT) の順で保持していくベクトル
	vector<char> RD_to_LT;
	//! 正の傾きの線分が通過していく面の順番を，右上 (RT) から左下 (LB) の順で保持していくベクトル
	vector<char> RT_to_LD;
};
