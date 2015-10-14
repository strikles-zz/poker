
#include <cassert>

#include "Bot/Bot_State.h"

#include "Bot/Bot_Globalvars.h"
#include "Util/Util_MagicNumbers.h"
#include "Util/Util_Logging.h"




void CState::ExtractState(const holdem_state& curState)
{
	/////////////////////////////
	// DETECT NEW GAME / ROUND //
	/////////////////////////////

	// Determine if we started a new game by using the Dealer chair 
	// or the betting round has looped around.
	m_bNewGame	= false;
	m_bNewRound	= false;

	/*
	if(betround_by_cards < m_PrevBetround)
		m_bNewGame = true;
	*/

	if(CurState().m_dealer_chair != m_current_hand_info.m_prev_dealer_chair || m_PrevHandsPlayed != gVars.get_handsplayed())
		m_bNewGame = true;
	else if(ePreflop < betround_by_cards && betround_by_cards != m_PrevBetround)
		m_bNewRound = true;

	// Just like OpenHoldem we are always going to be processing the state from the last actor 5.50
	// (dealer+1 for new rounds or userchair+1 for multiple actions per round). 
	// m_CurActor will hold this index
	if(m_bNewGame)
	{
		gLog.WriteLog(eSeverityInfo, eCatNewRound,		"Starting New Hand\n");

		m_current_hand_info.NewHand();
		ExtractBlinds();

		if(eBBUnknown == m_current_hand_info.bb_chair)
			gLog.WriteLog(eSeverityError, eCatGeneral, "Error: Got a new hand but no BB was found\n");
		if(eSBUnknown == m_current_hand_info.sb_chair)
			gLog.WriteLog(eSeverityDebug, eCatGeneral, "Warning: Got a new hand but no SB was found\n");
		if(m_current_hand_info.bb_chair == m_current_hand_info.sb_chair)
			gLog.WriteLog(eSeverityError, eCatGeneral, "Error: BB chair = SB chair\n");

		assert(eBBUnknown != m_current_hand_info.bb_chair);

		m_bNewGame = false;
		m_CurActor = (m_current_hand_info.bb_chair + 1) % k_max_chairs;
	}
	else if(m_bNewRound) 
	{
		gLog.WriteLog(eSeverityInfo, eCatNewRound,		"Starting New Round: %s\n", rounds_str[betround_by_cards]);

		m_current_hand_info.NewBettingRound();
		ExtractMissingActions();

		m_CurActor	= (CurState().m_dealer_chair+1) % k_max_chairs;
		m_bNewRound = false;
	}

	ExtractActions();

	m_CurActor				= (gVars.userchair+1) % k_max_chairs;
	m_PrevBetround			= betround_by_cards;
	m_PrevHandsPlayed		= gVars.get_handsplayed();
	m_current_hand_info.m_prev_dealer_chair	= CurState().m_dealer_chair;
}


void CState::ExtractBlinds()
{
	//////////////
	// NEW GAME //
	//////////////
	gLog.WriteLog(eSeverityInfo, eCatPerformance,	"GAME - Start Finding Blinds\n");

	enumStreetIndices br_ndx = (enumStreetIndices) BR2NDX(betround_by_cards);
	assert(ePreflopIndex <= br_ndx && eRoundIndices > br_ndx);

	bool hero_is_sb	= IsEqual(gVars.currentbet[gVars.userchair], gVars.get_sblind());
	bool hero_is_bb	= !hero_is_sb && IsEqual(gVars.currentbet[gVars.userchair], gVars.get_bblind());

	is_heads_up	= (2 == bitcount(gVars.get_playersdealtbits()));

	// Set the initial 'in game' to be set to if they are in the game 
	for(int chair = 0; chair < k_max_chairs; chair++)
		m_current_hand_info.m_player_actions[chair].bWasInGame = gVars.InBits(chair, gVars.get_playersseatedbits());	

	if(hero_is_sb)
	{
		m_current_hand_info.sb_chair = gVars.userchair;
		m_current_hand_info.RecordPlayerAction(br_ndx, m_current_hand_info.sb_chair, eAction_SB, gVars.get_sblind());
	}
	else if(hero_is_bb)
	{
		m_current_hand_info.bb_chair = gVars.userchair;

		if(is_heads_up)
			m_current_hand_info.sb_chair = CurState().m_dealer_chair;
		else
		{
			for(int ndx = 0; ndx < k_max_chairs; ndx++)
			{
				int chair = (CurState().m_dealer_chair + (ndx+1)) % k_max_chairs;
				if(0 < gVars.currentbet[chair])
				{
					m_current_hand_info.sb_chair = chair;
					break;
				}
			}
		}

		if(eSBUnknown != m_current_hand_info.sb_chair)
			m_current_hand_info.RecordPlayerAction(br_ndx, m_current_hand_info.sb_chair, eAction_SB, gVars.get_sblind());

		m_current_hand_info.RecordPlayerAction(br_ndx, m_current_hand_info.bb_chair, eAction_BB, gVars.get_bblind());
	}

	///////////////
	// SB SEARCH //
	///////////////		
	if(eSBUnknown == m_current_hand_info.sb_chair)
	{
		for(int ndx = 0; ndx < k_max_chairs; ndx++)
		{
			int	chair = (CurState().m_dealer_chair + (ndx+1)) % k_max_chairs;
			if(IsEqual(gVars.currentbet[chair], gVars.get_sblind()))
			{
				m_current_hand_info.sb_chair = chair;
				m_current_hand_info.RecordPlayerAction(br_ndx, m_current_hand_info.sb_chair, eAction_SB, gVars.get_sblind());

				break;
			}
		}
	}

	///////////////
	// BB SEARCH //
	///////////////
	if(eBBUnknown == m_current_hand_info.bb_chair)
	{
		for(int ndx = 0; ndx < k_max_chairs; ndx++)
		{
			int	chair = (eSBUnknown != m_current_hand_info.sb_chair ? (m_current_hand_info.sb_chair + (ndx+1)) % k_max_chairs : (CurState().m_dealer_chair + (ndx+1)) % k_max_chairs);
			if(IsEqual(gVars.currentbet[chair], gVars.get_bblind()))
			{
				m_current_hand_info.bb_chair = chair;
				m_current_hand_info.RecordPlayerAction(br_ndx, m_current_hand_info.bb_chair, eAction_BB, gVars.get_bblind());

				break;
			}
		}
	}
	// could not find match so find 1st largest ?
	if(eBBUnknown == m_current_hand_info.bb_chair)
	{
		for(int ndx = 0; ndx < k_max_chairs; ndx++)
		{
			int	chair = (eSBUnknown != m_current_hand_info.sb_chair ? (m_current_hand_info.sb_chair + (ndx+1)) % k_max_chairs : (CurState().m_dealer_chair + (ndx+1)) % k_max_chairs);
			if(gVars.currentbet[chair] > gVars.get_bblind())
			{
				m_current_hand_info.bb_chair = chair;
				m_current_hand_info.RecordPlayerAction(br_ndx, m_current_hand_info.bb_chair, eAction_BB, gVars.get_bblind());

				break;
			}
		}
	}

	// Hmmm...
	// m_CurrentHandInfo.m_PlayerActions[next_chair].actions[ePreflop][0].left_to_act = get_nplayersdealt() - 1;

	gLog.WriteLog(eSeverityInfo, eCatPerformance, "GAME - End Finding Blinds\n");
} 


void CState::ExtractMissingActions()
{
	/*
		Checks for missing actions between our last action in the previous round and this round
	*/

	gLog.WriteLog(eSeverityInfo, eCatPerformance,	"ROUND - Start Picking Missing Actions\n");

	enumStreetIndices previous_br_ndx = (enumStreetIndices) (BR2NDX(betround_by_cards) - 1);
	assert(previous_br_ndx >= ePreflopIndex || previous_br_ndx < eRoundIndices);

	//////////////////////////
	// FIND MISSING ACTIONS //
	//////////////////////////
	int end_chair = ((ePreflopIndex == previous_br_ndx && gVars.ntimesacted == 1 ? m_current_hand_info.bb_chair : CurState().m_dealer_chair) + 1) % k_max_chairs;

	for(int ndx = 0; ndx < k_max_chairs; ndx++)
	{
		int chair = (gVars.userchair + (ndx+1)) % k_max_chairs;

		// Actions added by userchair are never missed
		if(end_chair == chair || gVars.userchair == chair)
			break;

		bool	isInGame						= .0;
		double	previousCall				= .0;
		double	playerPreviousBet	= .0;
		if(previous_br_ndx != eRound_Unknown)
		{
			previousCall				= m_current_hand_info.m_amount_to_call[(int)previous_br_ndx];
			playerPreviousBet	= m_current_hand_info.m_player_actions[chair].prevBet[(int)previous_br_ndx];
		}
		// Wasn't in game and isn't in game (All Good) -> Ignore and continuebWasInGame
		if(!isInGame && !m_current_hand_info.m_player_actions[chair].bWasInGame)
			continue;

		// Folded
		if(!isInGame && m_current_hand_info.m_player_actions[chair].bWasInGame)
		{
			m_current_hand_info.m_player_actions[chair].bWasInGame = false;
			m_current_hand_info.RecordPlayerAction(previous_br_ndx, chair, eAction_F, 0, true);
		}
		// Called
		else if(isInGame && playerPreviousBet < previousCall)
		{
			m_current_hand_info.RecordPlayerAction(previous_br_ndx, chair, eAction_C, previousCall, true);
		}
		// Checked
		else if(isInGame && (IsEqual(previousCall, playerPreviousBet)))
		{
			m_current_hand_info.RecordPlayerAction(previous_br_ndx, chair, eAction_X, 0, true);
		}

		int tot_actions = m_current_hand_info.m_player_actions[chair].total_actions[previous_br_ndx];
		m_current_hand_info.m_player_actions[chair].actions[previous_br_ndx][tot_actions].to_act = LeftToAct(chair);
	}

	gLog.WriteLog(eSeverityInfo, eCatPerformance, "ROUND - End Picking Missing Actions\n");
}


void CState::ExtractActions()
{
	//////////////////////////////
	// SEARCH REMAINING ACTIONS //
	//////////////////////////////

	enumStreetIndices br_ndx = (enumStreetIndices) BR2NDX(betround_by_cards);
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);

	for(int ndx = 0; ndx < k_max_chairs; ndx++)
	{
		int chair = (m_CurActor + ndx) % k_max_chairs;

		// Since we should only get here if it's our turn, stop at userchair. 
		// Players behind us had no chance to act yet ...
		if(gVars.userchair == chair)
			break;

		bool	isInGame			= InGame(CurState().m_player[chair]);
		double	currentCall			= m_current_hand_info.m_amount_to_call[br_ndx];
		double	playerPreviousBet	= m_current_hand_info.m_player_actions[chair].prevBet[br_ndx];
		int		num_actions			= m_current_hand_info.m_player_actions[chair].total_actions[br_ndx];		
		
		// Wasn't in game and isn't in game (All Good) -> Ignore and continuebWasInGame
		if(!isInGame && !m_current_hand_info.m_player_actions[chair].bWasInGame)
			continue;

		// Player popped-out from nowhere : No Magic -> Scrape Error ?
		else if(isInGame && !m_current_hand_info.m_player_actions[chair].bWasInGame)
		{
			gLog.WriteLog(eSeverityError, eCatHandHistory, "Error seat %d wasn't in the hand but is now in the hand\n", chair);
			continue;
		}
		// Fold
		else if(!isInGame && m_current_hand_info.m_player_actions[chair].bWasInGame)
		{
			m_current_hand_info.m_player_actions[chair].bWasInGame = false;
			m_current_hand_info.RecordPlayerAction(br_ndx, chair, eAction_F, 0);
			// Hmmm...
			m_current_hand_info.m_player_actions[chair].actions[br_ndx][num_actions].to_act = LeftToAct(chair);
		}
		// Check
		else if(IsEqual(gVars.currentbet[chair], playerPreviousBet))
		{
			m_current_hand_info.RecordPlayerAction(br_ndx, chair, eAction_X, gVars.currentbet[chair]);
			// Hmmm...
			m_current_hand_info.m_player_actions[chair].actions[br_ndx][num_actions].to_act = LeftToAct(chair);
		}
		// Call
		else if(IsEqual(m_current_hand_info.m_amount_to_call[BR2NDX(betround_by_cards)], gVars.currentbet[chair]))
		{
			m_current_hand_info.RecordPlayerAction(br_ndx, chair, eAction_C, gVars.currentbet[chair]);
			m_current_hand_info.m_pot_round += (gVars.currentbet[chair] - playerPreviousBet);
		} 
		// Player did no have enough $ to Call so had to go all-in
		else if(m_current_hand_info.m_amount_to_call[BR2NDX(betround_by_cards)] > gVars.currentbet[chair] && 0 < gVars.currentbet[chair] && IsEqual(0, gVars.balance[chair]))
		{
			m_current_hand_info.RecordPlayerAction(br_ndx, chair, eAction_C, gVars.currentbet[chair]);
			m_current_hand_info.m_pot_round += (gVars.currentbet[chair] - playerPreviousBet);
		} 
		// Bet-Raise
		else if(gVars.currentbet[chair] > currentCall)
		{
			m_current_hand_info.RecordPlayerAction(br_ndx, chair, eAction_R, gVars.currentbet[chair]);
			m_current_hand_info.m_pot_round += (gVars.currentbet[chair] - playerPreviousBet);
			// Hmmm...
			m_current_hand_info.m_player_actions[chair].actions[br_ndx][num_actions].to_act = LeftToAct(chair);
		}

		int tot_actions = m_current_hand_info.m_player_actions[chair].total_actions[br_ndx];
		m_current_hand_info.m_player_actions[chair].actions[br_ndx][tot_actions].to_act = LeftToAct(chair);
	}
}

