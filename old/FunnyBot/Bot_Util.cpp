#include "stdafx.h"

#include <algorithm>

#include "Bot_Util.h"



char GetRankChar(unsigned char c)
{
	/*
	*
	*/

	const char randChars[] = "  23456789TJQKA";

	if (ISCARDBACK(c))
		return '?';
	if (ISUNKNOWN(c))
		return '_';

	return randChars[RANK(c)];
}

char GetSuitChar(unsigned char c)
{
	/*
	*
	*/

	const char suitChars[] = " hdcs";

	if (ISCARDBACK(c))
		return '?';
	if (ISUNKNOWN(c))
		return '_';

	return suitChars[SUIT(c)];
}

int GetNSuited(StdDeck_CardMask hand)
{
	/*
	* 
	*/

	int nsuit[4];

	nsuit[0] = bitcount(StdDeck_CardMask_CLUBS(hand));
	nsuit[1] = bitcount(StdDeck_CardMask_DIAMONDS(hand));
	nsuit[2] = bitcount(StdDeck_CardMask_HEARTS(hand));
	nsuit[3] = bitcount(StdDeck_CardMask_SPADES(hand));

	for (int i=0;i<4;i++) 
	{
		for (int j=i;j<4;j++) 
		{
			if (nsuit[i] > nsuit[j]) 
			{
				int temp = nsuit[i];
				nsuit[i] = nsuit[j];
				nsuit[j] = temp;
			}
		}
	}

	return nsuit[3];
}

char Rank_ASC(int Rank)
{
	/*
	*  
	*/

	switch (Rank)
	{
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'T';
		case 11:
			return 'J';
		case 12:
			return 'Q';
		case 13:
			return 'K';
		case 14:
			return 'A';
		default:
			return 'X';
	}
}

char Suit52_ASC(int Suit)
{
	/*
	*  
	*/

	switch (Suit)
	{
		case 1:
			return 'c';
		case 2:
			return 'd';
		case 3:
			return 'h';
		case 4:
			return 's';
		default:
			return '0';
	}
}

/*
void CleanVector(vector<int>& vec)
{
	// Copy Paste Gymnastics
	// Is this right ?
	sort(vec.begin(), vec.end());
	vec.erase(unique(vec.begin(), vec.end()), vec.end());
}
*/

CRank169To1326Utility::CRank169To1326Utility()
{
	/*
	*  
	*/

	GeneratePreflopHandRanks1326();

	for (int hc1=0, index1326=0; hc1<51; hc1++)
	{
		for (int hc2=hc1+1; hc2<52; hc2++, index1326++)	
		{
			cardMaskFor1326Hand[index1326] = StdDeck_MASK(hc1);
			StdDeck_CardMask_OR(cardMaskFor1326Hand[index1326], cardMaskFor1326Hand[index1326], StdDeck_MASK(hc2));

			FillRanges(hc1, hc2, index1326);
		}
	}
}

void CRank169To1326Utility::Generate169(const int c1, const int c2, char* buffer)
{
	const char* rankChars = "23456789TJQKA";
	memset(buffer, '\0', 4);

	int rank_card1 = StdDeck_RANK(c1);
	int suit_card1 = StdDeck_SUIT(c1);

	int rank_card2 = StdDeck_RANK(c2);
	int suit_card2 = StdDeck_SUIT(c2);

	char str_rank1 = rankChars[rank_card1];
	char str_rank2 = rankChars[rank_card2];

	char suited = ((suit_card1 == suit_card2) ? 's' : 'o');

	if (rank_card1 > rank_card2)
		sprintf_s(buffer, 4, "%c%c%c", str_rank1, str_rank2, suited);
	else
		sprintf_s(buffer, 4, "%c%c%c", str_rank2, str_rank1, suited);
}

int CRank169To1326Utility::GeneratePreflopHandRanks1326()
{
	//vector <vector <vector <pair<int,string>> >> vec;
	std::vector <std::vector <std::vector <std::pair<int,int>> >> vec;

	vec.resize(9);

	for (int i=0; i<9; i++)
		vec[i].resize(169);

	for (int hc1 = 0, ndx1326 = 0; hc1 < 51; hc1++)
	{
		for (int hc2 = hc1+1; hc2 < 52; hc2++, ndx1326++)	
		{
			char hand169[4];
			Generate169(hc1, hc2, hand169);
			//string hand = string(hand169);

			for (int nplayers = 10; nplayers > 1; nplayers--)
			{
				for (int ndx169 = 0; ndx169 < 169; ndx169++)
				{
					if (strncmp(hand169, handranks_169[10-nplayers][ndx169], 4) == 0)
					{
						//pair <int, string> current_pair(ndx1326, hand);
						std::pair<int,int> current_pair(ndx1326, ndx169);
						vec[10-nplayers][ndx169].push_back(current_pair);
						break;
					}
				}
			}
		}
	}

	handranks_1326.resize(9);

	for (int nplayers = 10; nplayers > 1; nplayers--)
	{
		for (int ndx169 = 0; ndx169 < 169; ndx169++)
		{
			for (size_t ndx1326 = 0; ndx1326 < vec[10-nplayers][ndx169].size(); ndx1326 ++)
			{
				//pair <int, string> current_ordered_pair(vec[10-nplayers][ndx169][ndx1326]);
				std::pair<int, int> current_ordered_pair(vec[10-nplayers][ndx169][ndx1326]);
				handranks_1326[10-nplayers].push_back(current_ordered_pair);		
			}
		}
	}

	return 0;
}

void CRank169To1326Utility::FillRanges(const int hc1, const int hc2, const int index1326)
{
	// CONTAINS RANKS
	// hasAces, hasKings, hasQueens, hasTens, hasNines, hasEights, hasSevens, hasSixes, hasFives, hasFours, hasThrees, hasTwos;
	// Aces
	if (StdDeck_RANK(hc1) == 12 || StdDeck_RANK(hc2) == 12)
		hasAces.insert(index1326);
	// Kings
	if (StdDeck_RANK(hc1) == 11 || StdDeck_RANK(hc2) == 11)
		hasKings.insert(index1326);
	// Queens
	if (StdDeck_RANK(hc1) == 10 || StdDeck_RANK(hc2) == 10)
		hasQueens.insert(index1326);
	// Jacks
	if (StdDeck_RANK(hc1) == 9 || StdDeck_RANK(hc2) == 9)
		hasJacks.insert(index1326);
	// Tens
	if (StdDeck_RANK(hc1) == 8 || StdDeck_RANK(hc2) == 8)
		hasTens.insert(index1326);
	// Nines
	if (StdDeck_RANK(hc1) == 7 || StdDeck_RANK(hc2) == 7)
		hasNines.insert(index1326);
	// Eights
	if (StdDeck_RANK(hc1) == 6 || StdDeck_RANK(hc2) == 6)
		hasEights.insert(index1326);
	// Sevens
	if (StdDeck_RANK(hc1) == 5 || StdDeck_RANK(hc2) == 5)
		hasSevens.insert(index1326);
	// Sixes
	if (StdDeck_RANK(hc1) == 4 || StdDeck_RANK(hc2) == 4)
		hasSixes.insert(index1326);
	// Fives
	if (StdDeck_RANK(hc1) == 3 || StdDeck_RANK(hc2) == 3)
		hasFives.insert(index1326);
	// Fours
	if (StdDeck_RANK(hc1) == 2 || StdDeck_RANK(hc2) == 2)
		hasFours.insert(index1326);
	// Threes
	if (StdDeck_RANK(hc1) == 1 || StdDeck_RANK(hc2) == 1)
		hasThrees.insert(index1326);
	// Twos
	if (StdDeck_RANK(hc1) == 0 || StdDeck_RANK(hc2) == 0)
		hasTwos.insert(index1326);

	// SUITS
	// hasHearts, hasDiamonds, hasClubs, hasSpades
	// hearts
	if (StdDeck_SUIT(hc1) == 0 || StdDeck_SUIT(hc2) == 0)
		hasHearts.insert(index1326);
	// diamonds
	if (StdDeck_SUIT(hc1) == 1|| StdDeck_SUIT(hc2) == 1)
		hasDiamonds.insert(index1326);
	// clubs
	if (StdDeck_SUIT(hc1) == 2 || StdDeck_SUIT(hc2) == 2)
		hasClubs.insert(index1326);
	// spades
	if (StdDeck_SUIT(hc1) == 3 || StdDeck_SUIT(hc2) == 3)
		hasSpades.insert(index1326);

	// PAIRS
	// pairAcesKings, pairQueensJacks, pairTensToSevens, pairLow;
	if (StdDeck_RANK(hc1) == StdDeck_RANK(hc2))
	{
		// Aces + Kings
		if (StdDeck_RANK(hc1) == 12 || StdDeck_RANK(hc1) == 11)
			pairAcesKings.insert(index1326);
		// Jacks + Queens
		else if (StdDeck_RANK(hc1) == 10 || StdDeck_RANK(hc1) == 9)
			pairQueensJacks.insert(index1326);
		// Sevens to Tens
		else if (StdDeck_RANK(hc1) >= 5 || StdDeck_RANK(hc1) <= 8)
			pairTensToSevens.insert(index1326);

		// Low Pairs
		else
			pairLow.insert(index1326);
	}

	// SUITED
	else if (StdDeck_SUIT(hc1) == StdDeck_SUIT(hc2))
	{
		int suited_max = -1, suited_min = -1;

		if (StdDeck_RANK(hc1) > StdDeck_RANK(hc2))
		{
			suited_max = StdDeck_RANK(hc1);
			suited_min = StdDeck_RANK(hc2);
		}
		else
		{
			suited_max = StdDeck_RANK(hc2);
			suited_min = StdDeck_RANK(hc1);
		}

		// SUITED ACE
		// suitedAceBig, suitedAceGood, suitedAce2To9;
		if (suited_max == 12)
		{
			// Suited Ace Big
			if (suited_min == 11)
				suitedAceBig.insert(index1326);
			// Suited Ace Good
			else if (suited_min >= 8)
				suitedAceGood.insert(index1326);
			// Suited Ace + 2to9
			else
				suitedAce2To9.insert(index1326);
		}

		// SUITED CONNECTORS
		// suitedConnectorHigh, suitedConnectorMed, suitedConnectorLow;
		if (suited_max - suited_min == 1)
		{
			// Suited Connector High
			if (suited_max >= 9)
				suitedConnectorHigh.insert(index1326);
			// Suited Connector Med
			else if (suited_max >= 6)
				suitedConnectorMed.insert(index1326);
			// Suited Connector Low
			else
				suitedConnectorLow.insert(index1326);
		}
	}

	// OFFSUIT
	else
	{
		int off_max = -1, off_min = -1;

		if (StdDeck_RANK(hc1) > StdDeck_RANK(hc2))
		{
			off_max = StdDeck_RANK(hc1);
			off_min = StdDeck_RANK(hc2);
		}
		else
		{
			off_max = StdDeck_RANK(hc2);
			off_min = StdDeck_RANK(hc1);
		}

		// OFFSUIT CONNECTORS
		// OffconnectorHigh, OffconnectorMed, OffconnectorLow;
		if (off_max - off_min == 1)
		{
			// Off Connector High
			if (off_max >= 9)
				offConnectorHigh.insert(index1326);
			// Off Connector Med
			else if (off_max >= 6)
				offConnectorMed.insert(index1326);
			// Off Connector Low
			else
				offConnectorLow.insert(index1326);
		}
	}
}