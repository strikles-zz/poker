#pragma once

#include <set>

#include "Config/CPreferences.h"
#include "DB/CData.h"
#include "CANN/CANN.h"
#include "Util/Util_logging.h"

class CGlobalVars
{
public:
	CGlobalVars(int ndealt);
	~CGlobalVars();
	
public:
	// gvars
	std::set<int> hopper_rebuy_buffer;

	// hmm
public:
	// Public interface
	CDataObject*	GetDataObject(int br_ndx, int times_acted, eBetType action_filter);
	CANNObject*		GetAnnQueryObject(int br_ndx, int times_acted, eBetType action_filter);

	// Used in CANNInterface
	eBetType	GetAction(enumStreetIndices br_ndx, int times_acted, int ndx, CDatabase* p_db);
	double		GetPot(enumStreetIndices br_ndx, int times_acted, int ndx, CDatabase* p_db);
	double		GetProfit(int hand_id, CDatabase* p_db);
	double		GetTableAvgProfit(enumStreetIndices br_ndx, int times_acted);

private:
	double eps;
	int m_max_pts;
	// Space hogs
	CDataObject*	m_data_obj[k_max_actions][eRoundIndices][eTotalActions];
	CANNObject*		m_ann_obj[k_max_actions][eRoundIndices][eTotalActions];

private:
	void Init(int ndealt);
	void Clear();
};

extern CGlobalVars gVars;