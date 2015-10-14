#include "stdafx.h"

#include "Logic_Preflop.h"
#include "Bot_Globalvars.h"



double CPreflopLogic::preflop_FirstAction()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "No Action Before - Posted SB/BB\n");

	/////////////////////////////////////////////
	// Reset strategy vars
	/////////////////////////////////////////////
	gVars.preflop_position = gVars.set_preflop_position();

	gVars.preflop_info_first_to_act							= gVars.info_first_to_act();
	gVars.preflop_info_single_limper						= gVars.info_single_limper();
	gVars.preflop_info_multiple_limpers					= gVars.info_multiple_limpers();
	gVars.preflop_info_single_raiser_no_caller			= gVars.info_single_raiser_no_caller();
	gVars.preflop_info_single_raiser_single_caller		= gVars.info_single_raiser_single_caller();
	gVars.preflop_info_single_raiser_multiple_caller	= gVars.info_single_raiser_multiple_caller();
	gVars.preflop_info_multiple_raisers					= gVars.info_multiple_raisers();

	//Flags
	bool blind_stealing = false;
	bool use_stats = false;

	/////////////////////////////////////////////
	//Check preflop position
	/////////////////////////////////////////////
	if(gVars.preflop_position == 0)
	{
		gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: Incorrect preflop position\n");
		return preflopErrorStrategy();
	}

	/////////////////////////////////////////////
	//Check bool (only one activated):
	/////////////////////////////////////////////
	int counter = 0;

	if(gVars.preflop_info_first_to_act)
		counter++;
	if(gVars.preflop_info_single_limper)
		counter++;
	if(gVars.preflop_info_multiple_limpers)
		counter++;
	if(gVars.preflop_info_single_raiser_no_caller)
		counter++;
	if(gVars.preflop_info_single_raiser_single_caller)
		counter++;
	if(gVars.preflop_info_single_raiser_multiple_caller)
		counter++;
	if(gVars.preflop_info_multiple_raisers)
		counter++;

	if(counter > 1)
	{
		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: There will be only one bool activated\n");

		//Check what bool is activated
		if(gVars.preflop_info_first_to_act)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: First to act activated\n");
		if(gVars.preflop_info_single_limper)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Single Limper activated\n");
		if(gVars.preflop_info_multiple_limpers)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Multiple Limpers activated\n");
		if(gVars.preflop_info_single_raiser_no_caller)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Single raiser No caller activated\n");
		if(gVars.preflop_info_single_raiser_single_caller)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Single raiser Single caller activated\n");
		if(gVars.preflop_info_single_raiser_multiple_caller)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Single raiser Multiple caller activated\n");
		if(gVars.preflop_info_multiple_raisers)
			gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Multiple Raisers activated\n");
	}

	if(counter == 0)
	{
		gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: No one preflop strategy bool activated\n");
		return preflopErrorStrategy();
	}

	/////////////////////////////////////////////
	// 1. FIRST TO ACT
	//
	//DESCRIPTION: We are the first to act (Raise or Fold)
	//UNVALID POSITIONS: BB
	/////////////////////////////////////////////
	if(gVars.preflop_info_first_to_act)
		return FirstToAct();

	/////////////////////////////////////////////
	// 2. SINGLE LIMPER
	//
	//DESCRIPTION:
	//UNVALID POSITIONS:
	/////////////////////////////////////////////
	if(gVars.preflop_info_single_limper)
		return SingleLimper();

	/////////////////////////////////////////////
	// 3. MULTIPLE LIMPERS
	//
	//DESCRIPTION:
	//UNVALID POSITIONS:
	/////////////////////////////////////////////
	if(gVars.preflop_info_multiple_limpers)
		return MultipleLimpers();

	/////////////////////////////////////////////
	// 4. SINGLE RAISER AND NO CALLER
	//
	//DESCRIPTION:
	//UNVALID POSITIONS: UTG
	/////////////////////////////////////////////
	if(gVars.preflop_info_single_raiser_no_caller)
		return SingleRaiser_NoCaller();

	/////////////////////////////////////////////
	// 5. SINGLE RAISE AND SINGLE CALLER
	//
	//DESCRIPTION: Chance for a squeeze play (best done on the Button or BBlind)
	//UNVALID POSITIONS: UTG, HJ
	/////////////////////////////////////////////
	if(gVars.preflop_info_single_raiser_single_caller)
		return SingleRaiser_SingleCaller();

	/////////////////////////////////////////////
	// 6. SINGLE RAISER, MULTIPLE CALLER
	//
	//DESCRIPTION:
	//UNVALID POSITIONS: UTG, HJ, CO
	/////////////////////////////////////////////
	if(gVars.preflop_info_single_raiser_multiple_caller)
		return SingleRaiser_MultipleCallers();

	/////////////////////////////////////////////
	// 7. MULTIPLE RAISERS
	//
	//DESCRIPTION:
	//UNVALID POSITIONS: 
	/////////////////////////////////////////////
	if(gVars.preflop_info_multiple_raisers)
		return MultipleRaisers();

	//undefined strategy
	gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: Strategy not defined. Automatically Fold/Check...\n");
	return c_fold;
}


double CPreflopLogic::FirstToAct()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "FIRST TO ACT\n");

	if(gVars.preflop_position == c_UTG)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
			"AKs AQs AJs KQs "
			"AKo AQo"))
		{
			return doRaise(4 * gVars.get_bblind());
		}
	}

	if(gVars.preflop_position == c_HJ)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
			"AKs AQs AJs "
			"KQs AKo AQo"))
		{
			return doRaise(3.5f * gVars.get_bblind());
		}
	}

	if(gVars.preflop_position == c_CO)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
			"AKs AQs AJs ATs KQs "
			"AKo AQo AJo KQo"))
		{
			return doRaise(3.5f * gVars.get_bblind());
		}
	}

	if(gVars.preflop_position == c_BT)
	{
		//Small Blind posted?
		if(gVars.SB)
		{
			//we raise 3 bb to win 1,5bb (1bb + 1sb)
			//Raise for value
			if(gVars.isHand(
				"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
				"AKs AQs AJs ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs QJs JTs T9s 98s 87s 76s 65s "
				"AKo AQo AJo KQo QJo JTo T9o 98o"))
			{
				return doRaise(3 * gVars.get_bblind());
			}
			/*
			//Raise exploiting stats  (%fold vs steal(sb) x %fold vs steal(bb))
			if(fold_to_steal(get_chair_bb) * fold_to_steal(get_chair_sb)){
				blind_stealing = true;
				return doRaise(3 * gVars.get_bblind());
			}
			*/
		}

		else
		{

			/*
			if(get_balance(get_chair_bb)/get_bblind() < 5*get_bblind()){//ultra-short stack player
							
			}
			*/

			//we raise 3 bb to win 1bb
			//Raise for value (TODO: Increase range of hand!!) 
			if(gVars.isHand(
				"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
				"AKs AQs AJs ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs QJs JTs T9s 98s 87s 76s 65s "
				"AKo AQo AJo KQo QJo JTo T9o 98o"))
			{
				return doRaise(3 * gVars.get_bblind());
			}

			/*
			//Raise exploiting stats (%fold vs steal(bb))
			if(fold_to_steal(get_bb_chair)){
				blind_stealing = true;
				return doRaise(3 * gVars.get_bblind());
			}
			*/
		}

	}

	if(gVars.preflop_position == c_SB)
	{

		/*
		if(get_balance(get_chair_bb)/get_bblind() < 5*get_bblind()){//ultra-short stack player
						
		}
		*/

		//Raise for value
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
			"AKs AQs AJs ATs KQs "
			"AKo AQo AJo KQo"))
		{
			return doRaise(3 * gVars.get_bblind());
		}

		/*
		//Raise exploiting stats (%fold vs steal(bb))
		if(){
			blind_stealing = true;
			return doRaise(3 * gVars.get_bblind());
		}
		*/
	}

	if(gVars.preflop_position == c_BB)
	{
		gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: We are in BB and we are the first to act\n");
		return preflopErrorStrategy();
	}

	//If I post blind I check, not fold
	if(gVars.call > 0)
	{
		return c_fold;
	}
				
	else
	{
		return c_check;
	}
}

double CPreflopLogic::SingleLimper()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE LIMPER\n");

	if(gVars.preflop_position == c_UTG)
	{
		//we can be in UTG if an opponent posted blinds (he has started to play now)
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs KQs "
			"AKo AQo"))
		{
			return doRaise(5 * gVars.get_bblind());
		}
		if(gVars.isHand("77 66 55 44 33 22"))
		{
			return c_call;
		}
	}

	if(gVars.preflop_position == c_HJ)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs KQs "
			"AKo AQo"))
		{
			return doRaise(5 * gVars.get_bblind());
		}
		if(gVars.isHand("77 66 55 44 33 22"))
		{
			return c_call;
		}
	}
	if(gVars.preflop_position == c_CO){
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs ATs KQs "
			"AKo AQo AJo KQo"))
		{
			return doRaise(5 * gVars.get_bblind());
		}
		if(gVars.isHand("77 66 55 44 33 22"))
		{
			return c_call;
		}
	}
	if(gVars.preflop_position == c_BT)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs KJs QJs QTs JTs J9s T9s T8s 98s 97s 87s 76s 65s "
			"AKo AQo AJo ATo A9o KQo KJo QJo JTo T9o"))
		{
			return doRaise(5 * gVars.get_bblind());
		}
		if(gVars.isHand("77 66 55 44 33 22")){
			return c_call;
		}
	}
	if(gVars.preflop_position == c_SB)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs ATs "
			"KQs AKo AQo AJo KQo"))
		{
			return doRaise(4 * gVars.get_bblind());
		}	
		if(gVars.isHand(
			"77 66 55 44 33 22 "
			"A9s A8s A7s A6s A5s A4s A3s A2s"))
		{
			return c_call;
		}
	}
	if(gVars.preflop_position == c_BB)
	{
		////if is SB who limped, raise with more range because we have position!
		if(gVars.info_sb_limped())
		{
			/*
			limp/fold stats
			if(limp_fold(get_sb_chair) < ){
				return doRaise(4 * gVars.get_bblind());
			}
			*/

			if(gVars.isHand(
				"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
				"AKs AQs AJs ATs KQs "
				"AKo AQo AJo KQo"))
			{
				return doRaise(4 * gVars.get_bblind());
			}

		}
		else
		{
			if(gVars.isHand(
				"AA KK QQ JJ TT 99 88 "
				"AKs AQs AJs KQs "
				"AKo AQo AJo KQo"))
			{
				return doRaise(4 * gVars.get_bblind());
			}

		}

		return c_check; //we posted BB, so check if no raise...
	}

	//If I post blind I check, not fold
	if(gVars.call > 0)
		return c_fold;

	return c_check;
}

double CPreflopLogic::MultipleLimpers()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "MULTIPLE LIMPERS\n");

	if(gVars.preflop_position == c_UTG)
	{
		//we can be in UTG position, if two or more players started to play
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs "
			"AKo AQo"))
		{
			return doRaise((4 + 1.5 * gVars.nopponentscalling) * gVars.get_bblind());
		}
		if(gVars.isHand(
			"77 66 55 44 33 22 "
			"ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs QJs JTs T9s 98s 87s 76s 65s 54s 43s 32s"))
		{
			return c_call;
		}
	}
	if(gVars.preflop_position == c_HJ)
	{
		//we can be in HJ position, if two or more players started to play
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs "
			"AKo AQo"))
		{
			return doRaise((4 + 1.5 * gVars.nopponentscalling) * gVars.get_bblind());
		}
		if(gVars.isHand(
			"77 66 55 44 33 22 "
			"ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs QJs JTs T9s 98s 87s 76s 65s 54s 43s 32s"))
		{
			return c_call;
		}
	}
	if(gVars.preflop_position == c_CO)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs "
			"AKo AQo"))
		{
			return doRaise((4 + 1.5f * gVars.nopponentscalling) * gVars.get_bblind());
		}
		if(gVars.isHand(
			"77 66 55 44 33 22 "
			"ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs QJs JTs T9s 98s 87s 76s 65s 54s 43s 32s"))
		{
			return c_call;
		}
	}

	if(gVars.preflop_position == c_BT)
	{
		//Button open range
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
			"AKs AQs AJs ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs KJs QJs QTs JTs J9s T9s T8s 98s 97s 87s 76s 65s "
			"AKo AQo KQo QJo JTo T9o 98o"))
		{
			return doRaise((4 + 1.5 * gVars.nopponentscalling) * gVars.get_bblind()); 
		}
	}
	if(gVars.preflop_position == c_SB)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs "
			"AKo AQo"))
		{
			return doRaise((4 + 1.5 * gVars.nopponentscalling) * gVars.get_bblind());
		}
		if(gVars.isHand(
			"77 66 55 44 33 22 "
			"ATs A9s A8s A7s A6s A5s A4s A3s A2s KQs QJs JTs T9s 98s 87s 76s 65s 54s 43s 32s"))
		{
			return c_call;
		}
	}
	if(gVars.preflop_position == c_BB)
	{
		if(gVars.isHand(
			"AA KK QQ JJ TT 99 88 "
			"AKs AQs AJs "
			"AKo AQo"))
		{
			return doRaise((4 + 1.5 * gVars.nopponentscalling) * gVars.get_bblind());
		}

		return c_check; //we posted BB, so check if no raise...
	}

	//If I post blind I check, not fold
	if(gVars.call > 0)
		return c_fold;

	return c_check;
}

double CPreflopLogic::SingleRaiser_NoCaller()
{
	// Single raise, raiser raised = 2 BB (Min Raise)
	if((gVars.currentbets[gVars.raischair]/gVars.get_bblind()) == 2)
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE RAISER NO CALLER, RAISE IS == 2*bblind\n");

		if(gVars.preflop_position == c_UTG)
		{
			gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: UTG raises, so I can't be UTG\n");
			return preflopErrorStrategy();
		}

		if(gVars.preflop_position == c_HJ)
		{
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{
				return raise_preflop_reraise_amount();
			}

			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_CO)
		{
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{
				return raise_preflop_reraise_amount();
			}

			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_BT)
		{
			if(gVars.isHand(
				"AA KK QQ JJ TT "
				"AKs AQs AJs "
				"AKo AQo AJo"))
			{
				return raise_preflop_reraise_amount();
			}

			if(gVars.isHand("99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_SB)
		{
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{
				return raise_preflop_reraise_amount();
			}

			if(gVars.pf_raiser_position() == c_BT)
			{
				if(gVars.rais_chair_hands > 50 && gVars.rais_chair_ats > 0.25f && (gVars.get_handrank169() / 169) * 0.6f < gVars.rais_chair_ats)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Single raise, raiser raised = 2 BB (Min Raise). We are SB Re-Raising BT raise\n");
					return raise_preflop_reraise_amount();
				}
			}

			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_BB)
		{
			//Raise for value
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{ 
				//FIX: We are the last to act, if raiser is ultra-short stack or short stack, we go all-in?
				if(gVars.raiser_balance()/gVars.get_bblind() <= 5)
				{
					return c_allin;
				}
				else
				{
					return raise_preflop_reraise_amount();
				}
			}
			if(gVars.pf_raiser_position() == c_BT)
			{
				//raise for stats (ats)
				if(gVars.rais_chair_hands > 50 && gVars.rais_chair_ats > 0.25f && (gVars.get_handrank169() / 169) * 0.6f < gVars.rais_chair_ats)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Single raise, raiser raised = 2 BB (Min Raise). We are BB Re-Raising BT raise\n");
					return raise_preflop_reraise_amount();
				}
			}
			if(gVars.pf_raiser_position() == c_SB)
			{
				//raise for stats (ats)
				if (gVars.rais_chair_hands > 50 && gVars.rais_chair_ats > 0.25f && (gVars.get_handrank169() / 169) * 0.8f < gVars.rais_chair_ats)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Single raise, raiser raised = 2 BB (Min Raise). We are BB Re-Raising SB raise\n");
					return raise_preflop_reraise_amount();
				}
			}

			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		return c_fold;
	}

	// Single raise, raiser raised <= 5 BB && raise > 2 BB
	if(((gVars.currentbets[gVars.raischair]/gVars.get_bblind()) <= 5) && ((gVars.currentbets[gVars.raischair]/gVars.get_bblind()) > 2))
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE RAISER NO CALLER, RAISE IS <= 5*bblind\n");

		if(gVars.preflop_position == c_UTG)
		{
			gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: UTG raises, so I can't be UTG\n");
			return preflopErrorStrategy();
		}

		if(gVars.preflop_position == c_HJ)
		{
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{
				return raise_preflop_reraise_amount();
			}
			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_CO)
		{
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{
				return raise_preflop_reraise_amount();
			}
			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_BT)
		{
			if(gVars.isHand(
				"AA KK QQ JJ TT "
				"AKs AQs AJs "
				"AKo AQo AJo"))
			{
				return raise_preflop_reraise_amount();
			}
			if(gVars.isHand("99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_SB)
		{
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{
				return raise_preflop_reraise_amount();
			}

			/*
			if(gVars.pf_raiser_position() == c_BT)
			{
				if(gVars.rais_chair_hands > 50 && gVars.rais_chair_ats > 0.25f && (gVars.get_handrank169() / 169) * 0.6f < gVars.rais_chair_ats)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Single raise, raiser raised > 2 BB && < 5 BB. We are SB Re-Raising BT raise\n");
					return raise_preflop_reraise_amount();
				}
			}
			*/

			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		if(gVars.preflop_position == c_BB)
		{
			//Raise for value
			if(gVars.isHand(
				"AA KK QQ "
				"AKs AQs "
				"AKo AQo"))
			{ 
				//FIX: We are the last to act, if raiser is ultra-short stack or short stack, we go all-in?
				if(gVars.raiser_balance()/gVars.get_bblind() <= 5)
				{
					return c_allin;
				}
				else
				{
					return raise_preflop_reraise_amount();
				}
			}

			/*
			// Disabled for the moment
			if(gVars.pf_raiser_position() == c_BT)
			{
				//raise for stats (ats)
				if(gVars.rais_chair_hands > 50 && gVars.rais_chair_ats > 0.25f && (gVars.get_handrank169() / 169) * 0.6f < gVars.rais_chair_ats)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Single raise, raiser raised > 2 BB && < 5 BB. We are BB Re-Raising BT raise\n");
					return raise_preflop_reraise_amount();
				}
			}

			if (gVars.pf_raiser_position() == c_SB)
			{
				//raise for stats (ats)
				if (gVars.rais_chair_hands > 50 && gVars.rais_chair_ats > 0.25f && (gVars.get_handrank169() / 169) * 0.8f < gVars.rais_chair_ats)
				{
					gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Single raise, raiser raised > 2 BB && < 5 BB. We are BB Re-Raising SB raise\n");
					return raise_preflop_reraise_amount();
				}
			}
			*/

			if(gVars.isHand("JJ TT 99 88 77 66 55 44 33 22"))
			{
				return c_call;
			}
		}

		return c_fold;
	}

	// Single raise, raiser raised > 5 BB
	if((gVars.currentbets[gVars.raischair]/gVars.get_bblind()) > 5)
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE RAISER NO CALLER, RAISE IS > 5*bblind\n");

		if(gVars.preflop_position == c_UTG)
		{
			gLog->WriteLog(eSeverityError, eCatPreFlop, "ERROR: UTG raises, so I can't be UTG\n");
			return preflopErrorStrategy();
		}

		//OUT OF POSITION
		if((gVars.preflop_position == c_HJ) || (gVars.preflop_position == c_CO) || (gVars.preflop_position == c_SB))
		{
			if(gVars.isHand("AA"))
			{
				return raise_preflop_reraise_amount();
			}

			if(((gVars.raiser_balance()/gVars.get_bblind()) > 80) && (gVars.raiser_balance()/gVars.get_bblind()) < 120 && gVars.get_ncallbets() <= 10)
			{
				if(gVars.isHand(
					"KK "
					"AKs "
					"AKo"))
				{
					return raise_preflop_reraise_amount();
				}

				if(gVars.isHand("QQ JJ TT 99 88 77 66 55 44 33 22"))
				{
					return c_call;
				}
			}

			//Raiser has less than 40 big blinds left and we have something decent, push all in
			if(gVars.raiser_balance()/gVars.get_bblind() < 40)
			{
				if (gVars.isHand(
					"AA KK QQ "
					"AKs "
					"AKo"))
				{
					return c_allin;
				}
			}
		}

		//BUTTON and BIG BLIND
		if(gVars.preflop_position == c_BT || gVars.preflop_position == c_BB)
		{

			if(gVars.isHand("AA"))
			{
				return raise_preflop_reraise_amount();
			}
			//Raiser has less than 40 big blinds left and we have something decent, push all in
			if( (gVars.raiser_balance()/gVars.get_bblind()) < 40 && gVars.isHand("AA KK QQ JJ AKs AKo"))
			{
				return c_allin;
			}
		}

		return c_fold;
	}

	gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: Can't determinate raise amount...\n");
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "raischair = %d . currentsbets[raischair] = %.2f (in bblind = %.2f)\n",gVars.raischair,gVars.currentbets[gVars.raischair],gVars.currentbets[gVars.raischair]/gVars.get_bblind());

	return preflopErrorStrategy();
}

double CPreflopLogic::SingleRaiser_SingleCaller()
{
	// Single raise, raiser raised <= 5 BB
	if( (gVars.currentbets[gVars.raischair]/gVars.get_bblind() ) <= 5 )
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE RAISER SINGLE CALLER. RAISE IS <= 5*bblind\n");

		//UTG
		if(gVars.preflop_position == c_UTG)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ call, so I can't be UTG\n");
			return preflopErrorStrategy();
		}

		//HJ
		if(gVars.preflop_position == c_HJ)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ call, so I can't be HJ\n");
			return preflopErrorStrategy();
		}

		// BUTTON
		if(gVars.preflop_position == c_BT)
		{

			if(gVars.isHand("AA KK"))
			{
				return raise_preflop_reraise_amount();
			}

			//TODO: Define info_fold_to_3bet()
			if(gVars.info_fold_to_3bet() >= .4f)
			{
				gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Fold to 3bet raise A\n");
				return raise_preflop_reraise_amount();
			}
		}

		// OTHER POSITIONS
		if((gVars.preflop_position == c_SB) || (gVars.preflop_position == c_BB) || (gVars.preflop_position == c_CO)){ 

			if(gVars.isHand("AA KK"))
			{
				return raise_preflop_reraise_amount();
			}
			//TODO: Define info_fold_to_3bet()
			if(gVars.info_fold_to_3bet() >= .5f)
			{ 
				gLog->WriteLog(eSeverityInfo, eCatDebugNow, "WARNING : Fold to 3bet raise B\n");
				return raise_preflop_reraise_amount();
			}
		}

		return c_fold;
	}

	// Single raise, raiser raised > 5 BB
	if( (gVars.currentbets[gVars.raischair]/gVars.get_bblind() ) > 5 )
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE RAISER SINGLE CALLER. RAISE IS > 5*bblind\n");

		//TODO: Review hand ranges

		//UTG
		if(gVars.preflop_position == c_UTG)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ call, so I can't be UTG\n");
			return preflopErrorStrategy();
		}

		//HJ
		if(gVars.preflop_position == c_HJ)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ call, so I can't be HJ\n");
			return preflopErrorStrategy();
		}

		// BUTTON
		if(gVars.preflop_position == c_BT)
		{
			if(gVars.isHand("AA KK"))
			{
				return raise_preflop_reraise_amount();
			}
			//TODO: Define info_fold_to_3bet()
			if(gVars.info_fold_to_3bet() >= .4f)
			{ 
				return raise_preflop_reraise_amount();
			}
		}

		// OTHER POSITIONS
		if((gVars.preflop_position == c_SB) || (gVars.preflop_position == c_BB) || (gVars.preflop_position == c_CO))
		{ 

			if(gVars.isHand("AA KK"))
			{
				return raise_preflop_reraise_amount();
			}
			//TODO: Define info_fold_to_3bet()
			if(gVars.info_fold_to_3bet() >= .5f)
			{ 
				return raise_preflop_reraise_amount();
			}
		}

		return c_fold;
	}

	gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: Can't determinate raise amount...\n");
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "raischair = %d . currentsbets[raischair] = %.2f (in bblind = %.2f)\n",gVars.raischair,gVars.currentbets[gVars.raischair],gVars.currentbets[gVars.raischair]/gVars.get_bblind());

	return preflopErrorStrategy();
}

double CPreflopLogic::SingleRaiser_MultipleCallers()
{
	// Single raise, raiser raised <= 5 BB
	if( (gVars.currentbets[gVars.raischair]/gVars.get_bblind() ) <= 5 )
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE RAISER SINGLE CALLER. RAISE IS <= 5*bblind\n");

		//UTG
		if(gVars.preflop_position == c_UTG)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ,CO call, so I can't be UTG\n");
			return preflopErrorStrategy();
		}

		//HJ
		if(gVars.preflop_position == c_HJ)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ,CO call, so I can't be HJ\n");
			return preflopErrorStrategy();
		}

		//CO
		if(gVars.preflop_position == c_CO)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ,CO call, so I can't be CO\n");
			return preflopErrorStrategy();
		}

		// AA && KK from all positions (EV+ a lot of dead money)
		if(gVars.isHand("AA KK"))
		{
			return raise_preflop_reraise_amount();
		}

		//Call with all pairs, ev+ if we call too with suited conectors?
		if(gVars.isHand("QQ JJ TT 99 88 77 66 55 44 33 22"))
		{
			return c_call;
		}

		return c_fold;
	}

	// Single raise, raiser raised > 5 BB
	if( (gVars.currentbets[gVars.raischair]/gVars.get_bblind() ) > 5 )
	{
		gLog->WriteLog(eSeverityInfo, eCatPreFlop, "SINGLE RAISER SINGLE CALLER. RAISE IS > 5*bblind\n");

		//UTG
		if(gVars.preflop_position == c_UTG)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ,CO call, so I can't be UTG\n");
			return preflopErrorStrategy();
		}

		//HJ
		if(gVars.preflop_position == c_HJ)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ,CO call, so I can't be HJ\n");
			return preflopErrorStrategy();
		}

		//CO
		if(gVars.preflop_position == c_CO)
		{
			gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: UTG raises and HJ,CO call, so I can't be CO\n");
			return preflopErrorStrategy();
		}

		// AA && KK from all positions (EV+ a lot of dead money)
		if(gVars.isHand("AA KK"))
		{
			return raise_preflop_reraise_amount();
		}

		//Call with all pairs, ev+ if we call too with suited conectors?
		if(gVars.isHand("QQ JJ TT 99 88 77 66 55 44 33 22"))
		{
			return c_call;
		}

		return c_fold;
	}

	gLog->WriteLog(eSeverityError, eCatDebug, "ERROR: Can't determinate raise amount...\n");
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "raischair = %d . currentsbets[raischair] = %.2f (in bblind = %.2f)\n",gVars.raischair,gVars.currentbets[gVars.raischair],gVars.currentbets[gVars.raischair]/gVars.get_bblind());

	return preflopErrorStrategy();
}

double CPreflopLogic::MultipleRaisers()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "MULTIPLE RAISERS\n");

	//Reraised pot? 4bet AA & KK
	if(gVars.X_bet() < 4)
	{
		if(gVars.isHand("AA KK"))
		{
			return raise_preflop_reraise_amount();
		}
	}

	// In 4bet pot -> all in AA or fold
	else
	{
		if(gVars.isHand("AA"))
		{
			return c_allin;
		}
	}

	return c_fold;
}