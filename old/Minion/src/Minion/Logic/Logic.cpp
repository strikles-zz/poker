/////////////////////////////////////////////////
/////////////// BOT LOGIC ///////////////////////
/////////////////////////////////////////////////

/////////////////////////////
// Includes
/////////////////////////////

#include "Logic/Logic.h"

#include "Bot/Bot_Globalvars.h"
#include "Util/Util_MagicNumbers.h"



CBotLogic::CBotLogic()
{
}

CBotLogic::~CBotLogic()
{
}



double CBotLogic::FindDecision()
{
	if(!gVars.get_ismyturn())
		return decision;

	betType		= eBet_Unknown;
	actionType	= eAction_Unknown;
	decision	= 0.0;

	KnnLogic.GetDecision();

	betType		= KnnLogic.decision_bettype;
	actionType	= KnnLogic.actiontype;
	decision	= ToMoney(KnnLogic.decision_amt);

	VerifyDecision();
	LogDecision();

	return decision;
}

void CBotLogic::VerifyDecision()
{
	decision = ToMoney(decision);

	double min_bet = ToMoney(gVars.get_bblind());
	if(eCall < betType)
	{
		if(!gVars.isActionAvailable(k_raise) ||											// should take care of most impossible raise problems
			min_bet > decision ||														// bet is too small
			(decision > gVars.call && gVars.call > gVars.balance[gVars.userchair]))		// should be redundant and handled by 1st case
		{
			decision	= gVars.call;
			actionType	= eAction_C;
			betType		= eCall;

			if(min_bet > decision)
				gLog.WriteLog(eSeverityDebug, eCatANN, "Decision < MinBet - Correcting to Call (%.2f)\n", decision);
			else if(!gVars.isActionAvailable(k_raise))
				gLog.WriteLog(eSeverityDebug, eCatANN, "No Raise Btn - Correcting Decision to Call (%.2f)\n", decision);
			else
				gLog.WriteLog(eSeverityError, eCatANN, "Impossible Raise - Correcting Decision to Call (%.2f)\n", decision);
		}
		else if(decision > gVars.balance[gVars.userchair] && decision > gVars.call)
		{
				decision	= gVars.call;
				actionType	= eAction_C;
				betType		= eCall;

				gLog.WriteLog(eSeverityDebug, eCatANN, "Impossible Raise - Correcting Decision to Call (%.2f)\n", decision);
		}
	}
	if(eCall == betType && IsEqual(0, decision))
	{
		decision	= 0.0;
		actionType	= eAction_X;

		gLog.WriteLog(eSeverityDebug, eCatANN, "Correcting to Check (%.2f)\n", decision);
	}
	if(eAction_X == actionType && 0 < gVars.call)
	{
		decision	= 0.0;
		actionType	= eAction_F;

		gLog.WriteLog(eSeverityError, eCatANN, "Check is impossible, will likely Fold ?\n", decision);
	}
}


void CBotLogic::LogDecision()
{
	/*
	if(decision < 0)
	{
		gLog.WriteLog(eSeverityError, eCatDecision, "Decision is negative : %.2f\n", decision);
		decision = 0.0;
	}
	*/

	gLog.WriteLog(eSeverityInfo, eCatInfo, "\n");

	if(eAction_X == actionType)
		gLog.WriteLog(eSeverityInfo, eCatDecision, ">>> CHECK : %.2f [call = %.2f]\n", decision, gVars.call);
	else if(eAction_C == actionType)
		gLog.WriteLog(eSeverityInfo, eCatDecision, ">>> CALL : %.2f [call = %.2f]\n", decision, gVars.call);
	else if(eAction_R == actionType)
		gLog.WriteLog(eSeverityInfo, eCatDecision, ">>> RAISE : %.2f [call = %.2f]\n", decision, gVars.call);
	else
		gLog.WriteLog(eSeverityInfo, eCatDecision, ">>> CHECK/FOLD : %.2f [call = %.2f]\n", decision, gVars.call);

	gLog.WriteLog(eSeverityInfo, eCatInfo, "\n");

}