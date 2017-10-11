#include "opg5_3.h"

vector<int> HueColorValues = {0, 46, 60, 148, 208, 274};


int opg5_3(int argc, char** argv) {
	FileStorage fs("C:\\School\\ueyecallib.yml", FileStorage::READ);

	// callibratie data ophalen
	Mat intrinsic, distCoeffs;
	fs["intrinsic"] >> intrinsic;
	fs["distCoeffs"] >> distCoeffs;

	// callibratie matrices tonen op het scherm
	cout << "intrinsic matrix: " << intrinsic << endl;
	cout << "distortion coeffs: " << distCoeffs << endl;

	// sluiten van de YML-file
	fs.release();

	VideoCapture cap = VideoCapture(1);
	int count = 0;
	//
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	Mat frame, frameUndistorted, gray_image, bin;

	while (1) {
		bool bSuccess = cap.read(frame);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		undistort(frame, frameUndistorted, intrinsic, distCoeffs);

		imshow("Original undistorted", frameUndistorted);
		if (count == 0) {
		imwrite("C:\\School\\frame.PNG", frameUndistorted);
		count++;
		}

		// convert RGB image to HSV
		cvtColor(frameUndistorted, gray_image, CV_BGR2GRAY);
		medianBlur(gray_image, gray_image, 5);

		imshow("Grey image", gray_image);
		threshold(gray_image, bin, 230, 255, THRESH_BINARY);
		imshow("Binair", bin);

		vector<Vec3f> circles;
		HoughCircles(bin, circles, HOUGH_GRADIENT, 1,
			bin.rows / 18,
			100, 20, 1, 50
		);

		//cout << circles.size() << endl;
		map<int, int> hue_map;
		for (size_t i = 0; i < circles.size(); i++)
		{
			Vec3i c = circles[i];
			//cout << c << endl;
			circle(frameUndistorted, Point(c[0], c[1]), c[2], Scalar(0, 0, 0), 3, LINE_AA);
			// circle( frame, Point(c[0], c[1]), 2, Scalar(0,0,0), 3, LINE_AA);
			// middelpunt is c[0] en c[1]. Radius is c[2].
			int x = c[0];
			int y = c[1];
			Mat rgb_image_roi = frameUndistorted(Rect(x, y, 1, 1));
			Mat hsv_roi;
			cvtColor(rgb_image_roi, hsv_roi, CV_BGR2HSV);

			Vec3b pixel = hsv_roi.at<Vec3b>(0, 0);
			int hue = pixel.val[0] * 2;
			float sat = pixel.val[1] / 255.0f;
			float val = pixel.val[2] / 255.0f;
			//cout << hue << endl;
			//cout << sat << endl;
			//cout << val << endl;
			//auto iter = hue_map.find(hue);
			//if (iter == hue_map.end()) {
			//	// doesn't exist, add
			//	hue_map.insert(pair<int, int>(hue, 1));
			//	/*cout << "Hue doesn't exist" << endl;*/
			//}
			//else {
			//	hue_map[hue] += 1;
			//	//cout << "Hue exists" << endl;
			//}
			//cout << "-------" << endl;
			// break;
			//for each (auto hueValue in HueColorValues)
			//{
			//	int max = hueValue + 10;
			//	int min = hueValue - 10;
			//	if (min < hue && hue < max) {
			//		// Deze kleur zit al in de hue_map, dus +1 toevoegen bij de occurences.
			//		auto iter = hue_map.find(hueValue);
			//		if (iter == hue_map.end()) {
			//			// doesn't exist, add
			//			hue_map.insert(pair<int, int>(hueValue, 1));
			//			/*cout << "Hue doesn't exist" << endl;*/
			//		}
			//		else {
			//			hue_map[hueValue] += 1;
			//			//cout << "Hue exists" << endl;
			//		}			
			//		break;
			//	} 
			//	
			//}
		}
		
		imshow("Afbeelding", frameUndistorted);
		// imshow("Gray image", gray_image);

		int key = waitKey(1);

		if (key == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}
		else if(key == ' ') {
			for (map<int, int>::iterator it = hue_map.begin(); it != hue_map.end(); ++it) {
				cout << "Hue " << it->first << " has a total of " << it->second << " ocurrences." << endl;
			}
		}
	}

	return 0;
}
