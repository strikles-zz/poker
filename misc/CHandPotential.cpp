
#include <cassert>
#include <ctime>

#include "Calc/CHandPotential.h"
#include "Util/Util_logging.h"
#include "Util/Util_MagicNumbers.h"

#include "inlines/eval.h"


CHandPotential::CHandPotential(const StdDeck_CardMask& hc, const StdDeck_CardMask& bc)
{
	Ppot = 0.0;
	Npot = 0.0;

	memset(&HP[0][0], 0, 3*3*sizeof(HP[0][0]));
	memset(&HPTotal[0], 0, 3*sizeof(HPTotal[0]));

	//gLog.WriteLog(eSeverityDebug, eCatHandPotential, "Player Mask: %s\n", Deck_maskString(our_cards));
	//gLog.WriteLog(eSeverityDebug, eCatHandPotential, "Board Mask: %s\n", Deck_maskString(board_cards));

	num_board_cards = StdDeck_numCards(bc);
	num_board_cards_to_come = 5 - num_board_cards;

	StdDeck_CardMask_OR(hero_cards, hc, hc);
	StdDeck_CardMask_OR(board_cards, bc, bc);
	StdDeck_CardMask_OR(hero_mask, hc, bc);
	hero_rank = StdDeck_StdRules_EVAL_N(hero_mask, 7);
}


void CHandPotential::CalcPotentials()
{	
	clock_t timer = clock();

	// Consider all two card combinations of the remaining cards for the opponent.
	StdDeck_CardMask opp_cards = {0};
	DECK_ENUMERATE_2_CARDS_D(StdDeck, opp_cards, hero_mask, this->EvalAllOppCards(opp_cards););

	// Ppot: were behind but moved ahead
	double Ppot_denominator = HPTotal[behind] + HPTotal[tied];
	Ppot = (Ppot_denominator > 0 ? (HP[behind][ahead] + HP[behind][tied]*0.5 + HP[tied][ahead]*0.5) / Ppot_denominator : 0.0);

	// Npot: were ahead but fell behind
	double Npot_denominator = HPTotal[ahead] + HPTotal[tied];
	Npot = (Npot_denominator > 0 ? (HP[ahead][behind] + HP[tied][behind]*0.5 + HP[ahead][tied]*0.5) / Npot_denominator : 0.0);

	timer = clock() - timer;  // end the timer
	gLog.WriteLog(eSeverityInfo, eCatPerformance, "HP time (%ld ms)\n", timer);
	
	if(Ppot > Npot)
	{
		gLog.WriteLog(eSeverityInfo, eCatPositive, "PPot: %.3f\n", Ppot);
		gLog.WriteLog(eSeverityInfo, eCatHandPotential, "NPot: %.3f\n", Npot);
	}
	else
	{
		gLog.WriteLog(eSeverityInfo, eCatHandPotential, "PPot: %.3f\n", Ppot);
		gLog.WriteLog(eSeverityInfo, eCatNegative, "NPot: %.3f\n", Npot);
	}
}


void CHandPotential::EvalAllOppCards(const StdDeck_CardMask& opp_cards)
{
	StdDeck_CardMask opp_mask = {0};
	StdDeck_CardMask_OR(opp_mask, opp_cards, board_cards);
	HandVal opp_rank = StdDeck_StdRules_EVAL_N(opp_mask, 7);

	int index = -1;
	if(hero_rank > opp_rank) 
		index = ahead;
	else if(hero_rank == opp_rank) 
		index = tied;
	else
		index = behind;

	StdDeck_CardMask dead_mask = {0}, board_cards_to_come = {0};
	StdDeck_CardMask_OR(dead_mask, hero_mask, opp_cards);
	switch(num_board_cards_to_come)
	{
		case 2:
			DECK_ENUMERATE_2_CARDS_D(StdDeck, board_cards_to_come, dead_mask, this->EvalOneTrial(index, opp_cards, board_cards_to_come););
			break;
		case 1:
			DECK_ENUMERATE_1_CARDS_D(StdDeck, board_cards_to_come, dead_mask, this->EvalOneTrial(index, opp_cards, board_cards_to_come););
			break;
		default:
			break;
	}
}


void CHandPotential::EvalOneTrial(int index, const StdDeck_CardMask& opp_cards, const StdDeck_CardMask& board_cards_to_come)
{
	// Final 5-card board
	StdDeck_CardMask final_board_mask = {0}, our_mask = {0}, opp_mask = {0};
	StdDeck_CardMask_OR(final_board_mask, board_cards, board_cards_to_come);
	StdDeck_CardMask_OR(our_mask, hero_cards, final_board_mask);
	StdDeck_CardMask_OR(opp_mask, opp_cards, final_board_mask);
	HandVal our_best_current = StdDeck_StdRules_EVAL_N(our_mask, 7);
	HandVal opp_best_current = StdDeck_StdRules_EVAL_N(opp_mask, 7);
	HPTotal[index] += 1.0;

	if(our_best_current > opp_best_current) 
		HP[index][ahead] += 1.0;
	else if(our_best_current == opp_best_current) 
		HP[index][tied] += 1.0;
	else
		HP[index][behind] += 1.0;
}