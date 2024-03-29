
#include <cassert>

#include "Bot/Bot_DLL.h"

#include "Bot/Bot_Globalvars.h"
#include "Bot/Bot_State.h"

#include "Calc/CHandPotential.h"

//#include "DB/CDatabase.h"
//#include "DB/CDatabaseInterface.h"
#include "Config/CPreferences.h"
#include "Logic/Logic.h"
#include "Util/Util_logging.h"

#include "CClientNet.h"

double gws(const char* name);


CPreferences gPrefs;
CLogging gLog;
CClientNet gNet;
CGlobalVars gVars;
CBotLogic gBotLogic;


///////////////////////////////// 
//DLLMAIN 
///////////////////////////////// 
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	/*
	*  DllMain is used to start all the work that must be done to put your bot in well defined and workable state. Things like calling
	*  static functions, load Look Up tables and construct your objects are best done here....
	*/


	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			gLog.WriteLogNoHeading(eSeverityInfo, eCatGeneral, "DLL ATTACHING DONE.. \n");
			break;
		}	
		case DLL_THREAD_ATTACH:
		{
			// Disabled in line (1)
			break;
		}
		case DLL_THREAD_DETACH:
		{
			// Disabled in line (1)
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			gLog.WriteLogNoHeading(eSeverityInfo, eCatGeneral, "DLL DETACH DONE.. \n");
			gLog.Detach();
			break;
		}
	}

	return TRUE;
}

double process_state(holdem_state* p_state)
{
	// Will not try to extract state
	if(p_state == NULL) 
		return 0;

	gVars.init_userchair();
	gVars.CriticalOHVars();

	if(!gVars.m_state.WillProcessState(*p_state))
		return 0;

	//State different, proceed ...
	gVars.m_state.m_ndx++;
	gVars.m_state.m_holdem_state[gVars.m_state.m_ndx] = *p_state;
	gVars.calc_oh_symbols();

	if(gVars.get_bblind() <= 0) 
	{
		// Ignore this frame as if it didn't exist
		gVars.m_state.m_ndx--;
		gLog.WriteLog(eSeverityError, eCatGeneral, "bblind is invalid cannot continue\n");

		return 0;
	}

	gVars.m_state.ExtractState(*p_state);
	int is_wait = 0;

	

	return 1;
}

double process_query(const char* pquery)
{
	/*
	*  
	*/

	int is_wait = 0;
	gLog.WriteLog(eSeverityInfo, eCatGeneral, "Process query started..\n");

	if(pquery == NULL)
		return 0;

	if(strcmp(pquery, "dll$decision")==0)
	{
		double ret = 0.0;

		if(gVars.get_ismyturn())
		{
			is_wait = 1;

			// Dummy Action array Test Fnc. Delete_Me :P
			if(gVars.enabled_flag[0])
				gVars.display_actions();

			gBotLogic.FindDecision();
			gVars.RecordBotsDecision(gBotLogic.actionType, gBotLogic.betType, gBotLogic.decision);

			gLog.WriteLog(eSeverityInfo, eCatGeneral, "Decision returned: %.2f\n", gBotLogic.decision);
			ret = gBotLogic.decision;
			///////////////////////////////////////
			// SWAG ADJUST
			///////////////////////////////////////
			//only if i raise (but not if i go all in)
			if(gBotLogic.actionType == eAction_R)
				ret = gBotLogic.decision;
			else if(gBotLogic.actionType == eAction_C)
				ret = call_value;
			else
				ret = 0.0;

			if(ret > call_value)
			{
				double adjusted_bet = 0.0;
				//f$srai
				if(gVars.get_swagtextmethod() == 1)
					adjusted_bet = ret;
				//f$srai + call
				else if(gVars.get_swagtextmethod() == 2)
					adjusted_bet = ret - gVars.call;
				//f$srai + call + currentbet
				else if(gVars.get_swagtextmethod() == 3)
					adjusted_bet = ret - gVars.call - gVars.currentbet[gVars.userchair];

				ret = adjusted_bet;
			}

			is_wait = 0;
			assert(ret >= 0);
		}

		return ret;
	}

	if(strcmp(pquery, "dll$play")==0)
	{	
		int ret = gBotLogic.play();
		gLog.WriteLog(eSeverityInfo, eCatGeneral, "f$play returned: %d\n", ret);
		return ret;
	}
	if(strcmp(pquery, "dll$israis")==0)
	{
		gLog.WriteLog(eSeverityInfo, eCatGeneral, "israis returned: %.2f\n", gVars.israis);
		return gVars.israis;
	}
	/*
	if(strcmp(pquery, "dll$prwin")==0)
	{
		gLog.WriteLog(eSeverityInfo, eCatGeneral, "prwin returned: %.2f\n", gVars.prw_xpoker_equity);
		return gVars.prw_xpoker_equity;
	}
	if(strcmp(pquery, "dll$boardvar")==0)
	{
		gLog.WriteLog(eSeverityInfo, eCatGeneral, "boardvar returned: %.2f\n", gVars.israis);
		return gVars.var_prwin;
	}
	*/
	if(strcmp(pquery,"dll$isconnected")==0) 
	{
		gLog.WriteLog(eSeverityInfo, eCatGeneral, "isconnected returned: 1\n");
		return 1;
	}
	if(strcmp(pquery,"dll$iswait")==0)
	{
		return is_wait;
	}

	return 0;
}

///////////////////////////////////////////////////// 
//WINHOLDEM RUNTIME ENTRY POINT 
///////////////////////////////////////////////////// 
WHUSER_API double process_message(const char* pmessage, const void* param)
{
	/*
	*  
	*/

	if(pmessage==NULL)
	{
		gLog.WriteLog(eSeverityError, eCatOHMessages, "pmessage==NULL.. \n");
		return 0;
	}
	if(param==NULL)
	{
		gLog.WriteLog(eSeverityError, eCatOHMessages, "param==NULL.. \n");
		return 0;
	}

	gLog.WriteLogNoHeading(eSeverityInfo, eCatGeneral, "PROCESS Message.. %s, Param: %s\n", pmessage, (const char*)param);

	if(strcmp(pmessage,"state")==0)
	{
		gLog.WriteLogNoHeading(eSeverityInfo, eCatOHMessages, "PROCESS STATE..\n\tParam: %s\n", (const char*)param);
		double ret = process_state( (holdem_state*)param );
		gLog.WriteLogNoHeading(eSeverityInfo, eCatOHMessages, "PROCESS STATE FINISHED.. %.2f\n", ret);
		return ret;
	}
	if(strcmp(pmessage,"query")==0)
	{ 
		gLog.WriteLogNoHeading(eSeverityInfo, eCatOHMessages, "RUN QUERY..\nParam: %s\n", (const char*)param);		
		double ret = process_query((const char*)param);
		gLog.WriteLogNoHeading(eSeverityInfo, eCatOHMessages, "PROCESS QUERY FINISHED.. %.2f\n", ret);
		return ret;
	}
	if(strcmp(pmessage,"pfgws")==0)
	{
		gVars.m_pget_winholdem_symbol = (pfgws_t)param;
		return 0;
	}

	return 0;
}
