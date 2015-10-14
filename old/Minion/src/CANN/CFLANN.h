#pragma once


#include <cstdio>						
#include <cstring>
#include <fstream>	
#include <string>
#include <vector>

#include <ANN/ANN.h>
#include "DB/CDatabase.h"
#include "Net/CSemaphore/CSemaphore.h"
#include "Util/Util_MagicNumbers.h"


const int num_prior_dims_preflop = 9;
const int num_prior_dims_postflop = 11;


////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
// There will be 4(=num_rounds) * arrays of size 4(=max_num_actions) = 16 of these objects    //
// instantiated.																			  //
// Usage is provided through CFLANNInterface													  //	
//																							  //
////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	k				number of nearest neighbors per query (default 1)
//	mpts			maximum number of data points (default = 1000)
//	br				enumStreets value for the betround (used to calculate dim)
//	timesActed		number of times the player  has already acted (used to calculate dim)
//	eps				is the error bound (default = 0.0)
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CFLANNObject
{
public:

	CFLANNObject();
	CFLANNObject(int num_nearest_neighbours, int num_pts, enumStreetIndices br_ndx, int times_acted, eBetType action_type, double eps, CDatabase* p_db);
	~CFLANNObject();

public:

	mySemaphore* ann_semaphore;

	// Public interface
	double	GetProfit(int ndx, CDatabase* p_db);
	bool	GetNeighbours(int* hand, ANNidxArray indexes, ANNdistArray dists, eBetType action_type);
	bool	GetKnnIndexesFromVector(std::vector<int>& point, ANNidxArray indexes, ANNdistArray dists);
	int		GetKnnHandIdFromNeighbourNdx(int db_ndx);
	int		get_total_points() { return total_points; }
	double	get_avg_profit() { return avg_profit; }

private:

	void	CleanUp();
	void	PrintPt(std::ostream &out, ANNpoint point);
	std::string PrintPtStr(ANNpoint point, int ndims);
	
	bool	ParseHandToVector(int* hand, std::vector<int>& point);
	bool	ReadPointFromVector(std::vector<int>& vec, ANNpoint point);

	// Used in CFLANNObject
	long	FillAnnArray(ANNpointArray& dataPts, enumStreetIndices br_ndx, int times_acted, int total_points, CDatabase* p_db);
	double	GetAvgProfit(enumStreetIndices br_ndx, int times_acted, CDatabase* p_db);

public:

	double					avg_profit;
	double					epsilon;				// error epsilon
	int						k;						// #nearest_neighbors
	int						num_dims;				// #dimensions
	int						total_points;			// #total_data_points
	int						total_times_acted;
	enumStreetIndices		br_index;
	eBetType				action_filter;

private:

	ANNpointArray			knn_data;				// data points array
	ANNkd_tree*				kd_tree;				// search structure
	int*					knn_hand_ids;
};

class CFLANNInterface
{
public:

	CFLANNInterface();
	~CFLANNInterface();

public:

	// Public interface
	
	
	CFLANNObject*	GetAnnQueryObject(int br_ndx, int times_acted, eBetType action_filter);

	// Used in CFLANNInterface
	eBetType	GetAction(enumStreetIndices br_ndx, int times_acted, int ndx, CDatabase* p_db);
	double		GetPot(enumStreetIndices br_ndx, int times_acted, int ndx, CDatabase* p_db);
	double		GetProfit(int hand_id, CDatabase* p_db);
	double		GetTableAvgProfit(enumStreetIndices br_ndx, int times_acted);

	void Init();
	void Clear();

private:

	int max_pts;
	double eps;

	CFLANNObject* ann_obj[eRoundIndices][k_max_actions][eTotalActions];
};

extern CFLANNInterface gANN;