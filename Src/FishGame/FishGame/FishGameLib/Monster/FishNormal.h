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
	void					InternalInit();
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
	virtual	void			ProcessPhysicalData(TiXmlElement*e_pElement);
	void					ProcessAIData(TiXmlElement*e_pElement);
	void					ProcessAnimation(TiXmlElement*e_pElement);
	void					ProcessDiedPrizeAnimation(TiXmlElement*e_pElement);
	void					ProcessShowProbability(TiXmlElement*e_pElement,const WCHAR*e_strFishName,int e_iFishID);
	//if true update speed multiply 10.
	bool					m_bLeaveScene;
	//
	virtual	void			InternalUpdate(float e_fElpaseTime);
	int						m_iCollisionMPDITarget;
	//
public:
	cNormalFish( TiXmlElement *e_pElement,const WCHAR*e_strFishName,int e_iFishID);
	cNormalFish( cNormalFish *e_pNormalFish );
	virtual ~cNormalFish();
	//
	CLONE_MYSELF(cNormalFish);
	DEFINE_TYPE_INFO();
	virtual	bool			IsVisible();
	virtual	bool			IsInFiled();
	virtual	void			Render();
	virtual	void			DebugRender();
	virtual	void			Destroy();
	virtual	bool			IsCollide(cbtShapeCollision*e_pbtShapeCollision);
	virtual	void			LeaveScene();
	void					SetPath(Vector3 e_vStart,Vector3 e_vEndPos,float _fSpeed);
	void					SetPath();
	virtual	void			SetPath(cCurveWithTime*e_pCurveWithTime);
	virtual	cCurveWithTime*	GetPath();
	bool					IsAIDone();
	virtual	void			RandomPath();
	virtual	void			SetPos(Vector3 e_vPos);

	//virtual	void			SetPosByImageCenter(Vector3 e_vPos);

	virtual	void			SetAngle(Vector3 e_vAngle);
	virtual	Vector3			GetPos();
	virtual	Vector3			GetCenterPos();
	virtual	cMatrix44		GetTransform();
	virtual	Vector3			GetAngle();
	virtual	void			CollisionDataCheck();
	cFAIMachine*			GetAIMachine(){return m_pAIMachine;}
	virtual	void			MonsterLeaveScene();
};
