
#include <cassert>

#include "Bot/Bot_State.h"
#include "Bot/Bot_Globalvars.h"
#include "Util/Util_MagicNumbers.h"
#include "Util/Util_Logging.h"


bool CState::InGame(const holdem_player& player)
{
	return (player.m_cards[0] && player.m_cards[1]);
}

bool CState::CardsDealt(const holdem_state& curState)
{

	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(InGame(curState.m_player[chair]))
			return true;
	}

	return false;
}

int CState::PlayersInGame()
{
	/*
	* 
	*/

	int temp = 0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(m_current_hand_info.m_player_actions[chair].bWasInGame) 
			temp++;
	}

	return temp;
}

int CState::LeftToAct(int actor)
{
	enumStreetIndices br_ndx = (enumStreetIndices) BR2NDX(gVars.get_betround());
	assert (br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);

	int left_to_act = 0;
	int end_chair = gVars.get_dealerchair();

	if(br_ndx == ePreflopIndex && gVars.ntimesacted == 0)
		end_chair = gVars.bb_chair();

	for(int ndx = 0; ndx < k_max_chairs; ndx++)
	{
		int chair = (actor + (ndx+1)) % k_max_chairs;

		if(chair == end_chair)
			break;

		// Fold prior to our action mateters
		// Fix-me !!!
		if(gVars.InBits(chair, gVars.get_playersplayingbits()))
			left_to_act++;
	}

	return left_to_act;
}




int CState::FindPreviousActor(const int actor)
{
	/*
	*
	*/

	if(actor < 0 || actor >= k_max_chairs)
		return -1;

	for(int chair=0; chair < k_max_chairs; chair++)
	{
		int prev_actor = (actor-(chair+1) + k_max_chairs) % k_max_chairs;
		if(m_current_hand_info.m_player_actions[prev_actor].bWasInGame)
			return prev_actor;
	}

	return -1;
}

int CState::FindFirstActor(const int br)
{
	/*
	*
	*/

	if(br < eFlop || br > eRiver)
	{
		gLog.WriteLog(eSeveritySevere, eCatGeneral, "ERROR in Find_First_Actor. Wrong argument <betting_round>\n");
		return -1;
	}

	int temp = 0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		int first_actor = (CurState().m_dealer_chair+(chair+1)) % k_max_chairs;
		if(InGame(CurState().m_player[first_actor])) 
		{
			if((br != 1) || ((temp == 2) && (PlayersInGame() > 2)) || ((temp == 1) && (PlayersInGame() == 2)))
				return first_actor;

			temp++;
		}
	}

	gLog.WriteLog(eSeverityError, eCatGeneral, "ERROR in StateProcessor::Find_First_Actor. Can't find the first Actor\n");

	return -1;
}

int CState::FindNextActor(const int actor)
{
	/*
	*
	*/

	if(actor < 0 || actor >= k_max_chairs)
	{
		if(gVars.get_ismyturn())
			return gVars.userchair;

		return -1;
	}

	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		int next_actor = (actor + (chair+1)) % k_max_chairs;
		if(m_current_hand_info.m_player_actions[next_actor].bWasInGame)
			return next_actor;
	}

	return -1;
}



/*
void CState::SetCurrentActor(const bool is_heads_up, const bool hero_in_blinds)
{
	///////////////////////
	// SET CURRENT ACTOR //
	///////////////////////

	enumStreetIndices br_ndx = (enumStreetIndices) BR_TO_NDX(betround_by_cards);
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);

	if(br_ndx == ePreflopIndex)
	{
		gLog.WriteLog(eSeverityDebug, eCatStates, "BB (%d), Heads_Up (%s), In_Blinds (%s)\n", 
													m_CurrentHandInfo.m_iBBChair, 
													bool_string(is_heads_up), 
													bool_string(hero_in_blinds));
	}

	if(gVars.userchair >= 0 && gVars.userchair < k_MaxChairs)
	{
		// Pick up missed actions if HU and BB
		int our_times_acted		= m_CurrentHandInfo.m_PlayerActions[gVars.userchair].total_actions[br_ndx];
		int dealer_times_acted	= m_CurrentHandInfo.m_PlayerActions[CurState().m_dealer_chair].total_actions[br_ndx];

		if(our_times_acted > dealer_times_acted && m_CurrentHandInfo.m_iBBChair == gVars.userchair && br_ndx > ePreflopIndex && is_heads_up)		
			m_CurActor = gVars.userchair;

		if(hero_in_blinds)
			m_CurActor = (m_CurrentHandInfo.m_iBBChair + 1) % k_MaxChairs;

		if(br_ndx == ePreflopIndex && m_CurrentHandInfo.m_PlayerActions[gVars.userchair].total_actions[br_ndx] == 0)
			m_CurActor = (m_CurrentHandInfo.m_iBBChair + 1) % k_MaxChairs;
	}

	if(m_CurActor < 0)
	{
		gLog.WriteLog(eSeverityError, eCatStates,	"Current Actor: %d (Trying to find First Actor)\n", m_CurActor);
		m_CurActor = (CurState().m_dealer_chair+1) % k_MaxChairs;
	}

	gLog.WriteLog(eSeverityDebug, eCatStates, "Current Actor: %d\n", m_CurActor);
}
*/