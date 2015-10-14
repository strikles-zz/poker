#include <cassert>

#include "StructDefines/structdefines.h"

#include "CANN/CANN.h"
#include "CANN/CScaling.h"

#include "Config/CPreferences.h"
#include "DB/CDatabase.h"

#include "Util/Util.h"
#include "Util/Util_logging.h"

#include "Master/CGlobalVars.h"




CANNObject::CANNObject(int num_nearest_neighbours, int total_pts, enumStreetIndices br_ndx, int times_acted, eBetType action, double eps) : 
	m_k(total_pts < num_nearest_neighbours ? total_pts : num_nearest_neighbours),  
	m_npoints(total_pts),
	m_br(br_ndx), 
	m_nacted(times_acted), 
	m_epsilon(eps),
	m_action(action),
	m_stable(true)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	p_data = gVars.GetDataObject(br_ndx, times_acted, action);
	if(p_data)
	{
		ann_semaphore = new mySemaphore(string("ANN"), 1);

		// scale teh data object
		p_data->m_points = p_data->m_points * p_data->diag_factor;

		// build search structure
		kd_tree = new ANNkd_tree(p_data->get_ann_array(),			// the data points
								p_data->get_npoints(),				// number of points	
								p_data->get_ndims());				// dimension of space
	}
}

CANNObject::~CANNObject()
{
	CleanUp();
}

void CANNObject::CleanUp()
{
	if(m_stable)
	{
		delete kd_tree;
		delete ann_semaphore;
	}

	// done with ANN
	annClose();	
}



bool CANNObject::ParseHandToVector(int* hand, RowVectorXd& point)
{
	int n = 0;
	// add teh 1 for teh intercept
	point(0) = 1.0;
	// fill the actions 
	if(m_nacted > 0)
	{
		for(int action_ndx = 1; action_ndx <= m_nacted; action_ndx++)
		{
			n = action_ndx;
			point(action_ndx) = (double) hand[eDim_Action1 + action_ndx - 1];
		}
	}
	// Fill a vector with the remaining various query point coordinates
	int index_stop = eDim_End;
	if((enumStreetIndices)m_br == ePreflopIndex)
	{
		index_stop = eDim_PF_End;
		for (int i = eDim_Start; i <= index_stop; i++)
		{
			n++;
			point(n) = (double) hand[i];
		}

	}
	else
	{
		for (int i = eDim_Start; i <= index_stop; i++)
		{
			n++;
			point(n) = (double) hand[i];
		}
	}

	if(0 == point.cols())
		return false;

	return true;
}

bool CANNObject::GetKnnIndexesFromVector(RowVectorXd& point, ANNidxArray indexes, ANNdistArray dists)
{
	//assert(num_dims == point.size());
	if(!(p_data->get_ndims() == point.cols() && point.cols() > 0 && p_data->get_npoints() > 0))
	{
		gLog.WriteLog(eSeverityDebug, eCatANN, "Error Point.size() %d does not match num_dims %d\n", point.cols(), p_data->get_ndims());
		return false;
	}
	else
	{
		ann_semaphore->lock();

		// scale teh query point
		#ifndef KASPER_WEIGHTS
			point -= p_data->dim_means;
			point = point.array() * p_data->dim_std.array();
		#endif
		point *= p_data->diag_factor;

		// search
		kd_tree->annkSearch(point.data(),					// query point
							m_k,								// number of near neighbors**
							indexes,						// nearest neighbors (returned)
							dists,							// distance (returned)
							epsilon);						// error bound

		// print summary
		#if defined _DEBUG
			gLog.WriteLog(eSeverityInfo, eCatANN, "\tNN:\tIndex\tDistance\n");
			for(int i = 0; i < m_k; i++) 
			{			
				gLog.WriteLog(eSeverityInfo, eCatANN, "\t %d \t %d \t %lf\n", i, indexes[i], dists[i]);
			}
		#endif

		ann_semaphore->unlock();
	}

	return true;
}

bool CANNObject::GetNeighbours(int* hand, ANNidxArray indexes, ANNdistArray dists, eBetType action_type)
{
	enumStreetIndices br_ndx = (enumStreetIndices) hand[eDim_brNdx];
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(hand[eDim_timesActed] >= 0);

	RowVectorXd query_point = RowVectorXd::Zero(p_data->get_ndims());
	if(!ParseHandToVector(hand, query_point))
	{
		gLog.WriteLog(eSeverityError, eCatANN, "Error: query_point.size() == 0\n");
		return false;
	}

	if (m_stable)
	{
		GetKnnIndexesFromVector(query_point, indexes, dists);
		return true;
	}

	return false;
}

int CANNObject::GetKnnHandIdFromNeighbourNdx(int db_ndx)
{
	assert(db_ndx != -1);
	int hand_id = p_data->get_handid(db_ndx);
	assert(hand_id != -1);

	return hand_id;
}


