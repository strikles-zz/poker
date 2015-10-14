#pragma once

#include <string>

#include "Net/CHostInfo/CHostInfo.h"
#include "Net/CSock/CSock.h"

#include "StructDefines/structdefines.h"


class CClientNet
{

public:

	CClientNet();
	~CClientNet();

	int Init();
	eBetType ExecuteKnnMessageExchange(int* message);
	int ExecuteProfitMessageExchange(int* profit);
	int ExecuteHandUpdateMessageExchange(int* hand);

private:

	std::string client_name;
	std::string server_name;
	std::string client_ip_str;
	std::string server_ip_str;


	myHostInfo clientInfo;
	myHostInfo serverInfo;
	myTcpSocket* net;
};

extern CClientNet gNet;