
#pragma once

#include <Windows.h>

#include <cmath>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "Util_MagicNumbers.h"
#include "poker_defs.h"


struct EnumWindowsCallbackArgs 
{
	EnumWindowsCallbackArgs(DWORD p) : pid(p) { }
	const DWORD pid;
	std::vector<HWND> handles;
};

static BOOL CALLBACK EnumWindowsCallback(HWND hnd, LPARAM lParam);
std::vector<HWND> getToplevelOHWindows();
bool checkFileExistence(const std::string& fileName);


inline double ToMoney(double val)
{
	val *= 100.0;
	val = (val > (floor(val)+0.5)) ? ceil(val) : floor(val);
	val /= 100.0;

	return val;
}

template <typename T>
std::string ToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

inline int bitcount(unsigned int u)
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

	return fabs(a-b) < epsilon;
}

inline bool isInvalidChair(const int chair)
{
	/*
	*  Function used to compare doubles
	*/

	return (chair < 0 || chair >= k_max_chairs);
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

	StdDeck_CardMask card_mask_for_hand1326[1326];
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

extern CRank169To1326Utility util1326;