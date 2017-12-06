#ifndef UTILS_H
#define UTILS_H

#include "includes.h"
#include <vector>

bool pixelInContour(const std::vector<cv::Point>& contour, cv::Point pixel);

template <typename T>
bool elementInVector(const std::vector<T> vector, T element);

#endif

template<typename T>
inline bool elementInVector(const std::vector<T> vector, T element)
{
	return find(vector.begin(), vector.end(), element) != vector.end();
}
