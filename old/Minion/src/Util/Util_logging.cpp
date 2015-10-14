/////////////////////////////////////////////////
//////////////// LOGGING ////////////////////////
/////////////////////////////////////////////////

/////////////////////////////
// Includes
/////////////////////////////


#include <conio.h>
#include <ctime>
#include <iostream>

#include "Util/Util_logging.h"
#include "Config/CPreferences.h"




CLogging::CLogging() 
: m_bDirectToConsole(gPrefs.log.get_console()), m_bDirectToFile(gPrefs.log.get_file()), m_ErrorLog(NULL), m_pConsole(0)
{
	for(int i=0; i<eCatLastItem; i++)
		m_bOutputCategory[i] = true;

#ifdef MASTER
	LogSemaphore = NULL;
#endif

	////////////////////
	// Turning off categories
	////////////////////
	m_bOutputCategory[eCatNet] = gPrefs.net.get_debug();
	m_bOutputCategory[eCatNetSend] = gPrefs.net.get_debug();
	m_bOutputCategory[eCatNetRecv] = gPrefs.net.get_debug();
	m_bOutputCategory[eCatANN] = gPrefs.knn.get_debug();
	m_bOutputCategory[eCatDB] = gPrefs.db.get_debug();
	//m_bOutputCategory[eCatDBQuery] = gPrefs..db.get_debug();
	m_bOutputCategory[eCatGeneral] = false;
	m_bOutputCategory[eCatOHMessages] = false;
}

void CLogging::Init()
{
	/*
	*  
	*/

	if(m_bDirectToConsole && !m_pConsole) 
	{
		AllocConsole();
		m_pConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	if(m_bDirectToFile && !m_ErrorLog) 
	{
		char logname[MAX_PATH], _startup_path[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH - 1, _startup_path);
		std::string log_filename_str;

	#ifdef MASTER
		log_filename_str = "%s\\master_%d.log";
	#else
		log_filename_str = "%s\\minion_%d.log";
	#endif

		sprintf_s(logname, log_filename_str.c_str(), _startup_path, GetCurrentProcessId());
		fopen_s(&m_ErrorLog, logname, "a+");
	}
}

void CLogging::Detach()
{
	/*
	*  
	*/

	if(m_pConsole) 
	{
		m_pConsole = 0;
		FreeConsole();
	}
	if(m_ErrorLog) 
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

	if(iSeverity < eSeverityDebug && !m_bOutputCategory[iCategory])
		return;

#ifdef MASTER
	if(LogSemaphore != NULL)
		LogSemaphore->lock();
#endif

	Init();
	bool log_console = m_bDirectToConsole;
	bool log_file = (m_bDirectToFile && m_ErrorLog);
	if(log_console || log_file)
	{
		time_t t = time(&t);
		struct tm ts;
		localtime_s(&ts, &t);
		memset(short_timebuf, 0, sizeof(short_timebuf));
		strftime(short_timebuf, sizeof(short_timebuf), "%X", &ts);
		if(log_console)
		{
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			_cprintf("%s : ", short_timebuf);
		}
		if(log_file)
		{
			fprintf(m_ErrorLog, "%s : ", short_timebuf);
		}
	}

	va_list argList;
	_crt_va_start(argList, _Format);
	DoWriteLog(iSeverity, iCategory, _Format, argList);
	_crt_va_end(argList);

#ifdef MASTER
	if(LogSemaphore != NULL)
		LogSemaphore->unlock();
#endif
}

void CLogging::WriteLogNoHeading(eSeverity iSeverity, eCategory iCategory, const char * _Format, ...)
{
	/*
	*  
	*/


	if(iSeverity < eSeverityDebug && !m_bOutputCategory[iCategory])	
		return;

#ifdef MASTER
	if(LogSemaphore != NULL)
		LogSemaphore->lock();
#endif

	Init();
	va_list argList;
	_crt_va_start(argList, _Format);
	DoWriteLog(iSeverity, iCategory, _Format, argList);
	_crt_va_end(argList);

#ifdef MASTER
	if(LogSemaphore != NULL)
		LogSemaphore->unlock();
#endif
}

void CLogging::DoWriteLog(eSeverity iSeverity, eCategory iCategory, const char * _Format, va_list argList)
{
	/*
	*  
	*/

	if(m_bDirectToFile && m_ErrorLog) 
	{
		vfprintf(m_ErrorLog, _Format, argList); 
		fflush(m_ErrorLog);
	}

	if(m_bDirectToConsole) 
	{
		// gray
		SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);

		if(iCategory == eCatHandHistory) 
		{
			// cyan
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else if(iCategory == eCatHandHistoryBlinds) 
		{
			// cyan
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatHandHistoryMissing) 
		{
			// cyan
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatNewRound) 
		{
			// blue on white
			SetConsoleTextAttribute(m_pConsole, 0 | BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
		else if(iCategory == eCatANN) 
		{
			// white on magenta
			SetConsoleTextAttribute(m_pConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		}
		else if(iCategory == eCatDB) 
		{
			// white on magenta
			SetConsoleTextAttribute(m_pConsole,  0 | BACKGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
		else if(iCategory == eCatDBQuery) 
		{
			// white on magenta
			SetConsoleTextAttribute(m_pConsole,  0 | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatNet) 
		{
			// white on magenta
			SetConsoleTextAttribute(m_pConsole, 0 | BACKGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
		else if(iCategory == eCatOpponentModelling) 
		{
			// yellow
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatCalculator) 
		{
			// white on grey ???
			SetConsoleTextAttribute(m_pConsole,  0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatNetRecv) 
		{
			// light green
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_GREEN);
		}
		else if(iCategory == eCatNetSend) 
		{
			// light green
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_RED | FOREGROUND_GREEN );
		}
		else if(iCategory == eCatNegative) 
		{
			// red
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatPositive) 
		{
			// light green
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatEquityOpp) 
		{
			// green
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		}
		else if(iCategory == eCatPreFlop) 
		{
			// cyan
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatPostFlop) 
		{
			// white on green
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else if(iCategory == eCatDecision) 
		{
			// white on blue
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}


		if(iSeverity == eSeverityDebug) 
		{
			// red on light yellow
			SetConsoleTextAttribute(m_pConsole, 0 | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);

			if(gPrefs.log.get_debug_bip())
			{
				//PlaySound("error.wav",NULL,SND_FILENAME|SND_ASYNC); 
				Beep(300, 400);
			}
		}
		else if(iSeverity >= eSeverityError)
		{
			// yellow on red
			SetConsoleTextAttribute(m_pConsole, 0 | BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			if(gPrefs.log.get_error_bip())
			{
				//PlaySound("error.wav",NULL,SND_FILENAME|SND_ASYNC); 
				Beep(750, 200);
			}
		}

		// Color Fix :P
		const char *pch = strchr(_Format,'\n');
		if(pch!=NULL)
		{
			size_t len = pch - _Format;
			memcpy(szBuffer, _Format, len);
			if (len > 0)
			{
				szBuffer[len] = '\0';
				_vcprintf(szBuffer, argList);
				SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
				_cprintf("\n");
			}
		}
		else
		{
			_vcprintf(_Format, argList);
			SetConsoleTextAttribute(m_pConsole, 0 | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		}

#if defined _DEBUG
		if(iSeverity >= eSeverityError)
			std::cin.get();
#endif
	}
}
