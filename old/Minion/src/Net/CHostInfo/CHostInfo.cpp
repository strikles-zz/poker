#include "Net/CHostInfo/CHostInfo.h"

#include "Net/CException/CException.h"
//#include "Net/CLog/CLog.h"


myHostInfo::myHostInfo()
{
#ifdef WIN32
	char sName[HOST_NAME_LENGTH+1];
	memset(sName,0,sizeof(sName));
	gethostname(sName,HOST_NAME_LENGTH);

	try 
	{
		hostPtr = gethostbyname(sName);
		if (hostPtr == NULL)
		{
			int errorCode;
			string errorMsg = "";
			detectErrorGethostbyname(&errorCode,errorMsg);
			myException gethostbynameException(errorCode,errorMsg);
			throw gethostbynameException;
		}
	}
	catch(myException& excp)
	{
		excp.response();
		//std::exit(EXIT_FAILURE);
	}
#else
	openHostDb();
	// winLog<<"UNIX version myHostInfo() is called...\n";
#endif

}

myHostInfo::myHostInfo(string& hostName,hostType type)
{
#ifdef __linux
	searchHostDB = 0;
#endif

	try 
	{
		if (type == NAME)
		{
			// Retrieve host by name
			hostPtr = gethostbyname(hostName.c_str());

			if (hostPtr == NULL)
			{
			#ifdef WIN32
				int errorCode;
				string errorMsg = "";
				detectErrorGethostbyname(&errorCode,errorMsg);
				myException gethostbynameException(errorCode,errorMsg);
				throw gethostbynameException;
			#else
				myException gethostbynameException(0,"unix: error getting host by name");
				throw gethostbynameException;
			#endif
			}
        }
		else if (type == ADDRESS)
		{
			// Retrieve host by address
		    unsigned long netAddr = inet_addr(hostName.c_str());
			if (netAddr == -1)
			{
				myException inet_addrException(0,"Error calling inet_addr()");
				throw inet_addrException;
			}

	        hostPtr = gethostbyaddr((char *)&netAddr, sizeof(netAddr), AF_INET);
			if (hostPtr == NULL)
			{
			#ifdef WIN32
				int errorCode;
				string errorMsg = "";
				detectErrorGethostbyaddr(&errorCode,errorMsg);
				myException gethostbyaddrException(errorCode,errorMsg);
				throw gethostbyaddrException;
			#else
				myException gethostbynameException(0,"unix: error getting host by name");
				throw gethostbynameException;
			#endif
			}
        }
		else
		{
			myException unknownTypeException(0,"unknown host type: host name/address has to be given ");
			throw unknownTypeException;
		}
    }
	catch(myException& excp)
	{
		excp.response();
		//std::exit(EXIT_FAILURE);
	}
}

#ifdef WIN32
void myHostInfo::detectErrorGethostbyname(int* errCode,string& errorMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errorMsg.append("need to call WSAStartup to initialize socket system on Window system.");
	else if ( *errCode == WSAENETDOWN )
		errorMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAHOST_NOT_FOUND )
		errorMsg.append("Authoritative Answer Host not found.");
	else if ( *errCode == WSATRY_AGAIN )
		errorMsg.append("Non-Authoritative Host not found, or server failure.");
	else if ( *errCode == WSANO_RECOVERY )
		errorMsg.append("Nonrecoverable error occurred.");
	else if ( *errCode == WSANO_DATA )
		errorMsg.append("Valid name, no data record of requested type.");
	else if ( *errCode == WSAEINPROGRESS )
		errorMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEFAULT )
		errorMsg.append("The name parameter is not a valid part of the user address space.");
	else if ( *errCode == WSAEINTR )
		errorMsg.append("A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
}
void myHostInfo::detectErrorGethostbyaddr(int* errCode,string& errorMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errorMsg.append("A successful WSAStartup must occur before using this function.");
	if ( *errCode == WSAENETDOWN )
		errorMsg.append("The network subsystem has failed.");
	if ( *errCode == WSAHOST_NOT_FOUND )
		errorMsg.append("Authoritative Answer Host not found.");
	if ( *errCode == WSATRY_AGAIN )
		errorMsg.append("Non-Authoritative Host not found, or server failed."); 
	if ( *errCode == WSANO_RECOVERY )
		errorMsg.append("Nonrecoverable error occurred.");
	if ( *errCode == WSANO_DATA )
		errorMsg.append("Valid name, no data record of requested type.");
	if ( *errCode == WSAEINPROGRESS )
		errorMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	if ( *errCode == WSAEAFNOSUPPORT )
		errorMsg.append("The type specified is not supported by the Windows Sockets implementation.");
	if ( *errCode == WSAEFAULT )
		errorMsg.append("The addr parameter is not a valid part of the user address space, or the len parameter is too small.");
	if ( *errCode == WSAEINTR )
		errorMsg.append("A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
}
#endif

#ifdef __linux
char myHostInfo::getNextHost()
{
	// winLog<<"UNIX getNextHost() is called...\n";
    // Get the next host from the database
    if (searchHostDB == 1)
    {
         if ((hostPtr = gethostent()) == NULL)
              return 0;
         else
              return 1;
    }
    return 0;
}
#endif

