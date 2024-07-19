#include "UpdateID.hpp"
#include "Direction.hpp"
#include <iostream>
#include "Correspondence.hpp"
#include <algorithm>

using namespace std;

/***************
 * private 関数
 ***************/

UpdateID::UpdateID(IDManager& idManager) : idManager(idManager) {}

/*************************************************
 * (x, y) 座標と，方向から面のIDを求めるメンバ関数
 * dir のサイズの for ループが回ったら，
 * 次の (x, y) 座標の開始位置を計算する
 *************************************************/
void UpdateID::getFacesIDFromDirection(int& x, int& y, vector<char> dir, string str, vector<int>& faces, UnfoldingDimensions unfoldDim) {
	faces.push_back(idManager.getFacesID(x, y));
	for(int i = 0; i < dir.size(); ++i) {
		if(dir[i] == 'l') y--;
		else if(dir[i] == 'r') y++;
		else if(dir[i] == 't') x--;
		else if(dir[i] == 'b') x++;
		faces.push_back(idManager.getFacesID(x, y));
	}
	if(unfoldDim.a != 0) {
		if(str == "LT_to_RD") {y++; x++;}
		else if(str == "RD_to_LT") {y--; x--;}
		else if(str == "LD_to_RT") {y++; x--;}
		else if(str == "RT_to_LD") {y--; x++;}
	} else {
		if(str == "LT_to_RD") {y++;}
		else if(str == "RD_to_LT") {y--;}
		else if(str == "LD_to_RT") {x--;}
		else if(str == "RT_to_LD") {x++;}
	}
}

/*************************************************
 * 展開図のコーナーで曲がる方向を調整するメンバ関数
 *************************************************/
void UpdateID::adjustConerDirection(int& x, int& y, string str_before, string str_after, UnfoldingDimensions unfoldDim) {
	if(str_before == "RD_to_LT" && str_after == "RT_to_LD") {
		if(unfoldDim.a != 0) x++;
		else y++;
	}
	else if(str_before == "RT_to_LD" && str_after == "LT_to_RD") {
		if(unfoldDim.a != 0) y++;
		else x--;
	}
	else if(str_before == "LD_to_RT" && str_after == "LT_to_RD") { x++; }
	else if(str_before == "LT_to_RD" && str_after == "RT_to_LD") { y--; }
	else if(str_before == "RT_to_LD" && str_after == "RD_to_LT") { x--; }
	else if(str_before == "RD_to_LT" && str_after == "LD_to_RT") { y++; }
}

/*************************************************
 * 面 0, 1, 2 の境界に位置する面の ID を求めるメンバ関数
 * 
 * 下記の順で面の ID を格納していく
 * 面 2 の上部 -> 面 1 の上部 -> 面 0 の上部 -> 
 * 面 0 の左部 ->
 * 面 0 の下部 -> 面 1 の下部 -> 面 2 の下部
 *************************************************/
vector<int> UpdateID::getConerFaces012(int x, int y, Direction dirs, UnfoldingDimensions unfoldDim) {
	vector<int> faces;
	/* 面 2 の上部 -> 面 1 の上部 -> 面 0 の上部 */
	for(int i = 0; i < (unfoldDim.x + unfoldDim.z + unfoldDim.x); ++i) {
		getFacesIDFromDirection(x, y, dirs.RD_to_LT, "RD_to_LT", faces, unfoldDim);
	}
	adjustConerDirection(x, y, "RD_to_LT", "RT_to_LD", unfoldDim);
	/* 面 0 の左部 */
	for(int i = 0; i < (unfoldDim.y); ++i) {
		getFacesIDFromDirection(x, y, dirs.RT_to_LD, "RT_to_LD", faces, unfoldDim);
	}
	adjustConerDirection(x, y, "RT_to_LD", "LT_to_RD", unfoldDim);
	/* 面 0 の下部 -> 面 1 の下部 -> 面 2 の下部 */
	for(int i = 0; i < (unfoldDim.x + unfoldDim.z + unfoldDim.x); ++i) {
		getFacesIDFromDirection(x, y, dirs.LT_to_RD, "LT_to_RD", faces, unfoldDim);
	}
	// for(auto f : faces) { cout << f << " "; } cout << endl;
	return faces;
}

/*************************************************
 * 面 4, 3, 5 の境界に位置する面の ID を求めるメンバ関数
 * 
 * 下記の順で面の ID を格納していく
 * 面 4 の左部 -> 
 * 面 4 の上部 -> 
 * 面 4 の右部 -> 面 3 の右部 -> 面 5 の右部 -> 
 * 面 5 の下部 -> 
 * 面 5 の左部
 *************************************************/
vector<int> UpdateID::getConerFaces435(int x, int y, Direction dirs, UnfoldingDimensions unfoldDim) {
	vector<int> faces;
	/* 面 4 の左部 */
	for(int i = 0; i < (unfoldDim.x); ++i) {
		getFacesIDFromDirection(x, y, dirs.LD_to_RT, "LD_to_RT", faces, unfoldDim);
	}
	adjustConerDirection(x, y, "LD_to_RT", "LT_to_RD", unfoldDim);
	/* 面 4 の上部 */
	for(int i = 0; i < (unfoldDim.z); ++i) {
		getFacesIDFromDirection(x, y, dirs.LT_to_RD, "LT_to_RD", faces, unfoldDim);
	}
	adjustConerDirection(x, y, "LT_to_RD", "RT_to_LD", unfoldDim);
	/* 面 4 の右部 -> 面 3 の右部 -> 面 5 の右部 */
	for(int i = 0; i < (unfoldDim.x + unfoldDim.y + unfoldDim.x); ++i) {
		getFacesIDFromDirection(x, y, dirs.RT_to_LD, "RT_to_LD", faces, unfoldDim);
	}
	adjustConerDirection(x, y, "RT_to_LD", "RD_to_LT", unfoldDim);
	/* 面 5 の下部 */
	for(int i = 0; i < (unfoldDim.z); ++i) {
		getFacesIDFromDirection(x, y, dirs.RD_to_LT, "RD_to_LT", faces, unfoldDim);
	}
	adjustConerDirection(x, y, "RD_to_LT", "LD_to_RT", unfoldDim);
	/* 面 5 の左部 */
	for(int i = 0; i < (unfoldDim.x); ++i) {
		getFacesIDFromDirection(x, y, dirs.LD_to_RT, "LD_to_RT", faces, unfoldDim);
	}
	// for(auto f : faces) { cout << f << " "; } cout << endl;
	return faces;
}

/*************************************************
 * 面の ID どうしの対応関係を求めるメンバ関数
 *************************************************/
pair<vector<int>, vector<int>> UpdateID::getFacesPairs(UnfoldingDimensions unfoldDim) {
	int x2_right_top, y2_right_top; // 面 2 の右上に位置する面の (x, y) 座標
	x2_right_top = unfoldDim.margin_top
				 + unfoldDim.top_shift
				 + unfoldDim.a * unfoldDim.x
				 + unfoldDim.a * unfoldDim.z
				 + unfoldDim.a * unfoldDim.x
				 - 1;
	y2_right_top = unfoldDim.margin_left
				 + unfoldDim.a * unfoldDim.y
				 + unfoldDim.b * unfoldDim.x
				 + unfoldDim.b * unfoldDim.z
				 + unfoldDim.b * unfoldDim.x
				 - 1;
	if(unfoldDim.a == 0) {
		x2_right_top += 1;
	}
	int x4_left_bottom, y4_left_bottom; // 面 4 の左下に位置する面の (x, y) 座標
	x4_left_bottom = unfoldDim.margin_top
				   + unfoldDim.top_shift
				   + unfoldDim.a * unfoldDim.x
				   + unfoldDim.a * unfoldDim.z
				   + unfoldDim.a * unfoldDim.x
				   - 1;
	y4_left_bottom = unfoldDim.margin_left
				   + unfoldDim.a * unfoldDim.y
				   + unfoldDim.b * unfoldDim.x
				   + unfoldDim.b * unfoldDim.z
				   + unfoldDim.b * unfoldDim.x;
	Direction dirs;
	dirs.LD_to_RT = idManager.getLineSegment_LD_to_RT(unfoldDim);
	dirs.LT_to_RD = idManager.getLineSegment_LT_to_RD(unfoldDim);
	dirs.RD_to_LT = idManager.getLineSegment_Reverse(dirs.LT_to_RD);
	dirs.RT_to_LD = idManager.getLineSegment_Reverse(dirs.LD_to_RT);
	vector<int> f0_f1_f2 = getConerFaces012(x2_right_top, y2_right_top, dirs, unfoldDim);
	vector<int> f4_f3_f5 = getConerFaces435(x4_left_bottom, y4_left_bottom, dirs, unfoldDim);
	return make_pair(f0_f1_f2, f4_f3_f5);
}

/*************************************************
 * coordPairs に値を格納していくメンバ関数
 * 対応している辺の間には，label を貼る
 * faces_status の 2 ~ 8 は不要なので，全て 1 に更新
 *************************************************/
void UpdateID::getCoordPairs(vector<Correspondence>& coordPairs, pair<vector<int>, vector<int>> facesPairs, int roop, int label, UnfoldingDimensions unfoldDim) {
	vector<int> Face012 = facesPairs.first;
	vector<int> Face435 = facesPairs.second;
	for(int i = 0; i < idManager.getCrossingNumber(unfoldDim); ++i) {
		int num = idManager.getCrossingNumber(unfoldDim) * roop + i;
		int x012 = idManager.getFacesCoordintes(Face012[num]).x;
		int y012 = idManager.getFacesCoordintes(Face012[num]).y;
		int x435 = idManager.getFacesCoordintes(Face435[num]).x;
		int y435 = idManager.getFacesCoordintes(Face435[num]).y;

		coordPairs[num].x_first = x012;
		coordPairs[num].y_first = y012;
		coordPairs[num].x_second = x435;
		coordPairs[num].y_second = y435;
		coordPairs[num].label = label;

		idManager.setFacesStatus(x012, y012, 1);
		idManager.setFacesStatus(x435, y435, 1);

	}
}

/*************************************************
 * facesPairs から対応する面の座標を求めるメンバ関数
 *************************************************/
vector<Correspondence> UpdateID::getFacesCoordPairs(pair<vector<int>, vector<int>> facesPairs, UnfoldingDimensions unfoldDim) {
	int PairsSize = facesPairs.first.size();
	vector<Correspondence> coordPairs(PairsSize);
	int roop = unfoldDim.x
			 + unfoldDim.z
			 + unfoldDim.x
			 + unfoldDim.y
			 + unfoldDim.x
			 + unfoldDim.z
			 + unfoldDim.x;
	for(int i = 0; i < roop; ++i) {
		int label;
		int label_cnt = 0;
		if(i < (label_cnt += unfoldDim.x)) label = 0;
		else if(i < (label_cnt += unfoldDim.z)) label = 1;
		else if(i < (label_cnt += unfoldDim.x)) label = 2;
		else if(i < (label_cnt += unfoldDim.y)) label = 3;
		else if(i < (label_cnt += unfoldDim.x)) label = 4;
		else if(i < (label_cnt += unfoldDim.z)) label = 5;
		else if(i < (label_cnt += unfoldDim.x)) label = 6;

		getCoordPairs(coordPairs, facesPairs, i, label, unfoldDim);
	}
	return coordPairs;
}

/*************************************************
 * a != 0 の時， 4,3,5 面の外周の面をマージするメンバ関数
 * 同時に，4,3,5 面の外周の faces_status を 0 に更新
 *************************************************/
void UpdateID::overwriteFaces435(pair<vector<int>, vector<int>> facesPairs, UnfoldingDimensions unfoldDim) {
	int PairsSize = facesPairs.first.size();
	int roop = unfoldDim.x
			 + unfoldDim.z
			 + unfoldDim.x
			 + unfoldDim.y
			 + unfoldDim.x
			 + unfoldDim.z
			 + unfoldDim.x;
	for(int i = 0; i < roop; ++i) {
	vector<int> Face012 = facesPairs.first;
	vector<int> Face435 = facesPairs.second;
		for(int j = 0; j < idManager.getCrossingNumber(unfoldDim); ++j) {
			int num = idManager.getCrossingNumber(unfoldDim) * i + j;
			int x012 = idManager.getFacesCoordintes(Face012[num]).x;
			int y012 = idManager.getFacesCoordintes(Face012[num]).y;
			int x435 = idManager.getFacesCoordintes(Face435[num]).x;
			int y435 = idManager.getFacesCoordintes(Face435[num]).y;
			idManager.setFacesID(x435, y435, idManager.getFacesID(x012, y012));
			idManager.setFacesStatus(x435, y435, 0);
		}
	}
}

/*************************************************
 * 直線と接触している頂点の組みを取得するメンバ関数
 *************************************************/
vector<Correspondence> UpdateID::getVerticesCoordPairs(UnfoldingDimensions unfoldDim) {
	int PairsSize = unfoldDim.x + unfoldDim.z + unfoldDim.x 
				  + unfoldDim.y 
				  + unfoldDim.x + unfoldDim.z + unfoldDim.x;
	vector<Correspondence> coordPairs(PairsSize);
	int x3_left_top, y3_left_top; // 面 3 の右上に位置する頂点の (x, y) 座標
	x3_left_top = unfoldDim.margin_top
				+ unfoldDim.top_shift
				+ unfoldDim.a * unfoldDim.x
				+ unfoldDim.a * unfoldDim.z
				+ unfoldDim.a * unfoldDim.x;
	y3_left_top = unfoldDim.margin_left
				+ unfoldDim.a * unfoldDim.y
				+ unfoldDim.b * unfoldDim.x
				+ unfoldDim.b * unfoldDim.z
				+ unfoldDim.b * unfoldDim.x;
	/* 0, 1, 2 面の側の頂点の把握 */
	int x012 = x3_left_top; int y012 = y3_left_top;
	int cnt012 = 0;
	/* 0, 1, 2 面の上部 */
	for(int i = 0; i < (unfoldDim.x + unfoldDim.z + unfoldDim.x); ++i){
		x012 -= unfoldDim.a;
		y012 -= unfoldDim.b;
		coordPairs[cnt012].x_first = x012;
		coordPairs[cnt012].y_first = y012;
		cnt012 += 1;
	}
	/* 0 面の左部 */
	for(int i = 0; i < (unfoldDim.y); ++i){
		x012 += unfoldDim.b;
		y012 -= unfoldDim.a;
		coordPairs[cnt012].x_first = x012;
		coordPairs[cnt012].y_first = y012;
		cnt012 += 1;
	}
	/* 0, 1, 2 面の下部 */
	for(int i = 0; i < (unfoldDim.x + unfoldDim.z + unfoldDim.x); ++i){
		x012 += unfoldDim.a;
		y012 += unfoldDim.b;
		coordPairs[cnt012].x_first = x012;
		coordPairs[cnt012].y_first = y012;
		cnt012 += 1;
	}
	/* 4, 3, 5 面の側の頂点の把握 */
	int x435 = x3_left_top; int y435 = y3_left_top;
	int cnt435 = 0;
	/* 4 面の左部 */
	for(int i = 0; i < (unfoldDim.x); ++i){
		x435 -= unfoldDim.b;
		y435 += unfoldDim.a;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		cnt435 += 1;
	}
	/* 4 面の上部 */
	for(int i = 0; i < (unfoldDim.z); ++i){
		x435 += unfoldDim.a;
		y435 += unfoldDim.b;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		cnt435 += 1;
	}
	/* 4, 3, 5 面の右部 */
	for(int i = 0; i < (unfoldDim.x + unfoldDim.y + unfoldDim.x); ++i){
		x435 += unfoldDim.b;
		y435 -= unfoldDim.a;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		cnt435 += 1;
	}
	/* 5 面の下部 */
	for(int i = 0; i < (unfoldDim.z); ++i){
		x435 -= unfoldDim.a;
		y435 -= unfoldDim.b;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		cnt435 += 1;
	}
	/* 5 面の左部 */
	for(int i = 0; i < (unfoldDim.x); ++i){
		x435 -= unfoldDim.b;
		y435 += unfoldDim.a;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		cnt435 += 1;
	}
	return coordPairs;
}

/*************************************************
 * 直線と接触している頂点の更新を行うメンバ関数
 *************************************************/
void UpdateID::overwriteVertices435(vector<Correspondence> verticesCoordPairs) {
	for(int i = 0; i < verticesCoordPairs.size(); ++i) {
		Correspondence corres = verticesCoordPairs[i];
		int id = idManager.getVerticesID(corres.x_first, corres.y_first);
		idManager.setVerticesID(corres.x_second, corres.y_second, id);
	}
}

/*************************************************
 * 直線と一致している辺の組みを取得するメンバ関数
 * (a = 0 の時のみ呼び出される)
 *************************************************/
vector<Correspondence> UpdateID::getEdgesCoordPairs(UnfoldingDimensions unfoldDim) {
	int PairsSize = unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.z + unfoldDim.b * unfoldDim.x 
				  + unfoldDim.b * unfoldDim.y
				  + unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.z + unfoldDim.b * unfoldDim.x;
	vector<Correspondence> coordPairs(PairsSize);
	int x3_top, y3_top; // 面 3 の上に位置する辺の (x, y) 座標
	x3_top = (unfoldDim.margin_top
		   + unfoldDim.top_shift
		   + unfoldDim.a * unfoldDim.x
		   + unfoldDim.a * unfoldDim.z
		   + unfoldDim.a * unfoldDim.x) * 2;
	y3_top = unfoldDim.margin_left
		   + unfoldDim.a * unfoldDim.y
		   + unfoldDim.b * unfoldDim.x
		   + unfoldDim.b * unfoldDim.z
		   + unfoldDim.b * unfoldDim.x - 1;
	/* 0, 1, 2 面の側の辺の把握 */
	int x012 = x3_top; int y012 = y3_top;
	int cnt012 = 0;
	coordPairs[cnt012].x_first = x012;
	coordPairs[cnt012].y_first = y012;
	// cout << idManager.getEdgesID(x012, y012) << endl;
	cnt012 += 1;
	/* 0, 1, 2 面の上部 (注) 以降，ループ回数は -1 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.z + unfoldDim.b * unfoldDim.x - 1); ++i){
		y012 -= 1;
		coordPairs[cnt012].x_first = x012;
		coordPairs[cnt012].y_first = y012;
		// cout << idManager.getEdgesID(x012, y012) << endl;
		cnt012 += 1;
	}
	/* 0 面の左上のコーナー */
	x012 += 1;
	coordPairs[cnt012].x_first = x012;
	coordPairs[cnt012].y_first = y012;
	// cout << idManager.getEdgesID(x012, y012) << endl;
	cnt012 += 1;
	/* 0 面の左部 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.y - 1); ++i){
		x012 += 2;
		coordPairs[cnt012].x_first = x012;
		coordPairs[cnt012].y_first = y012;
		// cout << idManager.getEdgesID(x012, y012) << endl;
		cnt012 += 1;
	}
	/* 0 面の左下のコーナー */
	x012 += 1;
	coordPairs[cnt012].x_first = x012;
	coordPairs[cnt012].y_first = y012;
	// cout << idManager.getEdgesID(x012, y012) << endl;
	cnt012 += 1;
	/* 0, 1, 2 面の下部 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.z + unfoldDim.b * unfoldDim.x - 1); ++i){
		y012 += 1;
		coordPairs[cnt012].x_first = x012;
		coordPairs[cnt012].y_first = y012;
		// cout << idManager.getEdgesID(x012, y012) << endl;
		cnt012 += 1;
	}
	/* 4, 3, 5 面の側の辺の把握 */
	int x4_left, y4_left; // 面 4 の左に位置する辺の (x, y) 座標
	x4_left = (unfoldDim.margin_top
			+ unfoldDim.top_shift
			+ unfoldDim.a * unfoldDim.x
			+ unfoldDim.a * unfoldDim.z
			+ unfoldDim.a * unfoldDim.x) * 2 - 1;
	y4_left = unfoldDim.margin_left
			+ unfoldDim.a * unfoldDim.y
			+ unfoldDim.b * unfoldDim.x
			+ unfoldDim.b * unfoldDim.z
			+ unfoldDim.b * unfoldDim.x;
	int x435 = x4_left; int y435 = y4_left;
	int cnt435 = 0;
	coordPairs[cnt435].x_second = x435;
	coordPairs[cnt435].y_second = y435;
	// cout << idManager.getEdgesID(x435, y435) << endl;
	cnt435 += 1;
	/* 4 面の左部 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x - 1); ++i){
		x435 -= 2;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		// cout << idManager.getEdgesID(x435, y435) << endl;
		cnt435 += 1;
	}
	/* 4 面の左上のコーナー */
	x435 -= 1;
	coordPairs[cnt435].x_second = x435;
	coordPairs[cnt435].y_second = y435;
	// cout << idManager.getEdgesID(x435, y435) << endl;
	cnt435 += 1;
	/* 4 面の上部 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.z - 1); ++i){
		y435 += 1;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		// cout << idManager.getEdgesID(x435, y435) << endl;
		cnt435 += 1;
	}
	/* 4 面の右上のコーナー */
	x435 += 1;
	y435 += 1;
	coordPairs[cnt435].x_second = x435;
	coordPairs[cnt435].y_second = y435;
	// cout << idManager.getEdgesID(x435, y435) << endl;
	cnt435 += 1;
	/* 4, 3, 5 面の右部 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.y + unfoldDim.b * unfoldDim.x - 1); ++i){
		x435 += 2;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		// cout << idManager.getEdgesID(x435, y435) << endl;
		cnt435 += 1;
	}
	/* 5 面の右下のコーナー */
	x435 += 1;
	y435 -= 1;
	coordPairs[cnt435].x_second = x435;
	coordPairs[cnt435].y_second = y435;
	// cout << idManager.getEdgesID(x435, y435) << endl;
	cnt435 += 1;
	/* 5 面の下部 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.z - 1); ++i){
		y435 -= 1;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		// cout << idManager.getEdgesID(x435, y435) << endl;
		cnt435 += 1;
	}
	/* 5 面の左下のコーナー */
	x435 -= 1;
	coordPairs[cnt435].x_second = x435;
	coordPairs[cnt435].y_second = y435;
	// cout << idManager.getEdgesID(x435, y435) << endl;
	cnt435 += 1;
	/* 5 面の左部 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x - 1); ++i){
		x435 -= 2;
		coordPairs[cnt435].x_second = x435;
		coordPairs[cnt435].y_second = y435;
		// cout << idManager.getEdgesID(x435, y435) << endl;
		cnt435 += 1;
	}
	return coordPairs;
}

/*************************************************
 * 直線と一致している辺の更新を行うメンバ関数
 *************************************************/
void UpdateID::overwriteEdges435(vector<Correspondence> edgesCoordPairs) {
	for(int i = 0; i < edgesCoordPairs.size(); ++i) {
		Correspondence corres = edgesCoordPairs[i];
		int id = idManager.getEdgesID(corres.x_first, corres.y_first);
		idManager.setEdgesID(corres.x_second, corres.y_second, id);
	}	
}

/*************************************************
 * 展開図の外周の面を塗りつぶすメンバ関数
 * (a = 0 の時のみ呼び出される)
 *************************************************/
void UpdateID::setFacesRoundCoordPairs(vector<Correspondence> facesCoordPairs, UnfoldingDimensions unfoldDim) {
	int cnt012 = 0;
	int x012, y012, x435, y435;
	/* 0, 1, 2 面の側の面の外周の把握 */
	/* 0, 1, 2 面の上側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.z + unfoldDim.b * unfoldDim.x); ++i) {
		x012 = facesCoordPairs[cnt012].x_first;
		y012 = facesCoordPairs[cnt012].y_first;
		x435 = facesCoordPairs[cnt012].x_second;
		y435 = facesCoordPairs[cnt012].y_second;
		idManager.setFacesID(x012 - 1, y012, idManager.getFacesID(x435, y435));	
		cnt012 += 1;	
	}
	/* 0 面の左側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.y); ++i) {
		x012 = facesCoordPairs[cnt012].x_first;
		y012 = facesCoordPairs[cnt012].y_first;
		x435 = facesCoordPairs[cnt012].x_second;
		y435 = facesCoordPairs[cnt012].y_second;
		idManager.setFacesID(x012, y012 - 1, idManager.getFacesID(x435, y435));	
		cnt012 += 1;	
	}
	/* 0, 1, 2 面の下側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.z + unfoldDim.b * unfoldDim.x); ++i) {
		x012 = facesCoordPairs[cnt012].x_first;
		y012 = facesCoordPairs[cnt012].y_first;
		x435 = facesCoordPairs[cnt012].x_second;
		y435 = facesCoordPairs[cnt012].y_second;
		idManager.setFacesID(x012 + 1, y012, idManager.getFacesID(x435, y435));	
		cnt012 += 1;	
	}
	/* 4, 3, 5 面の側の面の外周の把握 */
	int cnt435 = 0;
	/* 4 面の左側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x); ++i) {
		x435 = facesCoordPairs[cnt435].x_second;
		y435 = facesCoordPairs[cnt435].y_second;
		x012 = facesCoordPairs[cnt435].x_first;
		y012 = facesCoordPairs[cnt435].y_first;
		idManager.setFacesID(x435, y435 - 1, idManager.getFacesID(x012, y012));	
		cnt435 += 1;
	}
	/* 4 面の上側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.z); ++i) {
		x435 = facesCoordPairs[cnt435].x_second;
		y435 = facesCoordPairs[cnt435].y_second;
		x012 = facesCoordPairs[cnt435].x_first;
		y012 = facesCoordPairs[cnt435].y_first;
		idManager.setFacesID(x435 - 1, y435, idManager.getFacesID(x012, y012));	
		cnt435 += 1;
	}
	/* 4, 3, 5 面の右側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x + unfoldDim.b * unfoldDim.y + unfoldDim.b * unfoldDim.x); ++i) {
		x435 = facesCoordPairs[cnt435].x_second;
		y435 = facesCoordPairs[cnt435].y_second;
		x012 = facesCoordPairs[cnt435].x_first;
		y012 = facesCoordPairs[cnt435].y_first;
		idManager.setFacesID(x435, y435 + 1, idManager.getFacesID(x012, y012));	
		cnt435 += 1;
	}
	/* 5 面の下側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.z); ++i) {
		x435 = facesCoordPairs[cnt435].x_second;
		y435 = facesCoordPairs[cnt435].y_second;
		x012 = facesCoordPairs[cnt435].x_first;
		y012 = facesCoordPairs[cnt435].y_first;
		idManager.setFacesID(x435 + 1, y435, idManager.getFacesID(x012, y012));	
		cnt435 += 1;
	}
	/* 5 面の左側の外周の面の更新 */
	for(int i = 0; i < (unfoldDim.b * unfoldDim.x); ++i) {
		x435 = facesCoordPairs[cnt435].x_second;
		y435 = facesCoordPairs[cnt435].y_second;
		x012 = facesCoordPairs[cnt435].x_first;
		y012 = facesCoordPairs[cnt435].y_first;
		idManager.setFacesID(x435, y435 - 1, idManager.getFacesID(x012, y012));	
		cnt435 += 1;
	}
}

/*************************************************
 * faces_status が 9の座標を取得するメンバ関数
 *************************************************/
vector<Coordinates> UpdateID::getState9FacesCoordinates() {
	vector<Coordinates> coordinates;
	Coordinates coord;
	for(int i = 0; i < idManager.getFacesIDsize().row; ++i) {
		for(int j = 0; j < idManager.getFacesIDsize().column; ++j) {
			if(idManager.getFacesStatus(i, j) == 9) {
				coord.x = i;
				coord.y = j;
				coordinates.push_back(coord);
			}
		}
	}
	return coordinates;
}

/*************************************************
 * 状態が 9 となっている
 * 面 ID における Adjacent List を取得するメンバ関数
 *************************************************/
void UpdateID::getState9AdjacentLists(vector<AdjacentList>& adjLists,  vector<Coordinates> coordinates) {
	AdjacentList adjList;
	for(int i = 0; i < coordinates.size(); ++i) {
		int x = coordinates[i].x;
		int y = coordinates[i].y;
		adjList.id = idManager.getFacesID(x, y);
		adjList.vertex.push_back(idManager.getVerticesID(x, y));
		adjList.vertex.push_back(idManager.getVerticesID(x, y+1));
		adjList.vertex.push_back(idManager.getVerticesID(x+1, y+1));
		adjList.vertex.push_back(idManager.getVerticesID(x+1, y));
		adjList.edge.push_back(idManager.getEdgesID(x*2, y));
		adjList.edge.push_back(idManager.getEdgesID(x*2+1, y+1));
		adjList.edge.push_back(idManager.getEdgesID(x*2+2, y));
		adjList.edge.push_back(idManager.getEdgesID(x*2+1, y));
		adjList.face.push_back(idManager.getFacesID(x-1, y));
		adjList.face.push_back(idManager.getFacesID(x, y+1));
		adjList.face.push_back(idManager.getFacesID(x+1, y));
		adjList.face.push_back(idManager.getFacesID(x, y-1));
		adjLists.push_back(adjList);
		adjList.vertex.clear();
		adjList.edge.clear();
		adjList.face.clear();
	}
}

/*************************************************
 * 0 度回転して一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchRotate0(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点の隣接情報 */
	if(idManager.getVerticesStatus(x1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	}
	if(idManager.getVerticesStatus(x1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	}
	/* 辺の隣接情報 */
	if(idManager.getEdgesStatus(x1*2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1+1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	}
	if(idManager.getEdgesStatus(x1*2+2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	}
	/* 面の隣接情報 */
	if(idManager.getFacesStatus(x1-1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1-1, y1));
	}
	if(idManager.getFacesStatus(x1, y1+1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2+1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	if(idManager.getFacesStatus(x1+1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1+1, y1));
	}
	if(idManager.getFacesStatus(x1, y1-1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adjLists.push_back(adj);	
}

/*************************************************
 * 90 度回転して一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchRotate90(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点の隣接情報 */
	if(idManager.getVerticesStatus(x1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	}
	if(idManager.getVerticesStatus(x1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	}
	/* 辺の隣接情報 */
	if(idManager.getEdgesStatus(x1*2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1+1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	}
	if(idManager.getEdgesStatus(x1*2+2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	}
	/* 面の隣接情報 */
	if(idManager.getFacesStatus(x1-1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1-1, y1));
	}
	if(idManager.getFacesStatus(x1, y1+1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	if(idManager.getFacesStatus(x1+1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2+1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1+1, y1));
	}
	if(idManager.getFacesStatus(x1, y1-1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adjLists.push_back(adj);	
}

/*************************************************
 * 180 度回転して一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchRotate180(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点の隣接情報 */
	if(idManager.getVerticesStatus(x1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	}
	if(idManager.getVerticesStatus(x1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	}
	/* 辺の隣接情報 */
	if(idManager.getEdgesStatus(x1*2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1+1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	}
	if(idManager.getEdgesStatus(x1*2+2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	}
	/* 面の隣接情報 */
	if(idManager.getFacesStatus(x1-1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1-1, y1));
	}
	if(idManager.getFacesStatus(x1, y1+1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	if(idManager.getFacesStatus(x1+1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1+1, y1));
	}
	if(idManager.getFacesStatus(x1, y1-1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2+1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adjLists.push_back(adj);	
}

/*************************************************
 * 270 度回転して一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchRotate270(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点の隣接情報 */
	if(idManager.getVerticesStatus(x1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	}
	if(idManager.getVerticesStatus(x1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1+1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	}
	if(idManager.getVerticesStatus(x1+1, y1) == 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	}
	/* 辺の隣接情報 */
	if(idManager.getEdgesStatus(x1*2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1+1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	}
	if(idManager.getEdgesStatus(x1*2+2, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	}
	if(idManager.getEdgesStatus(x1*2+1, y1) == 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	}
	/* 面の隣接情報 */
	if(idManager.getFacesStatus(x1-1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2+1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1-1, y1));
	}
	if(idManager.getFacesStatus(x1, y1+1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	if(idManager.getFacesStatus(x1+1, y1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1+1, y1));
	}
	if(idManager.getFacesStatus(x1, y1-1) == 0) {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adjLists.push_back(adj);	
}

/*************************************************
 * Label 0 で一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchLabel0(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x2, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1+1));
	if(idManager.getFacesID(x1, y1) != idManager.getFacesID(x1+1, y1)) {
		adj.face.push_back(idManager.getFacesID(x1+1, y1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1+1, y1+1));
	}
	adj.face.push_back(idManager.getFacesID(x1, y1-1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();

	adj.id = idManager.getFacesID(x2, y2);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	if(idManager.getFacesStatus(x2-1, y2) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1-1));
	}
	adj.vertex.push_back(idManager.getVerticesID(x2+1, y2+1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	/* 辺 */
	if(idManager.getFacesStatus(x2-1, y2) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1-1));
	}
	adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	/* 面 */
	if(idManager.getFacesStatus(x2-1, y2) != 0) {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adj.face.push_back(idManager.getFacesID(x2, y2+1));
	adj.face.push_back(idManager.getFacesID(x2+1, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
}

/*************************************************
 * Label 1 で一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchLabel1(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x2, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1+1));
	adj.face.push_back(idManager.getFacesID(x1+1, y1));
	adj.face.push_back(idManager.getFacesID(x1, y1-1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
	
	adj.id = idManager.getFacesID(x2, y2);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	if(idManager.getFacesStatus(x2, y2+1) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1-1));
	}
	if(idManager.getFacesStatus(x2, y2-1) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1+2));
	}
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	if(idManager.getFacesStatus(x2, y2+1) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1-1));
	}
	adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	if(idManager.getFacesStatus(x2, y2-1) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1+1));
	}
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x1, y1));
	if(idManager.getFacesStatus(x2, y2+1) != 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2+1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adj.face.push_back(idManager.getFacesID(x2+1, y2));
	if(idManager.getFacesStatus(x2, y2-1) != 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
}

/*************************************************
 * Label 2 で一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchLabel2(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x2, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1+1));
	adj.face.push_back(idManager.getFacesID(x1+1, y1));
	if(idManager.getFacesStatus(x1, y1-1) != 0) {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	}
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
	
	adj.id = idManager.getFacesID(x2, y2);
	/* 頂点 */
	if(idManager.getFacesStatus(x2-1, y2) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1, y1+2));
	}
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	/* 辺 */
	if(idManager.getFacesStatus(x2-1, y2) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2, y1+1));
	}
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	/* 面 */
	if(idManager.getFacesStatus(x2-1, y2) != 0) {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	adj.face.push_back(idManager.getFacesID(x1, y1));
	adj.face.push_back(idManager.getFacesID(x2+1, y2));
	adj.face.push_back(idManager.getFacesID(x2, y2-1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
}

/*************************************************
 * Label 3 で一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchLabel3(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	/* 面 */
	if(idManager.getFacesStatus(x1-1, y1) != 0) {
		adj.face.push_back(idManager.getFacesID(x1-1, y1));
	} else {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	}
	adj.face.push_back(idManager.getFacesID(x1, y1+1));
	if(idManager.getFacesStatus(x1+1, y1) != 0) {
		adj.face.push_back(idManager.getFacesID(x1+1, y1));
	} else {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	}
	adj.face.push_back(idManager.getFacesID(x2, y2));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
	
	adj.id = idManager.getFacesID(x2, y2);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x2-1, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1));
	adj.face.push_back(idManager.getFacesID(x2+1, y2));
	adj.face.push_back(idManager.getFacesID(x2, y2-1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();

	idManager.setFacesID(x1, y1-1, idManager.getFacesID(x2, y2));
	idManager.setFacesID(x2, y2+1, idManager.getFacesID(x1, y1));
	idManager.setEdgesID(x2*2+1, y2+1, idManager.getEdgesID(x1*2+1, y1));
	idManager.setVerticesID(x2, y2+1, idManager.getVerticesID(x1, y1));
	idManager.setVerticesID(x2+1, y2+1, idManager.getVerticesID(x1+1, y1));
}

/*************************************************
 * Label 4 で一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchLabel4(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x1-1, y1));
	adj.face.push_back(idManager.getFacesID(x1, y1+1));
	adj.face.push_back(idManager.getFacesID(x2, y2));
	if(idManager.getFacesStatus(x1, y1-1) != 0) {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x2-1, y2));
	}
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
	
	adj.id = idManager.getFacesID(x2, y2);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	if(idManager.getFacesStatus(x2+1, y2) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+2));
	}
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	if(idManager.getFacesStatus(x2+1, y2) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1+1));
	}
	adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x2-1, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1));
	if(idManager.getFacesStatus(x2+1, y2) != 0) {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	adj.face.push_back(idManager.getFacesID(x2, y2-1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
}

/*************************************************
 * Label 5 で一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchLabel5(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x1-1, y1));
	adj.face.push_back(idManager.getFacesID(x1, y1+1));
	adj.face.push_back(idManager.getFacesID(x2, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1-1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
	
	adj.id = idManager.getFacesID(x2, y2);
	/* 頂点 */
	if(idManager.getFacesStatus(x2, y2-1) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+2));
	}
	if(idManager.getFacesStatus(x2, y2+1) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1-1));
	}
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	if(idManager.getFacesStatus(x2, y2+1) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1-1));
	}
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	if(idManager.getFacesStatus(x2, y2-1) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1+1));
	}
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x2-1, y2));
	if(idManager.getFacesStatus(x2, y2+1) != 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2+1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adj.face.push_back(idManager.getFacesID(x1, y1));
	if(idManager.getFacesStatus(x2, y2-1) != 0) {
		adj.face.push_back(idManager.getFacesID(x2, y2-1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1+1));
	}
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
}

/*************************************************
 * Label 6 で一致する面を隣接リストに格納するメンバ関数
 *************************************************/
void UpdateID::matchLabel6(int x1, int y1, int x2, int y2, vector<AdjacentList>& adjLists) {
	AdjacentList adj;
	adj.id = idManager.getFacesID(x1, y1);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1, y1));
	adj.vertex.push_back(idManager.getVerticesID(x1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x1*2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1+1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	adj.edge.push_back(idManager.getEdgesID(x1*2+1, y1));
	/* 面 */
	if(idManager.getFacesID(x1-1, y1) != idManager.getFacesID(x1, y1)) {
		adj.face.push_back(idManager.getFacesID(x1-1, y1));
	} else {
		adj.face.push_back(idManager.getFacesID(x1-1, y1+1));
	}
	adj.face.push_back(idManager.getFacesID(x1, y1+1));
	adj.face.push_back(idManager.getFacesID(x2, y2));
	adj.face.push_back(idManager.getFacesID(x1, y1-1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
	
	adj.id = idManager.getFacesID(x2, y2);
	/* 頂点 */
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1+1));
	adj.vertex.push_back(idManager.getVerticesID(x2, y2+1));
	if(idManager.getFacesStatus(x2+1, y2) != 0) {
		adj.vertex.push_back(idManager.getVerticesID(x2+1, y2+1));
	} else {
		adj.vertex.push_back(idManager.getVerticesID(x1+1, y1-1));
	}
	adj.vertex.push_back(idManager.getVerticesID(x1+1, y1));
	/* 辺 */
	adj.edge.push_back(idManager.getEdgesID(x2*2, y2));
	adj.edge.push_back(idManager.getEdgesID(x2*2+1, y2+1));
	if(idManager.getFacesStatus(x2+1, y2) != 0) {
		adj.edge.push_back(idManager.getEdgesID(x2*2+2, y2));
	} else {
		adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1-1));
	}
	adj.edge.push_back(idManager.getEdgesID(x1*2+2, y1));
	/* 面 */
	adj.face.push_back(idManager.getFacesID(x2-1, y2));
	adj.face.push_back(idManager.getFacesID(x2, y2+1));
	if(idManager.getFacesStatus(x2+1, y2) != 0) {
		adj.face.push_back(idManager.getFacesID(x2+1, y2));
	} else {
		adj.face.push_back(idManager.getFacesID(x1, y1-1));
	}
	adj.face.push_back(idManager.getFacesID(x1, y1));
	adjLists.push_back(adj);
	adj.vertex.clear(); adj.edge.clear(); adj.face.clear();
}

/*************************************************
 * 境界の面・辺・頂点 ID を更新するメンバ関数
 *************************************************/
void UpdateID::getState1AdjacentLists(vector<AdjacentList>& adjLists, vector<Correspondence> facesCoordPairs, UnfoldingDimensions unfoldDim) {
	for(int i = 0; i < facesCoordPairs.size(); ++i) {
		int x012 = facesCoordPairs[i].x_first;
		int y012 = facesCoordPairs[i].y_first;
		int x435 = facesCoordPairs[i].x_second;
		int y435 = facesCoordPairs[i].y_second;
		int label = facesCoordPairs[i].label;
	
		if(unfoldDim.a != 0) {
			if(label == 0 || label == 4) {
				matchRotate270(x012, y012, x435, y435, adjLists);
			} 
			else if(facesCoordPairs[i].label == 1 || facesCoordPairs[i].label == 5) {
				matchRotate180(x012, y012, x435, y435, adjLists);
			} 
			else if(facesCoordPairs[i].label == 2 || facesCoordPairs[i].label == 6) {
				matchRotate90(x012, y012, x435, y435, adjLists);
			} 
			else if(facesCoordPairs[i].label == 3){
				matchRotate0(x012, y012, x435, y435, adjLists);
			}
		} 
		else {
			if(label == 0) matchLabel0(x012, y012, x435, y435, adjLists);
			else if(label == 1) matchLabel1(x012, y012, x435, y435, adjLists);
			else if(label == 2) matchLabel2(x012, y012, x435, y435, adjLists);
			else if(label == 3) matchLabel3(x012, y012, x435, y435, adjLists);
			else if(label == 4) matchLabel4(x012, y012, x435, y435, adjLists);
			else if(label == 5) matchLabel5(x012, y012, x435, y435, adjLists);
			else matchLabel6(x012, y012, x435, y435, adjLists);
		}
	}
}

/*************************************************
 * ソート，重複のある組みを削除（id が一致しているかで判定）
 *************************************************/
vector<AdjacentList> UpdateID::deleteSameAdjLists(vector<AdjacentList> adjLists) {
	vector<AdjacentList> deleteSameAdj;
	std::sort(adjLists.begin(), adjLists.end(), [](const AdjacentList &a, const AdjacentList &b) { 
		return a.id < b.id; 
	});
	int id = adjLists[0].id;
	deleteSameAdj.push_back(adjLists[0]);
	for(int i = 0; i < adjLists.size() - 1; ++i){
		if(id == adjLists[i + 1].id) continue;
		deleteSameAdj.push_back(adjLists[i + 1]);
		id = adjLists[i + 1].id;
	}
	return deleteSameAdj;
}

/*************************************************
 * 面の ID を再ラベリング
 *************************************************/
void UpdateID::relabelFacesID(vector<int>& func_f, set<int>& faces, vector<AdjacentList> adjLists) {
	int faces_label = 0;
	for(auto adj: adjLists) {
		func_f[adj.id] = faces_label++;
	}
	for(auto adj: adjLists) {
		for(int i=0; i<4; ++i) {
			faces.insert(adj.face[i]);
		}
	}
}

/*************************************************
 * 頂点の ID を再ラベリング
 *************************************************/
void UpdateID::relabelVerticesID(vector<int>& func_v, set<int>& vertices, vector<AdjacentList> adjLists) {
	for(auto adj: adjLists) {
		for(int i = 0; i < 4; ++i) { 
			vertices.insert(adj.vertex[i]); 
		}
	}
	int vertices_label = 0;
	for(int i : vertices) {
		func_v[i] = vertices_label++;
	}
}

/*************************************************
 * 辺の ID を再ラベリング
 *************************************************/
void UpdateID::relabelEdgesID(vector<vector<int>>& func_e, vector<int> func_v, set<pair<int, int>>& edges, vector<AdjacentList> adjLists) {
	for(auto adj: adjLists) {
		for(int i=0; i<4; ++i) {
			int u = func_v[adj.vertex[i%4]];
			int v = func_v[adj.vertex[(i+1)%4]];
			if(u > v) swap(u, v);
			edges.insert(make_pair(u, v));
		}
	}
	int edges_label = 0;
	for(auto e: edges) func_e[e.first][e.second] = edges_label++;
}

/*************************************************
 * 面・辺・頂点の個数および，辺の両端点のリストを出力
 *************************************************/
void UpdateID::printNumbersAndEdgesList(set<int> faces, set<pair<int, int>> edges, set<int> vertices) {
	cout << "# Faces: " << faces.size() << endl;
	cout << "# Vertices: " << vertices.size() << endl;
	cout << "# Edges: " << edges.size() << endl;
	cout << "\nEdge Lists: " << endl;
	for(auto e: edges) cout << e.first << " " << e.second << endl;
	cout << endl;
}

/*************************************************
 * ラベリングした隣接リストを出力
 *************************************************/
void UpdateID::printLabeledList(vector<AdjacentList> adjLists) {
	for(auto adj: adjLists) {
		cout << "id: " << adj.id << endl;
		cout << "v: " 
			 << adj.vertex[0] << " " 
			 << adj.vertex[1] << " " 
			 << adj.vertex[2] << " " 
			 << adj.vertex[3] << endl;
		cout << "e: " 
			 << adj.edge[0] << " " 
			 << adj.edge[1] << " " 
			 << adj.edge[2] << " " 
			 << adj.edge[3] << endl;
		cout << "f: " 
			 << adj.face[0] << " " 
			 << adj.face[1] << " " 
			 << adj.face[2] << " " 
			 << adj.face[3] << endl;
		cout << endl;
	}
}

/*************************************************
 * 再ラベリングした隣接リストを出力
 *************************************************/
void UpdateID::printRelabeledList(vector<int> func_f, vector<int> func_v, vector<vector<int>> func_e, vector<AdjacentList> adjLists) {
	cout << "# --- #Faces ---" << endl;
	cout << "NF " << adjLists.size() << endl;
	for(auto adj: adjLists) {
		cout << "# --- Face " << func_f[adj.id] << " ---" << endl;
		cout << "N 4" << endl;
		// cout << "V " 
		// 	 << func_v[adj.vertex[0]] << " " 
		// 	 << func_v[adj.vertex[1]] << " " 
		// 	 << func_v[adj.vertex[2]] << " " 
		// 	 << func_v[adj.vertex[3]] << endl;
		cout << "E " ;
		int u, v;
		u = func_v[adj.vertex[0]];
		v = func_v[adj.vertex[1]];
		if(u > v) swap(u, v);
		cout << func_e[u][v] << " ";
		u = func_v[adj.vertex[1]];
		v = func_v[adj.vertex[2]];
		if(u > v) swap(u, v);
		cout << func_e[u][v] << " ";
		u = func_v[adj.vertex[2]];
		v = func_v[adj.vertex[3]];
		if(u > v) swap(u, v);
		cout << func_e[u][v] << " ";
		u = func_v[adj.vertex[3]];
		v = func_v[adj.vertex[0]];
		if(u > v) swap(u, v);
		cout << func_e[u][v] << endl;
		cout << "F " 
			 << func_f[adj.face[0]] << " " 
			 << func_f[adj.face[1]] << " " 
			 << func_f[adj.face[2]] << " " 
			 << func_f[adj.face[3]] << endl;
		// cout << endl;
	}
}

/*************************************************
 * 基準面と転がす方向のペアを全て出力
 *************************************************/
void UpdateID::printBaseFaceBaceEdgePairs(vector<int> func_f, vector<int> func_v, vector<vector<int>> func_e, vector<AdjacentList> adjLists) {
	for(auto adj: adjLists) {
		int u, v;
		u = func_v[adj.vertex[0]];
		v = func_v[adj.vertex[1]];
		if(u > v) swap(u, v);
		cout << func_f[adj.id] << " " << func_e[u][v] << endl;
		u = func_v[adj.vertex[1]];
		v = func_v[adj.vertex[2]];
		if(u > v) swap(u, v);
		cout << func_f[adj.id] << " " << func_e[u][v] << endl;
		u = func_v[adj.vertex[2]];
		v = func_v[adj.vertex[3]];
		if(u > v) swap(u, v);
		cout << func_f[adj.id] << " " << func_e[u][v] << endl;
		u = func_v[adj.vertex[3]];
		v = func_v[adj.vertex[0]];
		if(u > v) swap(u, v);
		cout << func_f[adj.id] << " " << func_e[u][v] << endl;
	}
}

/***************
 * public 関数
 ***************/

/*************************************************
 * 隣接リストを求めるメンバ関数
 *************************************************/
void UpdateID::getAdjacentList(vector<AdjacentList>& adjLists, UnfoldingDimensions unfoldDim) {
	/* 面どうしの対応関係を求める */
	pair<vector<int>, vector<int>> facesPairs = getFacesPairs(unfoldDim);
	/* 面の座標どうしの対応関係を求め，435面側を更新 */
	vector<Correspondence> facesCoordPairs = getFacesCoordPairs(facesPairs, unfoldDim);
	if(unfoldDim.a != 0) overwriteFaces435(facesPairs, unfoldDim);
	/* 頂点の座標どうしの対応関係を求め，435面側を更新 */
	vector<Correspondence> verticesCoordPairs = getVerticesCoordPairs(unfoldDim);
	overwriteVertices435(verticesCoordPairs);
	/* 状態が 9 の面における隣接リストを取得 */
	vector<Coordinates> coordinates = getState9FacesCoordinates();
	getState9AdjacentLists(adjLists, coordinates);
	/* a = 0 の時に，辺の座標どうしの対応関係を求め，435面側を更新 */
	vector<Correspondence> edgesCoordPairs;
	if(unfoldDim.a == 0) {
		vector<Correspondence> edgesCoordPairs = getEdgesCoordPairs(unfoldDim);
		overwriteEdges435(edgesCoordPairs);
		setFacesRoundCoordPairs(facesCoordPairs, unfoldDim);
	}
	/* 状態が 1 の面における隣接リストを取得 */
	getState1AdjacentLists(adjLists, facesCoordPairs, unfoldDim);
}

/*************************************************
 * 元のラベリングでの隣接リストを出力するメンバ関数
 *************************************************/
void UpdateID::printOriginalAdjacentList(vector<AdjacentList> adjLists) {
	cout << "------- Original Adjacent List -------" << endl;

	adjLists = deleteSameAdjLists(adjLists);
	set<int> faces;
	set<pair<int, int>> edges;
	set<int> vertices;

	for(auto adj: adjLists) {
		for(int i=0; i<4; ++i) {
			faces.insert(adj.face[i]);
			int u = adj.vertex[i%4];
			int v = adj.vertex[(i+1)%4];
			if(u > v) swap(u, v);
			edges.insert(make_pair(u, v));
			vertices.insert(u);
			vertices.insert(v);
		}
	}

	printLabeledList(adjLists);
	printNumbersAndEdgesList(faces, edges, vertices);
}

/*************************************************
 * 再ラベリングした後の隣接リストを出力するメンバ関数
 *************************************************/

void UpdateID::printAdjacentList(vector<AdjacentList> adjLists) {
	// cout << "------- Relabeled Adjacent List -------" << endl;

	adjLists = deleteSameAdjLists(adjLists);
	set<int> faces;
	set<pair<int, int>> edges;
	set<int> vertices;

	/* Faces ID の再ラベリング */
	int num_f = idManager.getFacesID(idManager.getFacesIDsize().row-1, idManager.getFacesIDsize().column-1);
	vector<int> func_f(num_f, -1);
	relabelFacesID(func_f, faces, adjLists);

	/* Vertices ID の再ラベリング */
	int num_v = num_f;
	vector<int> func_v(num_v, -1);
	relabelVerticesID(func_v, vertices, adjLists);

	/* Edges ID の再ラベリング */
	int num_e = vertices.size();
	vector<vector<int>> func_e(num_e, vector<int>(num_e, -1));
	relabelEdgesID(func_e, func_v, edges, adjLists);
	
	// printRelabeledList(func_f, func_v, func_e, adjLists);
	printNumbersAndEdgesList(faces, edges, vertices);
	// printBaseFaceBaceEdgePairs(func_f, func_v, func_e, adjLists);


	/* 写像の関係を出力 */
	// cout << "func_f :" << endl;
	// for(int i = 0; i < num_f; ++i){
	// 	if(func_f[i] != -1) {
	// 		cout << i << " -> " << func_f[i] << endl;
	// 	}
	// }
	// vector<int> func_v_inv(vertices.size());
	// cout << "\nfunc_v :" << endl;
	// for(int i = 0; i < num_v; ++i){
	// 	if(func_v[i] != -1) {
	// 		cout << i << " -> " << func_v[i] << endl;
	// 		func_v_inv[func_v[i]] = i;
	// 	}
	// }
	// cout << "\nfunc_e :" << endl;
	// for(int i = 0; i < num_e; ++i){
	// 	for(int j = 0; j < num_e; ++j) {
	// 		if(func_e[i][j] != -1) {
	// 			cout << "(" << func_v_inv[i] << ", " << func_v_inv[j] << ") -> " << "(" << i << ", " << j << ") -> " << func_e[i][j] << endl;
	// 		}
	// 	}
	// }
}
