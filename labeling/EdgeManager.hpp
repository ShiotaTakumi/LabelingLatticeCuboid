#pragma once
#include "IDManager.hpp"
#include "UnfoldingDimensions.hpp"

class EdgeManager {
private:
	IDManager& idManager;
	bool getEdgesIntersection(int x, int y, UnfoldingDimensions unfoldDim);

public:
	EdgeManager(IDManager& idManager);
	void getHorizonEdges(UnfoldingDimensions unfoldDim);
	void getVerticalEdges(UnfoldingDimensions unfoldDim);
};