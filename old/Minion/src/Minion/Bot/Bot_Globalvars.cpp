
#include <cassert>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "Bot/Bot_Globalvars.h"

#include "Logic/Logic.h"


// Constructor
CGlobalVars::CGlobalVars()
:
	userchair(-1),
	//prw_xpoker_equity(0.0),
	iserr(false)
{
	//LoadLuts();
}


// Destructor
CGlobalVars::~CGlobalVars()
{
}


void CGlobalVars::init_userchair()
{
	//USERCHAIR FIRST
	userchair = static_cast<int>(gws("userchair"));
	assert (userchair >= 0 && userchair < k_max_chairs);
}

void CGlobalVars::calc_oh_symbols()
{
	/*
	*  Initialize OH symbols and variables
	*/

	init_userchair();	
	

	/////////////////////////////////////////////////////////
	////////////// OpenHoldem Calculated Symbols ////////////
	/////////////////////////////////////////////////////////
	DirtyOHVars();

	br = get_betround();
	sb = get_sblind();
	bb = get_bblind();	

	ntimesacted = get_didcall() + get_didchec() + get_didrais() + get_didswag();
	enabled_flag[0] = get_f0();
	enabled_flag[1] = get_f1();

	//////////////////////////////////////////////////////////////////
	//OWN VARS
	//////////////////////////////////////////////////////////////////
	int balance_not_0 = 0;
	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(is_seated(chair))
		{
			currentbet[chair]	= ToMoney(m_state.CurState().m_player[chair].m_currentbet);
			balance[chair]		= ToMoney(m_state.CurState().m_player[chair].m_balance);

			assert(currentbet[chair] >= 0 && balance[chair] >= 0);

			if(get_betround() == ePreflop && ntimesacted == 0)
				initial_balance[chair] = balance[chair] + currentbet[chair];
		}
		else
		{
			balance[chair]			= 0.0;
			currentbet[chair]		= 0.0;
			initial_balance[chair]	= 0.0;
		}

		if (balance[chair] > 0)
			balance_not_0 ++;
	}

	//GENERAL VARS
	sb_posted = set_SB();

	ncurrentbet = set_maxcurrentbet();
	call = ToMoney(ncurrentbet - currentbet[userchair]);
	assert(call >= 0);

	//stack_depth = set_stack_depth(); //variable not used and not defined

	//OPPONENT VARS
	nopponentsbetting		= set_nopponentsbetting();
	nopponentscalling		= set_nopponentscalling();
	nopponents = nopponentsbetting + nopponentscalling;

	raischair				= set_raischair();
	adj_nopponentsraising	= set_adj_nopponentsraising();
	nplayersplaying			= bitcount(get_playersplayingbits());

	//PREFLOP VARS
	dealpositionrais = set_dealpositionrais();

	//updating card values
	set_card_values();
	//updating hand values
	issuited		= set_suited();
	rankhiplayer	= set_rankhiplayer();
	rankloplayer	= set_rankloplayer();

	//assign myHand
	getMyHand(); 
}





bool CGlobalVars::is_playing(unsigned int chair)
{
	/*
	* returns if player is playing at specified chair
	*/

	assert (chair < k_max_chairs);
	return (get_playersplayingbits() & k_exponents[chair]) != 0;
}

bool CGlobalVars::is_seated(unsigned int chair)
{
	/*
	* returns if player is playing at specified chair
	*/

	assert (chair < k_max_chairs);	
	return (get_playersseatedbits() & k_exponents[chair]) != 0;
}

bool CGlobalVars::InBits(int chair, unsigned int playing_bits )
{
	/*
	* returns if player is playing at specified chair
	*/

	assert (chair >= 0 && chair < k_max_chairs);
	return (playing_bits & k_exponents[chair]) != 0;
}

//////////////////////////////////////
////// OWN FUNCTIONS
//////////////////////////////////////
bool CGlobalVars::isActionAvailable( int action )
{
	/*
	* returns true if button is available
	* buttons:
	*	-F(0) Fold
	*	-C(1) Call
	*	-R(2) Raise
	*	-A(3) All-in
	*	-K(4) Check
	*/

	assert(action >= 0 && action < 5);
	unsigned int my_turn_bits = get_myturnbits();
	return (my_turn_bits & k_exponents[action]) != 0;
}


void CGlobalVars::RecordBotsDecision(ePlayerAction action, eBetType betType, double amount)
{
	/*
	* Record the bot decision
	*/

	enumStreetIndices br_ndx = (enumStreetIndices)BR2NDX(br);
	assert(br_ndx >= ePreflopIndex && br_ndx < eRoundIndices);

	if(m_state.m_current_hand_info.was_bots_action_recorded)
		return;

	m_state.m_current_hand_info.RecordPlayerAction(br_ndx, userchair, action, amount);
}


void CGlobalVars::RecordNewGame()
{
	/*
	*  
	*/

	//Unused function?

}

void CGlobalVars::display_actions()
{
	std::string debug_msg;
	for(int br_ndx = 0; br_ndx < gVars.get_betround(); br_ndx++)
	{
		std::stringstream br_str, amt_call, times_acted;

		br_str		<< br_ndx + 1;
		amt_call	<< m_state.m_current_hand_info.m_amount_to_call[br_ndx];
		times_acted	<< gVars.ntimesacted;

		debug_msg += ("\nBR " + br_str.str() + " Call : " + amt_call.str() + " Times_Acted : " + times_acted.str() + "\n\n");

		for(int ndx = 0; ndx < k_max_chairs; ndx++)
		{
			int chair = (get_dealerchair() + (ndx+1)) % k_max_chairs;
			if(gVars.InBits(chair, gVars.get_playersseatedbits()))
			{
				std::stringstream num_player, prev_bet, num_actions, act_string, to_act;
				int total_actions = m_state.m_current_hand_info.m_player_actions[chair].total_actions[br_ndx];

				num_player	<< chair;
				num_actions	<< std::left << std::setw(4) << total_actions;
				to_act		<< std::left << std::setw(4) << m_state.LeftToAct(chair);
				to_act		<< std::left << std::setw(4) << m_state.m_current_hand_info.m_player_actions[chair].actions[br_ndx][total_actions-1].to_act;
				act_string	<< std::left << std::setw(12) << m_state. m_current_hand_info.m_player_actions[chair].actions_this_round[br_ndx];
				prev_bet	<< std::setw(8) << m_state.m_current_hand_info.m_player_actions[chair].prevBet[br_ndx];		

				debug_msg += (num_player.str() + " - To Act : " + to_act.str() + " #Actions : " + num_actions.str() + " - " +  act_string.str() + prev_bet.str() + "\n");
			}
		}
	}

	MessageBox(NULL, debug_msg.c_str(), "Player Actions Debug", MB_OK | MB_ICONASTERISK);
}

