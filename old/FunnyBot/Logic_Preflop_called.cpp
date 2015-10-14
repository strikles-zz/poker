#include "stdafx.h"

#include "Logic_Preflop.h"
#include "Bot_Globalvars.h"



double CPreflopLogic::preflop_CalledBefore()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "I Called Before\n");

	/////////////////////////////////////////////
	//We limped bblind
	/////////////////////////////////////////////

	if(gVars.currentbet/gVars.get_bblind() == 1)
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We limped bblind (Amount == 1*bblind)\n");

		/*
			if(size_of_raise == 2*bblind){ //min-raise (1bblind to call)
						
			}
		*/

		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "WARNING: Strategy not defined. Automatically Fold/Check...\n");
		return c_fold;
	}

	/////////////////////////////////////////////
	//We called a minraise
	/////////////////////////////////////////////

	if(gVars.currentbet/gVars.get_bblind() == 2)
	{	
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We called a min-raise (Amount == 2*bblind)\n");
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "WARNING: Strategy not defined. Automatically Fold/Check...\n");
		return c_fold;
	}

	/////////////////////////////////////////////
	//We called a standard raise
	/////////////////////////////////////////////

	if(gVars.currentbet/gVars.get_bblind() > 2 && gVars.currentbet/gVars.get_bblind() <= 5)
	{			
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We called standard raise (2*bblind < Amount <= 5*bblind)\n");
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "WARNING: Strategy not defined. Automatically Fold/Check...\n");
		return c_fold;
	}
			
	/////////////////////////////////////////////
	//We called a big raise (raise > 5*bblind)
	/////////////////////////////////////////////

	if(gVars.currentbet/gVars.get_bblind() > 5)
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We called big raise (Amount > 5*bblind)\n");

		//we go allin if we are commited
		if(gVars.currentbet > 0.75f * (gVars.balance + gVars.currentbet))
		{
			gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We are commited so go allin...\n");
			return c_allin;
		}

		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "WARNING: Strategy not defined. Automatically Fold/Check...\n");
		return c_fold;
	}

	//undefined strategy
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "WARNING: Strategy not defined. Automatically Fold/Check...\n");
	return c_fold;
}