#pragma once
#include "IDManager.hpp"
#include "UnfoldingDimensions.hpp"
#include "AdjacentList.hpp"
#include <vector>
#include <set>
#include <utility>
#include "Correspondence.hpp"
#include "Direction.hpp"

class UpdateID {
private:
	IDManager& idManager;
	/* 面どうしの対応関係を求める */
	void getFacesIDFromDirection(int& x, int& y, vector<char> dirs, string str, vector<int>& faces, UnfoldingDimensions unfoldDim);
	void adjustConerDirection(int& x, int& y, string str_before, string str_after, UnfoldingDimensions unfoldDim);
	vector<int> getConerFaces012(int x, int y, Direction dirs, UnfoldingDimensions unfoldDim);
	vector<int> getConerFaces435(int x, int y, Direction dirs, UnfoldingDimensions unfoldDim);
	pair<vector<int>, vector<int>> getFacesPairs(UnfoldingDimensions unfoldDim);
	
	/* 面の座標どうしの対応関係を求め，435面側を更新 */
	void getCoordPairs(vector<Correspondence>& coordPairs, pair<vector<int>, vector<int>> facesPairs, int roop, int label, UnfoldingDimensions unfoldDim);
	vector<Correspondence> getFacesCoordPairs(pair<vector<int>, vector<int>> facesPairs, UnfoldingDimensions unfoldDim);
	void overwriteFaces435(pair<vector<int>, vector<int>> facesPairs, UnfoldingDimensions unfoldDim);
	
	/* 頂点の座標どうしの対応関係を求め，435面側を更新 */
	vector<Correspondence> getVerticesCoordPairs(UnfoldingDimensions unfoldDim);
	void overwriteVertices435(vector<Correspondence> VerticesCoordPairs);

	/* 辺の座標どうしの対応関係を求め，435面側を更新 (a = 0 の時のみ) */
	vector<Correspondence> getEdgesCoordPairs(UnfoldingDimensions unfoldDim);
	void overwriteEdges435(vector<Correspondence> EdgesCoordPairs);

	/* 面の座標どうしの対応関係から，展開図の周りの面を更新 (a = 0 の時のみ) */
	void setFacesRoundCoordPairs(vector<Correspondence> facesCoordPairs, UnfoldingDimensions unfoldDim);

	/* 状態が 9 の面における隣接リストを取得 */
	vector<Coordinates> getState9FacesCoordinates();
	void getState9AdjacentLists(vector<AdjacentList>& adjLists, vector<Coordinates> coordinates);

	void matchRotate0(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchRotate90(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchRotate180(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchRotate270(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);

	void matchLabel0(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchLabel1(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchLabel2(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchLabel3(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchLabel4(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchLabel5(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void matchLabel6(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists);
	void getState1AdjacentLists(vector<AdjacentList>& adjLists, vector<Correspondence> facesCoordPairs, UnfoldingDimensions unfoldDim);

	vector<AdjacentList> deleteSameAdjLists(vector<AdjacentList> adjLists);
	void relabelFacesID(vector<int>& func_f, set<int>& faces, vector<AdjacentList> adjLists);
	void relabelVerticesID(vector<int>& func_v, set<int>& vertices, vector<AdjacentList> adjLists);
	void relabelEdgesID(vector<vector<int>>& func_e, vector<int> func_v, set<pair<int, int>>& edges, vector<AdjacentList> adjLists);
	
	void printNumbersAndEdgesList(set<int> faces, set<pair<int, int>> edges, set<int> vertices);
	void printLabeledList(vector<AdjacentList> adjLists);
	void printRelabeledList(vector<int> func_f, vector<int> func_v, vector<vector<int>> func_e, vector<AdjacentList> adjLists);
	void printBaseFaceBaceEdgePairs(vector<int> func_f, vector<int> func_v, vector<vector<int>> func_e, vector<AdjacentList> adjLists);

public:
	UpdateID(IDManager& idManager);
	void getAdjacentList(vector<AdjacentList>& adjLists, UnfoldingDimensions unfoldDim);

	void printOriginalAdjacentList(vector<AdjacentList> adjLists);
	void printAdjacentList(vector<AdjacentList> adjLists);
};