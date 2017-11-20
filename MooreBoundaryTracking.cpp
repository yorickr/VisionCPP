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
bool pixelInContour(vector<vector<Point>> &contours, Point pixel);
void getContour(Mat binaryImage, vector<Point> &contour, Point start);

bool min_and_max_y(const cv::Point& lhs, const cv::Point& rhs)
{
	return lhs.y < rhs.y;
}

bool min_and_max_x(const cv::Point& lhs, const cv::Point& rhs)
{
	return lhs.x < rhs.x;
}

int allContours(Mat binaryImage, vector<vector<Point>>& contourVecVec)
{
	std::cout << "Rows:" << binaryImage.rows << ", Columns:" << binaryImage.cols << std::endl;

	vector<Point> allContourPoints;
	for (int r = 0; r < binaryImage.rows; r++) {
		for (int c = 0; c < binaryImage.cols; c++) {			
			Point pixel(c, r);
			bool pixelIs1 = binaryImage.at<__int16>(pixel) == 1;
			bool pixelIsInAContour = pixelInContour(contourVecVec, pixel);

			//std::cout << "Pixel: (" << c << ", " << r << ") is 1: " << pixelIs1 << std::endl;
			if (pixelIs1 && !pixelIsInAContour) {
				vector<Point> contour;
				getContour(binaryImage, contour, pixel);
				if (contour.size() >= 4) {
					contourVecVec.push_back(contour);
				}				
			}
		}
	}

	/*vector<Point> contour;
	getContour(binaryImage, contour, Point(1, 1));
	contourVecVec.push_back(contour);*/

	return (int)contourVecVec.size();
}

pair<Point, Point> getMinMaxY(const vector<Point> &contour) {
	auto min_max_y_it = minmax_element(contour.begin(), contour.end(), min_and_max_y);
	auto min_y = contour[min_max_y_it.first - contour.begin()];
	auto max_y = contour[min_max_y_it.second - contour.begin()];
	return pair<Point, Point>(min_y, max_y);
}

pair<Point, Point> getMinMaxXAtSameY(const vector<Point> &contour, Point pixel) {
	vector<Point> contourPointsAtSameY;
	copy_if(contour.begin(), contour.end(), back_inserter(contourPointsAtSameY), [pixel](Point contourPixel) { return contourPixel.y == pixel.y; });

	auto min_max_x_it = minmax_element(contourPointsAtSameY.begin(), contourPointsAtSameY.end(), min_and_max_x);
	auto min_x = contourPointsAtSameY[min_max_x_it.first - contourPointsAtSameY.begin()];
	auto max_x = contourPointsAtSameY[min_max_x_it.second - contourPointsAtSameY.begin()];
	return pair<Point, Point>(min_x, max_x);
}

bool pixelInContour(vector<vector<Point>> &contours, Point pixel) {
	/*if (contours.empty()) return false;

	for (auto const &contour : contours) {
		auto min_max_y = getMinMaxY(contour);
		
		if (pixel.y > min_max_y.first.y && pixel.y < min_max_y.second.y) {
			auto min_max_x_at_same_y = getMinMaxXAtSameY(contour, pixel);
			if (min_max_x_at_same_y.first.x < pixel.x && pixel.x < min_max_x_at_same_y.second.x) {
				return true;
			}
		}
	}

	return false;*/

	for (auto const &contour : contours) {
		auto result = pointPolygonTest(contour, pixel, false);
		if (result >= 0) return true;
	}

	return false;
}

void getContour(Mat binaryImage, vector<Point> &contour, Point start) {
	//Firstb0 is start
	Directions directions;
	Point b0 = start; //uppermost-leftmost bit of boundary 
	Point c0 = b0 + Point(-1,0); //west-neighbour �0� pixel of b0 

	/*std::cout << "b0: " << b0 << std::endl;
	std::cout << "c0: " << c0 << std::endl;
*/
	//Store Firstb0 = b0
	Point Firstb0 = b0;
	contour.push_back(b0);

	Point c1(0,0); //c1 := preceding �0� of b1

	Point b1 = firstNeighbour1pixel(binaryImage, b0, c0, &c1); //Rotate clockwise starting at c0 -> b1 := first neighbour �1� pixel of b0

	/*std::cout << "b1: " << b1 << std::endl;
	std::cout << "c1: " << c1 << std::endl;*/

	//Store Firstb1 = b1
	contour.push_back(b1);
	Point Firstb1 = b1;

	bool loop = true;

	while (loop) {
		b0 = b1;
		c0 = c1;

		b1 = firstNeighbour1pixel(binaryImage, b0, c0, &c1); //Rotate clockwise starting at c0 -> b1 := first neighbour �1� pixel of b0
		/*std::cout << "I: " << iteration
		<< "=> b" << iteration - 1	<< ": " << b0
		<< "=> c" << iteration - 1	<< ": " << c0
		<< "=> b" << iteration		<< ": " << b1
		<< "=> c" << iteration		<< ": " << c1 << std::endl;*/
		/*std::cout << "I: " << iteration
		<< "=> FirstB0: " << Firstb0
		<< "=> FirstB1: " << Firstb1
		<< "=> b0: " << b0
		<< "=> b1: " << b1 << std::endl; */		
		/*std::cout << "I: " << iteration << std::endl;
		std::cout << "Firstb0 is here again => " << b0 << " == " << Firstb0 << ": " << (b0 == Firstb0) << std::endl;
		std::cout << "Firstb1 is here again: " << (b1 == Firstb1) << std::endl;
		std::cout << "So loop again" << !(b0 == Firstb0 && b1 == Firstb1) << std::endl;*/

		if (!(b0 == Firstb0 && b1 == Firstb1))
			/*if (!(find(contour.begin(), contour.end(), b1) != contour.end())) {
				contour.push_back(b1);
			}*/
			contour.push_back(b1);
		else {
			loop = false;
			//std::cout << "Done" << std::endl;
		}
	}
}

Point firstNeighbour1pixel(Mat binaryImage, Point b0, Point c0, Point* c1) {
	Directions directions;
	int startPointClockIndex = directions.getClockDirectionIndex(directions.clockWise, c0 - b0); //get direction on clock
	int previousPointClockIndex = startPointClockIndex;
	int nextPointClockIndex = startPointClockIndex;
	Point nextPosition = Point();
	
	//std::cout << "\nStart " << startPointClockIndex << std::endl;

	for (int i = 1; i < directions.clockWise.size(); i++) {
		previousPointClockIndex = nextPointClockIndex;
		nextPointClockIndex = (startPointClockIndex + i) % directions.clockWise.size();
		
		/*std::cout << "I: " << i
		<< "=> Previous: " << previousPointClockIndex
		<< ", Next: " << nextPointClockIndex;*/

		nextPosition = b0 + directions.clockWise[nextPointClockIndex];
		//std::cout << "=> Next position: " << nextPosition << std::endl;
		if (binaryImage.at<_int16>(nextPosition) == 1) {
			Point preceding0(b0 + directions.clockWise[previousPointClockIndex]);
			//std::cout << "Found next 1, so preceding 0 has position" << preceding0 << std::endl;
			c1->x = preceding0.x;
			c1->y = preceding0.y;
			return nextPosition;
		}
	}

	return b0;
}