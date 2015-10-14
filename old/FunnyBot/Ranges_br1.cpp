#include "stdafx.h"

#include "Ranges.h"
#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"



void COpponentsModel::Get_Preflop_Ranges()
{
	//Preflop ranges
	bool bWasPreflopBlindStealAttempt = gVars.m_CurrentHandInfo.WasPreflopBlindStealAttempt();
	int ndx_playersdealt = 10 - gVars.get_nplayersdealt();

	if (ndx_playersdealt >= 0 && ndx_playersdealt <= k_MaxChairs)
	{
		for (int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++)
		{
			if(iPlayer != gVars.userchair && gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
			{
				ePreflopAction PreflopAction = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].GetPreflopAction(bWasPreflopBlindStealAttempt);

				int startRange = 0, endRange = 168;
				int secondaryStartRange = -1, secondaryEndRange= -1; 

				switch (PreflopAction)
				{
				case ePFActionUnknown:
					// Lets just use VPIP here 
					//endRange = 1326*gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip; 
					break; 

				case eLimp:
					if (gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].preflopPosition != eBB)
					{
						assert(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr < gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);

						startRange = static_cast<int>(169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
						endRange = static_cast<int>(169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					}
					break;

				case eLimpCall:
					if (gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr < gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip)
					{
						startRange = static_cast<int>(169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
						endRange = static_cast<int>(169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					}
					break;

				case eLimpRaise:
					// I have seen this a lot with big pocket JJ+
					endRange = static_cast<int>(0.5 * 169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
					break;

				case eRaise:
					endRange = static_cast<int>(0.5 * 169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr); 

					secondaryStartRange = endRange;
					secondaryEndRange = static_cast<int>(169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					break;

				case eRaiseStealPos:
					endRange = static_cast<int>(169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfats); 

					secondaryStartRange = endRange;
					secondaryEndRange = static_cast<int>(169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					break;

				case e3Bet:
					// Until we get 3 bet percentages
					endRange = static_cast<int>(0.25 * 169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr); 

					secondaryStartRange = endRange;
					secondaryEndRange = min(2 * endRange, 168); 
					break; 

				case e3BetSteal:
					// 3 bet from position
					endRange = static_cast<int>(0.25 * 169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr); 

					secondaryStartRange = endRange;
					secondaryEndRange = min(2 * endRange, 168); 
					break; 

				case eCallRaise:
					startRange = static_cast<int>(0.5 * 169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
					endRange = static_cast<int>(0.5 * 169 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					break;

				case e4Bet:
					break;
				}

				for(int i = 0; i < 1326; i++)
				{
					int ndx_169 = gVars.util1326.handranks_1326[ndx_playersdealt][i].second;
					int ndx_1326 = gVars.util1326.handranks_1326[ndx_playersdealt][i].first;

					if (ndx_169 >= startRange && ndx_169 <= endRange)					
						continue;

					else if (ndx_169 >= secondaryStartRange && ndx_169 <= secondaryEndRange)	
						P_Rank[iPlayer][i] *= 0.8;

					else										
						P_Rank[iPlayer][i] *= 0.3;
					
					gVars.HandLikelihood[iPlayer][ndx_1326] *= P_Rank[iPlayer][i];
				}
			}
		}
	}
}

/*
void COpponentsModel::Get_Preflop_Ranges()
{
	//Preflop ranges
	bool bWasPreflopBlindStealAttempt = gVars.m_CurrentHandInfo.WasPreflopBlindStealAttempt();
	int ndx_playersdealt = 10 - gVars.get_nplayersdealt();

	for (int iPlayer = 0; iPlayer < 10; iPlayer++)
	{
		if(iPlayer != gVars.userchair && gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
		{
			ePreflopAction PreflopAction = gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].GetPreflopAction(bWasPreflopBlindStealAttempt);

			int startRange = 0, endRange = 1325;
			int secondaryStartRange = -1, secondaryEndRange= -1; 

			switch (PreflopAction)
			{
				case ePFActionUnknown:
					// Lets just use VPIP here 
					//endRange = 1326*gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip; 
					break; 

				case eLimp:
					if (gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].preflopPosition != eBB)
					{
						assert(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr < gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);

						startRange = static_cast<int>(1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
						endRange = static_cast<int>(1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					}
					break;

				case eLimpCall:
					if (gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr < gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip)
					{
						startRange = static_cast<int>(1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
						endRange = static_cast<int>(1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					}
					break;

				case eLimpRaise:
					// I have seen this a lot with big pocket JJ+
					endRange = static_cast<int>(0.5 * 1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
					break;

				case eRaise:
					endRange = static_cast<int>(0.5 * 1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr); 

					secondaryStartRange = endRange;
					secondaryEndRange = static_cast<int>(1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					break;

				case eRaiseStealPos:
					endRange = static_cast<int>(1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfats); 

					secondaryStartRange = endRange;
					secondaryEndRange = static_cast<int>(1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					break;

				case e3Bet:
					// Until we get 3 bet percentages
					endRange = static_cast<int>(0.25 * 1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr); 

					secondaryStartRange = endRange;
					secondaryEndRange = min(2 * endRange, 1325); 
					break; 

				case e3BetSteal:
					// 3 bet from position
					endRange = static_cast<int>(0.25 * 1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr); 

					secondaryStartRange = endRange;
					secondaryEndRange = min(2 * endRange, 1325); 
					break; 

				case eCallRaise:
					startRange = static_cast<int>(0.5 * 1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_pfr);
					endRange = static_cast<int>(0.5 * 1326 * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_vpip);
					break;

				case e4Bet:
					break;
			}

			for(int i = 0; i < 1326; i++)
			{

				if (i >= startRange && i <= endRange)					
					continue;

				else if (i >= secondaryStartRange && i <= secondaryEndRange)	
					P_Rank[iPlayer][i] *= 0.8;

				else										
					P_Rank[iPlayer][i] *= 0.3;
			}

			if (ndx_playersdealt >= 0 && ndx_playersdealt <= 10)
			{
				for(int i = 0; i < 1326; i++)
				{
					int index = gVars.util1326.handranks_1326[ndx_playersdealt][i].first;
					gVars.HandLikelihood[iPlayer][index] *= P_Rank[iPlayer][i];
				}
			}
		}
	}
}
*/