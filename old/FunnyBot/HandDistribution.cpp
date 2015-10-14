///////////////////////////////////////
// Lab 42									 //
//	blablabla								 //
// (Derived from James Devlin Stuff) //
///////////////////////////////////////

#include "stdAfx.h"

#include "HandDistribution.h"
#include "Util_logging.h"
#include "Util_MagicNumbers.h"
#include "Bot_Globalvars.h"

#include "Poker.PokerEval\include\poker_defs.h"



///////////////////////////////////////////////////////////////////////////////
// Default constructor for HandDistribution objects. No-op.
///////////////////////////////////////////////////////////////////////////////
HandDistribution::HandDistribution() : 

	m_likelihood1326_short(NULL),
	m_sum(0)
{
	StdDeck_CardMask_RESET(m_currenthand);
	m_hands1326.clear();

}


///////////////////////////////////////////////////////////////////////////////
// Strikles Constructor for hero :)
///////////////////////////////////////////////////////////////////////////////
HandDistribution::HandDistribution(StdDeck_CardMask const& hand, StdDeck_CardMask const& deadCards) :

	m_likelihood1326_short(NULL),
	m_sum(0)
{
	StdDeck_CardMask_RESET(m_currenthand);
	m_hands1326.clear();
	InitMask(hand, deadCards);
}

///////////////////////////////////////////////////////////////////////////////
// Strikles Constructor for opponents
///////////////////////////////////////////////////////////////////////////////
HandDistribution::HandDistribution(short* likelihood1326, StdDeck_CardMask const& deadCards) :

	m_likelihood1326_short(likelihood1326),
	m_sum(0)
{	
	StdDeck_CardMask_RESET(m_currenthand);
	StdDeck_CardMask_RESET(nullMask);
	m_hands1326.clear();
	Init1326(m_likelihood1326_short, deadCards);
}

///////////////////////////////////////////////////////////////////////////////
// Strikles Hero Dist
///////////////////////////////////////////////////////////////////////////////
int HandDistribution::InitMask(StdDeck_CardMask const& hand, StdDeck_CardMask const& deadCards)
{
	handinfo i_hero;

	StdDeck_CardMask_RESET(i_hero.handMask);
	StdDeck_CardMask_OR(i_hero.handMask, i_hero.handMask, hand);
	StdDeck_CardMask_OR(m_currenthand, m_currenthand, i_hero.handMask);

	i_hero.interval_start = 0;
	i_hero.interval_end = max_weight;
	m_sum = max_weight;

	m_hands1326.push_back(i_hero);

	return m_hands1326.size();
}

///////////////////////////////////////////////////////////////////////////////
// Strikles Opp Dist
///////////////////////////////////////////////////////////////////////////////
int HandDistribution::Init1326(short* likelihood1326, StdDeck_CardMask const& deadCards)
{
	for (int index = 0; index < 1326; index++)
	{
		// Setup the intervals of the 1326 CDF for each cardmask
		// i.e we Map Cardmasks to corresponding probability space
		// 0 Probability hands are discarded 
		if (StdDeck_CardMask_ANY_SET(gVars.util1326.cardMaskFor1326Hand[index], deadCards))
			continue;
		
		if (likelihood1326[index] < 1)
			continue;

		handinfo i_opp;

		StdDeck_CardMask_RESET(i_opp.handMask);	
		StdDeck_CardMask_OR(i_opp.handMask, i_opp.handMask, gVars.util1326.cardMaskFor1326Hand[index]);

		i_opp.interval_start = m_sum;		
		m_sum += likelihood1326[index];
		i_opp.interval_end = m_sum;

		m_hands1326.push_back(i_opp);			
	}

	return m_hands1326.size();
}

StdDeck_CardMask HandDistribution:: binarySearch(std::vector<handinfo> const& vec, unsigned int value) 
{
	unsigned int left_ndx = 0;
	unsigned int right_ndx = vec.size() - 1;

	while (left_ndx <= right_ndx) 
	{
		int middle_ndx = (left_ndx + right_ndx) / 2;

		if (vec[middle_ndx].interval_start < value && vec[middle_ndx].interval_end >= value)	
			return vec[middle_ndx].handMask;

		else if (vec[middle_ndx].interval_start  > value)
			right_ndx = middle_ndx - 1;

		else
			left_ndx = middle_ndx + 1;
	}
	
	return nullMask;
}

///////////////////////////////////////////////////////////////////////////////
// Strikles Hand Choosing
///////////////////////////////////////////////////////////////////////////////
StdDeck_CardMask HandDistribution::Choose1326(StdDeck_CardMask const& deadCards, bool& bCollisionError)
{
	if (IsUnary1326())
		return m_currenthand;

	bCollisionError = false;

	if (m_likelihood1326_short) 
	{
		std::uniform_int_distribution<unsigned int>  uni_dist(0, m_sum);

		// Inverse Transform Sampling
		for (int trial_attempt = 0; trial_attempt < 10; trial_attempt++)
		{		
			tempRand = uni_dist(gVars.t_rand);

			StdDeck_CardMask_RESET(m_currenthand);
			StdDeck_CardMask_OR(m_currenthand, m_currenthand, binarySearch(m_hands1326, tempRand));	

			if (StdDeck_CardMask_ANY_SET(m_currenthand, deadCards) || StdDeck_CardMask_EQUAL(m_currenthand,	nullMask))
				continue;

			return m_currenthand;
		}
	}

	bCollisionError = true;
	inv_dist_ptr = (m_likelihood1326_short == NULL);

	return nullMask;
}



///////////////////////////////////////////////////////////////////////////////
// Used by std::sort when we sort the distribution prior to removing duplicate
// hands from the distribution.
///////////////////////////////////////////////////////////////////////////////
bool HandDistribution::CardMaskGreaterThan( StdDeck_CardMask a, StdDeck_CardMask b )
{
	return a.cards_n < b.cards_n;
}
