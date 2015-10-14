#pragma once
	
#include <fstream>
#include <map>
#include <string>

static int CLUBS = 0;
static int HEARTS = 1;
static int DIAMONDS = 2;
static int SPADES = 3;

// 91 for offsuited hands and suited hands, which don't have the suit of board (e.g.: AhKh : 5s7s9s same as AhKd : 5s8s9s)
static int SUIT_IND_COMBOS = 91;
// 78 for suited hands, where suit matters (e.g.: AsKs : 5s7s9s different to AhKh : 5s7s9s)
static int SUIT_DEP_COMBOS = 78;

void LUT_load(char f_name[], int size, short data[]);

	
class CLutFlop
{
public:
	
	CLutFlop();
	~CLutFlop();

public:

	enum
	{
		TABLE_SIZE = 1886886,
		INDEX_SIZE = 2197,
	};


	int* flopindex;
	short* data;		
	std::map<std::string,int> suitoffset;
	std::map<std::pair<int,int>,int> preflop_table;			

public:

	void load(char[]);	
	int g_index(int, int, int[]);
	void read_suitfile(std::string filename);
	std::string replace_suits_by_pattern(int board[]);
};

class CLutTurn
{
public:

	CLutTurn();
	~CLutTurn();

public:

	enum
	{
		TABLE_SIZE = 11522421,
		INDEX_SIZE = 28561,
	};

	int* turnindex;
	short* data;
	std::map<std::string,int> suitoffset;
	std::map<std::pair<int,int>,int> preflop_table;		
	
public:

	void load(char[]);
	int g_index(int, int, int[]);	
	void read_suitfile(std::string filename);
	std::string replace_suits_by_pattern(int board[]);
};

class CLutRiver
{
public:

	CLutRiver();
	~CLutRiver();

public:
	
	enum
	{
		TABLE_SIZE = 37486905,
		INDEX_SIZE = 371293,
	};

	int* riverindex;	
	short* data;
	std::map<std::string,int> suitoffset;
	std::map<std::pair<int,int>,int> preflop_table;

public:
		
	void load(char[]);
	int g_index(int, int, int[]);
	void read_suitfile(std::string filename);
	std::string replace_suits_by_pattern(int board[]);
};

class CLuts
{

public:

	CLuts();
	~CLuts();

public:

	CLutFlop *p_flop;
	CLutTurn *p_turn;
	CLutRiver *p_river;

public:

	int VerifyLut(const int lut_ndx);
	int LoadLuts();
};

