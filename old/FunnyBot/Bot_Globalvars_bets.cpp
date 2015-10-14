#include "stdafx.h"

#include "Bot_Globalvars.h"
#include "Util_MagicNumbers.h"



//////////////////////////
// Actions
//////////////////////////
bool CGlobalVars::set_SB()
{
	/*
	* checks if sblind was posted at the table
	*/

	//FIX: only set_SB one time per game

	//Determine small blind 
	for(int i = 0; i < k_MaxChairs; i++)
	{
		if(CurState().m_player[i].m_currentbet < get_bblind() - 0.01)
			return true;
	}

	//Determine sblind using past actions in preflop
	for(int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++)
	{
		for (int j=0; j < k_MaxActionsPerRound; j++)
		{
			if(gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].actions[0][j].act == actionPostedSB)
				return true;
		}
	}

	//Determine sblind using my preflop position (if my preflop position is sb, there is a SB)
	if(gVars.set_preflop_position() == 1)
		return true;

	return false;
}

double CGlobalVars::set_ncurrentbet()
{
	/*
	* returns the maximum currentbet on the table
	*/

	double dumpvar = 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if(is_playing(i) && CurState().m_player[i].m_currentbet > dumpvar)
			dumpvar = CurState().m_player[i].m_currentbet;
	}

	return dumpvar;
}

int CGlobalVars::set_nopponentsbetting()
{
	/*
	* number of opponents betting
	*/

	int lok_count = 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if(i == userchair)
			continue;

		if (!is_playing(i))
			continue;

		if(CurState().m_player[i].m_currentbet > get_bblind())
			lok_count++;
	}

	return lok_count;
}

int CGlobalVars::set_nopponentscalling()
{
	/*
	* number of opponents doing call
	*/

	int lok_count = 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if(i == userchair)
			continue;

		if (!is_playing(i))
			continue;

		if (CurState().m_player[i].m_currentbet > ncurrentbet-0.01 && CurState().m_player[i].m_currentbet < ncurrentbet+0.01)
				lok_count++;
	}

	if(lok_count-1 > 0)
		return lok_count-1;

	return 0;
}

double CGlobalVars::check_currentbet(int chair) 
{
	/*
	*
	*/

	if(chair >= 0 && chair < k_MaxChairs)
		return currentbets[chair];

	return 0;
}

int CGlobalVars::set_nopponentsraising()
{
	/*
	* Finds nopponentsraising with our algorithm
	*/

	int temp_raisers = 0;

	for (int i=0; i < k_MaxChairs; i++)
	{
		if(i == userchair)
			continue;

		if (!is_playing(i))
			continue;

		if (currentbets[i] > check_currentbet(previous_player(get_ac_dealpos(i))))
			temp_raisers++;
	}

	return temp_raisers;
}

int CGlobalVars::set_adj_nopponentsraising()
{
	/*
	* 
	*/
	//This sub adjusts nopponentsraising that we use our functions to find
	// not OH's since there are bugs.

	//It adjusts nopponentsraising for preflop
	// to subtract if sb or bb isn't raising
	int adjusted_nopponentsraising = nopponentsraising;

	if (br == 1)
	{
		// If the player is the SB and the SB was posted and the player is playing
		// and the SB's currentbet is less bblind but greater than zero
		for (int i=0; i < k_MaxChairs; i++)
		{
			if (!is_playing(i))
				continue;

			if (get_ac_dealpos(i) == 1 && SB && currentbets[i] < get_bblind() && currentbets[i] > 0)
				adjusted_nopponentsraising--;
		}

		// If the player is the BB and the player is playing
		// and the BB's currentbet is less or equal to (less than rare but possible)
		// bblind but greater than zero
		for (int i=0; i < k_MaxChairs; i++)
		{
			if (!is_playing(i))
				continue;

			if (get_ac_dealpos(i) == 2 && currentbets[i] <= get_bblind() && currentbets[i] > 0)
				adjusted_nopponentsraising--;
		}
	}

	return adjusted_nopponentsraising;
}



////////////////////////
// PREFLOP FUNCTIONS
////////////////////////

////////////////////////////
// Raise context
////////////////////////////
int CGlobalVars::info_nplayers_with_raise_amount()
{
	/*
	* Return number of players who have called the current bet
	*
	* Examples:
	* -Five players call bigblind (5 plus bblind), return 5
	* -One player raises and one player calls, return 1
	*/

	int dump = 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if (is_seated(i, playersseatedbits) &&
			CurState().m_player[i].m_currentbet > 0 && static_cast<int>(CurState().m_player[i].m_currentbet / get_bblind()) == get_ncallbets()) 
		{
				dump++;
		}
	}
	if(dump > 0)
	{
		// if one player raises and one player calls, their currentbet/bblind == ncallbets and we count 2 (but only call one player, so, we return 2-1 = 1)
		return (dump - 1);
	}
	else 
	{
		// no players call the current bet
		return dump;
	}
}

int CGlobalVars::info_nplayers_bet_less_then_current_raise()
{
	/*
	* Return number of players who have called less than the current bet (IE someone has raised after their bet/call/raise)
	*
	* Examples:
	* -One player raises, bblind and sblind posted, return 2
	* -One player raises, other player reraises, bblind and sblind posted, return 3
	*/

	int dump = 0;

	for(int i = 0; i < k_MaxChairs;i++)
	{
		if (is_seated(i, playersseatedbits) && 
			CurState().m_player[i].m_currentbet > 0 && static_cast<int>(CurState().m_player[i].m_currentbet / get_bblind()) == get_ncallbets()) 
		{
				dump++;
		}
	}

	return dump;
}

bool CGlobalVars::info_single_raiser_multiple_caller()
{
	/*
	* Return true if only a single raise with a multiple caller
	*
	* Examples:
	* -bblind and sblind posted, the rest of players fold, return 0
	* -bblind and sblind posted, one player raises and one player calls, return 0
	* -bblind and sblind posted, one player raises and two player calls, return 1
	FIX: SB no posted?
	*/

	// If there is no raise then false
	if (gVars.get_ncallbets() <= 1) 
		return false;

	if(br == 1)
	{
		//Preflop should only have blinds & raiser & caller
		if (info_nplayers_bet_less_then_current_raise() == 2 && info_nplayers_with_raise_amount() > 1) 
			return true;

	}
	else
	{
		//Postflop should only have raiser and caller
		if (info_nplayers_bet_less_then_current_raise() == 0 && info_nplayers_with_raise_amount() > 1) 
			return true;			
	}

	return false;
}

double CGlobalVars::raiser_balance()
{	
	/*
	* Return balance+bet of the player who did raise or 0 if not found
	*
	* Examples:
	* -
	* -
	*/

	if(raischair >= 0 && raischair < 10)
	{
		if(currentbet <= 0.001)
			return (CurState().m_player[raischair].m_balance + CurState().m_player[raischair].m_currentbet);
		else
			return (CurState().m_player[raischair].m_balance + CurState().m_player[raischair].m_currentbet - currentbet);
	}

	return 0;
}

int CGlobalVars::number_of_reraises()
{
	/*
	* Return number of reraises after my raise
	*
	* Examples:
	* -
	* -
	*/

	int dump = 0;

	for(int i = 0; i < k_MaxChairs;i++)
	{
		if (!is_playing(i))
			continue;

		if (CurState().m_player[i].m_currentbet > currentbet)
			dump++;
	}

	return dump;
}

int CGlobalVars::other_opp_call_reraise()
{
	/*
	* Return number of callers of the bigger raise
	*
	* Examples:
	* -
	* -
	*/

	double bigCurrentbet= 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if (!is_playing(i))
			continue;

		if (CurState().m_player[i].m_currentbet > bigCurrentbet)
			bigCurrentbet = CurState().m_player[i].m_currentbet;
	}

	int dump = 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if (!is_playing(i))
			continue;

		if (IsEqual(CurState().m_player[i].m_currentbet, bigCurrentbet))
			dump++;
	}

	if(dump > 0)
		return dump-1;

	return dump;
}

double CGlobalVars::reraiser_opp_currentbet()
{
	/*
	* Return reraiser opponent currentbet
	*
	* Examples:
	* -
	* -
	*/
	double dump = 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if (!is_playing(i))
			continue;

		if (CurState().m_player[i].m_currentbet > dump)
			dump = CurState().m_player[i].m_currentbet;
	}

	return dump;
}

double CGlobalVars::reraiser_opp_balance()
{
	/*
	* Return reraiser opponent balance
	*
	* Examples:
	* -
	* -
	*/

	double dump = 0;
	double opp_balance = 0;

	for(int i = 0; i < k_MaxChairs; i++)
	{
		if (!is_playing(i))
			continue;

		if (CurState().m_player[i].m_currentbet > dump)
		{
			dump = CurState().m_player[i].m_currentbet;
			opp_balance = CurState().m_player[i].m_balance;
		}
	}

	return opp_balance;
}

bool CGlobalVars::reraiser_opp_is_allin()
{
	/*
	* Return true is opp reraiser is allin
	*
	* Examples:
	* -
	* -
	*/

	if(reraiser_opp_balance() < 0.001)
		return true;

	return false;
}


/////////////////////////////
// Limpers
/////////////////////////////



bool CGlobalVars::info_first_to_act()
{
	/*
	*  return true is I'm the first to act
	*/

	if(br == 1 && (gVars.get_potplayer() <= gVars.get_sblind() + gVars.get_bblind()))
		return true;

	return false;
}

int CGlobalVars::first_limper()
{
	/*
	*  
	*/

	//Check UTG...BT
	for(int i=3; i < k_MaxChairs; i++)
	{
		for(int j=0; j < k_MaxChairs; j++)
		{
			if(IsEqual(currentbets[j], get_bblind()) && (get_ac_dealpos(j) == i))
				return j;
		}
	}

	//Check SB
	for(int i=0; i < k_MaxChairs; i++)
	{
		if(IsEqual(currentbets[i], get_bblind()) && (get_ac_dealpos(i) == 1))
			return i;
	}

	return -1;
}

bool CGlobalVars::info_single_limper()
{
	/*
	*  return true if there is a single limper
	* Example:
	*  -true: I'm Bigblind, SB fold and other player limped
	*  -true: I'm Bigblind, SB limped and the rest fold
	*/

	if (br==1)
	{
		if(SB)
		{
			//SB posted (2*bblind+1*sblind)
			if(IsEqual(get_potplayer(), (get_sblind() + get_bblind() + get_bblind())))
				return true;

			if(set_preflop_position() == 2 && IsEqual(get_potplayer(), (get_bblind() + get_bblind())))
				return true;
		}
		else
		{
			//no SB (2*bblind)
			if(IsEqual(get_potplayer(), get_bblind() + get_bblind()))
				return true;
		}
	}

	return false;
}

bool CGlobalVars::info_sb_limped()
{
	/*
	*  return true if smallblind limped
	*/

	if(set_preflop_position() == 2 && IsEqual(get_potplayer(), (get_bblind() + get_bblind())))
		return true;

	return false;
}

bool CGlobalVars::info_multiple_limpers()
{
	/*
	*  return true if there are multiple limpers
	*/

	if(br == 1 && get_ncallbets() <= 1)
	{
		if(SB)
		{
			if(get_potplayer() > (get_bblind() + get_bblind() + get_sblind())) 
				return true;
		}		
		else
		{
			if(get_potplayer() > (get_bblind() + get_bblind())) 
				return true;
		}
	}

	return false;
}

bool CGlobalVars::info_single_raiser_no_caller()
{
	/*
	*  return true if single raiser with no caller
	*/

	if (gVars.nopponentsbetting == 1)
		return true;

	if((br==1))
	{
		if(SB)
		{
			//Preflop should only have bb & sb & raiser & no caller
			if (info_nplayers_bet_less_then_current_raise() == 2 && info_nplayers_with_raise_amount() == 0)
				return true;
		}
		else
		{
			//Preflop should only have bb & raiser & no caller
			if (info_nplayers_bet_less_then_current_raise() == 1 && info_nplayers_with_raise_amount() == 0)
				return true;
		}
	}

	return false;
}

bool CGlobalVars::info_single_raiser_single_caller()
{
	/*
	* Return true if only a single raise with a single caller
	*
	* Examples:
	* -bblind and sblind posted, the rest of players fold, return 0
	* -bblind and sblind posted, one player raises and one player calls, return 1
	* -bblind and sblind posted, one player raises and two player calls, return 0
	FIX: SB no posted? Fixed but need test
	*/

	// If there is no raise then false
	if (gVars.get_ncallbets() <= 1) 
		return false;

	if(br == 1)
	{
		if(SB)
		{
			//Preflop should only have blinds & raiser & caller
			if (info_nplayers_bet_less_then_current_raise() == 2 && info_nplayers_with_raise_amount() == 1)
				return true;

			//but im in bb, if sb do the call, should only have bb (me), raiser, caller (sb)
			if(set_preflop_position() == 2)
			{
				if (info_nplayers_bet_less_then_current_raise() == 1 && info_nplayers_with_raise_amount() == 1) 
					return true;
			}
		}
		else
		{
			//Preflop should only have blinds & raiser & caller
			if (info_nplayers_bet_less_then_current_raise() == 1 && info_nplayers_with_raise_amount() == 1) 
				return true;
		}
	}
	else
	{
		//Postflop should only have raiser and caller
		if (info_nplayers_bet_less_then_current_raise() == 0 && info_nplayers_with_raise_amount() == 1) 
			return true;
	}

	return false;
}

bool CGlobalVars::info_multiple_raisers()
{
	/*
	* Return true if there are multiple raiser
	*
	* Examples:
	FIX: sb_is_raiser()
	*/

	// If there is no raise then false
	if (gVars.get_ncallbets() <= 1) 
		return false;

	if(br == 1)
	{
		if(SB)
		{
			//Preflop should only have blinds & 2 or more raisers (players bet less than the last raise = sb + bb + one raise)
			if (info_nplayers_bet_less_then_current_raise() > 2) 
				return true;

			//but im in bb, if other player do raises and sb re-raises, there only two player that bet less (me and the other player)
			if(set_preflop_position() == 2)
			{
				// && sb_is_raiser()
				if (info_nplayers_bet_less_then_current_raise() > 1) 
					return true;
			}
		}
		else
		{
			//Preflop should only have big blind & 2 or more raisers (players bet less than the last raise = bb + one raise)
			if (info_nplayers_bet_less_then_current_raise() > 1) 
				return true;
		}
	}

	return false;
}



////////////////////////
// Facing action
////////////////////////
int CGlobalVars::facing_0bet()
{
	/*
	*  
	*/

	if (adj_nopponentsraising == 0 && currentbet <= 1)
		return 1;
	else
		return 0;
}

int CGlobalVars::facing_1bet()
{
	/*
	*  
	*/

	if (adj_nopponentsraising == 1 && currentbet <= 1)
		return 1;
	else
		return 0;
}

int CGlobalVars::facing_2bet_plus()
{
	/*
	*  
	*/

	if (adj_nopponentsraising >= 2 && currentbet <= 1)
		return 1;
	else
		return 0;
}

int CGlobalVars::facing_2bet_response()
{
	/*
	*  
	*/

	if (adj_nopponentsraising == 1 && currentbet > 1)
		return 1;
	else
		return 0;
}

int CGlobalVars::facing_3bet_response()
{
	/*
	*  
	*/

	if (adj_nopponentsraising >= 2 && currentbet > 1)
		return 1;
	else
		return 0;
}

int CGlobalVars::X_bet()
{
	/*
	*  
	*/

	if (facing_0bet() == 1)
		return 0;
	else if (facing_1bet() == 1)
		return 1;
	else if (facing_2bet_plus() == 1)
		return 2;
	else if (facing_2bet_response() == 1)
		return 3;
	else if (facing_3bet_response() == 1)
		return 4;

	return 0;
}

double CGlobalVars::info_fold_to_3bet()
{
	/*
	*  
	*/
	//FUNCTION TODO!!

	return 0;
}





bool CGlobalVars::is_headsup()
{
	/*
	*  return true if we are hu
	*/
	unsigned int playersplayingbits = get_playersplayingbits();

	if (get_nplayersdealt() == 2 && (playersplayingbits & k_exponents[get_dealerchair()]))
	{
		return true;
	}

	return false;
}

double CGlobalVars::LeftToInvest(int iPlayer)
{
	return (initial_balance[iPlayer] > 0 ? (initial_balance[iPlayer] - CurState().m_player[iPlayer].m_balance) / initial_balance[iPlayer] : 0);
}

double CGlobalVars::ReductionFactor(int iPlayer)
{
	//double linear = 1 - invested;
	//double cosine = 1 + cos(0.5*PI*(invested+1));

	
	double sigmoid = 1.0 / (1 + exp(20*LeftToInvest(iPlayer) - 4));

	if (sigmoid > 1)
		sigmoid = 1;
	else if (sigmoid < 0)
		sigmoid = 0;

	return sigmoid;
}

