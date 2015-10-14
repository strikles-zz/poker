
#include "Logic/Logic.h"

#include "Bot/Bot_Globalvars.h"


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

	int br = gVars.get_betround();

	if(debug_mode)
	{
		return -1;
	}
	else
	{
		// disabled 3 out of 4 seconds 
		if(gVars.get_elapsed() % 4)
		{ 

			bool activeRule = false;
			//------------------------------------------------------------------------
			//----------------------- leave the table --------------------------------
			//------------------------------------------------------------------------
			// LEAVE RULE - Game broken up
			activeRule = false; // false=off, true=on
			if(activeRule)
			{
				if(gVars.get_issittingout() && gVars.get_nopponentsdealt() <= 2 && (br == 2 || br == 3))
				{
					return -2;
				}
			}
			//------------------------------------------------------------------------

			// LEAVE RULE - Wait around, but get out before you get booted
			activeRule = false; // false=off, true=on 
			if(activeRule)
			{
				if(gVars.get_issittingout() && gVars.get_elapsedauto() >= 100 * gVars.get_nopponentsdealt() && (br == 2 || br == 3))
				{
					return -2;
				}
			}
			//------------------------------------------------------------------------

			// SITIN RULE - Based on players
			activeRule = false; // false=off, true=on
			if(activeRule)
			{
				if(gVars.get_issittingout() && gVars.get_nchairsdealtright()==2 && gVars.get_nopponentsdealt() >=5  && (br == 2 || br == 3))
				{
					return 1;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on players
			activeRule = false; // false=off, true=on
			if(activeRule)
			{
				if(gVars.get_issittingin() && gVars.get_nopponentsdealt()>=5  && br==2 )
				{
					return 0;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on Time
			activeRule = false; // false=off, true=on
			if(activeRule)
			{
				//playing 6 hours
				if(gVars.get_issittingin() && gVars.get_elapsed() > 21600 && (br == 2 || br == 3))
				{ 
					return 0;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on Stack
			activeRule = false; // false=off, true=on
			if(activeRule)
			{
				//deep stack
				if(gVars.get_issittingin() && ( gVars.balance[gVars.userchair]/gVars.get_bblind() ) > 300 * gVars.get_bblind() && br == 1)
				{ 
					return 0;
				}
			}
			//------------------------------------------------------------------------

			// SITOUT RULE - Based on Stack
			activeRule = false; // false=off, true=on
			if(activeRule)
			{
				//short stack
				if(gVars.get_issittingin() && ( gVars.balance[gVars.userchair]/gVars.get_bblind() ) < 50 * gVars.get_bblind() && br == 1)
				{ 
					return 0;
				}
			}
			//------------------------------------------------------------------------

		}

		return -1;

	}
}