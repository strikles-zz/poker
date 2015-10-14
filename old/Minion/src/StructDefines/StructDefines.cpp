
#include "StructDefines/structdefines.h"
#include "Util/Util_logging.h"

void printHandStruct(int* hand)
{
	for (int i = 0; i < eDim_End; i++)
		gLog.WriteLog(eSeverityInfo, eCatANN, "%s\t=%d\n", dim_names[i], hand[i]);
}