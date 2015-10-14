#include "stdafx.h"

#include "Bot_PTStats.h"
#include "Util_logging.h"



CPlayerStatistics::CPlayerStatistics()
{
}	

double CPlayerStatistics::GetPTStat(const char *pt_stat, int iSeat)
{
	// Dummy test fnc

	char query[64];
	sprintf_s(query, 64, "%s%d", pt_stat, iSeat);

	return gws(query);
}

void CPlayerStatistics::SetUserChair(int user)
{
	userchair = user;
}

void CPlayerStatistics::GetDirtyPTVars(int iSeat)
{
	//////////////////
	// Dirty Poker Tracker stats
	/////////////////

	dirty_pt_hands(iSeat);
	dirty_pt_pfr(iSeat);
	dirty_pt_aggf(iSeat);
	dirty_pt_aggt(iSeat);
	dirty_pt_aggr(iSeat);
	dirty_pt_floppct(iSeat);
	dirty_pt_turnpct(iSeat);
	dirty_pt_riverpct(iSeat);
	dirty_pt_vpip(iSeat);

	dirty_pt_pf_rfi(iSeat);
	dirty_pt_pf_cr(iSeat);
	dirty_pt_pfats(iSeat);
	dirty_pt_wsdp(iSeat);
	dirty_pt_wssd(iSeat);
	dirty_pt_fsbts(iSeat);
	dirty_pt_fbbts(iSeat);

	// New Stats
	dirty_pt_aggfpct(iSeat);
	dirty_pt_aggtpct(iSeat);
	dirty_pt_aggrpct(iSeat);

	dirty_pt_f_cbet(iSeat);
	dirty_pt_t_cbet(iSeat);
	dirty_pt_r_cbet(iSeat);
	dirty_pt_f_fold_cbet(iSeat);
	dirty_pt_t_fold_cbet(iSeat);
	dirty_pt_r_fold_cbet(iSeat);

	dirty_pt_p_3bet(iSeat);
	dirty_pt_f_3bet(iSeat);
	dirty_pt_t_3bet(iSeat);
	dirty_pt_r_3bet(iSeat);
	dirty_pt_p_fold_3bet(iSeat);
	dirty_pt_f_fold_3bet(iSeat);
	dirty_pt_t_fold_3bet(iSeat);
	dirty_pt_r_fold_3bet(iSeat);

	dirty_pt_p_4bet(iSeat);
	dirty_pt_f_4bet(iSeat);
	dirty_pt_t_4bet(iSeat);
	dirty_pt_r_4bet(iSeat);
	dirty_pt_p_fold_4bet(iSeat);
	dirty_pt_f_fold_4bet(iSeat);
	dirty_pt_t_fold_4bet(iSeat);
	dirty_pt_r_fold_4bet(iSeat);
}

void CPlayerStatistics::SetDirtyPTStats(int iSeat)
{
	opponentInfo[iSeat].pt_hands = get_pt_hands(iSeat);

	//Poker Tracker pre-flop raise
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_pfr(iSeat) <= 0)
		opponentInfo[iSeat].pt_pfr = 0.15;
	else
		opponentInfo[iSeat].pt_pfr = get_pt_pfr(iSeat);



	//Poker Tracker flop aggression
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_aggf(iSeat) <= 0)
		opponentInfo[iSeat].pt_aggf = 1;
	else
		opponentInfo[iSeat].pt_aggf = get_pt_aggf(iSeat);

	//Poker Tracker turn aggression
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_aggt(iSeat) <= 0)
		opponentInfo[iSeat].pt_aggt = 1;
	else
		opponentInfo[iSeat].pt_aggt = get_pt_aggt(iSeat);

	//Poker Tracker river aggression
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_aggr(iSeat) <= 0)
		opponentInfo[iSeat].pt_aggr = 1;
	else
		opponentInfo[iSeat].pt_aggr = get_pt_aggr(iSeat);

	//Poker Tracker flop aggression %
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_aggfpct(iSeat) <= 0)
		opponentInfo[iSeat].pt_aggfpct = 0.3;
	else
		opponentInfo[iSeat].pt_aggfpct = get_pt_aggfpct(iSeat);

	//Poker Tracker turn aggression %
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_aggtpct(iSeat) <= 0)
		opponentInfo[iSeat].pt_aggtpct = 0.5;
	else
		opponentInfo[iSeat].pt_aggtpct = get_pt_aggtpct(iSeat);

	//Poker Tracker river aggression %
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_aggrpct(iSeat) <= 0)
		opponentInfo[iSeat].pt_aggrpct = 0.3;
	else
		opponentInfo[iSeat].pt_aggrpct = get_pt_aggrpct(iSeat);



	//Poker Tracker saw flop pct
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_floppct(iSeat) <= 0)
		opponentInfo[iSeat].pt_floppct = 0.25;
	else
		opponentInfo[iSeat].pt_floppct = get_pt_floppct(iSeat);

	//Poker Tracker saw turn pct
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_turnpct(iSeat) <= 0)
		opponentInfo[iSeat].pt_turnpct = 0.15;
	else
		opponentInfo[iSeat].pt_turnpct = get_pt_turnpct(iSeat);

	//Poker Tracker saw river pct
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_riverpct(iSeat) <= 0)
		opponentInfo[iSeat].pt_riverpct = 0.10;
	else
		opponentInfo[iSeat].pt_riverpct = get_pt_riverpct(iSeat);



	//Poker Tracker VP$IP
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_vpip(iSeat) <= 0)
		opponentInfo[iSeat].pt_vpip = 0.25;
	else
		opponentInfo[iSeat].pt_vpip = get_pt_vpip(iSeat);



	//Poker Tracker "pre-flop raise first in" pct
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_pf_rfi(iSeat) <= 0)
		opponentInfo[iSeat].pt_pf_rfi = 0.10;
	else
		opponentInfo[iSeat].pt_pf_rfi = get_pt_pf_rfi(iSeat);

	//Poker Tracker "pre-flop called raise" pct
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_pf_cr(iSeat) <= 0)
		opponentInfo[iSeat].pt_pf_cr = 0.05;
	else
		opponentInfo[iSeat].pt_pf_cr = get_pt_pf_cr(iSeat);

	//Poker Tracker attempt to steal blinds
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_pfats(iSeat) <= 0)
		opponentInfo[iSeat].pt_pfats = 0.1;
	else
		opponentInfo[iSeat].pt_pfats = get_pt_pfats(iSeat);

	//Poker Tracker folded small blind to steal
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_fsbts(iSeat) <= 0)
		opponentInfo[iSeat].pt_fsbts = 0.3;
	else
		opponentInfo[iSeat].pt_fsbts = get_pt_fsbts(iSeat);

	//Poker Tracker folded big blind to steal
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_fbbts(iSeat) <= 0)
		opponentInfo[iSeat].pt_fbbts = 0.25;
	else
		opponentInfo[iSeat].pt_fbbts = get_pt_fbbts(iSeat);



	//Poker Tracker went to showdown percentage
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_wsdp(iSeat) <= 0)
		opponentInfo[iSeat].pt_wsdp = 0.08;
	else
		opponentInfo[iSeat].pt_wsdp = get_pt_wsdp(iSeat);

	//Poker Tracker won $ at showdown
	if (opponentInfo[iSeat].pt_hands < 20 || get_pt_wssd(iSeat) <= 0)
		opponentInfo[iSeat].pt_wssd = 0.50;
	else
		opponentInfo[iSeat].pt_wssd = get_pt_wssd(iSeat);




	opponentInfo[iSeat].pt_f_cbet = get_pt_f_cbet(iSeat);
	opponentInfo[iSeat].pt_t_cbet = get_pt_t_cbet(iSeat);
	opponentInfo[iSeat].pt_r_cbet = get_pt_r_cbet(iSeat);
	opponentInfo[iSeat].pt_f_fold_cbet = get_pt_f_fold_cbet(iSeat);
	opponentInfo[iSeat].pt_t_fold_cbet = get_pt_t_fold_cbet(iSeat);
	opponentInfo[iSeat].pt_r_fold_cbet = get_pt_r_fold_cbet(iSeat);

	opponentInfo[iSeat].pt_p_3bet = get_pt_p_3bet(iSeat);
	opponentInfo[iSeat].pt_f_3bet = get_pt_f_3bet(iSeat);
	opponentInfo[iSeat].pt_t_3bet = get_pt_t_3bet(iSeat);
	opponentInfo[iSeat].pt_r_3bet = get_pt_r_3bet(iSeat);
	opponentInfo[iSeat].pt_p_fold_3bet = get_pt_p_fold_3bet(iSeat);
	opponentInfo[iSeat].pt_f_fold_3bet = get_pt_f_fold_3bet(iSeat);
	opponentInfo[iSeat].pt_t_fold_3bet = get_pt_t_fold_3bet(iSeat);
	opponentInfo[iSeat].pt_r_fold_3bet = get_pt_r_fold_3bet(iSeat);

	opponentInfo[iSeat].pt_p_4bet = get_pt_p_4bet(iSeat);
	opponentInfo[iSeat].pt_f_4bet = get_pt_f_4bet(iSeat);
	opponentInfo[iSeat].pt_t_4bet = get_pt_t_4bet(iSeat);
	opponentInfo[iSeat].pt_r_4bet = get_pt_r_4bet(iSeat);
	opponentInfo[iSeat].pt_p_fold_4bet = get_pt_p_fold_4bet(iSeat);
	opponentInfo[iSeat].pt_f_fold_4bet = get_pt_f_fold_4bet(iSeat);
	opponentInfo[iSeat].pt_t_fold_4bet = get_pt_t_fold_4bet(iSeat);
	opponentInfo[iSeat].pt_r_fold_4bet = get_pt_r_fold_4bet(iSeat);

//#define LOG_PT_VARS 1
#if defined(LOG_PT_VARS)

	gLog->WriteLog(eSeverityInfo, eCatDebug, "aggfq: %.3f \taggtq: %.3f \taggtq: %.3f\n",opponentInfo[iSeat].pt_aggfpct,opponentInfo[iSeat].pt_aggtpct,opponentInfo[iSeat].pt_aggrpct);

	/*
	gLog->WriteLog(eSeverityInfo, eCatDebug, "Player[%d]: %s \tHands: %d\n",iSeat,opponentInfo[iSeat].name,opponentInfo[iSeat].pt_hands);
	gLog->WriteLog(eSeverityInfo, eCatDebug, "F_Cbet: %.2f \tT_Cbet: %.2f \tR_Cbet: %.2f\n",opponentInfo[iSeat].pt_f_cbet,opponentInfo[iSeat].pt_t_cbet,opponentInfo[iSeat].pt_r_cbet);
	gLog->WriteLog(eSeverityInfo, eCatDebug, "F_Fold_Cbet: %.2f \tT_Fold_Cbet: %.2f \tR_Fold_Cbet: %.2f\n",opponentInfo[iSeat].pt_f_fold_cbet,opponentInfo[iSeat].pt_t_fold_cbet,opponentInfo[iSeat].pt_r_fold_cbet);
	gLog->WriteLog(eSeverityInfo, eCatDebug, "PF_3bet: %.2f \tF_3bet: %.2f \tT_3bet: %.2f\n",opponentInfo[iSeat].pt_p_3bet,opponentInfo[iSeat].pt_f_3bet,opponentInfo[iSeat].pt_t_3bet);
	gLog->WriteLog(eSeverityInfo, eCatDebug, "PF_Fold_3bet: %.2f \tF_Fold_3bet: %.2f \tT_Fold_3bet: %.2f\n",opponentInfo[iSeat].pt_p_fold_3bet,opponentInfo[iSeat].pt_f_fold_3bet,opponentInfo[iSeat].pt_t_fold_3bet);
	gLog->WriteLog(eSeverityInfo, eCatDebug, "PF_4bet: %.2f \tF_4bet: %.2f \tT_4bet: %.2f\n",opponentInfo[iSeat].pt_p_4bet,opponentInfo[iSeat].pt_f_4bet,opponentInfo[iSeat].pt_t_4bet);
	gLog->WriteLog(eSeverityInfo, eCatDebug, "PF_Fold_4bet: %.2f \tF_Fold_4bet: %.2f \tT_Fold_4bet: %.2f\n",opponentInfo[iSeat].pt_p_fold_4bet,opponentInfo[iSeat].pt_f_fold_4bet,opponentInfo[iSeat].pt_t_fold_4bet);
	*/

#endif

}


#if defined(EXPERIMENTAL_STATS)
void CPlayerStatistics::ServePTStats(int& iSeat)
{
	//////////////////
	// Asigning stats
	//////////////////

	//Poker Tracker auto-rate icon
	/*
	if (opponentInfo[i].pt_hands == 0 || get_pt_icon(iSeat) == 0)
		opponentInfo[i].pt_icon = XX;
	else
		opponentInfo[i].pt_icon = get_pt_icon(iSeat);
	*/

	//Poker Tracker total aggression
	/*
	if (opponentInfo[i].pt_hands == 0 || get_pt_aggtot(iSeat) == 0)
		opponentInfo[i].pt_aggtot = XX;
	else
		opponentInfo[i].pt_aggtot = get_pt_aggtot(iSeat);
	*/

	//Poker Tracker total aggression excluding preflop
	/*
	if (opponentInfo[i].pt_hands == 0 || get_pt_aggtotnopf(iSeat) == 0)
		opponentInfo[i].pt_aggtotnopf = XX;
	else
		opponentInfo[i].pt_aggtotnopf = get_pt_aggtotnopf(iSeat);
	*/

	//Poker Tracker preflop aggression
	/*
	if (opponentInfo[i].pt_hands == 0 || get_pt_aggpx(iSeat) == 0)
		opponentInfo[i].pt_aggpx = XX;
	else
		opponentInfo[i].pt_aggpx = get_pt_aggpx(iSeat);
	*/
}

#endif