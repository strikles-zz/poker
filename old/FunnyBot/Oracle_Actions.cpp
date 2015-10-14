/////////////////////////////////////////////////
///////////// DECISION PREDICTION ///////////////
/////////////////////////////////////////////////

/////////////////////////////
// Includes
/////////////////////////////
#include "stdafx.h"

#include "Bot_Globalvars.h"
#include "Bot_PlayerInfo.h"

#include "Ranges.h"
#include "Oracle_Actions.h"

//CDecisionPrediction* CDecisionPrediction::m_prediction = NULL;

////////////////////////////////////////////////////////////////////////
//PREDICTIONS NOT INCLUDING HERO BETAMMOUNT & EST. RAIS/RERAIS AMMOUNT
///////////////////////////////////////////////////////////////////////
CDecisionPrediction::CDecisionPrediction()
{
}

////////////////////FLOP PREDICTION/////////////////////////////
double CDecisionPrediction::floprais(int iPlayer)
{
	/*
	*  
	*/
	//his equity times number of opponents times his aggression factor
	return gVars.outResults[iPlayer] * gVars.nopponents * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggf;     
}

double CDecisionPrediction::flopfold(int iPlayer)
{
	/*
	*  
	*/

	return (gVars.outResults[iPlayer]*gVars.nopponents)* //his equity times the opponents
		(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_floppct - gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_turnpct)//times his usual round fold pct
		- floprais(iPlayer); //minus the part where he raises
	//consistency minus the prrais
}

double CDecisionPrediction::flopcall(int iPlayer)
{
	/*
	*  
	*/

	//whatever is left to make it sum 1
	return 1 - (flopfold(iPlayer) + floprais(iPlayer)); 
}



/////////////////////TURN PREDICTION/////////////////////////////
double CDecisionPrediction::turnrais(int iPlayer)
{
	/*
	*  
	*/

	//his equity times the opponents times his aggression factor
	return gVars.outResults[iPlayer]*gVars.nopponents * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggt;     
}

double CDecisionPrediction::turnfold(int iPlayer)
{
	/*
	*  
	*/

	return (gVars.outResults[iPlayer]*gVars.nopponents)* //his equity times the opponents
		(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_turnpct - gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_riverpct)//times his usual round fold pct
		- turnrais(iPlayer); //minus the part where he raises
	//consistency minus the prrais
}
double CDecisionPrediction::turncall(int iPlayer)
{
	/*
	*  
	*/

	//whatever is left to make it sum 1
	return 1 - (turnfold(iPlayer) + turnrais(iPlayer)); 
}



/////////////////////RIVER PREDICTION/////////////////////////////
double CDecisionPrediction::riverrais(int iPlayer)
{
	/*
	*  
	*/

	//his equity times the opponents times his aggression factor
	return gVars.outResults[iPlayer]*gVars.nopponents * gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggr;     
}

double CDecisionPrediction::riverfold(int iPlayer)
{
	/*
	*  
	*/

	return (gVars.outResults[iPlayer]*gVars.nopponents)* //his equity times the opponents
		(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_riverpct - gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_wsdp)//times his usual round fold pct
		- riverrais(iPlayer); //minus the part where he raises
}

double CDecisionPrediction::rivercall(int iPlayer)
{
	/*
	*  
	*/

	//whatever is left to make it sum 1
	return 1 - (riverfold(iPlayer) + riverrais(iPlayer));
}





///////////////////////////////////////////////////////////////////
//PREDICTIONS INCLUDING HERO BETAMMOUNT & EST. RAIS/RERAIS AMMOUNT
///////////////////////////////////////////////////////////////////



////////////////////FLOP PREDICTION/////////////////////////////
double CDecisionPrediction::floprais_betamount(int iPlayer, double herobetammount, double &opponentbetammount)
{
	/*
	*  
	*/

	//my ammount + the pot (a.k.a: new pot) his equity times the opponents 
	opponentbetammount = (herobetammount + gVars.get_pot())	* gVars.outResults[iPlayer] * gVars.nopponents;													

	if (herobetammount == 0)
		return floprais(iPlayer);

	return (0.5*(herobetammount/gVars.get_pot())+0.5)												//potpct counts for 50%..
		*gVars.outResults[iPlayer]*gVars.nopponents													//his equity times the opponents
		*gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggf/4;								//this should become rerais pct!!!!
}

double CDecisionPrediction::flopfold_betamount(int iPlayer, double betammount)
{
	/*
	*  
	*/

	double dump; //dump..

	return (0.5*(betammount/gVars.get_pot())+0.5)														//potpct counts for 50%..
		*(gVars.outResults[iPlayer]*gVars.nopponents)												//his equity times the opponents
		*(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_floppct - gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_turnpct)			//times his usual round fold pct
		- floprais_betamount(iPlayer, betammount, dump);											//minus the part where he raises
	
	//consistency minus the prrais
}

double CDecisionPrediction::flopcall_betamount(int iPlayer, double betammount)
{
	/*
	*  
	*/

	double dump;

	//whatever is left to make it sum 1
	return 1 - (flopfold_betamount(iPlayer, betammount) + floprais_betamount(iPlayer, betammount, dump)); 
}



/////////////////////TURN PREDICTION/////////////////////////////
double CDecisionPrediction::turnrais_betamount(int iPlayer, double herobetammount, double &opponentbetammount)
{
	/*
	*  
	*/

	//my ammount + the pot (a.k.a: new pot) his equity times the opponents 
	opponentbetammount = (herobetammount + gVars.get_pot()) * gVars.outResults[iPlayer]*gVars.nopponents;														

	if (herobetammount == 0)
		return turnrais(iPlayer);

	return (0.5*(herobetammount/gVars.get_pot())+0.5)													//potpct counts for 50%..
		*gVars.outResults[iPlayer]*gVars.nopponents														//his equity times the opponents
		*gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggt/4;									//this should become rerais pct!!!!
}

double CDecisionPrediction::turnfold_betamount(int iPlayer, double betammount)
{
	/*
	*  
	*/

	double  dump;
	
	return (0.5*(betammount/gVars.get_pot())+0.5)															//potpct counts for 50%..
		*(gVars.outResults[iPlayer]*gVars.nopponents)*													//his equity times the opponents
		(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_turnpct - gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_riverpct)			//times his usual round fold pct
		- turnrais_betamount(iPlayer, betammount, dump);												//minus the part where he raises
																															//consistency minus the prrais
}

double CDecisionPrediction::turncall_betamount(int iPlayer, double betammount)
{
	/*
	*  
	*/

	double dump;

	//whatever is left to make it sum 1
	return 1 - (turnfold_betamount(iPlayer, betammount) + turnrais_betamount(iPlayer, betammount, dump));	
}



/////////////////////RIVER PREDICTION/////////////////////////////
double CDecisionPrediction::riverrais_betamount(int iPlayer, double herobetammount, double &opponentbetammount)
{
	/*
	*  
	*/

	//my ammount + the pot (a.k.a: new pot) his equity times the opponents 
	opponentbetammount = (herobetammount + gVars.get_pot())	* gVars.outResults[iPlayer]*gVars.nopponents;												

	if (herobetammount == 0)
		return riverrais(iPlayer);

	return (0.5*(herobetammount/gVars.get_pot()) + 0.5)											//potpct counts for 50%..
		*gVars.outResults[iPlayer]*gVars.nopponents												//his equity times the opponents
		*gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_aggr/4;							//this should become rerais pct!!!!
}

double CDecisionPrediction::riverfold_betamount(int iPlayer, double betammount)
{
	/*
	*  
	*/

	double dump;

	return (0.5*(betammount/gVars.get_pot())+0.5)													//potpct counts for 50%..
		*(gVars.outResults[iPlayer]*gVars.nopponents)*											//his equity times the opponents
		(gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_riverpct - gVars.m_PlayerStatistics.opponentInfo[iPlayer].pt_wsdp)		//times his usual round fold pct
		- riverrais_betamount(iPlayer, betammount, dump);										//minus the part where he raises
}

double CDecisionPrediction::rivercall_betamount(int iPlayer, double betammount)
{
	/*
	*  
	*/

	double dump;

	//whatever is left to make it sum 1
	return 1 - (riverfold_betamount(iPlayer,betammount) + riverrais_betamount(iPlayer,betammount, dump));		
}



///////////////////////////
// PREDICTION TEST STUFF //
///////////////////////////
void CDecisionPrediction::prediction()
{
	/*
	*  
	*/
	// Some crap pessimistic rules
	for (int iPlayer = 0; iPlayer < 10; iPlayer++)
	{
		if(iPlayer != gVars.userchair && gVars.m_CurrentHandInfo.m_PlayerActions[iPlayer].bWasInGame)
		{
			gVars.m_PlayerStatistics.opponentInfo[iPlayer].pr_rais = 0.33;	//floprais(iPlayer);
			gVars.m_PlayerStatistics.opponentInfo[iPlayer].pr_fold = 0.33;	//flopfold(iPlayer);
			gVars.m_PlayerStatistics.opponentInfo[iPlayer].pr_call = 0.33;	//flopcall(iPlayer);

			switch(gVars.br)
			{
				case 2:
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_rais = 0.5;	//floprais(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_fold = 0.3;	//flopfold(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_call = 0.2;	//flopcall(iPlayer);
		
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_rais = 0.8;	//floprais(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_fold = 0.0;	//flopfold(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_call = 0.2;	//flopcall(iPlayer);
					break;

				case 3:
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_rais = 0.5;	//floprais(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_fold = 0.3;	//flopfold(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_call = 0.1;	//flopcall(iPlayer);
		
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_rais = 0.8;	//floprais(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_fold = 0.0;	//flopfold(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_call = 0.2;	//flopcall(iPlayer);
					break;

				case 4:
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_rais = 0.6;	//floprais(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_fold = 0.1;	//flopfold(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].rais_pr_call = 0.3;	//flopcall(iPlayer);
		
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_rais = 0.8;	//floprais(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_fold = 0.0;	//flopfold(iPlayer);
					gVars.m_PlayerStatistics.opponentInfo[iPlayer].call_pr_call = 0.2;	//flopcall(iPlayer);
					break;

				default:
					break;
			}
		}
	}
}