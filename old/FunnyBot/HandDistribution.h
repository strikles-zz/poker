///////////////////////////////////////
// Lab 42							
//	blablabla
// (Derived from James Devlin Stuff)
///////////////////////////////////////

#pragma once

#include <random>
#include "Poker.PokerEval\include\poker_defs.h"

class HandDistribution
{
public:

	HandDistribution();
	HandDistribution(const char* hand);
	HandDistribution(const char* hand, StdDeck_CardMask deadCards);

private:

	short* m_likelihood1326_short;
	StdDeck_CardMask m_currenthand;
	StdDeck_CardMask nullMask;

	typedef struct
	{
		unsigned int interval_start;
		unsigned int interval_end;
		StdDeck_CardMask handMask;
		
	} handinfo;

	std::vector<handinfo> m_hands1326;
	
	unsigned int m_sum;
	unsigned int tempRand;

	//MTRand_open t_rand;
	
	bool inv_dist_ptr;

public:

	HandDistribution(StdDeck_CardMask const& hand, StdDeck_CardMask const& deadCards);
	HandDistribution(short* likelihood1326, StdDeck_CardMask const& deadCards);

	int InitMask(StdDeck_CardMask const& hand, StdDeck_CardMask const& deadCards);
	int Init1326(short* likelihood1326, StdDeck_CardMask const& deadCards);

	StdDeck_CardMask binarySearch(std::vector<handinfo> const&, unsigned int value);
	StdDeck_CardMask Choose1326(StdDeck_CardMask const& deadCards, bool& bCollisionError);
	StdDeck_CardMask CurrentHand1326() const { return m_currenthand; }

	bool IsUnary1326() const { return m_hands1326.size() == 1; }
	bool NotEmpty() const { return (!m_hands1326.empty()); }

	static bool CardMaskGreaterThan( StdDeck_CardMask a, StdDeck_CardMask b );

	friend class EquityCalculator; // terrible programmer...
};
