// Functionaliteit: Testprogramma voor het testen van de allContours() functie
// Zie module Vision C++ voor Gevorderden Opdracht 2 van Fase 1
// De demo laat met avansvisionlib en OpenCV de contouren zien.
//
// De algoritmiek wordt in de eerste workshop toegelicht.
//
// Jan Oostindie, dd 8-11-2016
//
#include "allContoursTestProgramma.h"

int allContoursTestProgramma(int argc, char *argv[])
{
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

	// Converteren naar grijswaarde afbeelding
	cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << endl;

	////////////////////////////////////////////////////////////////////////////////

	imshow("Original", gray_image);
	waitKey(0);

	/** ! Gebruik een region of interest voor de rummikub images **/
	// Mat imageROI = gray_image(Rect(34, 28, 976, 82));
	// imshow("imageROI", imageROI);
	// waitKey(0);

	// Grijswaarde afbeelding thresholden
	Mat binaryImage;

	/** ! Gebruik een region of interest voor de rummikub figuren **/
	// threshold(imageROI, binaryImage, 65, 1, CV_THRESH_BINARY_INV);

	// Stel een threshold in. Laad evt de figuur eerst in VisionLab om een goede
	// threshold te bepalen.
	// ! Comment this line out when using rummikub images
	threshold(gray_image, binaryImage, 150, 1, CV_THRESH_BINARY_INV);

	// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
	// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
	Mat binary16S;
	binaryImage.convertTo(binary16S, CV_16S);

	show16SImageStretch(binary16S,"Binary image");

	// haal de contouren uit het binary image
	vector<vector<Point>> contours;
	cout << "Contouren bepalen..." << std::endl;
	int aantal = allContours(binary16S, contours);  // <<<<*** De opdracht is dus om deze functie te maken ***
	/*vector<cv::Vec4i> hierachy;
	cv::findContours(binary16S, contours, hierachy, cv::RETR_FLOODFILL, cv::CHAIN_APPROX_SIMPLE);
	int aantal = contours.size();*/

	// druk alle punten van alle gevonden contouren af
	string line;
	cout << "Aantal gevonden contouren = " << contours.size() << std::endl;
	getline(cin, line);
	for (int i = 0; i < contours.size(); i++) {
		cout << "*** Contour " << i + 1 << " ***" << endl;
		cout << "Press ENTER to continue....";
		getline(cin, line);
		for (int j = 0; j < contours[i].size(); j++) {
			cout << "(" << contours[i][j].x << "," << contours[i][j].y << ")" << endl;
		}
	}

	// Creeer een witte image
	IplImage* iplimage = cvCreateImage(cvSize(binary16S.cols, binary16S.rows), IPL_DEPTH_8U, 3);
	Mat contourImage = cvarrToMat(iplimage);
	contourImage = Scalar(255, 255, 255);

	// teken de contouren op de witte image
	drawContours(contourImage, contours, -1, CV_RGB(255, 0, 0));

	// druk het image met de contouren af
	imshow("Found contours", contourImage);
	waitKey(0);

	string pipo;
	cin >> pipo;

	return 0;
}
