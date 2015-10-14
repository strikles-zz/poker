
#include <cmath>

#include "Bot/Bot_Globalvars.h"

#include "Util/Util.h"



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
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(m_state.CurState().m_player[chair].m_currentbet < get_bblind() - 0.01)
			return true;
	}

	//Determine sblind using past actions in preflop
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		for(int action=0; action < k_MaxActionsPerRound; action++)
		{
			if(m_state.m_current_hand_info.m_player_actions[chair].actions[0][action].act == eAction_SB)
				return true;
		}
	}

	//Determine sblind using my preflop position (if my preflop position is sb, there is a SB)
	if(set_preflop_position() == 1)
		return true;

	return false;
}

double CGlobalVars::set_maxcurrentbet()
{
	/*
	* returns the maximum currentbet on the table
	*/

	double maxbet = 0.0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(is_playing(chair) && m_state.CurState().m_player[chair].m_currentbet > maxbet)
			maxbet = m_state.CurState().m_player[chair].m_currentbet;
	}

	return ToMoney(maxbet);
}

int CGlobalVars::set_nopponentsbetting()
{
	/*
	* number of opponents betting
	*/

	int	count = 0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(chair == userchair)
			continue;
		if(!is_playing(chair))
			continue;
		if(m_state.CurState().m_player[chair].m_currentbet > get_bblind())
			count++;
	}

	return count;
}

int CGlobalVars::set_nopponentscalling()
{
	/*
	* number of opponents doing call
	*/

	int count = 0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(chair == userchair)
			continue;

		if(!is_playing(chair))
			continue;

		if(m_state.CurState().m_player[chair].m_currentbet > ncurrentbet-0.01 && m_state.CurState().m_player[chair].m_currentbet < ncurrentbet+0.01)
			count++;
	}

	if(count-1 > 0)
		return count-1;

	return 0;
}

double CGlobalVars::check_currentbet(int chair) 
{
	/*
	*
	*/

	if(chair >= 0 && chair < k_max_chairs)
		return currentbet[chair];

	return 0;
}

int CGlobalVars::set_nopponentsraising()
{
	/*
	* Finds nopponentsraising with our algorithm
	*/

	int raisers = 0;
	for(int chair=0; chair < k_max_chairs; chair++)
	{
		if(chair == userchair)
			continue;

		if(!is_playing(chair))
			continue;

		if(currentbet[chair] > check_currentbet(previous_player(get_ac_dealpos(chair))))
			raisers++;
	}

	return raisers;
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
	int adjusted_nopponentsraising = get_nopponentsraising();

	if(get_betround() == 1)
	{
		// If the player is the SB and the SB was posted and the player is playing
		// and the SB's currentbet is less bblind but greater than zero
		for(int chair=0; chair < k_max_chairs; chair++)
		{
			if(!is_playing(chair))
				continue;

			if(get_ac_dealpos(chair) == 1 && sb_posted && currentbet[chair] < get_bblind() && currentbet[chair] > 0)
				adjusted_nopponentsraising--;
		}

		// If the player is the BB and the player is playing
		// and the BB's currentbet is less or equal to (less than rare but possible)
		// bblind but greater than zero
		for(int chair=0; chair < k_max_chairs; chair++)
		{
			if(!is_playing(chair))
				continue;

			if(get_ac_dealpos(chair) == 2 && currentbet[chair] <= get_bblind() && currentbet[chair] > 0)
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

	int	dump		= 0;
	int	ncallbets	= get_ncallbets();

	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(is_seated(chair) &&	m_state.CurState().m_player[chair].m_currentbet > 0 && static_cast<int>(m_state.CurState().m_player[chair].m_currentbet / get_bblind()) == ncallbets) 
			dump++;
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

	int	dump		= 0;
	int	ncallbets	= get_ncallbets();

	for(int chair = 0; chair < k_max_chairs;chair++)
	{
		if(is_seated(chair) && m_state.CurState().m_player[chair].m_currentbet > 0 && static_cast<int>(m_state.CurState().m_player[chair].m_currentbet / get_bblind()) == ncallbets) 
			dump++;
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
	if(get_ncallbets() <= 1) 
		return false;

	if(get_betround() == 1)
	{
		//Preflop should only have blinds & raiser & caller
		if(info_nplayers_bet_less_then_current_raise() == 2 && info_nplayers_with_raise_amount() > 1) 
			return true;

	}
	else
	{
		//Postflop should only have raiser and caller
		if(info_nplayers_bet_less_then_current_raise() == 0 && info_nplayers_with_raise_amount() > 1) 
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
		if(currentbet[gVars.userchair] <= 0.001)
			return (m_state.CurState().m_player[raischair].m_balance + m_state.CurState().m_player[raischair].m_currentbet);
		else
			return (m_state.CurState().m_player[raischair].m_balance + m_state.CurState().m_player[raischair].m_currentbet - currentbet[userchair]);
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
	for(int chair = 0; chair < k_max_chairs;chair++)
	{
		if(!is_playing(chair))
			continue;

		if(currentbet[chair] > currentbet[userchair])
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
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(!is_playing(chair))
			continue;

		if(currentbet[chair] > bigCurrentbet)
			bigCurrentbet = currentbet[chair];
	}

	int dump = 0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(!is_playing(chair))
			continue;

		if(IsEqual(currentbet[chair], bigCurrentbet))
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
	double dump = 0.0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(!is_playing(chair))
			continue;

		if(currentbet[chair] > dump)
			dump = currentbet[chair];
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

	double dump			= 0.0;
	double opp_balance	= 0.0;

	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(!is_playing(chair))
			continue;

		if(currentbet[chair] > dump)
		{
			dump = currentbet[chair];
			opp_balance = m_state.CurState().m_player[chair].m_balance;
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

	if(get_betround() == 1 && (get_potplayer() <= get_sblind() + get_bblind()))
		return true;

	return false;
}

int CGlobalVars::first_limper()
{
	/*
	*  
	*/

	//Check UTG...BT
	for(int i=3; i < k_max_chairs; i++)
	{
		for(int j=0; j < k_max_chairs; j++)
		{
			if(IsEqual(currentbet[j], get_bblind()) && (get_ac_dealpos(j) == i))
				return j;
		}
	}

	//Check SB
	for(int i=0; i < k_max_chairs; i++)
	{
		if(IsEqual(currentbet[i], get_bblind()) && (get_ac_dealpos(i) == 1))
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

	if(get_betround() == 1)
	{
		if(sb_posted)
		{
			//SB posted
			if(IsEqual(get_potplayer(), get_sblind() + 2*get_bblind()))
				return true;

			if(set_preflop_position() == 2 && IsEqual(get_potplayer(), 2*get_bblind()))
				return true;
		}
		else
		{
			//no SB
			if(IsEqual(get_potplayer(), 2*get_bblind()))
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

	if(set_preflop_position() == 2 && IsEqual(get_potplayer(), 2*get_bblind()))
		return true;

	return false;
}

bool CGlobalVars::info_multiple_limpers()
{
	/*
	*  return true if there are multiple limpers
	*/

	if(get_betround() == 1 && get_ncallbets() <= 1)
	{
		if(sb_posted)
		{
			if(get_potplayer() > 2*get_bblind() + get_sblind())
				return true;
		}		
		else
		{
			if(get_potplayer() > 2*get_bblind()) 
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

	if(nopponentsbetting == 1)
		return true;

	if(get_betround() == 1)
	{
		if(sb_posted)
		{
			//Preflop should only have bb & sb & raiser & no caller
			if(info_nplayers_bet_less_then_current_raise() == 2 && info_nplayers_with_raise_amount() == 0)
				return true;
		}
		else
		{
			//Preflop should only have bb & raiser & no caller
			if(info_nplayers_bet_less_then_current_raise() == 1 && info_nplayers_with_raise_amount() == 0)
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
	if(get_ncallbets() <= 1) 
		return false;

	if(get_betround() == 1)
	{
		if(sb_posted)
		{
			//Preflop should only have blinds & raiser & caller
			if(info_nplayers_bet_less_then_current_raise() == 2 && info_nplayers_with_raise_amount() == 1)
				return true;

			//but im in bb, if sb do the call, should only have bb (me), raiser, caller (sb)
			if(set_preflop_position() == 2)
			{
				if(info_nplayers_bet_less_then_current_raise() == 1 && info_nplayers_with_raise_amount() == 1) 
					return true;
			}
		}
		else
		{
			//Preflop should only have blinds & raiser & caller
			if(info_nplayers_bet_less_then_current_raise() == 1 && info_nplayers_with_raise_amount() == 1) 
				return true;
		}
	}
	else
	{
		//Postflop should only have raiser and caller
		if(info_nplayers_bet_less_then_current_raise() == 0 && info_nplayers_with_raise_amount() == 1) 
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
	if(get_ncallbets() <= 1) 
		return false;

	if(get_betround() == 1)
	{
		if(sb_posted)
		{
			//Preflop should only have blinds & 2 or more raisers (players bet less than the last raise = sb + bb + one raise)
			if(info_nplayers_bet_less_then_current_raise() > 2) 
				return true;

			//but im in bb, if other player do raises and sb re-raises, there only two player that bet less (me and the other player)
			if(set_preflop_position() == 2)
			{
				// && sb_is_raiser()
				if(info_nplayers_bet_less_then_current_raise() > 1) 
					return true;
			}
		}
		else
		{
			//Preflop should only have big blind & 2 or more raisers (players bet less than the last raise = bb + one raise)
			if(info_nplayers_bet_less_then_current_raise() > 1) 
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

	if(adj_nopponentsraising == 0 && currentbet[userchair] <= 1)
		return 1;

	return 0;
}

int CGlobalVars::facing_1bet()
{
	/*
	*  
	*/

	if(adj_nopponentsraising == 1 && currentbet[userchair] <= 1)
		return 1;

	return 0;
}

int CGlobalVars::facing_2bet_plus()
{
	/*
	*  
	*/

	if(adj_nopponentsraising >= 2 && currentbet[userchair] <= 1)
		return 1;

	return 0;
}

int CGlobalVars::facing_2bet_response()
{
	/*
	*  
	*/

	if(adj_nopponentsraising == 1 && currentbet[userchair] > 1)
		return 1;

	return 0;
}

int CGlobalVars::facing_3bet_response()
{
	/*
	*  
	*/

	if(adj_nopponentsraising >= 2 && currentbet[userchair] > 1)
		return 1;

	return 0;
}

int CGlobalVars::X_bet()
{
	/*
	*  
	*/

	if(facing_0bet() == 1)
		return 0;
	else if(facing_1bet() == 1)
		return 1;
	else if(facing_2bet_plus() == 1)
		return 2;
	else if(facing_2bet_response() == 1)
		return 3;
	else if(facing_3bet_response() == 1)
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

	if(get_nplayersdealt() == 2 && (playersplayingbits & k_exponents[m_state.CurState().m_dealer_chair]))
		return true;

	return false;
}

double CGlobalVars::StackCommitment(int chair)
{
	double commitment = (initial_balance[chair] > 0 ? (initial_balance[chair] - m_state.CurState().m_player[chair].m_balance) / initial_balance[chair] : 0.0);
	return ToMoney(commitment);
}

double CGlobalVars::ReductionFactor(int chair)
{
	//double linear = 1 - invested;
	//double cosine = 1 + cos(0.5*PI*(invested+1));

	
	double sigmoid = 1.0 / (1 + exp(20 * StackCommitment(chair) - 4));

	if(sigmoid > 1)
		sigmoid = 1;
	else if(sigmoid < 0)
		sigmoid = 0;

	return sigmoid;
}

