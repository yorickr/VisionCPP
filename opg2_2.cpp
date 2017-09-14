// The Basic Image Container // Part 2
// Uitleg: http://docs.opencv.org/doc/tutorials/core/mat_the_basic_image_container/mat_the_basic_image_container.html
// Jan Oostindie, dd 22-1-2015

#include "opg2_2.h"

int opg2_2(int argc, char** argv)
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

	// Wait for a keystroke in the window
	waitKey(0);

	// Eigenschappen van het beeld ophalen en afdrukken op het scherm
	int channels = gray_image.channels();
	int nRows = gray_image.rows;
	int nCols = gray_image.cols * channels;
	cout << "Deze image heeft: " << nRows << " X " << nCols << " pixels" << endl;
	cout << "Het aantal kanalen per pixel is: " << channels << endl;

	// Controleren of de afbeelding een grijswaarde beeld is.
	if ((!channels) == 1) {
		cout << "NB Dit is geen grijswaarde beeld" << endl;
		cout << "Het programma is hier niet op voorbereid en wordt daarom gestopt.";
		return -1;
	}

	// Controleren of de matrix als 1 lange continue rij getallen in het geheugen staat
	if (!gray_image.isContinuous())
	{
		cout << "NB Matrix is NOT continuous" << endl;
		cout << "Programma is hier niet op voorbereid en wordt daarom gestopt.";
		return -1;
	}

	// Pixelcoordinaten inlezen
	cout << "Geef pixel coordinaten x <,> y = ";
	string dummy;
	getline(cin, dummy);

	// invoerstring ontleden in i en j deel
	string istr = dummy.substr(0, dummy.find(','));
	string jstr = dummy.replace(0, dummy.find(',') + 1, "");

	// i en j deel omzetten in een integer
	int i, j;
	i = atoi(istr.c_str());
	j = atoi(jstr.c_str());

	// pointer naar eerste element van de matrix laten wijzen.
	// NB We gaan ervan uit dat de matrix als 1 lange continue rij getallen in het 
	//    geheugen staat. Dit kan eventueel gecontroleerd worden met de boolean lidfunctie
	//    isContinous. if (gray_image.isContinuous()) .....
	uchar* p;
	p = gray_image.ptr<uchar>(0);

	// pixel met positie (i,j) heeft index i*nCols+j (controleer dat!)
	unsigned int index = i*nCols + j;

	// pixelwaarde afdrukken. Met -0 wordt de char afgedrukt als int
	cout << "pixelwaarde(" << i << "," << j << ") = " << p[index] - 0 << endl;
	getline(cin, dummy);

	return 0;
}
