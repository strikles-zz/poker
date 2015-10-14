#include "stdafx.h"

#include "Ranges.h"
#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"



void COpponentsModel::Get_Turn_Ranges()
{

	for (int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++)
	{
		if (iPlayer == gVars.userchair || !gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
			continue;

		ePostflopAction TurnAction = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].GetPostflopAction(eTurn);
		int nActions = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].total_actions[eTurn];

		if (nActions == 0 || TurnAction == eActionUnknown)
			continue;

		for (int j = 0; j < 1326; j++)
		{
			if (nActions == 1) 
			{
				//////////// CALL /////////////
				if (TurnAction == eActionCall) 
				{
					if(j >= 1326 * (gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_turnpct / (gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_floppct))) 
						T_Rank[iPlayer][j] *= 0.8;
				} 			
				//////////// BET /////////////
				else if (TurnAction == eActionBet) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggtpct)) //ratio -> pct..
						T_Rank[iPlayer][j] *= 0.3;
				}
				//////////// RAISE /////////////
				else if (TurnAction == eActionRaise) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggtpct)) //ratio -> pct..
						T_Rank[iPlayer][j] *= 0.2;
				}
			}
			else if (nActions > 1) 
			{
				//////////// CHECK-CALL /////////////
				if (TurnAction == eActionCheckCall) 
				{  
					/*
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggtpct)) //ratio -> pct..
						F_Rank[iPlayer][j] *= (0.3 * gVars.ReductionFactor(iPlayer));
					*/
				}
				//////////// BET-CALL /////////////
				else if (TurnAction == eActionBetCall) 
				{
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggtpct)) //ratio -> pct..
						T_Rank[iPlayer][j] *= 0.5;
				}
				//////////// CHECK-RAISE /////////////
				else if (TurnAction == eActionCheckRaise) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggtpct)) //ratio -> pct..
						T_Rank[iPlayer][j] *= 0.2;
				}
				//////////// BET-RAISE /////////////
				else if (TurnAction == eActionBetBet) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggtpct)) //ratio -> pct..
						T_Rank[iPlayer][j] *= 0.1;
				}
			}
		}

		for(int i = 0; i < 1326; i++)
		{
			int index = gVars.Rank_Reorganization[eTurn - 1][i];
			gVars.HandLikelihood[iPlayer][index] *= T_Rank[iPlayer][i];
		}
	}
}