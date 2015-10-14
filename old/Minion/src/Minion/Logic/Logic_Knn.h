#pragma once

#include <Windows.h>
#include "StructDefines/StructDefines.h"
#include "Util/Util_MagicNumbers.h"


class CKnnLogic
{

public:

	CKnnLogic();

public:

	eBetType		decision_bettype;
	ePlayerAction	actiontype;
	double			decision_amt;

public:

	// ANN stuff
	void	FillHandInfoStruct(int* hand);
	double	TranslateANNActionToAmount(eBetType action);

	// Used in Logic_knn.cpp
	void	GetDecision();
	void	RandomAction();
	void	SaveCurrentInfo(int* hand);
	int		SaveHandSituation(int* hand, HWND instance_hwnd);
	int		SaveProfit(int* profit, HWND instance_hwnd);

private:

	// previous
	int		prev_br;
	int		prev_dealer;
	int		prev_ntimesacted;
	double	prev_balance_init;

	int		current_hand_id;
	int		msg_buffer[eDim_Total];

	eBetType hero_actions[k_max_actions];
};
