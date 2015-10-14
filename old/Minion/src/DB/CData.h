#pragma once


#include <cstdio>						
#include <cstring>
#include <fstream>	
#include <string>
#include <vector>

#include <Eigen/Dense>

#include "DB/CDatabase.h"
#include "Net/CSemaphore/CSemaphore.h"
#include "Util/Util_MagicNumbers.h"

//#define DEBUG_ALIGN
//#define KASPER_WEIGHTS

using namespace Eigen;

const int num_prior_dims_preflop = 9;
const int num_prior_dims_postflop = 11;

std::string PrintPtStr(double* point, int ndims);

////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
// There will be 4(=num_rounds) * arrays of size 4(=max_num_actions) = 16 of these objects    //																		  //
// Usage is provided through CDataInterface													  //	
//																							  //
////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	_max_pts		maximum number of data points (default = 1000)
//	_br				enumStreets value for the betround (used to calculate dim)
//	_nacted			number of times the player  has already acted (used to calculate dim)
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CDataObject
{
public:
	CDataObject(int npts, enumStreetIndices br_ndx, int times_acted, int ndealt, eBetType action_type);
	~CDataObject();

public:
	int			get_npoints()			{ return m_npoints; }
	int			get_ndims()				{ return m_ndims; }
	long		get_handid(long ndx)	{ return m_hand_ids[ndx]; }
	double		get_ndx_profit(int ndx) { return m_profits[ndx]; }
	double*		get_points_array()		{ return m_points.data(); }
	double*		get_profits_array()		{ return m_profits.data(); }
	double**	get_ann_array()			{ return m_data; }

private:
	void	CleanUp();
	void	FeatureNormalize();
	long	GetData(CDatabase* p_db);

	// why so public ?
public:
	int						m_ndims;			// #dimensions
	int						m_npoints;			// #total_data_points
	enumStreetIndices		m_br;
	int						m_nacted;
	int						m_ndealt;
	eBetType				m_action;

public:
	Matrix<double, Dynamic, Dynamic, RowMajor> m_points, diag_factor;
	VectorXd m_profits;
	VectorXd m_weights;
	RowVectorXd dim_means;
	RowVectorXd dim_std;

	double**				m_data;				// data points array
	long*					m_hand_ids;
};