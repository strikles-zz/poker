#include <vector>
#include <cassert>
#include <ctime>

#include "Master/CDominvs.h"

#include "Master/CGlobalVars.h"
#include "Config/CPreferences.h"
#include "DB/CDatabase.h"
#include "StructDefines/structdefines.h"

#include "Util/Util.h"
#include "Util/Util_logging.h"



CDominvs::CDominvs() : 
	bettype(eBet_Unknown), 
	actionType(eAction_Unknown)
{
	srand(time(NULL));
}

//RandomAction
eBetType CDominvs::RandomAction(int* hand, bool just_raises = true, int current_bet = 0)
{
	////////////////////////////////////////////////
	// Generate some random action to seed the db //
	////////////////////////////////////////////////

	std::vector<eBetType> possible_bets;

	// We can also probably always call/check 
	// or go all-in if we don't have enough to call
	if (!just_raises)
	{
		// skip folds since they add no value 
		if(hand[eDim_Call] == 0)
			possible_bets.push_back(eCheck);
		else if(hand[eDim_Balance] > 0)
			possible_bets.push_back(eCall);
	}

	for(int i = eCall+1; i < eTotalActions; i++)
	{
		double amt_required = TranslateActionToAmount(hand, (eBetType) i);
		double amt_available = hand[eDim_Balance];

		if(amt_required > amt_available)
			break;

		possible_bets.push_back((eBetType) i);
	}

	if(just_raises && possible_bets.size() == 0)
		return eBet_Unknown;

	int randbet_ndx = (possible_bets.size() > 0 ? rand() % possible_bets.size() : -1);
	bettype = (randbet_ndx >= 0 ? possible_bets[randbet_ndx] : eFold);
	gLog.WriteLog(eSeverityInfo, eCatANN, "#actions: %d, ndx: %d, value: %s\n", possible_bets.size(), randbet_ndx, bets_str[bettype]);

	return bettype;
}

double CDominvs::TranslateActionToAmount(int* hand, eBetType betType)
{
	// The amounts used in the paper in terms of the pot
	// Add opening bets ???

	double amt = 0.0;
	switch(betType)
	{
		case eFold:
			break;
		case eCheck:
			break;
		case eCall:
			amt = hand[eDim_Call];
			break;
		case eHalfPot:
			amt = hand[eDim_Call] + 0.5 * hand[eDim_Pot];
			break;
		case ePot:
			amt = hand[eDim_Call] + hand[eDim_Pot];
			break;
		case eTwoPots:
			amt = hand[eDim_Call] + 2.0 * hand[eDim_Pot];
			break;
			/*
		case eAllin:
			amt = hand[eDim_Balance];
			break;
			*/
		default:
			break;
	}

	return amt;
}

eBetType CDominvs::ANNDecision(int* hand, CDatabase* p_db)
{
	int num_neighbours[eTotalActions];

	double sum_dists[eTotalActions];
	double max_dists[eTotalActions];
	double min_dists[eTotalActions];
	double diag_dists[eTotalActions];
	double ev[eTotalActions];
	bool neighbours_fail[eTotalActions];

	//initialize arrays
	for (int i = 0; i < eTotalActions; i++)
	{
		num_neighbours[i] = 0;
		neighbours_fail[i] = false;
		ev[i] = 0.0;
		sum_dists[i] = 0.0;
		max_dists[i] = -DBL_MAX;
		min_dists[i] = DBL_MAX;
		diag_dists[i] = 0.0;
	}

	// Set the Fold EV
	// Instead of setting it to 0 we will penalize it.
	ev[eFold] = hand[eDim_Balance] - hand[eDim_InitialBalance];
	if(ev[eFold] > 0)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "ERROR: EV(F) > 0(%lf), [B(%d) - IB(%d)]\n",
													ev[eFold],
													hand[eDim_Balance],
													hand[eDim_InitialBalance]);
	}

	// Get Data
	for(int i = eCheck; i < eTotalActions; i++)
	{
		// Find the right ANN Object
		CANNObject* p_search = gVars.GetAnnQueryObject(hand[eDim_brNdx], hand[eDim_timesActed], (eBetType)i);
		if(!p_search || !p_search->get_stable())
		{
			gLog.WriteLog(eSeverityError, eCatANN, "ERROR: Could not get ANN Obj. for (br:%d; ta:%d; bt:%s)\n", 
																						hand[eDim_brNdx]+1, 
																						hand[eDim_timesActed], 
																						bets_str[i]);
			neighbours_fail[i] = true;
			continue;
		}

		num_neighbours[i] = ((p_search->get_npoints() < gPrefs.knn.get_num_neighbours(p_search->get_br())) ? 
			p_search->get_npoints() : gPrefs.knn.get_num_neighbours(p_search->get_br()));

		////////////////////////////////////
		// Calculate EV for several actions
		////////////////////////////////////
		ANNdist	curr_dists[MAX_NEIGHBOURS];
		ANNidx	curr_ndxs[MAX_NEIGHBOURS];

		// Initialize vars
		for (int neighbour_ndx = 0; neighbour_ndx < num_neighbours[i]; neighbour_ndx++)
		{
			curr_dists[neighbour_ndx]	= .0;
			curr_ndxs[neighbour_ndx]	= -1;
		}
		// Get neighbours and check for failure
		
		if(!p_search->GetNeighbours(hand, curr_ndxs, curr_dists, (eBetType)i))
		{
			gLog.WriteLog(eSeverityError, eCatANN, "Error: Could not get Nearest neighbours\n");
			neighbours_fail[i] = true;
			continue;
		}

		#define diagonal_scaling
		#ifdef diagonal_scaling

			for(int neighbour_ndx = 0; neighbour_ndx < num_neighbours[i]; neighbour_ndx++)
			{
				if(max_dists[i] < curr_dists[neighbour_ndx])
					max_dists[i] = curr_dists[neighbour_ndx];
				if(min_dists[i] > curr_dists[neighbour_ndx])
					min_dists[i] = curr_dists[neighbour_ndx];
			}

			diag_dists[i] = max_dists[i] - min_dists[i];

		#endif

		// scale ev by the distance
		for(int neighbour_ndx = 0; neighbour_ndx < num_neighbours[i]; neighbour_ndx++)
		{
		#ifdef diagonal_scaling
			double factor = (diag_dists[i] > 0 ? ((max_dists[i] - curr_dists[neighbour_ndx])/diag_dists[i]) : 1.0);

			#ifdef _DEBUG
				if(factor > 1 || factor < 0)
					gLog.WriteLog(eSeverityError, eCatANN, "Factor [%lf] c:%lf M:%lf m:%lf d:%lf\n", 
															factor,
															curr_dists[neighbour_ndx],
															max_dists[i],
															min_dists[i],
															diag_dists[i]);
			#endif

			ev[i] += (p_search->p_data->get_ndx_profit(curr_ndxs[neighbour_ndx]) * factor);
		#elif defined normal_scaling 
			ev[i] += (p_search->get_ndx_profit(curr_ndxs[neighbour_ndx]) * (curr_dists[neighbour_ndx] > 1 ? 1.0/curr_dists[neighbour_ndx] : 1.0);
		#else
			ev[i] += p_search->get_ndx_profit(curr_ndxs[neighbour_ndx]);
		#endif
		}
	}

	// Get max ev
	eBetType dec = eBet_Unknown;
	double max_ev = -DBL_MAX;
	for(int i = 0; i < eTotalActions; i++)
	{
		// ignore this action
		if(neighbours_fail[i])
			continue;
		
		gLog.WriteLog(eSeverityInfo, eCatANN, "EV(%s): %.2lf [%d] \n", bets_str[i], ev[i], num_neighbours[i]);
		if(ev[i] > max_ev)
		{
			gLog.WriteLog(eSeverityDebug, eCatANN, " New Max for EV(%s): %.2lf\n", bets_str[i], ev[i]);
			max_ev = ev[i];
			dec = (eBetType)i;
		}
	}

	gLog.WriteLog(eSeverityError, eCatANN, "Dec: %d\n", dec);

	// Save the current decision
	int current_dec_ndx = eDim_Action1 + hand[eDim_timesActed];
	hand[current_dec_ndx] = dec;

	return dec;
}

eBetType CDominvs::LWRDecision(int* hand, CDatabase* p_db)
{
	int num_neighbours = 0;

	double sum_dists[eTotalActions];
	double max_dists[eTotalActions];
	double min_dists[eTotalActions];
	double diag_dists[eTotalActions];
	double ev[eTotalActions];

	// Set the Fold EV
	// Instead of setting it to 0 we will penalize it.
	ev[eFold] = hand[eDim_Balance] - hand[eDim_InitialBalance];
	if(ev[eFold] > 0)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "ERROR: EV(F) > 0(%lf), [IB(%d) - B(%d)]\n",
													ev[eFold],
													hand[eDim_Balance],
													hand[eDim_InitialBalance]);
	}

	// Get Data
	for(int i = eCheck; i < eTotalActions; i++)
	{
		// Find the right ANN Object
		CANNObject* p_search = gVars.GetAnnQueryObject(hand[eDim_brNdx], hand[eDim_timesActed], (eBetType)i);
		if(!p_search || !p_search->get_stable())
		{
			gLog.WriteLog(eSeverityError, eCatANN, "ERROR: Could not get ANN Obj. for (br:%d; ta:%d; bt:%s)\n", 
																						hand[eDim_brNdx]+1, 
																						hand[eDim_timesActed], 
																						bets_str[i]);
			ev[i] = 0;
			continue;
		}

		num_neighbours = ((p_search->get_npoints() < gPrefs.knn.get_num_neighbours(p_search->get_br())) ? 
			p_search->get_npoints() : gPrefs.knn.get_num_neighbours(p_search->get_br()));

		////////////////////////////////////
		// Calculate EV for several actions
		////////////////////////////////////
		ANNdist	curr_dists[MAX_NEIGHBOURS];
		ANNidx	curr_ndxs[MAX_NEIGHBOURS];

		// Initialize vars
		for (int neighbour_ndx = 0; neighbour_ndx < num_neighbours; neighbour_ndx++)
		{
			curr_dists[neighbour_ndx]	= 0;
			curr_ndxs[neighbour_ndx]	= -1;
		}
		// Get neighbours and check for failure
		bool neighbours_fail = false;
		if(!p_search->GetNeighbours(hand, curr_ndxs, curr_dists, (eBetType)i))
		{
			gLog.WriteLog(eSeverityError, eCatANN, "Error: Could not get Nearest neighbours\n");
			neighbours_fail = true;
			continue;
		}

		// scale ev by the distance
		ev[i] = 0;
		for(int neighbour_ndx = 0; neighbour_ndx < num_neighbours; neighbour_ndx++)
		{
			double factor = (diag_dists[i] > 0 ? (max_dists[i] - curr_dists[neighbour_ndx])/diag_dists[i] : 1.0);
			ev[i] += (p_search->p_data->get_ndx_profit(curr_ndxs[neighbour_ndx]) * factor);
		}
	}

	// Get max ev
	eBetType dec = eBet_Unknown;
	double max_ev = -DBL_MAX;
	for(int i = 0; i < eTotalActions; i++)
	{
		gLog.WriteLog(eSeverityInfo, eCatANN, "EV(%s): %.2lf \n", bets_str[i], ev[i]);
		if(ev[i] > max_ev)
		{
			gLog.WriteLog(eSeverityDebug, eCatANN, " New Max for EV(%s): %.2lf\n", bets_str[i], ev[i]);
			max_ev = ev[i];
			dec = (eBetType)i;
		}
	}

	gLog.WriteLog(eSeverityError, eCatANN, "Dec: %d\n", dec);

	// Save the current decision
	int current_dec_ndx = eDim_Action1 + hand[eDim_timesActed];
	hand[current_dec_ndx] = dec;

	return dec;
}




