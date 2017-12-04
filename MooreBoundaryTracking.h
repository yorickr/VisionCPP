#ifndef MOOREBOUNDARYTRACKING
#define MOOREBOUNDARYTRACKING

#include "inleiding-vision-cpp\includes.h"

int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec);
bool pixelInContour(const vector<Point> &contour, Point pixel);

#endif

