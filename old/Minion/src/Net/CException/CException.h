#pragma once

#include <string>
#include <iostream>
using namespace std;

class myException
{

public:

    // int: error code, string is the concrete error message
	myException(int,const string&);   

	/*
	   how to handle the exception is done here, so 
	   far, just write the message to screen and log file
	*/
	virtual void response();  
	int getErrCode()    { return errorCode; }
	string& getErrMsg() { return errorMsg; }

private:
	void initVars();

private:
	int   errorCode;
	string errorMsg;
};