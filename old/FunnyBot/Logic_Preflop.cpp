/////////////////////////////////////////////////
//////////////// PREFLOP ////////////////////////
/////////////////////////////////////////////////

/////////////////////////////
// Includes
/////////////////////////////

#include "stdafx.h"
#include "Logic_Preflop.h"
#include "Bot_Globalvars.h"



CPreflopLogic::CPreflopLogic()
{
}

double CPreflopLogic::PreflopDecision()
{
	/*
	*  preflop function to take decision. This function is explained in Preflop Diagram
	*
	* Return:
	* -All-in: -1
	* -Raise: 0.0001 > Amount < balance
	* -Call: 0.0001
	* -Check: -2
	* -Fold: -3
	* -Error: -3
	*/

	/////////////////////////////////////////////
	// Check if we are playing correct strategy (BSS NL 6)
	/////////////////////////////////////////////

	//checkPreflopCorrectStrategy();

	ePlayerAction our_1st_action = gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][0].act;
	ePlayerAction our_2nd_action = gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][1].act;
	ePlayerAction our_3rd_action = gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][2].act;
	ePlayerAction our_4th_action = gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][3].act;

	if (gVars.is_headsup())
		return preflop_HeadsUp();

	/*
	if (gVars.get_nplayersdealt() == 3)
		return preflop_ThreeHanded();
	*/

	//NO ACTION BEFORE
	if (our_1st_action == NoAction ||
		(our_1st_action == actionPostedSB && our_2nd_action == NoAction) ||
		(our_1st_action == actionPostedBB && our_2nd_action == NoAction))
	{ 
		return preflop_FirstAction();
	}
	
	//I RAISED BEFORE
	else if	((our_1st_action == actionBetRaise && our_2nd_action == NoAction) ||
			(our_1st_action == actionPostedSB && our_2nd_action == actionBetRaise && our_3rd_action == NoAction) ||
			(our_1st_action == actionPostedBB && our_2nd_action == actionBetRaise && our_3rd_action == NoAction))
	{ 
		return preflop_RaisedBefore();
	}
	
	//I CALLED BEFORE (sure we did call for set mining and we receive a raise) NO ACTIONS LATER
	else if ((our_1st_action == actionCall && our_2nd_action == NoAction) ||
			(our_1st_action == actionPostedSB && our_2nd_action == actionCall && our_3rd_action == NoAction) ||
			(our_1st_action == actionPostedBB && our_2nd_action == actionCall && our_3rd_action == NoAction))
	{ 
		return preflop_CalledBefore();
	}
	
	else
	{
		////////////////////////////////////////////
		// Error Action Zone --> Automatically preflopErrorStrategy (usually fold)
		/////////////////////////////////////////////

		if((our_1st_action == actionCheck && our_2nd_action == NoAction))
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: we cant check if after we dont post blinds or something similar\n");
			return preflopErrorStrategy();

		}

		if(our_1st_action == actionPostedBB && our_2nd_action == actionCheck && our_3rd_action == NoAction)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: if we posted BB and we did check, error if we have to act in preflop now\n");
			return preflopErrorStrategy();

		}

		if(our_1st_action == actionPostedSB && our_2nd_action == actionCheck && our_3rd_action == NoAction)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: if we posted SB, we cant do check (only raise, call or fold)\n");
			return preflopErrorStrategy();

		}

		//Search for actionFold, if it is founded --> Error (we cant do fold in the same hand and in the next time be playing)
		for(int i = 0 ; i < k_MaxActionsPerRound; i++)
		{
			if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act == actionFold)
			{
				gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: we cant do fold in the same hand and in the same hand still playing\n");
				return preflopErrorStrategy();
			}
		}

		///////////////////////////////////
		//UNDEFINED ACTION BEFORE
		///////////////////////////////////

		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Undefined Action Before. Recognize Action Before:\n");

		//Recognize action before
		for(int i = 0 ; i < k_MaxActionsPerRound; i++)
		{
			if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act != NoAction)
			{
				//Logging Actions
				if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act == actionPostedSB)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Action number: %d. Posted Small Blind.\n",i);
				}
				if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act == actionPostedBB)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Action number: %d. Posted Big Blind.\n",i);
				}
				if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act == actionCall)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Action number: %d. Call.\n",i);
				}
				if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act == actionBetRaise)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Action number: %d. Bet/Raise. Raise amount: %.2f\n",i,gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].amount);
				}
				if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act == actionCheck)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Action number: %d. Check.\n",i);
				}
				if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][i].act == actionFold)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Action number: %d. Fold.\n",i);
				}
			}
		}

		//undefined strategy
		gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: Strategy not defined. Automatically Fold/Check...\n");
		return preflopErrorStrategy();
	}
}


double CPreflopLogic::raise_preflop_reraise_amount()
{
	/*
	*  return preflop reraise amount - whichever is higher of 3.5 * call amount or the pot
	*/

	if(3.5 * gVars.call > gVars.get_pot())
	{
		return doRaise(3.5f * gVars.call);
	}

	return doRaise(gVars.get_pot());
}

double CPreflopLogic::doRaise(double raiseAmmount)
{
	/*
	*  return preflop raise correct amount
	*  c_error: error
	*/

	//check if raiseAmmount is correct
	if(raiseAmmount < 0)
	{
		gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: Incorrect raiseAmmount < 0 (raiseAmmount = %.2f)\n", raiseAmmount);
		return c_error;
	}

	//delete this for test
	//return raiseAmmount;

	//Raise >= balance, so go all-in
	if(raiseAmmount >= gVars.balance)
	{ 
		//Check myturnbits
		/*
		*	-F(0) Fold
		*	-C(1) Call
		*	-R(2) Raise
		*	-A(3) All-in
		*	-K(4) Check
		*/

		//check all-in
		if(gVars.isButtonAvailable(3))
		{ 
			return c_allin;
		}

		//possible swag (to make sure you can swag, balance has to be > amount to call*2...)
		//check swag
		if(gVars.balance > 2 * gVars.call)
		{ 
			return gVars.balance;
		}

		//check call
		if(gVars.isButtonAvailable(1))
		{ 
			return c_call;
		}

		//check "check"
		if(gVars.isButtonAvailable(4))
		{ 
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: if we are raising, why we are checking?\n");//
			return c_check;
		}

		//check fold
		if(gVars.isButtonAvailable(0))
		{ 
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: if we are raising, why we are folding?\n");
			return c_fold;
		}

		gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: No button available.\n");
		return c_error;
	}

	//Round raiseAmmount
	return raiseAmmount;
}

void CPreflopLogic::checkPreflopCorrectStrategy()
{
	//CHECK IF IM PLAYING CORRECT STRATEGY

	// MAX players 6 (5 opp + me)
	if(gVars.get_nplayersdealt() > 6) 
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "INCORRECT STRATEGY: Number of players dealt is > 6. nplayersdealt: %d\n", gVars.get_nplayersdealt());
	}

	// MIN players 2 (me + 1 opp)
	if(gVars.get_nplayersdealt() <= 1)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "INCORRECT STRATEGY: Number of players dealt is <= 1. nplayersdealt: %d\n", gVars.get_nplayersdealt());
	}

	// (MY BALANCE+CURRENTBET)/bblind ~= 100*bblind
	if( (gVars.balance+gVars.currentbet)/gVars.get_bblind() < 80 )
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "INCORRECT STRATEGY: Balance: %.2f Currentbet: %.2f BBlind: %.2f (Balance+Currentbet)/bblind =%.2f  < 80\n", gVars.balance,gVars.currentbet,gVars.get_bblind(),(gVars.balance+gVars.currentbet)/gVars.get_bblind());
	}

	if( (gVars.balance+gVars.currentbet)/gVars.get_bblind() > 120 )
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "INCORRECT STRATEGY: Balance: %.2f Currentbet: %.2f BBlind: %.2f (Balance+Currentbet)/bblind =%.2f  > 120\n", gVars.balance,gVars.currentbet,gVars.get_bblind(),(gVars.balance+gVars.currentbet)/gVars.get_bblind());
	}

	//istournament?
	if(gVars.get_istournament())
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "INCORRECT STRATEGY: This is a Tournament (istournament==true)\n");
	}

	//lim == 0 (NL)
	if(static_cast<int>(gws("lim")) != 0)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "INCORRECT STRATEGY: Different Limit. We are playing FL or PL (lim != 0)\n");
	}

	//there not should be ante in cash game (there are cash who have ante, little cash games)
	if(gws("ante") > 0.001)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "INCORRECT STRATEGY: There are antes. This is a tournament?\n");
	}
}

double CPreflopLogic::preflopErrorStrategy()
{
	/*
	* 
	*/
	if(gVars.isHand("AA KK"))
	{
		return c_allin;
	}

	if(gVars.currentbet > 0.75f * (gVars.balance + gVars.currentbet))
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "We are commited so go allin...\n");
		return c_allin;
	}

	return c_fold;
}



