///////////////////////////////////////
// Lab 42									 //
//	blablabla								 //
// (Derived from James Devlin Stuff) //
///////////////////////////////////////

#include "stdafx.h"
#include "EquityCalculator.h"
#include "HandDistribution.h"

#include "Bot_Globalvars.h"

#include "Poker.PokerEval\include\poker_defs.h"
#include "Poker.PokerEval\include\rules_std.h"
#include "Poker.PokerEval\include\deck_std.h"
#include "Poker.PokerEval\include\inlines\eval.h"

#include "Util_logging.h"


///////////////////////////////////////////////////////////////////////////////
// Default constructor for EquityCalculator objects.
///////////////////////////////////////////////////////////////////////////////
EquityCalculator::EquityCalculator()
{
	Reset1326();
}


///////////////////////////////////////////////////////////////////////////////
// Strikles Reset 1326
///////////////////////////////////////////////////////////////////////////////
void EquityCalculator::Reset1326()
{
	StdDeck_CardMask_RESET(m_heroMask);
	StdDeck_CardMask_RESET(m_boardMask);
	StdDeck_CardMask_RESET(m_deadMask);

	m_MonteCarloThreshhold = UINT_MAX;
	m_numberOfBoardCards = 0;
	m_collisions = 0;
	m_totalPlayers = 0;

	m_indicatedTrials = 0;
	m_actualTrials = 0;

	m_pResults = NULL;

	memset(&m_wins[0], 0, k_MaxChairs*sizeof(double));
	memset(&m_tiedPlayerIndexes[0], 0, k_MaxChairs*sizeof(int));
	memset(&m_handVals[0], HandVal_NOTHING, k_MaxChairs*sizeof(HandVal));
}

////////////////////////////////////////
// Main fnc
////////////////////////////////////////
unsigned int EquityCalculator::CalculateEquity
(
	const unsigned int numberOfTrials, 
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
)
{
	//_cprintf("Starting CalculateWithLikelihood1326()\n");

	Reset1326();

	m_boardMask = boardMask;
	m_heroMask = heroMask;
	StdDeck_CardMask_OR(m_deadMask, m_heroMask, m_boardMask);

	// Get the number of board cards that were supplied. For Hold'em, should be 0, 3, 4 or 5.
	m_numberOfBoardCards = StdDeck_numCards(m_boardMask);
	m_indicatedTrials = numberOfTrials;

	// Tuck this away for later
	m_pResults = outResults;

	m_totalPlayers = k_MaxChairs;
	m_dists.resize(m_totalPlayers);

	short* likelihood1326[k_MaxChairs] = { NULL };

	if (likelihood1326_0)
		likelihood1326[0] = likelihood1326_0;
	if (likelihood1326_1)
		likelihood1326[1] = likelihood1326_1;
	if (likelihood1326_2)
		likelihood1326[2] = likelihood1326_2;
	if (likelihood1326_3)
		likelihood1326[3] = likelihood1326_3;
	if (likelihood1326_4)
		likelihood1326[4] = likelihood1326_4;
	if (likelihood1326_5)
		likelihood1326[5] = likelihood1326_5;
	if (likelihood1326_6)
		likelihood1326[6] = likelihood1326_6;
	if (likelihood1326_7)
		likelihood1326[7] = likelihood1326_7;
	if (likelihood1326_8)
		likelihood1326[8] = likelihood1326_8;
	if (likelihood1326_9)
		likelihood1326[9] = likelihood1326_9;

	for (int index = 0; index < m_totalPlayers; index++)
	{
		HandDistribution* pCur = NULL;

		if (index == gVars.userchair) 
			pCur = new HandDistribution(m_heroMask, m_boardMask);
		else if (likelihood1326[index])
			pCur = new HandDistribution(likelihood1326[index], m_deadMask);
		else
			pCur = new HandDistribution();

		m_dists[index] = pCur;

		if (pCur->NotEmpty())
			gLog->WriteLog(eSeverityInfo, eCatCalculator, "Player%d Sum : %d\n", index, pCur->m_sum);
	}

	CalculateMonteCarlo1326();

	return PostCalculate1326();
}

///////////////////////////////////////////////////////////////////////////////
// MC 1326
///////////////////////////////////////////////////////////////////////////////
unsigned int EquityCalculator::CalculateMonteCarlo1326()
{
	//_cprintf("Starting CalculateMonteCarlo1326()\n");

	m_wasMonteCarlo = true;

	StdDeck_CardMask cardCombo;
	StdDeck_CardMask usedCardsThisTrial;

	// Now run however many trials
	while (m_actualTrials < m_indicatedTrials)
	{
		// Reset "used cards"
		StdDeck_CardMask_RESET(cardCombo);
		StdDeck_CardMask_RESET(usedCardsThisTrial);
		StdDeck_CardMask_OR(usedCardsThisTrial, usedCardsThisTrial, m_boardMask);

		bool bCollisionError = false;
		
		// For each player in the hand, choose his hand. If the player has a specific hand,
		// great, use that one. Otherwise pick one hand from his distribution randomly.
		for (int i = 0; i < m_totalPlayers; i++)
		{
			HandDistribution* pDist = NULL;

			// keep it rotating
			//int chair = (m_actualTrials + i) % m_totalPlayers;
			pDist = m_dists[i];

			if (pDist->NotEmpty())
			{
				pDist->Choose1326(usedCardsThisTrial, bCollisionError);

				if (bCollisionError)
				{
					m_collisions++;	
					break;
				}

				// Add the chosen player hand to the dead/used cards...
				StdDeck_CardMask_OR(usedCardsThisTrial, usedCardsThisTrial, pDist->CurrentHand1326());
			}
		}

		if (bCollisionError)
			continue;

		// Now generate a single random board
		DECK_MONTECARLO_N_CARDS_D(StdDeck, cardCombo, usedCardsThisTrial, 
			5 - m_numberOfBoardCards, 1, this->EvalOneTrial1326(cardCombo););
	}

	return m_actualTrials;
}



///////////////////////////////////////////////////////////////////////////////
// Eval 1-trial 1326
///////////////////////////////////////////////////////////////////////////////
void EquityCalculator::EvalOneTrial1326(StdDeck_CardMask boardFragment)
{
	// boardFragment contains the randomly-sampled or -enumerated remainder of the
	// board. For example, if the flop is AsKsQs as above, this parameter will contain
	// the current randomly-sampled-or-enumerated turn and river cards.

	m_actualTrials++;
	StdDeck_CardMask temp;

	bool isTie = false;
	int numTies = 0, bestIndex = -1;
	HandVal cur = 0, best = 0;

	// Combine the base board (eg, flop of "AsKsQs") with the sampled fragment
	// (eg, turn and river of "2s5d"). After this call, 'boardFragment' will
	// contain the entire board (all 5 cards).
	StdDeck_CardMask_OR(boardFragment, boardFragment, m_boardMask);

	// Evaluate each players full 7-card hand in turn...
	for (int chair = 0; chair < m_totalPlayers; chair++)
	{
		HandDistribution* pDist = NULL;
		pDist = m_dists[chair];

		if (pDist->NotEmpty())
		{
			StdDeck_CardMask_RESET(temp);
			// Combine the 5-card board fragment with the player's 2 hole cards...
			StdDeck_CardMask_OR(temp, boardFragment, pDist->CurrentHand1326());

			// Evaluate the resulting hand...cur is a HandVal we can compare to
			// the value of other hands in order to determine a winner.
			cur = StdDeck_StdRules_EVAL_N(temp, 7);

			// If this hand is the best we've seen so far, adjust state...
			if (cur > best)
			{
				best = cur;
				isTie = false;
				bestIndex = chair;
				numTies = 0;
			}

			// Otherwise, if this hand ties another, adjust some state...
			else if (cur == best && cur > 0)
			{
				if (numTies == 0)
				{
					m_tiedPlayerIndexes[0] = bestIndex;
					m_tiedPlayerIndexes[1] = chair;
				}
				else
				{
					m_tiedPlayerIndexes[numTies + 1] = chair;
				}

				isTie = true;
				numTies++;
			}

			// Store the player's hand value...
			m_handVals[chair] = cur;
		}
	}

	// If there's no tie, then the player with the best hand gets a +1 for the win.
	if (bestIndex != -1 && !isTie)
		m_wins[bestIndex] += 1;

	// If there IS a tie, then the players with the tied winning hand get a +X for
	// the win, based on by how many players it was split.
	else
	{
		double partialWin = 1.0 / (1 + (static_cast<double>(numTies)));

		for (int i = 0; i <= numTies; i++)
			m_wins[m_tiedPlayerIndexes[i]] += partialWin;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Once the simulation has been run, this little helper function takes each
// player's win counts and converts these into equity/win percentage.
///////////////////////////////////////////////////////////////////////////////
unsigned int EquityCalculator::PostCalculate1326()
{
	//_cprintf("Starting PostCalculate1326()\n");
	memset(&m_pResults[0], 0, k_MaxChairs*sizeof(double));

	for (int r = 0; r < m_totalPlayers; r++)
	{
		m_pResults[r] = (m_wins[r] / m_actualTrials);
		delete m_dists[r];
	}

	m_dists.clear();

	return m_actualTrials;
}