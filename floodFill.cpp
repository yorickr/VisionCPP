#define _USE_MATH_DEFINES
#include "floodFill.h"
#include "Utils.h"

bool less_by_x(const cv::Point& lhs, const cv::Point& rhs)
{
	return lhs.x < rhs.x;
}

bool less_by_y(const cv::Point& lhs, const cv::Point& rhs)
{
	return lhs.y < rhs.y;
}

void floodFillStep(vector<vector<int>> &grid, Point startPoint, int stepFromStartPoint);
void makeGridDoubleBoundary(vector<vector<int>> &grid, const vector<Point> &contour);
void changeCellValue(vector<vector<int>>& grid, Point &cell, int cellValue);
int getCellValue(const vector<vector<int>>& grid, const Point &cell);
bool cellFitInGrid(const vector<vector<int>>& grid, const Point &cell);

int enclosedPixels(const vector<Point> & contourVec, vector<Point> & regionPixels) {
	std::copy(contourVec.begin(), contourVec.end(), back_inserter(regionPixels));

	//calculate min and max, x and y values
	auto mmx = std::minmax_element(contourVec.begin(), contourVec.end(), less_by_x);
	auto mmy = std::minmax_element(contourVec.begin(), contourVec.end(), less_by_y);
	
	const int width = mmx.second->x - mmx.first->x + 1;
	const int height = mmy.second->y - mmy.first->y + 1;
	Point translationVector(mmx.first->x, mmy.first->y);

	//make boundary box grid
	vector<vector<int>> grid(height);

	for (auto &columns : grid) {
		columns = vector<int>(width);
	}

	for (auto &columns : grid) {
		for (auto &cell : columns) {
			cell = -1;
		}
	}

	vector<Point> boundaryBoxContour;
	//make contour points 100;
	for (auto const &contour : contourVec) {
		int x = contour.x - translationVector.x;
		int y = contour.y - translationVector.y;
		/*if (x < 0 || x >= grid[y].size() || y < 0 || y >= grid.size()) {
			cout << "X,Y outside of the grid" << x << ", " << y << endl;
		} */

		boundaryBoxContour.push_back(Point(x, y));
		//cout << "Last placed y and x: " << y << ", " << x << endl;
		grid[y][x] = 100;
	}

	//make the contour water proof.
	makeGridDoubleBoundary(grid, boundaryBoxContour);

	Point startPoint(3, 3);
	
	//define a startpoint
	bool loop = true;
	bool pointInC = false;
	bool pointInBoundary = true;
	while (loop) {		
		startPoint.x = rand() % (mmx.second->x - mmx.first->x) + mmx.first->x;
		startPoint.y = rand() % (mmy.second->y - mmy.first->y) + mmy.first->y;

		pointInC = pixelInContour(boundaryBoxContour, startPoint);
		pointInBoundary = elementInVector(boundaryBoxContour, startPoint);
		loop = !(pointInC && !pointInBoundary);
	}

	cout << "start point found: " << startPoint << endl;

	grid[startPoint.y][startPoint.x] = 0;

	floodFillStep(grid, startPoint, 1);

	for (int r = 0; r < grid.size(); r++) {
		for (int c = 0; c < grid[r].size(); c++) {
			auto cell = grid[r][c];
			if (0 <= cell && cell < 100) {
				regionPixels.push_back(Point(c, r) + translationVector);
			}
		}
	}
	
	return regionPixels.size();
}

vector<Point> directions = { {-1, 0}, {-1, -1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1} };
/*
	grid, with the contour of values 100
	startPoints lies i the contour.

*/
void floodFillStep(vector<vector<int>> &grid, Point startPoint, int stepFromStartPoint) {
	vector<Point> foundPoints = { startPoint }, nextStepPoints;

	do {
		std::copy(foundPoints.begin(), foundPoints.end(), back_inserter(nextStepPoints));
		foundPoints.clear();

		for (auto const &point : nextStepPoints) {
			for (auto const &dir : directions) {
				auto nextStep = point + dir;
				if (!cellFitInGrid(grid, nextStep)) continue;
				auto cellValue = getCellValue(grid, nextStep);

				if (cellValue == -1) {
					foundPoints.push_back(nextStep);
					changeCellValue(grid, nextStep, stepFromStartPoint);
				}
			}
		}

		nextStepPoints.clear();
	} while (foundPoints.size() > 0);
}

float angle = 90 / 180.0 * M_PI;

void changeCellValue(vector<vector<int>>& grid, Point &cell, int cellValue) {
	if (!cellFitInGrid(grid, cell)) return;
	grid[cell.y][cell.x] = cellValue;
}

int getCellValue(const vector<vector<int>>& grid, const Point & cell)
{
	if (!cellFitInGrid(grid, cell)) throw out_of_range("Cell out of range");
	return grid[cell.y][cell.x];
}

bool cellFitInGrid(const vector<vector<int>>& grid, const Point & cell)
{
	if (cell.y < 0 || cell.y >= grid.size()) return false;
	if (cell.x < 0 || cell.x >= grid[cell.y].size()) return false;
	return true;
}

void makeGridDoubleBoundary(vector<vector<int>>& grid, const vector<Point>& contour)
{
	Point translation, newBoundaryTranslation, newBoundary1, newBoundary2;
	int j;
	int x = 0, y = 0;
	for (int i = 0; i < contour.size(); i++) {
		j = (i + 1) % contour.size();

		//substract next and current point to get the translation vector from the current to the next point.
		translation = contour[j] - contour[i];
		
		//rotate the translation vector
		newBoundaryTranslation.x = translation.x * cos(angle) - translation.y * sin(angle);
		newBoundaryTranslation.y = translation.x * sin(angle) + translation.y * cos(angle);

		newBoundary1 = contour[i] + newBoundaryTranslation;
		newBoundary2 = contour[j] + newBoundaryTranslation;

		changeCellValue(grid, newBoundary1, 100);
		changeCellValue(grid, newBoundary2, 100);
	}
}
