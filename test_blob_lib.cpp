// Functionaliteit: Test- en demoprogramma voor labeling functies
//                  uit de library avansvisionlib.
// Jan Oostindie, dd 17-3-2016
#include "includes.h"

#include "blobdetectionavans.h"

int test_blob_lib(int argc, char *argv[])
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

	//////////////////////////////////////////////////////////////////////////

	// Grijswaarde afbeelding thresholden
	Mat binaryImage;
	threshold(gray_image, binaryImage, 200, 1, CV_THRESH_BINARY_INV);
	imshow("Original", gray_image);

	// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
	// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
	Mat binary16S;
	binaryImage.convertTo(binary16S, CV_16S);

	// functie labelBLOBs doet hetzelfde als Label Blobs in VisionLab; input is een
	// binair beel. Output is een image waarin alle pixels van elke blob voorzien zijn van
	// volgnummer.
	Mat labeledImage;
	cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;

	// functie show16SImageStretch beeld elke image af op 0 - 255 zodat je vrijwel altijd
	// wel een redelijke view krijgt op de data.
	// (show16SImageClip laat alle pixels met een waarde tussen 0 en 255 zien. Waardes onder 0
	// worden op 0 afgebeeld, waardes boven 255 worden op 255 afgebeeld.)

	show16SImageStretch(labeledImage, "Labeled Image");

	cout << "Image naar schijf schrijven." << endl;
	imwrite("images/labeledImage.jpg", labeledImage);



    // labelBLOBsInfo: met deze functie kun je ook BLOBs labelen. De functie geeft van
	// alle BLOBs de volgende informatie terug:
	// - coordinaten van het eerste pixel van de BLOB
	// - coordinaten van het zwaartepunt van de BLOB.
	// - area van de BLOB
	Mat labeledImage2;
	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(binary16S, labeledImage2, firstpixelVec2, posVec2, areaVec2);
	show16SImageStretch(labeledImage2, "Labeled Image 2");

	cout << endl << "*******************************************" << endl << endl;

	// Toon alle informatie in de console
	cout << "Aantal gevonden BLOBs = " << firstpixelVec2.size() << endl;
	for (int i = 0; i < firstpixelVec2.size(); i++) {
		cout << "BLOB " << i + 1 << endl;
		cout << "firstpixel = (" << firstpixelVec2[i]->x << "," << firstpixelVec2[i]->y << ")" << endl;
		cout << "centre = (" << posVec2[i]->x << "," << posVec2[i]->y << ")" << endl;
		cout << "area = " << areaVec2[i] << endl;
	}

	cout << endl << "*******************************************" << endl << endl;

	// Met de functie labelBLOBsInfo kun je ook een threshold instellen voor de oppervlakte
	// van de BLOBs.

	Mat labeledImage3;
	vector<Point2d *> firstpixelVec3;
	vector<Point2d *> posVec3;
	vector<int> areaVec3;
	labelBLOBsInfo(binary16S, labeledImage3,
		           firstpixelVec3, posVec3, areaVec3, 15000, 16000);
	show16SImageStretch(labeledImage3, "Labeled Image 3");

	cout << endl << "*******************************************" << endl << endl;

	// Toon alle informatie in de console
	cout << "Aantal gevonden BLOBs = " << firstpixelVec3.size() << endl;
	for (int i = 0; i < firstpixelVec3.size(); i++) {
		cout << "BLOB " << i + 1 << endl;
		cout << "firstpixel = (" << firstpixelVec3[i]->x << "," << firstpixelVec3[i]->y << ")" << endl;
		cout << "centre = (" << posVec3[i]->x << "," << posVec3[i]->y << ")" << endl;
		cout << "area = " << areaVec3[i] << endl;
	}

	cout << endl << "*******************************************" << endl << endl;

	// Nog een manier om het aantal BLOBs te verkrijgen...
	cout << "Total number of BLOBs = " << maxPixelImage(labeledImage3) << endl;

	string pipo;
	cin >> pipo;

	return 0;

}
