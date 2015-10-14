#pragma once

#include "Logic_Strategies.h"
#include "Poker.PokerEval\include\poker_defs.h"

#include "EquityCalculator.h"

#include "Logic_Preflop.h"
#include "Logic_Postflop.h"

/////////////////////////////
// Decision values
/////////////////////////////

class CBotLogic
{
public:

	CBotLogic();
	~CBotLogic();

	// Rule of 3 :
	// Disabling Copy Constructor & assignement operator
	// Is this non-sense for a singleton ?
	CBotLogic(const CBotLogic&);
	CBotLogic& operator=(const CBotLogic&);


	//Calculate Decision
	double decision();
	//f$play Function
	int play();

private:

	StdDeck_CardMask lastPrwinCalc_playerMask, lastPrwinCalc_boardMask;
	double lastPrWinCalc_pot;
	int lastPrwinCalc_nopponents;
	unsigned short *prwinArray;

	EquityCalculator hc;
	//Preflop
	CPreflopLogic preflop;
	CPostflopLogic postflop;

	int strategyCount;
	CBaseStrategy **strategies;
	CBaseStrategy *activeStrategy;

	static CBotLogic* m_botlogic;

private:

	//Update Board Texture
	double update_board_texture();
};

extern CBotLogic botLogic;