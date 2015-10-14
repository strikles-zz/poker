/////////////////////////////////////////////////
//////////////// LOGGING ////////////////////////
/////////////////////////////////////////////////

/////////////////////////////
// Includes
/////////////////////////////

#include "stdafx.h"

#include <time.h>
#include <conio.h>

#include "Util_logging.h"

double gws(const char* name);

CLogging* CLogging::m_gLog = NULL;

CLogging::CLogging() : 
	m_bDirectToConsole(true),
	m_bDirectToFile(true),
	m_ErrorLog(NULL),
	m_pConsole(0)
{
	for (int i=0; i<eCatLastItem; i++)
	{
		m_bOutputCategory[i] = true;
	}

	////////////////////
	// Turning off categories
	////////////////////

	m_bOutputCategory[eCatGeneral] = false;
	m_bOutputCategory[eCatOHMessages] = false;
}

void CLogging::Init()
{
	/*
	*  
	*/

	if (m_bDirectToConsole && !m_pConsole) 
	{
		AllocConsole();
		m_pConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if (m_bDirectToFile && !m_ErrorLog) 
	{
		char logname[MAX_PATH], _startup_path[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH - 1, _startup_path);
		sprintf_s(logname, "%s\\nl6_%d.log", _startup_path, GetCurrentProcessId());
		fopen_s(&m_ErrorLog, logname, "a+");
	}
}

void CLogging::Detach()
{
	/*
	*  
	*/

	if (m_bDirectToConsole && m_pConsole) 
	{
		FreeConsole();
		m_pConsole = 0;
	}

	if (m_ErrorLog) 
	{
		// I can't seem to call fclose here, the handle must automatically be closed on DLL unloading?
		fclose(m_ErrorLog);
		m_ErrorLog = NULL;
	}
}

void CLogging::WriteLog(eSeverity iSeverity, eCategory iCategory, const char * _Format, ...)
{
	/*
	*  To write logs:
	*		-%d: Integer
	*		-%f: Double (you can use too %.2f, to especify decimals)
	*		-%s: Character (array [] char)
	*/

	if (!m_bOutputCategory[iCategory])
		return;

	Init();

	char timebuf[26];
	time_t ltime;

	//double handNumber = gws("handnumber");
	// Temporary Fix :(
	double handNumber = gws("handsplayed");

	time( &ltime );
	ctime_s(timebuf, 26, &ltime);
	timebuf[24]='\0';

	if (m_bDirectToFile && m_ErrorLog)
		fprintf(m_ErrorLog, "%s [%.0f]: ", timebuf, handNumber);

	if (m_bDirectToConsole)
	{
		char short_timebuf[10];
		struct tm  *ts = localtime(&ltime);

		strftime (short_timebuf, sizeof short_timebuf, "%X", ts);
		SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_INTENSITY);
		_cprintf("%s [%.0f]: ", short_timebuf, handNumber);
	}

	va_list argList;
	_crt_va_start(argList, _Format);

	DoWriteLog(iSeverity, iCategory, _Format, argList);

	_crt_va_end(argList);
}

void CLogging::WriteLogNoHeading(eSeverity iSeverity, eCategory iCategory, const char * _Format, ...)
{
	/*
	*  
	*/

	if (!m_bOutputCategory[iCategory])
		return;

	Init();

	va_list argList;
	_crt_va_start(argList, _Format);

	DoWriteLog(iSeverity, iCategory, _Format, argList);

	_crt_va_end(argList);
}

void CLogging::DoWriteLog(eSeverity iSeverity, eCategory iCategory, const char * _Format, va_list argList)
{
	/*
	*  
	*/

	if (m_bDirectToFile && m_ErrorLog) 
	{
		vfprintf(m_ErrorLog, _Format, argList); 
		fflush(m_ErrorLog);
	}

	if (m_bDirectToConsole) 
	{
		SetConsoleTextAttribute(m_pConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

		if (iCategory == eCatHandHistory) 
		{
			// Original
			SetConsoleTextAttribute(m_pConsole, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatOponentModelling) 
		{
			// Green
			SetConsoleTextAttribute(m_pConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatCalculator) 
		{
			// Green
			SetConsoleTextAttribute(m_pConsole,  0 | BACKGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatEquityHeroPositive) 
		{
			// Light Green
			SetConsoleTextAttribute(m_pConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatEquityHeroNegative) 
		{
			// Light Green
			SetConsoleTextAttribute(m_pConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatEquityOpp) 
		{
			// Green
			SetConsoleTextAttribute(m_pConsole, FOREGROUND_GREEN);
		}
		else if (iCategory == eCatPreFlop) 
		{
			// Cyan
			SetConsoleTextAttribute(m_pConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatPostFlop) 
		{
			// Green BG
			SetConsoleTextAttribute(m_pConsole, BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatPostFlopNegative) 
		{
			// Red
			SetConsoleTextAttribute(m_pConsole, BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatDecision) 
		{
			// Blue BG
			SetConsoleTextAttribute(m_pConsole, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatPerformance) 
		{
			// Gray
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatDebug) 
		{
			// Light Yellow
			SetConsoleTextAttribute(m_pConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if (iCategory == eCatDebugNow) 
		{
			// Light Yellow
			SetConsoleTextAttribute(m_pConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);

#define MAKE_NOISE 42
#if defined(MAKE_NOISE)
			// Requires a file named error.wav in dll dir
			//PlaySound("error.wav",NULL,SND_FILENAME|SND_ASYNC); 
			Beep( 300, 400 );
#endif
		}
		if (iSeverity > eSeverityInfo)
		{
			// Light Red
			SetConsoleTextAttribute(m_pConsole, BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

#if defined(MAKE_NOISE)
			// Requires a file named error.wav in dll dir
			//PlaySound("error.wav",NULL,SND_FILENAME|SND_ASYNC); 
			Beep( 750, 200 );
#endif

		}

		const char *pch = strchr(_Format,'\n');

		if (pch!=NULL)
		{
			char *szBuffer = new char[strlen(_Format)+1];
			strcpy(szBuffer,_Format);
			szBuffer[pch - _Format] = '\0';
			_vcprintf(szBuffer, argList);

			SetConsoleTextAttribute(m_pConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			_cprintf("\n");
			delete[] szBuffer;
		}
		else
		{
			_vcprintf(_Format, argList);
			SetConsoleTextAttribute(m_pConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
}
