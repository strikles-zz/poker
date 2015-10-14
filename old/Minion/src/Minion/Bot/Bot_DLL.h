#pragma once

#define WHUSER_API __declspec(dllexport)

struct holdem_player
{
	char            m_name[16]          ;	//player name if known
	double          m_balance           ;	//player balance 
	double          m_currentbet        ;	//player current bet
	unsigned char   m_cards[2]          ;	//player cards

	unsigned char   m_name_known    : 1 ;	//0=no 1=yes
	unsigned char   m_balance_known : 1 ;	//0=no 1=yes
	unsigned char   m_fillerbits    : 6 ;	//filler bits
	unsigned char   m_fillerbyte        ;	//filler bytes
};

struct holdem_state
{
	char            m_title[64]         ;	//table title
	double          m_pot[10]           ;	//total in each pot

	unsigned char   m_cards[5]          ;	//common cards

	unsigned char   m_is_playing    : 1 ;	//0=sitting-out, 1=sitting-in
	unsigned char   m_is_posting    : 1 ;	//0=autopost-off, 1=autopost-on
	unsigned char   m_fillerbits    : 6 ;	//filler bits

	unsigned char   m_fillerbyte        ;	//filler byte
	unsigned char   m_dealer_chair      ;	//0-9

	holdem_player   m_player[10]        ;	//player records
};

struct sprw1326_chair
{
	int	level;		//indicates weighting level for 'always consider'
	int	limit;		//max index into weight array - used for computational efficiency
	int	ignore;		//if non-zero no weighting will be applied to this chair
	int	rankhi[1326];	//higher ranked card number in pocket cards
	int	ranklo[1326];	//lower ranked card number in pocket cards
	int	weight[1326];	//the significance value for this hand
	double	scratch;	//for future reference 
};

struct sprw1326
{
	int	useme;		//unless set to 1326 the normal OH prwin will be used
	int	preflop;	//unless set to 1326 the normal OH prwin will be used pre-flop
	int	usecallback;	//unless set to 1326 the callback function will not be invoked
	double	(*prw_callback)(); //if enabled will be invoked before the prwin calculation pass
	double	scratch;	//for future reference
	int	bblimp;		//if non-zero no weighting will be applied if a chair has put nothing in the pot
	sprw1326_chair	vanilla_chair;	   //will be precalculated by OH at startup - convenience values
	sprw1326_chair  chair[10];  //structures for each chair
};

typedef double (*process_message_t)(const char* message, const void* param );
WHUSER_API double process_message( const char* message, const void* param );

typedef double (*pfgws_t)( int c, const char* psym, bool& iserr );
typedef bool hl1k_t[1000][13][13];  // list number, rank0, rank1 - rank0>=rank1 == suited, rank0<rank1 == unsuited
typedef hl1k_t* phl1k_t;
typedef sprw1326* pp13;

