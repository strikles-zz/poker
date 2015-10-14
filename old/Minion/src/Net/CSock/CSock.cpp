#include "Config/CPreferences.h"

#include "Net/CSock/CSock.h"
#include "Net/CException/CException.h"

#include "Util/Util_logging.h"


#define DEBUG_ACK

mySocket::mySocket(int pNumber) :
	m_iPort(pNumber),
	m_bReverse(true),
    blocking(1)
{
    try
    {
		m_iSock = socket(AF_INET, SOCK_STREAM, 0);
		if (m_iSock == INVALID_SOCKET)
        {
			#ifdef WIN32
				int errorCode;
				string errorMsg = "";
				detectErrorOpenWinSocket(&errorCode,errorMsg);
				myException openWinSocketException(errorCode,errorMsg);
				throw openWinSocketException;
			#else
				myException openUnixSocketException(0,"unix: error getting host by name");
				throw openUnixSocketException;
			#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}


    /* 
	   set the initial address of client that shall be communicated with to 
	   any address as long as they are using the same port number. 
	   The clientAddr structure is used in the future for storing the actual
	   address of client applications with which communication is going
	   to start
	*/
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(m_iPort);

	/*
		setDebug(1);
		setKeepAlive(1);
	*/
}

mySocket::~mySocket()
{
	#ifdef WIN32
        closesocket(m_iSock);
    #else
        close(m_iSock);
    #endif
}
    
void mySocket::setDebug(int debugToggle)
{
	try 
	{
		if (setsockopt(m_iSock,SOL_SOCKET,SO_DEBUG,(char *)&debugToggle,sizeof(debugToggle)) == SOCKET_ERROR)
		{
			#ifdef WIN32
				int errorCode;
				string errorMsg = "DEBUG option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				myException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#else
				myException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
}

void mySocket::setReuseAddr(int reuseToggle)
{
	try 
	{
		if (setsockopt(m_iSock,SOL_SOCKET,SO_REUSEADDR,(char *)&reuseToggle,sizeof(reuseToggle)) == SOCKET_ERROR)
		{
			#ifdef WIN32
				int errorCode;
				string errorMsg = "REUSEADDR option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				myException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#else
				myException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
} 

void mySocket::setKeepAlive(int aliveToggle)
{
	try 
	{
		if (setsockopt(m_iSock, SOL_SOCKET,SO_KEEPALIVE, (char *)&aliveToggle, sizeof(aliveToggle)) == SOCKET_ERROR)
		{
			#ifdef WIN32
				int errorCode;
				string errorMsg = "ALIVE option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				myException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#else
				myException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
} 

void mySocket::setLingerSeconds(int seconds)
{
	struct linger lingerOption;
	
	if (seconds > 0)
	{
		lingerOption.l_linger = seconds;
		lingerOption.l_onoff = 1;
	}
	else lingerOption.l_onoff = 0;
	 
	try 
	{
		if (setsockopt(m_iSock,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,sizeof(struct linger)) == SOCKET_ERROR)
		{
			#ifdef WIN32
				int errorCode;
				string errorMsg = "LINGER option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				myException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#else
				myException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
}

void mySocket::setLingerOnOff(bool lingerOn)
{
	struct linger lingerOption;

	if ( lingerOn ) lingerOption.l_onoff = 1;
	else lingerOption.l_onoff = 0;

	try 
	{
		if (setsockopt(m_iSock,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,sizeof(struct linger)) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "LINGER option:";
			detectErrorSetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
}

void mySocket::setSendBufSize(int sendBufSize)
{
	try 
	{
		if (setsockopt(m_iSock,SOL_SOCKET,SO_SNDBUF,(char *)&sendBufSize,sizeof(sendBufSize)) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "SENDBUFSIZE option:";
			detectErrorSetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
} 

void mySocket::setReceiveBufSize(int receiveBufSize)
{
	try 
	{
		if (setsockopt(m_iSock,SOL_SOCKET,SO_RCVBUF,(char *)&receiveBufSize,sizeof(receiveBufSize)) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "RCVBUF option:";
			detectErrorSetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
}

void mySocket::setSocketBlocking(int blockingToggle)
{
    if (blockingToggle)
    {
        if (getSocketBlocking()) return;
        else blocking = 1;
	}
	else
	{
		if (!getSocketBlocking()) return;
		else blocking = 0;
	}

	try 
	{
	#ifdef WIN32
		if (ioctlsocket(m_iSock,FIONBIO,(unsigned long *)&blocking) == SOCKET_ERROR)
		{
			int errorCode;
			string errorMsg = "Blocking option: ";
			detectErrorSetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		}
	#else
		if (ioctl(m_iSock,FIONBIO,(char *)&blocking) == SOCKET_ERROR)
		{
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		}
	#endif
	}
    catch(myException& excp)
	{
		excp.response();
	}
}

int mySocket::getDebug()
{
    int myOption;
    int myOptionLen = sizeof(myOption);

	try 
	{
		if (getsockopt(m_iSock,SOL_SOCKET,SO_DEBUG,(char *)&myOption,&myOptionLen) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "get DEBUG option: ";
			detectErrorGetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
    
    return myOption;
}

int mySocket::getReuseAddr()
{
    int myOption;        
    int myOptionLen = sizeof(myOption);

	try 
	{
		if (getsockopt(m_iSock,SOL_SOCKET,SO_REUSEADDR,(char *)&myOption,&myOptionLen) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "get REUSEADDR option: ";
			detectErrorGetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
		return -1;
	}

	return myOption;
}

int mySocket::getKeepAlive()
{
    int myOption;        
    int myOptionLen = sizeof(myOption);

	try 
	{
		if (getsockopt(m_iSock,SOL_SOCKET,SO_KEEPALIVE,(char *)&myOption,&myOptionLen) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "get KEEPALIVE option: ";
			detectErrorGetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
		return -1;
	}
    return myOption;    
}

int mySocket::getLingerSeconds()
{
	struct linger lingerOption;
	int myOptionLen = sizeof(struct linger);

	try 
	{
		if (getsockopt(m_iSock,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,&myOptionLen) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "get LINER option: ";
			detectErrorGetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
		return -1;
	}

	return lingerOption.l_linger;
}

bool mySocket::getLingerOnOff()
{
	struct linger lingerOption;
	int myOptionLen = sizeof(struct linger);

	try 
	{
		if (getsockopt(m_iSock,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,&myOptionLen) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "get LINER option: ";
			detectErrorGetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}

	if ( lingerOption.l_onoff == 1 ) 
		return true;
	else 
		return false;
}

int mySocket::getSendBufSize()
{
    int sendBuf;
    int myOptionLen = sizeof(sendBuf);

	try 
	{
		if (getsockopt(m_iSock,SOL_SOCKET,SO_SNDBUF,(char *)&sendBuf,&myOptionLen) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "get SNDBUF option: ";
			detectErrorGetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
		return -1;
	}
    return sendBuf;
}    

int mySocket::getReceiveBufSize()
{
    int rcvBuf;
    int myOptionLen = sizeof(rcvBuf);

	try 
	{
		if (getsockopt(m_iSock,SOL_SOCKET,SO_RCVBUF,(char *)&rcvBuf,&myOptionLen) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode;
			string errorMsg = "get RCVBUF option: ";
			detectErrorGetSocketOption(&errorCode,errorMsg);
			myException socketOptionException(errorCode,errorMsg);
			throw socketOptionException;
		#else
			myException unixSocketOptionException = new myException(0,"unix: error getting host by name");
			throw unixSocketOptionException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
		//WSACleanup();
		return -1;
	}
    return rcvBuf;
}



ostream& operator<<(ostream& io,mySocket& s)
{
	string flagStr = "";

	io << endl;
	io << "Summary of socket settings:" << endl;
	io << "   Socket Id:     " << s.getSocketId() << endl;
	io << "   port #:        " << s.getPortNumber() << endl;
	io << "   debug:         " << (flagStr = s.getDebug()? "true":"false" ) << endl;
	io << "   reuse addr:    " << (flagStr = s.getReuseAddr()? "true":"false" ) << endl;
	io << "   keep alive:    " << (flagStr = s.getKeepAlive()? "true":"false" ) << endl;
	io << "   send buf size: " << s.getSendBufSize() << endl;
	io << "   recv bug size: " << s.getReceiveBufSize() << endl;
	io << "   blocking:      " << (flagStr = s.getSocketBlocking()? "true":"false" ) << endl;
	io << "   linger on:     " << (flagStr = s.getLingerOnOff()? "true":"false" ) << endl;
	io << "   linger seconds: " << s.getLingerSeconds() << endl;
	io << endl;
	return io;
}

#ifdef WIN32
myTcpSocket::myTcpSocket()
{
	//initialize();
}

myTcpSocket::myTcpSocket(int portId) : mySocket(portId) 
{
	//initialize();
}

myTcpSocket::~myTcpSocket()
{
	shutdown(m_iSock, SD_BOTH);
	closesocket(m_iSock);

	gLog.WriteLog(eSeverityDebug, eCatNet, "==> A client has disconnected!\n");
}

void myTcpSocket::initialize()
{
	// initialization work
	WSADATA wsaData;
	try 
	{
		WORD ws_vs = MAKEWORD(2,2);
		if (WSAStartup(ws_vs, &wsaData))
		{
			myException initializationException(0,"Error: calling WSAStartup()");
			throw initializationException;
        }
	}
	catch(myException& excp)
	{
		excp.response();
	}
}
#endif

void myTcpSocket::bindSocket()
{
	/*
	   Binds the socket to an address and port number
	   a server call
	*/

	try
	{
		if (bind(m_iSock,(struct sockaddr *)&client_addr,sizeof(struct sockaddr_in))==SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode = 0;
			string errorMsg = "error calling bind(): \n";
			detectErrorBind(&errorCode,errorMsg);
			myException socketBindException(errorCode,errorMsg);
			throw socketBindException;
		#else
			myException unixSocketBindException(0,"unix: error calling bind()");
			throw unixSocketBindException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
}

void myTcpSocket::connectToServer(string& server_ip, hostType hType)
{ 
	/* 
	   when this method is called, a client socket has been built already,
	   so we have the m_iSock and m_iPort ready.

       a myHostInfo instance is created, no matter how the server's name is 
	   given (such as www.yuchen.net) or the server's address is given (such
	   as 169.56.32.35), we can use this myHostInfo instance to get the 
	   IP address of the server
	*/

	// name lookup
	myHostInfo serverInfo(server_ip, hType);
	
    // Store the IP address and socket port number	
	struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(serverInfo.getHostIPAddress());
    server_addr.sin_port = htons(m_iPort);

	// Connect to the given address
	gLog.WriteLog(eSeverityInfo, eCatNet, "Connecting to %s %s:%d\n", server_ip.c_str(), serverInfo.getHostIPAddress(), m_iPort);
	try 
	{
		if (connect(m_iSock,(struct sockaddr *)&server_addr,sizeof(server_addr)) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode = 0;
			string errorMsg = "error calling connect():\n";
			detectErrorConnect(&errorCode,errorMsg);
			myException socketConnectException(errorCode,errorMsg);
			throw socketConnectException;
		#else
			myException unixSocketConnectException(0,"unix: error calling connect()");
			throw unixSocketConnectException;
		#endif
        }

		gLog.WriteLog(eSeverityInfo, eCatNet, "Connected\n");
	}
    catch(myException& excp)
	{
		excp.response();
	}
}

myTcpSocket* myTcpSocket::acceptClient(string& client_ip)
{
	
	/*
	   accepts a connecting client.  The address of the connected client is stored in the parameter
	   a server call
	*/

	// the new socket file descriptor returned by the accept systme call
	SOCKET newSocket;   

    // the length of the client's address
    int clientAddressLen = sizeof(struct sockaddr_in);
	struct sockaddr_in client_adress;

    // Accepts a new client connection and stores its socket file descriptor
	try 
	{
		newSocket = accept(m_iSock, (struct sockaddr *)&client_adress, &clientAddressLen);
		if (newSocket == INVALID_SOCKET)
		{
		#ifdef WIN32
			int errorCode = 0;
			string errorMsg = "error calling accept(): \n";
			detectErrorAccept(&errorCode,errorMsg);
			myException socketAcceptException(errorCode,errorMsg);
			throw socketAcceptException;
		#else
			myException unixSocketAcceptException(0,"unix: error calling accept()");
			throw unixSocketAcceptException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
		return NULL;
	}
    
	//Get the host name given the address
    client_ip = string(inet_ntoa((struct in_addr)client_adress.sin_addr));
	//gLog.WriteLog(eSeverityDebug, eCatDB, "%s\n", client_ip.c_str());

    // Create and return the new myTcpSocket object
    myTcpSocket* retSocket = NULL;
	retSocket = new myTcpSocket();

	if(retSocket)
		retSocket->setSocketId(newSocket);

    return retSocket;
}

void myTcpSocket::listenToClient(int totalNumPorts)
{
	// Listens to connecting clients, a server call

	try 
	{
		if (listen(m_iSock,totalNumPorts) == SOCKET_ERROR)
		{
		#ifdef WIN32
			int errorCode = 0;
			string errorMsg = "error calling listen():\n";
			detectErrorListen(&errorCode,errorMsg);
			myException socketListenException(errorCode,errorMsg);
			throw socketListenException;
		#else
			myException unixSocketListenException(0,"unix: error calling listen()");
			throw unixSocketListenException;
		#endif
        }
	}
    catch(myException& excp)
	{
		excp.response();
	}
}       

void myTcpSocket::set_clientid(std::string id)
{ 
	client_id = string(id); 
}

// send a string to the socket
bool myTcpSocket::SendString(char* pStr)
{
	if (send(m_iSock, (char *) pStr, strlen(pStr), 0) == -1)
	{
		perror("myTcpSocket::SendString, send");              
		return false;
	}

	if (gPrefs.net.get_debug()) 
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: sending string '%s'\n", pStr);

#ifdef DEBUG_ACK
	if (!RecvAck())
		return false;
	if (!SendAck())
		return false;
#endif

	return true;
}

// Send some bytes over the wire
bool myTcpSocket::SendBytes(char* pVals, int iLen)
{
	int i = 0;

	if (send(m_iSock, (char *) pVals, iLen, 0) == -1)
	{
		perror("myTcpSocket::SendBytes, send bytes");              
		return false;
	}

	if (gPrefs.net.get_debug())
	{ 
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: sending %d bytes - ", iLen);                       
		for (i = 0; i < iLen; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%d ", pVals[i]);
		gLog.WriteLog(eSeverityInfo, eCatNet, "\n");
	}

#ifdef DEBUG_ACK
	if (!RecvAck())
		return false;
	if (!SendAck())
		return false;
#endif

	return true;
}

// Send some integers over the wire
bool myTcpSocket::SendInts(int* pVals, int iLen)
{
	int*	buff	= NULL;
	char*	ptr		= NULL;
	char*	valptr	= NULL;
	int		i		= 0;
	int		j		= 0;

	// We may need to reverse the byte order, oh joy
	if (m_bReverse)
	{
		// Set the buff pointer to our previously allocated spot
		//buff	= new int[BUFF_SIZE];
		buff	= (int *) m_pBuffer; 
		ptr		= (char *) buff;
		valptr	= (char *) pVals;

		// We need to reverse the order of each set of bytes
		for (i = 0; i < iLen; i++)
		{
			for (j=0; j < sizeof(int); j++)
				ptr[i * sizeof(int) + j] = (char) valptr[(i + 1) * sizeof(int) - j - 1];
		}
		if (send(m_iSock, (char *) buff, sizeof(int) * iLen, 0) == -1)
		{
			perror("myTcpSocket::SendInts, send ints");           
			//delete buff;
			return false;
		}

		//delete buff;
	}
	else
	{
		if (send(m_iSock, (char *) pVals, sizeof(int) * iLen, 0) == -1)
		{
			perror("send ints");
			return false;
		}
	}

#if defined _DEBUG
	if (gPrefs.net.get_debug())
	{ 
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: sending %d ints\n", iLen);
		/*
		for (i = 0; i < iLen; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%d \n", pVals[i]);
		*/
	}
#endif

#ifdef DEBUG_ACK
	if (!RecvAck())
		return false;
	if (!SendAck())
		return false;
#endif

	return true;
}

// Send some floats over the wire
bool myTcpSocket::SendFloats(float* pVals, int iLen)
{
	float*	buff	= NULL;
	char*	ptr		= NULL;
	char*	valptr	= NULL;
	int		i		= 0;
	int		j		= 0;

	// We may need to reverse the byte order, oh joy
	if (m_bReverse)
	{
		// Set the buff pointer to our previously allocated spot
		//buff	= new float[BUFF_SIZE];
		buff	= (float *) m_pBuffer; 
		ptr		= (char *)  buff;
		valptr	= (char *)  pVals;

		// We need to reverse the order of each set of bytes
		for (i = 0; i < iLen; i++)
		{
			for (j=0; j < sizeof(float); j++)
				ptr[i * sizeof(float) + j] = (char) valptr[(i + 1) * sizeof(float) - j - 1];
		}
		if (send(m_iSock, (char *) buff, sizeof(float) * iLen, 0) == -1)
		{
			perror("myTcpSocket::SendFloats, send floats");              		
			return false;
		}
	}
	else
	{
		if (send(m_iSock, (char *) pVals, sizeof(float) * iLen, 0) == -1)
		{
			perror("send floats");
			return false;
		}
	}

	if (gPrefs.net.get_debug())
	{ 
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: sending %d floats - ", iLen);                       
		for (i = 0; i < iLen; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%0.3f ", pVals[i]);
		gLog.WriteLog(eSeverityInfo, eCatNet, "\n");
	}

#ifdef DEBUG_ACK
	if (!RecvAck())
		return false;
	if (!SendAck())
		return false;
#endif

	return true;
}

// Send some floats over the wire
bool myTcpSocket::SendDoubles(double* pVals, int iLen)
{
	double*	buff	= NULL;
	char*	ptr		= NULL;
	char*	valptr	= NULL;
	int		i		= 0;
	int		j		= 0;

	// We may need to reverse the byte order, oh joy
	if (m_bReverse)
	{
		// Set the buff pointer to our previously allocated spot
		//buff	= new double[BUFF_SIZE];
		buff	= (double*) m_pBuffer; 
		ptr		= (char *)  buff;
		valptr	= (char *)  pVals;

		// We need to reverse the order of each set of bytes
		for (i = 0; i < iLen; i++)
		{
			for (j=0; j < sizeof(double); j++)
				ptr[i * sizeof(double) + j] = (char) valptr[(i + 1) * sizeof(double) - j - 1];
		}
		if (send(m_iSock, (char *) buff, sizeof(double) * iLen, 0) == -1)
		{
			perror("myTcpSocket::SendDoubles, send double");              		
			return false;
		}
	}
	else
	{
		if (send(m_iSock, (char *) pVals, sizeof(double) * iLen, 0) == -1)
		{
			perror("myTcpSocket::SendDoubles, send double");
			return false;
		}
	}

	if (gPrefs.net.get_debug())
	{ 
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: sending %d doubles - ", iLen);                       
		for (i = 0; i < iLen; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%0.3f ", pVals[i]);
		gLog.WriteLog(eSeverityInfo, eCatNet, "\n");
	}

#ifdef DEBUG_ACK
	if (!RecvAck())
		return false;
	if (!SendAck())
		return false;
#endif

	return true;
}

// Receive a string, returns num of bytes received.
int myTcpSocket::RecvString(char* pStr, int iMax, char chTerm)
{
	int		iNumBytes	= 0;
	bool	bEnd		= false;
	int		i			= 0;
	int		j			= 0;
	int		iLastRead	= 0;

	// Set the temp buffer to our already allocated spot
	char* pTemp = (char *) m_pBuffer;

	// This is annoying, but the java end is sending a char
	// at a time, so we recv some chars (probably 1), append
	// it to our str string, then carry on until we see
	// the terminal character.
	while (!bEnd)
	{
		if ((iLastRead = recv(m_iSock, pTemp, BUFF_SIZE, 0)) == -1)
		{
			perror("myTcpSocket::RecvString, recv");
			return -1;
		}
		for (i = 0; i < iLastRead; i++)
		{
			pStr[j] = pTemp[i];	
			j++;
		}
		if ((pTemp[i - 1] == chTerm) || (j == (iMax - 1)))
			bEnd = true;

		iNumBytes += iLastRead;
	}

	pStr[j] = '\0';

	if (gPrefs.net.get_debug()) 
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: received '%s'\n", pStr);                       	

#ifdef DEBUG_ACK
	if (!SendAck())
		return -1;
	if (!RecvAck())
		return -1;
#endif

	return iNumBytes;
}


// Receive some bytes, returns number of bytes received.
int myTcpSocket::RecvBytes(char* pVals, int iLen)
{
	int			i				= 0;
	int			j				= 0;
	char*		pTemp			= NULL;
	int			iTotalBytes		= 0;
	int			iNumBytes		= 0;
	bool		bEnd			= false;

	pTemp = (char *) m_pBuffer;

	// We receiving the incoming ints one byte at a time.
	while (!bEnd)
	{
		if ((iNumBytes = recv(m_iSock, pTemp, BUFF_SIZE, 0)) == -1)
		{
			perror("myTcpSocket::RecvBytes, recv");
			return -1;
		}
		for (i = 0; i < iNumBytes; i++)
		{
			pVals[j] = pTemp[i];
			j++;
		}

		iTotalBytes += iNumBytes;
		if (iTotalBytes == iLen)
			bEnd = true;
	}

	if (gPrefs.net.get_debug())
	{
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: received %d bytes - ", iTotalBytes);
		for (i = 0; i < iTotalBytes; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%d ", pVals[i]);
		gLog.WriteLog(eSeverityInfo, eCatNet, "\n");
	}

#ifdef DEBUG_ACK
	if (!SendAck())
		return -1;
	if (!RecvAck())
		return -1;
#endif

	return iTotalBytes;
}


// Receive some ints, returns num of ints received.
int myTcpSocket::RecvInts(int* pVals, int iLen)
{
	int			i				= 0;
	int			j				= 0;
	char*		pTemp			= NULL;
	char*		pResult			= NULL;
	int			iTotalBytes		= 0;
	int			iNumBytes		= 0;
	bool		bEnd			= false;

	pTemp	= (char *) m_pBuffer;
	pResult	= (char *) m_pBuffer2;

	// We receiving the incoming ints one byte at a time.
	while (!bEnd)
	{
		iNumBytes = recv(m_iSock, (char*) pTemp, sizeof(int) * iLen, 0);
		//gLog.WriteLog(eSeverityInfo, eCatNet, "Reading %d ints, Received %d bytes\n", iLen, iTotalBytes);
        if ( iNumBytes > 0 )
		{
#if defined _DEBUG
            gLog.WriteLog(eSeverityInfo, eCatNet,"Bytes received: %d\n", iNumBytes);
#endif
		}
        else if ( iNumBytes == 0 )
		{
            // gLog.WriteLog(eSeverityInfo, eCatNet,"Connection closed\n");
			return 0;
		}
        else
		{
            gLog.WriteLog(eSeverityInfo, eCatNet,"recv failed: %d\n", WSAGetLastError());
			return -1;
		}

		for (i = 0; i < iNumBytes; i++)
		{
			pResult[j] = pTemp[i];	
			j++;
		}

		iTotalBytes += iNumBytes;
		if (iTotalBytes == (iLen * sizeof(int)))
			bEnd = true;
	}

	// Now we need to put the array of bytes into the array of ints
	char*	ptr		= (char *) pVals;
	int		iNum	= j / sizeof(int);

	// The significance order depends on the platform
	if (m_bReverse)
	{
		// we need to reverse the order of each set of bytes
		for (i = 0; i < iNum; i++)
		{
			for (j = 0; j < sizeof(int); j++)
				ptr[i * sizeof(int) + j] = (char) pResult[(i + 1) * sizeof(int) - j - 1];
		}
	}
	else
	{
		// Leave the byte order as is
		for (i = 0; i < j; i++)
		{
			ptr[i] = pResult[i];
		}
	}

	if (gPrefs.net.get_debug()) 
	{
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: received %d ints\n", iNum);
		/*
		for (i = 0; i < iNum; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%d\n", pVals[i]);
		*/
	}

	//delete pTemp;
	//delete pResult;

#ifdef DEBUG_ACK
	if (!SendAck())
		return -1;
	if (!RecvAck())
		return -1;
#endif

	return iNum;
}

// Receive some floats, returns num of floats received.
int myTcpSocket::RecvFloats(float* pVals, int iLen)
{
	int			i				= 0;
	int			j				= 0;
	char*		pTemp			= NULL;
	char*		pResult			= NULL;
	int			iTotalBytes		= 0;
	int			iNumBytes		= 0;
	bool		bEnd			= false;

	pTemp	= (char *) m_pBuffer;
	pResult	= (char *) m_pBuffer2;

	// We receiving the incoming ints one byte at a time.
	while (!bEnd)
	{
		if ((iNumBytes = recv(m_iSock, pTemp, BUFF_SIZE, 0)) == -1)
		{
			perror("myTcpSocket::RecvFloats, recv");
			return -1;
		}
		for (i = 0; i < iNumBytes; i++)
		{
			pResult[j] = pTemp[i];	
			j++;
		}

		iTotalBytes += iNumBytes;
		if (iTotalBytes == (iLen * sizeof(float)))
			bEnd = true;
	}

	// Now we need to put the array of bytes into the array of ints
	char*	ptr		= (char *) pVals;
	int		iNum	= j / sizeof(float);

	// The significance order depends on the platform
	if (m_bReverse)
	{
		// we need to reverse the order of each set of bytes
		for (i = 0; i < iNum; i++)
		{
			for (j = 0; j < sizeof(float); j++)
				ptr[i * sizeof(float) + j] = (char) pResult[(i + 1) * sizeof(float) - j - 1];
		}
	}
	else
	{
		// Leave the byte order as is
		for (i = 0; i < j; i++)
		{
			ptr[i] = pResult[i];
		}
	}

	if (gPrefs.net.get_debug()) 
	{
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: received %d floats - ", iNum);             	
		for (i = 0; i < iNum; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%0.3f ", pVals[i]);
		gLog.WriteLog(eSeverityInfo, eCatNet, "\n");
	}

#ifdef DEBUG_ACK
	if (!SendAck())
		return -1;
	if (!RecvAck())
		return -1;
#endif

	return iNum;
}

// Receive some doubles, returns num of doubles received.
int myTcpSocket::RecvDoubles(double* pVals, int iLen)
{
	int			i				= 0;
	int			j				= 0;
	char*		pTemp			= NULL;
	char*		pResult			= NULL;
	int			iTotalBytes		= 0;
	int			iNumBytes		= 0;
	bool		bEnd			= false;

	pTemp	= (char *) m_pBuffer;
	pResult	= (char *) m_pBuffer2;

	// We receiving the incoming ints one byte at a time.
	while (!bEnd)
	{
		if ((iNumBytes = recv(m_iSock, pTemp, BUFF_SIZE, 0)) == -1)
		{
			perror("myTcpSocket::RecvDoubles, recv");
			return -1;
		}
		for (i = 0; i < iNumBytes; i++)
		{
			pResult[j] = pTemp[i];	
			j++;
		}

		iTotalBytes += iNumBytes;
		if (iTotalBytes == (iLen * sizeof(double)))
			bEnd = true;
	}

	// Now we need to put the array of bytes into the array of ints
	char*	ptr		= (char *) pVals;
	int		iNum	= j / sizeof(double);

	// The significance order depends on the platform
	if (m_bReverse)
	{
		// we need to reverse the order of each set of bytes
		for (i = 0; i < iNum; i++)
		{
			for (j = 0; j < sizeof(double); j++)
				ptr[i * sizeof(double) + j] = (char) pResult[(i + 1) * sizeof(double) - j - 1];
		}
	}
	else
	{
		// Leave the byte order as is
		for (i = 0; i < j; i++)
		{
			ptr[i] = pResult[i];
		}
	}

	if (gPrefs.net.get_debug()) 
	{
		gLog.WriteLog(eSeverityInfo, eCatNet, "Server: received %d doubles - ", iNum);             	
		for (i = 0; i < iNum; i++)
			gLog.WriteLog(eSeverityInfo, eCatNet, "%0.3f ", pVals[i]);
		gLog.WriteLog(eSeverityInfo, eCatNet, "\n");
	}

#ifdef DEBUG_ACK
	if (!SendAck())
		return -1;
	if (!RecvAck())
		return -1;
#endif

	return iNum;
}

// Receive a short ack from the client 
bool myTcpSocket::RecvAck()
{
	char temp[1];
	int iTotal = 0;

	//gLog.WriteLog(eSeverityDebug, eCatNet, "Waiting for ack...\n");

	int iResult = 0;
	while ((iTotal < 1) && (iResult != -1))
	{
		iResult = recv(m_iSock, temp, 1, 0);	
		iTotal += iResult;
	}
	if (iResult == -1)
	{
		perror("Server::RecvAck, recv");
		return false;
	}

	//gLog.WriteLog(eSeverityDebug, eCatNet, "Ack recieved.\n");

	return true;
}	

// Send a short ack to the client 
bool myTcpSocket::SendAck()
{
	char temp[1];
	temp[0] = 42;

	//gLog.WriteLog(eSeverityDebug, eCatNet, "Sending ack...\n");

	if (send(m_iSock, temp, 1, 0) == -1)
		return false;
	return true;
}

