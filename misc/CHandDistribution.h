///////////////////////////////////////
// Lab 42							
//	blablabla
// (Derived from James Devlin Stuff)
///////////////////////////////////////

#pragma once

#include <vector>
#include "poker_defs.h"

#include "Util/Util.h"
#include "Util/Util_MagicNumbers.h"

#if (_MSC_VER < VS_2010)
	#include "Util/Util_mtrand.h"
#else
	#include <random>
#endif



class CHandDistribution
{
public:

	CHandDistribution();
	CHandDistribution(const char* hand);
	CHandDistribution(const char* hand, StdDeck_CardMask dead_cards);

private:

	short* m_likelihood1326_short;
	StdDeck_CardMask m_currenthand;
	StdDeck_CardMask null_mask;

	typedef struct
	{
		unsigned int interval_start;
		unsigned int interval_end;
		StdDeck_CardMask handMask;
		
	} handinfo;

	std::vector<handinfo> m_hands1326;
	
	unsigned int m_sum;
	unsigned int tmp_rand;


#if (_MSC_VER < VS_2010)
	MTRand53 t_rand;
#else
	std::mt19937 t_rand;
	std::random_device  rnd_device;
#endif
	
	bool inv_dist_ptr;

public:

	CHandDistribution(StdDeck_CardMask const& hand, StdDeck_CardMask const& dead_cards);
	CHandDistribution(short* likelihood1326, StdDeck_CardMask const& dead_cards);

	int InitMask(StdDeck_CardMask const& hand, StdDeck_CardMask const& dead_cards);
	int Init1326(short* likelihood1326, StdDeck_CardMask const& dead_cards);

	StdDeck_CardMask binarySearch(std::vector<handinfo> const&, unsigned int value);
	StdDeck_CardMask Choose1326(StdDeck_CardMask const& dead_cards, bool& bCollisionError);
	StdDeck_CardMask CurrentHand1326() const { return m_currenthand; };

	bool IsUnary1326() const { return m_hands1326.size() == 1; };
	bool NotEmpty() const { return (!m_hands1326.empty()); };

	static bool CardMaskGreaterThan( StdDeck_CardMask a, StdDeck_CardMask b );
	StdDeck_CardMask Hand1326ToCardMask(int index) { return  util1326.card_mask_for_hand1326[index]; }

	friend class CEquityCalculator; // terrible programmer...
};
