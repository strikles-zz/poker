#pragma once

#include <random>

#include "Bot_General.h"
#include "Bot_Util.h"

#include "Bot_DLL.h"

#include "Bot_PlayerState.h"
#include "Bot_HandHistory.h"

#include "Bot_PTStats.h"

#include "Util_Lut.h"
#include "Util_logging.h"


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

	//CONSECUTIVE STATES
	holdem_state    m_holdem_state[256]; 
	unsigned char   m_ndx;

	//SYMBOLS VAR
	pfgws_t m_pget_winholdem_symbol;

	///////////////////////////////////////////////////
	//OWN VARS
	///////////////////////////////////////////////////

	std::mt19937 t_rand;
	std::random_device  rnd_device;

	//Userchair
	int br;
	int userchair;
	int maxchairs;
	int enabled_flag[k_MaxChairs];

	int playersseatedbits;

	int didswag;
	int didrais;
	int didcall;
	int didchec;
	int ntimesacted;

	//GWS VAR
	bool iserr;

	//GENERAL VARS
	bool SB;
	double decision;
	int action_type;
	bool israis;

	//trigger indicating round has changed
	int round_trigger;

	//OWN PRWIN CALCULATION VARIABLES
	double outResults[k_MaxChairs];
	double prw_xpoker_equity;
	std::string board_cards;

	// OPP MODELING
	CLutFlop *p_flop;
	CLutTurn *p_turn;
	CLutRiver *p_river;
	int Rank_Reorganization[3][1326]; // Flop, Turn, River
	short HandLikelihood[k_MaxChairs][1326];

	//BOARD TEXTURE VARIABLES
	double SUMprwin;
	double avg_prwin;
	double SUMvar_prwin;
	double var_prwin;

	//MONEY VARS
	double call;
	double currentbet;
	double currentbets[k_MaxChairs];
	double ncurrentbet; //maximum currentbet on the table
	double balance;
	double initial_balance[k_MaxChairs];

	//OPPONENT VARS
	int nopponents;
	int nopponentsbetting;
	int nopponentscalling; //how many opponents called a raise
	int raischair;
	int nopponentsraising;
	int adj_nopponentsraising;
	int nopponentsplaying;

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
	double rais_chair_bet_amount;
	int no_limpers;
	//these variables still need to be defined:
	int limpReraiser;
	int threeBetters;
	int rais_chair_position;
	int isolatedLimper;

	//SUIT VARS
	int flushrank;

	//HAND VARS
	bool issuited;
	int Bot_Common_Cards[7][2];		//rank 2 ; ... ; 14=A suit 1=C ; 2=D ; 3=H ; 4=S
	StdDeck_CardMask curBoardMask;
	int rankhiplayer;
	int rankloplayer;
	bool ispair;
	char myHand[3];

	int TimesActed[eRounds];
	unsigned int InGameBits[eRounds][k_MaxActionsPerRound];
	unsigned int FoldedBits[eRounds][k_MaxActionsPerRound];
	CHandHistory m_CurrentHandInfo;

	// State Variables
	bool m_bNewGame;
	bool m_bNewRound;
	int  m_CurActor;

	bool	is_heads_up;
	bool hero_in_blinds;

	int m_PrevBetround;
	int m_PrevHandsPlayed;

	CRank169To1326Utility util1326;

	////////////////////////////
	//Poker Tracker Variables
	////////////////////////////
	CPlayerStatistics m_PlayerStatistics;

	//POKER TRACKER
	double orig_rais_chair_pfr;
	double rais_chair_pfr;
	double rais_chair_vpip;
	double rais_chair_hands;
	double rais_chair_ats;

	////////////////////////////
	// OpenHoldem Symbols
	////////////////////////////
	OH_VAR_INT(nchairs);
	OH_VAR_INT(swagtextmethod);
	OH_VAR(bblind);
	OH_VAR(sblind);
	OH_VAR_INT(istournament);

	OH_VAR_INT(handrank169);
	OH_VAR_INT(dealerchair);
	OH_VAR_INT(betround);
	OH_VAR_INT(dealposition);

	OH_VAR(pot);
	OH_VAR(potplayer);
	OH_VAR_INT(ncallbets);
	OH_VAR_INT(nplayersdealt);
	OH_VAR_INT(nopponentsdealt);
	OH_VAR_INT(nchairsdealtright);

	OH_VAR_INT(playersseatedbits);
	OH_VAR_INT(playersdealtbits);
	OH_VAR_INT(playersplayingbits);
	OH_VAR_INT(opponentsplayingbits);

	OH_VAR_INT(f0);
	OH_VAR_INT(nsuitedcommon);
	OH_VAR_INT(elapsed);
	OH_VAR_INT(elapsedauto);

	OH_VAR_INT(myturnbits);
	OH_VAR_INT(ismyturn);
	OH_VAR_INT(issittingin);
	OH_VAR_INT(issittingout);

	OH_VAR_INT(raisbits1);
	OH_VAR_INT(callbits1);
	OH_VAR_INT(handsplayed);

	OH_SEAT_VAR(ac_dealpos);

public:

	void init_oh_symbols();
	void calc_oh_symbols();

	bool is_playing(int chair);
	bool is_seated(int chair, int seatedbits);
	bool isButtonAvailable(int button);
	void display_actions();


	bool InGame(const holdem_player& player);
	int  PlayersInGame();

	// Holdem State processing functions	
	bool CardsDealt(const holdem_state& curState);
	bool IsStateUsable(const holdem_state& a);
	bool IsStateDifferent(const holdem_state& a, const holdem_state& b, bool bConsiderName);
	bool WillProcessState(const holdem_state& pstate);
	void DumpHoldemState(const holdem_state& curState, int index);
	void ExtractState(const holdem_state& curState);

	void ExtractActionsNewGame(enumStreets BetroundByCards, bool is_heads_up, bool& hero_in_blinds);
	void ExtractActionsNewRound(enumStreets BetroundByCards);

	int FindPreviousActor(const int chair);
	int FindFirstActor(const int betting_round);
	int FindNextActor(const int chair);

	void SetCurrentActor(enumStreets BetroundByCards, const bool is_heads_up, const bool hero_in_blinds);
	void ExtractActions(enumStreets BetroundByCards);
	void ExtractContext(enumStreets BetroundByCards);

	void RecordBotsDecision(double action);
	void RecordNewGame();

	double LeftToInvest(int iPlayer);
	double ReductionFactor(int iPlayer);
	int LeftToAct(int BetroundByCards, int chair);

	int VerifyLut(const int lut_ndx);
	int LoadLuts();


	bool rainbow_board();
	bool paired_board();
	bool dry_board();

	//inline holdem_state& PrevState() { return (m_ndx > 0 ? m_holdem_state[m_ndx-1] : m_holdem_state[m_ndx]; }
	inline holdem_state& CurState() { return m_holdem_state[m_ndx]; }

	StdDeck_CardMask Hand1326ToCardMask(int index) { return  util1326.cardMaskFor1326Hand[index]; }

	void DirtyOHVars() 
	{
		dirty_f0();
		dirty_nchairs();
		dirty_swagtextmethod();
		dirty_bblind();
		dirty_sblind();
		dirty_istournament();
		dirty_handrank169();
		dirty_dealerchair();
		dirty_betround();
		dirty_dealposition();
		dirty_pot();
		dirty_potplayer();
		dirty_ncallbets();
		dirty_nplayersdealt();
		dirty_nopponentsdealt();
		dirty_nchairsdealtright();
		dirty_playersseatedbits();
		dirty_playersdealtbits();
		dirty_playersplayingbits();
		dirty_opponentsplayingbits();
		dirty_nsuitedcommon();
		dirty_elapsed();
		dirty_elapsedauto();
		dirty_myturnbits();
		dirty_ismyturn();
		dirty_issittingin();
		dirty_issittingout();
		dirty_raisbits1();
		dirty_callbits1();
		dirty_handsplayed();

		// Hmmm...
		for (int i =0 ; i < k_MaxChairs; i++) 
		{
			dirty_ac_dealpos(i);
		}
	}

	///////////////////
	//    PREFLOP    //
	///////////////////

	bool is_headsup();

	//Preflop Info Functions
	bool info_first_to_act();
	bool info_single_limper();
	bool info_multiple_limpers();
	bool info_single_raiser_no_caller();
	bool info_single_raiser_single_caller();
	bool info_single_raiser_multiple_caller();
	bool info_multiple_raisers();

	int info_nplayers_bet_less_then_current_raise();
	int info_nplayers_with_raise_amount();
	double raiser_balance();
	int pf_raiser_position();

	void set_card_values();
	int set_rankhiplayer();
	int set_rankloplayer();
	bool set_suited();

	bool set_SB();
	bool isHand(const char* hands);
	char* getMyHand();

	//Preflop Functions
	int set_preflop_position();	
	int pf_pos_rais();

	double set_ncurrentbet();
	int set_nopponentsbetting();
	int set_nopponentscalling();
	int sb_chair();
	int bb_chair();

	int get_acdeal(int chair);
	int latest_playing_player();
	int previous_player(int dp);
	double check_currentbet(int chair);

	int set_raischair();
	int set_dealpositionrais();
	double set_rais_chair_pfr();
	double set_rais_chair_vpip();
	double set_rais_chair_hands();
	double set_rais_chair_ats();

	int set_nopponentsraising();
	int set_adj_nopponentsraising();
	double adj_pfr();

	int opponent_position(int opponent_chair);

	int first_limper();
	int first_limper_position();
	double first_limper_vpip();
	double first_limper_pfr();
	double first_limper_hands();
	int facing_0bet();
	int facing_1bet();
	int facing_2bet_plus();
	int facing_2bet_response();
	int facing_3bet_response();
	int X_bet();
	double info_fold_to_3bet();
	bool IP();

	//Preflop Action Before Functions
	int number_of_reraises();
	int other_opp_call_reraise();
	double reraiser_opp_currentbet();
	double reraiser_opp_balance();
	bool reraiser_opp_is_allin();

	bool info_sb_limped();

};

//extern CGlobalVars* gVars;
extern CGlobalVars gVars;