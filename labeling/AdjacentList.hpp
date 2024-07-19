/**
 * @file AdjacentList.hpp
 * @brief AdjacentList 構造体
 * @author Takumi SHIOTA
 * @date 2024/06/24 最終更新
 * @version 1.0
 */

#pragma once

#include <vector>
using namespace std;

/**
 * @struct AdjacentList
 * @brief 正方形のID，およびその頂点，辺，隣接する面のIDを管理するための構造体
 */
struct AdjacentList {
	//! 着目している正方形のID
	int id; 				
	//! 正方形の四隅の頂点のID（左上 → 右上 → 右下 → 左下の順）
	vector<int> vertex; 	
	//! 正方形の四辺のID（上 → 右 → 下 → 左の順）
	vector<int> edge; 		
	//! 隣接している四面のID（上 → 右 → 下 → 左の順）
	vector<int> face; 		
};
