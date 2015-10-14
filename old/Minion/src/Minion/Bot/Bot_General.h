#pragma once

#include "Util/Util_MagicNumbers.h"

double gws(int chair, const char* name, bool& iserr);
double gws(int chair, const char* name);
double gws(const char* name);

////////////////////////////
// AiKu's Ninja Macros :) //
////////////////////////////

/*
OpenHoldem Variables

OH_VAR(x) - double variable
OH_VAR_INT(x) - OH variable casted to an int
OH_VAR_UNSIGNED_INT(x) - OH variable casted to an unsigned int
OH_VAR_BOOL(x) - OH variable casted to an bool
OH_SEAT_VAR(x) - OH double variable that is applicable to all seats (becomes and array of values)

For all OH variables use: dirty_x();
*/

#define OH_VAR(x) \
	protected:\
	double priv##x;\
	bool bDirty_##x;\
	public:\
	void dirty_##x() { bDirty_##x = true; }\
	double get_##x() \
{ \
	if(bDirty_##x) { \
	priv##x = gws(userchair, #x);\
	bDirty_##x = false;\
	} \
	return priv##x; \
};

#define OH_VAR_INT(x) \
	protected:\
	int priv##x;\
	bool bDirty_##x;\
	public:\
	void dirty_##x() { bDirty_##x = true; }\
	int get_##x() \
{ \
	if(bDirty_##x) { \
	priv##x = (int)gws(userchair, #x);\
	bDirty_##x = false;\
	} \
	return priv##x; \
};

#define OH_VAR_USIGNED_INT(x) \
	protected:\
	unsigned int priv##x;\
	bool bDirty_##x;\
	public:\
	void dirty_##x() { bDirty_##x = true; }\
	unsigned int get_##x() \
{ \
	if(bDirty_##x) { \
	priv##x = (unsigned int)gws(userchair, #x);\
	bDirty_##x = false;\
	} \
	return priv##x; \
};

#define OH_VAR_BOOL(x) \
	protected:\
	bool priv##x;\
	bool bDirty_##x;\
	public:\
	void dirty_##x() { bDirty_##x = true; }\
	bool get_##x() \
{ \
	if(bDirty_##x) { \
	priv##x = (bool)gws(userchair, #x);\
	bDirty_##x = false;\
	} \
	return priv##x; \
};

#define OH_SEAT_VAR(x) \
	protected:\
	double priv##x[k_max_chairs];\
	bool bDirty_##x[k_max_chairs];\
	public:\
	void dirty_##x(int iSeat) { bDirty_##x[iSeat] = true; }\
	double get_##x(int iSeat) \
{ \
	if(bDirty_##x[iSeat]) { \
	char query[255];\
	sprintf_s(query, 255, "%s%d", #x, iSeat);\
	priv##x[iSeat] = gws(userchair, query);\
	bDirty_##x[iSeat] = false;\
	} \
	return priv##x[iSeat]; \
};