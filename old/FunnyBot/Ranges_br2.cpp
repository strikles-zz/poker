#include "stdafx.h"

#include "Ranges.h"
#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"



void COpponentsModel::Get_Flop_Ranges()
{

	/*allchairs:
		
	things taken into consideration:
	-action vs stats
	
	things needed to be added:
	-position
	-bet ammount
	-board texture
	-(even later: opponent rating -> what level do we think he's playing?)
	*/

	for (int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++) 
	{
		if (iPlayer == gVars.userchair || (!gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame))
			continue;

		ePostflopAction FlopAction = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].GetPostflopAction(eFlop);
		int nActions = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].total_actions[eFlop];

		if (nActions == 0 || FlopAction == eActionUnknown)
			continue;

		for (int j = 0; j < 1326; j++)
		{
			if (nActions == 1) 
			{
				//////////// CALL /////////////
				if (FlopAction == eActionCall) 
				{
					if(j >= 1326 * (gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_floppct))
						F_Rank[iPlayer][j] *= 0.8;
				} 			
				//////////// BET /////////////
				else if (FlopAction == eActionBet) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggfpct)) //ratio -> pct..
						F_Rank[iPlayer][j] *= 0.3;
				}
				//////////// RAISE /////////////
				else if (FlopAction == eActionRaise) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggfpct)) //ratio -> pct..
						F_Rank[iPlayer][j] *= 0.2;
				}
			}
			else if (nActions > 1) 
			{
				//////////// CHECK-CALL /////////////
				if (FlopAction == eActionCheckCall) 
				{  
					/*
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggfpct)) //ratio -> pct..
						F_Rank[iPlayer][j] *= (0.3 * gVars.ReductionFactor(iPlayer));
					*/
				}
				//////////// BET-CALL /////////////
				else if (FlopAction == eActionBetCall) 
				{
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggfpct)) //ratio -> pct..
						F_Rank[iPlayer][j] *= 0.5;
				}
				//////////// CHECK-RAISE /////////////
				else if (FlopAction == eActionCheckRaise) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggfpct)) //ratio -> pct..
						F_Rank[iPlayer][j] *= 0.2;
				}
				//////////// BET-RAISE /////////////
				else if (FlopAction == eActionBetBet) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggfpct)) //ratio -> pct..
						F_Rank[iPlayer][j] *= 0.1;
				}
			}
		}

		for(int i = 0; i < 1326; i++)
		{
			int index = gVars.Rank_Reorganization[eFlop - 1][i];
			gVars.HandLikelihood[iPlayer][index] *= F_Rank[iPlayer][i];
		}
	}
}