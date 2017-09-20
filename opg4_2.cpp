#include "opg4_2.h"

int opg4_2(int argc, char** argv) {
	// YML-file met callibratie data openen
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

	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	Mat frame, frameUndistorted;
	int count = 0;

	while (1) {
		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		/*cap >> frame;*/
		undistort(frame, frameUndistorted, intrinsic, distCoeffs);
		if (count == 0) {
			imwrite("C:\\School\\frame.PNG", frameUndistorted);
			count++;
		}
		imshow("Original", frameUndistorted);

		// De afbeelding converteren naar een grijswaarde afbeelding
		Mat gray_image;
		cvtColor(frameUndistorted, gray_image, CV_BGR2GRAY);

		// Filter ruis weg
		Mat result;
		Mat kernel = (Mat_<double>(5, 5) << 2, 4, 5, 4, 2, 4, 9, 12, 9, 4, 5, 12, 15, 12, 5, 4, 9, 12, 9, 4, 2, 4, 5, 4, 2);
		filter2D(gray_image, result, -1, kernel * (1.0f / 115), Point(-1, -1), 0, BORDER_REPLICATE);


		// Het tonen van grijswaarde beeld
		imshow("Grey image", gray_image);

		Mat tresholdImage;
		Mat binaryImage;
		const int tresh_value = 200;
		threshold(result, tresholdImage, tresh_value, 255, THRESH_BINARY);
		//debugging
		threshold(result, binaryImage, tresh_value, 1, THRESH_BINARY);

		imshow("Treshold image", tresholdImage);

		/*Mat binary16S;
		binaryImage.convertTo(binary16S, CV_16S);
		Mat labeledImage;
		cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;
		show16SImageStretch(labeledImage, "Blob image");*/

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}


	return 1;
}