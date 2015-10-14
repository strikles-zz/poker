#include <cassert>

#include "StructDefines/structdefines.h"

#include "CANN/CFLANN.h"
#include "CANN/CScaling.h"

#include "Config/CPreferences.h"
#include "DB/CDatabase.h"

#include "Util/Util.h"
#include "Util/Util_logging.h"




CFLANNObject::CFLANNObject(int num_nearest_neighbours, int total_pts, enumStreetIndices br_ndx, int times_acted, eBetType action_type, double eps, CDatabase* p_db) : 
	k(total_pts < num_nearest_neighbours ? total_pts : num_nearest_neighbours), 
	total_points(total_pts), 
	br_index(br_ndx), 
	total_times_acted(times_acted), 
	epsilon(eps),
	action_filter(action_type)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	if(br_ndx == ePreflopIndex)
		num_dims = num_prior_dims_preflop + times_acted;
	else
		num_dims = num_prior_dims_postflop + times_acted;

	ann_semaphore = new mySemaphore(string("ANN"), 1);

	// allocate data points
	knn_data = annAllocPts(total_points, num_dims+1);
	// allocate hand_ids for corresponding data points
	knn_hand_ids = new int[total_points];
	for(int i = 0; i < total_points; i++)
		knn_hand_ids[i] = -1;

	//ann_semaphore->lock();
	FillAnnArray(knn_data, br_ndx, times_acted, total_points, p_db);
	//ann_semaphore->unlock();
	gLog.WriteLog(eSeverityInfo, eCatANN, "br%d_%d: %8s - %8d points\n", br_ndx+1, times_acted, bets_str[action_type], total_points);
	

#if defined _DEBUG && defined DEBUG_ALIGN
	for(int i = 0; i < total_points; i++)
	{
		double progress_percent = ((total_points > 0) ? ((100.0 * (i+1))/total_points) : 0.0);
		gLog.WriteLogNoHeading(eSeverityInfo, eCatANN, "Data Point : %d/%d [%.2lf %%] - %s\n", 
												i+1, total_points, progress_percent, PrintPtStr(knn_data[i], num_dims).c_str());
	}
#endif

	// build search structure
	kd_tree = new ANNkd_tree(knn_data,		// the data points
							total_points,	// number of points	
							num_dims);		// dimension of space
}

CFLANNObject::~CFLANNObject()
{
	CleanUp();
}

void CFLANNObject::CleanUp()
{
	delete [] knn_hand_ids;
	delete kd_tree;
	delete ann_semaphore;

	// done with ANN
	annClose();	
}

long CFLANNObject::FillAnnArray(ANNpointArray& data_points_array, enumStreetIndices br_ndx, int times_acted, int total_points, CDatabase* p_db)
{
	/*
		Fills the data array and returns the number of points inserted
	*/

	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	std::string query_str = CDatabaseInterface::ConstructSelectQueryString(br_ndx, times_acted, action_filter);

	//p_db->CheckDB(br_ndx, times_acted);

	CDatabaseQuery db_query(p_db);
	db_query.Select(query_str.c_str());

	// Get the actual values
	for(int row = 0; row < db_query.get_numrows(); row++)
	{
		ANNpoint current_point = data_points_array[row];
		for(int column = 0; column < db_query.get_numcolumns(); column++)
		{
			// Get the hand id
			if(0 == column)
			{
				knn_hand_ids[row] = atoi(PQgetvalue(db_query.get_pgresult(), row, column));
			}
			// the dimension ndx will eq the column ndx-1 
			// since we prefix the sql query with hand id which is not a dim and is handled above
			else 
			{
				double tmp = atof(PQgetvalue(db_query.get_pgresult(), row, column));
				//current_point[column-1] = tmp;

				// Start 1 column after the hand_id and after the actions
				int dim_scaling_start = eDim_Start-1 + column - times_acted;
				if (0 <= column - times_acted)
				{
					//data_points_array[row][column-1] = tmp;

					if (br_ndx == ePreflopIndex) 
						current_point[column-1] = CScaling::get_dim_scale_preflop(dim_scaling_start, tmp);
					else 
						current_point[column-1] = CScaling::get_dim_scale_postflop(dim_scaling_start, tmp);
					
				}
			}
		}
	}

	avg_profit = GetAvgProfit(br_ndx, times_acted, p_db);
	long numrows = db_query.get_numrows();

	return numrows;
}

std::string CFLANNObject::PrintPtStr(ANNpoint point, int ndims)			
{
	// print point
	std::string out_str = string("(") + ToString(point[0]);
	for(int i = 1; i < ndims; i++) 
		out_str += (", " + ToString(point[i]));
	out_str += string(")");

	return out_str;
}

void CFLANNObject::PrintPt(std::ostream &out, ANNpoint point)			
{
	// print point
	out << "(" << point[0];
	for(int i = 1; i < num_dims; i++) 
		out << ", " << point[i];

	out << ")" << endl;
}

bool CFLANNObject::ParseHandToVector(int* hand, std::vector<int>& point)
{
	// fill the actions 
	if(total_times_acted >= 1)
	{
		for(int i = 1; i <= total_times_acted; i++)
			point.push_back(hand[eDim_Action1 + i - 1]);
	}

	// Fill a vector with the remaining various query point coordinates
	int index_stop = eDim_End;
	if((enumStreetIndices)br_index == ePreflopIndex)
	{
		index_stop = eDim_PF_End;
		for (int i = eDim_Start; i <= index_stop; i++)
		{
			//query_point.push_back(hand[i]);
			point.push_back(CScaling::get_dim_scale_preflop(i, hand[i]));
		}

	}
	else
	{
		for (int i = eDim_Start; i <= index_stop; i++)
		{
			//query_point.push_back(hand[i]);
			point.push_back(CScaling::get_dim_scale_postflop(i, hand[i]));
		}
	}

	if(point.size() == 0)
		return false;

	return true;
}

bool CFLANNObject::ReadPointFromVector(std::vector<int>& vec, ANNpoint point)			
{
	for(size_t i = 0; i < vec.size(); i++) 
		point[i] = vec[i];

	return true;
}


bool CFLANNObject::GetKnnIndexesFromVector(std::vector<int>& point, ANNidxArray indexes, ANNdistArray dists)
{
	//assert(num_dims == point.size());
	if(!(num_dims == point.size() && point.size() > 0 && total_points > 0))
	{
		gLog.WriteLog(eSeverityDebug, eCatANN, "Error Point.size() %d does not match num_dims %d\n", point.size(), num_dims);
		return false;
	}
	else
	{
		ann_semaphore->lock();

		// allocate query point
		ANNpoint query_point = annAllocPt((int)point.size());			
		// read query point
		ReadPointFromVector(point, query_point);

		
		// search
		kd_tree->annkSearch(query_point,					// query point
							k,								// number of near neighbors**
							indexes,						// nearest neighbors (returned)
							dists,							// distance (returned)
							epsilon);						// error bound

		// print summary
#if defined _DEBUG && defined DEBUG_ALIGN
		gLog.WriteLog(eSeverityInfo, eCatANN, "\tNN:\tIndex\tDistance\n");
		for(int i = 0; i < k; i++) 
		{			
			// unsquare distance ? why ?
			//knn_dists[i] = sqrt(knn_dists[i]);			
			gLog.WriteLog(eSeverityInfo, eCatANN, "\t %d \t %d \t %lf\n", i, knn_indexes[i], knn_dists[i]);
		}
#endif
		annDeallocPt(query_point);

		ann_semaphore->unlock();
	}

	return true;
}

bool CFLANNObject::GetNeighbours(int* hand, ANNidxArray indexes, ANNdistArray dists, eBetType action_type)
{
	enumStreetIndices br_ndx = (enumStreetIndices) hand[eDim_brNdx];
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(hand[eDim_timesActed] >= 0);

	std::vector<int> query_point;
	if(!ParseHandToVector(hand, query_point))
	{
		gLog.WriteLog(eSeverityError, eCatANN, "Error: query_point.size() == 0\n");
		return false;
	}

	// Find the right ANN Object
	CFLANNObject* p_search = this;
	if (get_total_points() > 0)
	{
		GetKnnIndexesFromVector(query_point, indexes, dists);
		return true;
	}

	return false;
}

int CFLANNObject::GetKnnHandIdFromNeighbourNdx(int db_ndx)
{
	assert(db_ndx != -1);
	int hand_id = knn_hand_ids[db_ndx];
	assert(hand_id != -1);

	//gLog.WriteLog(eSeverityInfo, eCatANN, "knn_ndx:%d corresponds to hand_id:%d\n", db_ndx, hand_id);

	return hand_id;
}

// DB
double CFLANNObject::GetProfit(int ndx, CDatabase* p_db)
{
	assert(br_index >= ePreflopIndex && br_index < eRoundIndices);
	assert(total_times_acted >= 0);

	int hand_id = GetKnnHandIdFromNeighbourNdx(ndx);

	std::string query = "SELECT profit"
						" FROM profits "
						" WHERE profits.hand_id = " + ToString(hand_id) + 
						" AND profits.profit IS NOT NULL";

	CDatabaseQuery db_query(p_db);
	db_query.Select(query.c_str());

	if(db_query.get_numcolumns() != 1  || db_query.get_numrows() != 1)
	{
		gLog.WriteLog(eSeverityError, eCatDBQuery, "FIXME: Query: %s\n", query.c_str());
		gLog.WriteLog(eSeverityError, eCatDBQuery, "FIXME: profit: %s\n", PQgetvalue(db_query.get_pgresult(), 0, 0));
		return -DBL_MAX;
	}

	double profit = atof(PQgetvalue(db_query.get_pgresult(), 0, 0));

	return profit;
}

// DB
double CFLANNObject::GetAvgProfit(enumStreetIndices br_ndx, int times_acted, CDatabase* p_db)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	std::string db_table_str = "br" + ToString(br_ndx+1) + "_" + ToString(times_acted);
	std::string query = "SELECT avg(profit)"
						" FROM profits"
						" WHERE hand_id IN( select hand_id from "+ db_table_str +" );";

	CDatabaseQuery db_query(p_db);
	db_query.Select(query.c_str());
	if (gPrefs.db.get_debug())
	{
		//gLog.WriteLog(eSeverityInfo, eCatANN, "Pts:%d, nDims:%d\n", db_query.get_numrows(), db_query.get_numcolumns());
		if(!(db_query.get_numcolumns() == 1 && db_query.get_numrows() == 1))
		{
			gLog.WriteLog(eSeverityError, eCatDBQuery, "FIXME: Query: %s\n", query.c_str());
			gLog.WriteLog(eSeverityError, eCatDBQuery, "FIXME: profit: %s\n", PQgetvalue(db_query.get_pgresult(), 0, 0));

			return -DBL_MAX;
		}
	}

	double profit = atof(PQgetvalue(db_query.get_pgresult(), 0, 0));

	return profit;
}




CFLANNInterface::CFLANNInterface()
{
	Init();
}

CFLANNInterface::~CFLANNInterface()
{
	Clear();
}

// DB
void CFLANNInterface::Init()
{
	//	maxPts			maximum number of data points
	max_pts = gPrefs.knn.get_max_pts();
	//	eps	
	eps = gPrefs.knn.get_eps();

	CDatabase* p_db = new CDatabase();

	// 4 rounds
	for(int i = 0; i < eRoundIndices; i++)
	{
		// 4 max actions
		for(int j = 0; j < k_max_actions; j++)
		{
			// Total actions
			for(int k = eCheck; k < eTotalActions; k++)
			{
				std::string query_str;
				query_str = "SELECT COUNT(*) FROM br" + ToString(i+1) + "_" + ToString(j) + 
											" as a INNER JOIN profits ON a.hand_id = profits.hand_id"
											" WHERE dim_action" + ToString(j+1) + " = " + ToString(k) +
											" AND profits.profit IS NOT NULL";

				/*
				query_str = "SELECT COUNT(*) FROM br" + ToString(i+1) + "_" + ToString(j) + 
											" as a INNER JOIN profits ON a.hand_id = profits.hand_id"
											" WHERE profits.profit IS NOT NULL";
											*/

				CDatabaseQuery db_query(p_db);
				db_query.Select(query_str.c_str());
				if(db_query.get_pgresult())
				{
					long total_pts = strtol(PQgetvalue(db_query.get_pgresult(), 0, 0), NULL, 0);
					ann_obj[i][j][k] = new CFLANNObject(gPrefs.knn.get_num_neighbours(i), total_pts, (enumStreetIndices)i, j, (eBetType)k, eps, p_db);
				}
				else
					ann_obj[i][j][k] = NULL;
			}
		}
	}

	delete p_db;
}

void CFLANNInterface::Clear()
{
	for(int i = 0; i < eRoundIndices; i++)
	{
		for(int j = 0; j < k_max_actions; j++)
		{
			for(int k = 0; k <= eTotalActions; k++)
			{
				delete ann_obj[i][j][k];
			}
		}
	}
}

CFLANNObject* CFLANNInterface::GetAnnQueryObject(int br_ndx, int times_acted, eBetType action_filter)
{
	if(times_acted >= k_max_actions)
		times_acted = k_max_actions -1;

	// Deal with the case when no object was contructed 1st
	if((!ann_obj[br_ndx][times_acted][action_filter]) || (ann_obj[br_ndx][times_acted][action_filter]->get_total_points() < gPrefs.knn.get_num_neighbours(br_ndx)))
	{
		// fatal, can't recover
		if(times_acted <= 0)
		{
		#ifdef _DEBUG
			gLog.WriteLog(eSeverityError, eCatANN, "ERROR: No Stable Object exists for %d_%d -> giving up\n", br_ndx, times_acted);
		#endif
			return NULL;
		}
		// try the obj corresponding to times_acted-1
		else
		{
		#ifdef _DEBUG
			gLog.WriteLog(eSeverityDebug, eCatANN, "Warning: Unstable object %d_%d -> trying %d_%d\n", br_ndx, times_acted, br_ndx, times_acted-1);
		#endif		
			return GetAnnQueryObject(br_ndx, times_acted - 1, action_filter);
		}
	}
	
	return ann_obj[br_ndx][times_acted][action_filter];
}


// DB
eBetType CFLANNInterface::GetAction(enumStreetIndices br_ndx, int times_acted, int db_ndx, CDatabase* p_db)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	if (times_acted >= k_max_actions)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "FIXME: I have acted more than max = 4!");
		times_acted = k_max_actions-1;
	}

	int hand_id = -1;
	// Only used in generic soup ???
	CFLANNObject* p_search = GetAnnQueryObject(br_ndx, times_acted, eTotalActions);
	if (!p_search)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "ERROR: Could not get an ann query object\n");
		return eBet_Unknown;
	}

	hand_id = p_search->GetKnnHandIdFromNeighbourNdx(db_ndx);

	std::string db_table_str	= "br" + ToString(br_ndx+1) + "_" + ToString(times_acted);
	std::string query_str		= "SELECT dim_action" + ToString(times_acted+1) + 
									" FROM " + db_table_str + 
									" WHERE hand_id = " + ToString(hand_id);

	CDatabaseQuery db_query(p_db);
	bool query_status = db_query.Select(query_str.c_str());
	assert(db_query.get_numrows() == 1 && db_query.get_numcolumns() == 1);
	eBetType decision = (eBetType) atoi(PQgetvalue(db_query.get_pgresult(), 0, 0));

	return decision;
}

// DB
double CFLANNInterface::GetPot(enumStreetIndices br_ndx, int times_acted, int db_ndx, CDatabase* p_db)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	if (times_acted >= k_max_actions)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "FIXME: I have more times acted than max actions\n");
		times_acted = k_max_actions-1;
	}

	int hand_id = -1;
	// Only used in generic soup ???
	CFLANNObject* p_search = GetAnnQueryObject(br_ndx, times_acted, eTotalActions);
	if (!p_search)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "Error: Could not get an ann query object\n");
		return -DBL_MAX;
	}

	hand_id = p_search->GetKnnHandIdFromNeighbourNdx(db_ndx);

	std::string db_table_str	= "br" + ToString(br_ndx + 1) + "_" + ToString(times_acted);
	std::string query_str		= "SELECT dim_pot"
									" FROM " + db_table_str + 
									" WHERE hand_id = " + ToString(hand_id);

	CDatabaseQuery db_query(p_db);
	bool query_status = db_query.Select(query_str.c_str());
	assert(db_query.get_numrows() == 1 && db_query.get_numcolumns() == 1);

	double pot = atof(PQgetvalue(db_query.get_pgresult(), 0, 0));

	return pot;
}


double CFLANNInterface::GetTableAvgProfit(enumStreetIndices br_ndx, int times_acted)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);
	assert(times_acted >= 0);

	double avg_profit = -DBL_MAX;
	// Only used in generic soup ???
	CFLANNObject* p_search = GetAnnQueryObject(br_ndx, times_acted, eTotalActions);
	if (!p_search)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "ERROR: could not get an ann query object\n");
		return -DBL_MAX;
	}
	avg_profit = p_search->get_avg_profit();

	return avg_profit;
}