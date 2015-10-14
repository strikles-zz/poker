#include "stdafx.h"

#include <cassert>

#include <iostream>
#include <sstream>

#include "CDatabase.h"
#include "Util_MagicNumbers.h"


template <typename T>
std::string ToString ( T Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}


CDatabase gDB;

CDatabase::CDatabase()
{
	conn_str =	"host="			+ std::string(m_host) + 
				" port="		+ std::string(m_port) + 
				" user='"		+ std::string(m_username) + 
				"' password='"	+ std::string(m_password) + 
				"' dbname='"	+ std::string(m_dbname) + 
				"'";

	Connect();
}

CDatabase::~CDatabase()
{
    Disconnect();
}

void CDatabase::Connect()
{
	p_conn = PQconnectdb(conn_str.c_str());

	if (PQstatus(p_conn) == CONNECTION_OK)
	{
		printf("DB %s Opened successfully\n", m_dbname);
		isConnected = true;
	}
	else
	{
		printf("ERROR opening PostgreSQL DB: %s\n", PQerrorMessage(p_conn));
		isConnected = false;
		Disconnect();
	}
}

void CDatabase::Disconnect()
{
	PQfinish(p_conn);
}



void CDatabase::ExecuteQuery(PGresult** pg_result, const char* query, int& nRows, int& nColumns)
{
	if(!isConnected || PQstatus(p_conn) != CONNECTION_OK)
	{
		printf("not connected, trying to reconnect\n");
		Connect();

		if(!isConnected)
		{
			printf("Connection to DB Failed, Error:%s\n", PQerrorMessage(p_conn));
			PQclear(*pg_result);
			Disconnect();
		}
	}
	else
	{
		// do the query against the PT database
		*pg_result = PQexec(p_conn, query);

		// check for errors and clean-up
		if (PQresultStatus(*pg_result) != PGRES_TUPLES_OK)
		{
			printf("%s query failed: %s\n", query, PQerrorMessage(p_conn));
			PQclear(*pg_result);
			Disconnect();
		}

		nRows		= PQntuples(*pg_result);
		nColumns	= PQnfields(*pg_result);

		if (DEBUG_DB)
		{
			PQprintOpt        options = {0};
			// Ask for column headers
			options.header    = 1;
			// Pad short columns for alignment
			options.align     = 1;
			// Use a pipe as the field separator
			options.fieldSep  = "|";
			PQprint( stdout, *pg_result, &options );
		}
	}
}

int CDatabase::FillAnnArray(ANNpointArray& dataPts, enumStreets br, int timesActed)
{
	assert(br >= ePreflop && br < eRounds);
	assert(timesActed >= 0 && timesActed < maxActions);

	std::string query;
	std::string actionTables;
	std::string bdTable;

	if (br == ePreflop)
	{
		query = std::string(preflopQuery);
		bdTable = "br1_" + ToString(timesActed);
	}
	else
	{
		query = std::string(postflopQuery);
		bdTable = "br" + ToString(br+1) + "_" + ToString(timesActed);
	}

	switch (timesActed)
	{
		case 0:
			actionTables = "";
			break;
		case 1:
			actionTables = "dim_action1,";
			break;
		case 2:
			actionTables = "dim_action1, dim_action2,";
			break;
		case 3:
			actionTables = "dim_action1, dim_action2, dim_action3,";
			break;
		default:
			break;
	}

	bool foundActionsToken = false, foundBdTableToken = false;
	size_t posActionTables, posBdTable;

	posBdTable = query.find("%BD_TABLE%");
	if(posBdTable != std::string::npos)
	{
		query.replace(posBdTable, 10, bdTable);
		foundBdTableToken = true;
	}

	posActionTables = query.find("%ACTIONS%");
	if(posActionTables != std::string::npos)
	{
		query.replace(posActionTables, 9, actionTables);
		foundActionsToken = true;
	}

	if(foundActionsToken == false || foundBdTableToken == false)
	{
		printf("Invalid parameter - br / times acted\n");
	}	

	int nPts = 0;
	int nDims = 0;
	PGresult* pg_result = NULL;
	ExecuteQuery(&pg_result, query.c_str(), nPts, nDims);

	if (DEBUG_DB)
		printf("\nPts:%d, nDims:%d\n", nPts, nDims);

	for (int pnt = 0; pnt < nPts; pnt++)
	{
		for (int dim = 0; dim < nDims; dim++)
		{
			dataPts[pnt][dim] = atof(PQgetvalue(pg_result, pnt, dim));
		}
	}

	PQclear(pg_result);

	return nPts;
}

int CDatabase::GetAction(enumStreets br, int timesActed, int ndx)
{
	assert(br >= ePreflop && br < eRounds);
	assert(timesActed >= 0 && timesActed < maxActions);

	std::string actionColumn;
	std::string bdTable;

	if (br == ePreflop)
		bdTable = "br1_" + ToString(timesActed);
	else
		bdTable = "br" + ToString(br+1) + "_" + ToString(timesActed);

	actionColumn = "dim_action" + ToString(timesActed+1);

	std::string query = "SELECT " + actionColumn + " FROM " + bdTable + " WHERE hand_id = " + ToString(ndx+1);

	int nRows		= 0;
	int nColumns	= 0;
	PGresult* pg_result = NULL;
	ExecuteQuery(&pg_result, query.c_str(), nRows, nColumns);

	if (DEBUG_DB)
	{
		printf("\nPts:%d, nDims:%d\n", nRows, nColumns);
		assert(nRows == 1 && nColumns == 1);
	}

	int decision = atoi(PQgetvalue(pg_result, 0, 0));

	return decision;
}


