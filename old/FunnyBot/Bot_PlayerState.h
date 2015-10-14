#pragma once

#include <string>

#include "Util_MagicNumbers.h"

class CPlayerActionInfo
{
public:

	ePlayerAction act;
	double amount;
	double balance;

	int ncalls, nraises, nchecks;
	double facing_bet;

	int left_to_act;
	int facing_context;
};


class CPlayerState
{
public:

	CPlayerState();

public:

	bool bWasInGame;
	double prevBet[eRounds];
	ePreflopPosition preflopPosition;
	CPlayerActionInfo actions[eRounds][k_MaxActionsPerRound];
	int total_actions[eRounds];
	// Dummy var for testing. Delete_Me :P
	std::string actions_this_round[eRounds];

public:

	void AddAction(int betround, ePlayerAction action, double amount, double balance, int ncalls, int nraises, int nchecks, double facing_bet, double pot);
	void Clear();

	ePreflopAction GetPreflopAction(const bool bStealAttempt);
	ePostflopAction GetPostflopAction(enumStreets iRound);
};