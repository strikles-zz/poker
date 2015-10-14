#pragma once

#include <stdio.h>

#include "Bot_General.h"
#include "Bot_PlayerInfo.h"


class CPlayerStatistics
{
public:

	CPlayerStatistics();

public:

	int userchair;
	CPlayerInfo opponentInfo[k_MaxChairs];

	//OH_SEAT_VAR(pt_icon);
	OH_SEAT_VAR(pt_pfr);
	//OH_SEAT_VAR(pt_aggtot);
	//OH_SEAT_VAR(pt_aggtotnopf);
	//OH_SEAT_VAR(pt_aggp);
	OH_SEAT_VAR(pt_aggf);
	OH_SEAT_VAR(pt_aggt);
	//OH_SEAT_VAR(pt_aggr);
	OH_SEAT_VAR(pt_aggr);
	OH_SEAT_VAR(pt_floppct);
	OH_SEAT_VAR(pt_turnpct);
	OH_SEAT_VAR(pt_riverpct);
	OH_SEAT_VAR(pt_vpip);
	OH_SEAT_VAR(pt_hands);
	OH_SEAT_VAR(pt_pf_rfi);
	OH_SEAT_VAR(pt_pf_cr);
	OH_SEAT_VAR(pt_pfats);
	OH_SEAT_VAR(pt_wsdp);
	OH_SEAT_VAR(pt_wssd);
	OH_SEAT_VAR(pt_fsbts);
	OH_SEAT_VAR(pt_fbbts);

	// Strikles New Stats
	// aggq
	OH_SEAT_VAR(pt_aggfpct);
	OH_SEAT_VAR(pt_aggtpct);
	OH_SEAT_VAR(pt_aggrpct);
	// cbet
	OH_SEAT_VAR(pt_f_cbet);
	OH_SEAT_VAR(pt_t_cbet);
	OH_SEAT_VAR(pt_r_cbet);
	OH_SEAT_VAR(pt_f_fold_cbet);
	OH_SEAT_VAR(pt_t_fold_cbet);
	OH_SEAT_VAR(pt_r_fold_cbet);
	// 3-bet
	OH_SEAT_VAR(pt_p_3bet);
	OH_SEAT_VAR(pt_f_3bet);
	OH_SEAT_VAR(pt_t_3bet);
	OH_SEAT_VAR(pt_r_3bet);
	OH_SEAT_VAR(pt_p_fold_3bet);
	OH_SEAT_VAR(pt_f_fold_3bet);
	OH_SEAT_VAR(pt_t_fold_3bet);
	OH_SEAT_VAR(pt_r_fold_3bet);
	// 4-bet
	OH_SEAT_VAR(pt_p_4bet);
	OH_SEAT_VAR(pt_f_4bet);
	OH_SEAT_VAR(pt_t_4bet);
	OH_SEAT_VAR(pt_r_4bet);
	OH_SEAT_VAR(pt_p_fold_4bet);
	OH_SEAT_VAR(pt_f_fold_4bet);
	OH_SEAT_VAR(pt_t_fold_4bet);
	OH_SEAT_VAR(pt_r_fold_4bet);

public:

	void SetUserChair(int user);
	void GetDirtyPTVars(int iSeat);
	void SetDirtyPTStats(int iSeat);

	double GetPTStat(const char *pt_stat, int iSeat);
};