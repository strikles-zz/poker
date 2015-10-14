#pragma once

#include <ANN/ANN.h>

#include "CANN/CANN.h"
#include "DB/CDatabase.h"
#include "StructDefines/StructDefines.h"
#include "Util/Util_MagicNumbers.h"

#define MAX_NEIGHBOURS 200


class CDominvs
{
	// provide access to various decision objects to each client
public:
	CDominvs();

public:
	double		TranslateActionToAmount(int* hand, eBetType betType);
	eBetType	RandomAction(int* hand, bool just_raises, int current_bet);
	eBetType	ANNDecision(int* hand, CDatabase* p_db);
	eBetType	LWRDecision(int* hand, CDatabase* p_db);

private:
	eBetType		bettype;
	ePlayerAction	actionType;
};
