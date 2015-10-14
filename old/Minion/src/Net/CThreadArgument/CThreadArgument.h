#if !defined myThreadArgument_H
#define myThreadArgument_H

#include <vector>
#include <string>

#include "DB/CDatabase.h"
#include "Net/CEvent/CEvent.h"
#include "Net/CSock/CSock.h"
#include "Net/CSemaphore/CSemaphore.h"
using namespace std;



const int MAX_NUM_CLIENTS = 100;

class myThreadArgument
{

private:

	// you have to change here to fit your needs

	mySemaphore* log_semaphore;       // semaphore for accessing the log file
	mySemaphore* rebuy_semaphore;     // semaphore for DB ... Do I really need it ?

	myTcpSocket* client_connection;    // the connecting socket to the client
	string client_hostname;            // the name of the client
	myThreadArgument* client_argument[MAX_NUM_CLIENTS];
    int num_connected_clients;

	// but the exitEvent is always needed
	myEvent* exit_event;
	bool signal_end;


	vector<mySemaphore*> semaphores;

public:

	// you have to change here to fit your needs
	myThreadArgument(myTcpSocket* client_sock, vector<mySemaphore*>& Semaphores, string& cName);
	~myThreadArgument() ;

	// use the accessor functions that fit your needs

	mySemaphore*	get_LogSemaphore()		{ return semaphores[0]; }
	mySemaphore*	get_GeneralSemaphore()	{ return semaphores[1]; }

	myTcpSocket*	getClientConnect()		{ return client_connection; }
	string&			getHostName()			{ return client_hostname; }
	bool			getSignalToEnd()		{ return signal_end; }
	void			setSignalToEnd(bool s)	{ signal_end = s; }

	void addClientArgument(myThreadArgument*, int);
	void deleteClientArgument(int index);
	myThreadArgument* getClientArgument(int);
	vector<mySemaphore*>& get_semaphores() { return semaphores; }  

	// do not change here
    myEvent* getExitEvent() { return exit_event;}
};

#endif