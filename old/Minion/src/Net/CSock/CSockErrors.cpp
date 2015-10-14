
#pragma once

#include "Net/CSock/CSock.h"


#ifdef WIN32
void mySocket::detectErrorOpenWinSocket(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("Successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem or the associated service provider has failed.");
	else if ( *errCode == WSAEAFNOSUPPORT )
		errMsg.append("The specified address family is not supported.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("No more socket descriptors are available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available. The socket cannot be created.");
	else if ( *errCode == WSAEPROTONOSUPPORT )
		errMsg.append("The specified protocol is not supported.");
	else if ( *errCode == WSAEPROTOTYPE )
		errMsg.append("The specified protocol is the wrong type for this socket.");
	else if ( *errCode == WSAESOCKTNOSUPPORT )
		errMsg.append("The specified socket type is not supported in this address family.");
	else errMsg.append("unknown problems!");
}

void mySocket::detectErrorSetSocketOption(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("optval is not in a valid part of the process address space or optlen parameter is too small.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEINVAL )
		errMsg.append("level is not valid, or the information in optval is not valid.");
	else if ( *errCode == WSAENETRESET )
		errMsg.append("Connection has timed out when SO_KEEPALIVE is set.");
	else if ( *errCode == WSAENOPROTOOPT )
		errMsg.append("The option is unknown or unsupported for the specified provider or socket (see SO_GROUP_PRIORITY limitations).");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("Connection has been reset when SO_KEEPALIVE is set.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else errMsg.append("unknown problem!");
}

void mySocket::detectErrorGetSocketOption(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("One of the optval or the optlen parameters is not a valid part of the user address space, or the optlen parameter is too small.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The level parameter is unknown or invalid.");
	else if ( *errCode == WSAENOPROTOOPT )
		errMsg.append("The option is unknown or unsupported by the indicated protocol family.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");

	else errMsg.append("unknown problems!");
}

void myTcpSocket::detectErrorBind(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("A process on the machine is already bound to the same\n");
		errMsg.append("fully-qualified address and the socket has not been marked\n"); 
		errMsg.append("to allow address re-use with SO_REUSEADDR. For example,\n");
		errMsg.append("IP address and port are bound in the af_inet case");
	}
	else if ( *errCode == WSAEADDRNOTAVAIL )
		errMsg.append("The specified address is not a valid address for this machine.");
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The name or the namelen parameter is not a valid part of\n");
		errMsg.append("the user address space, the namelen parameter is too small,\n");
		errMsg.append("the name parameter contains incorrect address format for the\n");
		errMsg.append("associated address family, or the first two bytes of the memory\n");
		errMsg.append("block specified by name does not match the address family\n");
		errMsg.append("associated with the socket descriptor s.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The socket is already bound to an address. ");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("Not enough buffers available, too many connections.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else errMsg.append("unknown problems!");
}

void myTcpSocket::detectErrorRecv(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("The buf parameter is not completely contained in a valid part of the user address space.");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("The socket is not connected.");
	else if ( *errCode == WSAEINTR )
		errMsg.append("The (blocking) call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAENETRESET )
	{
		errMsg.append("The connection has been broken due to the keep-alive activity\n");
		errMsg.append("detecting a failure while the operation was in progress.");
	}
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
	{
		errMsg.append("MSG_OOB was specified, but the socket is not stream-style\n");
		errMsg.append("such as type SOCK_STREAM, out-of-band data is not supported\n");
		errMsg.append("in the communication domain associated with this socket, or\n");
		errMsg.append("the socket is unidirectional and supports only send operations.");
	}
	else if ( *errCode == WSAESHUTDOWN )
	{
		errMsg.append("The socket has been shut down; it is not possible to recv on a\n");
		errMsg.append("socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.");
	}
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and the receive operation would block.");
	else if ( *errCode == WSAEMSGSIZE )
		errMsg.append("The message was too large to fit into the specified buffer and was truncated.");
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The socket has not been bound with bind, or an unknown flag\n");
		errMsg.append("was specified, or MSG_OOB was specified for a socket with\n");
		errMsg.append("SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.");
	}
	else if ( *errCode == WSAECONNABORTED )
	{
		errMsg.append("The virtual circuit was terminated due to a time-out or\n");
		errMsg.append("other failure. The application should close the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAETIMEDOUT )
	{
		errMsg.append("The connection has been dropped because of a network\n");
		errMsg.append("failure or because the peer system failed to respond.");
	}
	else if ( *errCode == WSAECONNRESET )
	{
		errMsg.append("The virtual circuit was reset by the remote side executing a\n");
		errMsg.append("\"hard\" or \"abortive\" close. The application should close\n");
		errMsg.append("the socket as it is no longer usable. On a UDP datagram socket\n");
		errMsg.append("this error would indicate that a previous send operation\n");
		errMsg.append("resulted in an ICMP \"Port Unreachable\" message.");
	}
	else errMsg.append("unknown problems!");
}

void myTcpSocket::detectErrorConnect(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("The socket's local address is already in use and the socket\n");
		errMsg.append("was not marked to allow address reuse with SO_REUSEADDR. This\n");
		errMsg.append("error usually occurs when executing bind, but could be delayed\n");
		errMsg.append("until this function if the bind was to a partially wild-card\n");
		errMsg.append("address (involving ADDR_ANY) and if a specific address needs\n");
		errMsg.append("to be committed at the time of this function.");
	}
	else if ( *errCode == WSAEINTR )
		errMsg.append("The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or\n");
		errMsg.append("the service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEALREADY )
	{
		errMsg.append("A nonblocking connect call is in progress on the specified socket.\n");
		errMsg.append("Note In order to preserve backward compatibility, this error is\n");
		errMsg.append("reported as WSAEINVAL to Windows Sockets 1.1 applications that\n");
		errMsg.append("link to either WINSOCK.DLL or WSOCK32.DLL.");
	}
	else if ( *errCode == WSAEADDRNOTAVAIL )
		errMsg.append("The remote address is not a valid address (such as ADDR_ANY).");
	else if ( *errCode == WSAEAFNOSUPPORT )
		errMsg.append("Addresses in the specified family cannot be used with this socket.");
	else if ( *errCode == WSAECONNREFUSED )
		errMsg.append("The attempt to connect was forcefully rejected.");
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The name or the namelen parameter is not a valid part of\n");
		errMsg.append("the user address space, the namelen parameter is too small,\n");
		errMsg.append("or the name parameter contains incorrect address format for\n");
		errMsg.append("the associated address family.");
	}
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The parameter s is a listening socket, or the destination\n");
		errMsg.append("address specified is not consistent with that of the constrained\n");
		errMsg.append("group the socket belongs to.");
	}
	else if ( *errCode == WSAEISCONN )
		errMsg.append("The socket is already connected (connection-oriented sockets only).");
	else if ( *errCode == WSAENETUNREACH )
		errMsg.append("The network cannot be reached from this host at this time.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available. The socket cannot be connected.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAETIMEDOUT )
		errMsg.append("Attempt to connect timed out without establishing a connection.");
	else if ( *errCode == WSAEWOULDBLOCK )
	{
		errMsg.append("The socket is marked as nonblocking and the connection\n");
		errMsg.append("cannot be completed immediately.");
	}
	else if ( *errCode == WSAEACCES )
	{
		errMsg.append("Attempt to connect datagram socket to broadcast address failed\n");
		errMsg.append("because setsockopt option SO_BROADCAST is not enabled.");
	}
	else errMsg.append("unknown problems!");
}

void myTcpSocket::detectErrorAccept(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this FUNCTION.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("The addrlen parameter is too small or addr is not a valid part of the user address space.");
	else if ( *errCode == WSAEINTR )
		errMsg.append("A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The listen function was not invoked prior to accept.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("The queue is nonempty upon entry to accept and there are no descriptors available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
		errMsg.append("The referenced socket is not a type that supports connection-oriented service.");
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and no connections are present to be accepted.");
	else errMsg.append("unknown problems!");
}

void myTcpSocket::detectErrorListen(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("The socket's local address is already in use and the socket was\n");
		errMsg.append("not marked to allow address reuse with SO_REUSEADDR. This error\n");
		errMsg.append("usually occurs during execution of the bind function, but could\n");
		errMsg.append("be delayed until this function if the bind was to a partially\n");
		errMsg.append("wild-card address (involving ADDR_ANY) and if a specific address\n");
		errMsg.append("needs to be \"committed\" at the time of this function.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service\n");
		errMsg.append("provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The socket has not been bound with bind.");
	else if ( *errCode == WSAEISCONN )
		errMsg.append("The socket is already connected.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("No more socket descriptors are available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
		errMsg.append("The referenced socket is not of a type that supports the listen operation.");
	else errMsg.append("unknown problems!");
}

void myTcpSocket::detectErrorSend(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEACCES )
	{
		errMsg.append("The requested address is a broadcast address,\n");
		errMsg.append("but the appropriate flag was not set. Call setsockopt\n");
		errMsg.append("with the SO_BROADCAST parameter to allow the use of the broadcast address.");
	}
	else if ( *errCode == WSAEINTR )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call was canceled\n");
		errMsg.append("through WSACancelBlockingCall.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress,\n");
		errMsg.append("or the service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The buf parameter is not completely contained in a\n");
		errMsg.append("valid part of the user address space.");
	}
	else if ( *errCode == WSAENETRESET )
	{
		errMsg.append("The connection has been broken due to the keep-alive\n");
		errMsg.append("activity detecting a failure while the operation was in progress.");
	}
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("The socket is not connected.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
	{
		errMsg.append("MSG_OOB was specified, but the socket is not stream-style\n");
		errMsg.append("such as type SOCK_STREAM, out-of-band data is not supported\n");
		errMsg.append("in the communication domain associated with this socket,\n");
		errMsg.append("or the socket is unidirectional and supports only receive operations.");
	}
	else if ( *errCode == WSAESHUTDOWN )
	{
		errMsg.append("The socket has been shut down; it is not possible to send\n");
		errMsg.append("on a socket after shutdown has been invoked with how set\n");
		errMsg.append("to SD_SEND or SD_BOTH.");
	}
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and the requested operation would block.\n");
	else if ( *errCode == WSAEMSGSIZE )
	{
		errMsg.append("The socket is message oriented, and the message is larger\n");
		errMsg.append("than the maximum supported by the underlying transport.");
	}
	else if ( *errCode == WSAEHOSTUNREACH )
		errMsg.append("The remote host cannot be reached from this host at this time.");
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The socket has not been bound with bind, or an unknown flag\n");
		errMsg.append("was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.");
	}
	else if ( *errCode == WSAECONNABORTED )
	{
		errMsg.append("The virtual circuit was terminated due to a time-out or \n");
		errMsg.append("other failure. The application should close the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAECONNRESET )
	{
		errMsg.append("The virtual circuit was reset by the remote side executing a \"hard\" \n");
		errMsg.append("or \"abortive\" close. For UPD sockets, the remote host was unable to\n");
		errMsg.append("deliver a previously sent UDP datagram and responded with a\n");
		errMsg.append("\"Port Unreachable\" ICMP packet. The application should close\n");
		errMsg.append("the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAETIMEDOUT )
	{
		errMsg.append("The connection has been dropped, because of a network failure\n");
		errMsg.append("or because the system on the other end went down without notice.");
	}
	else errMsg.append("unknown problems!");
}


#endif