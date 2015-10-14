#include "stdafx.h"

#include <set>
#include "Ranges.h"

#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"
#include "Util_MagicNumbers.h"

#include "Poker.PokerEval\include\inlines\eval_type.h"


struct PrwinWithIndex
{
	short prwin;
	int index1326;
};



int comparePrwinWIthIndex(const void *arg1, const void *arg2)
{
	if (((PrwinWithIndex*)arg1)->prwin < ((PrwinWithIndex*)arg2)->prwin)
		return 1;
	else if (((PrwinWithIndex *)arg1)->prwin > ((PrwinWithIndex *)arg2)->prwin)
		return -1;

	return 0;
}

COpponentsModel::COpponentsModel() 
{
	std::fill(&P_Rank[0][0], &F_Rank[0][0]+k_MaxChairs*1326, 1.0);
	std::fill(&F_Rank[0][0], &F_Rank[0][0]+k_MaxChairs*1326, 1.0);
	std::fill(&T_Rank[0][0], &T_Rank[0][0]+k_MaxChairs*1326, 1.0);
	std::fill(&R_Rank[0][0], &R_Rank[0][0]+k_MaxChairs*1326, 1.0);

	std::fill(&EmptyRange[0], &EmptyRange[0]+1326, max_weight);
}




short *COpponentsModel::opponent_handrange(const int iPlayer)
{
	return &gVars.HandLikelihood[iPlayer][0];
}


short *COpponentsModel::empty_handrange()
{
	// Incomplete : Needs Improving :(
	// Infer something reasonable from available info

	return EmptyRange;
}


void COpponentsModel::rearrange_handrank()
{
	PrwinWithIndex rArray[1326];
	int board[5];

	for(int i = 0; i < (gVars.br + 1); i++)
	{
		board[i] = 4*(gVars.Bot_Common_Cards[2+i][0] - 2) + (gVars.Bot_Common_Cards[2+i][1] - 1);
	}

	for (int i = 0, index1326=0; i < 51; i++)
	{
		//_cprintf("[%d] %d\n", i, (i/13) + 4*(i%13));

		for (int j = i+1; j < 52; j++, index1326++)
		{
			int hc1 = (i/13) + 4*(i%13);
			int hc2 = (j/13) + 4*(j%13);

			int index;
			short data = -999;

			if (gVars.br == 2)
			{
				index = gVars.p_flop->g_index(hc1, hc2, board);
				data = gVars.p_flop->data[index];
			}
			else if (gVars.br == 3)
			{		
				index = gVars.p_turn->g_index(hc1, hc2, board);
				data = gVars.p_turn->data[index];
			}
			else if (gVars.br == 4)
			{
				index = gVars.p_river->g_index(hc1, hc2, board);
				data = gVars.p_river->data[index];
			}

			rArray[index1326].prwin = data;
			rArray[index1326].index1326 = index1326;

			if (hc1 == board[0] || hc1 == board[1] || hc1 == board[2] || hc1 == board[3] || hc1 == board[4] ||
				 hc2 == board[0] || hc2 == board[1] || hc2 == board[2] || hc2 == board[3] || hc2 == board[4])
				continue;

			if (rArray[index1326].prwin == -1)
				gLog->WriteLog(eSeverityError, eCatDebugNow, "[ndx:%d] prwin:%d\n", rArray[index1326].index1326, rArray[index1326].prwin);
		}
	}

	qsort(rArray, 1326, sizeof(PrwinWithIndex), comparePrwinWIthIndex);

	for (int i = 0; i < 1326; i++)
		gVars.Rank_Reorganization[gVars.br - 2][i] = rArray[i].index1326;
}


////////////////////////////
// Weight setting utilities
////////////////////////////
void COpponentsModel::Reset(const int iPlayer, const short weight)
{
	if (weight < 0 || weight > max_weight)
	{
		gLog->WriteLog(eSeverityError, eCatOponentModelling, "Error : Value for weight should be in interval [0, 100]\n", weight);
		return;
	}

	std::fill(&gVars.HandLikelihood[iPlayer][0], &gVars.HandLikelihood[iPlayer][0]+1326, weight);
}


void COpponentsModel::SetWeight(const std::set<int>& vec, const int iPlayer, const short weight)
{
	if (weight < 1 || weight > max_weight)
	{
		gLog->WriteLog(eSeverityError, eCatOponentModelling, "Error : Value for weight should be in interval [0, 100]\n", weight);
		return;
	}

	for (std::set<int>::const_iterator i = vec.cbegin(); i != vec.cend(); ++i)
		gVars.HandLikelihood[iPlayer][*i] = weight;
}


void COpponentsModel::IncreaseWeight(const std::set<int>& vec, const int iPlayer, const short delta)
{
	for (std::set<int>::const_iterator i = vec.cbegin(); i != vec.cend(); ++i)
	{
		double temp = gVars.HandLikelihood[iPlayer][*i] + delta;

		if (temp < max_weight)
			gVars.HandLikelihood[iPlayer][*i] = temp;
		else
			gVars.HandLikelihood[iPlayer][*i] = max_weight;
	}
}


void COpponentsModel::DecreaseWeight(const std::set<int>& vec, const int iPlayer, const short delta)
{
	for (std::set<int>::const_iterator i = vec.cbegin(); i != vec.cend(); ++i)
	{
		double temp = gVars.HandLikelihood[iPlayer][*i] - delta;

		// Might want to set this to a small epsilon ?
		if (temp > 1)	
			gVars.HandLikelihood[iPlayer][*i] = temp;
		else
			gVars.HandLikelihood[iPlayer][*i] = 1;
	}
}


void COpponentsModel::VerifyWeights()
{
	int count[k_MaxChairs] = {0};

	for (int iPlayer = 0; iPlayer < 10; iPlayer++)
	{
		if (iPlayer == gVars.userchair || (!gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame))
			continue;

		for(int i=0; i<1326; i++)
		{
			// Might want to set this to a small epsilon ?
			if (gVars.HandLikelihood[iPlayer][i] < 1)
				gVars.HandLikelihood[iPlayer][i] = 1;

			else if (gVars.HandLikelihood[iPlayer][i] > max_weight)
				gVars.HandLikelihood[iPlayer][i] = max_weight;

			if (gVars.HandLikelihood[iPlayer][i] > 0)
				count[iPlayer]++;
		}

		//gLog->WriteLog(eSeverityInfo, eCatOponentModelling, "Opponent %d: %d hands\n", iPlayer, count[iPlayer]);
	}
}


void COpponentsModel::CompensateForAgression()
{
	// Incomplete
	// Should Take into account :
	// Left to act + TotalPlayers
	// Betround
	// Board Texture
	// Player Credibility (Stack Size, etc ...)

	std::vector<int> awesome_ranges;
	std::vector<int> crap_ranges;

	StdDeck_CardMask tempMask, deadMask, plCards, boardMask, card;
	StdDeck_CardMask_RESET(deadMask);
	StdDeck_CardMask_RESET(boardMask);
	StdDeck_CardMask_RESET(plCards);

	for (int i=0; i<(1+gVars.br); i++)
	{
		card = StdDeck_MASK(StdDeck_MAKE_CARD((gVars.Bot_Common_Cards[2+i][0]-2), ConvertSuitsForPokerEval[gVars.Bot_Common_Cards[2+i][1]]));
		StdDeck_CardMask_OR(boardMask, boardMask, card);
	}

	int board_value = StdDeck_StdRules_EVAL_TYPE(boardMask, gVars.br+1);

	for (int i=0; i<2; i++)
	{
		card = StdDeck_MASK(StdDeck_MAKE_CARD((gVars.Bot_Common_Cards[i][0]-2), ConvertSuitsForPokerEval[gVars.Bot_Common_Cards[i][1]]));
		StdDeck_CardMask_OR(plCards, plCards, card);
	}

	StdDeck_CardMask_OR(deadMask, plCards, boardMask);

	for (int hc1 = 0, ndx1326 = 0; hc1 < 51; hc1++)
	{
		for (int hc2 = hc1+1; hc2 < 52; hc2++, ndx1326++)	
		{
			StdDeck_CardMask_RESET(tempMask);
			StdDeck_CardMask card1 = StdDeck_MASK(StdDeck_MAKE_CARD(hc1%13, hc1/13));
			StdDeck_CardMask card2 = StdDeck_MASK(StdDeck_MAKE_CARD(hc2%13, hc2/13));
			StdDeck_CardMask_OR(tempMask, card1, card2);

			if (StdDeck_CardMask_ANY_SET(deadMask, tempMask))
				continue;

			StdDeck_CardMask_OR(tempMask, tempMask, boardMask);

			int hand_value = StdDeck_StdRules_EVAL_TYPE(deadMask, gVars.br+3);

			if (hand_value > board_value)
				awesome_ranges.push_back(ndx1326);
			else
				crap_ranges.push_back(ndx1326);
		}
	}

	for (int iPlayer = 0; iPlayer < 10; iPlayer++)
	{
		if (iPlayer != gVars.userchair && gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
		{
			if (gVars.currentbets[iPlayer] > 100 || gVars.LeftToInvest(iPlayer) < 0.7)
			{
				for (std::vector<int>::const_iterator i = crap_ranges.cbegin(); i != crap_ranges.cend(); ++i)
					gVars.HandLikelihood[iPlayer][*i] = 1;
			}
		}
	}
}


void COpponentsModel::calculate_opponents_handrange()
{
	nOpponents = 0;

	for (int iPlayer=0; iPlayer<10; iPlayer++)
	{
		if (iPlayer != gVars.userchair && gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
			nOpponents++;
	}

	rearrange_handrank(); // Get the order of the 1326 hands ordered by their PRWin
	
	// Lazy (and possibly unsafe) approach
	// Global HandLikelihood
	if (gVars.br == 2)
	{
		Get_Preflop_Ranges();
		Get_Flop_Ranges();
	}
	if (gVars.br == 3)
		Get_Turn_Ranges();
	if (gVars.br == 4)
		Get_River_Ranges();

	/*
	// Safe approach 
	// Local HandLikelihood

	if (gVars.br >= 1)
		Get_Preflop_Ranges();
	if (gVars.br >= 2)
		Get_Flop_Ranges();
	if (gVars.br >= 3)
		Get_Turn_Ranges();
	if (gVars.br == 4)
		Get_River_Ranges();
	*/

	//CompensateForAgression();
	//VerifyWeights();
}


#if defined(EXPERIMENTAL_DECISION)
void COpponentsModel::PredictReaction(int botAction)
{
	// PerceivedBotWeakness = Analyze_Betting_Pattern_And_Board_From_Oppenents_View
	// RoomToBluff = Analyze_Bots_Commitment_To_Pot
	// BoardBluffQuality

	for (int iPlayer = 0; iPlayer < 10; iPlayer++)
	{
		if(iPlayer == gVars.userchair || !gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
			continue;

		// OpponentPropensityToBet = Lookup_Opponent_Aggressiveness_Relative_To_Saw_Specific_Round
		// OpponentCommitment = Analyze_Opponents_Commitment_To_Pot

		for (int iHand1326=0; iHand1326<1326; iHand1326++)
		{
			if (HandLikelihood[iPlayer][iHand1326] < 1)
				continue;

			// OutsToNearNuts = CalculateOutsToNearNuts(iHand1326, Board) // For example 9 outs to a nut flush draw with unpaired board, 8 outs to Open Ended NUT Straight draw on a rainbow board
			// OutsToStrongHand = CalculateOutsToStrongHand(iHand1326, Board)
			// ChanceToHitOuts =  ( OutsToNearNuts + OutsToStrongHand ) / (bFlop ? 47 : (bTurn ? 46 : 0));

			HandStrengthCategory curHandStrength = HandStrength(gVars.br, gVars.Hand1326ToCardMask(iHand1326), gVars.curBoardMask);

			if (curHandStrength == hsMonster) 
			{
				// fold = 0.0;
			} 
			else if (curHandStrength == hsVeryStrong) 
			{
				// fold = 0.0;
			} 
			else if (curHandStrength == hsStrong) 
			{
				// fold = 0.0; // Unless big overbet?
			} 
			else if (curHandStrength == hsSomething) 
			{
				// Caught something weak, bottom pair etc.. 
			} 
			else 
			{
				// Air

				// bool bCanFloatBluff = (OpponentInPosition && nopponents==1);
				// BluffWithAir = 0;
				// if (RoomToBluff >= Some)
				//    BluffWithAir = CalculateBluffPercent(OpponentPropensityToBet, PerceivedBotWeakness, BoardBluffQuality);
				//
				// Raise	= BluffWithAir * (bCanFloatBluff ? 0.90 : 1);
				// if (botAction > 0.0) {
				//    Call	= BluffWithAir * (bCanFloatBluff ? 0.10 : 0);
				//    if (ChanceToHitOuts > PotOdds)
				//       Call	= 1.0 - Raise;
				//    else if (ChanceToHitOuts > PotOdds * 0.75) {
				//    }
				// } else
				//    Call	= 1.0 - Raise;
				// Fold	= 1.0 - (Raise + Call);
			}
		}
	}
}

#endif