#pragma once

#include <set>

#include "Util_MagicNumbers.h"
#include "Poker.PokerEval\include\poker_defs.h"


class CPlayerInfo;

class COpponentsModel
{
public:

	COpponentsModel();

protected:

	int nOpponents;

	short EmptyRange[1326];

	double P_Rank[k_MaxChairs][1326];
	double F_Rank[k_MaxChairs][1326];
	double T_Rank[k_MaxChairs][1326];
	double R_Rank[k_MaxChairs][1326];

	StdDeck_CardMask pairAcesKings, pairJacksQueen, pairSevensToTens, pairLow;
	StdDeck_CardMask suitedAceBig, suitedAceGood, suitedAce2To9;
	StdDeck_CardMask suitedConnectorHigh, suitedConnectorMed, suitedConnectorLow;
	StdDeck_CardMask connectorOffHigh, connectorOffMed, connectorOffLow;

public:

	void Get_Preflop_Ranges();
	void Get_Flop_Ranges();
	void Get_Turn_Ranges();
	void Get_River_Ranges();

	void calculate_opponents_handrange();
	short* opponent_handrange(const int iPlayer);
	short* empty_handrange();

	void PredictReaction(const int botAction);
	void CompensateForAgression();

	void Reset(const int iPlayer, const short weight);
	void SetWeight(const std::set<int>& vec, const int iPlayer, const short weight);
	void IncreaseWeight(const std::set<int>& vec, const int iPlayer, const short delta);
	void DecreaseWeight(const std::set<int>& vec, const int iPlayer, const short delta);
	void VerifyWeights();

	//HANDREARRANGE VARS
	void rearrange_handrank();	
};