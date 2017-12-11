#include "floodFillTest.h"
#include "floodFill.h"
#include <chrono>
#include <stdlib.h>
#include <stdio.h>

using namespace std::chrono;

int floodFillTest(int argc, char * argv[])
{
	srand(time(nullptr));
	// Controle of er een argument aan het programma is meegegeven.
	if (argc != 2)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}

	// Mat is een class voor objecten waarin een afbeelding kan worden opgeslagen.
	Mat image;

	// Lees de afbeelding in en sla deze op in image.
	// De filenaam is het eerste argument dat meegegeven is bij aanroep van het programma.
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	// Controleer of alles goed is gegaan
	if (!image.data)
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	// Mat object aanmaken voor grijswaarde beeld
	Mat gray_image;
	Mat bin;

	// Converteren van de ingelezen afbeelding naar een grijswaarde beeld.
	cvtColor(image, gray_image, CV_BGR2GRAY);
	// Converteren naar grijswaarde afbeelding
	cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << endl;

	threshold(gray_image, bin, 170, 255, THRESH_BINARY_INV);

	imshow("Original", image);
	imshow("Binary", bin * 255);
	waitKey(0);
	cout << "Press enter to go futher" << endl;
	//waitKey(0);

	vector<vector<Point>> contours;
	findContours(bin, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	/*for (auto const& contour : contours) {
		cout << "Determine region..." << std::endl;
		vector<Point> region;
		int area = enclosedPixels(contour, region);
	}*/

	if (contours.size() < 1) {
		cout << "No contours found in image" << argv[1] << endl;
		return -1;
	}

	cout << "Determine region..." << std::endl;

	Mat drawing = Mat::zeros(bin.size(), CV_8UC3);
	Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
	drawContours(drawing, contours, 0, color, 1, 8);
	imshow("Contour", drawing);
	//imwrite("C:\\School\\floodfill\\flood_fill_test_contour.png", drawing);
	waitKey(0);


	//Self build function
	vector<Point> region;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	int area = enclosedPixels(contours[0], region);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	cout << "Region has a area of: " << area << ", In " << duration * 0.001 << " s" << endl;

	// Creeer een witte image
	IplImage* iplimage = cvCreateImage(cvSize(bin.cols, bin.rows), IPL_DEPTH_8U, 3);
	Mat regionsImage = cvarrToMat(iplimage);
	regionsImage = Scalar(255, 255, 255);

	/*drawContours(regionsImage, region, -1, CV_RGB(255, 0, 0));*/

	Vec3b mycolor(255, 0, 0);
	for (int i = 0; i<region.size(); i++)
		regionsImage.at<Vec3b>(region[i].y, region[i].x) = mycolor;


	imwrite("C:\\School\\floodfill\\filled.png", regionsImage);
	imshow("Found Regions: "+to_string(area), regionsImage);
	waitKey(0);

	return 0;
}
