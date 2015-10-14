/////////////////////////////////////////////////
//////////////// LOGGING ////////////////////////
/////////////////////////////////////////////////

#pragma once

#include <stdio.h>


enum eSeverity
{ 
	eSeverityInfo, 
	eSeverityError, 
	eSeveritySevere 
};

enum eCategory
{
	eCatInfo,
	eCatGeneral, 
	eCatOHMessages, 
	eCatStates, 
	eCatHandHistory, 
	eCatOponentModelling,
	eCatCalculator,
	eCatEquityHeroPositive, 
	eCatEquityHeroNegative, 
	eCatEquityOpp,
	eCatPreFlop,
	eCatPostFlop,
	eCatPostFlopNegative,
	eCatDecision,
	eCatPerformance, 
	eCatDebug, 
	eCatDebugNow, 
	eCatLastItem 
};


class CLogging
{
public:
	// public methods
	static CLogging* GetInstance()
	{
		if (!m_gLog)
			m_gLog = new CLogging();

		return m_gLog;
	}

	void WriteLog(eSeverity iSeverity, eCategory iCategory, const char* format, ...);
	void WriteLogNoHeading(eSeverity iSeverity, eCategory iCategory, const char* _Format, ...);
	void Detach();

private:
	// Private vars
	bool m_bDirectToConsole, m_bDirectToFile;
	FILE* m_ErrorLog;
	HANDLE m_pConsole;
	bool m_bOutputCategory[eCatLastItem];

	static CLogging* m_gLog;

private:
	// private constructor and destructor
	CLogging();
	~CLogging()
	{
		delete m_gLog;
		Detach();
	}

	// Rule of 3 :
	// Disabling Copy Constructor & assignement operator
	// Is this non-sense for a singleton ?
	CLogging(const CLogging&);
	CLogging& operator=(const CLogging&);
		
	// private functions
	void DoWriteLog(eSeverity iSeverity, eCategory iCategory, const char* _Format, va_list argptr);
	void Init();
};

extern CLogging* gLog;