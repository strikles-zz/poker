
#include <cassert>
#include <iostream>

#include "DB/CDatabase.h"
#include "Config/CPreferences.h"

#include "Util/Util.h"
#include "Util/Util_logging.h"


void GetActionsColumnsVector(std::vector<std::string>& actions, int times_acted, bool is_insert)
{
	std::string action_tables_str = "";

	int loop_start = eDim_Action1;
	int loop_stop = (is_insert ? times_acted+1 : times_acted);
	for (int i = 0; i < loop_stop; i++)
		actions.push_back(dim_names[loop_start+i]);
}

void GetDimColumnsVector(std::vector<std::string>& dims, int br_ndx)
{
	int index_stop = (br_ndx == ePreflopIndex ? eDim_PF_End : eDim_End);
	for (int i = eDim_Start; i <= index_stop; i++)
		dims.push_back(dim_names[i]);
}

std::string GetActionsColumnsStr(int times_acted, bool is_insert)
{
	std::string action_tables_str = "";
	std::vector<std::string> actions;

	GetActionsColumnsVector(actions, times_acted, is_insert);
	for (size_t i = 0; i < actions.size(); i++)
		action_tables_str += (actions[i] + ",");

	return action_tables_str;
}

std::string GetDimColumnsStr(int br_ndx)
{
	std::string dims_str = "";
	std::vector<std::string> dims;

	GetDimColumnsVector(dims, br_ndx);
	for (size_t i = 0; i < dims.size(); i++)
		dims_str += (dims[i] + (i < dims.size()-1 ? "," : ""));

	return dims_str;
}


///////////////
// CDatabase //
///////////////
CDatabase::CDatabase()
{
	conn_str =	"host="			+ gPrefs.db.get_ip() + " " +
				"port="			+ ToString(gPrefs.db.get_port()) + " " +
				"user='"		+ gPrefs.db.get_user() + "' " +
				"password='"	+ gPrefs.db.get_pass() + "' " +
				"dbname='"		+ gPrefs.db.get_name() + "'";

	Connect();
}

CDatabase::~CDatabase()
{
	Disconnect();
}

bool CDatabase::CreateDB()
{
	bool created = false;
	return created;
}

bool CDatabase::CheckDB(int br_ndx, int times_acted)
{
	// Check if the db exists
	// TODO: Should separate these 2 - schema exists vs table and respective columns
	std::string str_db_exists = "SELECT 1 AS result FROM pg_database WHERE datname='kasper_knn'"; 
	CDatabaseQuery query_db_exists(this);
	query_db_exists.Select(str_db_exists.c_str());

	bool db_ok = true;
	bool db_exists = (atoi(PQgetvalue(query_db_exists.get_pgresult(), 0, 0)) == 1);

	if (!db_exists)
		db_ok = false;

	else
	{
		// Find out if the expected tables exist.
		// brX_Y
		// X = br (1..4); Y = times_acted (0..3)
				
		std::string str_db_tables = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'";
		CDatabaseQuery query_db_tables(this);
		query_db_tables.Select(str_db_tables.c_str());
		
		int i = 4 * br_ndx + times_acted;
		gLog.WriteLog(eSeverityInfo, eCatDB, "TEST TABLE : %s - %s\n", PQgetvalue(query_db_tables.get_pgresult(), i, 0), db_tables[i]);
		if (strcmp(PQgetvalue(query_db_tables.get_pgresult(), i, 0), db_tables[i]))
		{
			db_ok = false;
			system("PAUSE");
		}

		std::string str_table_columns = string("SELECT column_name FROM information_schema.columns WHERE table_name ='") + ToString(db_tables[i]) + string("'");
		CDatabaseQuery query_tables_columns(this);
	
		query_tables_columns.Select(str_table_columns.c_str());

		vector<std::string> expected_dim_names;
		if(!(db_tables_helper[i][0] == ePreflopIndex && db_tables_helper[i][1] == 0))
			expected_dim_names.push_back(string("id"));

		expected_dim_names.push_back(string("hand_id"));

		GetActionsColumnsVector(expected_dim_names, db_tables_helper[i][1], false);
		GetDimColumnsVector(expected_dim_names, db_tables_helper[i][0]);

		// Iterate through the columns and match column names to expected names !
		// Start at 2 to ignore junk
		for (int j = 0; j < query_tables_columns.get_numrows(); j++)
		{	
			gLog.WriteLog(eSeverityInfo, eCatDB, "TEST COLUMNS: %s - %s\n", PQgetvalue(query_tables_columns.get_pgresult(), j, 0), expected_dim_names[j].c_str());
			if (strcmp(PQgetvalue(query_tables_columns.get_pgresult(), j, 0), expected_dim_names[j].c_str()))
			{
				gLog.WriteLog(eSeverityError, eCatDB, "%s %s\n", PQgetvalue(query_tables_columns.get_pgresult(), j, 0), expected_dim_names[j].c_str());
				db_ok = false;
				system("PAUSE");
				break;
			}
		}
	}

	if (!db_ok)
	{
		/*
		if (db_exists)
			DropDB();
		*/
		return CreateDB();
	}

	return false;
}

void CDatabase::Connect()
{
	p_conn = PQconnectdb(conn_str.c_str());

	if(PQstatus(p_conn) == CONNECTION_OK)
	{
		//gLog.WriteLog(eSeverityInfo, eCatDB, "DB Opened successfully\n");
		isConnected = true;
	}
	else
	{
		gLog.WriteLog(eSeverityError, eCatDB, "ERROR opening PostgreSQL DB: %s\n", PQerrorMessage(p_conn));
		gLog.WriteLog(eSeverityError, eCatDB, "conn_str was = %s\n", conn_str.c_str());
		isConnected = false;
		Disconnect();
	}
}

void CDatabase::Disconnect()
{
	if(isConnected)
		PQfinish(p_conn);
}


bool CDatabase::isConnectionOK()
{
	bool is_connected = (isConnected && CONNECTION_OK == PQstatus(p_conn));
	if(is_connected)
		return true;
	else
	{
		gLog.WriteLog(eSeverityError, eCatDB, "not connected, trying to reconnect\n");	
		Connect();

		is_connected = isConnected && CONNECTION_OK == PQstatus(p_conn);
		if(is_connected)
			return true;
		else if(p_conn != NULL)
			gLog.WriteLog(eSeverityError, eCatDB, "Connection to DB Failed %s\n", conn_str.c_str());

		Disconnect();
		return false;
	}
}

////////////////////
// CDatabaseQuery //
////////////////////

void CDatabaseQuery::DebugInfo()
{
	if(gPrefs.db.get_debug())
	{
		gLog.WriteLog(eSeverityInfo, eCatDBQuery, "Pts:%d, nDims:%d\n", num_rows, num_columns);

		PQprintOpt options = {0};
		// Ask for column headers
		options.header    = 1;
		// Pad short columns for alignment
		options.align     = 1;
		// Use a pipe as the field separator
		options.fieldSep  = "|";

		PQprint(stdout, pg_result, &options);
	}
}

bool CDatabaseQuery::ExecuteQuery(const char* query, ExecStatusType statusCheck)
{
	if(!p_database->isConnectionOK())
		return false;

	if(gPrefs.db.get_debug())
		gLog.WriteLog(eSeverityInfo, eCatDBQuery, "%s\n", query);

	// do the query against the PT database
	pg_result = PQexec(p_database->get_conn(), query);

	if(!pg_result)
	{
		DebugInfo();
		gLog.WriteLog(eSeverityInfo, eCatDBQuery, "query failed. Null PTR - No error code available\n");
		return false;
	}

	// check for errors and clean-up
	ExecStatusType curr_status = PQresultStatus(pg_result);
	switch(curr_status)
	{
		case PGRES_COMMAND_OK:
		{
			//gLog.WriteLog(eSeverityInfo, eCatDBQuery, "%s query executed OK - %s rows affected\n", query, PQcmdTuples(pg_result));
			break;
		}
		case PGRES_TUPLES_OK:
		{
			//gLog.WriteLog(eSeverityInfo, eCatDBQuery, "%s may have returned data: %s\n", query, PQfname(pg_result, 0));
			break;
		}
		default:
		{
			DebugInfo();
			gLog.WriteLog(eSeverityError, eCatDBQuery, "query failed\n");
			gLog.WriteLog(eSeverityError, eCatDBQuery, "(%d) Error:%s\n", PQresStatus(curr_status), PQresultErrorMessage(pg_result));
			//gDB.Disconnect();

			return false;
		}
	}

	num_rows	= PQntuples(pg_result);
	num_columns	= PQnfields(pg_result);

	if(!num_rows)
	{
		gLog.WriteLog(eSeverityError, eCatDBQuery, "query %s failed. No rows returned\n", query);
		//system("PAUSE");
		return false;
	}

	//DebugInfo();
	return true;
}

// Used in CANNINterface and CANNObject
bool CDatabaseQuery::Select(const char* query)
{
	return ExecuteQuery(query, PGRES_COMMAND_OK);
}

// Used in Logic_knn and CKNN
long CDatabaseQuery::Insert(const char* query)
{
	/*
		returns the inserted hand_id or -1 if the command executed OK
	*/
	ExecStatusType statusCheck = PGRES_COMMAND_OK;
	
	if (!ExecuteQuery(query, statusCheck))
		return -1;

	char* hand_id_str = PQgetvalue(pg_result, 0, 0);
	char* p_end;
	long hand_id = strtol(hand_id_str, &p_end, 10);
	//assert (hand_id > 0);

	return hand_id;
}


////////////////////////
// CDatabaseInterface //
////////////////////////

std::string CDatabaseInterface::GetTableStr(enumStreetIndices br_ndx, int times_acted)
{
	return "br" + ToString(br_ndx+1) + "_" + ToString(times_acted);
}

// Used in CANNObject::FillAnnArray
std::string CDatabaseInterface::ConstructSelectQueryString(enumStreetIndices br_ndx, int times_acted, int ndealt, eBetType action_type)
{
	std::string db_table_str = GetTableStr(br_ndx,times_acted);
	std::string db_select_query_str = "";

	if(ePreflopIndex == br_ndx)
		db_select_query_str = std::string(preflop_query);
	else
		db_select_query_str = std::string(postflop_query);

	bool foundActionsToken = false, foundActionFilterToken = false, foundBdTableToken = false, foundDimsToken = false, foundNDealtToken = false;
	size_t posActions, posActionFilter, posDims, posBdTable, posNDealt;

	posBdTable = db_select_query_str.find("%BD_TABLE%");
	if(std::string::npos != posBdTable)
	{
		db_select_query_str.replace(posBdTable, 10, db_table_str);
		foundBdTableToken = true;
	}

	posActions = db_select_query_str.find("%ACTIONS%");
	if(std::string::npos != posActions)
	{
		db_select_query_str.replace(posActions, 9, GetActionsColumnsStr(times_acted, false));
		foundActionsToken = true;
	}

	posDims = db_select_query_str.find("%DIMENSIONS%");
	if(std::string::npos != posDims)
	{
		std::string dims_str = GetDimColumnsStr(br_ndx);
		db_select_query_str.replace(posDims, 12, dims_str);
		foundDimsToken = true;
	}

	posActionFilter = db_select_query_str.find("%ACTION_FILTER%");
	if(std::string::npos != posActionFilter)
	{
		if(action_type != eTotalActions)
		{
			std::string filter_str = "dim_action" + ToString(times_acted+1) + " = " + ToString(action_type) + " AND ";
			db_select_query_str.replace(posActionFilter, 15, filter_str);
		}
		else
		{
			db_select_query_str.replace(posActionFilter, 15, "");
		}

		foundActionFilterToken = true;
	}

	posNDealt = db_select_query_str.find("%NDEALT%");
	if(std::string::npos != posNDealt)
	{
		std::string ndealt_str = "dim_players_dealt = " + ToString(ndealt) + " AND ";
		db_select_query_str.replace(posNDealt, 8, ndealt_str);
		foundNDealtToken = true;
	}

	// Error handling - finish me
	if(!foundActionsToken || !foundBdTableToken)
	{
		gLog.WriteLog(eSeverityError, eCatANN, "Invalid parameter - br / times acted\n");
	}	

	return db_select_query_str;
}

// Used by Logic_knn and CKNN
std::string CDatabaseInterface::ConstructInsertQueryString(int* hand)
{
	int times_acted = min(hand[eDim_timesActed], k_max_actions-1);
	std::string db_table_str = GetTableStr((enumStreetIndices)hand[eDim_brNdx], times_acted);

	// Add 1 since we are saving the action we just made
	// or not
	std::string action_columns_str = GetActionsColumnsStr(times_acted, true);
	std::string action_values_str = "";

	for (int i = eDim_Action1; i <= eDim_Action1 + times_acted; i++)
		action_values_str += ToString(hand[i]) + ",";

	std::string hand_id_column_str	= "hand_id, ";
	std::string hand_id_value_str	= ToString(hand[eDim_handId]) + ",";
	std::string tail_columns_str, tail_values_str;

	int index_stop = eDim_End;

	// No hand_id yet
	// One will be returned
	if(ePreflopIndex == hand[eDim_brNdx])
	{
		if(0 == times_acted)
		{
			//hand_id_column_str = "";
			hand_id_value_str = "DEFAULT,";
		}
		index_stop = eDim_PF_End;
	}

	// Iterate dims and construct the string
	tail_columns_str = GetDimColumnsStr(hand[eDim_brNdx]);
	for (int i = eDim_Start; i <= index_stop; i++)
		tail_values_str += ToString(hand[i]) + ToString(i == index_stop ? "" : ",");

	std::string query_str = ("INSERT INTO " + db_table_str + 
		" (" + hand_id_column_str + action_columns_str + tail_columns_str + ")"
		" VALUES (" + hand_id_value_str + action_values_str + tail_values_str + ")" 
		+ " RETURNING hand_id");

	return query_str;
}

// DB
bool CDatabaseInterface::GetProfit(int hand_id, double& profit, CDatabase* p_db)
{
	std::string query = "SELECT profit"
						" FROM profits "
						" WHERE profits.hand_id = " + ToString(hand_id) + 
						" AND profits.profit IS NOT NULL";

	CDatabaseQuery db_query(p_db);
	db_query.Select(query.c_str());

	if(db_query.get_numcolumns() != 1  || db_query.get_numrows() != 1)
	{
		gLog.WriteLog(eSeverityError, eCatDBQuery, "FIXME: Query: %s\n", query.c_str());
		gLog.WriteLog(eSeverityError, eCatDBQuery, "FIXME: profit: %s\n", PQgetvalue(db_query.get_pgresult(), 0, 0));
		profit = -DBL_MAX;
		return false;
	}

	profit = atof(PQgetvalue(db_query.get_pgresult(), 0, 0));

	return true;
}

int CDatabaseInterface::SaveHand(int* hand, CDatabase* p_db)
{
	assert(hand[eDim_brNdx] >= ePreflopIndex && hand[eDim_brNdx] < eRoundIndices);
	assert(hand[eDim_timesActed] >= 0);

	// Error code for times_acted > k_max_actions
	int inserted_hand_id = -9999;
	std::string bd_table = "br" + ToString(hand[eDim_brNdx]+1) + "_" + ToString(hand[eDim_timesActed]);
	std::string db_check_handid_str = "SELECT COUNT(*) FROM " + bd_table + " WHERE hand_id = " + ToString(hand[eDim_handId]);
	
	CDatabaseQuery dbSanityCheckQuery(p_db);
	dbSanityCheckQuery.Select(db_check_handid_str.c_str());

	std::string query_str = CDatabaseInterface::ConstructInsertQueryString(hand);
	CDatabaseQuery dbQuery(p_db);
	inserted_hand_id = dbQuery.Insert(query_str.c_str());

	gLog.WriteLog(eSeverityInfo, eCatDBQuery, "SaveHandSituation() - %s\n", query_str.c_str());
	if(inserted_hand_id < 0)
	{
		gLog.WriteLog(eSeverityError, eCatDBQuery, "ERROR: SaveHandSituation() - inserted_hand_id is invalid - FIXME !!!\n");
	}

	return inserted_hand_id;
}

int CDatabaseInterface::SaveProfit(int* profit, CDatabase* p_db)
{
	if(profit[eProfit_handId] < 0)
	{
		gLog.WriteLog(eSeverityError, eCatDBQuery, "ERROR: SaveProfit() - hand{eDim_handId] is invalid - FIXME !!!\n");
		return -1;
	}

	std::string db_profit_check_handid_str = "SELECT COUNT(*) FROM profits WHERE hand_id = " + ToString(profit[eProfit_handId]);
	// Should probably check if hand_id exists
	std::string db_profit_insert_query_str = "INSERT INTO profits (hand_id, profit) " 
										"VALUES (" 
											+ ToString(profit[eProfit_handId]) + "," 
											+ ToString(profit[eProfit_Amt]) + 
										") RETURNING hand_id";

	CDatabaseQuery dbInsertQuery(p_db);
	int inserted_hand_id = dbInsertQuery.Insert(db_profit_insert_query_str.c_str());
	
	gLog.WriteLog(eSeverityInfo, eCatDBQuery, "SaveProfit() - %s\n", db_profit_insert_query_str.c_str());
	if(inserted_hand_id < 0)
		gLog.WriteLog(eSeverityError, eCatDBQuery, "ERROR: SaveProfit() - inserted_hand_id is invalid - FIXME !!!\n");

	return inserted_hand_id;
}
