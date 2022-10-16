#include "Bridges.h"
#include "ColorGrid.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>
using namespace std;
using namespace bridges;

// This program implements the mountain paths assignment in C++. See the README file

// function prototypes
ColorGrid *getImage(int *img_arr, int imgWidth, int imgHeight, int maxVal);
int *readData(int *imgWidth, int *imgHeight, int *maxVal, string file_name);
void findPath(int *img_arr, int imgWidth, int imgHeight, int startRow, int maxVal);

int main(int argc, char **argv) {
#ifndef MAKEFILE
	static_assert(false, "Type 'make' to compile this project. --Kerney\n");
#endif
	const int bridges_project_id = 1001;
	const string bridges_username = "Scuttlebuns";
	const string bridges_api_key  = "834608098017";

	// bridges object initialization
	Bridges bridges(bridges_project_id, bridges_username, bridges_api_key);

	// read input elevation data
	int width, height, maxVal;
	int *elev_data;

	// parse command line arguments
	// argument 2 is input file
	string filename = "./san_joaquin.dat";
	if (argc >= 2) filename = argv[1]; //This is how you read a command line parameter
	elev_data = readData(&width, &height, &maxVal, filename);

	//YOU: set title - change it to have all of your last names in it
	bridges.setTitle("Mountain Paths "s + filename + " -Sayre, Voss, Heins: Greedy Algorithms Example");


	//WRK - Added this section
	//Get starting row and column - the greedy algorithm only needs starting row,
	int startRow = 0;
	cout << "Please enter starting row:\n";
	cin >> startRow;

	// find path by applying a greedy algorithm from the left side of the screen to the right
	findPath(elev_data, width, height, startRow, maxVal);

	// get the path written into a color grid for visualization
	ColorGrid *cg = getImage(elev_data, width, height, maxVal);

	// visualize
	bridges.setDataStructure(cg);
	bridges.visualize();

	delete cg; //WRK Fix memory leak in BRIDGES
	delete[] elev_data; //WRK Ditto
}

int *readData(int *width, int *height, int *maxVal, string file_name) {

	ifstream infile(file_name.c_str());

	// read header
	int max = 0, w, h, val;
	infile >> w >> h;

	*width = w;
	*height = h;

	// allocate space for the input data
	// we will use a 1D array
	int size = w * h;
	int *elev_data = new int[size];
	assert(elev_data); //WRK

	// read the elevation data
	for (int k = 0; k < size; k++) {
		infile >>  val;

		// update the max val
		max = (val > max) ? val : max;

		// store  in elevation array
		elev_data[k] = val;
	}
	// record the max value
	*maxVal = max;

	return elev_data;
}

// takes in the processed elevation data and returns a color grid for
// visualization using BRIDGES
ColorGrid *getImage(int *elev_data, int width, int height, int maxVal) {
	// create color grid
	ColorGrid *cg = new ColorGrid(height, width);

	float pixel_val;
	int n = 0, gray;

	// load the elevation data
	for (int j = 0; j < height; j++)
		for (int k = 0; k < width; k++) {
			pixel_val = (float) elev_data[n++];
			if (pixel_val == -1.) {
				//YOU: (DONE) Change the color
				cg->set(j, k, Color(0, 0, 255));
			} else {
				// scale value to be within 0-255, for r,g,b range
				gray = (int)(pixel_val * 255. / maxVal);
				cg->set(j, k, Color(gray, gray, gray));
			}
		}
	return cg;
}

// determines the least effort path through the mountain starting a point on
// the left edge of the image
void findPath(int *elev_data, int imgWidth, int imgHeight, int startRow, int maxVal) {

	if (startRow < 0 || startRow >= imgHeight) {
		cout << "Invalid starting row.\n"; //WRK Let them know
		return;
	}

	int col = 0, row = startRow;
	int pix_addr = startRow * imgWidth + col;

	while (col < imgWidth - 2) {
	vector <int> height_diff(3,10000);
		//YOU: Pick the spot to the right that has the smallest change between our height and their height
		//You will update row to be one higher, one lower, or the same.
		//You will set the elevation at that point to be -1
		if(row - 1 > 0) height_diff.at(0) = abs(elev_data[(row - 1) * imgWidth + (col + 1)] - elev_data[row*imgWidth + col]);
		height_diff.at(1) = abs(elev_data[row * imgWidth + (col + 1)] - elev_data[row*imgWidth + col]);
		if(row + 1 < imgHeight) height_diff.at(2) = abs(elev_data[(row + 1) * imgWidth + (col + 1)] - elev_data[row*imgWidth + col]);

		int smallestHeightDiff = 0;
		if(height_diff.at(0) < height_diff.at(1) and height_diff.at(0) < height_diff.at(2)) smallestHeightDiff = 0;
		else if(height_diff.at(1) < height_diff.at(2) and height_diff.at(1) < height_diff.at(0)) smallestHeightDiff = 1;
		else smallestHeightDiff = 2;
	
		if(smallestHeightDiff == 0) row--;	
		else if(smallestHeightDiff == 2) row++;
		else row += 0;
		elev_data[row * imgWidth + col] = -1;
		//Col always moves to the right
		col++;
	}
}
