#include "stdafx.h"

#include "Bot_Globalvars.h"
#include "Util_MagicNumbers.h"



double CGlobalVars::set_rais_chair_pfr() 
{
	/*
	* We set pfr for our raischair before raischair or pfr ar
	*/

	if (raischair >= 0 && raischair < 10)
		return m_PlayerStatistics.opponentInfo[raischair].pt_pfr;

	return 0;
}

double CGlobalVars::set_rais_chair_vpip() 
{
	/*
	* Sets VPIP using our raischair
	*/

	if (raischair >= 0 && raischair < 10)
		return m_PlayerStatistics.opponentInfo[raischair].pt_vpip;

	return 0;
}

double CGlobalVars::set_rais_chair_hands() 
{
	/*
	* sets raischair hands from PT3 with our raischair variable
	*/

	if (raischair >= 0 && raischair < 10)
		return m_PlayerStatistics.opponentInfo[raischair].pt_hands;

	return 0;
}

double CGlobalVars::set_rais_chair_ats()
{
	/*
	* sets raischair ats from PT3 with our raischair variable
	*/

	if (raischair >= 0 && raischair < 10)
		return m_PlayerStatistics.opponentInfo[raischair].pt_pfats;

	return 0;
}


double CGlobalVars::first_limper_vpip()
{
	/*
	*  
	*/

	int f_limper = first_limper();

	if (f_limper >= 0 && f_limper < 5)
		return m_PlayerStatistics.opponentInfo[f_limper].pt_vpip;

	return 0;
}

double CGlobalVars::first_limper_pfr()
{
	/*
	*  
	*/

	int f_limper = first_limper();

	if (f_limper >= 0 && f_limper < 5)
		return m_PlayerStatistics.opponentInfo[f_limper].pt_pfr;

	return 0;
}

double CGlobalVars::first_limper_hands()
{
	/*
	*  
	*/

	int f_limper = first_limper();

	if (f_limper >= 0 && f_limper < 5)
		return m_PlayerStatistics.opponentInfo[f_limper].pt_hands;

	return 0;
}


double CGlobalVars::adj_pfr()
{
	/*
	*  
	*/

	// This function dynamically changes pfr for raischair
	// for the position they are inf to tighten or loosen
	// the pfr stat based on their pfr as a whole
	// and the dealposition they are in
	// also we use the dealposition that is found dynamically
	// This allows our bot to loosen/tighten up as we get fewer/more players
	// and to adjust to position.
	if (rais_chair_hands > 30)
	{
		if (rais_chair_position == 3)
		{
			if (orig_rais_chair_pfr >= 0 && orig_rais_chair_pfr < 5){
				return 3;
			}
			if (orig_rais_chair_pfr >= 5 && orig_rais_chair_pfr < 7){
				return 4;
			}
			if (orig_rais_chair_pfr >= 7 && orig_rais_chair_pfr < 9){
				return 6;
			}
			if (orig_rais_chair_pfr >= 9 && orig_rais_chair_pfr < 11){
				return 8;
			}
			if (orig_rais_chair_pfr >= 11 && orig_rais_chair_pfr < 13){
				return 9;
			}
			if (orig_rais_chair_pfr >= 13 && orig_rais_chair_pfr < 15){
				return 10;
			}
			if (orig_rais_chair_pfr >= 15 && orig_rais_chair_pfr < 17){
				return 12;
			}
			if (orig_rais_chair_pfr >= 17 && orig_rais_chair_pfr < 19){
				return 13;
			}
			if (orig_rais_chair_pfr >= 19 && orig_rais_chair_pfr < 21){
				return 14;
			}
			if (orig_rais_chair_pfr >= 21 && orig_rais_chair_pfr < 23){
				return 15;
			}
			if (orig_rais_chair_pfr >= 23 && orig_rais_chair_pfr < 25){
				return 16;
			}
			if (orig_rais_chair_pfr >= 25 && orig_rais_chair_pfr < 27){
				return 17;
			}
			if (orig_rais_chair_pfr >= 27 && orig_rais_chair_pfr < 29){
				return 18;
			}
			if (orig_rais_chair_pfr >= 29 && orig_rais_chair_pfr < 31){
				return 19;
			}
			if (orig_rais_chair_pfr >= 31){
				return 20;
			}
		}
		else if (rais_chair_position == 4)
		{
			if (orig_rais_chair_pfr >= 0 && orig_rais_chair_pfr < 5){
				return 4;
			}
			if (orig_rais_chair_pfr >= 5 && orig_rais_chair_pfr < 7){
				return 6;
			}
			if (orig_rais_chair_pfr >= 7 && orig_rais_chair_pfr < 9){
				return 7;
			}
			if (orig_rais_chair_pfr >= 9 && orig_rais_chair_pfr < 11){
				return 9;
			}
			if (orig_rais_chair_pfr >= 11 && orig_rais_chair_pfr < 13){
				return 10;
			}
			if (orig_rais_chair_pfr >= 13 && orig_rais_chair_pfr < 15){
				return 11;
			}
			if (orig_rais_chair_pfr >= 15 && orig_rais_chair_pfr < 17){
				return 13;
			}
			if (orig_rais_chair_pfr >= 17 && orig_rais_chair_pfr < 19){
				return 14;
			}
			if (orig_rais_chair_pfr >= 19 && orig_rais_chair_pfr < 21){
				return 15;
			}
			if (orig_rais_chair_pfr >= 21 && orig_rais_chair_pfr < 23){
				return 16;
			}
			if (orig_rais_chair_pfr >= 23 && orig_rais_chair_pfr < 25){
				return 17;
			}
			if (orig_rais_chair_pfr >= 25 && orig_rais_chair_pfr < 27){
				return 18;
			}
			if (orig_rais_chair_pfr >= 27 && orig_rais_chair_pfr < 29){
				return 19;
			}
			if (orig_rais_chair_pfr >= 29 && orig_rais_chair_pfr < 31){
				return 20;
			}
			if (orig_rais_chair_pfr >= 31){
				return 21;
			}
		}

		//for co, bn, sb, and bb coach uses ATS, but
		//i don't want to introduce another stat
		//might try it later and check against pfr

		else if (rais_chair_position == 5)
		{
			if (orig_rais_chair_pfr  < 13){
				return 9;
			}
			if (orig_rais_chair_pfr >= 13 && orig_rais_chair_pfr < 15){
				return 11;
			}
			if (orig_rais_chair_pfr >= 15 && orig_rais_chair_pfr < 17){
				return 13;
			}
			if (orig_rais_chair_pfr >= 17 && orig_rais_chair_pfr < 19){
				return 15;
			}
			if (orig_rais_chair_pfr >= 19 && orig_rais_chair_pfr < 21){
				return 17;
			}
			if (orig_rais_chair_pfr >= 21 && orig_rais_chair_pfr < 23){
				return 19;
			}
			if (orig_rais_chair_pfr >= 23 && orig_rais_chair_pfr < 25){
				return 21;
			}
			if (orig_rais_chair_pfr >= 25 && orig_rais_chair_pfr < 27){
				return 23;
			}
			if (orig_rais_chair_pfr >= 27 && orig_rais_chair_pfr < 29){
				return 25;
			}
			if (orig_rais_chair_pfr >= 29 && orig_rais_chair_pfr < 31){
				return 27;
			}
			if (orig_rais_chair_pfr >= 31){
				return 29;
			}
		}
		else if (rais_chair_position == 6)
		{
			if (orig_rais_chair_pfr  < 13)
			{
				return 14;
			}
			if (orig_rais_chair_pfr >= 13 && orig_rais_chair_pfr < 15){
				return 16;
			}
			if (orig_rais_chair_pfr >= 15 && orig_rais_chair_pfr < 17){
				return 18;
			}
			if (orig_rais_chair_pfr >= 17 && orig_rais_chair_pfr < 19){
				return 20;
			}
			if (orig_rais_chair_pfr >= 19 && orig_rais_chair_pfr < 21){
				return 22;
			}
			if (orig_rais_chair_pfr >= 21 && orig_rais_chair_pfr < 23){
				return 24;
			}
			if (orig_rais_chair_pfr >= 23 && orig_rais_chair_pfr < 25){
				return 26;
			}
			if (orig_rais_chair_pfr >= 25 && orig_rais_chair_pfr < 27){
				return 29;
			}
			if (orig_rais_chair_pfr >= 27 && orig_rais_chair_pfr < 29){
				return 31;
			}
			if (orig_rais_chair_pfr >= 29 && orig_rais_chair_pfr < 31){
				return 33;
			}
			if (orig_rais_chair_pfr >= 31){
				return 35;
			}
		}
		else if (rais_chair_position == 1)
		{
			if (orig_rais_chair_pfr  < 13){
				return 15;
			}
			if (orig_rais_chair_pfr >= 13 && orig_rais_chair_pfr < 15){
				return 17;
			}
			if (orig_rais_chair_pfr >= 15 && orig_rais_chair_pfr < 17){
				return 19;
			}
			if (orig_rais_chair_pfr >= 17 && orig_rais_chair_pfr < 19){
				return 21;
			}
			if (orig_rais_chair_pfr >= 19 && orig_rais_chair_pfr < 21){
				return 23;
			}
			if (orig_rais_chair_pfr >= 21 && orig_rais_chair_pfr < 23){
				return 25;
			}
			if (orig_rais_chair_pfr >= 23 && orig_rais_chair_pfr < 25){
				return 27;
			}
			if (orig_rais_chair_pfr >= 25 && orig_rais_chair_pfr < 27){
				return 30;
			}
			if (orig_rais_chair_pfr >= 27 && orig_rais_chair_pfr < 29){
				return 32;
			}
			if (orig_rais_chair_pfr >= 29 && orig_rais_chair_pfr < 31){
				return 34;
			}
			if (orig_rais_chair_pfr >= 31){
				return 36;
			}
		}
		else if (rais_chair_position == 2)
		{
			if (orig_rais_chair_pfr  < 13)
			{
				return 15;
			}
			if (orig_rais_chair_pfr >= 13 && orig_rais_chair_pfr < 15){
				return 17;
			}
			if (orig_rais_chair_pfr >= 15 && orig_rais_chair_pfr < 17){
				return 19;
			}
			if (orig_rais_chair_pfr >= 17 && orig_rais_chair_pfr < 19){
				return 21;
			}
			if (orig_rais_chair_pfr >= 19 && orig_rais_chair_pfr < 21){
				return 23;
			}
			if (orig_rais_chair_pfr >= 21 && orig_rais_chair_pfr < 23){
				return 25;
			}
			if (orig_rais_chair_pfr >= 23 && orig_rais_chair_pfr < 25){
				return 27;
			}
			if (orig_rais_chair_pfr >= 25 && orig_rais_chair_pfr < 27){
				return 30;
			}
			if (orig_rais_chair_pfr >= 27 && orig_rais_chair_pfr < 29){
				return 32;
			}
			if (orig_rais_chair_pfr >= 29 && orig_rais_chair_pfr < 31){
				return 34;
			}
			if (orig_rais_chair_pfr >= 31){
				return 36;
			}
		}
	}

	else
	{
		if (rais_chair_position == 3){
			return 12;
		}
		else if (rais_chair_position == 4){
			return 13;
		}
		else if (rais_chair_position == 5){
			return 21;
		}
		else if (rais_chair_position == 6){	
			return 26;
		}
		else if (rais_chair_position == 1){
			return 27;
		}
		else if (rais_chair_position == 2){
			return 27;
		}
	}

	return 0;
}