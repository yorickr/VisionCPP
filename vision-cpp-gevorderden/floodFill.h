#ifndef FLOODFILL_H
#define FLOODFILL_H

#include "includes.h"
#include "avansvisionlib.h"

// func: finds all coordinates enclosed by a contour
// pre: contourVec contains the points of a contour (=boundary).
// post: regionPixels contains all the enclosedPixels inclusive the contourpixels
// return_value: the total number of pixels (the total area).
// evaluation: this function uses the Boundary Fill or Flood Fill algorithm
int enclosedPixels(const vector<Point> & contourVec, vector<Point> & regionPixels);

#endif // !FLOODFILL_H
