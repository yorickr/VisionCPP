#include "MooreBoundaryTracking.h"
#include <algorithm>

struct Directions {
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
			clockWise.push_back(left_bottom);	}

		int getClockDirectionIndex(vector<Point> clockDirection, Point direction) {
			for (int i = 0; i < clockDirection.size(); i++) {
				if (clockWise[i].x == direction.x &&
					clockWise[i].y == direction.y) {
					return i;
				}
			}

			return -1;
		}
};

Point firstNeighbour1pixel(Mat binaryImage, Point b0, Point c0, Point* c1);
bool pixelInOneOfTheContours(vector<vector<Point>> &contours, Point pixel);
void getContour(Mat binaryImage, vector<Point> &contour, Point start);

int allContours(Mat binaryImage, vector<vector<Point>>& contourVecVec)
{	
	for (int r = 0; r < binaryImage.rows; r++) {
		for (int c = 0; c < binaryImage.cols; c++) {			
			Point pixel(c, r);
			if (binaryImage.at<__int16>(pixel) != 1) continue;
			
			if (!pixelInOneOfTheContours(contourVecVec, pixel)) {
				vector<Point> contour;
				getContour(binaryImage, contour, pixel);
				if (contour.size() >= 4) {
					contourVecVec.push_back(contour);
				}				
			}
		}
	}

	return (int)contourVecVec.size();
}

bool pixelInOneOfTheContours(vector<vector<Point>> &contours, Point pixel) {
	if (contours.empty()) return false;

	for (auto const &contour : contours) {			
		/*auto result = pointPolygonTest(contour, pixel, false);
		if (result >= 0) return true;*/

		//self-made point in polygon function
		if (pixelInContour(contour, pixel)) return true;
	}

	return false;
}

void getContour(Mat binaryImage, vector<Point> &contour, Point start) {
	//uppermost-leftmost bit of boundary 
	Point b0 = start; 
	//west-neighbour “0” pixel of b0 
	Point c0 = b0 + Point(-1,0); 
	
	//Store Firstb0 = b0
	Point Firstb0 = b0;
	contour.push_back(b0);

	//c1 := preceding “0” of b1
	Point c1(0,0); 

	//Rotate clockwise starting at c0 -> b1 := first neighbour “1” pixel of b0
	Point b1 = firstNeighbour1pixel(binaryImage, b0, c0, &c1); 

	//Store Firstb1 = b1
	contour.push_back(b1);
	Point Firstb1 = b1;

	bool loop = true;

	while (loop) {
		b0 = b1;
		c0 = c1;

		//Rotate clockwise starting at c0 -> b1 := first neighbour “1” pixel of b0
		b1 = firstNeighbour1pixel(binaryImage, b0, c0, &c1); 
		
		if (!(b0 == Firstb0 && b1 == Firstb1))			
			contour.push_back(b1);
		else {
			loop = false;
		}
	}
}

Point firstNeighbour1pixel(Mat binaryImage, Point b0, Point c0, Point* c1) {
	Directions directions;
	//get direction on clock
	int startPointClockIndex = directions.getClockDirectionIndex(directions.clockWise, c0 - b0); 
	int previousPointClockIndex = startPointClockIndex;
	int nextPointClockIndex = startPointClockIndex;
	Point nextPosition = Point();

	for (int i = 1; i < directions.clockWise.size(); i++) {
		previousPointClockIndex = nextPointClockIndex;
		nextPointClockIndex = (startPointClockIndex + i) % directions.clockWise.size();	

		nextPosition = b0 + directions.clockWise[nextPointClockIndex];		

		//check if next position is inside binaryimage
		if (nextPosition.x < 0 || nextPosition.x >= binaryImage.cols || 
			nextPosition.y < 0 || nextPosition.y >= binaryImage.rows) 
			continue;

		if (binaryImage.at<_int16>(nextPosition) == 1) {
			Point preceding0(b0 + directions.clockWise[previousPointClockIndex]);
			
			c1->x = preceding0.x;
			c1->y = preceding0.y;
			return nextPosition;
		}
	}

	return b0;
}

///*
//return true, if the pixel is in the contour
//Use the ray-casting algorithm, to see if pixel is in the contour.
//*/
//bool pixelInContour(const vector<Point>& contour, Point pixel)
//{
//	//check if pixel lies on contour
//	if (find(contour.begin(), contour.end(), pixel) != contour.end()) {
//		return true;
//	}
//	
//	// if pixel never cross line, pixel is lies inside the contour
//	bool inside = false;
//	
//	for (int i = 0; i < contour.size(); ++i) {
//
//		//i is the first point and j is the next one.
//		int j = (i + 1) % contour.size();
//
//		Point first = contour[i];
//		Point second = contour[j];
//		
//		//check if the pixel crosses the horizontale line between the y coordinates of first and second point.
//		if ((first.y <= pixel.y) && (second.y > pixel.y) || (second.y <= pixel.y) && (first.y > pixel.y)) {
//			
//			double xCross = (second.x - first.x) * (pixel.y - first.y) / (second.y - first.y) + first.x;
//
//			if (xCross < pixel.x)
//				inside = !inside;
//		}
//	}
//
//	return inside;
//}