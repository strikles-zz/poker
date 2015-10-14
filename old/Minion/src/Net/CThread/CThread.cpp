#include "Net/CThread/CThread.h"

#include "Net/CException/CException.h"


myThread::myThread(unsigned (__stdcall *pThreadFunc) (void *),
				   void* pThreadFuncParameter,
				   DWORD exeFlags,
				   DWORD sSize,
				   BOOL inheritable)
{
	sa = (LPSECURITY_ATTRIBUTES)HeapAlloc(GetProcessHeap(), 0, sizeof(SECURITY_ATTRIBUTES));
	if(!sa)
	{
		int errCode = 0;
		string header = "sa setup myEvent::myEvent() failed";
		getErrorDetail(errCode,header);
		myException excp(errCode,header);
        throw excp;
	}

	sa->nLength = sizeof(SECURITY_ATTRIBUTES);
    sa->lpSecurityDescriptor = NULL;
	sa->bInheritHandle = inheritable;

	threadFunction = pThreadFunc;
	threadArgument = pThreadFuncParameter;

	executeFlag = exeFlags;
	stackSize = sSize;
	exitCode = 0xFFFFFFFF;

}

void myThread::execute()
{
	
	//if ((thread = CreateThread(sa, stackSize, threadFunction, threadArgument, executeFlag, &threadId)) == NULL)
	//( NULL, 0, &SecondThreadFunc, NULL, 0, &threadID );
	if ((thread = (HANDLE)_beginthreadex(sa, stackSize, threadFunction, threadArgument, executeFlag, &threadId)) == NULL)
    {
		int errCode = 0;
		string header = "CreateThread() failed";
		getErrorDetail(errCode, header);
        myException excp(errCode, header);
        throw excp;
        return;
    }
}

myThread::~myThread()
{
	if (sa) 
	{
		HeapFree(GetProcessHeap(), 0, sa); 
		sa = NULL;
	}
}

void myThread::waitForThreadToFinish()
{
	if (WaitForSingleObject(thread,INFINITE) == WAIT_FAILED)
	{
		int errCode = 0;
		string header = "WaitForSingleObject() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
		throw excp;
		return;
    }
}

DWORD myThread::getExitCode()
{
	if (GetExitCodeThread(thread, &exitCode))
    {
		return exitCode;
    }
    else
	{
		int errCode = 0;
		string header = "GetExitCodeThread() failed";
		getErrorDetail(errCode, header);
        myException excp(errCode, header);
        throw excp;
    }
}

DWORD myThread::getThreadId()
{
	return threadId;
}

void myThread::resume()
{
	if (ResumeThread(thread) == 0xFFFFFFFF)
	{
		int errCode = 0;
		string header = "ResumeThread() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
        throw excp;
        return;
    }
}

void myThread::suspend()
{
	if (SuspendThread(thread) == 0xFFFFFFFF)
	{
		int errCode = 0;
		string header = "SuspendThread() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
		throw excp;
        return;
    }
}

int myThread::getThreadPriority()
{
    int retVal;
	if ((retVal = GetThreadPriority(thread)) == THREAD_PRIORITY_ERROR_RETURN)
	{
		int errCode;
		string header = "GetThreadPriority() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
        throw excp;
        return 0;
    }
    return retVal;
}

void myThread::setThreadPriority(int thePriority)
{
	if (!SetThreadPriority(thread,thePriority))
    {
		int errCode = 0;
		string header = "SetThreadPriority() failed";
		getErrorDetail(errCode, header);
        myException excp(errCode, header);
        throw excp;
    }
}

void myThread::disablePriorityBoost(BOOL disable)
{
    if (!SetThreadPriorityBoost(thread,disable))
	{
		int errCode = 0;
		string header = "SetThreadPriorityBoost() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
        throw excp;
    }
}

BOOL myThread::isPriorityBoostDisabled()
{
	BOOL boosted;
	if (!GetThreadPriorityBoost(thread,&boosted))
	{
		int errCode = 0;
		string header = "GetThreadPriorityBoost() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
		throw excp;
    }

	return boosted;
}

void myThread::setProcessorMask(DWORD mask)
{
	if (!SetThreadAffinityMask(thread,mask))
	{
		int errCode = 0;
		string header = "SetThreadAffinityMask() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
		throw excp;
    }
}

void myThread::setIdealProcessor(DWORD proc)
{
	if (SetThreadIdealProcessor(thread, proc) == -1)
	{
		int errCode = 0;
		string header = "SetThreadIdealProcessor failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
		throw excp;
    }
}

void myThread::getThreadStatistics(LPFILETIME  creationTime, LPFILETIME exitTime, LPFILETIME kernelTime, LPFILETIME userTime)
{
	if (!GetThreadTimes(thread, creationTime, exitTime, kernelTime, userTime))
    {
		int errCode = 0;
		string header = "GetThreadTimes() failed";
		getErrorDetail(errCode, header);
		myException excp(errCode, header);
        throw excp;
    }
}

void myThread::getErrorDetail(int& errCode, string& header)
{
	char buf[256];
	LPVOID lpDetailMsgBuf;
	UINT errorCode;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL, errorCode=GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (LPTSTR)&lpDetailMsgBuf, 0, NULL);

	memset(buf, 0, 255);
	wsprintf(buf,": %s", (LPTSTR)lpDetailMsgBuf);
    LocalFree(lpDetailMsgBuf);	
	
	header.append(string(buf));
	errCode = errorCode;
}
