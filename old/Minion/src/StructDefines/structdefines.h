#pragma once


enum eDims { 
	// header
	eDim_RequestType,
	eDim_Decision,
	eDim_handId,
	eDim_isNew,
	eDim_brNdx,
	eDim_timesActed,
	// dims
	eDim_Action1,
	eDim_Action2,
	eDim_Action3,
	eDim_Action4, 
	eDim_HS, 
	eDim_InHand, 
	eDim_Dealt, 
	eDim_ToAct, 
	eDim_Balance, 
	eDim_InitialBalance, 
	eDim_Call, 
	eDim_Pot, 
	eDim_Raises,
	eDim_PPot, 
	eDim_NPot,
	// markers
	eDim_Total,
	eDim_End = eDim_NPot,
	eDim_PF_End = eDim_Raises,
	eDim_Start = eDim_HS
};

static const char* dim_names[] = 
{
	// header
	"request_type",
	"decision",
	"hand_id",
	"is_new",
	"br_ndx",
	"times_acted",
	// dims
	"dim_action1",
	"dim_action2",
	"dim_action3",
	"dim_action4",
	"dim_hand_strength", 
	"dim_players_in_hand", 
	"dim_players_dealt", 
	"dim_players_yet_to_act", 
	"dim_balance", 
	"dim_initial_balance", 
	"dim_call", 
	"dim_pot", 
	"dim_facing_nraises",
	"dim_ppot", 
	"dim_npot"
};

/*
	Methods that need to be changed when adding dimensions
	CKnnLogic::FillHandInfoStruct(int* hand)
*/
enum eProfit
{
	// headers
	eProfit_RequestType,
	eProfit_handId,
	eProfit_hwnd,
	// profit
	eProfit_Amt,
	// markers
	eProfit_Total
};

static const char* profit_names[] = {
	"Request_Type",
	"hand_id",
	"HWND",
	"amount",
	
};

void printHandStruct(int* hand);

/*
#pragma pack(push)
#pragma pack(1)

struct knn_hand_info
{
	// hand_info header
	int hand_id;
	int is_new;						// 0 = new Hand, 1 = new Street, 2 = new round of bets
	int br_ndx;						// 0..3
	int times_acted;				// 0..3

	// dim data
	int action[k_max_actions];		// 0..4
	int hand_strength;				// 0..100 prwin
	int ppot;						// 0..100
	int npot;						// 0..100
	int num_players_in_hand;		// 0..10
	int num_players_dealt;			// 0..10
	int num_players_to_act;			// 0..10
	int initial_balance;			// ...
	int balance;					// ...
	int call;						// ...
	int pot;						// ...
	int facing_nraises;				// ...
};

struct profit_info
{
	int hand_id;
	int amount;
	int connected_hwnd;
};

*/


