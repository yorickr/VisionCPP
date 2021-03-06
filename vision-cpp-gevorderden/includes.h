#ifndef INCLUDES_H
#define INCLUDES_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <map>
#include <set>

#include <stdint.h>

#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

#if defined(__APPLE__) || defined(__linux__)
typedef int16_t _int16; // want avans gebruikt types die niet in stdint.h zijn gedefined..
#endif

#include "avansvisionlib.h"

#endif
