#include "stdafx.h"

#include "Bot_HandHistory.h"
#include "Bot_GlobalVars.h"



CHandHistory::CHandHistory()
{
	NewHand();
}

void CHandHistory::NewHand()
{
	RecordedBotsAction = false;
	m_PrevDealerChair = m_iSBChair = m_iBBChair = -1;

	memset(&m_AmountToCall[0], 0, eRounds*sizeof(double));

	for (int i=0; i < k_MaxChairs; i++)
		m_PlayerActions[i].Clear();

	NewBettingRound();
}

void CHandHistory::NewBettingRound()
{
	RecordedBotsAction = false;

	if (gVars.br >= 1 && gVars.br <= 4) 
		gLog->WriteLog(eSeverityInfo, eCatHandHistory, g_sRounds[gVars.br]);

	m_PotRound = 0;
	m_nCallsRound = m_nChecksRound = m_nRaiseRound = 0;
}

void CHandHistory::RecordPlayerAction(int betround, int index, ePlayerAction Action, double Amount, double Balance)
{
	gLog->WriteLog(eSeverityInfo, eCatHandHistory, "Player: %s (%d) %s [%.2f]\n", gVars.CurState().m_player[index].m_name, index, sPlayerAction[Action], Amount);

	double facing_bet =  m_AmountToCall[betround] - m_PlayerActions[index].prevBet[betround];
	m_PlayerActions[index].AddAction(betround, Action, Amount, Balance, m_nCallsRound, m_nRaiseRound, m_nChecksRound, facing_bet, m_PotRound);

	if (Action == actionBetRaise)
	{
		m_AmountToCall[betround] = Amount;
		m_PlayerActions[index].prevBet[betround] = Amount;
		m_nRaiseRound++;
	}
	else if (Action == actionCall)
	{
		m_AmountToCall[betround] = Amount;
		m_PlayerActions[index].prevBet[betround] = Amount;
		m_nCallsRound++;
	}
	else if (Action == actionCheck)
	{
		m_nChecksRound++;
	}
	else if (Action == actionPostedSB)
	{
		m_AmountToCall[betround] = Amount;
		m_PlayerActions[index].prevBet[betround] = Amount;
	}
	else if (Action == actionPostedBB)
	{
		m_AmountToCall[betround] = Amount;
		m_PlayerActions[index].prevBet[betround] = Amount;
	}

	RecordedBotsAction = (index == gVars.userchair);
}

bool CHandHistory::WasPreflopBlindStealAttempt()
{
	for (int i = 0; i < k_MaxChairs; i++)
	{
		if (m_PlayerActions[i].preflopPosition == eBT)
			return (m_PlayerActions[i].GetPreflopAction(false) == eRaiseStealPos);
	}

	return false;
}
