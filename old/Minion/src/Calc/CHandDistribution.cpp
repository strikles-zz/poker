///////////////////////////////////////
// Lab 42							
//	blablabla						
// (Derived from James Devlin Stuff) //
///////////////////////////////////////

#include <cassert>
#include <ctime>

#include "Calc/CHandDistribution.h"
#include "Util/Util.h"
#include "Util/Util_logging.h"
#include "Util/Util_MagicNumbers.h"


CRank169To1326Utility util1326;

///////////////////////////////////////////////////////////////////////////////
// Default constructor for HandDistribution objects. No-op.
///////////////////////////////////////////////////////////////////////////////
CHandDistribution::CHandDistribution() : 
	m_likelihood1326_short(NULL),
	m_sum(0)
{
	StdDeck_CardMask_RESET(m_currenthand);
	m_hands1326.clear();

#if (_MSC_VER >= 1600)
	t_rand.seed(rnd_device());
#else
	t_rand.seed(time(NULL));
#endif

}


///////////////////////////////////////////////////////////////////////////////
// Strikles Constructor for hero :)
///////////////////////////////////////////////////////////////////////////////
CHandDistribution::CHandDistribution(StdDeck_CardMask const& hand, StdDeck_CardMask const& deadCards) :

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
CHandDistribution::CHandDistribution(short* likelihood1326, StdDeck_CardMask const& deadCards) :

	m_likelihood1326_short(likelihood1326),
	m_sum(0)
{	
	StdDeck_CardMask_RESET(m_currenthand);
	StdDeck_CardMask_RESET(null_mask);
	m_hands1326.clear();
	Init1326(m_likelihood1326_short, deadCards);
}

///////////////////////////////////////////////////////////////////////////////
// Strikles Hero Dist
///////////////////////////////////////////////////////////////////////////////
int CHandDistribution::InitMask(StdDeck_CardMask const& hand, StdDeck_CardMask const& deadCards)
{
	handinfo i_hero;

	StdDeck_CardMask_RESET(i_hero.handMask);
	StdDeck_CardMask_OR(i_hero.handMask, i_hero.handMask, hand);
	StdDeck_CardMask_OR(m_currenthand, m_currenthand, i_hero.handMask);

	i_hero.interval_start = 0;
	i_hero.interval_end = max_weight;
	m_sum = max_weight;

	// whatever
	m_hands1326.push_back(i_hero);

	return m_hands1326.size();
}

///////////////////////////////////////////////////////////////////////////////
// Strikles Opp Dist
///////////////////////////////////////////////////////////////////////////////
int CHandDistribution::Init1326(short* likelihood1326, StdDeck_CardMask const& deadCards)
{
	for(int index = 0; index < 1326; index++)
	{
		// Setup the intervals of the 1326 CDF for each cardmask
		// i.e we Map Cardmasks to corresponding probability space
		// 0 Probability hands are discarded 
		if(StdDeck_CardMask_ANY_SET(util1326.card_mask_for_hand1326[index], deadCards))
			continue;
		
		if(likelihood1326[index] < 1)
			continue;

		handinfo i_opp;

		StdDeck_CardMask_RESET(i_opp.handMask);	
		StdDeck_CardMask_OR(i_opp.handMask, i_opp.handMask, util1326.card_mask_for_hand1326[index]);

		i_opp.interval_start = m_sum;		
		m_sum += likelihood1326[index];
		i_opp.interval_end = m_sum;

		m_hands1326.push_back(i_opp);			
	}

	return m_hands1326.size();
}

StdDeck_CardMask CHandDistribution:: binarySearch(std::vector<handinfo> const& vec, unsigned int value) 
{
	int left_ndx = 0;
	int right_ndx = vec.size() - 1;

	while(left_ndx <= right_ndx) 
	{
		int middle_ndx = (left_ndx + right_ndx) / 2;
		assert(left_ndx >= 0 && right_ndx >= 0 && middle_ndx >= 0);

		if(vec[middle_ndx].interval_start < value && vec[middle_ndx].interval_end >= value)	
			return vec[middle_ndx].handMask;

		else if(vec[middle_ndx].interval_start  > value)
			right_ndx = middle_ndx - 1;

		else
			left_ndx = middle_ndx + 1;
	}
	
	return null_mask;
}

///////////////////////////////////////////////////////////////////////////////
// Strikles Hand Choosing
///////////////////////////////////////////////////////////////////////////////
StdDeck_CardMask CHandDistribution::Choose1326(StdDeck_CardMask const& dead_cards, bool& bCollisionError)
{
	if(IsUnary1326())
		return m_currenthand;

	bCollisionError = false;

	if(m_likelihood1326_short) 
	{

#if (_MSC_VER >= VS_2010)
		std::uniform_int_distribution<unsigned int>  uni_dist(0, m_sum);
#endif

		// Inverse Transform Sampling
		for(int trial_attempt = 0; trial_attempt < 10; trial_attempt++)
		{	

#if (_MSC_VER < VS_2010)
			tmp_rand = t_rand.under(m_sum);		
#else
			tmp_rand = uni_dist(t_rand);
#endif

			StdDeck_CardMask_RESET(m_currenthand);
			StdDeck_CardMask_OR(m_currenthand, m_currenthand, binarySearch(m_hands1326, tmp_rand));	

			if(StdDeck_CardMask_ANY_SET(m_currenthand, dead_cards) || StdDeck_CardMask_EQUAL(m_currenthand,	null_mask))
				continue;

			return m_currenthand;
		}
	}

	bCollisionError = true;
	inv_dist_ptr = (m_likelihood1326_short == NULL);

	return null_mask;
}


///////////////////////////////////////////////////////////////////////////////
// Used by std::sort when we sort the distribution prior to removing duplicate
// hands from the distribution.
///////////////////////////////////////////////////////////////////////////////
bool CHandDistribution::CardMaskGreaterThan( StdDeck_CardMask a, StdDeck_CardMask b )
{
	return a.cards_n < b.cards_n;
}
