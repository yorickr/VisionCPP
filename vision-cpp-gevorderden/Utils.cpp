#include "Utils.h"

/*
return true, if the pixel is in the contour
Use the ray-casting algorithm, to see if pixel is in the contour.
If points is a contour pixel also return true;
*/
bool pixelInContour(const std::vector<cv::Point>& contour, cv::Point pixel)
{	
	//check if pixel lies on contour
	if (find(contour.begin(), contour.end(), pixel) != contour.end()) {
		return true;
	}	

	// if pixel never cross line, pixel is lies inside the contour
	bool inside = false;

	for (int i = 0; i < contour.size(); ++i) {

		//i is the first point and j is the next one.
		int j = (i + 1) % contour.size();

		cv::Point first = contour[i];
		cv::Point second = contour[j];

		//check if the pixel crosses the horizontale line between the y coordinates of first and second point.
		if ((first.y <= pixel.y) && (second.y > pixel.y) || (second.y <= pixel.y) && (first.y > pixel.y)) {

			double xCross = (second.x - first.x) * (pixel.y - first.y) / (second.y - first.y) + first.x;

			if (xCross < pixel.x)
				inside = !inside;
		}
	}

	return inside;
}