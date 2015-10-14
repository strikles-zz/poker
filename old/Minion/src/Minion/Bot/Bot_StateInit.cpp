
#include "Bot/Bot_State.h"

#include "Bot/Bot_Globalvars.h"

#include "Util/Util.h"
#include "Util/Util_MagicNumbers.h"
#include "Util/Util_Logging.h"


CState::CState()
{
	m_ndx = 0;
	m_bNewGame		= true;
	m_bNewRound		= true;
	state_counter	= 0;
	m_PrevBetround	= 0;

	m_CurActor		= -1;
	betround_by_cards = eRound_Unknown;
}


bool CState::WillProcessState(const holdem_state& curState)
{
	/*
		Only called in dll main
		CurState() will return the last saved state (i.e. the previous state)

		return true if different + usable + stable
	*/

	if(gVars.userchair == -1)
	{
		gLog.WriteLog(eSeverityDebug, eCatStates, "[State Not Usable] Unknown userchair\n");
		return false;
	}
	if(!gVars.get_ismyturn())
	{
		return false;
	}

	// CurState() will still return the last saved state
	if(m_ndx > 0 && (!IsStateDifferent(curState, CurState(), false)))
		return false;

	if(!IsStateUsable(curState)) 
	{
		gLog.WriteLog(eSeverityDebug, eCatStates, "Unusable state provided by OH\n");
		return false;
	}

	if(state_counter < 2)
	{
		gLog.WriteLog(eSeverityInfo, eCatStates, "Stable State %d\n", state_counter);
		state_counter++;
		return false;
	}

	state_counter = 0;
	DumpHoldemState(curState, m_ndx);

	return true;
}


bool CState::IsStateDifferent(const holdem_state &a, const holdem_state &b, bool bConsiderName)
{
	/*
	* return true is the state is different
	*/

	//Title
	if(strcmp(a.m_title, b.m_title) != 0)
		return true;

	//Pot
	for(int chair = 0 ; chair < k_max_chairs; chair++) 
	{
		if(!IsEqual(a.m_pot[chair], b.m_pot[chair]))
			return true;
	}

	//Board cards
	for(int bc = 0; bc < 5; bc++) 
	{
		if(a.m_cards[bc] != b.m_cards[bc])
			return true;
	}
	if(a.m_is_playing != b.m_is_playing ||
		 a.m_is_posting != b.m_is_posting ||
		 a.m_dealer_chair != b.m_dealer_chair)
	{
			return true;
	}

	//Players in table
	for(int chair = 0; chair < k_max_chairs; chair++) 
	{
		if(bConsiderName && strcmp(a.m_player[chair].m_name, b.m_player[chair].m_name) != 0)
			return true;

		if(!IsEqual(a.m_player[chair].m_balance ,b.m_player[chair].m_balance) ||
			 !IsEqual(a.m_player[chair].m_currentbet, b.m_player[chair].m_currentbet) ||
			 a.m_player[chair].m_cards[0] != b.m_player[chair].m_cards[0] ||
			 a.m_player[chair].m_cards[1] != b.m_player[chair].m_cards[1] ||
			 a.m_player[chair].m_name_known != b.m_player[chair].m_name_known ||
			 a.m_player[chair].m_balance_known != b.m_player[chair].m_balance_known)
		{
				return true;
		}
	}

	return false;
}


bool CState::IsStateUsable(const holdem_state& curState)
{
	/*
	*
	*/


	if(!CardsDealt(curState) || !InGame(curState.m_player[gVars.userchair]))
	{
		m_bNewGame = true;
		m_bNewRound = true;
		m_PrevBetround = 0;
		gLog.WriteLog(eSeverityDebug, eCatStates, "[State Not Usable] No Cards dealt\n");
		return false;
	}

	//Check if there are players with the balance know and how many players are seated
	int nPlayersWithCards = 0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		bool seated = gVars.InBits(chair, gVars.get_playersseatedbits());		
		if(seated)
		{
			bool has_cards = curState.m_player[chair].m_cards[0] != 0 && curState.m_player[chair].m_cards[1] != 0;
			if(has_cards)
				nPlayersWithCards++;

			if(has_cards && !curState.m_player[chair].m_balance_known)
			{
				gLog.WriteLog(eSeverityDebug, eCatStates, "[State Not Usable] Balance unknown for the player %d\n", chair);
				return false;
			}
			if((255 == curState.m_player[chair].m_cards[0] && 0 == curState.m_player[chair].m_cards[1]) ||
				(255 == curState.m_player[chair].m_cards[1] && 0 == curState.m_player[chair].m_cards[0]))
			{
				gLog.WriteLog(eSeverityDebug, eCatStates, "Opponent is holding a single card\n");
				return false;
			}
			// Check to see if we are at showdown which we will ignore otherwise initialize which players are in the hand
			// m_bNewGame will still be true the next time into ExtractState so we will do the initialization again.
			if(chair != gVars.userchair && 
				((0 < curState.m_player[chair].m_cards[0] && 255 > curState.m_player[chair].m_cards[0]) || 
				(0 < curState.m_player[chair].m_cards[1] && 255 > curState.m_player[chair].m_cards[1])))
			{
				gLog.WriteLog(eSeverityError, eCatHandHistory, "Must be at showdown as we see opponent cards\n");
				return false;
			}
		}
	}

	//Check if there are enough players with cards
	if(nPlayersWithCards < 2)
	{
		gLog.WriteLog(eSeverityDebug, eCatStates, "[State Not Usable] There aren't enough players with cards\n");
		return false;
	}

	if(gVars.get_betround() < ePreflop || gVars.get_betround() > eRiver)
	{
		gLog.WriteLog(eSeverityDebug, eCatStates, "[State Not Usable] Invalid Betround (%d)\n", gVars.get_betround());
		return false;
	}

	//Check how many board cards are
	int nBoardCards = 0;
	for(int bc = 0; bc < 5; bc++)
	{
		if(curState.m_cards[bc] > 0)
			nBoardCards++;
	}

	//Check if there are incorrect board cards
	if((gVars.get_betround() == ePreflop && nBoardCards != 0) || (gVars.get_betround() > ePreflop && nBoardCards != gVars.get_betround() + 1))
	{
		gLog.WriteLog(eSeverityDebug, eCatStates, "[State Not Usable] %s - Wrong Number of Boards Cards (%d)\n", rounds_str[gVars.get_betround()], nBoardCards);
		return false;
	}

	//Check if there aren't any occluded board cards
	for(int bc = 0; bc < nBoardCards; bc++)
	{
		if(curState.m_cards[bc] <= 0 || curState.m_cards[bc] >= 255)
		{
			gLog.WriteLog(eSeverityDebug, eCatStates, "[State Not Usable] Occluded board card number: %d\n", bc);
			return false;
		}
	}

	betround_by_cards = eRound_Unknown;
	switch(nBoardCards) 
	{
	case 0: 
		betround_by_cards = ePreflop; 
		break;
	case 3: 
		betround_by_cards = eFlop; 
		break;
	case 4: 
		betround_by_cards = eTurn; 
		break;
	case 5: 
		betround_by_cards = eRiver; 
		break;
	default: // This error condition should never happen as this case is checked for in IsStateUsable
		gLog.WriteLog(eSeverityError, eCatStates, "stableframe set to false (%d board cards found)\n", nBoardCards);
		return false;
	}

	if(betround_by_cards > ePreflop && betround_by_cards - m_PrevBetround > 1)
	{
		gLog.WriteLog(eSeverityDebug, eCatStates, "stableframe set to false (skipped a whole street PrevBR=%d BetRound=%d)\n", 
			m_PrevBetround, 
			betround_by_cards);

		m_PrevBetround = betround_by_cards;
		return false;
	}


	return true;
}


void CState::DumpHoldemState(const holdem_state &curState, int ndx)
{
	/*
	*
	*/

	gLog.WriteLogNoHeading(eSeverityInfo, eCatStates,	">> Update: %d\n", ndx+1);
	gLog.WriteLogNoHeading(eSeverityInfo, eCatStates,	">> Title: %s\n", curState.m_title);

	gLog.WriteLogNoHeading(eSeverityInfo, eCatStates,	">> Pots: %.2f %.2f %.2f\n", 
														curState.m_pot[0], 
														curState.m_pot[1], 
														curState.m_pot[2]);

	gLog.WriteLogNoHeading(eSeverityInfo, eCatStates,	">> Board: %c%c %c%c %c%c %c%c %c%c\n", 
														GetRankChar(curState.m_cards[0]), GetSuitChar(curState.m_cards[0]),
														GetRankChar(curState.m_cards[1]), GetSuitChar(curState.m_cards[1]),
														GetRankChar(curState.m_cards[2]), GetSuitChar(curState.m_cards[2]),
														GetRankChar(curState.m_cards[3]), GetSuitChar(curState.m_cards[3]),
														GetRankChar(curState.m_cards[4]), GetSuitChar(curState.m_cards[4]));

	gLog.WriteLogNoHeading(eSeverityInfo, eCatStates,	">> Dealer: %d\t IsPlaying: %d\t IsPosting: %d\n", 
														curState.m_dealer_chair, 
														curState.m_is_playing, 
														curState.m_is_posting);

	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(gVars.InBits(chair, gVars.get_playersplayingbits()))
		{
			gLog.WriteLogNoHeading(eSeverityInfo, eCatStates, ">> %d ", chair);

			if(curState.m_player[chair].m_name_known)
				gLog.WriteLogNoHeading(eSeverityInfo, eCatStates, "- %-16s  ", curState.m_player[chair].m_name);
			else
				gLog.WriteLogNoHeading(eSeverityInfo, eCatStates, "                        ");


			if(curState.m_player[chair].m_balance_known)
				gLog.WriteLogNoHeading(eSeverityInfo, eCatStates, "Balance: %8.2f  ", curState.m_player[chair].m_balance);
			else
				gLog.WriteLogNoHeading(eSeverityInfo, eCatStates, "                 ");


			gLog.WriteLogNoHeading(eSeverityInfo, eCatStates, "Bet: %8.2f  ", curState.m_player[chair].m_currentbet);

			gLog.WriteLogNoHeading(eSeverityInfo, eCatStates,	"Cards: %c%c %c%c\n", 
																GetRankChar(curState.m_player[chair].m_cards[0]), 
																GetSuitChar(curState.m_player[chair].m_cards[0]),
																GetRankChar(curState.m_player[chair].m_cards[1]), 
																GetSuitChar(curState.m_player[chair].m_cards[1]));
		}
	}
}