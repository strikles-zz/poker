#include "Net/CSemaphore/CSemaphore.h"

#include "Net/CException/CException.h"

mySemaphore::mySemaphore(string& sName,long initialCount,long maximumCount,int accessMode,BOOL canInherit)
{
	// initialization
	sa = NULL;
	semaphore = NULL;
	semaphoreName = "";
	initCount = 0;
	maxCount  = 0;
	inherit   = TRUE;

	// create it
	if ( accessMode == CREATE )
    {
		// security handling
		sa = (LPSECURITY_ATTRIBUTES)HeapAlloc(GetProcessHeap(),0,sizeof(SECURITY_ATTRIBUTES));

		if(!sa)
		{
			int errCode = 0;
			string header = "sa setup myEvent::myEvent() failed";
			getErrorDetail(errCode,header);
			myException excp(errCode,header);
			throw excp;
		}

        sa->nLength = sizeof(SECURITY_ATTRIBUTES);
        sa->lpSecurityDescriptor = NULL; //no restrictions
        sa->bInheritHandle = canInherit;

		// make sure the count is legal
		// normally, you want to create the semaphore will an initialcount = 0;
		// therefore, the original state of the semaphore is locked.
		// once the system finishes all the initialization work, it can
		// call unlock() to increase it to be 1, so other threads can
		// start to use it
		if ( maximumCount < initialCount ) 
		{
			maximumCount = 1;
			initialCount = 0;
		}

		// create the semaphore
        if ( (semaphore = CreateSemaphore(sa,initialCount,maximumCount,sName.c_str())) == NULL)
        {
			int errCode = 0;
			string header = "CreateSemaphore failed";
			getErrorDetail(errCode,header);
			myException excp(errCode,header);
            throw excp;
            return;
        }

		// save the key values
		initCount = initialCount;
		maxCount  = maximumCount;
		inherit   = canInherit;

		// save the name, if specified
		if ( sName.compare("") ) semaphoreName.append(sName);

	}

	// open it
    else
    {
        if ((semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS,canInherit,sName.c_str())) == NULL)
        {
			int errCode;
			string header = "OpenSemaphore failed";
			getErrorDetail(errCode,header);
            myException excp(errCode,header);
			throw excp;
            return;
        }
    }
}

mySemaphore::~mySemaphore()
{
	if (sa) HeapFree(GetProcessHeap(),0,sa);
	if ( semaphore ) closeSemaphore();
}

void mySemaphore::getErrorDetail(int& errCode,string& header)
{
	char buf[256];
	LPVOID lpDetailMsgBuf;
	UINT errorCode;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,errorCode=GetLastError(),MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				  (LPTSTR)&lpDetailMsgBuf,0,NULL);

	memset(buf,0,sizeof(buf));
	wsprintf(buf,": %s",(LPTSTR)lpDetailMsgBuf);
    LocalFree(lpDetailMsgBuf);	
	
	header.append(string(buf));
	errCode = errorCode;
}

void mySemaphore::closeSemaphore()
{
    if ( !CloseHandle(semaphore) )
    {
		int errCode = 0;
		string header = "CloseHandle failed";
		getErrorDetail(errCode,header);
        myException excp(errCode,header);
		throw excp;
    }
	semaphore = NULL;
}

BOOL mySemaphore::lock(BOOL wait)
{
	if ( wait == TRUE )
	{
		if ( WaitForSingleObject(semaphore,INFINITE) == WAIT_FAILED )
		{
			int errCode = 0;
			string header = "WaitForSingleObject failed";
			getErrorDetail(errCode,header);
			myException excp(errCode,header);
			throw excp;
		}
		return TRUE;
	}
	else 
	{
		DWORD retVal = WaitForSingleObject(semaphore,0);

		if ( retVal == WAIT_TIMEOUT ) return FALSE;
		if ( retVal == WAIT_OBJECT_0) return TRUE;

		if ( retVal == WAIT_FAILED )
		{
			int errCode = 0;
			string header = "WaitForSingleObject failed";
			getErrorDetail(errCode,header);
			myException excp(errCode,header);
			throw excp;
		}
	}

	return FALSE;
}

void mySemaphore::unlock()
{
	if ( !ReleaseSemaphore(semaphore,1,NULL) )
	{
		int errCode = 0;
		string header = "ReleaseSemaphore failed: ";
		header.append(semaphoreName);
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
		throw excp;
	}
}

/*
long mySemaphore::getSemaphoreCount()
{
	long currCount = 0;

	if ( !ReleaseSemaphore(semaphore,0,&currCount) )
	{
		int errCode = 0;
		string header = "ReleaseSemaphore failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
		throw excp;
	}
    return currCount;
}
*/