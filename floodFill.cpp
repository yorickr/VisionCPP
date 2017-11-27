#include "floodFill.h"

int enclosedPixels(const vector<Point> & contourVec, vector<Point> & regionPixels) {
	std::copy(contourVec.begin(), contourVec.end(), back_inserter(regionPixels));

	return regionPixels.size();
}