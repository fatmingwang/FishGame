#pragma once
//
#include "FishNormal.h"
//
//it only has 3 AI,random path,fixed path.2 point move path.







class	cFishMultiCollisionData : public cNormalFish
{
private:
	Frame					m_Frame;
	//
	virtual	void			SetTransform()override;
	//most time is one but some monster are assemble by parts
	struct	sCollisionData
	{
		int						iNumCollisionShape;
		int*					piCollisionTargetIndex;
		cPuzzleImage*			pCollisionPITarget;
		c2DImageCollisionData	CollisionData;
		sCollisionData();
		~sCollisionData();
		sCollisionData*			Clone();
	};
	std::vector<sCollisionData*>	m_CollisionDataList;
	//
	virtual	void			ProcessPhysicalData(TiXmlElement*e_pElement)override;
	virtual	void			InternalUpdate(float e_fElpaseTime)override;
public:
	cFishMultiCollisionData(TiXmlElement*e_pElement, const wchar_t *l_strTypeID ,int e_iFishID);
	cFishMultiCollisionData(cFishMultiCollisionData*e_pMultiCollisionDataFish);
	virtual ~cFishMultiCollisionData();
	CLONE_MYSELF(cFishMultiCollisionData);
	DEFINE_TYPE_INFO();
	virtual	void			DebugRender()override;
	virtual	bool			IsCollide(cbtShapeCollision*e_pbtShapeCollision)override;
	virtual	void			CollisionDataCheck()override;
	virtual	Vector3			GetCenterPos()override { return GetPos(); }
};
