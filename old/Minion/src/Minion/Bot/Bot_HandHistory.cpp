
#include <cassert>

#include "Bot/Bot_HandHistory.h"

#include "Bot/Bot_GlobalVars.h"



CHandHistory::CHandHistory()
: m_prev_dealer_chair(-1), sb_chair(eSBUnknown), bb_chair(eBBUnknown)
{
	NewHand();
}

void CHandHistory::NewHand()
{
	was_bots_action_recorded = false;
	bb_chair = eBBUnknown;
	sb_chair = eSBUnknown;

	memset(&m_amount_to_call[0], 0, eRoundIndices*sizeof(m_amount_to_call[0]));

	for(int chair=0; chair < k_max_chairs; chair++)
	{
		m_player_actions[chair].bWasInGame		= false;
		m_player_actions[chair].preflopPosition	= ePos_Unknown;
		for(int br_ndx = 0; br_ndx < eRoundIndices; br_ndx++)
			m_player_actions[chair].ResetPlayerState(br_ndx);
	}

	NewBettingRound();
}

void CHandHistory::NewBettingRound()
{
	int br_ndx = BR2NDX(gVars.get_betround());

	m_pot_round				= 0.0;
	m_amount_to_call[br_ndx]	= 0.0;
	was_bots_action_recorded		= false;
}

void CHandHistory::RecordPlayerAction(enumStreetIndices br_ndx, int chair, ePlayerAction action, double amount, bool isMissingAction)
{
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);

	// Logging eye-candy
	eCategory log_category = eCatHandHistory;
	if(action == eAction_SB || action == eAction_BB)
		log_category = eCatHandHistoryBlinds;

	else if(br_ndx != BR2NDX(gVars.get_betround()))
		log_category = eCatHandHistoryMissing;

	double player_call		= 0.0;
	double player_balance	= 0.0;

	if(chair == gVars.userchair)
	{
		if(eAction_C <= action && gVars.balance[chair] < amount)
			player_call = gVars.balance[chair];
		else
			player_call = gVars.call;
	}
	else
	{
		/*
		player_balance = (isMissingAction ? m_PlayerActions[chair].balance[br_ndx] : gVars.balance[chair]);
		if(eAction_C <= action && player_balance < amount)
			player_call = player_balance;
		else
		*/

		player_call = ToMoney(m_amount_to_call[br_ndx] - m_player_actions[chair].prevBet[br_ndx]);
	}

	double raise_amt = (action == eAction_R ? ToMoney(amount - player_call) : 0.0);

	// log stuff
	if(eAction_SB == action || eAction_BB == action)
	{
		gLog.WriteLog(eSeverityInfo, log_category, ">> %d - %-16s %-7s %10.2f\n",			
			chair, 
			gVars.m_state.CurState().m_player[chair].m_name,	
			actions_str[action], 
			amount);
	}
	else if(eAction_R == action)
	{
		gLog.WriteLog(eSeverityInfo, log_category, ">> %d - %-16s %-7s %10.2f [%.2f(R) + %.2f(C)]\n",			
			chair, 
			gVars.m_state.CurState().m_player[chair].m_name,	
			actions_str[action], 
			amount,
			raise_amt,
			player_call);
	}
	else
	{
		gLog.WriteLog(eSeverityInfo, log_category, ">> %d - %-16s %-7s %10.2f [%.2f(C)]\n",			
			chair, 
			gVars.m_state.CurState().m_player[chair].m_name,	
			actions_str[action], 
			amount,
			player_call);
	}

	// To do - Correct stuff
	if(eAction_F == action)
	{
		assert(IsEqual(0, raise_amt));
	}
	else if(eAction_X == action && !IsEqual(0, gVars.balance[chair]))
	{
		assert(IsEqual(0, player_call));
		assert(IsEqual(0, raise_amt));
	}
	else if(eAction_C == action)
	{
		assert(0 < player_call);
		assert(IsEqual(0, raise_amt));
	}
	else if(eAction_R == action)
	{
		assert (0 < raise_amt);
	}

	m_player_actions[chair].SaveAction(br_ndx, action, player_call, raise_amt, m_pot_round);

	// hmmm...
	if(amount > m_amount_to_call[br_ndx])
		m_amount_to_call[br_ndx] = amount;

	m_player_actions[chair].prevBet[br_ndx] = ToMoney(amount);
	m_player_actions[chair].balance[br_ndx] = ToMoney(isMissingAction ? gVars.balance[chair] - player_call : gVars.balance[chair]);

	was_bots_action_recorded = (chair == gVars.userchair);
}
