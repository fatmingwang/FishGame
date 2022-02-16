#ifndef _MONSTER_GENERATE_H_
#define _MONSTER_GENERATE_H_

class	cMonster;

struct	sFishShowProbaility
{
	enum	eFishGoType
	{
		eFGT_GROUP_MPDI = 0,
		eFGT_GROUP,
		eFGT_SINGLE,
		eFGT_MAX,
	};

	struct	sData
	{
		int						iProbaility;
		int						iFishIndex;
		float					fFishGroupProbability;
		float					fFishGroupMPDIProbability;
		float					fFishSingleProbability;
		eFishGoType				GetFishGoType();
		UT::sMinMaxData<int>	iFishGroupCount;
		std::wstring			strFishName;
		sData();
	};

	struct	sFishOutData
	{
		//how many fish out at same time
		UT::sMinMaxData<float>	fTimeCircle;
		UT::sMinMaxData<int>	iOutFishCount;
	};
	void								ProcessFishData(TiXmlElement*e_pTiXmlElement);
	void								ProcessShowProbability(TiXmlElement*e_pTiXmlElement,const WCHAR*e_strMonsterName,int e_iMonsterID);
	//for probability
	float								fFishGroupProbability;
	float								fTimeToGenerateMonsters;
	int									iTotalProbability;
	int									GetRandomFishID();
	std::vector<sData>					FishShowProbabilityList;
	std::map<int,sFishOutData>			PlayerCountWithFish;
	bool								bPauseGenerateMonster;
	int									iNumMonsterOutAtSameTime;	//how many fish out at same time
	UT::sTimeCounter					TimeGapToGenerateMonster;
	void								MonsterGenerate( float e_fElpaseTime );
	cMonster*							MonsterGo( int e_iMonsterID );
	sFishShowProbaility();
	~sFishShowProbaility();
	void								Init();
	void								Update(float e_fElpaseTime);
};



#endif
