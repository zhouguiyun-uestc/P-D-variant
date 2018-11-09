#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include "dem.h"
#include "Node.h"
#include "utils.h"
#include <time.h>
using namespace std;


//The implementation of our variant in the manuscript
int FillDEM_Wei(char* inputFile, char* outputFilledPath)
{
	CDEM DEM;

	double geoTransformArgs[6];

	cout<<"Reading tiff file..."<<endl;

	if (!readTIFF(inputFile, GDALDataType::GDT_Float32, DEM, geoTransformArgs)){
		cout<<"error!"<<endl;
		return 0;
	}

	int width = DEM.Get_NX();

	int height = DEM.Get_NY();

	cout<<"DEM Width:"<<width<<"  Height:"<<height<<endl;

	time_t timeStart, timeEnd;

	timeStart = time(NULL);

	//A transient surface that converging to the depression-filled DEM

	CDEM W;

	W.SetHeight(height);

	W.SetWidth(width);

	if (!W.Allocate()){
		printf("Failed to allocate memory!\n");
		return 0;
	}

	//Stage 1:Initialization of the surface to a huge positive number m

	queue<Node> P;

	const float m = 99999.f;

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			if (DEM.is_NoData(row, col))
			{
				W.Set_Value(row, col, DEM.asFloat(row, col));
			}
			else
			{
				bool isborder = false;

				for (int i = 0; i < 8; ++i)
				{
					int iRow = Get_rowTo(i, row);
					int iCol = Get_colTo(i, col);
					if (!DEM.is_InGrid(iRow, iCol) || DEM.is_NoData(iRow, iCol))
					{
						isborder = true;
						break;
					}
				}

				if (isborder)
				{
					W.Set_Value(row, col, DEM.asFloat(row, col));
					Node node(row, col);
					P.push(node);
				}
				else
				{
					W.Set_Value(row, col, m);
				}

			} //End if

		} //End for

	} //End for

	cout << "filling depressions..." << endl;

	//Stage 2:Removal of excess water
	
	queue<Node> Q;

    //We assume epsilon is 0.f
	const float epsilon = 0.f;		

	while (!P.empty() || !Q.empty()) 
	{
		Node node(-1, -1);

		if (!P.empty()) 
		{
			node = P.front();
			P.pop();
		}
		else 
		{
			node = Q.front();
			Q.pop();
			if (DEM.asFloat(node.row, node.col) >= W.asFloat(node.row, node.col)) continue;
		}

		float spill = W.asFloat(node.row, node.col);

		for (int i = 0; i < 8; ++i)
		{
			int irow = Get_rowTo(i, node.row);
			int icol = Get_colTo(i, node.col);

			if (!DEM.is_InGrid(irow, icol)||DEM.asFloat(irow, icol) >= W.asFloat(irow, icol)) continue;

			if (DEM.asFloat(irow, icol) >= spill + epsilon)
			{                  
				//Dried cell
				W.Set_Value(irow, icol, DEM.asFloat(irow, icol));
				Node temp(irow, icol);
				P.push(temp);
			}
			else if (W.asFloat(irow, icol) > spill + epsilon)
			{                                   
				//Remove excess water
				W.Set_Value(irow, icol, spill + epsilon);
				Node temp(irow, icol);
				Q.push(temp);
			}

		} //End for

	} //End while

	

	timeEnd = time(NULL);

	double consumeTime = difftime(timeEnd, timeStart);

	cout<<"Time used:"<<consumeTime<<" seconds"<<endl;

	double min, max, mean, stdDev;

	calculateStatistics(W, &min, &max, &mean, &stdDev);

	CreateGeoTIFF(outputFilledPath, W.Get_NY(), W.Get_NX(), 
		(void *)W.getDEMdata(),GDALDataType::GDT_Float32, geoTransformArgs,
		&min, &max, &mean, &stdDev, -9999);

	return 1;
}
