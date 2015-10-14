/////////////////////////////////////////////////
///////////// DECISION PREDICTION ///////////////
/////////////////////////////////////////////////

class CDecisionPrediction
{
public:

	CDecisionPrediction();

	// Nothing to destroy yet
	//~CDecisionPrediction();

	// Rule of 3 :
	// Disabling Copy Constructor & assignement operator
	// Is this non-sense for a singleton ?
	CDecisionPrediction(const CDecisionPrediction&);
	CDecisionPrediction& operator=(const CDecisionPrediction&);

	//PREDICTION
	void prediction();

	//FLOP
	double floprais(int iPlayer);
	double flopcall(int iPlayer);
	double flopfold(int iPlayer);

	//TURN
	double turnrais(int iPlayer);
	double turncall(int iPlayer);
	double turnfold(int iPlayer);

	//RIVER
	double riverrais(int iPlayer);
	double rivercall(int iPlayer);
	double riverfold(int iPlayer);

	//FLOP
	double floprais_betamount(int iPlayer, double herobetammount, double &opponentbetammount);
	double flopcall_betamount(int iPlayer, double betammount);
	double flopfold_betamount(int iPlayer, double betammount);

	//TURN
	double turnrais_betamount(int iPlayer, double herobetammount, double &opponentbetammount);
	double turncall_betamount(int iPlayer, double betammount);
	double turnfold_betamount(int iPlayer, double betammount);

	//RIVER
	double riverrais_betamount(int iPlayer, double herobetammount, double &opponentbetammount);
	double rivercall_betamount(int iPlayer, double betammount);
	double riverfold_betamount(int iPlayer, double betammount);

};

extern CDecisionPrediction predict;