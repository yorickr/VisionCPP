#pragma once

#include "includes.h"

int vcpp1_main(int argc, char** argv);
double bendingEnergy(Mat binaryImage, vector<Point> & contourVec);
int allBoundingBoxes(const vector<vector<Point>> & contours, vector<vector<Point>> & bbs);
