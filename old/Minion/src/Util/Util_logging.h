/////////////////////////////////////////////////
//////////////// LOGGING ////////////////////////
/////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <stdio.h>


#ifdef MASTER
#include "Net/CSemaphore/CSemaphore.h"
#endif


enum eSeverity
{ 
	eSeverityInfo, 
	eSeverityDebug,
	eSeverityError, 
	eSeveritySevere 
};

enum eCategory
{
	eCatANN,
	eCatCalculator,
	eCatData,
	eCatDB,
	eCatDBQuery,
	eCatDecision,
	eCatEquityOpp,
	eCatGeneral, 
	eCatHandHistory,
	eCatHandHistoryBlinds,
	eCatHandHistoryMissing,
	eCatHandPotential,
	eCatInfo,
	eCatLogic,
	eCatNewRound,
	eCatNet,
	eCatNetSend,
	eCatNetRecv,
	eCatOHMessages, 
	eCatOpponentModelling,
	eCatPerformance, 
	eCatPreFlop,
	eCatPostFlop,
	eCatStates, 
	eCatPositive, 
	eCatNegative, 
	eCatLastItem 
};


class CLogging
{
public:
	// public methods

	CLogging();
	~CLogging()
	{
		Detach();
	}

public:
	void WriteLog(eSeverity iSeverity, eCategory iCategory, const char* format, ...);
	void WriteLogNoHeading(eSeverity iSeverity, eCategory iCategory, const char* _Format, ...);
	void Detach();

#ifdef MASTER
	void SetSemaphore(mySemaphore* semaphore) { LogSemaphore = semaphore; }
	mySemaphore* GetSemaphore() { return LogSemaphore; }
#endif

private:
	// Private vars
	bool m_bDirectToConsole, m_bDirectToFile;
	bool m_bOutputCategory[eCatLastItem];
	char szBuffer[2048];
	char short_timebuf[10];
	FILE* m_ErrorLog;
	HANDLE m_pConsole;

private:

#ifdef MASTER
	mySemaphore* LogSemaphore;
#endif
		
	// private functions
	void DoWriteLog(eSeverity iSeverity, eCategory iCategory, const char* _Format, va_list argptr);
	void Init();
};

extern CLogging gLog;