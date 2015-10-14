#pragma once

#include "Calc/CEquityCalculator.h"
#include "Logic/Logic_Knn.h"

#include "poker_defs.h"

#include "StructDefines/structdefines.h"



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

public:

	eBetType betType;
	ePlayerAction actionType;
	double decision;

public:

	//Calculate Decision
	double FindDecision();

	//f$play Function
	int play();

private:

	CKnnLogic KnnLogic;

private:

	void VerifyDecision();
	void LogDecision();

};

extern CBotLogic botLogic;