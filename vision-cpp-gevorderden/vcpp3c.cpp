#include "vcpp3c.h"
#include "vcpp1.h"
#include "floodFill.h"
#include "../inleiding-vision-cpp/includes.h"

int vcpp3c_main(int argc, char ** argv)
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

	int thresh = 170;
	threshold(gray_image, bin, thresh, 255, THRESH_BINARY_INV);

	imshow("Original", image);
	imshow("Binary",   bin);
	waitKey(0);

	vector<vector<Point>> contours, filteredContours;

	findContours(bin, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); // TODO: change me to our own function.

	int minContourSize = 200;

	cout << "Filtering contours with minimum contour size of: " << minContourSize << endl;
	for (auto const &contour : contours) {
		if (contour.size() >= minContourSize) filteredContours.push_back(contour);
	}

	// save em.
	string s = argv[1];
	string delim = "/";
	cout << s << endl;

	int start = 0;
	int end = s.find(delim);
	while (end != string::npos)
	{
		// std::cout << s.substr(start, end - start) << std::endl;
		start = end + delim.length();
		end = s.find(delim, start);
	}
	string filename = s.substr(start, end);
	cout << "Filename is " << filename << endl;
	string name = filename.substr(0, filename.find("."));
	cout << "Name is " << name << endl;

	cout << "Drawing contours" << endl;
		
	for (size_t i = 0; i< filteredContours.size(); i++) {
		Mat drawing = Mat::zeros(bin.size(), CV_8UC3);
		Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
		drawContours(drawing, filteredContours, (int)i, color, 1, 8);
		imshow(name + to_string(i) + "_contour", drawing);
		imwrite("C:\\School\\floodfill\\" + name + "_" + to_string(i) + "_contour.jpg", drawing);
	}	
	waitKey(0);

	vector<vector<Point>> regions;
	vector<int> areas;

	for (size_t i = 0; i < filteredContours.size(); i++) {
		cout << "getting region" << endl;
		vector<Point> region;
		int area = enclosedPixels(filteredContours[i], region);

		regions.push_back(region);
		areas.push_back(area);
	}

	cout << "found all areas" << endl;
	
	//max width and height for all regions to fit in
	Size roiSize(0, 0);
	for (auto const& region : regions) {
		auto mmx = std::minmax_element(region.begin(), region.end(), [](const cv::Point& lhs, const cv::Point& rhs) {return lhs.x < rhs.x; });
		auto mmy = std::minmax_element(region.begin(), region.end(), [](const cv::Point& lhs, const cv::Point& rhs) {return lhs.y < rhs.y; });
		auto width = mmx.second->x - mmx.first->x;
		auto height = mmy.second->y - mmy.first->y;
		if (width > roiSize.width) roiSize.width = width;
		if (height > roiSize.height) roiSize.height = height;
	}

	cout << "Region of Interest size: " << roiSize << endl;

	for (int i = 0; i < regions.size(); i++) {
		auto region = regions[i];
		auto mmx = std::minmax_element(region.begin(), region.end(), [](const cv::Point& lhs, const cv::Point& rhs) {return lhs.x < rhs.x; });
		auto mmy = std::minmax_element(region.begin(), region.end(), [](const cv::Point& lhs, const cv::Point& rhs) {return lhs.y < rhs.y; });
		
		Mat roi(image, Rect(
			mmx.first->x,
			mmy.first->y,
			mmx.second->x - mmx.first->x,
			mmy.second->y - mmy.first->y
		));
		Mat objectRoi = Mat::zeros(roiSize, CV_8UC3);
		roi.copyTo(objectRoi(
			Rect(
				objectRoi.cols / 2 - roi.cols / 2,
				objectRoi.rows / 2 - roi.rows / 2,
				roi.cols,
				roi.rows
			)
		));

		imshow(name + to_string(i), objectRoi);
		imwrite("C:\\School\\floodfill\\" + name + "_" + to_string(i) + ".jpg", objectRoi);
	}
	waitKey(0);
}
