#pragma once

#include <set>
#include <vector>

#include "Util_MagicNumbers.h"
#include "Poker.PokerEval\include\poker_defs.h"


inline int	bitcount(unsigned int u)
{ 
	/*
	* returns count of number of bits set in the given value
	*/

	unsigned int uCount = 0;

	uCount = u 
		- ((u >> 1) & 033333333333) 
		- ((u >> 2) & 011111111111);

	return ((uCount + (uCount >> 3)) & 030707070707) % 63;
}


inline bool IsEqual(const double a, const double b)
{
	/*
	*  Function used to compare doubles
	*/

	return ((a > (b - epsilon)) && (a < (b + epsilon)));
}

inline bool isInvalidChair(const int chair)
{
	/*
	*  Function used to compare doubles
	*/

	return (chair < 0 || chair >= k_MaxChairs);
}

inline const char* bool_string(const bool b) 
{
	return (b ? "TRUE" : "FALSE");
}


char GetRankChar(unsigned char c);
char GetSuitChar(unsigned char c);
int GetNSuited(StdDeck_CardMask hand);

char Rank_ASC(int Rank);
char Suit52_ASC(int Suit);



class CRank169To1326Utility
{
public:

	CRank169To1326Utility();	

public:

	void FillRanges(const int hc1, const int hc2, const int index1326);
	void CleanRanges();

	void Generate169(const int c1, const int c2, char* buffer);
	int GeneratePreflopHandRanks1326();

public:

	StdDeck_CardMask cardMaskFor1326Hand[1326];
	//std::vector <std::vector <std::pair<int,std::string>> > handranks_1326;
	std::vector<std::vector<std::pair<int,int>>> handranks_1326;

	// Card Indexes for Opp Model 
	std::set<int> pairAcesKings, pairQueensJacks, pairTensToSevens, pairLow;
	std::set<int> suitedAceBig, suitedAceGood, suitedAce2To9;
	std::set<int> suitedConnectorHigh, suitedConnectorMed, suitedConnectorLow;
	std::set<int> offConnectorHigh, offConnectorMed, offConnectorLow;
	std::set<int> hasAces, hasKings, hasQueens, hasJacks, hasTens, hasNines, hasEights, hasSevens, hasSixes, hasFives, hasFours, hasThrees, hasTwos;
	std::set<int> hasClubs, hasSpades, hasHearts, hasDiamonds;
};


/*
class Stopwatch
{
	LARGE_INTEGER m_StartCount;
	LARGE_INTEGER m_StopCount;

	static void Check(bool b)
	{
		if (!b) throw std::runtime_error("Stopwatch runtime error");
	}

public:

	Stopwatch()
	{
		Reset();
	}

	Stopwatch(bool start)
	{
		Reset();
		if (start)
			Start();
	}

	static LONGLONG GetFrequency()
	{
		static LARGE_INTEGER ticksPerSecond = {0, 0};
		if (ticksPerSecond.QuadPart == 0)
			Check(QueryPerformanceFrequency(&ticksPerSecond));
		return ticksPerSecond.QuadPart;
	}

	void Reset()
	{
		m_StopCount.QuadPart = 0;
		m_StartCount.QuadPart = 0;

		assert(!IsRunning());
		assert(GetElapsedTicks() == 0);
	}

	void Start()
	{
		if (IsRunning())
			return;

		const bool resume = (m_StartCount.QuadPart != 0);
		if (resume)
			m_StopCount.QuadPart = 0;
		else
			Check(QueryPerformanceCounter(&m_StartCount));

		assert(IsRunning());
	}

	void Stop()
	{
		if (!IsRunning())
			return;
		Check(QueryPerformanceCounter(&m_StopCount));

		assert(!IsRunning());
	}

	LONGLONG GetElapsedTicks() const
	{
		if (!IsRunning())
			return (m_StopCount.QuadPart - m_StartCount.QuadPart);

		LARGE_INTEGER performanceCount;
		Check(QueryPerformanceCounter(&performanceCount));
		return (performanceCount.QuadPart - m_StartCount.QuadPart);
	}

	LONGLONG GetElapsedMilliseconds() const
	{
		return GetElapsedTicks() / (GetFrequency() / 1000);
	}

	bool IsRunning() const
	{
		return m_StartCount.QuadPart != 0 && m_StopCount.QuadPart == 0;
	}
};
*/