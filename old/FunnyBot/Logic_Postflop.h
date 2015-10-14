#pragma once

double gws(const char* name);

class CPostflopLogic
{
public:

	CPostflopLogic();

public:

	//Flop
	int FlopDecision();
	double checkraisflop();
	//Turn
	int TurnDecision();
	double checkraisturn();
	//River
	int RiverDecision();
	double checkraisriver();

	// raiseamount
	double raisamount();
	double optimalfold();
	double optimalpot();

	// Simple EV
	double Pot_Increase_Fom_Opp_Calls(const bool we_raise, const double raise_amt);
	double Probability_All_Fold();
	double EV_Raise(const double raise_amt);
	double EV_Call();
	double EV_Fold();

	int EV_decision();


	// Strikles utility test stuff
	double isoelastic_util(const double bet, const double risk_aversion_coeff);
	double isoelastic_ce(const double wealth, const double lambda);
	double isoelastic_amount(const double wealth, const double lambda);
	double isoelastic_call(const double aversion_coeff);
	double isoelastic_raise(const double aversion_coeff);
	double isoelastic_premium(const double aversion_coeff);

	int utility_decision();
};