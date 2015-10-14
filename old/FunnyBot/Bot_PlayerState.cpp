#include "stdafx.h"

#include "Bot_PlayerState.h"
#include "Util_MagicNumbers.h"



CPlayerState::CPlayerState()
{
	Clear();
}

void CPlayerState::Clear()
{

	bWasInGame = false;
	preflopPosition = ePosUnknown;

	for(int i = 0; i < 4; i++)
	{
		for (int j = 0; j < k_MaxActionsPerRound; j++)
		{
			actions[i][j].act = NoAction;

			actions[i][j].amount = 0.0;
			actions[i][j].balance = 0.0;
			actions[i][j].facing_bet = 0.0;

			actions[i][j].ncalls = 0;
			actions[i][j].nraises = 0;
			actions[i][j].nchecks = 0;

			actions[i][j].left_to_act = -1;
		}

		prevBet[i] = 0.0;
		total_actions[i] = 0;
		actions_this_round[i] = " .";
	}
}

void CPlayerState::AddAction(int betround, ePlayerAction action, double amount, double balance, int ncalls, int nraises, int nchecks, double facing_bet, double pot)
{
	for (int i = 0; i < k_MaxActionsPerRound; i++)
	{
		if (actions[betround][i].act == NoAction) 
		{
			actions[betround][i].act = action;

			actions[betround][i].amount = amount;
			actions[betround][i].balance = balance;
			actions[betround][i].facing_bet = facing_bet;

			actions[betround][i].ncalls = ncalls;
			actions[betround][i].nraises = nraises;
			actions[betround][i].nchecks = nchecks;

			total_actions[betround] = i+1;

			if (i == 0)
				actions_this_round[betround] = action_str[action];
			else
				actions_this_round[betround] += action_str[action];

			break;
		}
	}
}

ePreflopAction CPlayerState::GetPreflopAction(const bool bWasPreflopBlindStealAttempt)
{
	ePreflopAction ret = ePFActionUnknown;

	int nActions = 0, iFirstAction = 0;

	for (int i=0; i<k_MaxActionsPerRound; i++)
	{
		if (actions[ePreflop][i].act == NoAction || actions[ePreflop][i].act == actionFold)
			break;
		if (actions[ePreflop][i].act == actionPostedSB || actions[ePreflop][i].act == actionPostedBB)
			iFirstAction++;
		else
			nActions++;
	}


	if (nActions == 1) 
	{
		if (actions[ePreflop][iFirstAction].act == actionCall) 
		{
			if (bWasPreflopBlindStealAttempt && preflopPosition == eBB)
				ret = eBBDefenseCall;
			else
				ret = eCallRaise;
		} 

		else if (actions[ePreflop][iFirstAction].act == actionCheck)
			ret = eLimp;

		else if (actions[ePreflop][iFirstAction].act == actionBetRaise) 
		{
			if (actions[ePreflop][iFirstAction].nraises == 0) 
			{
				if (preflopPosition == eBB && bWasPreflopBlindStealAttempt)
					ret = eBBDefense3Bet;
				else if (preflopPosition == eBT)
					ret = eRaiseStealPos;
				else
					ret = eRaise;
			}

			else if (actions[ePreflop][iFirstAction].nraises == 1)
				ret = e3Bet;
			else if (actions[ePreflop][iFirstAction].nraises > 1)
				ret = e4Bet;

			else 
			{
				// error
			}
		}
	} 
	
	// Hmmm...
	// Must finish this
	else if (nActions > 1) 
	{
		if (actions[ePreflop][iFirstAction].act == actionCall) 
		{
			if (actions[ePreflop][iFirstAction+1].act == actionBetRaise)
				ret = eLimpRaise;
			else if (actions[ePreflop][iFirstAction+1].act == actionCall)
				ret = eLimpCall;

			else 
			{
				// error
			}
		}
	}

	return ret;
}

ePostflopAction CPlayerState::GetPostflopAction(enumStreets iRound)
{
	ePostflopAction ret = eActionUnknown;
	int nActions = total_actions[iRound];

	if (actions[iRound][nActions].act == NoAction || actions[iRound][nActions].act == actionFold)
		return ret;

	if (nActions == 1) 
	{
		if (actions[iRound][0].act == actionCall)
			ret = eActionCall;

		else if (actions[iRound][0].act == actionCheck)
			ret = eActionCheck;
		else if (actions[iRound][0].act == actionBetRaise) 
		{
			if (actions[iRound][0].nraises == 0)
				ret = eActionBet;
			else if (actions[iRound][0].nraises == 1)
				ret = eActionRaise;
			else if (actions[iRound][0].nraises > 1)
				ret = eActionRaise;
			else 
			{
				// error
			}
		}
	} 

	// Hmmm...
	// Must finish this
	else if (nActions > 1) 
	{
		if (actions[iRound][0].act == actionCheck) 
		{
			if (actions[iRound][1].act == actionBetRaise)
				ret = eActionCheckRaise;
			else if (actions[iRound][1].act == actionCall)
				ret = eActionCheckCall;

			else 
			{
				// error
			}
		} 

		else if (actions[iRound][0].act == actionCall) 
		{
			if (actions[iRound][1].act == actionBetRaise)
				ret = eActionCallRaise;
			else if (actions[iRound][1].act == actionCall)
				ret = eActionCallCall;

			else 
			{
				// error
			}
		} 

		else if (actions[iRound][0].act == actionBetRaise) 
		{
			if (actions[iRound][1].act == actionBetRaise)
				ret = eActionBetBet;
			else if (actions[iRound][1].act == actionCall)
				ret = eActionBetCall;

			else 
			{
				// error
			}
		}
	}

	return ret;
}