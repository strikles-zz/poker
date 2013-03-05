#include <io.h>
#include <windows.h>
#include <tchar.h>
#include <omp.h>

#include <iostream>

#include "Checksum.h"
#include "generate_table.h"
#include "LUT.h"

static const bool verify_checksums = true;
static const bool test_luts = false;

static const bool generate_flop = true;
static const bool generate_turn = true;
static const bool generate_river = true;

static const char* expected_2p2_md5 = "5de2fa6f53f4340d7d91ad605a6400fb";
static const char* expected_flopsuit_md5 = "0eb988590477a6a940dc7088c178828f";
static const char* expected_turnsuit_md5 = "5d2c05df980b19361712d72731fea9cb";
static const char* expected_riversuit_md5 = "db6db52e5e338632ba4493e5bc726a4e";
static const char* expected_flop_md5 = "42ad9eec489498d78893e180890cc221";
static const char* expected_turn_md5 = "021e23f1134d0d5b23907280dbf31f70";
static const char* expected_river_md5 = "e6de985c1e49b30a39d4d97473dae1da";

// the card deck is represented as integers, from 0 to 51. 
// Whereas 0-3 are Twos, 4-7 are Threes up to 48-51 are Aces.

/*
2c = 0 2d = 1 2h = 2 2s = 3
3c = 4 3d = 5 3h = 6 3s = 7
4c = 8 4d = 9 4h = 10 4s = 11
5c = 12 5d = 13 5h = 14 5s = 15
6c = 16 6d = 17 6h = 18 6s = 19
7c = 20 7d = 21 7h = 22 7s = 23
8c = 24 8d = 25 8h = 26 8s = 27
9c = 28 9d = 29 9h = 30 9s = 31
Tc = 32 Td = 33 Th = 34 Ts = 35
Jc = 36 Jd = 37 Jh = 38 Js = 39
Qc = 40 Qd = 41 Qh = 42 Qs = 43
Kc = 44 Kd = 45 Kh = 46 Ks = 47
Ac = 48 Ad = 49 Ah = 50 As = 51
*/

// Conv PE to this = 4*Rank + Suit
// Rank (0 .. 12)
// Suit (0 .. 3) -> CDHS



int main (int args) 
{ 
	// Check if 2P2 HandRanks.dat exists, if not generate it  
	if(_access("HandRanks.dat", 0) == -1) 
	{
		Generate2p2 TwoPlusTwo;
		TwoPlusTwo.Generate_DAT();
	}
	else if(verify_checksums)
	{
		char* current_2p2_md5 = new char[33];
		current_2p2_md5 = CalcChecksum("Handranks.dat", current_2p2_md5);

		if (strncmp(expected_2p2_md5, current_2p2_md5, 33) != 0)
		{
			printf("CHECKSUM ERROR : regenerating HandRanks.dat\n");

			// If the file is successfully deleted, a zero value is returned.
			if (remove("HandRanks.dat") == 0)
			{
				Generate2p2 TwoPlusTwo;
				TwoPlusTwo.Generate_DAT();
			}
			else
				printf("ERROR : Could not remove HandRanks.dat\n");
		}

		delete [] current_2p2_md5;
	}



	///////////////////////////////////////////////////////
	// LOAD 2P2 HandRanks.dat and map it into the HR array
	///////////////////////////////////////////////////////
	printf("Loading HandRanks.DAT file...");

	int* HR = new int[32487834];
	memset(HR, 0, sizeof(int)*32487834);

	errno_t err_2p2 = 0;
	FILE* f_2p2 = NULL;
	
	err_2p2 = fopen_s(&f_2p2, "HandRanks.dat", "rb");

	if (err_2p2 != 0)
	{
		return false;
	}

	size_t bytesread = fread(HR, sizeof(int)*32487834, 1, f_2p2);	// get the HandRank Array
	fclose(f_2p2);
	printf("complete.\n\n");



	/////////////////////
	// Generate FLOP LUT
	/////////////////////
	if(generate_flop)
	{
		CLutFlop *p_flop = new CLutFlop();

		// Check if flop_suit.dat exists, if not generate it  
		if(_access("flop_suit.dat", 0) == -1) 
		{
			// before using a postflop table you need to generate a suitfile first, you only need to do that once.
			p_flop->write_suitfile();
		}
		else if(verify_checksums)
		{
			char* current_flopsuit_md5 = new char[33];
			current_flopsuit_md5 = CalcChecksum("flop_suit.dat", current_flopsuit_md5);

			if (strncmp(expected_flopsuit_md5, current_flopsuit_md5, 33) != 0)
			{
				printf("CHECKSUM ERROR : regenerating flop_suit.dat\n");

				if (remove("flop_suit.dat") == 0)
					p_flop->write_suitfile();
				else
					printf("ERROR : Could not remove flop_suit.dat\n");
			}

			delete [] current_flopsuit_md5;
		}

		//before doing anything with the lut, load the suitfile
		p_flop->read_suitfile("flop_suit.dat");

		// Check if flop.dat exists, if not generate it  
		if(_access("flop.dat", 0) == -1) 
		{
			//then you can populate the table
			p_flop->generate("flop.dat", HR);
		}
		else if(verify_checksums)
		{
			char* current_flop_md5 = new char[33];		
			current_flop_md5 = CalcChecksum("flop.dat", current_flop_md5);

			if (strncmp(expected_flop_md5, current_flop_md5, 33) != 0)
			{
				printf("CHECKSUM ERROR : regenerating flop.dat\n");

				if (remove("flop.dat") == 0)
					p_flop->generate("flop.dat", HR);
				else
					printf("ERROR : Could not remove flop.dat\n");
			}

			delete [] current_flop_md5;
		}

		/////////////////////
		// TEST FLOP
		/////////////////////
		if(test_luts)
		{
			p_flop->load("flop.dat");
			long flop_counter = 0, err_flop_counter = 0L;

			for (int hc1 = 0; hc1 < 52; hc1++)
			{
				for (int hc2 = hc1+1; hc2 < 52; hc2++)
				{
					#pragma omp parallel for
					for (int f1 = 0; f1 < 52; f1++)
					{
						for (int f2 = f1+1; f2 < 52; f2++)
						{
							for (int f3 = f2+1; f3 < 52; f3++)
							{

								if (f1 == hc1 || f1 == hc2 || f2 == hc1 || 
									f2 == hc2 || f3 == hc1 || f3 == hc2)
									continue;

								else
								{
									int board[3] = {f1,f2,f3};
									int index = p_flop->g_index(hc1, hc2, board);
									short data = p_flop->data[index];

									double hs = 0;
									compute_hs_flop(hc1, hc2, f1, f2, f3, hs, HR);
									flop_counter++;

									if (data != round(hs))
									{
										err_flop_counter++;
										printf("\nERROR : %d [%ld] %5u %5u\n", index, err_flop_counter, data, round(hs));
									}
									else
									{
										printf("\rFLOP : %ld - %5u", flop_counter, data);
									}
								}
							}
						}	
					}
				}
			}
		}
	}

	/////////////////////
	// TURN
	/////////////////////
	if(generate_turn)
	{
		CLutTurn *p_turn = new CLutTurn();

		// Check if turn_suit.dat exists, if not generate it  
		if(_access("turn_suit.dat", 0) == -1) 
		{
			// before using a postflop table you need to generate a suitfile first, you only need to do that once.
			p_turn->write_suitfile();
		}
		else if(verify_checksums)
		{
			char* current_turnsuit_md5 = new char[33];
			current_turnsuit_md5 = CalcChecksum("turn_suit.dat", current_turnsuit_md5);

			if (strncmp(expected_turnsuit_md5, current_turnsuit_md5, 33) != 0)
			{
				printf("CHECKSUM ERROR : regenerating turn_suit.dat\n");

				if (remove("turn_suit.dat") == 0)
					p_turn->write_suitfile();
				else
					printf("ERROR : Could not remove turn_suit.dat\n");
			}

			delete [] current_turnsuit_md5;
		}

		//before doing anything with the lut, load the suitfile
		p_turn->read_suitfile("turn_suit.dat");

		// Check if turn.dat exists, if not generate it  
		if(_access("turn.dat", 0) == -1) 
		{
			// then you can populate the table
			p_turn->generate("turn.dat", HR);
		}
		else if(verify_checksums)
		{
			char* current_turn_md5 = new char[33];
			current_turn_md5 = CalcChecksum("turn.dat", current_turn_md5);

			if (strncmp(expected_turn_md5, current_turn_md5, 33) != 0)
			{
				printf("CHECKSUM ERROR : regenerating turn.dat\n");

				if (remove("turn.dat") == 0)
					p_turn->generate("turn.dat", HR);
				else
					printf("ERROR : Could not remove turn.dat\n");
			}

			delete [] current_turn_md5;
		}

		/////////////////////
		// TEST TURN
		/////////////////////
		if(test_luts)
		{
			p_turn->load("turn.dat");
			long turn_counter = 0, err_turn_counter = 0;

			for (int hc1 = 0; hc1 < 52; hc1++)
			{
				for (int hc2 = hc1+1; hc2 < 52; hc2++)
				{
					#pragma omp parallel for
					for (int f1 = 0; f1 < 52; f1++)
					{
						for (int f2 = f1+1; f2 < 52; f2++)
						{
							for (int f3 = f2+1; f3 < 52; f3++)
							{
								for (int t = f3+1; t < 52; t++)
								{

									if (f1 == hc1 || f1 == hc2 || 
										f2 == hc1 || f2 == hc2 || 
										f3 == hc1 || f3 == hc2 ||
										t == hc1 || t == hc2)
										continue;

									else
									{
										int board[4] = {f1,f2,f3, t};
										int index = p_turn->g_index(hc1, hc2, board);
										short data = p_turn->data[index];

										double hs = 0;
										compute_hs_turn(hc1, hc2, f1, f2, f3, t, hs, HR);
										turn_counter++;

										if (data != round(hs))
										{
											err_turn_counter++;
											printf("\nERROR : %d [%ld] - %5u\n", index, err_turn_counter, data-hs);
										}
										else
										{
											printf("\rTURN : %ld - %5u", turn_counter, data);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}


	/////////////////////
	// RIVER
	/////////////////////
	if(generate_river)
	{
		CLutRiver *p_river = new CLutRiver();

		// Check if flop_suit.dat exists, if not generate it  
		if(_access("river_suit.dat", 0) == -1) 
		{
			// before using a postflop table you need to generate a suitfile first, you only need to do that once.
			p_river->write_suitfile();
		}
		else if(verify_checksums)
		{
			char* current_riversuit_md5 = new char[33];
			current_riversuit_md5 = CalcChecksum("river_suit.dat", current_riversuit_md5);

			if (strcmp(expected_riversuit_md5, current_riversuit_md5) != 0)
			{
				printf("CHECKSUM ERROR : regenerating river_suit.dat\n");

				if (remove("river_suit.dat") == 0)
					p_river->write_suitfile();
				else
					printf("ERROR : Could not remove river_suit.dat\n");

			}

			delete [] current_riversuit_md5;
		}

		//before doing anything with the lut, load the suitfile
		p_river->read_suitfile("river_suit.dat");

		// Check if river.dat exists, if not generate it  
		if(_access("river.dat", 0) == -1) 
		{
			//then you can populate the table
			p_river->generate("river.dat", HR);
		}
		else if(verify_checksums)
		{
			char* current_river_md5 = new char[33];
			current_river_md5 = CalcChecksum("river.dat", current_river_md5);

			if (strncmp(expected_river_md5, current_river_md5, 33) != 0)
			{
				printf("CHECKSUM ERROR : regenerating river.dat\n");

				if (remove("river.dat") == 0)
					p_river->generate("river.dat", HR);
				else
					printf("ERROR : Could not remove river.dat\n");
			}

			delete [] current_river_md5;
		}

		/////////////////////
		// TEST RIVER
		/////////////////////
		if(test_luts)
		{
			p_river->load("river.dat");
			long river_counter = 0, err_river_counter = 0;

			for (int hc1 = 0; hc1 < 52; hc1++)
			{
				for (int hc2 = hc1+1; hc2 < 52; hc2++)
				{
					#pragma omp parallel for
					for (int f1 = 0; f1 < 52; f1++)
					{
						for (int f2 = f1+1; f2 < 52; f2++)
						{
							for (int f3 = f2+1; f3 < 52; f3++)
							{
								for (int t = f3+1; t < 52; t++)
								{
									for (int r = t+1; r < 52; r++)
									{
										if (f1 == hc1 || f1 == hc2 || 
											f2 == hc1 || f2 == hc2 || 
											f3 == hc1 || f3 == hc2 ||
											t == hc1 || t == hc2 ||
											r == hc1 || r == hc2)
											continue;

										else
										{
											int board[5] = {f1,f2,f3, t, r};
											int index = p_river->g_index(hc1, hc2, board);
											short data = p_river->data[index];

											double hs = 0;
											compute_single_hs(hc1, hc2, f1, f2, f3, t, r, hs, HR);
											river_counter++;

											if (data != round(hs))
											{
												err_river_counter++;
												printf("\nERROR : %d [%ld] - %5u\n", index, err_river_counter, data-hs);
											}
											else
											{
												printf("\rRIVER : %ld - %5u", river_counter, data);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	/////////////////////
	// Preflop
	/////////////////////
	//CLutPreFlop *p_preflop = new CLutPreFlop();

	// the preflop table works similarly, the only difference is that there is no suitfile
	// but you have to call prepare before you can do stuff with it
	// p_preflop->prepare();

	// thats it have fun with the tables

	delete [] HR;
	printf("\n\n>>> Finished :)\n");

	getchar();

	return 0;
}