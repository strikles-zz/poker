#pragma once

class Generate2p2
{
public:

	__int64 *IDs;
	int *HR; 

	int numIDs;
	int numcards;
	int maxHR;
	__int64 maxID;

	Generate2p2();
	~Generate2p2();

	// adding a new card to this ID
	__int64 MakeID(__int64 IDin, int newcard);  
	int SaveID(__int64 ID);
	int DoEval(__int64 IDin);
	int Generate_DAT();
};



