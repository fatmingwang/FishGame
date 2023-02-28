#pragma once

#include "../Monster/Monster.h"

//<Monster Type="Normal">
//	<Animation	MPDIList="xxx.mpdi" MPDI="xxxx" >
//	<Physical File="xxx.xml">
//	<AI Moving="xxx.xml">
//</Monster>
//it only has 3 AI,random path,fixed path.2 point move path.


class	cNormalFish : public cMonster
{
protected:
	float					m_fAngle;
	void					InternalInit()override;
	//
	virtual	void			SetTransform();
	//
	c2DImageCollisionData	m_CollisionData;
	//
	cMPDIList*				m_pMPDIList;
	cFAIMachine*			m_pAIMachine;
	//
	cMPDI*					m_pCurrentMPDI;
	cMPDI*					m_pAliveMPDI;
	cMPDI*					m_pHittedMPDI;
	cMPDI*					m_pDyingMPDI;
	cMPDI*					m_pDiedShowMPDI;
	cMPDI*					m_pStruggleMPDI;
	//
	virtual	void			ProcessPhysicalData(TiXmlElement*e_pElement)override;
	void					ProcessAIData(TiXmlElement*e_pElement);
	void					ProcessAnimation(TiXmlElement*e_pElement);
	void					ProcessDiedPrizeAnimation(TiXmlElement*e_pElement);
	void					ProcessShowProbability(TiXmlElement*e_pElement,const WCHAR*e_strFishName,int e_iFishID);
	//if true update speed multiply 10.
	bool					m_bLeaveScene;
	//
	virtual	void			InternalUpdate(float e_fElpaseTime)override;
	int						m_iCollisionMPDITarget;
	//
public:
	cNormalFish( TiXmlElement *e_pElement,const WCHAR*e_strFishName,int e_iFishID);
	cNormalFish( cNormalFish *e_pNormalFish );
	virtual ~cNormalFish();
	//
	CLONE_MYSELF(cNormalFish);
	DEFINE_TYPE_INFO();
	virtual	bool			IsVisible()override;
	virtual	bool			IsInFiled()override;
	virtual	void			Render()override;
	virtual	void			DebugRender()override;
	virtual	void			Destroy();
	virtual	bool			IsCollide(cbtShapeCollision*e_pbtShapeCollision)override;
	virtual	void			LeaveScene()override;
	virtual	void			SetPath(Vector3 e_vStart,Vector3 e_vEndPos,float _fSpeed);
	virtual	void			SetPath()override;
	virtual	void			SetPath(cCurveWithTime*e_pCurveWithTime)override;
	virtual	cCurveWithTime*	GetPath()override;
	bool					IsAIDone();
	virtual	void			RandomPath()override;
	virtual	void			SetPos(Vector3 e_vPos)override;

	//virtual	void			SetPosByImageCenter(Vector3 e_vPos);

	virtual	void			SetAngle(Vector3 e_vAngle)override;
	virtual	Vector3			GetPos()override;
	virtual	Vector3			GetCenterPos()override;
	virtual	cMatrix44		GetTransform()override;
	virtual	Vector3			GetAngle()override;
	virtual	void			CollisionDataCheck()override;
	cFAIMachine*			GetAIMachine(){return m_pAIMachine;}
	virtual	void			MonsterLeaveScene()override;
};
