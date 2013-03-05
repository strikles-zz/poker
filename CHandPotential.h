#pragma once

#include "poker_defs.h"

const int ahead		= 0;
const int tied		= 1;
const int behind	= 2;


class CHandPotential
{

public:

	CHandPotential(const StdDeck_CardMask& our_cards, const StdDeck_CardMask& board_cards);

public:

	void CalcPotentials();
	void EvalAllOppCards(const StdDeck_CardMask& opp_cards);
	void EvalOneTrial(int index, const StdDeck_CardMask& opp_cards, const StdDeck_CardMask& board_cards_to_come);

public:

	StdDeck_CardMask hero_cards;
	StdDeck_CardMask board_cards;
	StdDeck_CardMask hero_mask;
	HandVal hero_rank;

	//Hand potential array, each index represents ahead, tied, and behind.
	double HP[3][3];	 /* initialize to 0	*/
	double HPTotal[3];	/* initialize to 0 */

	int num_board_cards;
	int num_board_cards_to_come;

	double Ppot;
	double Npot;
};