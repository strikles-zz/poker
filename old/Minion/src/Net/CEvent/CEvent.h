#if !defined myEvent_H 
#define myEvent_H

#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <process.h>
#include <string>
using namespace std;


class myEvent  
{

public:
	
	myEvent(string eventName="",PAPCFUNC _pfnAPC=NULL,BOOL queueUserAPC=FALSE);
    virtual ~myEvent();

	// return the event handle
    HANDLE getEventHandle() { return event; }

	// return the thread handle
	HANDLE getThread() { return thread; }

	// return the address of the callback function 
    PAPCFUNC getAPCFunc() { return pfnAPC; }

	// return whether the user wants to queue the callback 
	// when event happens
	BOOL getQueueUserAPC() { return queueUserAPC; }

    void setEvent();      // wrapper to SetEvent()  
	void pulseEvent();    // wrapper to PulseEvent()
	void resetEvent();    // wrapper to ResetEvent()

	// wait for the event
    BOOL waitForEvent(DWORD waitTime=INFINITE);
    
private:

	// called by the OS when the thread is instantiated, this
	// must have the WINAPI function declaration
    static unsigned WINAPI ThreadProc(void* lpData);

private:

    HANDLE event;                  // event handle
    PAPCFUNC pfnAPC;               // address of user-provided callback
	LPSECURITY_ATTRIBUTES sa;      // SECURITY_ATTRBUTES structure
    unsigned int threadID;         // thread id
    HANDLE thread;                 // handle to thread

	// if the following is set to be TRUE, when the events occurs, 
	// will call QueueUserAPC() to queue the user-provided callback
	// as a user defined API. if the following is set to be FALSE;
	// then no user-provided callback will be queued. 
	// if pfnAPC is NULL (no user-provided callback is defined),
	// then the following is ignored 
	BOOL queueUserAPC;        
	
private:

	static void getErrorDetail(int&,string&);

};

#endif
