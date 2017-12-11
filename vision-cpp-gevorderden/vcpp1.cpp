#include "vcpp1.h"

bool is_white(Vec3b &vec) {
    for (size_t x = 0; (x < 3); x++) {
        if (vec[x] != 255) { // binary image, so this should work.
            return false;
        }
    }
    return true;
}

// return true if vec contains point p
bool vec_contains(vector<Point> &vec, Point &p) {
    for (size_t i = 0; i < vec.size(); i++){
        Point *p2 = &vec.at(i);
        if (p.x == p2->x && p.y == p2->y) {
            return true;
        }
    }
    return false;
}

pair<int, Point> find_eight_neighbours(vector<Point> &vec, Point &p, int offset, vector<Point> &done) {
    // P is our starting point.
    // cout << "Starting from "  << p << endl;
    for (size_t i = 0; i < 8; i++) {
        Point to_check;
        switch(i) {
            case 0:
                // check right
                to_check = Point(p.x + offset   , p.y);
            break;
            case 1:
                // check right top
                to_check = Point(p.x + offset   , p.y + offset);
            break;
            case 2:
                // check top
                to_check = Point(p.x            , p.y + offset);
            break;
            case 3:
                // check left top
                to_check = Point(p.x - offset   , p.y + offset);
            break;
            case 4:
                // check left
                to_check = Point(p.x - offset   , p.y);
            break;
            case 5:
                // check left bot
                to_check = Point(p.x - offset   , p.y - offset);
            break;
            case 6:
                // check bot
                to_check = Point(p.x            , p.y - offset);
            break;
            case 7:
                // check right bot
                to_check = Point(p.x + offset   , p.y - offset);
            break;
            default:
                cerr << "This should never happen in find_eight_neighbours" << endl;
                break;
        }
        // cout << "Checking " << to_check << endl;
        if (vec_contains(vec, to_check) && !vec_contains(done, to_check)) {
            // cout << "This point is in vec" << endl;
            return make_pair(i, to_check);
        }
    }
    return make_pair(-1, p);
}

pair<int, Point> find_first_neighbour(vector<Point> &vec, Point &start, vector<Point> &done) {
    for (size_t i = 1; i <= 10; i++) {
        pair<int, Point> p = find_eight_neighbours(vec, start, i, done);
        if (p.second != start) {
            return p;
        }
    }
    return make_pair(-1, start);
}

vector<int> find_chain(vector<Point> &contourVec, Point &start) {
    vector<int> chain;
    Point *first = &start;
    Point *current = &start;
    vector<Point> found;
    found.push_back(start);
    bool running = true;
    while (running) {
        // cout << "Finding for " << *current << endl;
        pair<int, Point> p = find_first_neighbour(contourVec, *current, found);
        // cout << "We have found " << p.second << " with id " << p.first << endl;
        // cout << "--------------" << endl;
        found.push_back(p.second);
        if (p.first == -1) {
            return chain;
        }
        // find next
        current = &p.second;
        chain.push_back(p.first);
    }

    return chain;
}

// pass bbs in here to calc contour area.
int contourArea(const vector<vector<Point>> &bbs, vector<int> &area) {
    for (size_t i = 0; i < bbs.size(); i++) {
        vector<Point> v = bbs.at(i);
        if (v.size() != 4) {
            // Error
            cerr << "ContourArea received a vector which is not a bounding box vector.";
            return -1;
        }
        Point xMin = v.at(0);
        Point xMax = v.at(1);
        Point yMin = v.at(2);
        Point yMax = v.at(3);

        area.push_back((xMax.x - xMin.x) * (yMax.y - yMin.y));

    }
    return 0;
}

int allBoundingBoxes(const vector<vector<Point>> & contours, vector<vector<Point>> & bbs) {
    int i = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        vector<Point> v = contours.at(i);
        vector<Point> boundingPoints;

        int xMax = numeric_limits<int>::min();
        int xMin = numeric_limits<int>::max();

        int yMax = numeric_limits<int>::min();
        int yMin = numeric_limits<int>::max();

        for (size_t j = 0; j < v.size(); j++) {
            Point cur = v.at(j);
            if (cur.x > xMax) {
                xMax = cur.x;
            }
            if (cur.x < xMin) {
                xMin = cur.x;
            }
            if (cur.y > yMax) {
                yMax = cur.y;
            }
            if (cur.y < yMin) {
                yMin = cur.y;
            }
        }

        boundingPoints.push_back(Point(xMin, xMin));
        boundingPoints.push_back(Point(xMax, xMax));
        boundingPoints.push_back(Point(yMin, yMin));
        boundingPoints.push_back(Point(yMax, yMax));
        bbs.push_back(boundingPoints);
        i++;
    }
    return i;
}

double bendingEnergy(Mat binaryImage, vector<Point> &contourVec) {
    // cout << "In function be	ndingEnergy" << endl;
    // cout << "Size of vec is " << contourVec.size() << endl;

    // find start pixel, so x is closest to 0 and y is also closest to 0
    // note, multiple x'es having the same value can occur.
    Point *start;
    start = &contourVec.at(0);
    for (size_t i = 0; i < contourVec.size(); i++) {
        Point *p = &contourVec.at(i);
        if ((p->x <= start->x) && (p->y <= start->y)) {
            start = p;
        }
    }

    // cout << "Most top left is " << *start << endl;

    // Assume that a 45 degree bend requires 1 energy, assume that a 90 degree bend requires 2.
    double energy = 0.0;

    vector<int> chain = find_chain(contourVec, *start);
    // cout << "Found chain is " << chain.size() << endl;
    if (chain.size() > 1) {
        int last = chain[0];
        for (size_t i = 1; i < chain.size(); i++) {
            // cout << chain[i];
            int current = chain[i];
            // diff of 1 adds 1, diff of 2 adds 2, diff of 3 adds 3
            energy += abs((current - last));
            last = current;
        }
        // cout << endl;
    }

    return energy;
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

    int thresh = 170;
    // Canny(gray_image, bin, thresh, thresh*2, 3);
    threshold(gray_image, bin, thresh, 255, THRESH_BINARY_INV);

    imshow("Original", image);
    imshow("Binary", bin);

    vector<vector<Point>> contours;

    findContours(bin, contours, RETR_TREE, CHAIN_APPROX_SIMPLE); // TODO: change me to our own function.

    Mat drawing = Mat::zeros(bin.size(), CV_8UC3);
    for( size_t i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar( rand()%255, rand() % 255, rand() % 255 );
        drawContours( drawing, contours, (int)i, color, 2, 8);

        // double f = bendingEnergy(bin, contours.at(i));
        //
        // cout << "Bending energy is " << f << endl;
    }

    imshow("Contours", drawing);

    vector<vector<Point>> bbs;
    allBoundingBoxes(contours, bbs);

    vector<int> areas;
    contourArea(bbs, areas);

    for (size_t i = 0; i < areas.size(); i++) {
        cout << "Area of " << i << " is " << areas[i] << endl;
    }

    // remove small contours that aren't useful
    cout << "Removing" << endl;

    vector<vector<Point>> filtered_bbs;

    for (size_t i = 0; i < areas.size(); i++) {
        int area = areas.at(i);
        if ((area > 500)) {
            // add this index.
            filtered_bbs.push_back(bbs.at(i));
        }
    }

    // save em.
    string s = argv[1];
    string delim = "/";
    cout << s << endl;

    int start = 0;
    int end = s.find(delim);
    while (end != string::npos)
    {
        // std::cout << s.substr(start, end - start) << std::endl;
        start = end + delim.length();
        end = s.find(delim, start);
    }
    string filename = s.substr(start, end);
    cout << "Filename is " << filename << endl;
    string name = filename.substr(0, filename.find("."));
    cout << "Name is " << name << endl;

    Mat area_img = Mat::zeros(bin.size(), CV_8UC3);
    for (size_t i = 0; i < filtered_bbs.size(); i++) {
        vector<Point> v = filtered_bbs.at(i);
        Point p1 = Point(v.at(0).x,v.at(2).y); // xmin and ymin
        Point p2 = Point(v.at(1).x, v.at(3).y);
        Scalar color = Scalar( rand()%255, rand() % 255, rand() % 255 );
        rectangle(area_img, p1, p2, color, 5);
        cout << p1 << " " << p2 << endl;
        cout << Rect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y) << endl;
        cout << "---" << endl;
        Mat roi(image, Rect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y));
        imshow("Roi" + to_string(i), roi);
        imwrite("./images/"+name+to_string(i)+".jpg", roi);
    }
    imshow("Bounding boxes", area_img);



    waitKey(0);
    return 0;
}
