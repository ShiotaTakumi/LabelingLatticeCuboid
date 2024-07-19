/**
 * @file Correspondence.hpp
 * @brief Correspondence 構造体
 * @author Takumi SHIOTA
 * @date 2024/06/24 最終更新
 * @version 1.0
 */

#pragma once

/**
 * @struct Correspondence
 * @brief 格子展開図のうち，貼り合わされる部分の
 * 面／辺／頂点の座標の対応関係を管理するための構造体
 */
struct Correspondence {
	//! 一方の座標の x 座標
	int x_first;
	//! 一方の座標の y 座標
	int y_first;
	//! 他方の座標の x 座標
	int x_second;
	//! 他方の座標の y 座標
	int y_second;
	//! 一方の座標と他方の座標との対応関係の種類を管理するラベル
	int label;
};
