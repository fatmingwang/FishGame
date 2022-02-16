#ifndef _FREE_MINING_GAME_H_
#define _FREE_MINING_GAME_H_

#include "BaseExtraGame.h"

#include "FallDownNewSpinObject.h"

class	cTrackFalldownWithRock:public cFallDownNewSpinObject
{
	//a rock?
	cBaseImage*	m_pWrongDirectionImage;
	eDirection	Direction[4];
public:
	cTrackFalldownWithRock(int e_iNumObject,Vector3 e_vEndPos,float	e_fSpeed,float	e_fObjectGap,cBaseImage*e_pWrongDirectionImage);
	virtual	void	Render();
	bool	IsPass(eDirection e_eDirection){ return Direction[e_eDirection] == eD_MAX?true:false; }
	void	SetDirection(eDirection*e_peDirection){memcpy(Direction,e_peDirection,sizeof(eDirection)*4); }
};


class	cTrainMovingTrack
{
	cBaseImage*	m_pTrainImage;
	cCurveWithTime	m_CurveWithTime;
public:
	cTrainMovingTrack(cCurveWithTime e_CurveWithTime,cBaseImage*e_pTrainImage)
	{
		m_pTrainImage = e_pTrainImage;
		m_CurveWithTime = e_CurveWithTime;
		m_CurveWithTime.Init();
	}
	~cTrainMovingTrack(){}
	void	Init(){ m_CurveWithTime.Init(); }
	void	SetupMovingData(std::vector<std::vector<eDirection> >e_eDirectionTree);
	void	Update(float e_fElpaseTime)
	{
		m_CurveWithTime.Update(e_fElpaseTime);
	}
	void	Render()
	{
		if( m_pTrainImage )
		{
			m_pTrainImage->SetPos(m_CurveWithTime.GetCurrentPosition());
			m_pTrainImage->Render();
		}
	}
	bool	IsDone(){ return m_CurveWithTime.IsCurveMoveDone(); }
};

//free game 1
class	cFreeMiningGame_Mining:public cBaseExtraGame
{//1,3,5 are constant,2,4 are show object
	cFallDownNewSpinObjectManager	m_FallDownNewSpinObjectManager;
	float							m_fTrackFallDownSpeed;
	float							m_fTrainToDestinationTime;
	//
	cMPDI*							m_pEnteringMPDI;
	cMPDI*							m_pLeavingMPDI1;
	cMPDI*							m_pLeavingMPDI2;
	cMPDIList*						m_pMPDIList;
	int								m_iRestTimeToPlay;
	//
	cMPDI*							m_pTrackIdelMPDI;
	cTrainMovingTrack*				m_pTrainMovingTrack;
	//set result for play
	void							SetupResult();
	//
	virtual	bool					InternalConditionCheck();
public:
	DEFINE_TYPE_INFO();
	cFreeMiningGame_Mining(const WCHAR*e_strConditionName,int e_iConditionCount,float e_fTrackFallDownSpeed,float e_fTrainToDestinationTime);
	~cFreeMiningGame_Mining();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Destroy();
	virtual	void	LoadResource(){}
};




#endif