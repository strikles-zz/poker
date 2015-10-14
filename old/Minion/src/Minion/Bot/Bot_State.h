#pragma once

#include "Bot/Bot_DLL.h"
#include "Bot/Bot_HandHistory.h"

#include "Util/Util_MagicNumbers.h"


class CState
{

public:

	CState();

public:

	//CONSECUTIVE STATES
	unsigned char   m_ndx;
	holdem_state    m_holdem_state[256]; 
	CHandHistory	m_current_hand_info;

public:

	inline holdem_state& CurState() { return m_holdem_state[m_ndx]; }

	// Init
	bool	IsStateUsable(const holdem_state& a);
	bool	IsStateDifferent(const holdem_state& a, const holdem_state& b, bool bConsiderName);
	bool	WillProcessState(const holdem_state& pstate);
	void	DumpHoldemState(const holdem_state& curState, int index);

	// Extract
	void	ExtractState(const holdem_state& curState);
	void	ExtractBlinds();
	void	ExtractMissingActions();
	void	ExtractActions();
	void	ExtractContext();

	// Util
	int		FindPreviousActor(const int chair);
	int		FindFirstActor(const int betting_round);
	int		FindNextActor(const int chair);
	//void	SetCurrentActor(const bool is_heads_up);
	bool	InGame(const holdem_player& player);
	bool	CardsDealt(const holdem_state& curState);
	int		PlayersInGame();
	int		LeftToAct(int chair);

private:

	// State Variables
	bool	m_bNewGame;
	bool	m_bNewRound;
	int		m_CurActor;

	bool	is_heads_up;
	int		m_PrevBetround;
	int		m_PrevHandsPlayed;

	unsigned int	state_counter;

	enumStreets betround_by_cards;

	int ntimesacted;
};