#pragma once

#include "Util_MagicNumbers.h"
#include "Bot_PlayerState.h"

class CHandHistory
{
public:

	CHandHistory();

public:

	CPlayerState m_PlayerActions[k_MaxChairs];

	double m_AmountToCall[eRounds];
	double m_PotRound;
	int m_nCallsRound;
	int m_nChecksRound;
	int m_nRaiseRound;

	bool RecordedBotsAction;

	int m_iSBChair, m_iBBChair;
	int m_PrevDealerChair;

public:

	void RecordPlayerAction(int betround, int index, ePlayerAction Action, double Amount, double Balance);
	void NewHand();
	void NewBettingRound();

	bool WasPreflopBlindStealAttempt();
};