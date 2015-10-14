#if !defined myThread_H
#define myThread_H

#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <process.h>
#include <string>
using namespace std;

class myThread  
{

public:

    myThread(
		unsigned (__stdcall *pThreadFunc) (void *),	// address of thread callback
		void* pThreadFuncParameter=NULL,			// address of callback's parameter,
		DWORD  exeFlags=0,							// creation flag
		DWORD  sSize=0,								// stack size
		BOOL   inheritable=FALSE					// inheritable
	);        

    ~myThread();

public:

	void execute();
	void waitForThreadToFinish();    
	void suspend();
    void resume();

	void setThreadPriority(int);
    int  getThreadPriority();

    void disablePriorityBoost(BOOL);
	BOOL isPriorityBoostDisabled();

    void setProcessorMask(DWORD);
	void setIdealProcessor(DWORD);

	LPVOID getThreadArgument() { return threadArgument; }
    DWORD getExitCode();    
	DWORD getThreadId();
	void  getThreadStatistics(LPFILETIME,LPFILETIME,LPFILETIME,LPFILETIME);

private:
	
    HANDLE thread;
    unsigned int  threadId;
	DWORD  stackSize;
	DWORD  executeFlag;
	unsigned (__stdcall *threadFunction) (void *);
	void* threadArgument;
    LPSECURITY_ATTRIBUTES sa;
    DWORD exitCode;

private:

    static void getErrorDetail(int&,string&);
};
#endif