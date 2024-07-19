#pragma once
#include "IDManager.hpp"
#include "UnfoldingDimensions.hpp"
#include "ArraySize.hpp"

class VertexManager {
private:
	IDManager& idManager;
	void updateConerStatus_012_top(UnfoldingDimensions unfoldDim);
	void updateConerStatus_012_bottom(UnfoldingDimensions unfoldDim);
	void updateConerStatus_0_left(UnfoldingDimensions unfoldDim);
	void updateConerStatus_435_left(UnfoldingDimensions unfoldDim);
	void updateConerStatus_4_top(UnfoldingDimensions unfoldDim);
	void updateConerStatus_435_right(UnfoldingDimensions unfoldDim);
	void updateConerStatus_5_bottom(UnfoldingDimensions unfoldDim);

public:
	VertexManager(IDManager& idManager);
	void getInnnerVertices();
	void getConerVertices(UnfoldingDimensions unfoldDim);
};