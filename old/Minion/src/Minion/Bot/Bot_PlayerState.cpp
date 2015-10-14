
#include <cassert>

#include "Bot/Bot_PlayerState.h"

#include "Util/Util_MagicNumbers.h"



CPlayerState::CPlayerState() : 
	bWasInGame(false), 
	preflopPosition(ePos_Unknown)
{
	for(int br_ndx = 0; br_ndx < eRoundIndices; br_ndx++)
		ResetPlayerState(br_ndx);
}

void CPlayerState::ResetPlayerState(int br_ndx)
{
	prevBet[br_ndx]				= 0.0;
	balance[br_ndx]				= 0.0;
	total_actions[br_ndx]		= 0;
	actions_this_round[br_ndx]	= std::string(" .");

	for(int action_ndx = 0; action_ndx < k_MaxActionsPerRound; action_ndx++)
	{
		actions[br_ndx][action_ndx].act				= eAction_Unknown;
		actions[br_ndx][action_ndx].betType			= eBet_Unknown;

		actions[br_ndx][action_ndx].call_amount		= 0.0;
		actions[br_ndx][action_ndx].raise_amount	= 0.0;
		actions[br_ndx][action_ndx].to_act			= -1;
	}
}

void CPlayerState::SaveAction(enumStreetIndices br_ndx, ePlayerAction action, double to_call, double raise_amt, double pot)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);

	for(int action_ndx = 0; action_ndx < k_MaxActionsPerRound; action_ndx++)
	{
		// find the 1st available index -> eAction_Unknown
		if(actions[br_ndx][action_ndx].act == eAction_Unknown) 
		{
			actions[br_ndx][action_ndx].act				= action;
			actions[br_ndx][action_ndx].call_amount		= to_call;
			actions[br_ndx][action_ndx].raise_amount	= raise_amt;

			total_actions[br_ndx] = action_ndx + 1;

			if(action_ndx == 0)
				actions_this_round[br_ndx] = std::string(min_actions_str[action]);
			else
				actions_this_round[br_ndx] += std::string(min_actions_str[action]);

			// exit
			break;
		}
	}
}