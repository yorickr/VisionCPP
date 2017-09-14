
#include "opg3_3.h"

int opg3_3(int argc, char* argv[])
{
	// Open de camera met nummer 1 in lijst (red.: nr 0 was bij mij de camera in de klep van mijn laptop)
	VideoCapture cap(0);

	// Controle of de camera wordt herkend.
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	string windowName = "Opgave 3_3";

	// Breedte en hoogte van de frames die de camera genereert ophalen.
	cap.set(CV_CAP_PROP_FPS, 10);
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
	cout << "FPS: " << cap.get(CV_CAP_PROP_FPS) << endl;

	// Window maken waarin de beelden "live" getoond worden
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	Mat frame;

	while (1)
	{

		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		// De afbeelding converteren naar een grijswaarde afbeelding
		Mat gray_image;
		cvtColor(frame, gray_image, CV_BGR2GRAY);

		// Filter ruis weg
		Mat result;
		Mat kernel = (Mat_<double>(5,5) << 2, 4, 5, 4, 2, 4, 9, 12, 9, 4, 5, 12, 15, 12, 5, 4, 9, 12, 9, 4, 2, 4, 5, 4, 2);
		filter2D(gray_image, result, -1, kernel * (1.0f/115), Point(-1, -1), 0, BORDER_REPLICATE);


		// Het tonen van grijswaarde beeld
		imshow(windowName, gray_image);

		Mat tresholdImage;
		Mat binaryImage;
		const int tresh_value = 100;
		threshold(result, tresholdImage, tresh_value, 200, THRESH_BINARY_INV);
		threshold(result, binaryImage, tresh_value, 1, THRESH_BINARY_INV);

		imshow("Treshold image", tresholdImage);

		Mat binary16S;
		binaryImage.convertTo(binary16S, CV_16S);
		Mat labeledImage;
		cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;
		show16SImageStretch(labeledImage, "Blob image");

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}
