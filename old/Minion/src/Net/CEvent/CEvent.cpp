#include "Net/CEvent/CEvent.h"
#include "Net/CException/CException.h"

myEvent::myEvent(string eventName,PAPCFUNC _pfnAPC,BOOL _queueUserAPC)
{
	// set up the security features
    sa= (LPSECURITY_ATTRIBUTES) HeapAlloc(GetProcessHeap(),0,sizeof(SECURITY_ATTRIBUTES));
	if(!sa)
	{
		int errCode = 0;
		string header = "sa setup myEvent::myEvent() failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
        throw excp;
	}

    sa->nLength= sizeof(SECURITY_ATTRIBUTES);
    sa->lpSecurityDescriptor= NULL;
    sa->bInheritHandle= TRUE;

	// create the event, it is manual reset, initial state is nonsignaled
	if ((event= CreateEvent(sa,TRUE,FALSE,eventName.c_str())) == NULL)
	{
		int errCode = 0;
		string header = "CreateEvent() in myEvent::myEvent() failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
        throw excp;
	}

	// if a callback is provided, create a thread to monitor
	// the event. When the event occurs, queue the callback 
	// as an APC (if the user wants to do so)
    if ((pfnAPC=_pfnAPC) != NULL)
    {
		queueUserAPC = _queueUserAPC;
		//if ((thread = CreateThread(sa,0,ThreadProc,this,0, &threadID)) == NULL)
		if ((thread = (HANDLE)_beginthreadex(sa,0,ThreadProc,this,0, &threadID)) == NULL)
		{
			int errCode = 0;
			string header = "CreateThread() in myEvent::myEvent() failed";
			getErrorDetail(errCode,header);
			myException excp(errCode,header);
			throw excp;
		}
	}
	else 
		thread = NULL;
}

myEvent::~myEvent()
{
	//cout << "I am in myEvent destructor..." << endl;

    HeapFree(GetProcessHeap(),0,sa); 

    if (thread) 
		CloseHandle(thread);

    CloseHandle(event);
}


BOOL myEvent::waitForEvent(DWORD waitTime)
{
    DWORD retVal;
	
	if ((retVal = WaitForSingleObjectEx(event,waitTime,TRUE)) == 0xFFFFFFFF)
	{
		int errCode = 0;
		string header = "WaitForSingleObjectEx() in myEvent::waitForEvent() failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
		throw excp;
	}
	if (retVal == WAIT_TIMEOUT) 
		return FALSE;
    else 
		return TRUE;
}

void myEvent::pulseEvent()
{
	if (!PulseEvent(event))
	{
		int errCode = 0;
		string header = "PulseEvent() in myEvent::pulseEvent() failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
		throw excp;
	}
}

void myEvent::setEvent()
{
	if (!SetEvent(event))
	{
		int errCode = 0;
		string header = "SetEvent() in myEvent::setEvent() failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
		throw excp;
	}
}

void myEvent::resetEvent()
{
	if (!ResetEvent(event))
	{
		int errCode = 0;
		string header = "ResetEvent() in myEvent::resetEvent() failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
		throw excp;
	}
}

unsigned WINAPI myEvent::ThreadProc(void* lpData)
{ 
    // winLog << "[THREAD] the thread is started by OS (ThreadProc) ... " << endl;

    myEvent* pEvent = (myEvent*)lpData;
	DWORD waitRetVal = 0;

    while (1)
    {
		// winLog << "[THREAD] calling WaitForSingleObjectEx() ... " << endl;

		// wait on this event,no time_out and APC can finish the waiting
		waitRetVal = WaitForSingleObjectEx(pEvent->getEventHandle(),INFINITE,TRUE);
		// winLog << "[THREAD] WaitForSingleOjectEx() returned ... return value = " << waitRetVal << endl;

		if ( waitRetVal == 0xFFFFFFFF )
		{
			int errCode = 0;
			string header = "WaitForSingleObjectEx() in myEvent::ThreadProc() failed";
			getErrorDetail(errCode,header);
			myException excp(errCode,header);
			throw excp;
		}
		else if ( waitRetVal == WAIT_OBJECT_0 )
		{
			//cout << "the state of the specified event object is signaled." << endl;
			// winLog << "[THREAD] the state of the specified event object is signaled." << endl;
		}
		else if ( waitRetVal == WAIT_ABANDONED )
		{
			/*
			cout << "The specified object is a mutex object that was not released by the thread" << endl;
			cout << "that owned the mutex object before the owning thread terminated." << endl;
			cout << "Ownership of the mutex object is granted to the calling thread, " << endl;
			cout << "and the mutex is set to nonsignaled." << endl;
			*/
		}
		else if (waitRetVal == WAIT_IO_COMPLETION)
		{
			//cout   << "One or more I/O completion routines are queued for execution." << endl;
			// winLog << "[THREAD] One or more I/O completion routines are queued for execution." << endl;
		}
		else if ( waitRetVal == WAIT_TIMEOUT )
			//cout << "The time-out interval elapsed, and the object's state is nonsignaled." << endl; 

		if ( pEvent->getQueueUserAPC() == TRUE )
		{
			// cout << "schedule the callback..." << endl;
			// winLog << "[THREAD] calling QueueUserAPC() to schedule the callback ... " << endl;

			if (!QueueUserAPC(pEvent->getAPCFunc(), GetCurrentThread(), (ULONG_PTR)lpData))
			{
				int errCode = 0;
				string header = "QueueUserAPC() in myEvent::ThreadProc() failed";
				getErrorDetail(errCode,header);
				myException excp(errCode,header);
				throw excp;
			}
			// winLog << "[THREAD] finished schedule the callback... " << endl;
			SleepEx(INFINITE,TRUE);  // INFINITE to make sure the callback is called!!
		}
		break;
    }
	
	// winLog << "[THREAD] this thread is done... " << endl; 
    return 0;
}

void myEvent::getErrorDetail(int& errCode,string& header)
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