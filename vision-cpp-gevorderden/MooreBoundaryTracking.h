#pragma once

#include "includes.h"

class Directions {
	public:
		Point left = Point(-1, 0);
		Point left_top = Point(-1, -1);
		Point top = Point(0, -1);
		Point right_top = Point(1, -1);
		Point right = Point(1, 0);
		Point right_bottom = Point(1, 1);
		Point bottom = Point(0, 1);
		Point left_bottom = Point(-1, 1);
		vector<Point> clockWise;

		Directions() {
			clockWise.push_back(left);
			clockWise.push_back(left_top);
			clockWise.push_back(top);
			clockWise.push_back(right_top);
			clockWise.push_back(right);
			clockWise.push_back(right_bottom);
			clockWise.push_back(bottom);
			clockWise.push_back(left_bottom);
        }

		int getClockDirectionIndex(Point direction) {
			for (int i = 0; i < this->clockWise.size(); i++) {
				if (clockWise[i].x == direction.x &&
					clockWise[i].y == direction.y) {
					return i;
				}
			}

			return -1;
		}
};

int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec);
