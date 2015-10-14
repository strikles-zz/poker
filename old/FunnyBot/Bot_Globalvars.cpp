#include "stdafx.h"

#include <io.h>
#include <tchar.h>

#include <bitset>
#include <sstream>
#include <iomanip>

#include "Bot_Globalvars.h"
#include "Logic.h"
#include "Util_Checksum.h"

//CGlobalVars* CGlobalVars::m_gVars = NULL;


// Constructor
CGlobalVars::CGlobalVars() :

	br(0),
	userchair(-1),
	maxchairs(k_MaxChairs),

	round_trigger(0),

	ntimesacted(0),
	decision(0.0),

	SUMprwin(0.0),
	avg_prwin(0.0),
	SUMvar_prwin(0.0),
	var_prwin(0.0),
	prw_xpoker_equity(0.0),

	m_bNewGame(true),
	m_bNewRound(true),
	m_CurActor(-1),
	m_PrevBetround(1),

	iserr(false)
{
	t_rand.seed(rnd_device());
	LoadLuts();
}


// Destructor
CGlobalVars::~CGlobalVars()
{
	delete p_flop;
	delete p_turn;
	delete p_river;

	//delete m_gVars;
}


int CGlobalVars::VerifyLut(const int lut_ndx)
{
	bool valid_path = true;
	char debug_msg[512];

	if (_access(lut_specs[lut_ndx].filename, 0) == -1)
	{		
		sprintf_s(debug_msg, 512, 
			"ERROR : Could not find %s !\n\n"
			"Generate the file with the Lutinator\n"
			"and place it in the CWD to continue.\n\n"
			"FunnyBot will now crash\n"
			"Press OK to Exit...", 
			lut_specs[lut_ndx].filename);

		MessageBox(NULL, debug_msg, "ERROR : LUT", MB_OK);
		valid_path = false;
	}

	if (valid_path == false)
		exit(1);

	
	bool valid_checksum = true;
	char current_md5[33];
	CalcChecksum(lut_specs[lut_ndx].filename, current_md5);
	

	if (strncmp(lut_specs[lut_ndx].md5sum, current_md5, 33))
	{
		sprintf_s(debug_msg, 512, 
			"ERROR : Checksum mismatch for %s \n\n"
			"Expected checksum : %s\n"
			"current checksum : %s\n"
			"Regenerate the file with the Lutinator\n"
			"and place it in the CWD to continue.\n\n"
			"FunnyBot will now crash\n"
			"Press OK to Exit...", 
			lut_specs[lut_ndx].filename, 
			lut_specs[lut_ndx].md5sum, 
			current_md5);

		MessageBox(NULL, debug_msg, "ERROR : MD5 Checksum Failed", MB_OK);
		valid_checksum = false;
	}

	if (valid_checksum == false)
		exit(1);

	return 0;
}


int CGlobalVars::LoadLuts()
{
	for (int lut_ndx = 0; lut_ndx < 6; lut_ndx++)
		VerifyLut(lut_ndx);

	// Load LUTS
	p_flop = new CLutFlop();
	p_flop->read_suitfile("flop_suit.dat");
	p_flop->load("flop.dat");

	p_turn = new CLutTurn();
	p_turn->read_suitfile("turn_suit.dat");
	p_turn->load("turn.dat");

	p_river = new CLutRiver();
	p_river->read_suitfile("river_suit.dat");
	p_river->load("river.dat");

	return 0;
}


void CGlobalVars::init_oh_symbols()
{
	//USERCHAIR FIRST
	userchair = static_cast<int>(gws(0, "userchair"));
	playersseatedbits = static_cast<int>(gws(userchair, "playersseatedbits"));

	assert (userchair >= 0 && userchair < k_MaxChairs);

	br = get_betround();
	maxchairs = get_nchairs();

	enabled_flag[0] = get_f0();

	didswag = static_cast<int>(gws(userchair,"didswag"));
	didrais = static_cast<int>(gws(userchair,"didrais"));
	didcall = static_cast<int>(gws(userchair,"didcall"));
	didchec = static_cast<int>(gws(userchair,"didchec"));
}

void CGlobalVars::calc_oh_symbols()
{
	/*
	*  Initialize OH symbols and variables
	*/

	init_oh_symbols();	
	DirtyOHVars();

	/////////////////////////////////////////////////////////
	////////////// OpenHoldem Calculated Symbols ////////////
	/////////////////////////////////////////////////////////

	ntimesacted = didswag + didrais + didchec + didcall;

	//////////////////////////////////////////////////////////////////
	//OWN VARS
	///////////////////////////////////////////////////////////////////

	// Belongs in state extraction ?
	for (int i = 0; i < k_MaxChairs; i++)
	{
		if (is_seated(i, playersseatedbits))
			currentbets[i] = CurState().m_player[i].m_currentbet;

		if (br == 1 && ntimesacted == 0)
		{
			if (is_seated(i, playersseatedbits))
				initial_balance[i] = CurState().m_player[i].m_balance - currentbets[i];
			else
				initial_balance[i] = 0;
		}
	}

	//GENERAL VARS
	SB = set_SB();

	currentbet = CurState().m_player[userchair].m_currentbet;
	balance = CurState().m_player[userchair].m_balance;

	ncurrentbet = set_ncurrentbet();
	call = ncurrentbet - currentbet;	

	//stack_depth = set_stack_depth(); //variable not used and not defined

	//OPPONENT VARS
	nopponentsbetting = set_nopponentsbetting();
	nopponentscalling = set_nopponentscalling();
	nopponents = nopponentsbetting + nopponentscalling;

	nopponentsraising = set_nopponentsraising();
	raischair = set_raischair();
	adj_nopponentsraising = set_adj_nopponentsraising();

	nopponentsplaying = bitcount(get_opponentsplayingbits());

	//PREFLOP VARS
	dealpositionrais = set_dealpositionrais();

	/*
	pf_entered = set_pf_entered(); //not used
	limpReraiser = set_limp_reraiser(); //not used
	no_limpers = set_no_limpers(); //not used
	isolatedLimper = set_isolated_limper(); //not used
	threeBetters = set_three_betters(); //not used
	*/

	//updating card values
	set_card_values();

	//updating hand values
	issuited = set_suited();
	rankhiplayer = set_rankhiplayer();
	rankloplayer = set_rankloplayer();

	//assign myHand
	getMyHand(); 


	//////////////////
	// Getting stats from opponents
	/////////////////

	m_PlayerStatistics.SetUserChair(userchair);

	for (int i = 0; i < k_MaxChairs; i++)
	{
		if (i == userchair)
			continue;

		if(!is_seated(i, playersseatedbits))
			continue;
				
		if ((m_PlayerStatistics.opponentInfo[i].nReadsDeferred < 20) && (_tcscmp(m_PlayerStatistics.opponentInfo[i].name, CurState().m_player[userchair].m_name) == 0))
		{
			//Opponent is the same, so we add 1 to nReadsDeferred and when we do 20 reads we will get stats again
			m_PlayerStatistics.opponentInfo[i].nReadsDeferred++;
			continue;
		}

		//Restart nReadsDeferred
		m_PlayerStatistics.opponentInfo[i].nReadsDeferred = 0;
		//Opponent changed, so we get the new name
		_tcscpy_s(m_PlayerStatistics.opponentInfo[i].name, CurState().m_player[i].m_name);

		m_PlayerStatistics.GetDirtyPTVars(i);
		m_PlayerStatistics.SetDirtyPTStats(i);
	}

	//////////////////
	// raischair
	/////////////////
	//Gets the pfr from the raischair before we convert it dynamically
	orig_rais_chair_pfr = set_rais_chair_pfr();
	//Adjust the pfr for the position of the raiser
	rais_chair_pfr = adj_pfr();
	//Normal vpip for raischair
	rais_chair_vpip = set_rais_chair_vpip();
	//Normal hands
	rais_chair_hands = set_rais_chair_hands();
	//Dynamically adjust postion
	rais_chair_position = pf_pos_rais();
	//Normal ats for raischair
	rais_chair_ats = set_rais_chair_ats();
}


int CGlobalVars::PlayersInGame()
{
	/*
	* 
	*/

	int temp = 0;

	for (int i = 0; i < k_MaxChairs; i++)
	{
		if (m_CurrentHandInfo.m_PlayerActions[(get_dealerchair()+(i+1)) % k_MaxChairs].bWasInGame) 
			temp++;
	}

	return temp;
}


bool CGlobalVars::InGame(const holdem_player& player)
{
	return (player.m_cards[0] > 0 && player.m_cards[0] <= 255 && player.m_cards[1] > 0 && player.m_cards[1] <= 255);
}


bool CGlobalVars::is_playing(int chair)
{
	/*
	* returns if player is playing at specified chair
	*/

	assert (chair >= 0 && chair < k_MaxChairs);
	std::bitset<16> lok_play(get_playersplayingbits());

	return lok_play.test(chair);
}


bool CGlobalVars::is_seated(int chair, int seatedbits)
{
	/*
	* returns if player is playing at specified chair
	*/

	assert (chair >= 0 && chair < k_MaxChairs);
	std::bitset<16> lok_play(seatedbits);

	return lok_play.test(chair);
}


//////////////////////////////////////
////// OWN FUNCTIONS
//////////////////////////////////////
bool CGlobalVars::isButtonAvailable(int button)
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

	if(button >= 0 && button < 5) 
	{
		std::bitset<16> lok_play(get_myturnbits());

		return lok_play.test(button);
	}

	return false;
}


void CGlobalVars::RecordBotsDecision(double action)
{
	/*
	* Record the bot decision
	*/

	int br_ndx = br-1;

	if(m_CurrentHandInfo.RecordedBotsAction)
		return;

	if(action > call_value)
	{
		//Raise
		m_CurrentHandInfo.RecordPlayerAction(br_ndx, userchair, actionBetRaise, action, CurState().m_player[userchair].m_balance);
		m_CurrentHandInfo.m_AmountToCall[br_ndx] = action;
	}

	else if(IsEqual(action, call_value))
	{
		//Check
		if(IsEqual(CurState().m_player[userchair].m_currentbet, m_CurrentHandInfo.m_AmountToCall[br_ndx]))
			m_CurrentHandInfo.RecordPlayerAction(br_ndx, userchair, actionCheck, action, CurState().m_player[userchair].m_balance);

		//Call
		else 
			m_CurrentHandInfo.RecordPlayerAction(br_ndx, userchair, actionCall, m_CurrentHandInfo.m_AmountToCall[br_ndx], CurState().m_player[userchair].m_balance);
	}

	else
	{
		//Check
		if(IsEqual(CurState().m_player[userchair].m_currentbet, m_CurrentHandInfo.m_AmountToCall[br_ndx]))
		{
			m_CurrentHandInfo.RecordPlayerAction(br_ndx, userchair, actionCheck, action, CurState().m_player[userchair].m_balance);
		}

		//Fold
		// Who cares if we fold ? Ignore
		else
		{
			//m_CurrentHandInfo.RecordPlayerAction(get_betround(), userchair, actionFold, action);
		}
	}
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

	for (int br_ndx = 0; br_ndx < br; br_ndx++)
	{
		std::stringstream round_string, amt_call, times_acted;

		round_string << br;
		amt_call << m_CurrentHandInfo.m_AmountToCall[br_ndx];
		times_acted << TimesActed[br_ndx];

		debug_msg += ("\nBR " + round_string.str() + " Call_Amt : " + amt_call.str() + " Times Acted : " + times_acted.str() + "\n\n");

		for (int i = 0; i < k_MaxChairs; i++)
		{
			int iPlayer = (get_dealerchair() + (i+1)) % k_MaxChairs;

			if (is_seated(iPlayer, playersseatedbits))
			{
				std::stringstream num_player, prev_bet, tot_actions, act_string, left_to_act;
				int total_actions = m_CurrentHandInfo.m_PlayerActions[iPlayer].total_actions[br_ndx];

				num_player << iPlayer;
				tot_actions << std::left << std::setw(4) << total_actions;

				//left_to_act << left << setw(4) << LeftToAct(iPlayer);
				left_to_act << std::left << std::setw(4) << m_CurrentHandInfo.m_PlayerActions[iPlayer].actions[br_ndx][total_actions-1].left_to_act;
				act_string << std::left << std::setw(12) << m_CurrentHandInfo.m_PlayerActions[iPlayer].actions_this_round[br_ndx];
				prev_bet << std::setw(8) << m_CurrentHandInfo.m_PlayerActions[iPlayer].prevBet[br_ndx];		

				debug_msg += ("Player" + num_player.str() + "  Left To Act : " + left_to_act.str() + " Tot. Actions : " + tot_actions.str() + " - " +  act_string.str() + prev_bet.str() + "\n");
			}
		}
	}

	MessageBox(NULL, debug_msg.c_str(), "Player Actions Debug", MB_OK | MB_ICONASTERISK);
}

