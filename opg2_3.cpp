#include "includes.h"

const int MAX_BINARY_VALUE = 255;

string window_name = "Opgave 2.3";

int opg2_3(int argc, char** argv)
{
	// Controle of er een argument aan het programma is meegegeven.
	if (argc != 3)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay threshold_value" << endl;
		return -1;
	}

	// Mat is een class voor objecten waarin een afbeelding kan worden opgeslagen.
	Mat image;

	// Lees de afbeelding in en sla deze op in image.
	// De filenaam is het eerste argument dat meegegeven is bij aanroep van het programma.
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	int threshold_value = stoi(argv[2]);

	// Controleer of alles goed is gegaan
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Mat object aanmaken voor grijswaarde beeld
	Mat gray_image;
	Mat bin, bin_inv, tresh_trunc, tozero, tozero_inv;

	// Converteren van de ingelezen afbeelding naar een grijswaarde beeld.
	cvtColor(image, gray_image, CV_BGR2GRAY);

	/****** Resultaten naar het scherm sturen ******/
	// Voer verschillende threshhold methoden uit.
	threshold( gray_image, bin, threshold_value, MAX_BINARY_VALUE, THRESH_BINARY );
	threshold( gray_image, bin_inv, threshold_value, MAX_BINARY_VALUE, THRESH_BINARY_INV );
	threshold( gray_image, tresh_trunc, threshold_value, MAX_BINARY_VALUE, THRESH_TRUNC );
	threshold( gray_image, tozero, threshold_value, MAX_BINARY_VALUE, THRESH_TOZERO );
	threshold( gray_image, tozero_inv, threshold_value, MAX_BINARY_VALUE, THRESH_TOZERO_INV );

	// Grijswaarde beeld tonen op het scherm
	// namedWindow( window_name, WINDOW_AUTOSIZE );
	imshow("Binary", bin);
	imshow("Binary Inverted", bin_inv);
	imshow("Truncated", tresh_trunc);
	imshow("ToZero", tozero);
	imshow("ToZero Inverted", tozero_inv);
	waitKey(0);

	cout << "Klaar!" << endl;
	string dummy;
	getline(cin, dummy);

	return 0;
}
