#ifndef mySocket_H
#define mySocket_H

#ifdef WIN32
	#include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <iostream.h>
    #include <sys/types.h>
    #include <stropts.h>
    #include <sys/filio.h> 
#endif

#include "Net/CHostInfo/CHostInfo.h"
#include "Util/Util_MagicNumbers.h"


class mySocket
{

protected:

	/*
	   only used when the socket is used for client communication
	   once this is done, the next necessary call is setSocketId(int)
	*/
	mySocket(){};
	void setSocketId(SOCKET socketFd) { m_iSock = socketFd; }

public:

	bool				m_bReverse;							// Am I reversing byte order or not?
	int					m_iPort;							// The port I'm listening on
	int					m_iPortDatagram;					// Port I listen for datagrams on 
															// (can be same or different from port)
	//int					m_iListen;							// Descriptor we are listening on
	SOCKET				m_iSock;							// Descriptor for the socket

	/*
	struct sockaddr_in	m_addrRemote;						// Connector's address information
	struct sockaddr_in	m_addrMe;							// My address information
	*/

	//char*				m_pBuffer;							// Reuse the same memory for buffer
	//char*				m_pBuffer2;
	char m_pBuffer[sizeof(double) * BUFF_SIZE];
	char m_pBuffer2[sizeof(double) * BUFF_SIZE];


    int blocking;											// Blocking flag
    int bindFlag;											// Binding flag

    struct sockaddr_in client_addr;    // Address of the client that sent data

	/*
	char length_buffer[MSG_HEADER_SIZE+1];
	char type_buffer[MSG_HEADER_TYPE+1];
	char recv_msg_buffer[MAX_MSG_LEN];
	char send_msg_buffer[MAX_MSG_LEN];
	*/

public:

    mySocket(int);                       // given a port number, create a socket
    virtual ~mySocket();

public:

	// socket options : ON/OFF
    void setDebug(int);
    void setReuseAddr(int);
    void setKeepAlive(int);
    void setLingerOnOff(bool);
	void setLingerSeconds(int);
    void setSocketBlocking(int);

    // size of the send and receive buffer
    void setSendBufSize(int);
    void setReceiveBufSize(int);

    // retrieve socket option settings
    int  getDebug();
    int  getReuseAddr();
    int  getKeepAlive();
    int  getSendBufSize();
    int  getReceiveBufSize();
    int  getSocketBlocking() { return blocking; }
	int  getLingerSeconds();
    bool getLingerOnOff();
	
    // returns the socket file descriptor
	SOCKET getSocketId() { return m_iSock; }

	// returns the port number
	int getPortNumber() { return m_iPort; }

	// show the socket 
	friend ostream& operator<<(ostream&, mySocket&);

private:

	
#ifdef WIN32
	// Gets the system error
	void detectErrorOpenWinSocket(int*, string&);
	void detectErrorSetSocketOption(int*, string&);
	void detectErrorGetSocketOption(int*, string&);
#else
	char *sGetError()
	{
       return strerror(errno);
    }
#endif


};

class myTcpSocket : public mySocket
{

public:

	std::string client_id;

private:

/*
#ifdef WIN32
	int XPrecieveMessage(char* message, int& msg_size, eMessageType& msg_type);
#endif
*/

	void detectErrorBind(int*, string&);
	void detectErrorSend(int*, string&);
	void detectErrorRecv(int*, string&);
	void detectErrorConnect(int*, string&);
	void detectErrorAccept(int*, string&);
	void detectErrorListen(int*, string&);

public:

	/* 
	   Constructor. used for creating instances dedicated to client 
	   communication:

	   when accept() is successful, a socketId is generated and returned
	   this socket id is then used to build a new socket using the following
	   constructor, therefore, the next necessary call should be setSocketId()
	   using this newly generated socket fd
	*/
	myTcpSocket();
	~myTcpSocket();

	// Constructor.  Used to create a new TCP socket given a port
	myTcpSocket(int portId);

public:

	static void		initialize();
	/*
	int				sendMessage(char* message, eMessageType msg_type);
	int				receiveMessage(char* message, int& msg_len, eMessageType& msg_type);
	*/
	void			bindSocket();
	void			listenToClient(int numPorts = 5);
	virtual void	connectToServer(string&,hostType);
	myTcpSocket*	acceptClient(string& client_ip);

	bool			SendString(char* pStr);							// Send a string to socket
	bool			SendInts(int* pVals, int iLen);					// Send some integers
	bool			SendBytes(char* pVals, int iLen);				// Send some bytes
	bool			SendFloats(float* pVals, int iLen);				// Send some floats
	bool			SendDoubles(double* pVals, int iLen);			// Send some doubles

	int				RecvString(char* pStr, int iMax, char chTerm);	// Receive a string
	int				RecvInts(int* pVals, int iLen);  				// Receive some ints
	int				RecvFloats(float* pVals, int iLen);  			// Receive some floats
	int				RecvDoubles(double* pVals, int iLen);  			// Receive some doubles
	int				RecvBytes(char* pVals, int iLen);  				// Receive some bytes

	void			set_clientid(std::string id);
	const char*		get_clientid() { return client_id.c_str(); }

private:
	bool			RecvAck();
	bool			SendAck();
};

#endif
        
