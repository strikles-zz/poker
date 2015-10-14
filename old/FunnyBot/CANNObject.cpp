#include "stdafx.h"

#include "CANNObject.h"	
#include "CDatabase.h"


CANNObject::CANNObject(int m_k, int m_maxPts, enumStreets m_br, int m_timesActed, double m_eps)
: k(m_k), maxPts(m_maxPts), br(m_br), timesActed(m_timesActed), eps(m_eps)
{
	if (br == ePreflop)
		dim	= 6 + timesActed;
	else
		dim = 8 + timesActed;

	dataPts = annAllocPts(maxPts, dim);		// allocate data points
	nnIdx = new ANNidx[k];					// allocate near neigh indices
	dists = new ANNdist[k];					// allocate near neighbor dists

	nPts = gDB.FillAnnArray(dataPts, br, timesActed);

	/*
	if (DEBUG_ANN)
	{
		for (int i = 0; i < nPts; i++)
		{
			printf("Data Point: ");
			PrintPt(std::cout, dataPts[i]);
		}
	}
	*/

	// build search structure
	kdTree = new ANNkd_tree(dataPts,		// the data points
							nPts,			// number of points
							dim);			// dimension of space
}

CANNObject::~CANNObject()
{
	CleanUp();
}

void CANNObject::CleanUp()
{
    delete [] nnIdx;							
    delete [] dists;
    delete kdTree;

	// done with ANN
	annClose();	
}

void CANNObject::PrintPt(std::ostream &out, ANNpoint p)			
{
	// print point

	out << "(" << p[0];
	for (int i = 1; i < dim; i++) 
	{
		out << ", " << p[i];
	}

	out << ")" << std::endl;
}

bool CANNObject::ReadPointFromVector(std::vector<int>& vec, ANNpoint p)			
{
	for (size_t i = 0; i < vec.size(); i++) 
	{ 
		p[i] = vec[i];
	}

	return true;
}

int CANNObject::GetNdxFromVector(std::vector<int>& point)
{
	// allocate query point
	ANNpoint queryPt = annAllocPt(point.size());			
	// read query point
	ReadPointFromVector(point, queryPt);

	// echo query point
	if (DEBUG_ANN)
	{
		printf("Query point: ");				
		PrintPt(std::cout, queryPt);
	}

	// search
	kdTree->annkSearch(	queryPt,						// query point
						k,								// number of near neighbors
						nnIdx,							// nearest neighbors (returned)
						dists,							// distance (returned)
						eps);							// error bound

	// print summary
	if (DEBUG_ANN)
	{
		std::cout << "\tNN:\tIndex\tDistance\n";
		for (int i = 0; i < k; i++) 
		{			
			// unsquare distance
			dists[i] = sqrt(dists[i]);			
			std::cout << "\t" << i << "\t" << nnIdx[i] << "\t" << dists[i] << std::endl;
		}
	}					

	annDeallocPt(queryPt);

	return nnIdx[0];
}