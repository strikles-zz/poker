///////////////////////////////////////
// Lab 42							
//	blablabla						
// (Derived from James Devlin Stuff) //
///////////////////////////////////////

#include "Calc/CEquityCalculator.h"
#include "Calc/CHandDistribution.h"
#include "Util/Util_logging.h"

#include "inlines\eval.h"


///////////////////////////////////////////////////////////////////////////////
// Default constructor for EquityCalculator objects.
///////////////////////////////////////////////////////////////////////////////
CEquityCalculator::CEquityCalculator()
{
	Reset1326();
}


///////////////////////////////////////////////////////////////////////////////
// Strikles Reset 1326
///////////////////////////////////////////////////////////////////////////////
void CEquityCalculator::Reset1326()
{
	StdDeck_CardMask_RESET(m_heroMask);
	StdDeck_CardMask_RESET(m_boardMask);
	StdDeck_CardMask_RESET(m_deadMask);

	m_MonteCarloThreshhold = UINT_MAX;
	m_numberOfBoardCards = 0;
	m_collisions = 0;
	m_total_players = 0;

	m_indicatedTrials = 0;
	m_actual_trials = 0;

	m_p_results = NULL;

	memset(&m_wins[0], 0, k_max_chairs*sizeof(m_wins[0]));
	memset(&m_tiedPlayerIndexes[0], 0, k_max_chairs*sizeof(m_tiedPlayerIndexes[0]));
	memset(&m_handVals[0], HandVal_NOTHING, k_max_chairs*sizeof(m_handVals[0]));
}

////////////////////////////////////////
// Main fnc
////////////////////////////////////////
unsigned int CEquityCalculator::CalculateEquity
(
	const unsigned int numberOfTrials,
	const unsigned int userchair,
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
	Reset1326();

	m_boardMask = boardMask;
	m_heroMask = heroMask;
	StdDeck_CardMask_OR(m_deadMask, m_heroMask, m_boardMask);

	// Get the number of board cards that were supplied. For Hold'em, should be 3, 4 or 5.
	m_numberOfBoardCards = StdDeck_numCards(m_boardMask);
	m_indicatedTrials = numberOfTrials;

	// Tuck this away for later
	m_p_results = outResults;

	m_total_players = k_max_chairs;
	m_dists.resize(m_total_players);

	short* likelihood1326[k_max_chairs] = { NULL };

	if(likelihood1326_0)
		likelihood1326[0] = likelihood1326_0;
	if(likelihood1326_1)
		likelihood1326[1] = likelihood1326_1;
	if(likelihood1326_2)
		likelihood1326[2] = likelihood1326_2;
	if(likelihood1326_3)
		likelihood1326[3] = likelihood1326_3;
	if(likelihood1326_4)
		likelihood1326[4] = likelihood1326_4;
	if(likelihood1326_5)
		likelihood1326[5] = likelihood1326_5;
	if(likelihood1326_6)
		likelihood1326[6] = likelihood1326_6;
	if(likelihood1326_7)
		likelihood1326[7] = likelihood1326_7;
	if(likelihood1326_8)
		likelihood1326[8] = likelihood1326_8;
	if(likelihood1326_9)
		likelihood1326[9] = likelihood1326_9;

	for(int index = 0; index < m_total_players; index++)
	{
		CHandDistribution* pCur = NULL;

		if(index == userchair) 
			pCur = new CHandDistribution(m_heroMask, m_boardMask);
		else if(likelihood1326[index])
			pCur = new CHandDistribution(likelihood1326[index], m_deadMask);
		else
			pCur = new CHandDistribution();

		m_dists[index] = pCur;

		if(pCur->NotEmpty())
			gLog.WriteLog(eSeverityInfo, eCatCalculator, "Player%d Sum : %d\n", index, pCur->m_sum);
	}

	CalculateMonteCarlo1326();

	return PostCalculate1326();
}

///////////////////////////////////////////////////////////////////////////////
// MC 1326
///////////////////////////////////////////////////////////////////////////////
unsigned int CEquityCalculator::CalculateMonteCarlo1326()
{
	m_wasMonteCarlo = true;

	StdDeck_CardMask cardCombo;
	StdDeck_CardMask usedCardsThisTrial;

	// Now run however many trials
	while(m_actual_trials < m_indicatedTrials)
	{
		// Reset "used cards"
		StdDeck_CardMask_RESET(cardCombo);
		StdDeck_CardMask_RESET(usedCardsThisTrial);
		StdDeck_CardMask_OR(usedCardsThisTrial, usedCardsThisTrial, m_boardMask);

		bool bCollisionError = false;
		
		// For each player in the hand, choose his hand. If the player has a specific hand,
		// great, use that one. Otherwise pick one hand from his distribution randomly.
		for(int i = 0; i < m_total_players; i++)
		{
			CHandDistribution* pDist = NULL;

			// keep it rotating
			//int chair = (m_actualTrials + i) % m_totalPlayers;
			pDist = m_dists[i];

			if(pDist->NotEmpty())
			{
				pDist->Choose1326(usedCardsThisTrial, bCollisionError);

				if(bCollisionError)
				{
					m_collisions++;	
					break;
				}

				// Add the chosen player hand to the dead/used cards...
				StdDeck_CardMask_OR(usedCardsThisTrial, usedCardsThisTrial, pDist->CurrentHand1326());
			}
		}

		if(bCollisionError)
			continue;

		// Now generate a single random board
		DECK_MONTECARLO_N_CARDS_D(StdDeck, cardCombo, usedCardsThisTrial, 5 - m_numberOfBoardCards, 1, this->EvalOneTrial1326(cardCombo););
	}

	return m_actual_trials;
}



///////////////////////////////////////////////////////////////////////////////
// Eval 1-trial 1326
///////////////////////////////////////////////////////////////////////////////
void CEquityCalculator::EvalOneTrial1326(StdDeck_CardMask boardFragment)
{
	// boardFragment contains the randomly-sampled or -enumerated remainder of the
	// board. For example, if the flop is AsKsQs as above, this parameter will contain
	// the current randomly-sampled-or-enumerated turn and river cards.

	m_actual_trials++;
	StdDeck_CardMask temp;
	bool isTie = false;
	int numTies = 0, bestIndex = -1;
	HandVal cur = 0, best = 0;

	// Combine the base board (eg, flop of "AsKsQs") with the sampled fragment
	// (eg, turn and river of "2s5d"). After this call, 'boardFragment' will
	// contain the entire board (all 5 cards).
	StdDeck_CardMask_OR(boardFragment, boardFragment, m_boardMask);

	// Evaluate each players full 7-card hand in turn...
	for(int chair = 0; chair < m_total_players; chair++)
	{
		CHandDistribution* pDist = NULL;
		pDist = m_dists[chair];

		if(pDist->NotEmpty())
		{
			StdDeck_CardMask_RESET(temp);
			// Combine the 5-card board fragment with the player's 2 hole cards...
			StdDeck_CardMask_OR(temp, boardFragment, pDist->CurrentHand1326());

			// Evaluate the resulting hand...cur is a HandVal we can compare to
			// the value of other hands in order to determine a winner.
			cur = StdDeck_StdRules_EVAL_N(temp, 7);

			// If this hand is the best we've seen so far, adjust state...
			if(cur > best)
			{
				best = cur;
				isTie = false;
				bestIndex = chair;
				numTies = 0;
			}

			// Otherwise, if this hand ties another, adjust some state...
			else if(cur == best && cur > 0)
			{
				if(numTies == 0)
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
	if(bestIndex != -1 && !isTie)
		m_wins[bestIndex] += 1;

	// If there IS a tie, then the players with the tied winning hand get a +X for
	// the win, based on by how many players it was split.
	else
	{
		double partialWin = 1.0 / (1.0 + (static_cast<double>(numTies)));

		for(int i = 0; i <= numTies; i++)
			m_wins[m_tiedPlayerIndexes[i]] += partialWin;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Once the simulation has been run, this little helper function takes each
// player's win counts and converts these into equity/win percentage.
///////////////////////////////////////////////////////////////////////////////
unsigned int CEquityCalculator::PostCalculate1326()
{
	memset(&m_p_results[0], 0, k_max_chairs*sizeof(m_p_results[0]));

	for(int r = 0; r < m_total_players; r++)
	{
		m_p_results[r] = (m_wins[r] / m_actual_trials);
		delete m_dists[r];
	}

	m_dists.clear();

	return m_actual_trials;
}