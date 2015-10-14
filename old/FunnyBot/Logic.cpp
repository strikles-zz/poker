/////////////////////////////////////////////////
/////////////// BOT LOGIC ///////////////////////
/////////////////////////////////////////////////

/////////////////////////////
// Includes
/////////////////////////////

#include "stdafx.h"

#include <time.h>

#include "Logic.h"
#include "Bot_Globalvars.h"
#include "Util_MagicNumbers.h"

#include "Ranges.h"
#include "Oracle_Actions.h"
#include "Logic_Strategies.h"

#include "struct_defines.h"
#include "CANNSearch.h"

#include "Poker.PokerEval\include\poker_defs.h"
#include "Poker.PokerEval\include\rules_std.h"
#include "Poker.PokerEval\include\deck_std.h"
#include "Poker.PokerEval\include\inlines\eval.h"
#include "Poker.PokerEval\include\inlines\eval_type.h"


CBotLogic::CBotLogic() :

	lastPrWinCalc_pot(0),
	lastPrwinCalc_nopponents(0),
	strategyCount(1),
	strategies(new CBaseStrategy* [strategyCount]),
	activeStrategy (NULL)

{
	strategies[0] = new CStealBlinds();
	StdDeck_CardMask_RESET(lastPrwinCalc_playerMask);
	StdDeck_CardMask_RESET(lastPrwinCalc_boardMask);
}

CBotLogic::~CBotLogic()
{
	delete [] strategies; 
	//delete m_botlogic;
}

double CBotLogic::decision()
{

	if (!gVars.get_ismyturn())
	{
		// Why are we returning a decision if it's not our turn ?
		return gVars.decision;

		//return 0;
	}

	/*
	// PostFlop Calculations
	if (gVars.br >= 2) 
	{
		StdDeck_CardMask plCards, boardMask, deadMask;
		StdDeck_CardMask_RESET(deadMask);
		StdDeck_CardMask_RESET(boardMask);
		StdDeck_CardMask_RESET(plCards);
		StdDeck_CardMask card;

		for (int i=0; i<2; i++)
		{
			card = StdDeck_MASK(StdDeck_MAKE_CARD((gVars.Bot_Common_Cards[i][0]-2), ConvertSuitsForPokerEval[gVars.Bot_Common_Cards[i][1]]));
			StdDeck_CardMask_OR(plCards, plCards, card);
		}

		for (int i=0; i<(1+gVars.br); i++)
		{
			card = StdDeck_MASK(StdDeck_MAKE_CARD((gVars.Bot_Common_Cards[2+i][0]-2), ConvertSuitsForPokerEval[gVars.Bot_Common_Cards[2+i][1]]));
			StdDeck_CardMask_OR(boardMask, boardMask, card);
		}

		StdDeck_CardMask_OR(deadMask, plCards, boardMask);

		if (!StdDeck_CardMask_EQUAL(lastPrwinCalc_playerMask, plCards) || 
			!StdDeck_CardMask_EQUAL(lastPrwinCalc_boardMask, boardMask) || 
			lastPrWinCalc_pot != gVars.get_pot() || 
			lastPrwinCalc_nopponents != gVars.nopponents)
		{
			////////////////////
			// Calculating Opponent Hand Range
			////////////////////
			gLog->WriteLog(eSeverityInfo, eCatPerformance, "Start: Calculating opponent hand range\n");
			COpponentsModel* opponents = new COpponentsModel();
			//opponents->calculate_opponents_handrange_simple();
			opponents->calculate_opponents_handrange();
			gLog->WriteLog(eSeverityInfo, eCatPerformance, "End: Calculating opponent hand range\n");

			int iOpp = 0;
			short blank[1326] = {0};
			short* opponentHandRange[k_MaxChairs] = { NULL };

			for (int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++)
			{
				if (iPlayer == gVars.userchair)
					continue;

				// If player is not hero and is playing add entry to opponentHandRange array 
				if(gVars.InGame(gVars.CurState().m_player[iPlayer]))
				{
					opponentHandRange[iPlayer] = opponents->opponent_handrange(iPlayer);

					// Verify Handranges
					if (memcmp(blank, opponentHandRange[iPlayer], 1326*sizeof(blank[0]))==0) 
					{
						// seat x wasn't in the hand but is now in the hand ?
						gLog->WriteLog(eSeverityError, eCatDebugNow, "ERROR: Seat %d has an empty hand range\n", iPlayer);
						opponentHandRange[iPlayer] = opponents->empty_handrange();
					}

					iOpp++;
				}
			}

			if (iOpp) 
			{
				////////////////////
				// Calculating Handrank
				////////////////////
				memset(&gVars.outResults[0], 0, k_MaxChairs*sizeof(double));

				gLog->WriteLog(eSeverityInfo, eCatPerformance, "Start: Calculating handrank\n");

				clock_t timer = clock();		
				hc.CalculateEquity
				(
					k_iterations, 
					plCards, 
					boardMask, 
					gVars.outResults, 
					opponentHandRange[0], 
					opponentHandRange[1], 
					opponentHandRange[2], 
					opponentHandRange[3], 
					opponentHandRange[4], 
					opponentHandRange[5], 
					opponentHandRange[6], 
					opponentHandRange[7], 
					opponentHandRange[8], 
					opponentHandRange[9]
				);				
				timer = clock() - timer;  // end the timer

				gLog->WriteLog(eSeverityInfo, eCatPerformance, "End: Calculating handrank (%ld ms)\n", timer);
				gLog->WriteLog(eSeverityInfo, eCatInfo, "\n");

				char PlayerCards[128];
				char BoardCards[128];
				StdDeck_maskToString(plCards, PlayerCards);
				StdDeck_maskToString(boardMask, BoardCards);

				int hand_value = StdDeck_StdRules_EVAL_TYPE(deadMask, gVars.br+3);
				
				if (hand_value > 2)
					gLog->WriteLog(eSeverityInfo, eCatDebugNow, "Cards : [%s] %s - %s\n", PlayerCards, BoardCards, StdRules_handTypeNames[hand_value]);
				else
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Cards : [%s] %s - %s\n", PlayerCards, BoardCards, StdRules_handTypeNames[hand_value]);

				gLog->WriteLog(eSeverityInfo, eCatInfo, "\n");

				for (int j = 0; j < k_MaxChairs; j++)
				{
					int chair = (gVars.userchair + j) % k_MaxChairs;

					if (chair == gVars.userchair)
					{
						gVars.prw_xpoker_equity = gVars.outResults[chair];

						if (gVars.prw_xpoker_equity > (1.0 / (gVars.nopponentsplaying + 1)))
							gLog->WriteLog(eSeverityInfo, eCatEquityHeroPositive, "Equity Hero  = %.2f\n", gVars.prw_xpoker_equity*100);
						else
							gLog->WriteLog(eSeverityInfo, eCatEquityHeroNegative, "Equity Hero  = %.2f\n", gVars.prw_xpoker_equity*100);
					}
					else if (gVars.InGame(gVars.CurState().m_player[chair]))
					{
						gLog->WriteLog(eSeverityInfo, eCatEquityOpp, "Equity Opp_%d = %.2f\n", chair, gVars.outResults[chair]*100);
					}
				}

				StdDeck_CardMask_RESET(lastPrwinCalc_playerMask);
				StdDeck_CardMask_OR(lastPrwinCalc_playerMask, lastPrwinCalc_playerMask, plCards);
				StdDeck_CardMask_RESET(lastPrwinCalc_boardMask);
				StdDeck_CardMask_OR(lastPrwinCalc_boardMask, lastPrwinCalc_boardMask, boardMask);

				lastPrWinCalc_pot = gVars.get_pot();
				lastPrwinCalc_nopponents = gVars.nopponents;

				////////////////////
				// Predict Opponent Actions
				////////////////////
				//CDecisionPrediction predict;
				predict.prediction();
			} 
			
			else 
				gLog->WriteLog(eSeveritySevere, eCatGeneral, "ERROR: It looks like all opponents have folded. Scrape Error?\n");

			delete opponents;
		}
	}
	*/

	handInfo br1_test;
	br1_test.br = ePreflop;
	br1_test.timesActed = 2;
	br1_test.action[0] = eQuarterPot;
	br1_test.action[1] = eCall;
	br1_test.hand169 = 76;
	br1_test.playersInHand = 5;
	br1_test.playersToAct = 4;
	br1_test.stackCommitment = 2;
	br1_test.call = 25;
	br1_test.pot = 100;
	gLog->WriteLog(eSeverityInfo, eCatEquityOpp, "knn decision = %d\n", gKnn.GetDecision(br1_test));

	// See if we have profitable predefined strategies that fit our situation
	bool bFollowStrategy = false;

	/*
	if (activeStrategy) 
	{
		if (activeStrategy->ContinueStrategy(&gVars)) 
		{
			gVars.decision = activeStrategy->StrategyAction(&gVars);
			bFollowStrategy = true;
		} 
		else 
			activeStrategy = NULL;
	}

	if (!bFollowStrategy) 
	{
		for (int i=0; i<strategyCount; i++)
		{
			if (strategies[i] && strategies[i]->PrerequisiteMet(&gVars)) 
			{
				activeStrategy = strategies[i];
				gVars.decision = activeStrategy->StrategyAction(&gVars);
				bFollowStrategy = true;
			}
		}
	}
	*/

	if (!bFollowStrategy) 
	{
		gVars.decision = 0;
		gVars.action_type = c_fold;

		gLog->WriteLog(eSeverityInfo, eCatInfo, "\n");

		// Take Decision
		if (gVars.br == 1) 
		{
			gVars.decision = preflop.PreflopDecision();
			gVars.israis = false;

			//Preflop all-in
			if (gVars.decision >= gVars.balance || gVars.decision == c_allin)
			{ 
				// Returning -1 for all-in is not handled by RecordPlayerAction() 
				// depends on swagtextmethod
				gVars.decision = gVars.balance;


				gVars.israis = true;
				gLog->WriteLog(eSeverityInfo, eCatDebug, "Preflop All-in...[%.2f]\n", gVars.balance);
			}

			//preflop raisammounts are static
			else if(gVars.decision > gVars.get_bblind())
			{ 
				gVars.israis = true; 
				gLog->WriteLog(eSeverityInfo, eCatDebug, "Preflop Raise...[%.2f]\n", gVars.decision);
			}
			
			//Call
			else if(gVars.decision == c_call)
			{
				gVars.decision = call_value;
				gLog->WriteLog(eSeverityInfo, eCatDebug, "Preflop Call...[%.2f]\n", gVars.call);
			}
			
			//Check
			else if(gVars.decision == c_check)
			{ 
				gVars.decision = 0;

				//check
				if (gVars.isButtonAvailable(4)) 
				{ 
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Preflop Check...[0]\n");
				} 
				//fold
				else if(gVars.isButtonAvailable(0)) 
				{ 
					gLog->WriteLog(eSeverityInfo, eCatDebug, "ERROR: Bot think he can do check, but we do Preflop Fold...\n");
				}
			}
			
			//Fold
			else if(gVars.decision == c_fold)
			{ 
				//check
				gVars.decision = 0;

				if(gVars.isButtonAvailable(4))
				{ 
					gLog->WriteLog(eSeverityInfo, eCatDebug, "ERROR: Bot think he cannot do check, but we do Preflop Check...\n");
				}
				//fold
				else if(gVars.isButtonAvailable(0))
				{ 
					gLog->WriteLog(eSeverityInfo, eCatDebug, "Preflop Fold...[0]\n");
				}
			}
			
			else
			{
				gLog->WriteLog(eSeverityInfo, eCatDebug, "+++ What to do? +++...\n");
			}
		} 
		
		else 
		{

			/* strikles test stuff
			if (gVars.br > 1)
			{
				//gVars.action_type = postflop.postflop_utility();
				gVars.action_type = postflop.postflop_EV();
			}
			*/

			// original stuff
			if (gVars.br == 2) 
				gVars.action_type = postflop.FlopDecision();

			else if (gVars.br == 3) 
				gVars.action_type = postflop.TurnDecision();

			else if (gVars.br == 4) 
				gVars.action_type = postflop.RiverDecision();

			gVars.israis = false;

			if (gVars.action_type == c_call) 
				gVars.decision = call_value;

			else if(gVars.action_type == c_raise) 
			{
				gVars.israis = true;

				gVars.decision = postflop.raisamount();

				if (gVars.decision == 0)
					gVars.decision = call_value;
			}
		}
	}

	/*
	if (gVars.decision < 0)
	{
		gLog->WriteLog(eSeverityError, eCatDebug, "Decision is negative : %.2f\n", gVars.decision);
		gVars.decision = 0;
	}
	*/

	gLog->WriteLog(eSeverityInfo, eCatInfo, "\n");

	if (gVars.decision == call_value)
	{
		if (gVars.israis == true)
			gLog->WriteLog(eSeverityInfo, eCatDecision, ">>> CHECK/RAISE : %.2f [call = %.2f]\n", gVars.decision, gVars.call);

		else if (gVars.call == 0)
			gLog->WriteLog(eSeverityInfo, eCatDecision, ">>> CHECK : %.2f [call = %.2f]\n", gVars.decision, gVars.call);

		else
			gLog->WriteLog(eSeverityInfo, eCatDecision, ">>> CALL : %.2f [call = %.2f]\n", gVars.decision, gVars.call);
	}

	else if (gVars.decision > call_value)
	{
		// depends on swagtextmethod
		gLog->WriteLog(eSeverityInfo, eCatDecision, ">>> RAISE : %.2f [call = %.2f]\n", gVars.decision, gVars.call);
	}
	else
		gLog->WriteLog(eSeverityInfo, eCatDecision, ">>> CHECK/FOLD : %.2f [call = %.2f]\n", gVars.decision, gVars.call);

	gLog->WriteLog(eSeverityInfo, eCatInfo, "\n");

	return gVars.decision;
}


int CBotLogic::play()
{
	/*
	* return how the autoplayer interact with the sitin, sitout and leave buttons as defined in the TM:
	*  -2, Leave Table
	*  -1, No change (if you are sitting out, stay out, if sitting in stay that way)
	*   0, Sit out
	*   1, Sit in/Stay in
	* Any other return values are ignored
	*/

	bool debug_mode = true;

	if(debug_mode)
	{
		return -1;
	}
	else
	{
		if(gVars.get_elapsed() % 4){ // disabled 3 out of 4 seconds 

			bool activeRule = false;
			//------------------------------------------------------------------------
			//----------------------- leave the table --------------------------------
			//------------------------------------------------------------------------
			// LEAVE RULE - Game broken up
			activeRule = false; // false=off, true=on
			if(activeRule){
				if(gVars.get_issittingout() && gVars.get_nopponentsdealt() <= 2 && (gVars.br == 2 || gVars.br == 3) ){
					return -2;
				}
			}
			//------------------------------------------------------------------------

			// LEAVE RULE - Wait around, but get out before you get booted
			activeRule = false; // false=off, true=on 
			if(activeRule){
				if(gVars.get_issittingout() && gVars.get_elapsedauto() >= 100 * gVars.get_nopponentsdealt() && (gVars.br == 2 || gVars.br == 3) ){
					return -2;
				}
			}
			//------------------------------------------------------------------------

			// SITIN RULE - Based on players
			activeRule = false; // false=off, true=on
			if(activeRule){
				if(gVars.get_issittingout() && gVars.get_nchairsdealtright()==2 && gVars.get_nopponentsdealt() >=5  && (gVars.br == 2 || gVars.br == 3) ){
					return 1;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on players
			activeRule = false; // false=off, true=on
			if(activeRule){
				if(gVars.get_issittingin() && gVars.get_nopponentsdealt()>=5  && gVars.br==2 ){
					return 0;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on Time
			activeRule = false; // false=off, true=on
			if(activeRule){
				if(gVars.get_issittingin() && gVars.get_elapsed() > 21600 && (gVars.br == 2 || gVars.br == 3) ){ //playing 6 hours
					return 0;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on Stack
			activeRule = false; // false=off, true=on
			if(activeRule){
				if(gVars.get_issittingin() && ( gVars.balance/gVars.get_bblind() ) > 300 * gVars.get_bblind() && gVars.br == 1){ //deep stack
					return 0;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on Stack
			activeRule = false; // false=off, true=on
			if(activeRule){
				if(gVars.get_issittingin() && ( gVars.balance/gVars.get_bblind() ) < 50 * gVars.get_bblind() && gVars.br == 1){ //short stack
					return 0;
				}
			}
			//------------------------------------------------------------------------

		}

		return -1;

	}
}