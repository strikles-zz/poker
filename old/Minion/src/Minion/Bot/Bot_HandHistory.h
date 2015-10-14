#pragma once

#include "Bot/Bot_PlayerState.h"
#include "Util/Util_MagicNumbers.h"


class CHandHistory
{

public:

	CHandHistory();

public:

	CPlayerState m_player_actions[k_max_chairs];

	double m_amount_to_call[eRoundIndices];
	double m_pot_round;

	bool was_bots_action_recorded;

	int sb_chair, bb_chair;
	int m_prev_dealer_chair;

public:

	void RecordPlayerAction(enumStreetIndices br_ndx, int chair, ePlayerAction action, double amount, bool isMissingAction=false);
	void NewHand();
	void NewBettingRound();

	bool WasPreflopBlindStealAttempt();
};