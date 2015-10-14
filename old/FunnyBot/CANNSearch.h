#pragma once

#include "CANNObject.h"	
#include "Util_MagicNumbers.h"

#include "struct_defines.h"


class CANNSearch
{
public:

	CANNSearch(int k, int maxPts, double eps);
	~CANNSearch();

public:

	// Public interface
	int GetDecisionFromVector(std::vector<int>& queryPt, enumStreets br, int timesActed);
	int GetDecision(handInfo& hand);

private:

	CANNObject* annPreflop[maxActions];
	CANNObject* annFlop[maxActions];
	CANNObject* annTurn[maxActions];
	CANNObject* annRiver[maxActions];
};

extern CANNSearch gKnn;