#include "stdafx.h"
#include "FishMultiCollision.h"

//<Monster Type="Normal">
//	<Animation	MPDIList="xxx.mpdi" MPDI="xxxx" >
//	<Physical File="xxx.xml">
//	<AI Moving="xxx.xml">
//	<MonsterData Moving="xxx.xml">
//</Monster>
//
const wchar_t*         cFishMultiCollisionData::TypeID( L"cMultiCollisionDataFish" );
//
cFishMultiCollisionData::sCollisionData*			cFishMultiCollisionData::sCollisionData::Clone()
{
	sCollisionData*l_pCollisionData = new sCollisionData();
	l_pCollisionData->iNumCollisionShape = iNumCollisionShape;
	l_pCollisionData->piCollisionTargetIndex = new int[iNumCollisionShape];
	memset(l_pCollisionData->piCollisionTargetIndex,0,sizeof(int)*l_pCollisionData->iNumCollisionShape);
	l_pCollisionData->pCollisionPITarget = pCollisionPITarget;
	l_pCollisionData->CollisionData.CloneFromList(&CollisionData);
	return l_pCollisionData;
}








cFishMultiCollisionData::sCollisionData::sCollisionData()
{
	iNumCollisionShape = 0;
	piCollisionTargetIndex = 0;	
	pCollisionPITarget = 0;
}








cFishMultiCollisionData::sCollisionData::~sCollisionData()
{
	SAFE_DELETE(piCollisionTargetIndex);
}








cFishMultiCollisionData::cFishMultiCollisionData(TiXmlElement*e_pElement, const WCHAR *l_strTypeID ,int e_iFishID):cNormalFish(e_pElement,l_strTypeID ,e_iFishID)
{
	m_Frame.SetLocalTransform(cMatrix44::Identity);
	m_fRadius = this->m_fRadius;
	sSphere	l_Sphere;
	l_Sphere.Radius = this->m_fRadius;
	cBound	l_Bound;
	l_Bound.SetSphere(l_Sphere);
	this->m_pAIMachine->GetSrcCharacter()->SetBound(&l_Bound);
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		if( !wcscmp(l_strValue,L"Physical") )
		{
			this->ProcessPhysicalData(e_pElement);
			return;
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
	if( m_pAliveMPDI )
		m_pAliveMPDI->SetParent(&m_Frame,false);
	if( m_pHittedMPDI )
		m_pHittedMPDI->SetParent(&m_Frame,false);
	if( m_pDyingMPDI )
		m_pDyingMPDI->SetParent(&m_Frame,false);
	if( m_pDiedShowMPDI )
		m_pDiedShowMPDI->SetParent(&m_Frame,false);
	if( m_pStruggleMPDI )
		m_pStruggleMPDI->SetParent(&m_Frame,false);
}








cFishMultiCollisionData::cFishMultiCollisionData(cFishMultiCollisionData*e_pNormalFish):cNormalFish(e_pNormalFish)
{
	m_fRadius = e_pNormalFish->m_fRadius;
	sSphere	l_Sphere;
	l_Sphere.Radius = this->m_fRadius;
	cBound	l_Bound;
	l_Bound.SetSphere(l_Sphere);
	this->m_pAIMachine->GetSrcCharacter()->SetBound(&l_Bound);

	size_t	l_uiSize = e_pNormalFish->m_CollisionDataList.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		m_CollisionDataList.push_back(e_pNormalFish->m_CollisionDataList[i]->Clone());
	}
	if( m_pAliveMPDI )
		m_pAliveMPDI->SetParent(&m_Frame,false);
	if( m_pHittedMPDI )
		m_pHittedMPDI->SetParent(&m_Frame,false);
	if( m_pDyingMPDI )
		m_pDyingMPDI->SetParent(&m_Frame,false);
	if( m_pDiedShowMPDI )
		m_pDiedShowMPDI->SetParent(&m_Frame,false);
	if( m_pStruggleMPDI )
		m_pStruggleMPDI->SetParent(&m_Frame,false);
}








cFishMultiCollisionData::~cFishMultiCollisionData()
{
	DELETE_VECTOR(m_CollisionDataList,sCollisionData*);
}








void	cFishMultiCollisionData::ProcessPhysicalData(TiXmlElement*e_pElement)
{
	e_pElement = e_pElement->FirstChildElement();
	while(e_pElement)
	{
		cFishMultiCollisionData::sCollisionData*l_pCollisionData = new cFishMultiCollisionData::sCollisionData;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("CollisionFile")
			{
				std::string	l_strFileName = UT::WcharToChar(l_strValue);
				if(!l_pCollisionData->CollisionData.Parse(l_strFileName.c_str()))
				{
					UT::ErrorMsg(l_strFileName.c_str(),"parse failed");
				}
			}
			else
			COMPARE_NAME("TargetPI")
			{
				if( m_pMPDIList )
				{
					l_pCollisionData->pCollisionPITarget = this->m_pMPDIList->GetPuzzleImage(l_strValue);
					if( !l_pCollisionData->pCollisionPITarget )
					{
						UT::ErrorMsg(L"pi not exist",l_strValue);
					}
					int	l_iNum = m_pAliveMPDI->Count();
					for( int i=0;i<l_iNum;++i )
					{
						if( m_pAliveMPDI->GetObject(i)->GetPointDataList()->size() )
						{
							if( m_pAliveMPDI->GetObject(i)->GetPointData(0)->pPI == l_pCollisionData->pCollisionPITarget )
								++l_pCollisionData->iNumCollisionShape;
						}
					}
					if( l_pCollisionData->iNumCollisionShape > 0 )
					{
						l_pCollisionData->piCollisionTargetIndex = new int[l_pCollisionData->iNumCollisionShape];
						memset(l_pCollisionData->piCollisionTargetIndex,0,sizeof(int)*l_pCollisionData->iNumCollisionShape);
					}
				}
			}
		PARSE_NAME_VALUE_END
		this->m_CollisionDataList.push_back(l_pCollisionData);
		e_pElement = e_pElement->NextSiblingElement();
	}
}








bool cFishMultiCollisionData::IsCollide(cbtShapeCollision*e_pbtShapeCollision)
{
	size_t	l_uiSize = m_CollisionDataList.size();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		sCollisionData*l_pCollisionData = m_CollisionDataList[i];
		for( int j=0;j<l_pCollisionData->iNumCollisionShape;++j )
		{
			int	l_iIndex = l_pCollisionData->piCollisionTargetIndex[j];
			if(  l_iIndex>= 0 )
			{
				if(l_pCollisionData->CollisionData.Count()>l_iIndex)
				{
					if(l_pCollisionData->CollisionData[l_iIndex]->Collide(e_pbtShapeCollision))
						return true;
				}
				else
				{
					int a=0;
				}
			}
		}
	}
	return false;
}








void	cFishMultiCollisionData::CollisionDataCheck()
{
	int			l_iCount =	m_pAliveMPDI->Count();
	size_t		l_uiSize =	m_CollisionDataList.size();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		sCollisionData*l_pCollisionData = m_CollisionDataList[i];
		int	l_iIndex = 0;
		for( int j=0;j<l_iCount;++j  )
		{
			std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = m_pAliveMPDI->GetObject(j)->GetPointDataList();
			size_t	l_uiNumPoint = l_pPointList->size();
			for( size_t k=0;k<l_uiNumPoint;++k  )
			{
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pPointList)[k];
				if( l_pTexBehaviorDataWithImageIndexData->pPI == l_pCollisionData->pCollisionPITarget )
				{
					int	l_iImageIndex = l_pTexBehaviorDataWithImageIndexData->iImageIndex;
					if(l_pCollisionData->CollisionData.Count() < l_iImageIndex	)
					{
						UT::ErrorMsg(l_pTexBehaviorDataWithImageIndexData->pPI->GetName(),L"collision data is not match");
					}
				}
			}
		}
	}
}








void	cFishMultiCollisionData::SetTransform()
{
	if( this->m_pCurrentMPDI )
	{
		Vector3	l_vSize = Vector3(m_pAliveMPDI->GetDrawSize().x,m_pAliveMPDI->GetDrawSize().y,0.f)/-2.f;
		m_pCurrentMPDI->SetLocalPosition(l_vSize);
	}
	if( m_pAliveMPDI )
	{
		//this->m_pAliveMPDI->SetPosByImageCenter(m_pAIMachine->GetSrcCharacter()->GetWorldPosition());
		cMatrix44	l_mat =		m_Frame.GetWorldTransform();
		int			l_iCount =	m_pAliveMPDI->Count();
		size_t		l_uiSize =	m_CollisionDataList.size();
		for( size_t i=0;i<l_uiSize;++i  )
		{
			sCollisionData*l_pCollisionData = m_CollisionDataList[i];
			memset(l_pCollisionData->piCollisionTargetIndex,-1,sizeof(int)*l_pCollisionData->iNumCollisionShape);
			int	l_iIndex = 0;
			for( int j=0;j<l_iCount;++j  )
			{
				//m_pAliveMPDI->GetObject(j)->SetAngle(Vector3(0,0,this->m_fAngle));
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_pAliveMPDI->GetObject(j)->GetCurrentPointData();
				if( !m_pAliveMPDI->GetObject(j)->IsCurveMoveDone() && l_pTexBehaviorDataWithImageIndexData->pPI == l_pCollisionData->pCollisionPITarget )
				{
					int	l_iImageIndex = l_pTexBehaviorDataWithImageIndexData->iImageIndex;
					if( l_iImageIndex != -1  )
					{
						l_pCollisionData->piCollisionTargetIndex[l_iIndex] = l_iImageIndex;
						l_pCollisionData->CollisionData[ l_iImageIndex ]->SetTransform(l_mat);
						//l_pCollisionData->CollisionData[ l_iImageIndex ]->SetTransform(m_pCurrentMPDI->GetTransformByImageCenter());
					}
					++l_iIndex;
				}
			}
		}
	}
}








void			cFishMultiCollisionData::DebugRender()
{
	cNormalFish::DebugRender();
	size_t	l_uiSize = m_CollisionDataList.size();
	cMatrix44	l_mat = m_pAliveMPDI->GetTransformByImageCenter();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		sCollisionData*l_pCollisionData = m_CollisionDataList[i];
		for( int j=0;j<l_pCollisionData->iNumCollisionShape;++j )
		{
			if( l_pCollisionData->piCollisionTargetIndex[j] != -1 )
				l_pCollisionData->CollisionData[ l_pCollisionData->piCollisionTargetIndex[j] ]->DebugRender();
		}
	}
}








void cFishMultiCollisionData::InternalUpdate(float e_fElpaseTime)
{
	
	if( this->m_eMonsterStatus == eMS_WAITING_FOR_CLEAN )
	{
		m_pAIMachine->SetCurrentWorkingObject(L"79979");
		return;
	}
	//
	if(m_eMonsterStatus == eMS_ALIVE)
	{
		m_pCurrentMPDI = m_pAliveMPDI;
	}
	else if( m_eMonsterStatus == eMS_HITTED && m_pHittedMPDI )
	{
		m_pCurrentMPDI = m_pHittedMPDI;
	}
	else if( m_eMonsterStatus == eMS_DIED_SHOW )
	{
		if( !this->m_pDiedShowMPDI )
		{
			m_eMonsterStatus = eMS_WAITING_FOR_CLEAN;
		}
		else
		{
			if( m_pCurrentMPDI != m_pDiedShowMPDI )
			{
				m_pCurrentMPDI = m_pDiedShowMPDI;
				this->SetTransform();
			}
			m_pCurrentMPDI->Update(e_fElpaseTime);

			if( m_pDiedShowMPDI->IsAnimationDone() )
			{
				m_eMonsterStatus = eMS_WAITING_FOR_CLEAN;
			}
		}
		return;
	}
			
	if( m_bLeaveScene )
	{
		e_fElpaseTime *= 2.f;
	}
				//
	if( this->m_pAIMachine )
	{
		m_pAIMachine->Update( e_fElpaseTime );
		//
		if( m_pAIMachine->IsDone() )
		{
						
			if( !this->IsVisible() )
			{
				this->m_eMonsterStatus = eMS_WAITING_FOR_CLEAN;
			}
			else
			{
				this->m_pAIMachine->SetCurrentWorkingObject( L"Leave" );
				assert(this->m_pAIMachine->GetCurrentWorkingObject());
			}
			return;
		}
	}
				//
	if( this->m_pCurrentMPDI )
	{
		if( this->m_pAIMachine )
		{
			m_fAngle = D3DXToDegree(m_pAIMachine->GetSrcCharacter()->GetAngle()->z) - 90;
		}
							//
		float	l_fSpeed = m_pAIMachine->GetSrcCharacter()->GetAnimationSpeed() / 200.f; 
		
		if( l_fSpeed>0.f  )
		{
			e_fElpaseTime *= l_fSpeed;

			if( e_fElpaseTime>0.5f )
			{
				e_fElpaseTime = 0.5f;
			}
			else if( e_fElpaseTime<0.016 )
			{
				e_fElpaseTime = 0.016f;
			}
		}
		m_pCurrentMPDI->Update(e_fElpaseTime);
		m_Frame.SetLocalTransform( cMatrix44::TranslationMatrix(*m_pAIMachine->GetSrcCharacter()->GetPosition())*cMatrix44::RotationMatrix(Vector3(0,0,m_fAngle)));
	
 		if( !this->m_HittedHintTC.bTragetTimrReached )
 		{
 			if( !this->m_pHittedMPDI )
 				m_pAliveMPDI->SetCurrentColor(this->m_vHittedHintColor);
 		}

		SetTransform();
	}
}








