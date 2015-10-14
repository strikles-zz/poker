
#include <cassert>
#include <string>
#include <vector>

#include <ctime>

#include "CANN/CANN.h"
#include "Master/CGlobalVars.h"
#include "Config/CPreferences.h"
#include "DB/CDatabase.h"
#include "Net/CEvent/CEvent.h"
#include "Net/CException/CException.h"
#include "Net/CHostInfo/CHostInfo.h"
#include "Net/CLog/CLog.h"
#include "Net/CSemaphore/CSemaphore.h"
#include "Net/CSock/CSock.h"
#include "Net/CThread/CThread.h"
#include "Net/CThreadArgument/CThreadArgument.h"
#include "Regression/CRegressionObject.h"

#include "StructDefines/structdefines.h"
#include "Util/Util.h"
#include "Util/Util_logging.h"

#include "Master/CDominvs.h"

CPreferences gPrefs;
CLogging gLog;

// parameter is nplayers dealt
CGlobalVars gVars(2);

unsigned WINAPI clientHandleThread(void* threadInfo)
{
	// this structure will contain all the data this callback will work on
    myThreadArgument* client_argument = (myThreadArgument*)threadInfo;

	// get the client connection: receiving messages from client and
	// sending messages to the client will all be done by using
	// this client connection
	myTcpSocket* client_connection = client_argument->getClientConnect();

	// the server is communicating with this client here
	// the semamphore to protect the access to the std output
    mySemaphore* log_semaphore = client_argument->get_LogSemaphore();
	// the semaphore to protect access to the db
	mySemaphore* general_semaphore = client_argument->get_GeneralSemaphore();

	CDatabase db;
	CDatabaseInterface db_interface;
	CDominvs decision_obj;

	while(1)
	{
		int msg_from_client[eDim_Total];
		int msg_to_client[eDim_Total];
		
		// receive from the client
		eMessageType recv_msgtype, send_msgtype;
		recv_msgtype = send_msgtype = eMsgUnknown;
		int recv_msglen = 0;

		int num_recv = client_connection->RecvInts(msg_from_client, eDim_Total);
		// Exit on error
		if (num_recv <= 0)
		{
			gLog.WriteLog(eSeverityDebug, eCatNet, "Warning: Connection closed ?\n", (int) recv_msgtype);
			break;
		}

		recv_msgtype = (eMessageType) msg_from_client[eDim_RequestType];
		// check for errors
		if (recv_msgtype <= eMsgUnknown || recv_msgtype >= eTotalMsgs)
		{
			gLog.WriteLog(eSeverityError, eCatNet, "ERROR: UNK MSG (%d)\n", (int) recv_msgtype);
			continue;
		}
		// if the client wants to discount
		else if (recv_msgtype == eMsgQuit)
		{
			break;
		}

		///////////////////////////
		// KNN Decision Messages //
		///////////////////////////

		// The data was OK Calculate decision
		if(recv_msgtype == eClientKnnRequest)
		{
			memcpy(msg_to_client, msg_from_client, sizeof msg_to_client);
			//printHandStruct(msg_to_client);
			eBetType decision = eBet_Unknown;

			//#define RANDOM_ACTION
			#if defined RANDOM_ACTION
				decision = decision_obj.RandomAction(msg_to_client, false, 0);
			#else
				decision = decision_obj.ANNDecision(msg_to_client, &db);
			#endif

			if (decision > eBet_Unknown && decision < eTotalActions)
				gLog.WriteLog(eSeverityInfo, eCatANN, "ANN Final Decision : %d [%s]\n", decision, bets_str[decision]);
			else
				gLog.WriteLog(eSeverityError, eCatANN, "Error: ANN Final Decision is invalid : %d\n", decision);

			// Fill the buffer
			msg_to_client[eDim_RequestType] = (int) eServerKnnReply;
			msg_to_client[eDim_Decision] = (int) decision;
			gLog.WriteLog(eSeverityDebug, eCatNet, "Sending Decision to client : %d\n", msg_to_client[eDim_Decision]);
		}

		//////////////////////
		// Context Messages //
		//////////////////////

		// The data was OK Insert Profit in DB
		else if(recv_msgtype == eClientSaveHandRequest)
		{
			memcpy(msg_to_client, msg_from_client, sizeof msg_to_client);
			msg_to_client[eDim_RequestType] = eServerInsertedHandID;
			msg_to_client[eDim_handId] = db_interface.SaveHand(msg_to_client, &db);
		}

		/////////////////////
		// Profit Messages //
		/////////////////////

		// Hooper performed a rebuy
		else if(recv_msgtype == eHopperRebuyEvent)
		{
			gLog.WriteLog(eSeverityDebug, eCatNet, "Got A Rebuy Message From a Hopper at : %s\n", msg_from_client);
			// The message will just be a casino window handle hex str for now. Should be a better unique identifier.
			general_semaphore->lock();
			pair<set<int>::iterator, bool> it = gVars.hopper_rebuy_buffer.insert(msg_from_client[0]);
			gLog.WriteLog(eSeverityDebug, eCatNet, "Inserting Handle from Hopper Message into set : %s\n", *it.first);
			general_semaphore->unlock();

			break;
		}

		// The data was OK Insert Profit in DB
		else if(recv_msgtype == eClientSaveProfit)
		{
			memcpy(msg_to_client, msg_from_client, sizeof msg_to_client);

			// Check if a rebuy event has been sent by the hopper (find handle marker)
			int handle_tmp = msg_to_client[eProfit_hwnd];
			std::set<int>::iterator it = gVars.hopper_rebuy_buffer.find(handle_tmp);
			if(it != gVars.hopper_rebuy_buffer.end())
			{
				// Subtract profit value
				msg_to_client[eProfit_Amt] -= (gPrefs.general.get_rebuy_amt()*100.0); // should really get the sb value here in a non-stupid way
				gLog.WriteLog(eSeverityDebug, eCatNet, "*** REBUY + PROFIT *** : %d\n", handle_tmp);

				// Remove the marker
				general_semaphore->lock();
				it = gVars.hopper_rebuy_buffer.erase(it);
				general_semaphore->unlock();	
			}

			// Insert the profit in the db and fill the buffer with the hand_id message
			int inserted_hand_id = db_interface.SaveProfit(msg_to_client, &db);

			msg_to_client[eProfit_RequestType] = eServerInsertedProfitHandID;
			msg_to_client[eProfit_handId] = inserted_hand_id;
		}

		if(msg_to_client[eDim_RequestType] != eMsgUnknown)
			client_connection->SendInts(msg_to_client, eDim_Total);
    }

	// if we reach here, this session with the client is done,
	// so we set the event on this thread to inform the main
	// control that this session is finished
	client_argument->getExitEvent()->setEvent();

	return 1;
}

unsigned WINAPI serverHandleThread(void* threadInfo)
{
	// this structure will contain all the data this callback will work on
    myThreadArgument* server_argument = (myThreadArgument*)threadInfo;		
	// get the server
	myTcpSocket* server_sock = server_argument->getClientConnect();
	//string server_name = server_argument->getHostName();

	// bind the server to the socket
    server_sock->bindSocket();
	gLog.WriteLog(eSeverityInfo, eCatNet, "server finishes binding process... \n");

	// server starts to wait for client calls
	server_sock->listenToClient();
	gLog.WriteLog(eSeverityInfo, eCatNet, "server is waiting for client calls ... \n");

	mySemaphore* log_semaphore = server_argument->get_LogSemaphore();
	mySemaphore* general_semaphore = server_argument->get_GeneralSemaphore();
	
	// server starts to listen, and generates a thread to handle each client
	myThreadArgument* client_argument[MAX_NUM_CLIENTS];
	myThread* client_handle[MAX_NUM_CLIENTS];
	bool client_slot_used[MAX_NUM_CLIENTS];
	for (int i = 0; i < MAX_NUM_CLIENTS; i++)
	{
		client_argument[i] = NULL;
		client_handle[i] = NULL;
		client_slot_used[i] = false;
	}

	// Server client accept loop
	while (true)
	{
		// connection dedicated for client communication
    	myTcpSocket* client_sock;
		// client name 
		std::string client_id_str = "";

		// wait to accept a client connection. processing is suspended until the client connects
		client_sock = server_sock->acceptClient(client_id_str);
		//gLog.WriteLog(eSeverityDebug, eCatDB, "Client Name: %s\n", client_id_str.c_str());

		if (!client_sock)
			continue;

		for(int i = 0; i < MAX_NUM_CLIENTS; i++)
		{
			// Clean Up
			myThreadArgument* client_info = server_argument->getClientArgument(i);
			if (client_info && client_info->getExitEvent()->waitForEvent(0))
			{
				delete client_handle[i];
				delete client_argument[i];
				server_argument->deleteClientArgument(i);
				client_slot_used[i] = false;

				gLog.WriteLog(eSeverityInfo, eCatNet, "<< Deleted a client\n", client_id_str.c_str());
			}

			// Create a thread for the client
			if (!client_slot_used[i])
			{
				client_id_str += (":" + ToString(i));
				gLog.WriteLog(eSeverityDebug, eCatDB, ">> Added a client: %s\n", client_id_str.c_str());
				client_sock->set_clientid(client_id_str);
				client_argument[i] = new myThreadArgument(client_sock, server_argument->get_semaphores(), client_id_str);
				client_handle[i] = new myThread(&clientHandleThread, (void*)client_argument[i]);
				server_argument->addClientArgument(client_argument[i], i);
				client_handle[i]->execute();
				client_slot_used[i] = true;

				gLog.WriteLog(eSeverityInfo, eCatNet, "==> A client from [ %s ] is connected!\n", client_id_str.c_str());
				break;
			}
		}
	}

    return 1;
}

int main()
{
	// build a semaphore so we can synchronize the access to std cout
	// also includes the log file
	mySemaphore log_emaphore(string("Log"), 1);
	mySemaphore general_semaphore(string("General"), 1);
	gLog.SetSemaphore(&log_emaphore);

	// Initialize the winsock library
	myTcpSocket::initialize();
    myHostInfo server_info;
	string serverName = string(server_info.getHostName());
    string serverIPAddress = string(server_info.getHostIPAddress());

	gLog.WriteLog(eSeverityInfo, eCatNet, "my localhost (server) information: \n");
	gLog.WriteLog(eSeverityInfo, eCatNet, "	Name:    %s\n", serverName.c_str());
    gLog.WriteLog(eSeverityInfo, eCatNet, "	Address: %s\n", serverIPAddress.c_str());

	// open socket on the local host(server) and show its configuration
	myTcpSocket server_sock(gPrefs.net.get_port());

	// create a thread to implement server process: listening to the socket,
	// accepting client calls and communicating with clients. This will free the 
	// main control (see below) to do other process.
	vector<mySemaphore*> semaphores;
	semaphores.push_back(&log_emaphore);
	semaphores.push_back(&general_semaphore);

	myThreadArgument* serverArgument = new myThreadArgument(&server_sock, semaphores, serverName);
	myThread* serverThread = new myThread(serverHandleThread, (void*)serverArgument);
	serverThread->execute();

	// main control: since the serverThread is handling the server functions,
	// this main control is free to do other things.
	while (true)
	{
		// do whatever you need to do here, I am using Sleep(x) 
		// to make a little delay, pretending to be the other 
		// possible processings.
		Sleep(50000);	

		gLog.WriteLog(eSeverityInfo, eCatNet, "-----------------------------------------------------------------\n");
		gLog.WriteLog(eSeverityInfo, eCatNet, "server (name: %s) status report:\n", serverName.c_str());
		gLog.WriteLog(eSeverityInfo, eCatNet, "   the following clients have successfully connected with server: \n");

		for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ )
		{
			myThreadArgument* clientInfo = serverArgument->getClientArgument(i);
			if ( clientInfo ) 
				gLog.WriteLog(eSeverityInfo, eCatNet, "         %s\n", clientInfo->getHostName().c_str());
		}

		gLog.WriteLog(eSeverityInfo, eCatNet, "   the following clients have shutdown the connection: \n");
		for (int i = 0; i < MAX_NUM_CLIENTS; i ++ )
		{
			myThreadArgument* clientInfo = serverArgument->getClientArgument(i);
			if ( clientInfo && clientInfo->getExitEvent()->waitForEvent(0) )
			{
				clientInfo->setSignalToEnd(true);
				gLog.WriteLog(eSeverityInfo, eCatNet, "         %s\n", clientInfo->getHostName().c_str());
			}
		}
        gLog.WriteLog(eSeverityInfo, eCatNet, "-----------------------------------------------------------------\n");
	}

    return 1;
}

