#pragma once


#include <cstdio>						
#include <cstring>
#include <fstream>	
#include <string>
#include <vector>

#include <ANN/ANN.h>
#include "DB/CData.h"
#include "DB/CDatabase.h"
#include "Net/CSemaphore/CSemaphore.h"
#include "Util/Util_MagicNumbers.h"


////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
// There will be 4(=num_rounds) * arrays of size 4(=max_num_actions) = 16 of these objects    //
// instantiated.																			  //
// Usage is provided through CANNInterface													  //	
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

class CANNObject
{

public:

	CDataObject*	p_data;		

private:

	int				m_k;
	int				m_br;
	int				m_nacted;
	int				m_action;
	long			m_npoints;
	double			m_epsilon;
	bool			m_stable;
	ANNkd_tree*		kd_tree;
	mySemaphore*	ann_semaphore;

public:

	CANNObject(int num_nearest_neighbours, 
				int num_pts, 
				enumStreetIndices br_ndx, 
				int times_acted, 
				eBetType action_type, 
				double eps);

	~CANNObject();

public:
	int		get_k()			{ return m_k; }
	long	get_npoints()	{ return m_npoints; }
	int		get_br()		{ return m_br; }
	int		get_nacted()	{ return m_nacted; }
	bool	get_stable()	{ return m_stable; }
	

public:

	// Public interface
	bool	GetNeighbours(int* hand, ANNidxArray indexes, ANNdistArray dists, eBetType action_type);
	bool	GetKnnIndexesFromVector(RowVectorXd& point, ANNidxArray indexes, ANNdistArray dists);
	int		GetKnnHandIdFromNeighbourNdx(int db_ndx);
	//double	get_avg_profit() { return avg_profit; }

private:
	void	CleanUp();	
	bool	ParseHandToVector(int* hand, RowVectorXd& point);

	// Used in CANNObject
	double	GetAvgProfit(CDatabase* p_db);
};

