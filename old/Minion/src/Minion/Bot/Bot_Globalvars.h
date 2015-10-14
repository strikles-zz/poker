#pragma once

#if (_MSC_VER >= 1600)
	#include <random>
#else
	#include "Util/Util_mtrand.h"
#endif

#include "Bot/Bot_General.h"
#include "Bot/Bot_DLL.h"
#include "Bot/Bot_State.h"
#include "Bot/Bot_PlayerState.h"
#include "Bot/Bot_HandHistory.h"

#include "Util/Util.h"
#include "Util/Util_Lut.h"
#include "Util/Util_logging.h"


class CGlobalVars
{
public:

	CGlobalVars();
	~CGlobalVars();

	// Rule of 3 :
	// Disabling Copy Constructor & assignement operator
	CGlobalVars(const CGlobalVars&);
	CGlobalVars& operator=(const CGlobalVars&);

public:

	///////////////////////////////////////////////////
	//OH DLL VARS
	///////////////////////////////////////////////////

	//SYMBOLS VAR
	pfgws_t m_pget_winholdem_symbol;

	///////////////////////////////////////////////////
	//OWN VARS
	///////////////////////////////////////////////////

	//Userchair
	int userchair;
	int enabled_flag[k_max_chairs];

	int br;
	int ntimesacted;

	//GWS VAR
	bool	iserr;

	//GENERAL VARS
	bool	sb_posted;
	int		action_type;
	bool	israis;

	CState	m_state;
	std::string board_cards;

	//MONEY VARS
	double call;
	double sb;
	double bb;
	double ncurrentbet; //maximum currentbet on the table
	double currentbet[k_max_chairs];
	double balance[k_max_chairs];
	double initial_balance[k_max_chairs];

	//OPPONENT VARS
	int nopponents;
	int nopponentsbetting;
	int nopponentscalling; //how many opponents called a raise
	int raischair;
	int adj_nopponentsraising;
	int nplayersplaying;

	//PREFLOP VARS
	int preflop_position;

	//Strategy 
	bool preflop_info_hu;
	bool preflop_info_first_to_act;
	bool preflop_info_single_limper;
	bool preflop_info_multiple_limpers;
	bool preflop_info_single_raiser_no_caller;
	bool preflop_info_single_raiser_single_caller;
	bool preflop_info_single_raiser_multiple_caller;
	bool preflop_info_multiple_raisers;

	int pf_entered;
	int dealpositionrais;
	int no_limpers;
	double rais_chair_bet_amount;

	//these variables still need to be defined:
	int limpReraiser;
	int threeBetters;
	int rais_chair_position;
	int isolatedLimper;

	//SUIT VARS
	int flushrank;

	//HAND VARS
	char myHand[3];
	bool issuited;
	bool ispair;
	int Bot_Common_Cards[7][2];		//rank 2 ; ... ; 14=A suit 1=C ; 2=D ; 3=H ; 4=S
	int rankhiplayer;
	int rankloplayer;
	StdDeck_CardMask curBoardMask;

	//OWN EQUITY CALCULATION VARIABLES
	double	outResults[k_max_chairs];
	double	prw_xpoker_equity;

	// OPP MODELING
	int		Rank_Reorganization[3][1326]; // Flop, Turn, River
	short	HandLikelihood[k_max_chairs][1326];

	CRank169To1326Utility util1326;


	////////////////////////////
	//Poker Tracker Variables
	////////////////////////////
	/*
	CPlayerStatistics m_PlayerStatistics;
	//POKER TRACKER
	double orig_rais_chair_pfr;
	double rais_chair_pfr;
	double rais_chair_vpip;
	double rais_chair_hands;
	double rais_chair_ats;
	//POSTGRES DB VARS
	int max_hand_id;
	*/

	////////////////////////////
	// OpenHoldem Symbols
	////////////////////////////

	OH_VAR(bblind);
	OH_VAR(pot);
	OH_VAR(potplayer);
	OH_VAR(prwin);
	OH_VAR(sblind);

	OH_VAR_INT(betround);
	OH_VAR_INT(dealerchair);
	OH_VAR_INT(dealposition)
	OH_VAR_INT(didcall);
	OH_VAR_INT(didchec);
	OH_VAR_INT(didrais);
	OH_VAR_INT(didswag);
	OH_VAR_INT(elapsed);
	OH_VAR_INT(elapsedauto);
	OH_VAR_INT(f0);
	OH_VAR_INT(f1);
	OH_VAR_INT(handrank169);
	OH_VAR_INT(handsplayed);
	OH_VAR_INT(ismyturn);
	OH_VAR_INT(issittingin);
	OH_VAR_INT(issittingout);
	OH_VAR_INT(istournament);
	OH_VAR_INT(myturnbits);
	OH_VAR_INT(ncallbets);
	OH_VAR_INT(nchairs);
	OH_VAR_INT(nchairsdealtright);
	OH_VAR_INT(nopponentsdealt);
	OH_VAR_INT(nopponentsraising);
	OH_VAR_INT(nplayersdealt);
	OH_VAR_INT(nsuitedcommon);
	OH_VAR_INT(swagtextmethod);

	OH_VAR_USIGNED_INT(playersdealtbits);
	OH_VAR_USIGNED_INT(playersplayingbits);
	OH_VAR_USIGNED_INT(playersseatedbits);

	OH_SEAT_VAR(ac_dealpos);

public:

	void	init_userchair();
	void	calc_oh_symbols();

	bool	is_playing(unsigned int chair);
	bool	is_seated(unsigned int chair);
	bool	InBits(int chair, unsigned int playing_bits);
	bool	isActionAvailable(int action);

	void	RecordBotsDecision(ePlayerAction action, eBetType betType, double amount);
	void	RecordNewGame();

	double	StackCommitment(int iPlayer);
	double	ReductionFactor(int iPlayer);

	int		GetTimesActed();
	void	display_actions();

	bool	rainbow_board();
	bool	paired_board();
	bool	dry_board();

	///////////////////
	//    PREFLOP    //
	///////////////////

	bool is_headsup();

	//Preflop Info Functions
	bool	info_first_to_act();
	bool	info_single_limper();
	bool	info_multiple_limpers();
	bool	info_single_raiser_no_caller();
	bool	info_single_raiser_single_caller();
	bool	info_single_raiser_multiple_caller();
	bool	info_multiple_raisers();
	int		info_nplayers_bet_less_then_current_raise();
	int		info_nplayers_with_raise_amount();
	int		pf_raiser_position();
	//Preflop Action Before Functions
	int		number_of_reraises();
	int		other_opp_call_reraise();
	double	reraiser_opp_currentbet();
	double	reraiser_opp_balance();
	bool	reraiser_opp_is_allin();

	bool	info_sb_limped();
	int		first_limper();
	int		first_limper_position();
	double	first_limper_vpip();
	double	first_limper_pfr();
	double	first_limper_hands();
	int		facing_0bet();
	int		facing_1bet();
	int		facing_2bet_plus();
	int		facing_2bet_response();
	int		facing_3bet_response();
	int		X_bet();
	double	info_fold_to_3bet();
	bool	IP();

	// position
	int		set_preflop_position();	
	bool	set_SB();
	int		sb_chair();
	int		bb_chair();
	int		get_acdeal(int chair);
	int		latest_playing_player();
	int		previous_player(int dp);
	int		opponent_position(int opponent_chair);
	// bets
	double	set_maxcurrentbet();
	int		set_nopponentsbetting();
	int		set_nopponentscalling();
	double	check_currentbet(int chair);
	double	raiser_balance();
	// rais
	int		pf_pos_rais();
	int		set_raischair();
	int		set_dealpositionrais();
	double	set_rais_chair_pfr();
	double	set_rais_chair_vpip();
	double	set_rais_chair_hands();
	double	set_rais_chair_ats();

	// generic
	int		set_nopponentsraising();
	int		set_adj_nopponentsraising();
	double	adj_pfr();
	// cards
	void	set_card_values();
	int		set_rankhiplayer();
	int		set_rankloplayer();
	bool	set_suited();
	bool	isHand(const char* hands);
	char*	getMyHand();


	void CriticalOHVars()
	{
		dirty_ismyturn();

		// ???
		dirty_betround();		
		dirty_dealposition();
		dirty_handsplayed();
		dirty_playersdealtbits();
		dirty_playersplayingbits();
		dirty_playersseatedbits();
		dirty_dealerchair();

		dirty_bblind();
		dirty_sblind();
	}
	void CommonOHVars() 
	{	
		dirty_didcall();
		dirty_didchec();
		dirty_didrais();
		dirty_didswag();
		dirty_elapsed();
		dirty_elapsedauto();
		dirty_f0();
		dirty_f1();
		dirty_handrank169();		
		dirty_issittingin();
		dirty_issittingout();
		dirty_istournament();
		dirty_myturnbits();
		dirty_ncallbets();
		dirty_nchairs();
		dirty_nchairsdealtright();
		dirty_nopponentsdealt();
		dirty_nopponentsraising();
		dirty_nplayersdealt();
		dirty_nsuitedcommon();
		dirty_swagtextmethod();

		dirty_pot();
		dirty_potplayer();
		dirty_prwin();

		// Hmmm...
		for(int i =0 ; i < k_max_chairs; i++) 
		{
			dirty_ac_dealpos(i);
		}
	}
	void DirtyOHVars()
	{
		CriticalOHVars();
		CommonOHVars();
	}

};

extern CGlobalVars gVars;