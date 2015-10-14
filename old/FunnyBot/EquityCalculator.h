///////////////////////////////////////
// Lab 42									 //
//	blablabla								 //
// (Derived from James Devlin Stuff) //
///////////////////////////////////////

#pragma once

#include "HandDistribution.h"
#include "Poker.PokerEval\include\poker_defs.h"

#include "Util_MagicNumbers.h"

class HandDistribution;

class EquityCalculator
{
public:

	EquityCalculator();

private:

	StdDeck_CardMask m_boardMask;
	StdDeck_CardMask m_deadMask;
	StdDeck_CardMask m_deadMaskDyn;
	StdDeck_CardMask m_heroMask;

	int m_numberOfBoardCards;
	int m_numberOfRangedHands;
	int m_numberOfSpecificHands;

	int m_totalHands;
	int m_collisions;
	double* m_pResults;

	unsigned int m_indicatedTrials;
	unsigned int m_actualTrials;
	unsigned int m_possibleOutcomes;
	unsigned int m_MonteCarloThreshhold;

	std::vector<HandDistribution*> m_dists;
	int m_totalPlayers;
	double m_wins[k_MaxChairs];
	HandVal m_handVals[k_MaxChairs];
	int m_tiedPlayerIndexes[k_MaxChairs];
	bool m_wasMonteCarlo;

public:

	unsigned int CalculateEquity
	(
		const unsigned int trialCount, 
		StdDeck_CardMask const& heroMask, 
		StdDeck_CardMask const& boardMask, 
		double* outResults, 
		short* likelihood1326_0,
		short* likelihood1326_1, 
		short* likelihood1326_2, 
		short* likelihood1326_3, 
		short* likelihood1326_4, 
		short* likelihood1326_5, 
		short* likelihood1326_6, 
		short* likelihood1326_7, 
		short* likelihood1326_8, 
		short* likelihood1326_9 

	);

private:

	void			Reset1326();
	unsigned int	CalculateMonteCarlo1326();
	unsigned int	Calculate_HU_MonteCarlo1326();
	void			EvalOneTrial1326(StdDeck_CardMask boardFragment);
	unsigned int	PostCalculate1326();
};
