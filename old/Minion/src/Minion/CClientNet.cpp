
#include <sstream>
#include <string>

#include "CClientNet.h"

#include "Config/CPreferences.h"
#include "Net/CException/CException.h"
#include "Net/CLog/CLog.h"

#include "StructDefines/structdefines.h"

#include "Util/Util.h"
#include "Util/Util_logging.h"
#include "Util/Util_MagicNumbers.h"



CClientNet::CClientNet()
{
	// Initialize the winsock library
	myTcpSocket::initialize();

	Init();
}

CClientNet::~CClientNet()
{
	delete net;
}

int CClientNet::Init()
{

	// get client's information (assume neither the name nor the address is given)
	gLog.WriteLog(eSeverityInfo, eCatNet, "Retrieve the localHost [CLIENT] name and address:\n");
    
	client_name = clientInfo.getHostName();
    client_ip_str = clientInfo.getHostIPAddress();
    gLog.WriteLog(eSeverityInfo, eCatNet, "Name: %s\n", client_name.c_str());
    gLog.WriteLog(eSeverityInfo, eCatNet, "Address: %s\n", client_ip_str.c_str());

	// get server's IP address and name
	server_ip_str = gPrefs.net.get_ip();
	gLog.WriteLog(eSeverityInfo, eCatNet, "Retrieve the remoteHost [SERVER] name and address:\n");
	gLog.WriteLog(eSeverityInfo, eCatNet, "		==> the given address is %s\n", server_ip_str.c_str());
	gLog.WriteLog(eSeverityInfo, eCatNet, "SRV Address: %s\n", server_ip_str.c_str());

    // create the socket for client
    net = new myTcpSocket(gPrefs.net.get_port());

	// connect to the server.
	hostType hType = ADDRESS;
	if (inet_addr(server_ip_str.c_str()) ==  INADDR_NONE)
		hType = NAME;

	net->connectToServer(server_ip_str, hType);

	return 1;

}

eBetType CClientNet::ExecuteKnnMessageExchange(int* hand)
{
	eBetType bet_type = eBet_Unknown;
	bool exchange_succeeded = false;

	for (int i = 0; i < 1; i++)
	{
		// send 1
		hand[eDim_RequestType] = eClientKnnRequest;
		net->SendInts(hand, eDim_Total);

		// recv 2
		eMessageType recv_msgtype = eMsgUnknown;
		int recv_buffer[eDim_Total];
		memset(recv_buffer, 0, sizeof recv_buffer);
		net->RecvInts(recv_buffer, eDim_Total);
		recv_msgtype = (eMessageType) recv_buffer[eDim_RequestType];
		if(recv_msgtype != eServerKnnReply)
		{
			gLog.WriteLog(eSeverityError, eCatNet, "ERROR: Expecting eServerKnnReply [Got %s]\n", messages_str[recv_msgtype]);
			continue;
		}

		exchange_succeeded = true;
		bet_type = (eBetType) recv_buffer[eDim_Decision];		
		break;
	}
	
    return bet_type;
}

int CClientNet::ExecuteHandUpdateMessageExchange(int* hand)
{
	int inserted_hand_id = -1;
	bool exchange_succeeded = false;
	eMessageType recv_msgtype = eMsgUnknown;

	int recv_buffer[eDim_Total];
	int send_buffer[eDim_Total];

	for (int i = 0; i < 1; i++)
	{
		// SEND 1 : profit request
		memset(send_buffer, 0, sizeof send_buffer);
		memset(recv_buffer, 0, sizeof recv_buffer);
		int recv_msglen = 0;
		// Serialize_me
		memcpy(send_buffer, hand, sizeof(int) * eDim_Total);
		gLog.WriteLog(eSeverityInfo, eCatNet, "Save Hand Request\n");
		send_buffer[eDim_RequestType] = eClientSaveHandRequest;
		net->SendInts(send_buffer, eDim_Total);
		
		// RECV 2
		gLog.WriteLog(eSeverityInfo, eCatNet, "Getting Inserted hand_id\n");
		int num_recv = net->RecvInts(recv_buffer, eDim_Total);
		if (num_recv == -1)
			continue;
		recv_msgtype = (eMessageType) recv_buffer[eDim_RequestType];
		// messages are different or of unexpected type
		if(eServerInsertedHandID != recv_msgtype)
		{
			gLog.WriteLog(eSeverityError, eCatNet, "ERROR: Expecting eServerInsertedID [Got %s]\n", messages_str[recv_msgtype]);
			continue;
		}

		inserted_hand_id = recv_buffer[eDim_handId];
		gLog.WriteLog(eSeverityInfo, eCatNet, "Inserted hand %d\n", inserted_hand_id);
		exchange_succeeded = true;
		break;
	}

	return inserted_hand_id;
}

int CClientNet::ExecuteProfitMessageExchange(int* profit)
{
	int inserted_hand_id = -1;
	bool exchange_succeeded = false;
	eMessageType recv_msgtype;

	int recv_buffer[eDim_Total];
	int send_buffer[eDim_Total];

	for (int i = 0; i < 1; i++)
	{
		// send 1 : profit request
		memset(send_buffer, 0, sizeof send_buffer);
		memset(recv_buffer, 0, sizeof recv_buffer);
		// Serialize_me
		memcpy(send_buffer, profit, sizeof(int) * eProfit_Total);
		gLog.WriteLog(eSeverityInfo, eCatNet, "Sending Profit Request\n");
		send_buffer[eDim_RequestType] = eClientSaveProfit;
		// SEND 1
		net->SendInts(send_buffer, eDim_Total);
	
		// RECV 2
		memset(recv_buffer, 0, sizeof recv_buffer);
		gLog.WriteLog(eSeverityInfo, eCatNet, "Getting Inserted hand_id\n");
		int num_recv = net->RecvInts(recv_buffer, eDim_Total);
		if (num_recv == -1)
			continue;
		recv_msgtype = (eMessageType) recv_buffer[eProfit_RequestType];
		// messages are different or of unexpected type
		if(eServerInsertedProfitHandID != recv_msgtype)
		{
			gLog.WriteLog(eSeverityError, eCatNet, "ERROR: Expecting eServerInsertedID [Got %s]\n", messages_str[recv_msgtype]);
			continue;
		}

		inserted_hand_id = recv_buffer[eProfit_handId];
		gLog.WriteLog(eSeverityInfo, eCatNet, "Inserted hand %d\n", recv_buffer[eProfit_handId]);
		exchange_succeeded = true;
		break;
	}

	return inserted_hand_id;
}