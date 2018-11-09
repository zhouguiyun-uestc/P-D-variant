#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <algorithm>
#include "dem.h"
#include "Node.h"
#include "utils.h"
#include <time.h>
#include <list>
#include <unordered_map>

using namespace std;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::fstream;
using std::ifstream;
using std::priority_queue;
using std::binary_function;

int FillDEM_Wei(char* inputFile, char* outputFilledPath);
int FillDEM_WT(char* inputFile, char* outputFilledPath);

//compute stats for a DEM
void calculateStatistics(const CDEM& dem, double* min, double* max, double* mean, double* stdDev)
{
	int width = dem.Get_NX();
	int height = dem.Get_NY();

	int validElements = 0;
	double minValue, maxValue;
	double sum = 0.0;
	double sumSqurVal = 0.0;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (!dem.is_NoData(row, col))
			{
				double value = dem.asFloat(row, col);
				
				if (validElements == 0)
				{
					minValue = maxValue = value;
				}
				validElements++;
				if (minValue > value)
				{
					minValue = value;
				}
				if (maxValue < value)
				{
					maxValue = value;
				}

				sum += value;
				sumSqurVal += (value * value);
			}
		}
	}

	double meanValue = sum / validElements;
	double stdDevValue = sqrt((sumSqurVal / validElements) - (meanValue * meanValue));
	*min = minValue;
	*max = maxValue;
	*mean = meanValue;
	*stdDev = stdDevValue;
}
int main(int argc, char* argv[])
{


	argc = 4;
	argv[1] = "WT";
	argv[2] = "F:\\TestData\\DEM Data\\Source File\\SizeBig\\grant-3m.tif";
	argv[3] = "F:\\TestData\\DEM Data\\Source File\\SizeBig\\grant-3m_WT_10.tif";

	if (argc < 4) {
		cout << "Fill DEM usage: FillDEM fillingMethod inputfileName outputfileName" << endl;
		cout << "WT: using the implementation of W&T variant in the manuscript" << endl;
		cout << "Wei: using the implementation of our variant in the manuscript" << endl;
		cout << "\nFor example, FillDEM WT f:\\dem\\dem.tif f:\\dem\\dem_filled.tif" << endl;
		return 1;
	}

	string path(argv[2]);

	string outputFilledPath(argv[3]);

	size_t index = path.find(".tif");

	if (index == string::npos) {
		cout << "Input file name should have an extension of '.tif'" << endl;
		return 1;
	}

	char* method = argv[1];

	string strFolder = path.substr(0, index);

	if (strcmp(method, "Wei") == 0)
	{
		int result = FillDEM_Wei(&path[0], &outputFilledPath[0]); 
	}
	else if (strcmp(method, "WT") == 0)
	{
		int result = FillDEM_WT(&path[0], &outputFilledPath[0]);
	}
	else
	{
		cout << "Unknown filling method" << endl;
	}

	std::cout << "\nPress any key to exit ..." << endl;

	getchar();
	return 0;
}


