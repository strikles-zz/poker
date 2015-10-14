#include "Net/CException/CException.h"

#include "Util/Util_logging.h"

myException::myException(int errCode,const string& errMsg)
{
	initVars();
	errorCode = errCode;
	if (errMsg[0] ) 
		errorMsg.append(errMsg);
}

void myException::initVars()
{
	errorCode = 0;
	errorMsg = "";
}

void myException::response()
{
	//winLog << "Error detect: " << endl;
	//winLog << "		==> error code: " << errorCode << endl;
	//winLog << "		==> error message: " << errorMsg << endl;

	gLog.WriteLogNoHeading(eSeverityError, eCatNet, "Error detect - code: %d - message: %s\n", errorCode, errorMsg.c_str());
	//cout.flush();
}