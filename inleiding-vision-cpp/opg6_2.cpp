#include "opg6_2.h"


int opg6_2(int argc, char** argv) {
	Mat frame;
	frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	// Controleer of alles goed is gegaan
	if (!frame.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat gray;

    // convert RGB image to gray
    cvtColor(frame, gray, CV_BGR2GRAY);

    imshow("Afbeelding", gray);

	Mat edges;
	blur( gray, edges, Size(3,3) );

	imshow("Edges", edges);

	int threshold = 30;
	int sobel_kernel_size = 3;
	Canny(edges, edges, threshold, threshold*3, sobel_kernel_size);

	imshow("Kenny", edges);

	Mat eind;
	eind = Scalar::all(0);
	frame.copyTo(eind, edges);

	imshow("Eind", eind);


	waitKey(0);

	return 0;
}
