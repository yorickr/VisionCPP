#include "opg5_2.h"


int opg5_2(int argc, char** argv) {
	Mat frame;
	frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	// Controleer of alles goed is gegaan
	if (!frame.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat gray_image;

    // convert RGB image to HSV
    cvtColor(frame, gray_image, CV_BGR2GRAY);
	medianBlur(gray_image, gray_image, 5);

	vector<Vec3f> circles;
    HoughCircles(gray_image, circles, HOUGH_GRADIENT, 1,
                 gray_image.rows/6,
                 100, 30, 1, 100
                 );
	cout << circles.size() << endl;
	map<int, int> hue_map;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
		cout << c << endl;
        circle( frame, Point(c[0], c[1]), c[2], Scalar(0,0,0), 3, LINE_AA);
        // circle( frame, Point(c[0], c[1]), 2, Scalar(0,0,0), 3, LINE_AA);
		// middelpunt is c[0] en c[1]. Radius is c[2].
		int x = c[0];
		int y = c[1];
		Mat rgb_image_roi = frame(Rect(x,y,1,1));
		Mat hsv_roi;
		cvtColor(rgb_image_roi, hsv_roi, CV_BGR2HSV);

		Vec3b pixel = hsv_roi.at<Vec3b>(0,0);
		int hue = pixel.val[0] * 2;
		float sat = pixel.val[1] / 255.0f;
		float val = pixel.val[2] / 255.0f;
		cout << hue << endl;
		cout << sat << endl;
		cout << val << endl;
		auto iter = hue_map.find(hue);
		if (iter == hue_map.end() ) {
		    // doesn't exist, add
			hue_map.insert(pair<int, int>(hue, 1));
			cout << "Hue doesn't exist" << endl;
		} else {
			hue_map[hue] += 1;
			cout << "Hue exists" << endl;
		}
		cout << "-------" << endl;
		// break;
    }

	for(map<int,int>::iterator it = hue_map.begin(); it != hue_map.end(); ++it) {
		cout << "Hue " << it->first << " has a total of " << it->second << " ocurrences." << endl;
	}

	imshow("Afbeelding", frame);
	// imshow("Gray image", gray_image);

	waitKey(0);

	return 0;
}
