#ifndef _SLOT_TABLE_RESULT_H_
#define	_SLOT_TABLE_RESULT_H_
//table result mean slot status which win the money
struct	sSlotBetTableResult
{
	struct sLineResult
	{
		int					iWinMoneySpinObjectID;
		int					iMatchCount;
		sLineResult(){ iWinMoney = 0; }
		int iIndex;
		//the index for win money spin object
		std::vector<int>	WinMoneySpinObjectIndexList;
		//data to check if win money or not
		std::vector<int>	SpinObjectResultList;
		int					iWinMoney;
	};
	sSlotBetTableResult();
	~sSlotBetTableResult();
	//each spin object's result
	std::vector<int>				SpinObjectResultList;
	int								NumSpinObjectInTable(int e_iSpinObjectIndex);
	//all lines
	std::vector<sLineResult>		AllLineList;
	//win money lines
	std::vector<sLineResult>		WinMoneyLineList;
	std::vector<int>				WinMoneyLineIndexList;
	//if true this spin object full the condition of prize
	bool*							pbWinMoneySpinObject;
	int								iWinMoney;
	//for control panel data,some memory problem so I make its maximum to 10 to avoid crash
	int								piConsecutiveScore[10];
	int								piConsecutiveCount[10];
	void							ClearData(int e_iNumLines);
	//int								i3ConsecutiveScore;
	//int								i4ConsecutiveScore;
	//int								i5ConsecutiveScore;
	//int								i3ConsecutiveCount;
	//int								i4ConsecutiveCount;
	//int								i5ConsecutiveCount;
};

#endif