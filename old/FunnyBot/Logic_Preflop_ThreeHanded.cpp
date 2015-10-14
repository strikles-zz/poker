#include "stdafx.h"

#include "Logic_Preflop.h"
#include "Bot_Globalvars.h"



double CPreflopLogic::preflop_ThreeHanded()
{
	gLog->WriteLog(eSeverityInfo, eCatPreFlop, "3-handed ... %d\n", gVars.set_preflop_position());

	return c_check;
}