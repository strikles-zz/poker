#include "Master/CGlobalVars.h"

#include "Config/CPreferences.h"
#include "Util/Util.h"
#include "Util/Util_logging.h"
#include "Util/Util_MagicNumbers.h"

CGlobalVars::CGlobalVars(int ndealt)
{
	Init(ndealt);
}

CGlobalVars::~CGlobalVars()
{
	Clear();
}


void CGlobalVars::Init(int ndealt)
{
	//	maxPts			maximum number of data points
	m_max_pts = gPrefs.knn.get_max_pts();
	//	eps	
	eps = gPrefs.knn.get_eps();

	// 4 rounds
	for(int nacted = 0; nacted < eRoundIndices; nacted++)
	{
		// 4 max actions
		for(int br = 0; br < k_max_actions; br++)
		{
			// Total actions
			for(int action = eCheck; action < eTotalActions; action++)
			{
				m_data_obj[nacted][br][action]	= NULL;
				m_ann_obj[nacted][br][action]	= NULL;
				// Get Number of points
				std::string query_str = "SELECT COUNT(*) FROM br" + ToString(br+1) + "_" + ToString(nacted) + 
											" as a INNER JOIN profits ON a.hand_id = profits.hand_id"
											" WHERE dim_action" + ToString(nacted+1) + " = " + ToString(action) +
											" AND dim_players_dealt = " + ToString(ndealt) +  
											" AND profits.profit IS NOT NULL";

				CDatabase p_db;
				CDatabaseQuery db_query(&p_db);
				db_query.Select(query_str.c_str());
				long total_pts = -1L;
				if(db_query.get_pgresult())
				{
					total_pts = strtol(PQgetvalue(db_query.get_pgresult(), 0, 0), NULL, 0);
					
					// Save our object
					if(total_pts >= 500L)
					{
						m_data_obj[nacted][br][action]	= new CDataObject(total_pts, (enumStreetIndices)br, nacted, ndealt, (eBetType)action);
						m_ann_obj[nacted][br][action]	= new CANNObject(gPrefs.knn.get_num_neighbours(br), total_pts, (enumStreetIndices)br, nacted, (eBetType)action, eps);
					}
					else
						gLog.WriteLog(eSeverityDebug, eCatData, "Warning: %d - br%d_%d - %s - Not enough points (%d) to build object\n", ndealt, br+1, nacted, bets_str[action], total_pts);
				}
			}
		}
	}
}

void CGlobalVars::Clear()
{
	for(int nacted = 0; nacted < eRoundIndices; nacted++)
	{
		for(int br = 0; br < k_max_actions; br++)
		{
			for(int action = 0; action < eTotalActions; action++)
			{
				if(m_data_obj[nacted][br][action])
					delete m_data_obj[nacted][br][action];
				if(m_ann_obj[nacted][br][action])
					delete m_ann_obj[nacted][br][action];
			}
		}
	}
}

CDataObject* CGlobalVars::GetDataObject(int br, int nacted, eBetType action)
{
	if(k_max_actions <= nacted)
		nacted = k_max_actions -1;

	// Deal with the case when object wasnt constructed or doesnt have enough points
	if(!m_data_obj[nacted][br][action])
		return NULL;
	
	return m_data_obj[nacted][br][action];
}


CANNObject* CGlobalVars::GetAnnQueryObject(int br_ndx, int times_acted, eBetType action_filter)
{
	if(times_acted >= k_max_actions)
		times_acted = k_max_actions -1;

	if(m_ann_obj[times_acted][br_ndx][action_filter])
		return m_ann_obj[times_acted][br_ndx][action_filter];
	else
	{
		// fatal, can't recover
		if(times_acted <= 0)
		{
			#ifdef _DEBUG
				gLog.WriteLog(eSeverityError, eCatANN, "ERROR: No Stable Object exists for %d_%d -> giving up\n", br_ndx+1, times_acted);
			#endif
			return NULL;
		}
		// try the obj corresponding to times_acted-1
		else
		{
			#ifdef _DEBUG
				gLog.WriteLog(eSeverityDebug, eCatANN, "Warning: Unstable object %d_%d -> trying %d_%d\n", br_ndx+1, times_acted, br_ndx+1, times_acted-1);
			#endif		
			return GetAnnQueryObject(br_ndx, times_acted - 1, action_filter);
		}
	}
}


