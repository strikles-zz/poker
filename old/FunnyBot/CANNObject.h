#pragma once

#include <cstdlib>						
#include <cstdio>						
#include <cstring>
#include <fstream>	
#include <vector>

#include <ANN/ANN.h>
#include "Util_MagicNumbers.h"
#include "struct_defines.h"


//	k				number of nearest neighbors per query (default 1)
//	mpts			maximum number of data points (default = 1000)
//	br				enumStreets value for the betround (used to calculate dim)
//	timesActed		number of times the player  has already acted (used to calculate dim)
//	eps				is the error bound (default = 0.0)

class CANNObject
{
public:
	
	CANNObject();
	CANNObject(int m_k, int m_maxPts, enumStreets m_br, int m_timesActed, double m_eps);
	~CANNObject();

public:

	// Public interface
	int GetNdxFromVector(std::vector<int>& point);

private:

	void CleanUp();
	void PrintPt(std::ostream &out, ANNpoint p);
	bool ReadPointFromVector(std::vector<int>& vec, ANNpoint p);

private:

	int						k;				// #nearest_neighbors
	int						dim;			// #dimensions
	int						maxPts;			// #max_data_points
	double					eps;			// error epsilon
	int						nPts;			// #data_points

	ANNpointArray			dataPts;		// data points array
	ANNidxArray				nnIdx;			// near neighbor indices array
	ANNdistArray			dists;			// near neighbor distances array
	ANNkd_tree*				kdTree;			// search structure

	enumStreets				br;
	int						timesActed;
};