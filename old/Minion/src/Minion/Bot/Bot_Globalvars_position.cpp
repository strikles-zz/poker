
#include "Bot/Bot_Globalvars.h"

#include "Util/Util_MagicNumbers.h"



////////////////////////////
// Position
////////////////////////////
int CGlobalVars::get_acdeal(const int chair)
{
	/*
	*
	*/

	if(chair >= 0 && chair < k_max_chairs) 
		return get_ac_dealpos(chair);

	return -1;
}

int CGlobalVars::sb_chair()
{
	/*
		Finds chair of SB
		Return -1 if no sb
	*/

	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(get_ac_dealpos(chair) == 1)
			return chair;
	}

	return -1;
}

int CGlobalVars::bb_chair()
{
	/*
		Finds chair of SB
		Return -1 if no sb
	*/

	for(int chair = 0; chair < k_max_chairs; chair++)
	{
		if(get_ac_dealpos(chair) == 2)
			return chair;
	}

	return -1;
}

int CGlobalVars::latest_playing_player()
{
	/*
		Finds the player that has the biggest dealposition and it still playing
	*/

	int temp_player = -1;
	for(int i = 0; i < k_max_chairs; i++)
	{
		if(!is_playing(i))
			continue;

		if(get_ac_dealpos(i) > get_acdeal(temp_player))
			temp_player = i;
	}

	return temp_player;
}

int CGlobalVars::previous_player(int dp)
{
	/*
		Gets chair from the players that is playing before us
	*/

	if(dp == 1)
		return latest_playing_player();

	else if(dp == 2)
	{
		int pos_sb_chair = sb_chair();
		if(is_playing(pos_sb_chair))
			return pos_sb_chair;
		else
			return latest_playing_player();
	}

	else if(dp > 2)
	{
		int p_player = -1;
		for(int i = 0; i < k_max_chairs; i++)
		{
			if(!is_playing(i))
				continue;

			if(get_ac_dealpos(i) < dp && dp > p_player)
				p_player = i;
		}

		return p_player;
	}

	//error
	return -1;
} 

int CGlobalVars::set_raischair()
{
	/*
		Our algorithm to find raischair
	*/

	for(int i=0; i < k_max_chairs; i++)
	{
		if(IsEqual(currentbet[i], ncurrentbet) && currentbet[i] > check_currentbet(previous_player(get_ac_dealpos(i))))
			return i;
	}

	return -1;
} 

int CGlobalVars::set_dealpositionrais() 
{
	/*
		We set dealpositonrais because of the bugs in OH's raischair
	*/

	if(raischair < 0)
		return 0;

	return get_ac_dealpos(raischair);
}


bool CGlobalVars::IP()
{
	//////////////////////
	//In position check:
	//////////////////////

	// return true if we are in position

	//If the player is playing and the chair doesn't equal our chair
	// and the player's dealposition is greater than ours then
	//we are not in position

	for(int i=0; i < k_max_chairs; i++)
	{
		if(!is_playing(i))
			continue;

		if(userchair == i)
			continue;

		if(get_ac_dealpos(i) > get_dealposition())
			return false;
	}

	return true;
}

//////////////////////////
// Preflop Position
//////////////////////////
int CGlobalVars::set_preflop_position()
{
	/*
		returns preflop position
		0 - error , 1 to 6 valid preflop position 
		(SB=1, BB=2, UTG=3, HJ=4, CO=5, BT=6)
	*/

	if(get_betround() > 1)
		return 0;

	if(get_nplayersdealt() == 2)
	{
		//IF our DP is UTG return BTN
		if(get_dealposition() == 1)
			return 2;
		if(get_dealposition() == 2)
			return 1;
	}

	//IF we are SB or BB always return the same DP
	else if(get_dealposition() == 1 || get_dealposition() == 2)
		return get_dealposition();

	else if(get_nplayersdealt() == 6)
		return get_dealposition();

	else if(get_nplayersdealt() == 5)
	{
		//IF our DP is UTG return HJ
		if(get_dealposition() == 3)
			return 4;
		//IF our DP is HJ return CO
		else if(get_dealposition() == 4)
			return 5;
		//IF our DP is CO return BTN
		else if(get_dealposition() == 5)
			return 6;
	}
	else if(get_nplayersdealt() == 4)
	{
		//IF our DP is UTG return CO
		if(get_dealposition() == 3)
			return 5;
		//IF our DP is HJ return BTN
		else if(get_dealposition() == 4)
			return 6;
	}
	else if(get_nplayersdealt() == 3)
	{
		//IF our DP is UTG return BTN
		if(get_dealposition() == 3)
			return 6;
	}

	return 0;
}

int CGlobalVars::opponent_position(int opponent_chair)
{

	//this function takes any static chair that is passed
	//returns the adjusted dealposition based on the number 
	//of players dealt, so the bot can make dynamic decisions

	double pre_adjusted_position = 0.0;

	if(opponent_chair >= 0 && opponent_chair < k_max_chairs)
	{
		pre_adjusted_position = get_ac_dealpos(opponent_chair); 
	}

	//Then get the adjusted chair
	if(get_nplayersdealt() > 6 || get_nplayersdealt() < 0) // ???????
	{
		//only for six or less players
		return -1;

	}
	else if(get_nplayersdealt() == 6)
		return pre_adjusted_position;

	else if(get_nplayersdealt() == 5)
	{
		// IF we are SB or BB always return the same DP
		if(pre_adjusted_position == 1 || pre_adjusted_position == 2)
			return pre_adjusted_position;
		//IF our DP is UTG return HJ
		if(pre_adjusted_position == 3)
			return 4;
		//If our DP is HJ return CO
		else if(pre_adjusted_position == 4)
			return 5;
		//if our DP is CO return BTN
		else if(pre_adjusted_position == 5)
			return 6;
	}
	else if(get_nplayersdealt() == 4)
	{
		//IF we are SB or BB always return the same DP
		if(pre_adjusted_position == 1 || pre_adjusted_position == 2)
			return pre_adjusted_position;
		//IF our DP is UTG return CO
		if(pre_adjusted_position == 3)
			return 5;
		//If our DP is HJ return BTN
		else if(pre_adjusted_position == 4)
			return 6;
	}
	else if(get_nplayersdealt() == 3)
	{
		// IF we are SB or BB always return the same DP
		if(pre_adjusted_position == 1 || pre_adjusted_position == 2)
			return pre_adjusted_position;
		//IF our DP is UTG return BTN
		if(pre_adjusted_position == 3)
			return 6;
	}

	return 0;
}

int CGlobalVars::pf_pos_rais()
{
	/*
		Only accounts for 6-max
		Change-me !
	*/

	if(get_nplayersdealt() == 6)
		return dealpositionrais;

	else if(get_nplayersdealt() == 5)
	{
		//IF we are SB or BB always return the same DP
		if(dealpositionrais == 1 || dealpositionrais == 2)
			return dealpositionrais;
		//IF our DP is UTG return HJ
		if(dealpositionrais == 3)
			return 4;
		//IF our DP is HJ return CO
		else if(dealpositionrais == 4)
			return 5;
		//IF our DP is CO return BTN
		else if(dealpositionrais == 5)
			return 6;
	}
	
	else if(get_nplayersdealt() == 4)
	{
		//IF we are SB or BB always return the same DP
		if(dealpositionrais == 1 || dealpositionrais == 2)
			return dealpositionrais;
		//IF our DP is UTG return CO
		if(dealpositionrais == 3)
			return 5;
		//IF our DP is HJ return BTN
		else if(dealpositionrais == 4)
			return 6;
	}
	
	else if(get_nplayersdealt() == 3)
	{
		//IF we are SB or BB always return the same DP
		if(dealpositionrais == 1 || dealpositionrais == 2)
			return dealpositionrais;
		//IF our DP is UTG return BTN
		if(dealpositionrais == 3)
			return 6;
	}

	return 0;
}

int CGlobalVars::pf_raiser_position() 
{
	/*
	* return preflop raiser position
	* no raiser: -1
	*/

	if(get_ncallbets() < 1)
		return -1; //no raiser
	if(get_nplayersdealt() == 6 && dealpositionrais == get_nplayersdealt()-3) 
		return 3; //UTG
	if(get_nplayersdealt() >= 5 && dealpositionrais == get_nplayersdealt()-2) 
		return 4; //HJ
	if(get_nplayersdealt() >= 4 && dealpositionrais == get_nplayersdealt()-1) 
		return 5; //CO
	if(get_nplayersdealt() >= 3 && dealpositionrais == get_nplayersdealt()) 
		return 6; //BTN

	return 1; //SB
}

int CGlobalVars::first_limper_position()
{
	/*
	*  return the adjusted first limper position
	*/

	//get first limper chair
	int f_limper = first_limper();

	return opponent_position(f_limper);
}