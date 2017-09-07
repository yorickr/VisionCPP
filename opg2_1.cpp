#include "includes.h"

int opg2_1(int argc, char** argv)
{
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
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Mat object aanmaken voor grijswaarde beeld
	Mat gray_image;

	// Converteren van de ingelezen afbeelding naar een grijswaarde beeld.
	cvtColor(image, gray_image, CV_BGR2GRAY);
	// Hoge en breedte ophalen
	Size s = gray_image.size();
	int height = s.height;
	int width = s.width;
	cout << height << endl;
	cout << width << endl;
	// Top bepalen
	Mat top(gray_image, Rect(0, 0, width, height/2));
	// Bot bepalen
	Mat bot(gray_image, Rect(0, height/2, width, height/2));

	/****** Resultaten naar het scherm sturen ******/

	// Grijswaarde beeld tonen op het scherm
	imshow("Top", top);
	imshow("Bottom", bot);
	waitKey(0);

	cout << "Klaar!" << endl;
	string dummy;
	getline(cin, dummy);

	return 0;
}
