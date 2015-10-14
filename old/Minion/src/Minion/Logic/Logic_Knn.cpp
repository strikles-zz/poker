
#include <cassert>
#include <ctime>

#include "Logic/Logic_Knn.h"

#include "Bot/Bot_Globalvars.h"
#include "Calc/CHandPotential.h"
#include "Config/CPreferences.h"
#include "Logic/Logic.h"
#include "StructDefines/structdefines.h"
#include "CClientNet.h"




CKnnLogic::CKnnLogic() : 
	decision_amt(0), 
	decision_bettype(eBet_Unknown), 
	actiontype(eAction_Unknown),
	prev_balance_init(0), 
	prev_ntimesacted(0), 
	prev_br(eRound_Unknown), 
	prev_dealer(-1),
	current_hand_id(-1)
{
	srand(time(NULL));
}

void CKnnLogic::RandomAction()
{
	////////////////////////////////////////////////
	// Generate some random action to seed the db //
	////////////////////////////////////////////////

	std::vector<eBetType> possible_bets;
	// We can always fold
	possible_bets.push_back(eFold);

	// We can also probably always call/all-in
	if(gVars.balance[gVars.userchair] > 0)
		possible_bets.push_back(eCall);

	double min_bet = ToMoney(gVars.get_bblind());
	for(int i = eCall+1; i < eTotalActions; i++)
	{
		double amt_required = TranslateANNActionToAmount((eBetType) i);
		double amt_available = gVars.balance[gVars.userchair];

		if(amt_required > amt_available)
			break;
		if(amt_required < min_bet)
			continue;

		possible_bets.push_back((eBetType) i);
	}

	assert(possible_bets.size() > 0);

	int randbet_ndx = (possible_bets.size() > 0 ? rand() % possible_bets.size() : -1);
	decision_bettype = (randbet_ndx >= 0 ? possible_bets[randbet_ndx] : eFold);

	gLog.WriteLog(eSeverityInfo, eCatANN, "num_available_actions: %d, ndx: %d, value: %s\n", possible_bets.size(), randbet_ndx, bets_str[decision_bettype]);
}

void CKnnLogic::GetDecision()
{
	int hand[eDim_Total];
	
	////////////////////////////////////////
	// Get data into the hand_info struct //
	////////////////////////////////////////
	FillHandInfoStruct(hand);

#ifdef _DEBUG
	printHandStruct(hand);
#endif

	// Get Decision from server
	decision_bettype = gNet.ExecuteKnnMessageExchange(hand);

	// Save our current Action
	hero_actions[min(gVars.ntimesacted, 3)]	= decision_bettype;
	hand[eDim_Action1 + min(gVars.ntimesacted, 3)] = hero_actions[gVars.ntimesacted];

	SaveCurrentInfo(hand);

	////////////////////
	// DECISION STUFF //
	////////////////////
	decision_amt = TranslateANNActionToAmount(decision_bettype);
	// log it
	if(eFold == decision_bettype || eCall == decision_bettype /*|| eAllin == decision_bettype*/)
		gLog.WriteLog(eSeverityInfo, eCatANN, "knn decision = %s [%.2f]\n", bets_str[decision_bettype], decision_amt);
	else
		gLog.WriteLog(eSeverityInfo, eCatANN,	"knn decision = %s [%.2f = %.2f(C) + %.2f(R)] %.2f(P)\n",
													bets_str[decision_bettype],
													decision_amt,
													gVars.call, 
													decision_amt-gVars.call, 
													gVars.get_pot());

	// set action_type
	if(eCall < decision_bettype)
		actiontype = eAction_R;
	else if(eCall == decision_bettype && 0 < gVars.call)
		actiontype = eAction_C;
	else if(eCall == decision_bettype)
		actiontype = eAction_X;
	else
		actiontype = eAction_F;
}


double CKnnLogic::TranslateANNActionToAmount(eBetType betType)
{
	// The amounts used in the paper in terms of the pot
	// Add opening bets ???

	double amt = 0.0;
	switch(betType)
	{
		case eFold:
			break;
		case eCall:
			amt = gVars.call;
			break;
		case eHalfPot:
			amt = gVars.call + 0.5 * gVars.get_pot();
			break;
		case ePot:
			amt = gVars.call + gVars.get_pot();
			break;
		case eTwoPots:
			amt = gVars.call + 2.0 * gVars.get_pot();
			break;
			/*
		case eAllin:
			amt = gVars.balance[gVars.userchair];
			break;
			*/
		default:
			break;
	}

	return amt;
}


void CKnnLogic::FillHandInfoStruct(int* hand)
{
	bool	new_hand, new_street, new_bet;

	hand[eDim_handId] = current_hand_id;

	/////////////////////////////
	// NEW HAND / STREET / BET //
	/////////////////////////////
	new_hand = new_street = new_bet = false;

	// New Hand
	if(gVars.get_dealerchair() != prev_dealer)	
	{
		new_hand = true;
		hand[eDim_isNew] = 1;
	}
	// New street
	else if(gVars.get_betround() != prev_br)
	{
		new_street = true;
		hand[eDim_isNew] = 2;
	}
	// New bet
	else if(prev_ntimesacted != gVars.ntimesacted)
	{
		new_bet	= true;
		hand[eDim_isNew] = 3;
	}
	// initialize hero_actions
	if(new_hand || new_street)
	{
		for(int i = 0; i < k_max_actions; i++)
			hero_actions[i] = eBet_Unknown;
	}

	hand[eDim_brNdx] = BR2NDX(gVars.get_betround());
	assert(ePreflopIndex <= hand[eDim_brNdx] && eRiverIndex >= hand[eDim_brNdx]);

	hand[eDim_timesActed] = min(gVars.ntimesacted,3);
	// save hero_actions to hand_info struct
	for(int i = 0; i < k_max_actions; i++)
		hand[eDim_Action1+i] = hero_actions[i];
	
	hand[eDim_HS] = 100 * (hand[eDim_InHand] > 0 && hand[eDim_InHand] < k_max_chairs ? hand[eDim_InHand] : 1) * gVars.get_prwin();  //0..1
	hand[eDim_Raises] = gVars.get_nopponentsraising();

	/////////////////////////////////////////////////
	//Save data normalized on the interval 0..1000://
	/////////////////////////////////////////////////
	
	// save hand169 to hand_info struct
	if(ePreflopIndex == hand[eDim_brNdx])
	{
		hand[eDim_PPot] = 0;
		hand[eDim_NPot] = 0;
	}
	else
	{
		// Calc hand potentials
		if(eRiverIndex != hand[eDim_brNdx])
		{
			// Generate card masks
			StdDeck_CardMask hero_cards = {0}, board_cards = {0};
			for(int i = 0; i < 2; i++)
			{
				int card = StdDeck_MAKE_CARD(gVars.Bot_Common_Cards[i][0]-2, ConvertSuitsForPokerEval[gVars.Bot_Common_Cards[i][1]]);
				StdDeck_CardMask_OR(hero_cards, hero_cards, StdDeck_MASK(card));
				//gLog.WriteLog(eSeverityDebug, eCatHandPotential, "Player Card: %d\n", card);
			}
			for(int i = 0; i < (1 + gVars.get_betround()); i++)
			{
				int card = StdDeck_MAKE_CARD(gVars.Bot_Common_Cards[2+i][0]-2, ConvertSuitsForPokerEval[gVars.Bot_Common_Cards[2+i][1]]);
				StdDeck_CardMask_OR(board_cards, board_cards, StdDeck_MASK(card));
				//gLog.WriteLog(eSeverityDebug, eCatHandPotential, "Board Card: %d\n", card);
			}

			// calculate potentials
			// Shouldn't this calc be moved to the server ?
			CHandPotential hp(hero_cards, board_cards);
			hp.CalcPotentials();

			// save potentials to hand_info struct
			hand[eDim_PPot]= (int)(100*hp.Ppot); //0..1
			hand[eDim_NPot] = (int)(100*hp.Npot); //0..1
		}
		else
		{
			// no such thing as ppot/npot in the river
			hand[eDim_PPot] = 0;
			hand[eDim_NPot] = 0;
		}
	}

	hand[eDim_InHand]	= gVars.nplayersplaying; //1..6 /4
	// replace me !!!
	hand[eDim_Dealt]	= gVars.get_nplayersdealt(); //1..6 /4
	// replace me !!!
	hand[eDim_ToAct]	= gVars.m_state.LeftToAct(gVars.userchair); //1..6 /4
	// replace me !!!
	hand[eDim_Balance]	= (int)(gVars.balance[gVars.userchair]); //0..+-400
	hand[eDim_InitialBalance]	= (int)(gVars.initial_balance[gVars.userchair]); //0..+-400

	hand[eDim_Call]		= (int)(gVars.call); //0..+-400
	hand[eDim_Pot]		= (int)(gVars.get_pot()); //0..+-800
}


void CKnnLogic::SaveCurrentInfo(int* hand)
{
	///////////////////////
	// SAVE CURRENT INFO //
	///////////////////////


	// Get the Connected handler identifier of this OH instance
	// When the server gets it it will check against a std::set of handles for a client rebuy
	// id = ip + str(handle) ?
	std::vector<HWND> oh_handles = getToplevelOHWindows();
	assert(1 == oh_handles.size()); // Silly assert
	HWND oh_hwnd = oh_handles[0]; // hmmm

	int this_hwnd = -1;
	for(size_t i = 0; i < oh_handles.size(); i++)
	{
		this_hwnd = SendMessage(oh_handles[i], 0x8000+4, 0, 0);
		gLog.WriteLog(eSeverityInfo, eCatNet, "OH Says Casino Hwnd = %08X\n", this_hwnd);
	}

	// Assume nothing has changed and we have already written
	bool written = true;
	// save some prev_info
	if(hand[eDim_isNew] == 1 || hand[eDim_isNew] == 2 || hand[eDim_isNew] == 3)
	{
		written	= false;
		prev_dealer			= gVars.get_dealerchair();
		prev_br				= gVars.get_betround();
		prev_ntimesacted	= gVars.ntimesacted;
	}
		
	// if this is true something must have changed so we write :)
	if(false == written)
		current_hand_id = SaveHandSituation(hand, oh_hwnd);

	// previous handsplayed
	if(hand[eDim_isNew] == 1)
	{
		// Just started. Save our current balance as the initial value
		if(0 == gVars.get_handsplayed())
			prev_balance_init = gVars.balance[gVars.userchair] + gVars.currentbet[gVars.userchair];

		// A hand_id has been returned so somthing was already inserted
		if(-1 != hand[eDim_handId])
		{
			// calc the profit in terms of sb's
			int profit = 100.0*(gVars.balance[gVars.userchair] + gVars.currentbet[gVars.userchair] - prev_balance_init);
			
			int profits[eProfit_Total];
			profits[eProfit_handId]= hand[eDim_handId];
			profits[eProfit_Amt] = profit;
			profits[eProfit_hwnd] = this_hwnd;
			SaveProfit(profits, oh_hwnd);
		}
	}
}

int CKnnLogic::SaveHandSituation(int* hand, HWND instance_hwnd)
{
	assert(hand[eDim_brNdx] >= ePreflopIndex && hand[eDim_brNdx] < eRoundIndices);
	assert(hand[eDim_timesActed] >= 0);

	// send the message to the server
	int inserted_hand_id = gNet.ExecuteHandUpdateMessageExchange(hand);

	return inserted_hand_id;
}

int CKnnLogic::SaveProfit(int* profit, HWND instance_hwnd)
{
	int inserted_hand_id = -1;
	if(profit[eProfit_handId] > 0)
	{
		inserted_hand_id = gNet.ExecuteProfitMessageExchange(profit);
		gLog.WriteLog(eSeverityInfo, eCatANN, "(%d) Knn profit = %d SB\n", profit[eProfit_handId], profit[eProfit_Amt]);
	}

	return inserted_hand_id;
}