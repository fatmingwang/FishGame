#ifndef _ReceiveData_MasterLee_H_
#define _ReceiveData_MasterLee_H_
//
struct sHittedFish
{
	unsigned short	usID;
	unsigned char	ucPlayerID;
	unsigned int	uiFishOutMoney;
};

struct	sReceiveFishData
{
	char				cHeader;
	unsigned char		cNumFishHitted;
	sHittedFish	HittedFish[MAX_FISH_DIED_AT_ONE_UPLOAD];
	char				cChecksum;
	//
	sReceiveFishData()
	{ 
		memset( this, 0, sizeof(sReceiveFishData)); 
		cHeader = (char)0xaa;
		cNumFishHitted = 0; 
	}
	~sReceiveFishData(){}
};

struct sHittedFish_Send
{
	unsigned short	usID;			//2
	unsigned char	ucPlayerID;		//1
	unsigned short	ucBulletValue;	//2
	unsigned int	uiFishOutMoney;	//4
};

struct	sSendFishData
{
	char				cHeader;
	int					iNumFishHitted;	//1
	char				cChecksum;
	sHittedFish_Send	HittedFish[MAX_FISH_DIED_AT_ONE_UPLOAD];
	//
	sSendFishData()
	{ 
		memset( this, 0, sizeof(sSendFishData) );
		cHeader = 0x55; 
	}
	~sSendFishData(){}
};
struct sCompareData_MasterLee;
struct	sSendDataWithMonster
{
	sSendDataWithMonster();
	~sSendDataWithMonster();
	sSendFishData					m_SendData;
	std::vector<sCompareData_MasterLee*>	m_CompareDataList;
	//
	void									Clear( int e_iCount1 );
	void									Copy( sSendDataWithMonster *e_pSendDataWithMonster_MasterLee, int e_iCount1 );
};
class cMonster;
class cPlayerBehaviorBase;
class cBullet;
struct	sCompareData_MasterLee
{
	cMonster				*m_pMonster;
	cPlayerBehaviorBase		*m_pPlayerBehaviorBase;
	cBullet					*m_pBullet;
	std::vector<cMonster*>	m_MiniGameRelatedMonsterVector;
	std::wstring*			m_pstrMiniGameName;
	//
	sCompareData_MasterLee( cMonster *e_pMonster, cPlayerBehaviorBase *e_pPlayerBehaviorBase, cBullet*e_pBullet, const WCHAR *e_strMinigameName );
	sCompareData_MasterLee( sCompareData_MasterLee *e_pCompareData );
	~sCompareData_MasterLee();
};

#endif