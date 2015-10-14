#include <cassert>

#include "StructDefines/structdefines.h"

#include "CANN/CANN.h"
#include "CANN/CScaling.h"

#include "Config/CPreferences.h"
#include "DB/CDatabase.h"

#include "Util/Util.h"
#include "Util/Util_logging.h"




int CScaling::dynamic_metric_pre(int j, int value)	
	{ 
		int scaled = value;

		//10, 20, 100, 10, 0, 0, 2, 2, 20
		//10, 80,  10,  0, 0, 0, 2, 2, 50, 1, 1
		
		switch(j)
		{
			case eDim_HS:
				scaled *= 10;//8
				break;
			case eDim_InHand:
				scaled *= 20;//10
				break;
			case eDim_Dealt:
				scaled *= 100;//5
				break;
			case eDim_ToAct:
				scaled *= 10;//5
				break;
			case eDim_Balance:
				scaled *= 0;
				break;
			case eDim_InitialBalance:
				scaled *= 0;
				break;
			case eDim_Call:
				scaled *= 2;//1
				break;
			case eDim_Pot:
				scaled *= 2;//1
				break;
			case eDim_Raises:
				scaled *= 20;//50
				break;
			default:
				break;
		}
		
		return scaled;
	}
	
int CScaling::dynamic_metric_post(int j, int value)	
	{ 
		int scaled = value;
		
		switch(j)
		{
			case eDim_HS:
				scaled *= 10;//8
				break;
			case eDim_InHand:
				scaled *= 80;//10
				break;
			case eDim_Dealt:
				scaled *= 10;//5
				break;
			case eDim_ToAct:
				scaled *= 0;//5
				break;
			case eDim_Balance:
				scaled *= 0;
				break;
			case eDim_InitialBalance:
				scaled *= 0;
				break;
			case eDim_Call:
				scaled *= 2;//1
				break;
			case eDim_Pot:
				scaled *= 2;//1
				break;
			case eDim_Raises:
				scaled *= 50;//50
				break;
			case eDim_PPot:
				scaled *= 1;//1
				break;
			case eDim_NPot:
				scaled *= 1;//1
				break;
			default:
				break;
		}
		
		return scaled;
	}
	


int CScaling::get_dim_scale_preflop(int j, int value)
{	
	return dynamic_metric_pre(j, value); 
}
int CScaling::get_dim_scale_postflop(int j, int value)	
{ 
	return dynamic_metric_post(j, value); 
}




