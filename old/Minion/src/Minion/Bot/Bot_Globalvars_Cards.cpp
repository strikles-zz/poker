
#include "Bot/Bot_Globalvars.h"

#include "Util/Util.h"
#include "Util/Util_MagicNumbers.h"



void update_board_cards()
{
	/*
	*  
	*/

	//PREFLOP
	if(gVars.get_betround() == 1)
	{
		gVars.board_cards = "XxXxXx";
	}

	//FLOP
	if(gVars.get_betround() == 2)
	{
		gVars.board_cards[0] = Rank_ASC(gVars.Bot_Common_Cards[2][0]);
		gVars.board_cards[1] = Suit52_ASC(gVars.Bot_Common_Cards[2][1]);
		gVars.board_cards[2] = Rank_ASC(gVars.Bot_Common_Cards[3][0]);
		gVars.board_cards[3] = Suit52_ASC(gVars.Bot_Common_Cards[3][1]);
		gVars.board_cards[4] = Rank_ASC(gVars.Bot_Common_Cards[4][0]);
		gVars.board_cards[5] = Suit52_ASC(gVars.Bot_Common_Cards[4][1]);

	}

	//TURN
	if(gVars.get_betround() == 3)
	{
		gVars.board_cards[0] = Rank_ASC(gVars.Bot_Common_Cards[2][0]);
		gVars.board_cards[1] = Suit52_ASC(gVars.Bot_Common_Cards[2][1]);
		gVars.board_cards[2] = Rank_ASC(gVars.Bot_Common_Cards[3][0]);
		gVars.board_cards[3] = Suit52_ASC(gVars.Bot_Common_Cards[3][1]);
		gVars.board_cards[4] = Rank_ASC(gVars.Bot_Common_Cards[4][0]);
		gVars.board_cards[5] = Suit52_ASC(gVars.Bot_Common_Cards[4][1]);
		gVars.board_cards[6] = Rank_ASC(gVars.Bot_Common_Cards[5][0]);
		gVars.board_cards[7] = Suit52_ASC(gVars.Bot_Common_Cards[5][1]);
	}

	//RIVER
	if(gVars.get_betround() == 4)
	{
		gVars.board_cards[0] = Rank_ASC(gVars.Bot_Common_Cards[2][0]);
		gVars.board_cards[1] = Suit52_ASC(gVars.Bot_Common_Cards[2][1]);
		gVars.board_cards[2] = Rank_ASC(gVars.Bot_Common_Cards[3][0]);
		gVars.board_cards[3] = Suit52_ASC(gVars.Bot_Common_Cards[3][1]);
		gVars.board_cards[4] = Rank_ASC(gVars.Bot_Common_Cards[4][0]);
		gVars.board_cards[5] = Suit52_ASC(gVars.Bot_Common_Cards[4][1]);
		gVars.board_cards[6] = Rank_ASC(gVars.Bot_Common_Cards[5][0]);
		gVars.board_cards[7] = Suit52_ASC(gVars.Bot_Common_Cards[5][1]);
		gVars.board_cards[8] = Rank_ASC(gVars.Bot_Common_Cards[6][0]);
		gVars.board_cards[9] = Suit52_ASC(gVars.Bot_Common_Cards[6][1]);
	}
}

void CGlobalVars::set_card_values()
{
	/*
	* sets the card values
	*/

	//pocket cards;
	Bot_Common_Cards[0][0] = RANK(m_state.CurState().m_player[userchair].m_cards[0]);
	Bot_Common_Cards[0][1] = SUIT(m_state.CurState().m_player[userchair].m_cards[0]);
	Bot_Common_Cards[1][0] = RANK(m_state.CurState().m_player[userchair].m_cards[1]);
	Bot_Common_Cards[1][1] = SUIT(m_state.CurState().m_player[userchair].m_cards[1]);

	//board cards
	StdDeck_CardMask curCard;
	StdDeck_CardMask_RESET(curBoardMask);

	for(int i=0;i<5;i++)
	{
		Bot_Common_Cards[i+2][0] = RANK(m_state.CurState().m_cards[i]);
		Bot_Common_Cards[i+2][1] = SUIT(m_state.CurState().m_cards[i]);

		if(!ISUNKNOWN(m_state.CurState().m_cards[i])) 
		{
			curCard = StdDeck_MASK(StdDeck_MAKE_CARD(Bot_Common_Cards[i+2][0]-2, ConvertSuitsForPokerEval[Bot_Common_Cards[i+2][1]]));
			StdDeck_CardMask_OR(curBoardMask, curBoardMask, curCard);
		}
	}
}

int CGlobalVars::set_rankhiplayer()
{
	/*
	* returns hi player rank
	*/

	if(Bot_Common_Cards[0][0] > Bot_Common_Cards[1][0])
		return Bot_Common_Cards[0][0];

	else
		return Bot_Common_Cards[1][0];
}

int CGlobalVars::set_rankloplayer()
{
	/*
	* returns lo player rank
	*/

	if(Bot_Common_Cards[0][0] > Bot_Common_Cards[1][0])
		return Bot_Common_Cards[1][0];

	else
		return Bot_Common_Cards[0][0];
}

bool CGlobalVars::set_suited()
{
	/*
	* returns 1 - suited ; 0 -unsuited
	*/

	if(Bot_Common_Cards[0][1] == Bot_Common_Cards[1][1])
		return true;

	else
		return false;
}



char *CGlobalVars::getMyHand()
{
	/*
	* returns a string with myHand value
	*/

	const char randChars[] = "  23456789TJQKA";
	myHand[0] = randChars[rankhiplayer];
	myHand[1] = randChars[rankloplayer];

	if(rankhiplayer == rankloplayer)
		myHand[2] = '\0';
	
	else
		myHand[2] = (issuited ? 's' : 'o');

	return myHand;
}

bool CGlobalVars::isHand(const char* handslist)
{
	/*
	* returns 1 is myHand is in handlist
	* returns 0 is myHand isn't in handlist
	*/

	if(strstr(handslist, myHand) != NULL)
	{
		//Hand found
		return 1;
	}

	//Hand not found in handlist
	return 0;
}