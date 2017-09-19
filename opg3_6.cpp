
#include "opg3_6.h"

int opg3_6(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "NB! Geef als command argument volledige padnaam van de imagefile mee" << endl;
		return -1;
	}
	else cout << "De imagefile = " << argv[1] << endl;

	// Lees de afbeelding in
	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	string windowName = "Opgave 3_6";

	// Window maken waarin de beelden "live" getoond worden
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	imshow(windowName, gray_image);

	Mat result;
	// Erode de image 1 keer.
	erode(gray_image, result, Mat(), Point(-1, -1), 1, 1, 1);
	imshow("Eroded image", result);

	// Haal de geerodeerde image van de greyscale image af. 
	imshow("Contour", (gray_image - result));

	while (1) {
		if (waitKey(30) == 27) {
			break;
		}
	}

	return 0;
}
