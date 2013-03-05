#pragma once
	
#include <fstream>
#include <map>
#include <string>

	
using namespace std;


static int CLUBS = 0;
static int HEARTS = 1;
static int DIAMONDS = 2;
static int SPADES = 3;

// 91 for offsuited hands and suited hands, which don't have the suit of board (e.g.: AhKh : 5s7s9s same as AhKd : 5s8s9s)
static int SUIT_IND_COMBOS = 91;

// 78 for suited hands, where suit matters (e.g.: AsKs : 5s7s9s different to AhKh : 5s7s9s)
static int SUIT_DEP_COMBOS = 78;

const double sd = 10000; 

inline short round( double x );
void print_status(time_t start_time, int i);

int compare_two_hands(int c1_hero, int c2_hero, int c1_opp, int c2_opp, int c3, int c4, int c5, int c6, int c7, int* HR);
bool is_value_in_array(int value, int * intarray, int size);
void generate_deck_without(int *cards, int size, int *new_deck);

void compute_single_hs(int h1, int h2, int f1, int f2, int f3, int t, int r, double& hs, int* HR);
void compute_hs_preflop(int h1, int h2, double& hs, int* HR);
void compute_hs_flop(int h1, int h2, int f1, int f2, int f3, double& hs, int* HR);
void compute_hs_turn(int h1, int h2, int f1, int f2, int f3, int t, double& hs, int* HR);

void LUT_save(char f_name[], int size, short data[]);
void LUT_load(char f_name[], int size, short data[]);

class CLutPreFlop
{
	public:
	
		enum
		{
			TABLE_SIZE = 169,
		};

		short *data;

		map <pair<int,int>, int> preflop_table;

		
		CLutPreFlop();
		~CLutPreFlop();			
		
		void load(char[]);
		void generate(char[]);		
		int g_index(int, int);
		void prepare();
};
	
class CLutFlop
{
	public:
	
		enum
		{
			TABLE_SIZE = 1886886,
			INDEX_SIZE = 2197,
		};

		short *data;

		int *flopindex;
		
		map<string, int> suitoffset;
		map<pair<int,int>, int> preflop_table;
		
		CLutFlop();
		~CLutFlop();			
		
		void load(char[]);
		void generate(char[], int* HR);		
		int g_index(int, int, int[]);
		void write_suitfile();
		void read_suitfile(string filename);
		string g_pattern(int c1, int c2, int board[]);
		string replace_suits_by_pattern(int board[]);
};

class CLutTurn
{
	public:

		enum
		{
			TABLE_SIZE = 11522421,
			INDEX_SIZE = 28561,
		};

		int *turnindex;
		
		map<string, int> suitoffset;
		map<pair<int,int>, int> preflop_table;
		
		short *data;
		
		CLutTurn();
		~CLutTurn();			
		
		void load(char[]);
		void generate(char[], int* HR);
		int g_index(int, int, int[]);
		
		void write_suitfile();
		void read_suitfile(string filename);
		string g_pattern(int c1, int c2, int board[]);
		string replace_suits_by_pattern(int board[]);
};

class CLutRiver
{
	public:
	
		enum
		{
			TABLE_SIZE = 37486905,
			INDEX_SIZE = 371293,
		};

		int *riverindex;
		
		map<string, int> suitoffset;
		map<pair<int,int>, int> preflop_table;

		short *data;
		
		CLutRiver();
		~CLutRiver();			
		
		void load(char[]);
		void generate(char[], int* HR);
		int g_index(int, int, int[]);

		void write_suitfile();
		void read_suitfile(string filename);
		string g_pattern(int c1, int c2, int board[]);
		string replace_suits_by_pattern(int board[]);
};

