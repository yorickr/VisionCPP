#include "opg5_1.h"


int opg5_1(int argc, char** argv) {
	Mat frame;
	frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	// Controleer of alles goed is gegaan
	if (!frame.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat hsvImage;

    // convert RGB image to HSV
    cvtColor(frame, hsvImage, CV_BGR2HSV);

    imshow("Afbeelding", frame);

    imshow("HSV", hsvImage);

    vector<Mat> hsv;
    split(hsvImage, hsv);
    Mat h = hsv[0];
    Mat s = hsv[1];
    Mat v = hsv[2];

    imshow("hue", h);

    imshow("saturation", s);

    imshow("value", v);

	waitKey(0);

	return 0;
}
