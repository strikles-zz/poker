#include "stdafx.h"

#include <cassert>

#include "CANNSearch.h"
#include "CANNObject.h"	

#include "CDatabase.h"



CANNSearch::CANNSearch(int k, int maxPts, double eps)
{
	for (int i = 0; i < maxActions; i++)
	{
		annPreflop[i]	= new CANNObject(k, maxPts, ePreflop, i, eps);
		annFlop[i]		= new CANNObject(k, maxPts, eFlop, i, eps);
		annTurn[i]		= new CANNObject(k, maxPts, eTurn, i, eps);
		annRiver[i]		= new CANNObject(k, maxPts, eRiver, i, eps);
	}
}

CANNSearch::~CANNSearch()
{
	for (int i = 0; i < maxActions; i++)
	{
		delete annPreflop[i];
		delete annFlop[i];
		delete annTurn[i];
		delete annRiver[i];
	}
}

int CANNSearch::GetDecisionFromVector(std::vector<int>& queryPt, enumStreets br, int timesActed)
{
	int decision_ndx = -1;

	CANNObject* p_search = NULL;

	switch(br)
	{
		case ePreflop:
			p_search = annPreflop[timesActed];
			break;
		case eFlop:
			p_search = annFlop[timesActed];
			break;
		case eTurn:
			p_search = annTurn[timesActed];
			break;
		case eRiver:
			p_search = annRiver[timesActed];
			break;
		default:
			break;
	}

	if (p_search)
		decision_ndx = p_search->GetNdxFromVector(queryPt);

	assert(decision_ndx != -1);

	int decision = -1;
	decision = gDB.GetAction(br, timesActed, decision_ndx);
	assert(decision >= eFold && decision < eTotalActions);

	if(DEBUG_ANN)
		printf("Decision: %s\n", actionStr[decision]);

	return decision;
}

int CANNSearch::GetDecision(handInfo& hand)
{
	assert(hand.br >= ePreflop && hand.br < eRounds);


	// Fill a query Point vector
	std::vector<int> queryPt;
	if (hand.timesActed > 0)
	{
		for (int i = 0; i < hand.timesActed; i++)
			queryPt.push_back(hand.action[i]);
	}
	if (hand.br == ePreflop)
		queryPt.push_back(hand.hand169);
	else
	{
		queryPt.push_back(hand.handType);
		queryPt.push_back(hand.handStrength);
		queryPt.push_back(hand.boardTexture);
	}
	queryPt.push_back(hand.playersInHand);
	queryPt.push_back(hand.playersToAct);
	queryPt.push_back(hand.stackCommitment);
	queryPt.push_back(hand.call);
	queryPt.push_back(hand.pot);


	// Find the right ANN Object
	int decision_ndx = -1;
	CANNObject* p_search = NULL;
	switch(hand.br)
	{
		case ePreflop:
		{
			p_search = annPreflop[hand.timesActed];
			break;
		}		
		case eFlop:
		{
			p_search = annFlop[hand.timesActed];
			break;
		}
		case eTurn:
		{
			p_search = annTurn[hand.timesActed];
			break;
		}
		case eRiver:
		{
			p_search = annRiver[hand.timesActed];
			break;
		}

		default:
			break;
	}

	if (p_search)
		decision_ndx = p_search->GetNdxFromVector(queryPt);

	assert(decision_ndx != -1);

	int decision = -1;
	decision = gDB.GetAction((enumStreets)hand.br, hand.timesActed, decision_ndx);
	assert(decision >= eFold && decision < eTotalActions);

	if(DEBUG_ANN)
		printf("Decision: %s\n", actionStr[decision]);

	return decision;
}