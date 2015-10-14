#include "stdafx.h"

#include <sstream>
#include <algorithm>

#include "Util_LUT.h"
#include "Util_logging.h"



// Misc Funcs
void LUT_load(char f_name[], int size, short data[])
{
	std::fstream f_bin(f_name, std::ios::in|std::ios::binary);
	
	f_bin.seekg(std::ios::beg);
	f_bin.read((char*)data, size*sizeof(short));
}


////////////////
// Flop
////////////////
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

std::string CLutFlop::replace_suits_by_pattern(int board[])
{
	std::map <int, char> patterns;
	int size = 5;
	int count = 0;
	int suit;
	char temp;
	std::map <int, char>::iterator it;
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
			patterns.insert(std::pair<int, char>(suit,temp));
			pattern_board[i]=temp;
		}
		else
		{
			pattern_board[i] = it->second;
		}
	}

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

	std::string result(pattern_board,size);
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

	std::string stemp;
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
		preflop_index = preflop_table.find(std::pair<int,int>(rc1,rc2))->second - 13;
		preflop_maximum -= 13;
	}
	else 
	{
		preflop_index = preflop_table.find(std::pair<int,int>(rc1,rc2))->second;
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

void CLutFlop::read_suitfile(std::string filename)
{
	delete [] flopindex;
	flopindex = new int[INDEX_SIZE];

	//insert the pairs
	for (int i = 0; i < 13; i++)
		preflop_table.insert(std::pair<std::pair<int,int>,int>(std::pair<int,int>(i,i), i));

	int count = 12;

	for(int i = 0; i < 13; i++)
	{
		for (int j = i+1; j < 13; j++)
		{
			count++;
			preflop_table.insert(std::pair<std::pair<int,int>,int>(std::pair<int,int>(i,j), count));
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

	std::fstream filestream(filename.c_str(), std::fstream::in);
	std::string temp_entry;
	char temp[256];
	char c[6];
	int size;

	while (filestream.good())
	{
		filestream.getline(temp, 256,'\n');
		std::stringstream tempstr;
		tempstr << temp;
		tempstr.get(c,6);
		tempstr >> size;
		suitoffset.insert(std::pair<std::string,int> (c,size));
	}

	filestream.flush();
	filestream.close();
}
	

void CLutFlop::load(char f_name[])
{
	LUT_load(f_name, TABLE_SIZE, data);
}


///////////////////////
// Turn LUT
///////////////////////
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

std::string CLutTurn::replace_suits_by_pattern(int board[])
{
	std::map<int, char> patterns;
	int size = 6;
	int count = 0, A=0, B=0, C=0;
	int suit;
	char temp;
	std::map<int, char>::iterator it;
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
				patterns.insert(std::pair<int, char>(suit,temp));

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

	std::string result(pattern_board,size);
	delete [] pattern_board;

	return result;
}


int CLutTurn::g_index(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);

	std::sort(board, board+4);

	int cards[6] = {c1, c2, board[0], board[1], board[2], board[3]};

	std::string stemp;
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
		preflop_index = preflop_table.find(std::pair<int,int>(rc1,rc2))->second - 13;
		preflop_maximum -= 13;
	}
	else 
	{
		preflop_index = preflop_table.find(std::pair<int,int>(rc1,rc2))->second;
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

void CLutTurn::read_suitfile(std::string filename)
{
	delete [] turnindex;
	turnindex = new int[INDEX_SIZE];

	//insert the pairs
	for (int i = 0; i < 13; i++)
		preflop_table.insert(std::pair<std::pair<int,int>,int>(std::pair<int,int>(i,i),i));

	int count = 12;

	for (int i = 0; i < 13; i++)
	{
		for (int j = i+1; j < 13; j++)
		{
			count++;
			preflop_table.insert(std::pair<std::pair<int,int>,int>(std::pair<int,int>(i,j),count));
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

	std::fstream filestream(filename.c_str(), std::fstream::in);
	std::string temp_entry;
	char temp[256];
	char c[7];
	int size;

	while (filestream.good())
	{
		filestream.getline(temp, 256,'\n');
		std::stringstream tempstr;
		tempstr << temp;
		tempstr.get(c,7);
		tempstr >> size;
		suitoffset.insert(std::pair<std::string,int>(c,size));
	}

	filestream.flush();
	filestream.close();
}

void CLutTurn::load(char f_name[])
{
	LUT_load(f_name, TABLE_SIZE, data);
}



////////////////////////
// RIVER
////////////////////////
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

std::string CLutRiver::replace_suits_by_pattern(int board[])
{
	std::map<int, char> patterns;
	int size = 7;
	int count = 0, A=0, B=0, C=0;
	int suit;
	char temp;
	std::map<int, char>::iterator it;
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
				patterns.insert(std::pair<int, char>(suit,temp));

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

	std::string result(pattern_board,size);
	delete [] pattern_board;

	return result;
}


int CLutRiver::g_index(int c1, int c2, int board[])
{
	// sort cards
	if(c1 > c2)
		std::swap(c1,c2);

	std::sort(board, board+5);

	int cards[7] = {c1, c2, board[0], board[1], board[2], board[3], board[4]};

	std::string stemp;
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
		preflop_index = preflop_table.find(std::pair<int,int>(rc1,rc2))->second - 13;
		preflop_maximum -= 13;
	}
	else 
	{
		preflop_index = preflop_table.find(std::pair<int,int>(rc1,rc2))->second;
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

void CLutRiver::read_suitfile(std::string filename)
{
	delete [] riverindex;
	riverindex = new int[INDEX_SIZE];

	//insert the pairs
	for (int i = 0; i < 13; i++)
		preflop_table.insert(std::pair<std::pair<int,int>,int>(std::pair<int,int>(i,i),i));

	int count = 12;
	for(int i = 0; i < 13; i++)
	{
		for (int j = i+1; j < 13; j++)
		{
			count++;
			preflop_table.insert(std::pair<std::pair<int,int>,int>(std::pair<int,int>(i,j),count));
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

	std::fstream filestream(filename.c_str(), std::fstream::in);
	std::string temp_entry;
	char temp[256];
	char c[8];
	int size;

	while (filestream.good())
	{
		filestream.getline(temp, 256,'\n');
		std::stringstream tempstr;
		tempstr << temp;
		tempstr.get(c,8);
		tempstr >> size;
		suitoffset.insert(std::pair<std::string,int>(c,size));
	}

	filestream.flush();
	filestream.close();
}

void CLutRiver::load(char f_name[])
{
	LUT_load(f_name, TABLE_SIZE, data);
}
