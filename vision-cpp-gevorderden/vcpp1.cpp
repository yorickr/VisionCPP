#include "vcpp1.h"

double bendingEnergy(Mat binaryImage, vector<Point> &contourVec) {
    cout << "In function bendingEnergy" << endl;
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
    for( size_t i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar( rand()%255, rand() % 255, rand() % 255 );
        drawContours( drawing, contours, (int)i, color, 2, 8);
    }
    imshow("Contours", drawing);

    double f = bendingEnergy(bin, contours.at(0));

    cout << "Bending energy is " << f << endl;

    waitKey(0);
    return 0;
}
