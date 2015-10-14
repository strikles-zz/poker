#pragma once

#include "Util_MagicNumbers.h"


typedef struct
{
	int hand_id;
	int br;
	int timesActed;
	int action[maxActions];
	int hand169;
	int handType;
	int handStrength;
	int boardTexture;
	int playersInHand;
	int playersToAct;
	int stackCommitment;
	int call;
	int pot;

} handInfo;