#include "opg4_2.h"

bool sortFunc(tuple<int, int, int, vector<Point>, Point, Point> left, tuple<int, int, int, vector<Point>, Point, Point> right) {
	return get<1>(left) < get<1>(right);
}

// Find max and min values for a given vector of Point, returns tuple where 1 is min, 2 is max, 3 is the vector<Point> set
vector<tuple<int, int, int, vector<Point>, Point, Point>> findMaxAndMin(const vector<vector<Point>> &contours, bool y = false) {
	vector<tuple<int, int, int, vector<Point>, Point, Point>> temp;
	for(int i = 0; i != contours.size(); i++) {
		// find max and min
		int min = numeric_limits<int>::max(), max = numeric_limits<int>::min();
		Point pMin, pMax;
		for (auto const& point : contours[i]) {
			int value;
			if (y) {
				value = point.y;
			} else {
				value = point.x;
			}
			if (value < min) {
				min = value;
				pMin = point;
			}
			if (value > max) {
				max = value;
				pMax = point;
			}
		}
		// cout << "Min is " << min << endl;
		// cout << "Max is " << max << endl;
		// cout << "I is " << i << endl;
		tuple<int, int, int, vector<Point>, Point, Point> tup = make_tuple(i, min, max, contours[i], pMin, pMax);
		temp.push_back(tup);
	}
	return temp;
}

void findTopAndBot(const vector<vector<Point>> contours, vector<vector<Point>> &inputVector) {
	vector<tuple<int, int, int, vector<Point>, Point, Point>> temp = findMaxAndMin(contours, true);
	sort(temp.begin(), temp.end(), sortFunc);
	inputVector.push_back(get<3>(temp.front()));
	inputVector.push_back(get<3>(temp.back()));

}

float calculatePixelToUnitRatio(const vector<Point> &vec, const Mat &img) {
	vector<vector<Point>> temp;
	temp.push_back(vec);
	// find max and min for x values to find the left and right side of the line
	vector<tuple<int, int, int, vector<Point>, Point, Point>> tmp = findMaxAndMin(temp);
	// find the lowest y value
	vector<tuple<int, int, int, vector<Point>, Point, Point>> tmp2 = findMaxAndMin(temp, true);
	tuple<int, int, int, vector<Point>, Point, Point> element = tmp.at(0);
	tuple<int, int, int, vector<Point>, Point, Point> element2 = tmp2.at(0);
	// cout << "Min " << get<1>(element) << endl;
	// cout << "Max " << get<2>(element) << endl;
	Point pMin, pMax, pMin2, pMax2;
	pMin = get<4>(element);
	pMax = get<5>(element);
	pMin2 = get<4>(element2);
	pMax2 = get<5>(element2);
	// cout << "MinPoint " << pMin << endl;
	// cout << "MaxPoint " << pMax << endl;
	// cout << "MinPoint2 " << pMin2 << endl;
	// cout << "MaxPoint2 " << pMax2 << endl;

	float dis = norm(pMin - pMin2); // in pixels
	float realDistance = 5.7f;
	float disPerPixel = realDistance/dis;
	cout << "Distance " << dis << endl;
	cout << "DistancePerPixel " << disPerPixel << endl;

	int thickness = 2;
	int lineType = 8;
	line( img,
		pMin,
		pMax,
		Scalar( 255, 255, 255 ),
		thickness,
		lineType );
	line( img,
		pMin2,
		pMax2,
		Scalar( 255, 255, 255 ),
		thickness,
		lineType );
	line( img,
		pMin,
		pMin2,
		Scalar( 255, 255, 255 ),
		thickness,
		lineType );
	return disPerPixel;
}

void calculateActualSizeOfRoundObject(const vector<Point> &obj, float disPerPixel, Mat &img) {
	vector<vector<Point>> temp;
	temp.push_back(obj);
	// find max and min for x values to find the left and right side of the line
	vector<tuple<int, int, int, vector<Point>, Point, Point>> tmp = findMaxAndMin(temp);
	tuple<int, int, int, vector<Point>, Point, Point> element = tmp.at(0);
	Point pMin, pMax;
	pMin = get<4>(element);
	pMax = get<5>(element);

	cout << "The size of the round object is " << norm(pMin - pMax)*disPerPixel << " CM" << endl;

	int thickness = 2;
	int lineType = 8;
	line( img,
		pMin,
		pMax,
		Scalar( 0, 255, 0 ),
		thickness,
		lineType );

}

void calculateSizeOfRoundObject(Mat frame) {
	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(frame, gray_image, CV_BGR2GRAY);

	// Filter ruis weg
	Mat result;
	Mat kernel = (Mat_<double>(5, 5) << 2, 4, 5, 4, 2, 4, 9, 12, 9, 4, 5, 12, 15, 12, 5, 4, 9, 12, 9, 4, 2, 4, 5, 4, 2);
	filter2D(gray_image, result, -1, kernel * (1.0f / 115), Point(-1, -1), 0, BORDER_REPLICATE);


	// Het tonen van grijswaarde beeld
	imshow("Grey image", gray_image);

	Mat tresholdImage;
	// Mat binaryImage;
	const int tresh_value = 200;
	threshold(result, tresholdImage, tresh_value, 255, THRESH_BINARY);
	// //debugging
	// threshold(result, binaryImage, tresh_value, 1, THRESH_BINARY);

	// imshow("Treshold image", tresholdImage);

	// Mat binary16S;
	// binaryImage.convertTo(binary16S, CV_16S);
	// Mat labeledImage;
	// cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;
	// show16SImageStretch(labeledImage, "Blob image");

	RNG rng(12345);

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Canny(tresholdImage, canny_output, tresh_value, tresh_value*2, 3 );
	dilate(canny_output, canny_output, Mat());
	erode(canny_output, canny_output, Mat());
	findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
	imshow("Thresh", canny_output);
	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for( size_t i = 0; i< contours.size(); i++ ) {
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		// waitKey(0);
		// cout << "Area is " << contourArea(contours[i]) << endl;
		drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
		// imshow("Contours", drawing);
	}
	// imshow("Contours", drawing);
	vector<vector<Point>> vec; // front is highest, back is lowest
	findTopAndBot(contours, vec);
	// cout << "Highest " << contourArea(vec.front()) << endl;
	// cout << "Lowest " << contourArea(vec.back()) << endl;
	drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (vector<int>::size_type i = 0; i != vec.size(); i++) {
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
	}
	float disPerPixel = calculatePixelToUnitRatio(vec.back(), drawing);
	calculateActualSizeOfRoundObject(vec.front(), disPerPixel, drawing);
	imshow("Contours 2", drawing);

}

int opg4_2(int argc, char** argv) {
	// // YML-file met callibratie data openen
	// FileStorage fs("C:\\School\\ueyecallib.yml", FileStorage::READ);
	//
	// // callibratie data ophalen
	// Mat intrinsic, distCoeffs;
	// fs["intrinsic"] >> intrinsic;
	// fs["distCoeffs"] >> distCoeffs;
	//
	// // callibratie matrices tonen op het scherm
	// cout << "intrinsic matrix: " << intrinsic << endl;
	// cout << "distortion coeffs: " << distCoeffs << endl;
	//
	// // sluiten van de YML-file
	// fs.release();
	//
	// VideoCapture cap = VideoCapture(1);
	//
	// if (!cap.isOpened())
	// {
	// 	cout << "Cannot open the video cam" << endl;
	// 	return -1;
	// }
	//
	// double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	// double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	// cout << "Frame size : " << dWidth << " x " << dHeight << endl;
	//
	// Mat frame, frameUndistorted;
	// int count = 0;
	//
	// while (1) {
	// 	// Lees een nieuw frame
	// 	bool bSuccess = cap.read(frame);
	//
	// 	// Controlleer of het frame goed gelezen is.
	// 	if (!bSuccess)
	// 	{
	// 		cout << "Cannot read a frame from video stream" << endl;
	// 		break;
	// 	}
	// 	/*cap >> frame;*/
	// 	undistort(frame, frameUndistorted, intrinsic, distCoeffs);
	// 	if (count == 0) {
	// 		imwrite("C:\\School\\frame.PNG", frameUndistorted);
	// 		count++;
	// 	}
	// 	imshow("Original", frameUndistorted);
	//
	// 	calculateSizeOfRoundObject(frameUndistorted);
	//
	// 	//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
	// 	if (waitKey(1) == 27)
	// 	{
	// 		cout << "esc key is pressed by user" << endl;
	// 		break;
	// 	}
	// }
	Mat frame;
	frame = imread("./images/frame.png", CV_LOAD_IMAGE_COLOR);
	// Controleer of alles goed is gegaan
	if (!frame.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	calculateSizeOfRoundObject(frame);

	waitKey(0);

	return 0;
}
