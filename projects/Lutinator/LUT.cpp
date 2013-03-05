#include "LUT.h"

#include <time.h>
#include <windows.h>
#include <tchar.h>
#include <omp.h>

#include <iostream>
#include <string>
#include <sstream>

#include <algorithm>
#include <vector>
#include <map>

using namespace std;

static const bool verbose = false;

inline short round( double x )
{
	return static_cast<short>(x * sd + 0.5);
}

void print_status(time_t start_time, int i)
{
	int size = 1326;
	time_t time_now = time(NULL);

	cout << endl << endl << "Iteration " << i << " of " << size << "   " << i*(100.0/size) << "%" << endl;
	cout << "Time elapsed: " << (time_now - start_time) / 60.0 << " min" << endl;
	cout << "Time remaining: " << (time_now-start_time) * (size-i) / (i*60.0) << " min" << endl;
	cout << "Time per saving: " << (time_now - start_time) / static_cast<double>(i) << endl;
}


int compare_two_hands(int c1_hero, int c2_hero, int c1_opp, int c2_opp, int c3, int c4, int c5, int c6, int c7, int* HR)
{

	int hand_hero = HR[53 + c1_hero];
	hand_hero = HR[hand_hero + c2_hero];
	hand_hero = HR[hand_hero + c3];
	hand_hero = HR[hand_hero + c4];
	hand_hero = HR[hand_hero + c5];
	hand_hero = HR[hand_hero + c6];
	hand_hero = HR[hand_hero + c7];

	int hand_opp = HR[53 + c1_opp];
	hand_opp = HR[hand_opp + c2_opp];
	hand_opp = HR[hand_opp + c3];
	hand_opp = HR[hand_opp + c4];
	hand_opp = HR[hand_opp + c5];
	hand_opp = HR[hand_opp + c6];
	hand_opp = HR[hand_opp + c7];

	if (hand_hero > hand_opp)
		return 1;
	else if (hand_hero == hand_opp)
		return 0;
	else
		return -1;
}


bool is_value_in_array(int value, int * intarray, int size)
{
	for (int i = 0; i < size; i++) 
	{
		if (value == intarray[i])
			return true;
	}

	return false;
}

void generate_deck_without(int *cards, int size, int *new_deck)
{
	int count = 0;

	for (int i = 0; i < 52; i++)
	{
		if (!is_value_in_array(i, cards, size))
		{
			new_deck[count] = i;
			count++;
		}
	}
}


void compute_single_hs(int h1, int h2, int f1, int f2, int f3, int turn, int river, double& hs, int* HR)
{
	int v1, v2;
	int hands = 0;

	int temp;

	int hand[7] = {h1, h2, f1, f2, f3, turn, river};
	int new_deck[45];

	hs = 0;

	generate_deck_without(hand, 7, new_deck);

	// generate opp first hole card
	for (int i = 0; i < 45; i++)
	{
		v1 = new_deck[i];
		// generate opp second hole card
		for (int j = i+1; j < 45; j++)
		{
			v2 = new_deck[j];

			// use the 2+2 hand evaluator:
			temp = compare_two_hands(h1+1, h2+1, v1+1, v2+1, f1+1, f2+1, f3+1, turn+1, river+1, HR);

			if (temp==1)
				hs++;

			// if it is a draw the equity is split in half
			else if (temp == 0)
				hs += 0.5;

			hands++;
		}
	}

	hs = hs / static_cast<double> (hands);
}

void compute_hs_preflop(int h1, int h2, double &hs, int* HR)
{
	int f1, f2, f3, turn, river;
	int i, j, k, l, m;
	
	int hands = 0;
	int handI[2]={h1,h2};
	int new_deck[50];

	double hstemp;
	hs = 0;

	generate_deck_without(handI, 2, new_deck);

	for (i = 0; i < 50; i++) 
	{
		f1 = new_deck[i];

		for (j = i+1; j < 50; j++) 
		{
			f2 = new_deck[j];

			for (k = j+1; k < 50; k++)
			{
				f3 = new_deck[k];

				for (l = k+1; l < 50; l++)
				{
					turn = new_deck[l];

					for (m = l+1; m < 50; m++)
					{
						river = new_deck[m];

						compute_single_hs(h1, h2, f1, f2, f3, turn, river, hstemp, HR);

						hs += hstemp;
						hands++;
					}
				}
			}
		}
	}

	hs = hs / static_cast<double> (hands);
}

void compute_hs_flop(int h1, int h2, int f1, int f2, int f3, double& hs, int* HR) 
{
	int turn, river;
	int hands = 0;
	int handI[5] = {h1, h2, f1, f2, f3};
	int new_deck[47];

	double hstemp = 0;
	hs = 0;

	generate_deck_without(handI, 5, new_deck);

	// generate the new turn card
	for (int i = 0; i < 47; i++) 
	{
		turn = new_deck[i];

		// generate the river card:
		for (int j = i+1; j < 47; j++) 
		{
			river = new_deck[j];

			compute_single_hs(h1, h2, f1, f2, f3, turn, river, hstemp, HR);

			hs += hstemp;
			hands++;
		}
	}

	hs = hs / static_cast<double> (hands);
}

void compute_hs_turn(int h1, int h2, int f1, int f2, int f3, int turn, double& hs, int* HR)
{
	int river;

	int hands = 0;
	int handI[6] = {h1, h2, f1, f2, f3, turn};

	int new_deck[46];

	double hstemp;
	hs = 0;

	// store an int array containing all cards from 0 to 51 except the ones in array handI
	generate_deck_without(handI, 6, new_deck);

	for (int i = 0; i < 46; i++) 
	{
		river = new_deck[i];

		compute_single_hs(h1, h2, f1, f2, f3, turn, river, hstemp, HR);

		hs += hstemp;
		hands++;
	}

	hs = hs / static_cast<double> (hands);
}

// Misc Funcs	
void LUT_save(char f_name[], int size, short data[])
{
	fstream f_bin(f_name, ios::out|ios::binary);
	
	f_bin.seekg(ios::beg);
	f_bin.write((char*)data, size*sizeof(short));
}	

void LUT_load(char f_name[], int size, short data[])
{
	fstream f_bin(f_name, ios::in|ios::binary);
	
	f_bin.seekg(ios::beg);
	f_bin.read((char*)data, size*sizeof(short));
}


/*
VaDe says :
"One thing, which I found is important is to distinguish between LUTs,
where the values only depend on the cards of the current round (e.g HS or similar values) defined as type I
and LUTs, where values also depend on the cards to come (e.g. ppot, nppot, EHS), defined as type II."
*/


/*
////////////////
// Flop
////////////////
(unique combos)

Flop board rank patterns
XXX (13)
XXY (78)
XYY (78)
XYZ (286)

Flop suit patterns
Code:
CCC
CDD
CDH


Hole card combined patterns
- 91 for offsuited hands and suited hands, which don't have the suit of board (e.g.: AhKh : 5s7s9s same as AhKd : 5s8s9s)
- 78 for suited hands, where suit matters (e.g.: AsKs : 5s7s9s different to AhKh : 5s7s9s)

//////////////////////
// Flop: Type I - LUT
//////////////////////
The XY stands here for any suit combination not listed.

rank pattern (possible suit patterns) : (unique hand suit patterns)

XXX (CDH) : (XY)							-> 13*91 = 1.183 entries
XXY (CDC = CDH) : (XY)						-> 78*91 = 7.098 entries
XYY = XXY									-> 78*91 = 7.098 entries
XYZ (CCC | CCD = CDS) : (CC, XY | XY)		-> 286*(78 + 2*91) = 74.360

Total number of enries = 89.739

////////////////////////
// Flop: Type II - LUT
////////////////////////
Code:
XXX (CDH) : (CC, DD, HH, CD, DC, CH, HC, DH, HD, CX, XC, DX, XD, HX, XH, XY)		-> 13*(3*78 + 13*91) = 18.421 entries
XXY (CDC) : (CC, DD, CD, DC, CX, XC, DX, XD, XY)
(CDH) : (CC, HH, CD, DC, CH, HC, CX, XC, HX, XH, XY)								-> 78*(4*78 + 16*91) = 137.904 entries
XYY = XXY																			-> 78*(4*78 + 16*91) = 137.904 entries
XYZ (CCC) : (CC, CX, XC, XY)
(CCD) : (CC, DD, CD, DC, CX, XC, DX, XD, XY)
(CDC | DCC) : (same as above)
(CDH) : (CC, DD, HH, CD, CH, DH, CX, XC, DX, XD, HX, XH, XY)						-> 286*(10*78 + 2*91) = 1.107.964 entries

Total number of enries = 1.402.193

*/

//Clut Flop
CLutFlop::CLutFlop()
{
	data = new short[TABLE_SIZE];

	for (int I = 0; I < TABLE_SIZE; I++) 
		data[I] = -1;

	flopindex = new int[2];
}

CLutFlop::~CLutFlop()
{
	delete [] data;
	delete [] flopindex;
}

string CLutFlop::replace_suits_by_pattern(int board[])
{
	map <int, char> patterns;
	int size = 5;
	int count = 0;
	int suit;
	char temp;
	map <int, char>::iterator it;
	char* pattern_board = new char[size];

	for (int i = 2; i < size; i++)
	{
		suit = board[i] % 4;
		it = patterns.find(suit);

		if (it == patterns.end())
		{
			switch (count) 
			{
				case 0: 
					temp = 'A'; 
					break;
				case 1: 
					temp = 'B'; 
					break;
				case 2: 
					temp = 'C'; 
					break;
			}

			count++;
			patterns.insert(pair<int, char>(suit,temp));
			pattern_board[i]=temp;
		}
		else
		{
			pattern_board[i] = it->second;
		}
	}

	//	// debugging
	//if (board[0] %4 == board[1]%4 && board[1]%4 == board[2] %4 && pattern_board[4] != 'C')
	//	int a = 0;

	// replace the hole cards
	int suit1 = board[0] % 4;
	int suit2 = board[1] % 4;

	it = patterns.find(suit1);

	if (it == patterns.end())
		pattern_board[0] = 'O';
	else
		pattern_board[0] = it->second;

	it = patterns.find(suit2);

	if (it == patterns.end())
		pattern_board[1] = 'O';
	else
		pattern_board[1] = it->second;

	string result(pattern_board,size);
	delete [] pattern_board;

	return result;	
}

int CLutFlop::g_index(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);
	
	std::sort(board, board+3);

	int cards[5] = {c1, c2, board[0], board[1], board[2]};

	string stemp;
	stemp = replace_suits_by_pattern(cards);

	int preflop_maximum = SUIT_IND_COMBOS;

	int preflop_index;
	
	// compute the ranks
	int rc1 = c1 / 4;
	int rc2 = c2 / 4;
	int rf1 = board[0] / 4;
	int rf2 = board[1] / 4;
	int rf3 = board[2] / 4;
	
	// check if the hole cards are suited, if so they cannot be any pair
	if (stemp[0] == stemp[1] && stemp[0] != 'O')
	{
		preflop_index = preflop_table.find(pair<int,int>(rc1,rc2))->second - 13;
		preflop_maximum -= 13;
	}
	else 
	{
		preflop_index = preflop_table.find(pair<int,int>(rc1,rc2))->second;
	}
	
	// compute the index of the flop ranks
	int index = 13*13*rf1 + 13*rf2 + rf3;

	index = flopindex[index];
	// add the index of the hole cards
	index = index * preflop_maximum + preflop_index;
	// add the offset for the suit pattern
	index += suitoffset.find(stemp)->second;

	return index;
}

string CLutFlop::g_pattern(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);
	
	std::sort(board, board+3);

	int cards[5] = {c1, c2, board[0], board[1], board[2]};

	string sboard;
	sboard = replace_suits_by_pattern(cards);

	return sboard;
}

void CLutFlop::write_suitfile()
{
	printf("Flop : Starting write_suitfile()...\n\n");

	int hc1, hc2, flop1, flop2, flop3, board[3];	

	// stp the time for performance measurments:
	time_t start_time = time(NULL);

	int size = 1326;
	int i= 0;

	map<string, int> suitcount;
	map<string, int>::iterator it;
	string index;
	
	// WTF is 22 ???
	for(hc1 = 0; hc1 < 52; hc1++)
	{
		for(hc2 = hc1+1; hc2 < 52; hc2++)
		{	
			i++;
			print_status(start_time, i);

			for(flop1 = 0; flop1 < 52; flop1++)
			{
				for(flop2 = flop1+1; flop2 < 52; flop2++)
				{
					for(flop3 = flop2+1; flop3 < 52; flop3++)
					{
						if (hc1 == flop1 || hc1 == flop2 || hc1 == flop3 || 
							hc2 == flop1 || hc2 == flop2 || hc2 == flop3)
							continue;

						board[0] = flop1;
						board[1] = flop2;
						board[2] = flop3;	
														
						index = g_pattern(hc1, hc2, board);

						if ((it = (suitcount.find(index))) == suitcount.end())
							suitcount.insert(pair<string,int>(index,1));
											
					}
				}
			}
		}
	}

	fstream filestream ("flop_suit.dat", fstream::out);

	size = 0;
	for (int f1 = 0; f1 < 13; f1++)
	{
		for (int f2 = f1; f2 < 13; f2++)
		{
			for(int f3 = f2; f3 < 13; f3++)
				size++;
		}
	}
		
	it = suitcount.begin();
	int offset = 0;
	string stemp;

	while (it != suitcount.end())
	{
		stemp = it->first;
		filestream << stemp << " " << offset <<"\n";
		it++;

		if (stemp[0] == stemp[1] && stemp[0] != 'O')
			offset += size*SUIT_DEP_COMBOS;
		else 
			offset += size*SUIT_IND_COMBOS;
	}

	filestream << "Sum: " << offset;

	filestream.flush();
	filestream.close();

}

void CLutFlop::read_suitfile(string filename)
{
	printf("Flop : Starting read_suitfile()...\n\n");

	delete [] flopindex;
	flopindex = new int[INDEX_SIZE];

	//insert the pairs
	for (int i = 0; i < 13; i++)
		preflop_table.insert(pair<pair<int,int>,int>(pair<int,int>(i,i), i));

	int count = 12;

	for(int i = 0; i < 13; i++)
	{
		for (int j = i+1; j < 13; j++)
		{
			count++;
			preflop_table.insert(pair<pair<int,int>,int>(pair<int,int>(i,j), count));
		}
	}

	// create the index table
	int rank_index = 0;
	int table_index;

	// flop card1
	for (int i = 0; i < 13; i++)
	{
		// flop card2
		for (int j = 0; j < 13; j++)
		{
			// flop card3
			for (int k = 0; k < 13; k++) 
			{
				table_index = 13*13*i + 13*j + k;

				if (i <= j && j <= k)
				{ 
					flopindex[table_index] = rank_index;
					rank_index++;
				}
				else
				{
					flopindex[table_index] = -1;
				}
			}
		}
	}

	fstream filestream(filename.c_str(), fstream::in);
	string temp_entry;
	char temp[256];
	char c[6];
	int size;

	while (filestream.good())
	{
		filestream.getline(temp, 256,'\n');
		stringstream tempstr;
		tempstr << temp;
		tempstr.get(c,6);
		tempstr >> size;
		suitoffset.insert(pair<string,int> (c,size));

	}

	filestream.flush();
	filestream.close();
}

void CLutFlop::generate(char f_name[], int* HR)
{
	printf("Flop : Starting generate()...\n\n");

	int hc1, hc2, flop1, flop2, flop3, index, board[3];

	// stp the time for performance measurments:
	time_t start_time = time(NULL);

	int size = 1326;
	int i = 0;

	for (hc1 = 0; hc1 < 52; hc1++)
	{
		for (hc2 = hc1+1; hc2 < 52; hc2++)
		{
			i++;
			print_status(start_time, i);

			long counter = 0;

			#pragma omp parallel for private(flop2, flop3, board, index)
			for (flop1 = 0; flop1 < 52; flop1++)
			{
				for(flop2 = flop1+1; flop2 < 52; flop2++)
				{
					for(flop3 = flop2+1; flop3 < 52; flop3++)
					{
						if (hc1 == flop1 || hc1 == flop2 || hc1 == flop3 || 
							hc2 == flop1 || hc2 == flop2 || hc2 == flop3)
							continue;

						board[0] = flop1;
						board[1] = flop2;
						board[2] = flop3;	

						index = g_index(hc1, hc2, board);

						if(data[index] == -1)
						{
							double handstrength = 0;

							// go ahead here you need to populate the lut
							compute_hs_flop(hc1, hc2, flop1, flop2, flop3, handstrength, HR);
							data[index] = round(handstrength);

							counter++;

							if (verbose)
								printf("\rFlop : HandStrenth = %5u [%ld]", data[index], counter);
						}
					}
				}
			}
		}
	}

	LUT_save(f_name, TABLE_SIZE, data);
}	

void CLutFlop::load(char f_name[])
{
	cout << "Loading " << f_name << "...";
	LUT_load(f_name, TABLE_SIZE, data);
	cout << "complete" << endl;
}


///////////////////////
// Turn LUT
///////////////////////
/*
Turn board rank patterns

pattern (unique combos)

XXXX (13)
XXXY (78)
XYYY (78)
XXYY (78)
XXYZ (286)
XYYZ (286)
XYZZ (286)
XYZA (715)

Turn suit patterns

Code:
CCCC
CCCD
CCDD
CCDH
CDHS



//////////////////////////
// Turn: Type I - LUT
//////////////////////////

rank pattern (possible suit patterns) : (unique hand suit patterns)

XXXX (CDHS) : (XY)								-> 13*91 = 1.183 entries
XXXY (CDHC = CDHS) : (XY)						-> 78*91 = 7.098 entries
XYYY = XXXY										-> 78*91 = 7.098 entries
XXYY = XXXY										-> 78*91 = 7.098 entries   
XXYZ (CDCC) : (CC, XY)
(rest) : (XY)									-> 286*(78 + 2*91) = 74.360 entries
XYYZ = XXYZ										-> 286*(78 + 2*91) = 74.360 entries
XYZZ = XXYZ										-> 286*(78 + 2*91) = 74.360 entries
XYZA (CCCC) : (CC, CX, XC, XY)
(CCCD) : (CC, XY)
(CCDC) : (CC, XY)
(CDCC) : (CC, XY)
(DCCC) : (CC, XY)
(rest) : (XY)									-> 715*(5*78 + 8*91) = 799.370 entries


Total number of enries = 1.044.927

///////////////////////////
// Turn: Type II - LUT
///////////////////////////

XY stands here for any suit combination not listed.
CX low pocket card has same suit, other suit does not matter.
XC hi pocket card has same suit, other suit does not matter.

rank pattern (possible suit patterns) : (unique hand suit patterns)

XXXX (CDHS) : (XY)								-> 13*91 = 1.183 entries
XXXY (CDHC | CDHS) : (CC, CX, XC, XY | XY)		-> 78*(78+4*91) = 34.476 entries
XYYY = XXXY										-> 78*(78+4*91) = 34.476 entries
XXYY (CDHC | CDHS) : (CC, CX, XC, XY | XY)
(CDCD) : (CC, CD, DC, CX, XC, XY)				-> 78*(2*78+8*91) = 68.952 entries

XXYZ (CDCC) : (CC, CX, XC, XY)
(CDCD) : (CC, DD, CD, DC, CX, XC, DX, XD, XY)
(CDDC) : (CC, DD, CD, DC, CX, XC, DX, XD, XY)
(CDCH) : (CC, CX, XC, XY)
(CDHC) : (CC, CX, XC, XY)
(DHCC) : (CC, CX, XC, XY)   
(CDHS) : (XY)									-> 286*(8*78 + 27*91) = 881.166 entries
XYYZ = XXYZ										-> 286*(8*78 + 27*91) = 881.166 entries
XYZZ = XXYZ										-> 286*(8*78 + 27*91) = 881.166 entries
XYZA (CCCC) : (CC, CX, XC, XY)
(CCCD) : (CC, CX, XC, XY)
(CCDC) : (CC, CX, XC, XY)
(CDCC) : (CC, CX, XC, XY)
(DCCC) : (CC, CX, XC, XY)
(CCDD) : (CC, DD, CD, DC, CX, XC, DX, XD, XY)
(CDCD) : (CC, DD, CD, DC, CX, XC, DX, XD, XY)
(CDDC) : (CC, DD, CD, DC, CX, XC, DX, XD, XY)
(CCDH) : (CC, CX, XC, XY)
(CDCH) : (CC, CX, XC, XY)
(CDHC) : (CC, CX, XC, XY)
(DCCH) : (CC, CX, XC, XY)
(DCHC) : (CC, CX, XC, XY)
(DHCC) : (CC, CX, XC, XY)
(CDHS) : (XY)									-> 715*(17*78 + 55*91) = 4.526.665 entries


Total number of enries = 7.309.250
*/

CLutTurn::CLutTurn()
{
	data = new short[TABLE_SIZE];

	for(int I=0;I<TABLE_SIZE;I++) 
		data[I] = -1;

	turnindex = new int[2];
}

CLutTurn::~CLutTurn()
{		
	delete [] data;	
	delete [] turnindex;
}

string CLutTurn::replace_suits_by_pattern(int board[])
{
	map<int, char> patterns;
	int size = 6;
	int count = 0, A=0, B=0, C=0;
	int suit;
	char temp;
	map<int, char>::iterator it;
	char* pattern_board = new char[size];

	for (int i = 2; i < size; i++)
	{
		suit = board[i] % 4;
		it = patterns.find(suit);

		if (it == patterns.end())
		{
			switch (count) 
			{
				case 0: 
					temp = 'A';
					A++; 
					break;
				case 1: 
					temp = 'B';
					B++; 
					break;
				case 2: 
					temp = 'C';
					C++; 
					break;
				// the last found suit can never be part of a FD
				case 3: 
					temp = 'O'; 
					break;
			}

			count++;
			// if there will be an 'O' we do not need to add it to the map

			if (count != 4)
				patterns.insert(pair<int, char>(suit,temp));

			pattern_board[i]=temp;
		}
		else
		{
			pattern_board[i] = it->second;

			if (pattern_board[i] == 'A')
				A++;
			else if (pattern_board[i] == 'B')
				B++;
			else if (pattern_board[i] == 'C')
				C++;
		}
	}

	// replace the hole cards
	int suit1 = board[0] % 4;
	int suit2 = board[1] % 4;

	it = patterns.find(suit1);

	if (it == patterns.end())
	{
		pattern_board[0] = 'O';
	}
	else
	{
		// if the suit appears only once on the turn board the suit does not matter, because it cannot participate in a FD:
		pattern_board[0] = it->second;

		if (pattern_board[0] == 'A' && A == 1)
			pattern_board[0] = 'O';
		else if (pattern_board[0] == 'B' && B == 1)
			pattern_board[0] = 'O';
		else if (pattern_board[0] == 'C' && C == 1)
			pattern_board[0] = 'O';
	}

	it = patterns.find(suit2);

	if (it == patterns.end())
	{
		pattern_board[1] = 'O';
	}
	else
	{
		pattern_board[1] = it->second;

		if (pattern_board[1] == 'A' && A == 1)
			pattern_board[1] = 'O';
		else if (pattern_board[1] == 'B' && B == 1)
			pattern_board[1] = 'O';
		else if (pattern_board[1] == 'C' && C == 1)
			pattern_board[1] = 'O';
	}

	//if the number of patterns is 1 they do not play any role in drawing a flush, hence they can be ignored
	// we do not need to check the last entry, because if it is a single appearence it is already an 'O'
	int reduced_size=size-1;

	for (int i = 2; i < reduced_size; i++)
	{
		if (A == 1 && pattern_board[i] == 'A')
			pattern_board[i] = 'O';
		else if (B == 1 && pattern_board[i] == 'B')
			pattern_board[i] = 'O';
		else if (C == 1 && pattern_board[i] == 'C')
			pattern_board[i] = 'O';
	}

	string result(pattern_board,size);
	delete [] pattern_board;

	return result;
}

string CLutTurn::g_pattern(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);

	std::sort(board, board+4);

	int cards[6] = {c1, c2, board[0], board[1], board[2], board[3]};

	string sboard;
	sboard = replace_suits_by_pattern(cards);

	return sboard;
}


int CLutTurn::g_index(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);

	std::sort(board, board+4);

	int cards[6] = {c1, c2, board[0], board[1], board[2], board[3]};

	string stemp;
	stemp = replace_suits_by_pattern(cards);

	int maximum = 13;
	int preflop_maximum = SUIT_IND_COMBOS;
	int preflop_index;

	// compute the ranks
	int rc1 = c1 / 4;
	int rc2 = c2 / 4;
	int rf1 = board[0] / 4;
	int rf2 = board[1] / 4;
	int rf3 = board[2] / 4;
	int rt = board[3]/ 4;

	// check if the hole cards are suited, if so they cannot be any pair
	if (stemp[0] == stemp[1] && stemp[0] != 'O')
	{
		preflop_index = preflop_table.find(pair<int,int>(rc1,rc2))->second - 13;
		preflop_maximum -= 13;
	}
	else 
	{
		preflop_index = preflop_table.find(pair<int,int>(rc1,rc2))->second;
	}

	// compute the index of the flop ranks
	int index = 13*13*13*rf1 + 13*13*rf2 + 13*rf3 + rt;

	index = turnindex[index];
	// add the index of the hole cards
	index = index * preflop_maximum + preflop_index;
	// add the offset for the suit pattern
	index += suitoffset.find(stemp)->second;

	return index;
}

void CLutTurn::read_suitfile(string filename)
{
	printf("Turn : Starting read_suitfile()...\n\n");

	delete [] turnindex;
	turnindex = new int[INDEX_SIZE];

	//insert the pairs
	for (int i = 0; i < 13; i++)
		preflop_table.insert(pair<pair<int,int>,int>(pair<int,int>(i,i),i));

	int count = 12;

	for (int i = 0; i < 13; i++)
	{
		for (int j = i+1; j < 13; j++)
		{
			count++;
			preflop_table.insert(pair<pair<int,int>,int>(pair<int,int>(i,j),count));
		}
	}

	// create the index table
	int rank_index = 0;
	int table_index;

	// flop card1
	for (int i = 0; i < 13; i++)
	{
		// flop card2
		for (int j = 0; j < 13; j++)
		{
			// flop card3
			for (int k = 0; k < 13; k++) 
			{
				// turn card
				for (int l = 0; l < 13; l++)
				{
					table_index = 13*13*13*i + 13*13*j + 13*k + l;

					if (i <= j && j <= k && k <= l)
					{ 
						turnindex[table_index] = rank_index;
						rank_index++;
					}
					else
					{
						turnindex[table_index]=-1;
					}
				}
			}
		}
	}

	fstream filestream(filename.c_str(), fstream::in);
	string temp_entry;
	char temp[256];
	char c[7];
	int size;

	while (filestream.good())
	{
		filestream.getline(temp, 256,'\n');
		stringstream tempstr;
		tempstr << temp;
		tempstr.get(c,7);
		tempstr >> size;
		suitoffset.insert(pair<string,int>(c,size));
	}

	filestream.flush();
	filestream.close();
}

void CLutTurn::write_suitfile()
{
	printf("Turn : Starting write_suitfile()...\n\n");

	int hc1, hc2, flop1, flop2, flop3, turn, board[4];	

	// stp the time for performance measurments:
	time_t start_time = time(NULL);

	int size = 1326;
	int i= 0;

	map<string, int> suitcount;
	map<string, int>::iterator it;
	string index;
	
	// WTF is 36 ???
	for (hc1 = 0; hc1 < 52; hc1++)
	{
		for (hc2 = hc1+1; hc2 < 52; hc2++)
		{
			i++;
			print_status(start_time, i);

			for (flop1 = 0; flop1 < 52; flop1++)
			{							
				for (flop2 = flop1+1; flop2 < 52; flop2++)
				{
					for (flop3 = flop2+1; flop3 < 52; flop3++)
					{
						for (turn = flop3+1; turn < 52; turn++)
						{
							if (hc1 == flop1 || hc1 == flop2 || hc1 == flop3 || hc1 == turn || 
								hc2 == flop1 || hc2 == flop2 || hc2 == flop3 || hc2 == turn)
								continue;

							board[0] = flop1;
							board[1] = flop2;
							board[2] = flop3;
							board[3] = turn;
													
							index = g_pattern(hc1, hc2, board);

							if ((it = (suitcount.find(index))) == suitcount.end())
								suitcount.insert(pair<string,int>(index,1));
						}
					}
				}
			}		
		}
	}

	fstream filestream ("turn_suit.dat", fstream::out);

	size = 0;
	// flop card1
	for (int f1 = 0; f1 < 13; f1++)
	{
		// flop card2
		for (int f2 = f1;  f2 < 13; f2++)
		{
			// flop card3
			for(int f3 = f2; f3 < 13; f3++)
			{
				// turn card
				for (int t = f3; t < 13; t++)
				{
					size++;
				}
			}
		}
	}
		
	it = suitcount.begin();
	int offset = 0;
	string stemp;

	while (it != suitcount.end())
	{
		stemp = it->first;
		filestream << stemp << " " << offset <<"\n";
		it++;

		if (stemp[0] == stemp[1] && stemp[0] != 'O')
			offset += size*SUIT_DEP_COMBOS;
		else 
			offset += size*SUIT_IND_COMBOS;
	}

	filestream << "Sum: " << offset;

	filestream.flush();
	filestream.close();
}

void CLutTurn::generate(char f_name[], int* HR)
{
	printf("Turn : Starting generate()...\n\n");

	int hc1, hc2, flop1, flop2, flop3, turn, index, board[4];	

	// stp the time for performance measurments:
	time_t start_time = time(NULL);

	int size = 1326;
	int i= 0;

	// WTF is 36 ???
	for (hc1 = 0; hc1 < 52; hc1++)
	{
		for (hc2 = hc1+1; hc2 < 52; hc2++)
		{
			i++;
			print_status(start_time, i);
			long counter = 0;

			#pragma omp parallel for private(flop2, flop3, turn, board, index)
			for (flop1 = 0; flop1 < 52; flop1++)
			{							
				for (flop2 = flop1+1; flop2 < 52; flop2++)
				{
					for (flop3 = flop2+1; flop3 < 52; flop3++)
					{
						for (turn = flop3+1; turn < 52; turn++)
						{
							if (hc1 == flop1 || hc1 == flop2 || hc1 == flop3 || hc1 == turn || 
								hc2 == flop1 || hc2 == flop2 || hc2 == flop3 || hc2 == turn)
								continue;

							board[0] = flop1;
							board[1] = flop2;
							board[2] = flop3;
							board[3] = turn;

							index = g_index(hc1, hc2, board);

							if(data[index] == -1)
							{																					
								double handstrength = 0;

								// go ahead here you need to populate the lut
								compute_hs_turn(hc1, hc2, flop1, flop2, flop3, turn, handstrength, HR);
								data[index] = round(handstrength);

								counter++;

								if (verbose)
									printf("\rTurn : HandStrenth = %5u [%ld]", data[index], counter);

							}
						}
					}
				}
			}
		}
	}

	LUT_save(f_name, TABLE_SIZE, data);
}

void CLutTurn::load(char f_name[])
{
	cout << "Loading " << f_name << "...";
	LUT_load(f_name, TABLE_SIZE, data);
	cout << "complete" << endl;
}

////////////////////////
// RIVER
////////////////////////

/*
River board rank patterns

Code:
pattern (unique combos)

XXXXY (78)
XXXYY (78)
XXXYZ (286)
XYYYZ (286)
XYZZZ (286)
XXYYZ (286)
XXYZZ (286)
XYYZZ (286)
XXYZA (715)
XYYZA (715)
XYZZA (715)
XYZAA (715)
XYZAB (1287)


River suit patterns

Code:
CCCCC
CCCCD
CCCDH = CCCDD
rest

////////////////////////////
// River: Type I - LUT
////////////////////////////
XY stands here for any suit combination not listed.
CX low pocket card has same suit, other suit does not matter.
XC hi pocket card has same suit, other suit does not matter.

Code:
rank pattern (possible suit patterns) : (unique hand suit patterns)

XXXXY (any) : (XY)										-> 78*91 = 7.098 entries
YXXXX = XXXXY											-> 78*91 = 7.098 entries
XXXYY = XXXXY											-> 78*91 = 7.098 entries
XXYYY = XXXXY											-> 78*91 = 7.098 entries
XXXYZ (CDHCC | other) : (CC, CX, XC, XY | XY)			-> 286*(78+4*91) = 126.412 entries
XYYYZ = XXXYZ											-> 286*(78+4*91) = 126.412 entries
XXXYZ = XXXYZ											-> 286*(78+4*91) = 126.412 entries
XYZZZ = XXXYZ											-> 286*(78+4*91) = 126.412 entries
XXYZZ = XXXYZ											-> 286*(78+4*91) = 126.412 entries
XYYZZ = XXXYZ											-> 286*(78+4*91) = 126.412 entries
XXYZA (CDCCC) : (CC, CX, XC, XY)               
(CDCCH) : (CC, CX, XC, XY)               
(CDCHC) : (CC, CX, XC, XY)   
(CDHCC) : (CC, CX, XC, XY)
(DHCCC) : (CC, CX, XC, XY)
(other) : (XY)											-> 715*(5*78+16*91) = 1.319.890 entries
XXYZA = XYYZA											-> 715*(5*78+16*91) = 1.319.890 entries
XXYZA = XYZZA											-> 715*(5*78+16*91) = 1.319.890 entries
XXYZA = XYZAA											-> 715*(5*78+16*91) = 1.319.890 entries
XYZAB (CCCCC) : (CC, CX, XC, XY)
(CCCCD) : (CC, CX, XC, XY)
(CCCDC) : (CC, CX, XC, XY)
(CCDCC) : (CC, CX, XC, XY)
(CDCCC) : (CC, CX, XC, XY)
(DCCCC) : (CC, CX, XC, XY)
(CCCDH) : (CC, CX, XC, XY)
(CCDCH) : (CC, CX, XC, XY)
(CCDHC) : (CC, CX, XC, XY)
(CDCCH) : (CC, CX, XC, XY)
(CDCHC) : (CC, CX, XC, XY)
(CDHCC) : (CC, CX, XC, XY)
(DCCCH) : (CC, CX, XC, XY)
(DCCHC) : (CC, CX, XC, XY)
(DCHCC) : (CC, CX, XC, XY)
(DHCCC) : (CC, CX, XC, XY)
(other) : (XY)											-> 1287*(16*78+49*91) = 7.344.909 entries


Total number of enries = 13.411.333
*/

CLutRiver::CLutRiver()
{	
	data = new short[TABLE_SIZE];

	for(int I=0;I<TABLE_SIZE;I++) 
		data[I] = -1;

	riverindex = new int[2];
}

CLutRiver::~CLutRiver()
{
	delete [] data;
	delete [] riverindex;
}

string CLutRiver::replace_suits_by_pattern(int board[])
{
	map<int, char> patterns;
	int size = 7;
	int count = 0, A=0, B=0, C=0;
	int suit;
	char temp;
	map<int, char>::iterator it;
	char* pattern_board = new char[size];

	for (int i = 2; i < size; i++)
	{
		suit = board[i] % 4;
		it = patterns.find(suit);

		if (it == patterns.end())
		{
			switch (count) 
			{
				case 0: 
					temp = 'A';
					A++; 
					break;
				case 1: 
					temp = 'B';
					B++; 
					break;
				case 2: 
					temp = 'C';
					C++; 
					break;
				// the last found suit can never be part of a Flush
				case 3: 
					temp = 'O'; 
					break;
				case 4: 
					temp = 'O';
					break;
			}

			count++;

			// if there will be an 'O' we do not need to add it to the map
			if (temp != 'O')
				patterns.insert(pair<int, char>(suit,temp));

			pattern_board[i]=temp;
		}
		else
		{
			pattern_board[i] = it->second;

			if (pattern_board[i] == 'A')
				A++;
			else if (pattern_board[i] == 'B')
				B++;
			else if (pattern_board[i] == 'C')
				C++;
		}
	}

	// replace the hole cards
	int suit1 = board[0] % 4;
	int suit2 = board[1] % 4;
	it = patterns.find(suit1);

	if (it == patterns.end())
	{
		pattern_board[0] = 'O';
	}
	else
	{
		// if the suit is only once on the turn board the suit does not matter, because it cannot participate in a FD:
		pattern_board[0] = it->second;

		if (pattern_board[0] == 'A' && A < 3)
			pattern_board[0] = 'O';
		else if (pattern_board[0] == 'B' && B < 3)
			pattern_board[0] = 'O';
		else if (pattern_board[0] == 'C' && C < 3)
			pattern_board[0] = 'O';
	}

	it = patterns.find(suit2);

	if (it == patterns.end())
	{
		pattern_board[1] = 'O';
	}
	else
	{
		pattern_board[1] = it->second;

		if (pattern_board[1] == 'A' && A < 3)
			pattern_board[1] = 'O';
		else if (pattern_board[1] == 'B' && B < 3)
			pattern_board[1] = 'O';
		else if (pattern_board[1] == 'C' && C < 3)
			pattern_board[1] = 'O';
	}

	//if the number of patterns is 1 or 2 they do not play any role for a flush, hence they can be ignored
	for (int i = 2; i < size; i++)
	{
		if (A < 3 && pattern_board[i] == 'A')
			pattern_board[i] = 'O';
		else if (B < 3 && pattern_board[i] == 'B')
			pattern_board[i] = 'O';
		else if (C < 3 && pattern_board[i] == 'C')
			pattern_board[i] = 'O';
	}

	string result(pattern_board,size);
	delete [] pattern_board;

	return result;
}

string CLutRiver::g_pattern(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);

	std::sort(board, board+5);

	int cards[7] = {c1, c2, board[0], board[1], board[2], board[3], board[4]};

	string sboard;
	sboard = replace_suits_by_pattern(cards);

	return sboard;
}

int CLutRiver::g_index(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);

	std::sort(board, board+5);

	int cards[7] = {c1, c2, board[0], board[1], board[2], board[3], board[4]};

	string stemp;
	stemp = replace_suits_by_pattern(cards);

	int preflop_maximum = SUIT_IND_COMBOS;
	int preflop_index;

	// compute the ranks
	int rc1 = c1 / 4;
	int rc2 = c2 / 4;
	int rf1 = board[0] / 4;
	int rf2 = board[1] / 4;
	int rf3 = board[2] / 4;
	int rt = board[3] / 4;
	int rr = board[4] / 4;

	// check if the hole cards are suited, if so they cannot be any pair
	if (stemp[0] == stemp[1] && stemp[0] != 'O')
	{
		preflop_index = preflop_table.find(pair<int,int>(rc1,rc2))->second - 13;
		preflop_maximum -= 13;
	}
	else 
	{
		preflop_index = preflop_table.find(pair<int,int>(rc1,rc2))->second;
	}

	// compute the index of the flop ranks
	int index = 13*13*13*13*rf1 + 13*13*13*rf2 + 13*13*rf3 + 13*rt + rr;

	index = riverindex[index];
	// add the index of the hole cards
	index = index * preflop_maximum + preflop_index;
	// add the offset for the suit pattern
	index += suitoffset.find(stemp)->second;

	return index;
}

void CLutRiver::write_suitfile()
{
	printf("River : Starting write_suitfile()...\n\n");

	int hc1, hc2, flop1, flop2, flop3, turn, river, board[5];	

	// stp the time for performance measurments:
	time_t start_time = time(NULL);

	int size = 1326;
	int i= 0;

	map<string, int> suitcount;
	map<string, int>::iterator it;
	string index;
	
	for(hc1 = 0; hc1 < 52; hc1++)
	{
		for(hc2 = hc1+1; hc2 < 52; hc2++)
		{
			i++;
			print_status(start_time, i);

			for (flop1 = 0; flop1 < 52; flop1++)
			{							
				for (flop2 = flop1+1; flop2 < 52; flop2++)
				{
					for (flop3 = flop2+1; flop3 < 52; flop3++)
					{
						for (turn = flop3+1; turn < 52; turn++)
						{
							for (river = turn+1; river < 52; river++)
							{
								if (hc1 == flop1 || hc1 == flop2 || hc1 == flop3 || hc1 == turn || hc1 == river ||
									hc2 == flop1 || hc2 == flop2 || hc2 == flop3 || hc2 == turn || hc2 == river)
									continue;

									board[0] = flop1;
									board[1] = flop2;
									board[2] = flop3;
									board[3] = turn;		
									board[4] = river;
							
									index = g_pattern(hc1, hc2, board);

									if ((it = (suitcount.find(index))) == suitcount.end())
										suitcount.insert(pair<string,int>(index,1));
							}
						}
					}
				}
			}
		}
	}

	fstream filestream ("river_suit.dat", fstream::out);

	size = 0;
	for (int f1 = 0; f1 < 13; f1++)
	{
		for (int f2 = f1;  f2 < 13; f2++)
		{
			for(int f3 = f2; f3 < 13; f3++)
			{
				for (int t= f3; t < 13; t++)
				{
					for (int r = t; r <13; r++)
						size++;
				}
			}
		}
	}
		
	it = suitcount.begin();
	int offset = 0;
	string stemp;

	while (it != suitcount.end())
	{
		stemp = it->first;
		filestream << stemp << " " << offset <<"\n";
		it++;

		if (stemp[0] == stemp[1] && stemp[0] != 'O')
			offset += size*SUIT_DEP_COMBOS;
		else 
			offset += size*SUIT_IND_COMBOS;
	}

	filestream << "Sum: " << offset;

	filestream.flush();
	filestream.close();

}

void CLutRiver::read_suitfile(string filename)
{
	printf("\nRiver : Starting read_suitfile()...\n\n");

	delete [] riverindex;
	riverindex = new int[INDEX_SIZE];

	//insert the pairs
	for (int i = 0; i < 13; i++)
		preflop_table.insert(pair<pair<int,int>,int>(pair<int,int>(i,i),i));

	int count = 12;
	for(int i = 0; i < 13; i++)
	{
		for (int j = i+1; j < 13; j++)
		{
			count++;
			preflop_table.insert(pair<pair<int,int>,int>(pair<int,int>(i,j),count));
		}
	}

	// create the index table
	int rank_index = 0;
	int table_index;

	// flop card1
	for (int i = 0; i < 13; i++)
	{
		// flop card2
		for (int j = 0; j < 13; j++)
		{
			// flop card3
			for (int k = 0; k < 13; k++) 
			{
				// turn card
				for (int l = 0; l < 13; l++)
				{
					// river card
					for(int m = 0; m < 13; m++)
					{
						table_index = 13*13*13*13*i + 13*13*13*j + 13*13*k + 13*l + m;

						if (i <= j && j <= k && k <= l && l <= m)
						{ 
							riverindex[table_index] = rank_index;
							rank_index++;
						}
						else
						{
							riverindex[table_index]=-1;
						}
					}
				}
			}
		}
	}

	fstream filestream(filename.c_str(), fstream::in);
	string temp_entry;
	char temp[256];
	char c[8];
	int size;

	while (filestream.good())
	{
		filestream.getline(temp, 256,'\n');
		stringstream tempstr;
		tempstr << temp;
		tempstr.get(c,8);
		tempstr >> size;
		suitoffset.insert(pair<string,int>(c,size));
	}

	filestream.flush();
	filestream.close();
}

void CLutRiver::generate(char f_name[], int* HR)
{	
	printf("River : Starting generate()...\n\n");

	int hc1, hc2, flop1, flop2, flop3, turn, river, index, board[5];	

	// stp the time for performance measurments:
	time_t start_time = time(NULL);

	int size = 1326;
	int i= 0;

	for(hc1 = 0; hc1 < 52; hc1++)
	{
		for(hc2 = hc1+1; hc2 < 52; hc2++)
		{
			i++;
			print_status(start_time, i);
			long counter = 0;

			#pragma omp parallel for private(flop2, flop3, turn, river, board, index)
			for (flop1 = 0; flop1 < 52; flop1++)
			{							
				for (flop2 = flop1+1; flop2 < 52; flop2++)
				{
					for (flop3 = flop2+1; flop3 < 52; flop3++)
					{
						for (turn = flop3+1; turn < 52; turn++)
						{
							for (river = turn+1; river < 52; river++)
							{
								if (hc1 == flop1 || hc1 == flop2 || hc1 == flop3 || hc1 == turn || hc1 == river ||
									hc2 == flop1 || hc2 == flop2 || hc2 == flop3 || hc2 == turn || hc2 == river)
									continue;

								board[0] = flop1;
								board[1] = flop2;
								board[2] = flop3;
								board[3] = turn;		
								board[4] = river;

								index = g_index(hc1, hc2, board);

								if(data[index] == -1)
								{																		
									double handstrength = 0;

									// go ahead here you need to populate the lut
									compute_single_hs(hc1, hc2, flop1, flop2, flop3, turn, river, handstrength, HR);
									data[index] = round(handstrength);

									counter++;

									if (verbose)
										printf("\rRiver : HandStrenth = %5u [%ld]", data[index], counter);
								}															
							}
						}
					}	
				}
			}
		}
	}

	LUT_save(f_name, TABLE_SIZE, data);
}

void CLutRiver::load(char f_name[])
{
	cout << "Loading " << f_name << "...";
	LUT_load(f_name, TABLE_SIZE, data);
	cout << "complete" << endl;
}




//CLut Preflop
CLutPreFlop::CLutPreFlop()
{
	data = new short[TABLE_SIZE];

	for (int i = 0; i < TABLE_SIZE; i++)
		data[i] = -1;
}

CLutPreFlop::~CLutPreFlop()
{
	delete [] data;
}

void CLutPreFlop::generate(char f_name[])
{
	int index;

	// stp the time for performance measurments:
	time_t start_time = time(NULL);

	int size = 1326, s = 0, i, j;

	for (i = 0; i < 52; i ++)
	{
		for (j = i+1; j < 52; j++) 
		{	
			s++;
			print_status(start_time, s);

			index = g_index(i, j);
			if (data[index] == -1)
			{
				// go ahead here you need to populate the lut
				//data[index] = ???

			}
		}
	}

	LUT_save(f_name, TABLE_SIZE, data);
}

int CLutPreFlop::g_index(int c1, int c2)
{

	if (c1 > c2)
	{
		std::swap(c1,c2);
	}

	int suit1 = c1 % 4;
	int suit2 = c2 % 4;
	int rank1 = c1 / 4;
	int rank2 = c2 / 4;

	if (suit2 == suit1)
		return preflop_table.find(pair <int,int> (rank1,rank2))->second;
	else
		return preflop_table.find(pair <int,int> (rank1+100,rank2+100))->second;
}

void CLutPreFlop::prepare()
{
	int count = 0;

	for (int i = 0; i < 13; i++)
	{
		for (int j = i+1; j < 13; j++)
		{
			preflop_table.insert(pair <pair <int,int>, int> (pair <int,int> (i,j), count));
			count++;
		}
	}

	for (int i = 100; i < 113; i++)
	{
		for (int j = i; j < 113; j++)
		{
			preflop_table.insert(pair <pair <int,int>, int> (pair <int,int>(i,j), count));
			count++;
		}
	}

}

void CLutPreFlop::load(char f_name[])
{
	cout << "Loading " << f_name << "...";
	LUT_load(f_name, TABLE_SIZE, data);
	cout << "complete" << endl;
}
