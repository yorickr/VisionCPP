#include "opg4_2.h"

bool sortFunc(tuple<int, int, int, vector<Point>> left, tuple<int, int, int, vector<Point>> right) {
	return get<1>(left) < get<1>(right);
}

void findTopAndBot (const vector<vector<Point>> contours, vector<vector<Point>> &inputVector) {
	vector<tuple<int, int, int, vector<Point>>> temp;
	for(int i = 0; i != contours.size(); i++) {
		// find max X and min X
		int minX = numeric_limits<int>::max(), maxX = numeric_limits<int>::min();
		for (auto const& point : contours[i]) {
			int x = point.x;
			if (x < minX) {
				minX = x;
			}
			if (x > maxX) {
				maxX = x;
			}
		}
		cout << "Min X is " << minX << endl;
		cout << "Max X is " << maxX << endl;
		cout << "I is " << i << endl;
		tuple<int, int, int, vector<Point>> tup = make_tuple(i, minX, maxX, contours[i]);
		temp.push_back(tup);
	}
	sort(temp.begin(), temp.end(), sortFunc);
	inputVector.push_back(get<3>(temp.front()));
	inputVector.push_back(get<3>(temp.back()));

}

void calculatePixelToUnitRatio(const vector<Point> &vec) {

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
	vector<vector<Point>> vec; // front is lowest, back is highest
	findTopAndBot(contours, vec);
	cout << "Lowest " << contourArea(vec.front()) << endl;
	cout << "Highest " << contourArea(vec.back()) << endl;
	drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (vector<int>::size_type i = 0; i != vec.size(); i++) {
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
	}
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
