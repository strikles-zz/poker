#pragma once

#include "libpq-fe.h"

#include <string>
#include "Util/Util_MagicNumbers.h"


static const char* db_tables[16] = {
	"br1_0", 
	"br1_1", 
	"br1_2", 
	"br1_3", 
	"br2_0", 
	"br2_1", 
	"br2_2", 
	"br2_3", 
	"br3_0", 
	"br3_1", 
	"br3_2", 
	"br3_3", 
	"br4_0", 
	"br4_1", 
	"br4_2", 
	"br4_3"
};

static const int db_tables_helper[16][2] = {
	{ePreflopIndex, 0},
	{ePreflopIndex, 1},
	{ePreflopIndex, 2},
	{ePreflopIndex, 3},
	{eFlopIndex, 0},
	{eFlopIndex, 1},
	{eFlopIndex, 2},
	{eFlopIndex, 3},
	{eTurnIndex, 0},
	{eTurnIndex, 1},
	{eTurnIndex, 2},
	{eTurnIndex, 3},
	{eRiverIndex, 0},
	{eRiverIndex, 1},
	{eRiverIndex, 2},
	{eRiverIndex, 3}
};

static const char* preflop_query	=	"SELECT a.hand_id, %ACTIONS% %DIMENSIONS% "
										"FROM %BD_TABLE% as a "
										"INNER JOIN profits ON a.hand_id = profits.hand_id "
										"WHERE %ACTION_FILTER% %NDEALT% profits.profit IS NOT NULL "
										"ORDER BY a.hand_id ASC ";

static const char* postflop_query = "SELECT a.hand_id, %ACTIONS% %DIMENSIONS% "
									"FROM %BD_TABLE% as a "
									"INNER JOIN profits ON a.hand_id = profits.hand_id "
									"WHERE %ACTION_FILTER% %NDEALT% profits.profit IS NOT NULL "
									"ORDER BY a.hand_id ASC ";


std::string GetActionsColumnsStr(int times_acted, bool is_insert);
// Get the action column name into a vector
void GetActionsColumnsVector(std::vector<std::string>& actions, int times_acted, bool is_insert);

std::string GetDimColumnsStr(int br_ndx);
// Get the remaining column dim names into a vector
void GetDimColumnsVector(std::vector<std::string>& dims, int br_ndx);


class CDatabase
{

public:

	CDatabase();
	~CDatabase();

public:

	bool isConnectionOK();
	void Disconnect();
	void Connect();
	bool CheckDB(int br_ndx, int times_acted);

	PGconn* get_conn() const { return p_conn;}

protected:

	bool isConnected;
	bool CreateDB();
	
	PGconn* p_conn;
	std::string conn_str;
};

// Basic Query Functions
class CDatabaseQuery
{

public:

	CDatabaseQuery::CDatabaseQuery(CDatabase *db) : p_database(db), num_rows(0), num_columns(0), pg_result(0)
	{
	}
	~CDatabaseQuery()
	{
		PQclear(pg_result);
	};

public:

	void DebugInfo();

	bool ExecuteQuery(const char* query, ExecStatusType statusCheck);
	bool Select(const char* query);
	long Insert(const char* query);

	int get_numrows() { return num_rows; }
	int get_numcolumns() { return num_columns; }
	PGresult* get_pgresult() { return pg_result; }

private:

	CDatabase* p_database;
	PGresult* pg_result;
	int num_rows;
	int num_columns;
};

// Functions That use Queries above
class CDatabaseInterface
{
public:
	int					SaveHand(int* hand, CDatabase* p_db);
	int					SaveProfit(int* profit, CDatabase* p_db);
	static std::string	GetTableStr(enumStreetIndices br_ndx, int times_acted);
	void				GetActionsColumnsVector(std::vector<std::string>& actions, int times_acted);
	static std::string	ConstructSelectQueryString(enumStreetIndices br_ndx, int times_acted, int ndealt, eBetType action_type);
	static std::string	ConstructInsertQueryString(int* hand);
	static bool			GetProfit(int hand_id, double& profit, CDatabase* p_db);
};








