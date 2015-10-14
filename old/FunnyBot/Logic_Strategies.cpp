#include "StdAfx.h"
#include "Logic_Strategies.h"


CBaseStrategy::CBaseStrategy()
{
}


CBaseStrategy::~CBaseStrategy()
{
}



// CStealBlinds
CStealBlinds::CStealBlinds()
{
}

bool CStealBlinds::PrerequisiteMet(CGlobalVars *pState)
{
	return false;
}

int CStealBlinds::StrategyAction(CGlobalVars *pState)
{
	return 0;
}

bool CStealBlinds::ContinueStrategy(CGlobalVars *pState)
{
	return false;
}

void CStealBlinds::StatsNewHandStarted()
{
}
