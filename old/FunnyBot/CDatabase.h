#pragma once

#include <string>
#include <vector>

#include "libpq-fe.h"
#include <ANN\ANN.h>
#include "Util_MagicNumbers.h"


static const char* preflopQuery		=	"SELECT %ACTIONS% "
											"dim_hand169, "
											"dim_players_in_hand, dim_players_yet_to_act, "
											"dim_stack_commitment, dim_call, dim_pot "
										"FROM %BD_TABLE%";

static const char* postflopQuery	=	"SELECT %ACTIONS% "
											"dim_handtype, dim_hand_strength, dim_board_texture, "
											"dim_players_in_hand, dim_players_yet_to_act, "
											"dim_stack_commitment, dim_call, dim_pot "
										"FROM %BD_TABLE%";


class CDatabase
{

public:

	CDatabase();
	~CDatabase();

public:

	void Disconnect();
	void Connect();

	void ExecuteQuery(PGresult** pg_result, const char* query, int& nRows, int& nColumns);
	void ClearQueryResult(PGresult* res);

	int FillAnnArray(ANNpointArray& dataPts, enumStreets br, int timesActed);
	int GetAction(enumStreets br, int timesActed, int ndx);

private:

	bool isConnected;
	
	PGconn* p_conn;
	std::string conn_str;
};

extern CDatabase gDB;






