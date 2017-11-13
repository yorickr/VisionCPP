
#include "opg3_4.h"

RNG rng(12345);

void doThings (Mat gray_image, Mat frame) {
			// Het tonen van grijswaarde beeld
			imshow("Gray Image", gray_image);

			Mat tresholdImage;
			Mat binaryImage;
			const int tresh_value = 100;
			threshold(gray_image, tresholdImage, tresh_value, 255, THRESH_BINARY);
			threshold(gray_image, binaryImage, tresh_value, 1, THRESH_BINARY);

			imshow("Threshold Image", tresholdImage);


			Mat distance;
		    distanceTransform(tresholdImage, distance, CV_DIST_L2, 3);
			normalize(distance, distance, 0, 1., NORM_MINMAX);
	   		imshow("Distance Transform Image", distance);


			threshold(distance, distance, 0.4, 1.0, THRESH_BINARY);
			// Dilate a bit the dist image
			Mat ones = Mat::ones(3, 3, CV_8UC1);
			dilate(distance, distance, ones);
			imshow("Peaks", distance);

			vector<vector<Point> > contours;
			Mat u8Image;
			distance.convertTo(u8Image, CV_8UC1);
		    findContours(u8Image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			Mat markers = Mat::zeros(distance.size(), CV_32SC1);

			for (size_t i = 0; i < contours.size(); i++)
				drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);
			//
			circle(markers, Point(5,5), 3, CV_RGB(255,255,255), -1);
			imshow("Markers", markers*10000);

			watershed(frame, markers);

			Mat mark = Mat::zeros(markers.size(), CV_8UC1);
			markers.convertTo(mark, CV_8UC1);
			bitwise_not(mark, mark);

			// imshow("Markers_v2", mark);

			vector<Vec3b> colors;
			for (size_t i = 0; i < contours.size(); i++)
			{
				int b = rng.uniform(0, 255);
				int g = rng.uniform(0, 255);
				int r = rng.uniform(0, 255);
				colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
			}

			Mat dst = Mat::zeros(markers.size(), CV_8UC3);
			for (int i = 0; i < markers.rows; i++)
			{
				for (int j = 0; j < markers.cols; j++)
				{
					int index = markers.at<int>(i,j);
					if (index > 0 && index <= static_cast<int>(contours.size()))
						dst.at<Vec3b>(i,j) = colors[index-1];
					else
						dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
				}
			}

			 imshow("Final Result", dst);
}

int opg3_4(int argc, char* argv[])
{
	// Open de camera met nummer 1 in lijst (red.: nr 0 was bij mij de camera in de klep van mijn laptop)
	VideoCapture cap(1);

	// Controle of de camera wordt herkend.
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	string windowName = "Opgave 3_3";

	// Breedte en hoogte van de frames die de camera genereert ophalen.
	cap.set(CV_CAP_PROP_FPS, 10);
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
	cout << "FPS: " << cap.get(CV_CAP_PROP_FPS) << endl;

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	Mat frame;

	while (1)
	{

		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		// De afbeelding converteren naar een grijswaarde afbeelding
		Mat gray_image;
		cvtColor(frame, gray_image, CV_BGR2GRAY);

		// Filter ruis weg
		// Mat result;
		// Mat kernel = (Mat_<double>(5,5) << 2, 4, 5, 4, 2, 4, 9, 12, 9, 4, 5, 12, 15, 12, 5, 4, 9, 12, 9, 4, 2, 4, 5, 4, 2);
		// filter2D(gray_image, result, -1, kernel * (1.0f/115), Point(-1, -1), 0, BORDER_REPLICATE);

		doThings(gray_image, frame);

		// Mat binary16S;
		// binaryImage.convertTo(binary16S, CV_16S);
		// Mat labeledImage;
		// cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;
		// show16SImageStretch(labeledImage, "Blob image");

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}
