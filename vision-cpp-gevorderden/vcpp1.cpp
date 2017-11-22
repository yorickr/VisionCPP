#include "vcpp1.h"

bool is_white(Vec3b &vec) {
    for (size_t x = 0; (x < 3); x++) {
        if (vec[x] != 255) { // binary image, so this should work.
            return false;
        }
    }
    return true;
}

// return true if vec contains point p
bool vec_contains(vector<Point> &vec, Point &p) {
    for (size_t i = 0; i < vec.size(); i++){
        Point *p2 = &vec.at(i);
        if (p.x == p2->x && p.y == p2->y) {
            return true;
        }
    }
    return false;
}

Point direction_to_point(Point &p) {
    // use a map to check all 7 options at once

    // int x = p.x;
    // int y = p.y;
    // switch(direction) {
    //     case 0: // check right
    //         return p
    //         return Point(x + 1, y    );
    //     case 1: // check right top
    //         return Point(x + 1, y + 1);
    //     case 2: // check top
    //         return Point(x    , y + 1);
    //     case 3:
    //         return Point(x - 1, y + 1);
    //     case 4:
    //         return Point(x - 1, y    );
    //     case 5:
    //         return Point(x - 1, y - 1);
    //     case 6:
    //         return Point(x    , y - 1);
    //     case 7:
    //         return Point(x + 1, y - 1);
    //     default:
    //         cerr << "Error in direction_to_point" << endl;
    //         return Point(0,0);
    // }
}

double bendingEnergy(Mat binaryImage, vector<Point> &contourVec) {
    cout << "In function bendingEnergy" << endl;

    for (size_t i = 0; i < contourVec.size(); i++) {
        Point *p = &contourVec.at(i);
        // Dereference pointer when we need the value.
        cout << *p << endl;
    }

    // find start pixel, so x is closest to 0 and y is also closest to 0
    // note, multiple x'es having the same value can occur.
    for (size_t i = 0; i < 7; i++) {
        // check direction.
        cout << "Direction check " << i << endl;
        // Point to_check = direction_to_point(i, current);
        // Vec3b vec = binaryImage.at<Vec3b>(to_check.x , to_check.y);
    }

    return 0.0;
}

int vcpp1_main(int argc, char** argv) {
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

    threshold(gray_image, bin, 240, 255, THRESH_BINARY_INV);

    imshow("Original", image);
    imshow("Binary", bin*255);

    vector<vector<Point>> contours;

    findContours(bin, contours, RETR_TREE, CHAIN_APPROX_SIMPLE); // TODO: change me to our own function.

    Mat drawing = Mat::zeros(bin.size(), CV_8UC3);
    for( size_t i = 2; i< contours.size(); i++ ) {
        Scalar color = Scalar( rand()%255, rand() % 255, rand() % 255 );
        drawContours( drawing, contours, (int)i, color, 2, 8);
        // break; // TODO: remove me
    }
    imshow("Contours", drawing);

    double f = bendingEnergy(bin, contours.at(2));

    cout << "Bending energy is " << f << endl;

    waitKey(0);
    return 0;
}
