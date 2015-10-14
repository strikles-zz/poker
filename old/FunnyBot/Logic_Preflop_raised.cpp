#include "stdafx.h"

#include "Logic_Preflop.h"
#include "Bot_Globalvars.h"



double CPreflopLogic::preflop_RaisedBefore()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "I Raised Before\n");

	//Action before variables
	int preflop_number_of_reraises				= gVars.number_of_reraises();
	int preflop_other_opp_call_reraise		= gVars.other_opp_call_reraise();
	double preflop_reraiser_opp_currentbet	= gVars.reraiser_opp_currentbet();
	double preflop_reraiser_opp_balance		= gVars.reraiser_opp_balance();
	bool preflop_reraiser_opp_is_allin			= gVars.reraiser_opp_is_allin();


	/////////////////////////////////////////////
	//We Raised First
	/////////////////////////////////////////////

	if(gVars.preflop_info_first_to_act || gVars.preflop_info_single_limper || gVars.preflop_info_multiple_limpers)
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We raise first last time\n");

		//we go allin if we are commited
		if(gVars.currentbet > 0.75f * (gVars.balance + gVars.currentbet))
		{
			gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We are commited so go allin...\n");
			return c_allin;
		}

		//TODO: raiser was limper or other player?
		//TODO: We are in pos?
				
		//number of raises

		//TODO: players left to act
		if(preflop_number_of_reraises == 1 && preflop_other_opp_call_reraise == 0)
		{ 			
			//stack opp

			//ultra-short stack
			if( (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() <= 11)
			{ 
				// % balance of his currentbet 
				// opp is all-in or near to be all-in
				if(preflop_reraiser_opp_is_allin || gVars.reraiser_opp_currentbet() > 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ JJ TT 99 "
						"AKs AQs "
						"AKo AQo")) 
					{ 
						if(preflop_reraiser_opp_is_allin)
						{
							//we do call
							return c_call;
						}
						else
						{
							//we go all-in
							return c_allin;
						}
					}
					return c_fold;
				}

				//opp does a raise (anyways he is going to be commited, but there is more prob that he folds)
				if(preflop_reraiser_opp_currentbet <= 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ JJ "
						"AKs "
						"AKo"))
					{ 
						//we go all-in
						return c_allin;
					}

					return c_fold;
				}
			}

			//short stack
			else if( (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() > 11 && (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() <= 35) 
			{ 
				//similar to ultra-short stack (but he has a less range than ultra-short stack)
				// %balance of his currentbet

				//opp is all-in or near to be all-in
				if(preflop_reraiser_opp_is_allin || preflop_reraiser_opp_currentbet > 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ JJ TT 99 "
						"AKs AQs "
						"AKo AQo")) 
					{ 
						if(preflop_reraiser_opp_is_allin)
						{
							//we do call
							return c_call;
						}
								
						else
						{
							//we go all-in
							return c_allin;
						}
					}
					return c_fold;
				}

				//opp does a raise (anyways he is going to be commited, but there is more prob that he folds)
				if(preflop_reraiser_opp_currentbet <= 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ JJ "
						"AKs "
						"AKo"))
					{ 
						//we go all-in
						return c_allin;
					}

					return c_fold;
				}

			}
					
			//middle stack
			else if( (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() > 35 &&  (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() <= 70) 
			{ 
				// %balance of his currentbet

				//opp is all-in or near to be all-in
				if(preflop_reraiser_opp_is_allin || preflop_reraiser_opp_currentbet > 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ JJ TT "
						"AKs "
						"AKo")) 
					{ 
						if(preflop_reraiser_opp_is_allin)
						{
							//we do call
							return c_call;
						}
						else
						{
							//we go all-in
							return c_allin;
						}
					}
					return c_fold;
				}

				//opp does a raise (anyways he is going to be commited, but there is more prob that he folds)
				if(preflop_reraiser_opp_currentbet <= 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ JJ TT "
						"AKs "
						"AKo"))
					{ 
						//we go all-in
						return c_allin;
					}

					return c_fold;
				}

			}

				//big stack
			else if( (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() > 70 &&  (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() <= 135) 
			{
				// %balance of his currentbet

				//opp is all-in or near to be all-in
				if(preflop_reraiser_opp_is_allin || preflop_reraiser_opp_currentbet > 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ "
						"AKs "
						"AKo")) 
					{ 
						if(preflop_reraiser_opp_is_allin)
						{
							//we do call
							return c_call;
						}
						else
						{
							//we go all-in
							return c_allin;
						}
					}
					return c_fold;
				}

				//opp does a raise (anyways he is going to be commited, but there is more prob that he folds)
				if(preflop_reraiser_opp_currentbet <= 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand(
						"AA KK QQ "
						"AKs "
						"AKo"))
					{ 
						//we go all-in
						return c_allin;
					}


					return c_fold;
				}
			}
					
			//deep stack
			else if( (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() > 135) 
			{ 
				// %balance of his currentbet

				//opp is all-in or near to be all-in
				if(preflop_reraiser_opp_is_allin || preflop_reraiser_opp_currentbet > 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{ 
					//TODO: calculate correct range
					if(gVars.isHand("AA KK")) 
					{ 
						if(preflop_reraiser_opp_is_allin)
						{
							//we do call
							return c_call;
						}
						else
						{
							//we go all-in
							return c_allin;
						}
					}
					return c_fold;
				}

				//opp does a raise (anyways he is going to be commited, but there is more prob that he folds)
				if(preflop_reraiser_opp_currentbet <= 0.85f * (preflop_reraiser_opp_currentbet + preflop_reraiser_opp_balance)/gVars.get_bblind() )
				{	
					//TODO: calculate correct range
					if(gVars.isHand("AA KK"))
					{
						//we go all-in
						return c_allin;
					}

					return c_fold;
				}
			}
			else
			{
				gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: I can't define opponent balance\n");
				return preflopErrorStrategy();
			}

			return c_fold;

		}
		else if(preflop_number_of_reraises == 1 && preflop_other_opp_call_reraise > 0)
		{
					
			//Set mining time
			//we check if EV+ do a call20?
			if(gVars.isHand("AA KK"))
			{
				return c_allin; 
			}

		}
		else if(preflop_number_of_reraises > 1)
		{				
			//a lot of reraises, so only go all-in with strong cards
			if(gVars.isHand("AA KK"))
			{
				return c_allin;
			}

			/*
			//we can increment range if the players who go raises were shortstackers
			if(reraisers_stack/gVars.get_bblind() < 35*gVars.get_bblind()){
				if(gVars.isHand("QQ JJ AKs AKo")){
					return c_allin;
				}
			}
			*/

			return c_fold;
		}
				
		else
		{
			gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: We raise and receive a raise/raises, and there arent raises?\n");
			return preflopErrorStrategy();
		}	

		return c_fold;
	}

	/////////////////////////////////////////////
	//We reraised other raise
	/////////////////////////////////////////////

	if(gVars.preflop_info_single_raiser_no_caller || gVars.preflop_info_single_raiser_single_caller || gVars.preflop_info_single_raiser_multiple_caller)
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We reraised other raise\n");

		//we go allin if we are commited
		if(gVars.currentbet > 0.75f * (gVars.balance + gVars.currentbet))
		{
			gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We are commited so go allin...\n");
			return c_allin;
		}


				
		//number of raises
		if(preflop_number_of_reraises == 1 && preflop_other_opp_call_reraise == 0)
		{
		}
		else if(preflop_number_of_reraises == 1 && preflop_other_opp_call_reraise > 0)
		{	
		}
		else if(preflop_number_of_reraises > 1)
		{
		}
		else
		{
			gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: We raised and receive a raise/raises, and there arent raises?\n");
			return preflopErrorStrategy();
		}
				
		gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: Strategy not defined. Automatically Fold/Check...\n");
		return c_fold;
	}

			
	/////////////////////////////////////////////
	//We reraised other raises
	/////////////////////////////////////////////

	if(gVars.preflop_info_multiple_raisers)
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We reraised other raises\n");

		//we go allin if we are commited
		if(gVars.currentbet > 0.75f * (gVars.balance + gVars.currentbet))
		{
			gLog->WriteLog(eSeverityInfo, eCatPreFlop, "We are commited so go allin...\n");
			return c_allin;
		}

		//number of raises
		if(preflop_number_of_reraises == 1 && preflop_other_opp_call_reraise == 0)
		{
		}
		else if(preflop_number_of_reraises == 1 && preflop_other_opp_call_reraise > 0)
		{
		}
		else if(preflop_number_of_reraises > 1)
		{
		}
		else
		{
			gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: We raised and receive a raise/raises, and there arent raises?\n");
			return preflopErrorStrategy();
		}

		gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: Strategy not defined. Automatically Fold/Check...\n");
		return c_fold;
	}

	//undefined strategy
	gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: Strategy not defined. Automatically Fold/Check...\n");
	return c_fold;
}