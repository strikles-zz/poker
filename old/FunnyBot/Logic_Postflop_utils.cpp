#include "stdafx.h"
#include "Logic_Postflop.h"

#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"

#include "Oracle_Actions.h"



bool firsttoact()
{
	int counter = 0;

	for (int i = 0; i < k_MaxChairs; i++)
	{
		int chair = (gVars.get_dealerchair()+(i+1)) % k_MaxChairs;

		if (chair == gVars.userchair)
			break;

		if (gVars.is_playing(chair))
			counter++;
	}

	if (counter == 0)
		return true;

	return false;
}

///////////////////////
// Kasper Raisamount
//////////////////////
double CPostflopLogic::raisamount()
{
	/*
	*  
	*/

	if (gVars.prw_xpoker_equity < (1.0 / (gVars.nopponentsplaying+1)))
	{
		//make the biggest P(allfold) corresponding with best equity
		return optimalfold();
	}
	else 
	{
		//make the biggest possible showdown pot if my equity is higher than my equal part.
		return optimalpot();
	}
}

double CPostflopLogic::optimalfold()
{
	
	double potpct = 1.0, tempfoldpct = 1.0;
	double EV = 0.0, betamount = 0.0;

	for (double temp = 0.0 ; temp < 1.5 ; temp += 0.1)
	{
		for (int iPlayer = 0; iPlayer < 10; iPlayer++)
		{
			if (gVars.is_playing(iPlayer))
			{
				switch(gVars.br)
				{
					case 2:
						tempfoldpct *= predict.flopfold_betamount(iPlayer, (temp*0.1)*gVars.get_pot());
						break;
					case 3:
						tempfoldpct *= predict.turnfold_betamount(iPlayer, (temp*0.1)*gVars.get_pot()); 
						break;
					case 4:
						tempfoldpct *= predict.riverfold_betamount(iPlayer, temp*gVars.get_pot()); 
						break;
					default:
						break;
				}
			}
				
			if (EV < (gVars.prw_xpoker_equity*(gVars.get_pot() + gVars.nopponentsplaying*(1-tempfoldpct)*(temp*0.1)*gVars.get_pot()) + gVars.get_pot()*tempfoldpct - (temp*0.1)*gVars.get_pot()))
			{
				EV = (gVars.prw_xpoker_equity*(gVars.get_pot() + gVars.nopponentsplaying*(1-tempfoldpct)*(temp*0.1)*gVars.get_pot()) + gVars.get_pot()*tempfoldpct - (temp*0.1)*gVars.get_pot());
				potpct = temp;
			}

			tempfoldpct = 1.0; //reset tempfoldpct
		}
	}

	betamount = (potpct*gVars.get_pot() > gVars.balance ? gVars.balance : potpct*gVars.get_pot());

	return betamount; //return optimal bet in potpct.
}

double CPostflopLogic::optimalpot()
{
	double tmp_amount = 0.0, betamount = 0.0;
	bool first_to_act = firsttoact();

	if (gVars.call > 0)
		tmp_amount = 3*gVars.call;
	else
		tmp_amount = gVars.get_pot();

	switch(gVars.br)
	{
		case 2:
		{
			if ((gVars.prw_xpoker_equity > 0.95) || (gVars.prw_xpoker_equity > 0.8 && gVars.ncurrentbet == 0 && first_to_act))
			{
				gLog->WriteLog(eSeverityInfo, eCatDebugNow, "Check Raising...\n");
				tmp_amount = 0.0;
			}

			break;
		}
		case 3:
		{
			/*
			if (gVars.prw_xpoker_equity > 0.95 && gVars.ncurrentbet == 0 && first_to_act)
			{
				gLog->WriteLog(eSeverityInfo, eCatDebugNow, "Check Raising...\n");
				tmp_amount = 0.0;
			}
			*/

			break;
		}
		case 4:
		{
			/*
			if (gVars.prw_xpoker_equity > 0.88)
			{
				tmp_amount = 2*gVars.get_pot();
			}
			*/

			break;
		}
			
		default:
			break;
	}

	betamount = (tmp_amount > gVars.balance ? gVars.balance : tmp_amount);

	//return optimal bet in potpct for highest possible pot.
	return betamount; 
}





////////////////////
// Strikles Stuff //
////////////////////

double CPostflopLogic::Probability_All_Fold()
{
	double pr_all_fold = 0.0;

	//Load player prfold and prrais values
	for (int iPlayer = 0; iPlayer < 10; iPlayer++)
	{
		if(iPlayer != gVars.userchair && gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
		{
			double pr_fold = gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_fold * gVars.ReductionFactor(iPlayer);

			if (pr_all_fold == 0)
				pr_all_fold = pr_fold;
			else
				pr_all_fold *= pr_fold;
		}		
	}

	return pr_all_fold;
}

double CPostflopLogic::Pot_Increase_Fom_Opp_Calls(const bool we_raise, const double raise_amt)
{
	double pot_increase = 0.0, total_call = 0.0;
	double max_bet = 0.0;

	for (int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++)
	{
		if (iPlayer == gVars.userchair)
			continue;

		double player_current_bet = gVars.CurState().m_player[iPlayer].m_currentbet;

		if (gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame && player_current_bet > max_bet)
			max_bet = player_current_bet;
	}

	if (we_raise)
		total_call = raise_amt + max_bet;
	else
		total_call = max_bet;		

	for (int iPlayer = 0; iPlayer < k_MaxChairs; iPlayer++)
	{
		if (iPlayer == gVars.userchair)
			continue;

		if (gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
		{
			double player_current_bet = gVars.CurState().m_player[iPlayer].m_currentbet;
			double player_balance = gVars.CurState().m_player[iPlayer].m_balance;

			double call_amt = min(max(total_call - player_current_bet, 0.0), player_balance);
			
			if (call_amt > 0)
			{
				double pr_call = 0.0;

				if (we_raise)
					pr_call = gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_call;
				else
					pr_call = gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_call;

				pot_increase += (pr_call * call_amt);
			}
		}
	}

	return pot_increase;
}

////////////////////////
// Strikles Simple EV //
////////////////////////

double CPostflopLogic::EV_Raise(const double raise_amt)
{
	double ev_raising = 0.0, ev_raise_win = 0.0, ev_raise_lose = 0.0;

	if (raise_amt == 0 || gVars.call > gVars.balance || raise_amt > gVars.balance)
	{
		if (gVars.call > gVars.balance)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "Impossible Raise. Call > Balance - returning ev_call\n");
		else if (raise_amt > gVars.balance)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "Impossible Raise. Raise > Balance -> returning ev_call\n");
		else if (raise_amt == 0)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "This is not a real raise - [CR ?] -> returning ev_call\n");

		return EV_Call();
	}

	ev_raise_win = gVars.prw_xpoker_equity * (gVars.get_pot() + Pot_Increase_Fom_Opp_Calls(true, raise_amt) + gVars.balance); 
	ev_raise_lose = (1 - gVars.prw_xpoker_equity) * (gVars.balance - raise_amt);

	double induced_fold_gain = Probability_All_Fold() * (gVars.get_pot() + gVars.balance);
	ev_raising = induced_fold_gain + (1 - Probability_All_Fold()) * (ev_raise_win + ev_raise_lose);

	return ev_raising;
}

double CPostflopLogic::EV_Call()
{
	double ev_calling = 0.0;

	double call_amt = gVars.call;
	double pot_amt = gVars.get_pot();

	if (gVars.call >= gVars.balance)
	{
		call_amt = gVars.balance;
		pot_amt -= (gVars.call - gVars.balance);
	}

	//double ev_call_win = gVars.prw_xpoker_equity * (pot_amt + Pot_Increase_Fom_Opp_Calls(false, 0.0) + gVars.balance);
	double ev_call_win = gVars.prw_xpoker_equity * (pot_amt + gVars.balance); 
	double ev_call_loose = (1 - gVars.prw_xpoker_equity) * (gVars.balance - call_amt);

	ev_calling = ev_call_win + ev_call_loose;

	return ev_calling;
}

double CPostflopLogic::EV_Fold()
{
	return gVars.balance;
}

int CPostflopLogic::EV_decision()
{
	double raise_amt = raisamount();

	double ev_raise = EV_Raise(raise_amt);
	double ev_call = EV_Call();

	ev_raise -= EV_Fold();
	ev_call -= EV_Fold();

	if (ev_raise > ev_call && ev_raise > 0)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebugNow, "EV(rais): %+10.2f\n", ev_raise);

		if (ev_call > 0)
			gLog->WriteLog(eSeverityInfo, eCatInfo, "EV(call): %+10.2f\n", ev_call);
		else
			gLog->WriteLog(eSeverityInfo, eCatPostFlopNegative, "EV(call): %+10.2f\n", ev_call);

		return c_raise;
	}

	else if (ev_call > 0)
	{
		if (ev_raise > 0)
			gLog->WriteLog(eSeverityInfo, eCatInfo, "EV(rais): %+10.2f\n", ev_raise);
		else
			gLog->WriteLog(eSeverityInfo, eCatPostFlopNegative, "EV(rais): %+10.2f\n", ev_raise);

		gLog->WriteLog(eSeverityInfo, eCatPostFlop, "EV(call): %+10.2f\n", ev_call);

		/*
		// Dumb test guard
		if (gVars.prw_xpoker_equity > (1.0/(gVars.nopponentsplaying+1)))
			return c_call;
		else
			return 0;
		*/

		return c_call;
	}

	else
	{
		gLog->WriteLog(eSeverityInfo, eCatPostFlopNegative, "EV(rais): %+10.2f\n", ev_raise);
		gLog->WriteLog(eSeverityInfo, eCatPostFlopNegative, "EV(call): %+10.2f\n", ev_call);
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////
// Strikles Utility Test //
///////////////////////////

// lambda = 1 => risk-neutral
// lambda > 1 => risk-loving

// (lambda = 1 - ro)

// Isoelastic util
double CPostflopLogic::isoelastic_util(const double wealth, const double lambda)
{
	return (pow(wealth, lambda) - 1) / lambda;  
}

// Isoelastic Certainty Equivalent (Inverse of the above)
double CPostflopLogic::isoelastic_ce(const double amt, const double lambda)
{
	return pow(lambda * (amt + 1/lambda), 1/lambda);  
}

double CPostflopLogic::isoelastic_call(const double aversion_coeff)
{
	double call_amt = gVars.call;
	double pot_amt = gVars.get_pot();

	if (gVars.call >= gVars.balance)
	{
		call_amt = gVars.balance;
		pot_amt -= (gVars.call - gVars.balance);
	}

	double expected_call_utility_win = gVars.prw_xpoker_equity * isoelastic_util(pot_amt + gVars.balance, aversion_coeff);
	double expected_call_utility_lose = (1 - gVars.prw_xpoker_equity) * isoelastic_util(gVars.balance - call_amt, aversion_coeff);
	double expected_call_utility = expected_call_utility_win + expected_call_utility_lose;

	return expected_call_utility;
}

double CPostflopLogic::isoelastic_raise(const double aversion_coeff)
{
	double raise_amt = raisamount();

	if (raise_amt == 0 || gVars.call > gVars.balance || raise_amt > gVars.balance)
	{
		return isoelastic_call(aversion_coeff);
		gLog->WriteLog(eSeverityInfo, eCatDebug, "Impossible Raise. Call > Balance ...\n");

		if (raise_amt > gVars.balance)
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR : Raise Amount > Balance - %.2f\n");
	}

	double expected_raise_utility_win = (gVars.prw_xpoker_equity) * isoelastic_util(gVars.get_pot() + Pot_Increase_Fom_Opp_Calls(true, raise_amt) + gVars.balance, aversion_coeff);
	double expected_raise_utility_loose = (1 - gVars.prw_xpoker_equity) * isoelastic_util(gVars.balance - raise_amt, aversion_coeff);

	double induced_fold_utility = Probability_All_Fold() * isoelastic_util(gVars.get_pot() + gVars.balance, aversion_coeff);
	double expected_raise_utility = induced_fold_utility + (1 - Probability_All_Fold()) * (expected_raise_utility_win + expected_raise_utility_loose);

	return expected_raise_utility;
}

double CPostflopLogic::isoelastic_premium(const double aversion_coeff)
{
	double certainty_equivalent = isoelastic_ce(isoelastic_call(aversion_coeff), aversion_coeff);
	double amt = min(max(certainty_equivalent - gVars.balance, 0), gVars.balance);

	return amt;
}

int CPostflopLogic::utility_decision()
{
	int stack_size = static_cast<int> (gVars.balance / gVars.get_bblind());
	double aversion_coeff;

	if (stack_size > 1600)
		aversion_coeff = 0.7;
	else if (stack_size > 800)
		aversion_coeff = 0.6;
	else if (stack_size > 400)
		aversion_coeff = 0.55;
	else if (stack_size > 200)
		aversion_coeff = 0.5;
	else if (stack_size > 100)
		aversion_coeff = 0.45;
	else
		aversion_coeff = 0.4;

	double expected_fold_utility = isoelastic_util(gVars.balance, aversion_coeff);
	double premium = isoelastic_premium(aversion_coeff);

	double expected_raise_utility = isoelastic_raise(aversion_coeff) - expected_fold_utility;
	double expected_call_utility = isoelastic_call(aversion_coeff) - expected_fold_utility;

	gLog->WriteLog(eSeverityInfo, eCatPostFlop, "U(rais): %.3f\n", expected_raise_utility);
	gLog->WriteLog(eSeverityInfo, eCatPostFlop, "U(call): %.3f\n", expected_call_utility);
	gLog->WriteLog(eSeverityInfo, eCatPostFlop, "Call   : %.2f, premium : %.2f\n", gVars.call, premium);

	if (expected_raise_utility > expected_call_utility && expected_raise_utility > 0)
	{
		gLog->WriteLog(eSeverityInfo, eCatPostFlop, "U(rais) activated : %.3f\n", expected_raise_utility);
		return c_raise;
	}
	else if (premium > 0 && expected_call_utility > 0)
	{
		gLog->WriteLog(eSeverityInfo, eCatPostFlop, "U(call) activated : %.3f\n", expected_call_utility);
		return c_call;
	}

	return 0;
}