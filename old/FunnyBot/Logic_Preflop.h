#pragma once

double gws(const char* name);

class CPreflopLogic
{
public:

	CPreflopLogic();

public:

	double PreflopDecision();

	double preflop_FirstAction();
	double preflop_CalledBefore();
	double preflop_RaisedBefore();

	double preflop_HeadsUp();
	double preflop_ThreeHanded();


	double FirstToAct();
	double SingleLimper();
	double MultipleLimpers();
	double SingleRaiser_NoCaller();
	double SingleRaiser_SingleCaller();
	double SingleRaiser_MultipleCallers();
	double MultipleRaisers();

	double raise_preflop_reraise_amount();
	double doRaise(double raiseAmmount);

	void checkPreflopCorrectStrategy();
	double preflopErrorStrategy();
};

