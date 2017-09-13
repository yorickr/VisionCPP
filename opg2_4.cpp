#include "opg2_4.h"

int opg2_4(int argc, char** argv)
{
	// met commandline argument wordt de filenaam doorgegeven
	// zie: project properties - configuration properties - debugging - command arguments
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

	// Grijswaarde afbeelding op het scherm zetten
	cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << endl;
	cout << "Druk op ENTER voor vervolg ==> " << endl;
	namedWindow("Display Gray window", WINDOW_AUTOSIZE);
	imshow("Display Gray window", gray_image);

	/*** Convolutie filter toepassen ==> de functie filter2D ***/
	/*** Zie voor uitleg de hyperlink bovenaan dit programma ***/
	/*** void filter2D(Mat src,
			Mat dst,
			int ddepth,
			Mat kernel,
			Point anchor,
			double delta,
			int borderType);
	***/

	Mat kernel = (Mat_<double>(5,5) << 2, 4, 5, 4, 2, 4, 9, 12, 9, 4, 5, 12, 15, 12, 5, 4, 9, 12, 9, 4, 2, 4, 5, 4, 2);
	Mat result;
	// print kernel
	cout << kernel << endl;
	// print kernel keer 1/115
	cout << kernel * (1.0f/115) << endl;
	// filter met kernel
	filter2D(gray_image, result, -1, kernel * (1.0f/100), Point(-1, -1), 0, BORDER_REPLICATE);

	// De gefilterde afbeelding op het scherm zetten
	cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << endl;
	cout << "Het grijswaarde beeld is gefilterd." << endl;
	cout << "Druk op ENTER voor vervolg ==> " << endl;
	namedWindow("Display Result filtering window", WINDOW_AUTOSIZE);
	imshow("Display Result filtering window", result);

	// Wait for a keystroke in the window
	waitKey(0);

	string dummy;
	getline(cin, dummy);

	return 0;

	return 0;
}
