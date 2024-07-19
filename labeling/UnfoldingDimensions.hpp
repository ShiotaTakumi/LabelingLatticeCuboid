/**
 * @file UnfoldingDimensions.hpp
 * @brief UnfoldingDimensions 構造体
 * @author Takumi SHIOTA
 * @date 2024/06/24 最終更新
 * @version 1.0
 */

#pragma once

/**
 * @struct UnfoldingDimensions
 * @brief 格子立方体または格子直方体の格子展開図を生成する際に，
 * 必要となる盤面のサイズおよび余白を管理するための構造体
 */
struct UnfoldingDimensions {
	//! 格子立方体の一辺の長さを決める入力値（小さい方の値）
	int a;
	//! 格子立方体の一辺の長さを決める入力値（大きい方の値）
	int b;
	//! 格子直方体の縦の長さ
	int x;
	//! 格子直方体の横の長さ
	int y;
	//! 格子直方体の高さ
	int z;
	//! 盤面の縦幅
	int height;
	//! 盤面の横幅
	int width;
	//! 盤面の上余白
	int margin_top;
	//! 盤面の下余白
	int margin_bottom;
	//! 盤面の右余白
	int margin_right;
	//! 盤面の左余白
	int margin_left;
	//! 上余白が足りない際にシフトする値
	int top_shift;
};
