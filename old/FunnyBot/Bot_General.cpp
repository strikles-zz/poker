#include "stdafx.h"

#include "Bot_General.h"
#include "Bot_GlobalVars.h"



double gws(int chair, const char* name, bool& iserr)
{
	/*
	* Get OpenHoldem Symbol (this function is usually not used)
	*/

	if (!gVars.m_pget_winholdem_symbol)
		return -1;

	return (*gVars.m_pget_winholdem_symbol)(chair,name,iserr); 
} 

double gws(int chair, const char* name)
{
	/*
	* Get OpenHoldem Symbol (this function is usually used in init_oh_symbol), only does one call
	*/

	if(!gVars.m_pget_winholdem_symbol)
		return -1;

	return (*gVars.m_pget_winholdem_symbol)(chair,name,gVars.iserr); 
} 

double gws(const char* name)
{
	if (gVars.userchair < 0)
		gVars.userchair = static_cast<int>(gws(0,"userchair"));

	return gws(gVars.userchair, name, gVars.iserr); 
}