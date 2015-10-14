#include "stdafx.h"

#include "Ranges.h"
#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"



void COpponentsModel::Get_River_Ranges()
{

	for (int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++)
	{
		if (iPlayer == gVars.userchair || !gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
			continue;

		ePostflopAction RiverAction = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].GetPostflopAction(eRiver);
		int nActions = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].total_actions[eRiver];

		if (nActions == 0 || RiverAction == eActionUnknown)
			continue;

		for (int j = 0; j < 1326; j++)
		{
			if (nActions == 1) 
			{
				//////////// CALL /////////////
				if (RiverAction == eActionCall) 
				{
					if(j >= 1326 * (gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_riverpct / gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_turnpct))
						R_Rank[iPlayer][j] *= 0.8;
				} 			
				//////////// BET /////////////
				else if (RiverAction == eActionBet) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggrpct)) //ratio -> pct..
						R_Rank[iPlayer][j] *= 0.3;
				}
				//////////// RAISE /////////////
				else if (RiverAction == eActionRaise) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggrpct)) //ratio -> pct..
						R_Rank[iPlayer][j] *= 0.2;
				}
			}
			else if (nActions > 1) 
			{
				//////////// CHECK-CALL /////////////
				if (RiverAction == eActionCheckCall) 
				{  
					/*
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggrpct)) //ratio -> pct..
						R_Rank[iPlayer][j] *= (0.3 * gVars.ReductionFactor(iPlayer));
					*/
				}
				//////////// BET-CALL /////////////
				else if (RiverAction == eActionBetCall) 
				{
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggrpct)) //ratio -> pct..
						R_Rank[iPlayer][j] *= 0.5;
				}
				//////////// CHECK-RAISE /////////////
				else if (RiverAction == eActionCheckRaise) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggrpct)) //ratio -> pct..
						R_Rank[iPlayer][j] *= 0.2;
				}
				//////////// BET-RAISE /////////////
				else if (RiverAction == eActionBetBet) 
				{  
					if(j >= 1326*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggrpct)) //ratio -> pct..
						R_Rank[iPlayer][j] *= 0.2;
				}
			}
		}

		for(int i = 0; i < 1326; i++)
		{
			int index = gVars.Rank_Reorganization[eRiver - 1][i];
			gVars.HandLikelihood[iPlayer][index] *= R_Rank[iPlayer][i];
		}
	}
}
	