#include <iostream>
#include "VertexManager.hpp"
#include "ArraySize.hpp"
#include "IDManager.hpp"

/***************
 * private 関数
 ***************/

VertexManager::VertexManager(IDManager& idManager) : idManager(idManager) {}

/*************************************************
 * 面 0 の左上の頂点から右下方向に，
 * 辺と交差する頂点を 2 に更新していくメンバ関数
 *************************************************/
void VertexManager::updateConerStatus_012_top(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < unfoldDim.x + unfoldDim.z + unfoldDim.x + 1; ++i) {
		int x = unfoldDim.margin_left
			  + unfoldDim.a * unfoldDim.y
			  + i * unfoldDim.b;
		int y = unfoldDim.margin_top
			  + unfoldDim.top_shift
			  + i * unfoldDim.a;
		idManager.setVerticesStatus(x, y, 2);
	}
}

/*************************************************
 * 面 0 の左下の頂点から右下方向に，
 * 辺と交差する頂点を 2 に更新していくメンバ関数
 *************************************************/
void VertexManager::updateConerStatus_012_bottom(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < unfoldDim.x + unfoldDim.z + unfoldDim.x + 1; ++i) {
		int x = unfoldDim.margin_left
			  + i * unfoldDim.b;
		int y = unfoldDim.margin_top
			  + unfoldDim.top_shift
			  + unfoldDim.b * unfoldDim.y
			  + i * unfoldDim.a;
		idManager.setVerticesStatus(x, y, 2);
	}
}

/*************************************************
 * 面 0 の左上の頂点から左下方向に，
 * 辺と交差する頂点を 2 に更新していくメンバ関数
 *************************************************/
void VertexManager::updateConerStatus_0_left(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < unfoldDim.y + 1; ++i) {
		int x = unfoldDim.margin_left
			  + unfoldDim.a * unfoldDim.y
			  - i * unfoldDim.a;
		int y = unfoldDim.margin_top
			  + unfoldDim.top_shift
			  + i * unfoldDim.b;
		idManager.setVerticesStatus(x, y, 2);
	}
}

/*************************************************
 * 面 4 の左上の頂点から左下方向に，
 * 辺と交差する頂点を 2 に更新していくメンバ関数
 *************************************************/
void VertexManager::updateConerStatus_435_left(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < unfoldDim.x + unfoldDim.y + unfoldDim.x + 1; ++i) {
		int x = unfoldDim.margin_left
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.b * unfoldDim.z
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.a * unfoldDim.y
			  + unfoldDim.a * unfoldDim.x
			  - unfoldDim.a * i;
		int y = unfoldDim.margin_top
			  + unfoldDim.top_shift
			  + unfoldDim.a * unfoldDim.x
			  + unfoldDim.a * unfoldDim.z
			  + unfoldDim.a * unfoldDim.x
			  - unfoldDim.b * unfoldDim.x
			  + unfoldDim.b * i;
		if(i < unfoldDim.x + 1 || i > unfoldDim.x + unfoldDim.y - 1){
			idManager.setVerticesStatus(x, y, 2);
		}
	}

}

/*************************************************
 * 面 4 の左上の頂点から右下方向に，
 * 辺と交差する頂点を 2 に更新していくメンバ関数
 *************************************************/
void VertexManager::updateConerStatus_4_top(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < unfoldDim.z + 1; ++i) {
		int x = unfoldDim.margin_left
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.b * unfoldDim.z
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.a * unfoldDim.y
			  + unfoldDim.a * unfoldDim.x
			  + unfoldDim.b * i;
		int y = unfoldDim.margin_top
			  + unfoldDim.top_shift
			  + unfoldDim.a * unfoldDim.x
			  + unfoldDim.a * unfoldDim.z
			  + unfoldDim.a * unfoldDim.x
			  - unfoldDim.b * unfoldDim.x
			  + unfoldDim.a * i;
		idManager.setVerticesStatus(x, y, 2);
	}
}

/*************************************************
 * 面 4 の右上の頂点から左下方向に，
 * 辺と交差する頂点を 2 に更新していくメンバ関数
 *************************************************/
void VertexManager::updateConerStatus_435_right(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < unfoldDim.x + unfoldDim.y + unfoldDim.x + 1; ++i) {
		int x = unfoldDim.margin_left
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.b * unfoldDim.z
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.a * unfoldDim.y
			  + unfoldDim.a * unfoldDim.x
			  + unfoldDim.b * unfoldDim.z
			  - unfoldDim.a * i;
		int y = unfoldDim.margin_top
			  + unfoldDim.top_shift
			  + unfoldDim.a * unfoldDim.x
			  + unfoldDim.a * unfoldDim.z
			  + unfoldDim.a * unfoldDim.x
			  - unfoldDim.b * unfoldDim.x
			  + unfoldDim.a * unfoldDim.z
			  + unfoldDim.b * i;
		idManager.setVerticesStatus(x, y, 2);
	}
}

/*************************************************
 * 面 5 の左下の頂点から右下方向に，
 * 辺と交差する頂点を 2 に更新していくメンバ関数
 *************************************************/
void VertexManager::updateConerStatus_5_bottom(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < unfoldDim.z + 1; ++i) {
		int x = unfoldDim.margin_left
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.b * unfoldDim.z
			  + unfoldDim.b * unfoldDim.x
			  - unfoldDim.a * unfoldDim.x
			  + unfoldDim.b * i;;
		int y = unfoldDim.margin_top
			  + unfoldDim.top_shift
			  + unfoldDim.b * unfoldDim.y
			  + unfoldDim.a * unfoldDim.x
			  + unfoldDim.a * unfoldDim.z
			  + unfoldDim.a * unfoldDim.x
			  + unfoldDim.b * unfoldDim.x
			  + unfoldDim.a * i;
		idManager.setVerticesStatus(x, y, 2);
	}
}

/***************
 * public 関数
 ***************/

/*************************************************
 * 状態が 9 の（完全な正方形で格子展開図に内包）面の
 * 四方の頂点は必ず内包している頂点である．
 * これらの頂点の状態を 1 とする．
 *************************************************/
void VertexManager::getInnnerVertices() {
	for(int i = 0; i < idManager.getFacesIDsize().row; ++i) {
		for(int j = 0; j < idManager.getFacesIDsize().column; ++j) {
			if(idManager.getFacesStatus(i, j) == 9) {
				idManager.setVerticesStatus(j, i, 1);
				idManager.setVerticesStatus(j, i+1, 1);
				idManager.setVerticesStatus(j+1, i, 1);
				idManager.setVerticesStatus(j+1, i+1, 1);
			}
		}
	}
}

/*************************************************
 * 直方体のコーナーに来る頂点の状態を 2 とする．
 * （辺を認識するために，1 と区別する必要がある）
 *************************************************/
void VertexManager::getConerVertices(UnfoldingDimensions unfoldDim) {
	updateConerStatus_012_top(unfoldDim);
	updateConerStatus_012_bottom(unfoldDim);
	updateConerStatus_0_left(unfoldDim);
	updateConerStatus_435_left(unfoldDim);
	updateConerStatus_4_top(unfoldDim);
	updateConerStatus_435_right(unfoldDim);
	updateConerStatus_5_bottom(unfoldDim);
}