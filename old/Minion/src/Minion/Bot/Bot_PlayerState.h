#pragma once

#include <string>

#include "Util/Util_MagicNumbers.h"

class CPlayerActionInfo
{

public:

	ePlayerAction	act;
	eBetType		betType;

	double call_amount;
	double raise_amount;

	int to_act;
	int facing_context;
};


class CPlayerState
{

public:

	CPlayerState();

public:

	ePreflopPosition preflopPosition;
	CPlayerActionInfo actions[eRoundIndices][k_MaxActionsPerRound];

	bool	bWasInGame;

	double	prevBet[eRoundIndices];
	double	balance[eRoundIndices];

	int		total_actions[eRoundIndices];

	// Dummy var for testing. Delete_Me :P
	std::string actions_this_round[eRoundIndices];

public:

	void SaveAction(enumStreetIndices br_ndx, ePlayerAction action, double to_call, double raise_amt, double pot);
	void ResetPlayerState(int br_ndx);

};