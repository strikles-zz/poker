#pragma once

class CPlayerInfo
{
public:

	CPlayerInfo() 
	{
		name[0] = '\0';
		nReadsDeferred = 0;
	}

public:

	/*
	TODO: stats needed to be added:
	-3b%  This is how often your opponent re-raises before the flop. 
	-F3   This is how often your opponent folds to three-bets.
	-cb   How often your opponent continuation bets on the flop as the pre-flop raiser.
	-2b   This is how often your opponent second barrels as the pre-flop raiser when his flop c-bet is called.
	-fc   How often your opponent folds to flop continuation bets.
	-f2   How often your opponent folds to a second barrel.
	*/

	/////////////////////////////
	// General Values
	/////////////////////////////

	char name[16];
	int nReadsDeferred;

	/////////////////////////////
	// Ring Symbols
	/////////////////////////////

	//Poker Tracker auto-rate icon code for chair x
	//double pt_icon;
	//Poker Tracker pre-flop raise percentage for chair x 
	double pt_pfr;
	//Poker Tracker total aggression for chair x
	//double pt_aggtot;
	//Poker Tracker total aggression excluding preflop for chair x
	//double pt_aggtotnopf;
	//Poker Tracker preflop aggression for chair x
	//double pt_aggp;
	//Poker Tracker flop aggression for chair x
	double pt_aggf;
	//Poker Tracker turn aggression for chair x
	double pt_aggt;
	//Poker Tracker river aggression for chair x
	//FIX? aggr or aggrv
	//double pt_aggr;
	double pt_aggr;
	//Poker Tracker saw flop pct for chair x
	double pt_floppct;
	//Poker Tracker saw turn pct for chair x
	double pt_turnpct;
	//Poker Tracker saw river pct for chair x
	double pt_riverpct;
	//Poker Tracker VP$IP for chair x
	double pt_vpip;
	//Poker Tracker number of hands that are in the database for chair x
	double pt_hands;
	//Poker Tracker "pre-flop raise first in" pct for chair x
	double pt_pf_rfi;
	//Poker Tracker "pre-flop called raise" pct for chair x
	double pt_pf_cr;
	//Poker Tracker attempt to steal blinds for chair x
	double pt_pfats;
	//Poker Tracker went to showdown percentage for chair x
	double pt_wsdp;
	//Poker Tracker won $ at showdown for chair x
	double pt_wssd;
	//Poker Tracker folded small blind to steal for chair x
	double pt_fsbts;
	//Poker Tracker folded big blind to steal for chair x
	double pt_fbbts;

	// AggQ
	double pt_aggfpct;
	double pt_aggtpct;
	double pt_aggrpct;
	// C-Bets
	double pt_f_cbet;
	double pt_t_cbet;
	double pt_r_cbet;
	double pt_f_fold_cbet;
	double pt_t_fold_cbet;
	double pt_r_fold_cbet;
	// 3-Bets
	double pt_p_3bet;
	double pt_f_3bet;
	double pt_t_3bet;
	double pt_r_3bet;
	double pt_p_fold_3bet;
	double pt_f_fold_3bet;
	double pt_t_fold_3bet;
	double pt_r_fold_3bet;
	// 4-Bets
	double pt_p_4bet;
	double pt_f_4bet;
	double pt_t_4bet;
	double pt_r_4bet;
	double pt_p_fold_4bet;
	double pt_f_fold_4bet;
	double pt_t_fold_4bet;
	double pt_r_fold_4bet;

	/////////////////////////////
	// Future action stochasts
	/////////////////////////////
	double pr_rais;
	double pr_call;
	double pr_check;
	double pr_fold;

	double call_pr_rais;
	double call_pr_call;
	double call_pr_check;
	double call_pr_fold;

	double rais_pr_rais;
	double rais_pr_call;
	double rais_pr_check;
	double rais_pr_fold;
};