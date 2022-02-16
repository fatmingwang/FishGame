#ifndef _FREE_GAME_H_
#define _FREE_GAME_H_

#include "FallDownNewSpinObject.h"
#include "BaseExtraGame.h"
#include "WinMoneySpinObjectMPDI.h"
#include "JPEffect.h"
class	cWinmoneyLines;
class	cFreeMiningGame_Mining;
class	cFreeGame;
class	cPickRockBonusGame;
//
//Erase win money line's Spin Object And Fall Down New Spin Object
//to filled the empty spin object
//
class	cFreeExplosionGame:public cBaseExtraGame
{
	void							WinLocalJP();
	//
	int								m_iCurrentWinMoney;
	std::vector<int>				m_iCurrentLineWinMoneyList;
	int								m_iAddWinMoney;
	//
	friend class cExtraGamePhase;
	cFallDownNewSpinObjectManager	m_FallDownNewSpinObject;
	//
	cWinmoneyLines*					m_pWinmoneyLines;
	cMPDIList*						m_pResourceMPDIList;
	cMPDI*							m_pExploreMPDI;
	//
	cNumerialImage*					m_pNumerialImage;
	//cSubMPDI*						m_ExplosionCountPosition;
	//
	cWinMoneySpinObjectMPDI*		m_pWinMoneySpinObjectMPDI;
	//invisible original spin object,and play a MPDI,then spin new spin object,and check data again.
	void							CheckResultAndSetNewSpinObject(std::vector<std::wstring> e_NewResult);
	//if object is explosion give it a new result
	std::vector<std::wstring>		GetNextExplosionResult();
	//how many time explosion
	int								m_iExplosionCount;
	//while m_iExplosionCount is reached value of m_iExplosionCount,setop new explosion
	int								m_iMaxAllowExplosionCount;
	int								m_iExtraBonusForReachedMaxAllowExplosionCountMin;
	int								m_iExtraBonusForReachedMaxAllowExplosionCountMax;
	//cMPDI*							m_pWinBigMoneyForReachedMaxAllowExplosionCount;
	cJPEffect*						m_pJPEffect;
	//
	cMPDI*							m_pConstantUI;
	float							m_fGameSpeed;
	//
	bool							m_bWinJP;
	cMPDI*							m_pJPMPDI;
	int								m_iFinalJPWinMoney;
	int								m_iStartJPWinMoney;
	int								m_iJPMoney;
public:
	DEFINE_TYPE_INFO();
	cFreeExplosionGame(const WCHAR*e_strConditionName,int e_iConditionCount,float e_fSpeed,cMPDI*e_pConstantUI,int e_TenComboExtraMoneyMin,int e_TenComboExtraMoneyMax,float e_fGameSpeed);
	~cFreeExplosionGame();
	virtual	bool	InternalConditionCheck();
	virtual	void	LoadResource();
	virtual	void	Destroy();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};

#endif