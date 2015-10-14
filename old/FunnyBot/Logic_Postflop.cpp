#include "stdafx.h"

#include "stdafx.h"
#include "Logic_Postflop.h"

#pragma once

#include "stdafx.h"
#include "Logic.h"
#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"



CPostflopLogic::CPostflopLogic()
{
}

int CPostflopLogic::FlopDecision()
{
	/*
	*  
	*/

	/////////////////////////////
	// Preflop Control Decision
	/////////////////////////////

	if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][0].act == NoAction)
		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Bot action at Preflop = NoAction\n");

	/////////////////////////////
	// Calculation
	/////////////////////////////
	int flop_decision = EV_decision();

	return flop_decision;
}

int CPostflopLogic::TurnDecision()
{
	/*
	*  
	*/

	/////////////////////////////
	// Flop Control Decision
	/////////////////////////////

	if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][0].act == NoAction)
		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Bot action at Preflop = NoAction\n");

	if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[1][0].act == NoAction)
		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Bot action at Flop = NoAction\n");

	/////////////////////////////
	// Calculation
	/////////////////////////////
	int turn_decision = EV_decision();

	return turn_decision;
}

int CPostflopLogic::RiverDecision()
{
	/*
	*  
	*/

	/////////////////////////////
	// River Control Decision
	/////////////////////////////

	if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[0][0].act == NoAction)
		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Bot action at Preflop = NoAction\n");

	if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[1][0].act == NoAction)
		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Bot action at Flop = NoAction\n");

	if(gVars.m_CurrentHandInfo.m_PlayerActions[gVars.userchair].actions[2][0].act == NoAction)
		gLog->WriteLog(eSeverityInfo, eCatDebug, "WARNING: Bot action at Turn = NoAction\n");

	/////////////////////////////
	// Calculation
	/////////////////////////////
	int river_decision = EV_decision();

	return river_decision;
}

