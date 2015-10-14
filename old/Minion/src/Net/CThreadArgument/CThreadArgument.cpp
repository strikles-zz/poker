#include "Net/CThreadArgument/CThreadArgument.h"

#include "Net/CEvent/CEvent.h"


myThreadArgument::myThreadArgument(myTcpSocket* client_sock, vector<mySemaphore*>& semaphs, string& client_id)
{
	semaphores = semaphs;
	client_connection = client_sock;
	client_hostname = client_id;
	exit_event = new myEvent();
	signal_end = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) 
		client_argument[i] = NULL;

    num_connected_clients = 0;
}

myThreadArgument::~myThreadArgument() 
{
	delete exit_event;
}

void myThreadArgument::addClientArgument(myThreadArgument* argument, int index)
{
	if (argument)
		client_argument[index] = argument;
}

void myThreadArgument::deleteClientArgument(int index)
{
	client_argument[index] = NULL;
}

myThreadArgument* myThreadArgument::getClientArgument(int index)
{
	if (index < MAX_NUM_CLIENTS) 
		return client_argument[index];

	return NULL;
}


