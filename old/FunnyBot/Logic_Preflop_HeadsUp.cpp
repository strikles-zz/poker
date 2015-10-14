#include "stdafx.h"

#include "Logic_Preflop.h"
#include "Bot_Globalvars.h"



const char* sblind_open = 
{
	"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
	"AKs AQs AJs ATs A9s A8s A7s A6s A5s A4s A3s A2s "
	"KQs KJs KTs K9s K8s K7s K6s K5s K4s K3s K2s "
	"QJs QTs Q9s Q8s Q7s Q6s Q5s Q4s Q3s Q2s "
	"JTs J9s J8s J7s J6s J5s J4s J3s "
	"T9s T8s T7s T6s T5s T4s T3s "
	"98s 97s 96s 95s 94s 93s "
	"87s 86s 85s 84s 83s "
	"76s 75s 74s 73s "
	"65s 64s 63s "
	"54s 53s "
	"43s "
	"AKo AQo AJo ATo A9o A8o A7o A6o A5o A4o A3o A2o "
	"KQo KJo KTo K9o K8o K7o K6o K5o "
	"QJo QTo Q9o Q8o Q7o Q6o "
	"JTo J9o J8o J7o J6o "
	"T9o T8o T7o T6o "
	"98o 97o 96o "
	"87o 86o "
	"76o 75o "
	"65o"
};

const char* sblind_call = 
{
	"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
	"AKs AQs AJs "
	"KQs KJs "
	"QJs "
	"JTs "
	"AKo AQo "
	"KQo"
};

const char* bblind_raise = 
{
	"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
	"AKs AQs AJs ATs A9s A8s A7s A6s A5s A4s A3s "
	"KQs KJs KTs K9s K8s "
	"QJs QTs Q9s "
	"JTs J9s J8s "
	"T9s T8s "
	"98s 97s 96s "
	"87s 86s "
	"76s 75s "
	"65s "
	"AKo AQo AJo ATo A9o A8o A7o "
	"KQo KJo KTo K9o "
	"QJo QTo Q9o "
	"JTo "
	"T9o"
};

const char* bblind_4bet = 
{
	"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
	"AKs AQs AJs ATs "
	"KQs KJs "
	"QJs QTs Q9s "
	"JTs J9s "
	"T9s "
	"98s 97s 96s "
	"87s 86s "
	"76s 75s "
	"AKo AQo AJo "
	"KQo"
};

const char* bblind_call = 
{
	"AA KK QQ JJ TT 99 88 77 66 55 44 33 22 "
	"AKs AQs AJs ATs "
	"KQs KJs "
	"QJs QTs Q9s "
	"JTs J9s "
	"T9s "
	"98s 97s 96s "
	"87s 86s "
	"76s 75s "
	"AKo AQo AJo "
	"KQo"
};



double CPreflopLogic::preflop_HeadsUp()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "Heads-Up ... %d\n", gVars.set_preflop_position());

	if(gVars.set_preflop_position() == c_BB)
	{
		if (gVars.ntimesacted == 0)
		{
			if(gVars.isHand(bblind_raise))
			{
				gLog->WriteLog(eSeverityInfo, eCatPreFlop, "Heads-Up BB Raise...\n");
				return doRaise(4 * gVars.get_bblind());
			}
		}
		else
		{
			if(gVars.isHand(bblind_call))
			{
				gLog->WriteLog(eSeverityInfo, eCatPreFlop, "Heads-Up BB Call...\n");
				return call_value;
			}
		}
	}

	else if(gVars.set_preflop_position() == c_SB)
	{
		if (gVars.ntimesacted == 0)
		{
			if(gVars.isHand(sblind_open))
			{
				gLog->WriteLog(eSeverityInfo, eCatPreFlop, "Heads-Up SB Raise...\n");
				return doRaise(3 * gVars.get_bblind());
			}
		}
		else
		{
			if(gVars.isHand(sblind_call))
			{
				gLog->WriteLog(eSeverityInfo, eCatPreFlop, "Heads-Up SB Call...\n");
				return call_value;
			}
		}
	}

	//If I post blind I check, not fold
	if(gVars.call > 0)
		return c_fold;

	return c_check;
}