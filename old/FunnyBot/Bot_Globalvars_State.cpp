#include "stdafx.h"

#include "Bot_Globalvars.h"
#include "Util_MagicNumbers.h"

#include "Util_Logging.h"



void CGlobalVars::DumpHoldemState(const holdem_state &curState, int index)
{
	/*
	*
	*/

	gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, ">> Update: %d\n", index+1);
	gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, ">> Title: %s\n", curState.m_title);
	gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, ">> Pots: %.2f %.2f %.2f\n", curState.m_pot[0], curState.m_pot[1], curState.m_pot[2]);

	gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, ">> Board: %c%c %c%c %c%c %c%c %c%c\n", 
		GetRankChar(curState.m_cards[0]), GetSuitChar(curState.m_cards[0]),
		GetRankChar(curState.m_cards[1]), GetSuitChar(curState.m_cards[1]),
		GetRankChar(curState.m_cards[2]), GetSuitChar(curState.m_cards[2]),
		GetRankChar(curState.m_cards[3]), GetSuitChar(curState.m_cards[3]),
		GetRankChar(curState.m_cards[4]), GetSuitChar(curState.m_cards[4]));

	gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, ">> Dealer: %d\t IsPlaying: %d\t IsPosting: %d\n", curState.m_dealer_chair, curState.m_is_playing, curState.m_is_posting);

	for (int i = 0; i < k_MaxChairs; i++)
	{
		if (is_seated(i, playersseatedbits))
		{
			gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, ">> %d ", i);

			if (curState.m_player[i].m_name_known)
				gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, "Name: %-16s  ", curState.m_player[i].m_name);
			else
				gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, "                        ");

			if (curState.m_player[i].m_balance_known)
				gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, "Balance: %8.2f  ", curState.m_player[i].m_balance);
			else
				gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, "                 ");

			gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, "Bet: %8.2f  ", curState.m_player[i].m_currentbet);
			gLog->WriteLogNoHeading(eSeverityInfo, eCatStates, "Cards: %c%c %c%c\n", 
				GetRankChar(curState.m_player[i].m_cards[0]), GetSuitChar(curState.m_player[i].m_cards[0]),
				GetRankChar(curState.m_player[i].m_cards[1]), GetSuitChar(curState.m_player[i].m_cards[1]));
		}
	}
}


bool CGlobalVars::IsStateDifferent(const holdem_state &a, const holdem_state &b, bool bConsiderName)
{
	/*
	* return true is the state is different
	*/

	//Title
	if (strcmp(a.m_title, b.m_title))
		return true;


	//Pot
	for (int i = 0 ; i < k_MaxChairs; i++) 
	{
		if (a.m_pot[i] != b.m_pot[i])
			return true;
	}


	//Board cards
	for (int i = 0; i < 5; i++) 
	{
		if (a.m_cards[i] != b.m_cards[i])
			return true;
	}
	if (a.m_is_playing != b.m_is_playing ||
		 a.m_is_posting != b.m_is_posting ||
		 a.m_dealer_chair != b.m_dealer_chair)
	{
			return true;
	}


	//Players in table
	for (int i = 0; i < k_MaxChairs; i++) 
	{
		if (bConsiderName && strcmp(a.m_player[i].m_name, b.m_player[i].m_name))
			return true;

		if (a.m_player[i].m_balance != b.m_player[i].m_balance ||
			 a.m_player[i].m_currentbet != b.m_player[i].m_currentbet ||
			 a.m_player[i].m_cards[0] != b.m_player[i].m_cards[0] ||
			 a.m_player[i].m_cards[1] != b.m_player[i].m_cards[1] ||
			 a.m_player[i].m_name_known != b.m_player[i].m_name_known ||
			 a.m_player[i].m_balance_known != b.m_player[i].m_balance_known)
		{
				return true;
		}
	}

	return false;
}

bool CGlobalVars::IsStateUsable(const holdem_state& curState)
{
	/*
	*
	*/
	if (userchair == -1)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "[State Not Usable] Unknown userchair\n");
		return false;
	}
	if (!CardsDealt(curState))
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "[State Not Usable] No Cards dealt\n");
		return false;
	}

	//Check if there are players with the balance know and how many players are seated
	int nPlayersWithCards = 0;

	for (int i = 0; i < k_MaxChairs; i++)
	{
		bool seated = is_seated(i, playersseatedbits);

		if (seated)
		{
			if (curState.m_player[i].m_cards[0] != 0 && curState.m_player[i].m_cards[1] != 0)
				nPlayersWithCards++;

			if (!curState.m_player[i].m_balance_known)
			{
				gLog->WriteLog(eSeverityInfo, eCatDebug, "[State Not Usable] Balance unknown for the player %d\n",i);
				return false;
			}
		}
	}

	//Check if there are enough players with cards
	if(nPlayersWithCards < 2)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "[State Not Usable] There aren't enough players with cards\n");
		return false;
	}

	//Check how many board cards are
	int nBoardCards = 0;

	for(int i = 0; i < 5; i++)
	{
		if(curState.m_cards[i] < 0)
		{
			gLog->WriteLog(eSeverityInfo, eCatDebug, "[State Not Usable] Wrong Card Value\n");
			return false;
		}
		if(curState.m_cards[i] > 0)
			nBoardCards++;
	}

	//Check if there are incorrect board cards
	if(nBoardCards == 1 || nBoardCards == 2)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "[State Not Usable] Wrong Number of Boards Cards\n");
		return false;
	}

	//Check if there aren't any occluded board cards
	for(int i = 0; i < nBoardCards; i++)
	{
		if (curState.m_cards[i] <= 0 || curState.m_cards[i] >= 255)
		{
			gLog->WriteLog(eSeverityInfo, eCatDebug, "[State Not Usable] Occluded board card number: %d\n",i);
			return false;
		}
	}

	return true;
}

bool CGlobalVars::WillProcessState(const holdem_state& curState)
{
	/*
		Only called in dll main
		CurState() will return the last saved state (i.e. the previous state)

		return true if different and usable state
	*/

	// CurState() will still return the last saved state
	if (m_ndx > 0 && (!IsStateDifferent(curState, CurState(), false)))
		return false;

	DumpHoldemState(curState, m_ndx);

	if (!IsStateUsable(curState)) 
	{
		gLog->WriteLog(eSeverityInfo, eCatStates, "Unusable state provided by OH\n");
		return false;
	}

	return true;
}

void CGlobalVars::ExtractActionsNewGame(enumStreets BetroundByCards, bool is_heads_up, bool& hero_in_blinds)
{
	//////////////
	// NEW GAME //
	//////////////

	bool	hero_is_sb = false, hero_is_bb = false;

	memset(&TimesActed[0], 0, eRounds*sizeof(int));
	memset(&InGameBits[0][0], 0, eRounds*k_MaxActionsPerRound*sizeof(uint32));
	memset(&FoldedBits[0][0], 0, eRounds*k_MaxActionsPerRound*sizeof(uint32));

	InGameBits[0][0] = get_playersdealtbits();

	gLog->WriteLog(eSeverityInfo, eCatHandHistory, "Starting New Hand\n");

	m_CurrentHandInfo.NewHand();
	m_CurrentHandInfo.m_PrevDealerChair = get_dealerchair();

	// Set the initial 'in game' to be set to if they are in the game 
	for(int i = 0; i < k_MaxChairs; i++)
	{
		if (i >= 0 && i < k_MaxChairs)
			m_CurrentHandInfo.m_PlayerActions[i].bWasInGame = is_seated(i, playersseatedbits);
		else
		{
			//error
		}
	}

	double HeroBet = CurState().m_player[userchair].m_currentbet;

	/////////////////////////////
	// CHECK IF HERO IN BLINDS //
	/////////////////////////////

	if (is_heads_up)
	{
		// Whichever way will search for BB

		hero_is_sb = (userchair == get_dealerchair());
		hero_in_blinds = true;
		m_CurrentHandInfo.m_iSBChair = get_dealerchair();
		m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, m_CurrentHandInfo.m_iSBChair, actionPostedSB, get_sblind(), CurState().m_player[m_CurrentHandInfo.m_iSBChair].m_balance);

		// Hmmm...
		// m_CurrentHandInfo.m_PlayerActions[get_dealerchair()].actions[ePreflop][0].left_to_act = 0;
	}

	else
	{
		// Hero is SB
		if (IsEqual(HeroBet, get_sblind()))
		{
			hero_is_sb = true;
			hero_in_blinds = true;
			m_CurrentHandInfo.m_iSBChair = userchair;
			m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, userchair, actionPostedSB, get_sblind(), CurState().m_player[m_CurrentHandInfo.m_iSBChair].m_balance);

			// Hmmm...
			// m_CurrentHandInfo.m_PlayerActions[userchair].actions[ePreflop][0].left_to_act = get_nplayersdealt() - 1;
		}
		// Hero is BB
		else if (IsEqual(HeroBet, get_bblind()))
		{
			hero_is_bb = true;
			hero_in_blinds = true;
			m_CurrentHandInfo.m_iBBChair = userchair;
			// We know BB chair but we delay Recording of Action for last.

			// Hmmm...
			// m_CurrentHandInfo.m_PlayerActions[userchair].actions[ePreflop][0].left_to_act = get_nplayersdealt() - 2;
		}
	}

	///////////////////
	// SEARCH BLINDS //
	///////////////////
		
	// Search for blinds
	for(int i = 0; i < k_MaxChairs; i++)
	{
		// If we know both blinds, quit loop
		if (m_CurrentHandInfo.m_iBBChair != -1 && m_CurrentHandInfo.m_iSBChair != -1)
			break;

		int next_chair = (get_dealerchair()+(i+1)) % k_MaxChairs;
		double player_bet = CurState().m_player[next_chair].m_currentbet;

		// search SB (will be skipped if HU)
		if (m_CurrentHandInfo.m_iSBChair < 0)
		{
			if (IsEqual(player_bet, get_sblind()))
			{
				m_CurrentHandInfo.m_iSBChair = next_chair;
				m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, m_CurrentHandInfo.m_iSBChair, actionPostedSB, get_sblind(), CurState().m_player[next_chair].m_balance);

				// Hmmm...
				// m_CurrentHandInfo.m_PlayerActions[next_chair].actions[ePreflop][0].left_to_act = get_nplayersdealt() - 1;
			}
			else if (hero_is_bb && player_bet >= get_sblind())
			{
				m_CurrentHandInfo.m_iSBChair = next_chair;
				m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, next_chair, actionPostedSB, get_sblind(), CurState().m_player[next_chair].m_balance);

				// Hmmm...
				// m_CurrentHandInfo.m_PlayerActions[next_chair].actions[ePreflop][0].left_to_act = get_nplayersdealt() - 1;
				// m_CurrentHandInfo.m_PlayerActions[next_chair].actions[ePreflop][1].left_to_act = 1;
			}

			continue;
		}

		// search BB (will be executed if is_heads_up and hero_is_bb, skipped if hero is bb and not is_heads_up)
		if (m_CurrentHandInfo.m_iBBChair < 0 && next_chair != m_CurrentHandInfo.m_iSBChair && IsEqual(player_bet, get_bblind()))
		{
			if (next_chair == userchair)
				hero_is_bb = true;

			m_CurrentHandInfo.m_iBBChair = next_chair;	
			// By now the SB should be found -> Record Action
			m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, m_CurrentHandInfo.m_iBBChair, actionPostedBB, get_bblind(), CurState().m_player[next_chair].m_balance);

			// Hmmm...
			// m_CurrentHandInfo.m_PlayerActions[next_chair].actions[ePreflop][0].left_to_act = get_nplayersdealt() - 2;

			continue;
		}
	}

	// If we are BB record action after finding SB so that if a player calls BB it's not marked as a raise
	// (HU case is handled above)
	if(is_heads_up == false && m_CurrentHandInfo.m_iBBChair == userchair)
	{
		if (userchair >= 0 && userchair < k_MaxChairs) 
			m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, userchair, actionPostedBB, get_bblind(), CurState().m_player[userchair].m_balance);
		else
		{
			//error
		}

		// Hmmm...
		// m_CurrentHandInfo.m_PlayerActions[userchair].actions[ePreflop][0].left_to_act = get_nplayersdealt() - 2;
	}

	m_CurActor = (get_dealerchair()+1) % k_MaxChairs;
	m_bNewGame = false;
	m_PrevBetround = BetroundByCards;
} 

void CGlobalVars::ExtractActionsNewRound(enumStreets BetroundByCards)
{
	///////////////
	// NEW ROUND //
	///////////////

	gLog->WriteLog(eSeverityInfo, eCatHandHistory, "Starting New Round\n");
	m_CurrentHandInfo.NewBettingRound();

	if (BetroundByCards <= ePreflop || BetroundByCards > eRounds)
	{
		gLog->WriteLog(eSeverityError, eCatHandHistory, "ExtractActionsNewRound() errouneously activated (BetroundByCards=%d)\n", BetroundByCards);
		return;
	}

	//////////////////////////
	// FIND MISSING ACTIONS //
	//////////////////////////

	// Check to see what action we are missing between our last action in the previous round and this round
	for(int i = 0; i < k_MaxChairs; i++)
	{
		// Actions added by userchair are never missed -> skip :P
		if (i == userchair)
			continue;

		// Folded
		if (m_CurrentHandInfo.m_PlayerActions[i].bWasInGame && !InGame(CurState().m_player[i]))	
		{
			m_CurrentHandInfo.m_PlayerActions[i].bWasInGame = false;
			m_CurrentHandInfo.RecordPlayerAction(BetroundByCards-1, i, actionFold, 0, CurState().m_player[i].m_balance);

			FoldedBits[BetroundByCards-1][TimesActed[BetroundByCards-1]] |= k_exponents[i];
		}
		// Called
		else if (m_CurrentHandInfo.m_PlayerActions[i].prevBet[BetroundByCards-1] < m_CurrentHandInfo.m_AmountToCall[BetroundByCards-1] && InGame(CurState().m_player[i]))
		{
			m_CurrentHandInfo.RecordPlayerAction(BetroundByCards-1, i, actionCall, m_CurrentHandInfo.m_AmountToCall[BetroundByCards-1], CurState().m_player[i].m_balance);	
		}
		// Checked
		else if (InGame(CurState().m_player[i]) && 
			(IsEqual(m_CurrentHandInfo.m_AmountToCall[BetroundByCards-1], 0) && m_CurrentHandInfo.m_PlayerActions[i].actions[BetroundByCards-1][0].act == NoAction) ||
			(i == m_CurrentHandInfo.m_iBBChair && (BetroundByCards-1 == ePreflop) && IsEqual(m_CurrentHandInfo.m_AmountToCall[BetroundByCards-1], get_bblind())))
		{
			m_CurrentHandInfo.RecordPlayerAction(BetroundByCards-1, i, actionCheck, 0, CurState().m_player[i].m_balance);
		}

		// Hmmm...
		// WTF do I do here
		int tot_actions = m_CurrentHandInfo.m_PlayerActions[i].total_actions[BetroundByCards-1];
		m_CurrentHandInfo.m_PlayerActions[i].actions[BetroundByCards-1][tot_actions].left_to_act = LeftToAct(BetroundByCards - 1, i);
	}
		
	m_CurActor = (get_dealerchair()+1) % k_MaxChairs;
	m_bNewRound = false;
	m_PrevBetround = BetroundByCards;
}


int CGlobalVars::FindPreviousActor(const int chair)
{
	/*
	*
	*/

	if ((chair < 0) || ( chair >= k_MaxChairs))
		return -1;

	for (int i=0; i < k_MaxChairs; i++)
	{
		int index = (chair-(i+1) + k_MaxChairs) % k_MaxChairs;

		if (m_CurrentHandInfo.m_PlayerActions[index].bWasInGame)
			return index;
	}

	return -1;
}

int CGlobalVars::FindFirstActor(const int betting_round)
{
	/*
	*
	*/

	if (betting_round < 1 || betting_round > 4)
	{
		gLog->WriteLog(eSeveritySevere, eCatGeneral, "ERROR in StateProcessor::Find_First_Actor. Wrong argument <betting_round>\n");
		return -1;
	}

	int temp = 0;

	for (int i = 0; i < k_MaxChairs; i++)
	{
		int index = (get_dealerchair()+(i+1)) % k_MaxChairs;

		if (InGame(CurState().m_player[index])) 
		{
			if ((betting_round != 1) || 
				((temp == 2) && (PlayersInGame() > 2)) || 
				((temp == 1) && (PlayersInGame() == 2)))
			{
				return index;
			}

			temp++;
		}
	}

	gLog->WriteLog(eSeverityError, eCatGeneral, "ERROR in StateProcessor::Find_First_Actor. Can't find the first Actor\n");

	return -1;
}

int CGlobalVars::FindNextActor(const int chair)
{
	/*
	*
	*/

	if ((chair < 0)||(chair >= k_MaxChairs))
	{
		if (get_ismyturn())
			return userchair;

		return -1;
	}

	for (int i = 0; i < k_MaxChairs; i++)
	{
		int index = (chair + (i+1)) % k_MaxChairs;
		if (m_CurrentHandInfo.m_PlayerActions[index].bWasInGame)
		{
			return index;
		}
	}

	return -1;
}


void CGlobalVars::SetCurrentActor(enumStreets BetroundByCards, const bool is_heads_up, const bool hero_in_blinds)
{
	///////////////////////
	// SET CURRENT ACTOR //
	///////////////////////

	gLog->WriteLog(eSeverityInfo, eCatDebugNow, "BB (%d), Heads_Up (%s), In_Blinds (%s)\n", 
		m_CurrentHandInfo.m_iBBChair, bool_string(is_heads_up), bool_string(hero_in_blinds));

	if (userchair >= 0 && userchair < k_MaxChairs)
	{
		// Pick up missed actions if HU and BB
		if (m_CurrentHandInfo.m_PlayerActions[userchair].total_actions[BetroundByCards] > m_CurrentHandInfo.m_PlayerActions[get_dealerchair()].total_actions[BetroundByCards] &&
			m_CurrentHandInfo.m_iBBChair == userchair && BetroundByCards > ePreflop && is_heads_up)		
			m_CurActor = userchair;

		if (hero_in_blinds)
			m_CurActor = (m_CurrentHandInfo.m_iBBChair + 1) % k_MaxChairs;

		if(BetroundByCards == ePreflop && m_CurrentHandInfo.m_PlayerActions[userchair].total_actions[BetroundByCards] == 0)
			m_CurActor = (m_CurrentHandInfo.m_iBBChair + 1) % k_MaxChairs;
	}

	if (m_CurActor < 0)
	{
		gLog->WriteLog(eSeverityError, eCatStates, "Current Actor: %d (Trying to find First Actor)\n", m_CurActor);
		m_CurActor = (get_dealerchair()+1) % k_MaxChairs;
	}

	gLog->WriteLog(eSeverityInfo, eCatDebug, "Current Actor: %d\n", m_CurActor);
}

void CGlobalVars::ExtractActions(enumStreets BetroundByCards)
{
	//////////////////////////////
	// SEARCH REMAINING ACTIONS //
	//////////////////////////////

	TimesActed[BetroundByCards] = ntimesacted;
	InGameBits[BetroundByCards][TimesActed[BetroundByCards]] = get_playersplayingbits();

	for (int i = 0; i < k_MaxChairs; i++)
	{
		// default -> player after dealer
		int next_chair = (m_CurActor + i) % k_MaxChairs;

		/*
		gLog->WriteLog(eSeverityInfo, eCatDebugNow, "Chair%d: MaxChairs (%d), BB (%d), Heads_Up (%s), In_Blinds (%s)\n", 
			next_chair, k_MaxChairs, m_CurrentHandInfo.m_iBBChair, bool_string(is_heads_up), bool_string(hero_in_blinds));
		*/

		// Since we should only get here if it's our turn, stop at userchair. Players behind us had no chance to act yet ...
		if (next_chair == userchair)
		{
			if (i == 0)
				continue;
			else
				break;
		}

		// Not seated -> Ignore and continue :)
		if (!is_seated(next_chair, get_playersdealtbits()))
			continue;

		if (next_chair >= 0 && next_chair < k_MaxChairs)
		{
			double playerCurrentBet = CurState().m_player[next_chair].m_currentbet;
			int tot_actions = m_CurrentHandInfo.m_PlayerActions[next_chair].total_actions[BetroundByCards];

			// Wasn't in game and isn't in game (All Good) -> Ignore and continue
			if (!m_CurrentHandInfo.m_PlayerActions[next_chair].bWasInGame && !InGame(CurState().m_player[next_chair])) 
				continue;

			// Player popped-out from nowhere : No Magic -> Scrape Error ?
			if (!m_CurrentHandInfo.m_PlayerActions[next_chair].bWasInGame && InGame(CurState().m_player[next_chair])) 
			{
				gLog->WriteLog(eSeverityError, eCatHandHistory, "Error seat %d wasn't in the hand but is now in the hand\n", next_chair);
				continue;
			}

			// Fold
			if (m_CurrentHandInfo.m_PlayerActions[next_chair].bWasInGame && !InGame(CurState().m_player[next_chair]))
			{
				m_CurrentHandInfo.m_PlayerActions[next_chair].bWasInGame = false;
				m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, next_chair, actionFold, 0, CurState().m_player[i].m_balance);

				FoldedBits[BetroundByCards][TimesActed[BetroundByCards]] |= k_exponents[next_chair];

				// Hmmm...
				m_CurrentHandInfo.m_PlayerActions[next_chair].actions[BetroundByCards][tot_actions].left_to_act = LeftToAct(BetroundByCards, next_chair);

				continue;
			}

			// Check
			if (IsEqual(playerCurrentBet, m_CurrentHandInfo.m_PlayerActions[next_chair].prevBet[BetroundByCards]))
			{
				m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, next_chair, actionCheck, playerCurrentBet, CurState().m_player[i].m_balance);

				// Hmmm...
				m_CurrentHandInfo.m_PlayerActions[next_chair].actions[BetroundByCards][tot_actions].left_to_act = LeftToAct(BetroundByCards, next_chair);

				continue;
			}

			else if (playerCurrentBet > m_CurrentHandInfo.m_PlayerActions[next_chair].prevBet[BetroundByCards])
			{
				// Call
				if (IsEqual(m_CurrentHandInfo.m_AmountToCall[BetroundByCards], playerCurrentBet))
				{
					m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, next_chair, actionCall, playerCurrentBet, CurState().m_player[i].m_balance);
					m_CurrentHandInfo.m_PotRound += (playerCurrentBet-m_CurrentHandInfo.m_PlayerActions[next_chair].prevBet[BetroundByCards]);
				} 

				// Bet-Raise
				else 
				{
					m_CurrentHandInfo.RecordPlayerAction(BetroundByCards, next_chair, actionBetRaise, playerCurrentBet, CurState().m_player[i].m_balance);
					m_CurrentHandInfo.m_PotRound += (playerCurrentBet-m_CurrentHandInfo.m_PlayerActions[next_chair].prevBet[BetroundByCards]);
				}

				// Hmmm...
				m_CurrentHandInfo.m_PlayerActions[next_chair].actions[BetroundByCards][tot_actions].left_to_act = LeftToAct(BetroundByCards, next_chair);

				continue;
			}
		}
		else
		{
			//error
		}
	}
}


void CGlobalVars::ExtractContext(enumStreets BetroundByCards)
{
	for (int next_chair = 0; next_chair < k_MaxChairs; next_chair++)
	{
		int last_action_ndx = m_CurrentHandInfo.m_PlayerActions[next_chair].total_actions[BetroundByCards] - 1;
		int user_last_action_ndx = m_CurrentHandInfo.m_PlayerActions[userchair].total_actions[BetroundByCards] - 1;

		if (m_CurrentHandInfo.m_PlayerActions[userchair].actions[BetroundByCards] && last_action_ndx > 0)
			m_CurrentHandInfo.m_PlayerActions[next_chair].actions[BetroundByCards][last_action_ndx].left_to_act = LeftToAct(BetroundByCards, next_chair);
	}
}


void CGlobalVars::ExtractState(const holdem_state& curState)
{
	///////////////////
	// DETECT ERRORS //
	///////////////////

	if (!get_ismyturn())
		return;

	if (!InGame(curState.m_player[userchair]))
	{
		m_PrevBetround = 0;
		return;
	}

	if (bitcount(get_playersplayingbits() < 2))
		return;

	// Just like OpenHoldem we are always going to be processing the state from the last actor (dealer+1 for new rounds or userchair+1 for multiple actions per round). 
	// m_CurActor will hold this index
	if (!CardsDealt(curState))
	{
		m_bNewGame = true;
		m_bNewRound = true;
		gLog->WriteLog(eSeverityInfo, eCatDebugNow, "ExtractState Exit Early: No Cards Dealt to Bot\n");

		return;
	}

	// Check to see if we are at showdown which we will ignore otherwise initialize which players are in the hand
	// m_bNewGame will still be true the next time into ExtractState so we will do the initialization again.
	for(int i=0; i < k_MaxChairs; i++)
	{
		int l = (get_dealerchair()+(i+1)) % k_MaxChairs;

		if (((curState.m_player[l].m_cards[0] > 0) && (curState.m_player[l].m_cards[0] < 255)) && 
			 ((curState.m_player[l].m_cards[1] > 0) && (curState.m_player[l].m_cards[1] < 255)))
		{
			if (l != userchair) 
			{
				gLog->WriteLog(eSeverityError, eCatHandHistory, "Must be at showdown as we see opponent cards\n");

				return;
			}
		}
	}

	int nBoardCards = 0;

	for(int i=0; i<5;i++)
	{
		if (curState.m_cards[i]>0)
			nBoardCards++;
	}

	enumStreets BetroundByCards = eUnknown;

	switch (nBoardCards) 
	{
		case 0: 
			BetroundByCards = ePreflop; 
			break;
		case 3: 
			BetroundByCards = eFlop; 
			break;
		case 4: 
			BetroundByCards = eTurn; 
			break;
		case 5: 
			BetroundByCards = eRiver; 
			break;
		default: // This error condition should never happen as this case is checked for in IsStateUsable
			gLog->WriteLog(eSeverityError, eCatGeneral, "stableframe set to false (%d board cards found)\n", nBoardCards);
			return;
	}



	if (BetroundByCards > ePreflop && (m_PrevBetround != BetroundByCards-1) && (m_PrevBetround != BetroundByCards))
	{
		gLog->WriteLog(eSeverityError, eCatHandHistory, "stableframe set to false (skipped a whole street PrevBR=%d BetRound=%d)\n", m_PrevBetround, BetroundByCards);

		return;
	}

	if (get_betround() != BetroundByCards + 1)
		gLog->WriteLog(eSeverityError, eCatHandHistory, "betround(%d) != BetroundByCards(%d)\n", get_betround(), BetroundByCards+1);


	/////////////////////////////
	// DETECT NEW GAME / ROUND //
	/////////////////////////////

	// Determine if we started a new game by using the Dealer chair or the betting round has looped around.
	if ((m_PrevBetround > BetroundByCards) || (m_CurrentHandInfo.m_PrevDealerChair != get_dealerchair()) || (m_PrevHandsPlayed != get_handsplayed()))
	{
		m_bNewGame = true;
		m_bNewRound = false;

		std::fill(&HandLikelihood[0][0], &HandLikelihood[0][0]+k_MaxChairs*1326, max_weight);
	}
	else if (BetroundByCards > ePreflop && m_PrevBetround != BetroundByCards)
	{
		m_bNewGame = false;
		m_bNewRound = true;
	}

	int playersdealtbits = get_playersdealtbits();
	is_heads_up = (bitcount(playersdealtbits) == 2);
	hero_in_blinds = false;

	if (m_bNewGame)
		ExtractActionsNewGame(BetroundByCards, is_heads_up, hero_in_blinds);
	else if (m_bNewRound)  
		ExtractActionsNewRound(BetroundByCards);

	SetCurrentActor(BetroundByCards, is_heads_up, hero_in_blinds);
	ExtractActions(BetroundByCards);

	m_PrevBetround = BetroundByCards;
	m_PrevHandsPlayed = get_handsplayed();
	m_CurActor = userchair;

	//ExtractContext(BetroundByCards);
}



int CGlobalVars::LeftToAct(int BetroundByCards, int chair)
{
	// Hmmm...
	// ntimesacted ?
	if (chair == bb_chair() && BetroundByCards == ePreflop && ntimesacted == 0)
		return 0;

	if (chair == get_dealerchair() && (BetroundByCards > ePreflop || ntimesacted > 0))
		return 0;

	//gLog->WriteLog(eSeverityInfo, eCatDebugNow, "Chair = %d (BB = %d)\n", chair, bb_chair());

	int left_to_act = 0;
	int action_start_bits = InGameBits[BetroundByCards][TimesActed[BetroundByCards]] | FoldedBits[BetroundByCards][TimesActed[BetroundByCards]];

	for (int i = 0; i < k_MaxChairs; i++)
	{
		int index = (chair + (i+1)) % k_MaxChairs;

		if (index == chair)
			break;

		if (index >= 0 && index < k_MaxChairs)
		{
			if (action_start_bits & k_exponents[index])
			{
				left_to_act++;
				//gLog->WriteLog(eSeverityInfo, eCatDebug, "Playing Chair = %d\n", index);
			}

			if (index == bb_chair() && BetroundByCards == ePreflop && ntimesacted == 0)
				break;

			if (index == get_dealerchair() && (BetroundByCards > ePreflop || ntimesacted > 0))
				break;
		}
		else
		{
			//error
		}
	}

	return left_to_act;
}

