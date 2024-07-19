#include "EdgeManager.hpp"

/***************
 * private 関数
 ***************/

EdgeManager::EdgeManager(IDManager& idManager) : idManager(idManager) {}

/*************************************************
 * 両端点の状態に応じて，辺の状況を返すメンバ関数
 * true の時は内包／交差している
 * false の時は内包／交差していない
 * 
 * 両端点を x <-> y で表す
 * 0: 内包していない頂点
 * 1: 内包している頂点
 * 2: 直方体のコーナーに該当する頂点
 * 
 * - 0 <-> 0 : false
 * - 0 <-> 1 : true (a == 0 の時は false)
 * - 0 <-> 2 : false
 * - 1 <-> 1 : true
 * - 1 <-> 2 : true
 * - 2 <-> 2 : true
 *************************************************/
bool EdgeManager::getEdgesIntersection(int x, int y, UnfoldingDimensions unfoldDim) {
	if(x > y) swap(x, y);
	if(x == 0 && y == 0) return false;
	else if(x == 0 && y == 2) return false;
	if(unfoldDim.a == 0 && x == 0 && y == 1) return false;
	else return true;
}

/***************
 * public 関数
 ***************/

/*************************************************
 * 水平方向の辺の状態を更新するメンバ関数
 *************************************************/
void EdgeManager::getHorizonEdges(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < idManager.getVerticesIDsize().row; ++i) {
		for(int j = 0; j < idManager.getVerticesIDsize().column - 1; ++j) {
			int left_vertex = idManager.getVerticesStatus(i, j);
			int right_vertex = idManager.getVerticesStatus(i, j+1);
			if(getEdgesIntersection(left_vertex, right_vertex, unfoldDim)) {
				idManager.setEdgesStatus(i*2, j, 1);
			}
		}
	}
}

/*************************************************
 * 垂直方向の辺の状態を更新するメンバ関数
 *************************************************/
void EdgeManager::getVerticalEdges(UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < idManager.getVerticesIDsize().row - 1; ++i) {
		for(int j = 0; j < idManager.getVerticesIDsize().column; ++j) {
			int top_vertex = idManager.getVerticesStatus(i, j);
			int bottom_vertex = idManager.getVerticesStatus(i+1, j);
			if(getEdgesIntersection(top_vertex, bottom_vertex, unfoldDim)) {
				idManager.setEdgesStatus(i*2+1, j, 1);
			}
		}
	}	
}
