#pragma once

#define OPEN 0
#define CREATE 1


#include <windows.h>
#include <winuser.h>
#include <winbase.h>
#include <string>
using namespace std;

class mySemaphore
{

public:

    mySemaphore(string& sName=string(""),
		        long initialCount=0,long maxCount=1,
				int accessMode=CREATE,BOOL canInherit=TRUE);

    virtual ~mySemaphore();

public:

    void closeSemaphore();    
	BOOL lock(BOOL wait=TRUE);
    void unlock();

public:

	string& getSemaphoreName() { return semaphoreName; }
	long    getInitialCount()  { return initCount; }
    long    getMaxCount()      { return maxCount; }
	BOOL    getInerit()        { return inherit; }
	
private:

	void getErrorDetail(int&,string&);

private:

	long initCount;   // initialCount 
	long maxCount;    // max count
    BOOL inherit;     // can be inherited by child

    string semaphoreName;
    HANDLE semaphore;
    LPSECURITY_ATTRIBUTES sa;
};
