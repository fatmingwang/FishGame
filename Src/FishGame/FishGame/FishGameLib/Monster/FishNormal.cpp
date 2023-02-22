#include "stdafx.h"
#include "FishNormal.h"
#include "../Monster/MonsterManager.h"
#include "../GameApp/GameApp.h"
#include "FishGroup.h"

const wchar_t*         cNormalFish::TypeID( L"cNormalFish" );
cNormalFish::cNormalFish(TiXmlElement*e_pElement,const WCHAR*e_strFishName,int e_iFishID)
{	
	m_fAngle = 0.f;
	m_iCollisionMPDITarget = 0;
	m_pCurrentMPDI = 0;
	m_pStruggleMPDI = 0;
	m_pMPDIList = 0;
	m_pAIMachine = 0;
	m_pAliveMPDI = 0;
	m_pDyingMPDI = 0;
	m_pHittedMPDI = 0;
	m_pDiedShowMPDI = 0;
	TiXmlElement*l_pRoot = e_pElement;
	if( l_pRoot )
	{
		l_pRoot = l_pRoot->FirstChildElement();
	}
	while( l_pRoot )
	{
		const WCHAR*l_strValue = l_pRoot->Value();
		COMPARE_VALUE("Physical")
		{
			if( Type() == cNormalFish::TypeID )
				ProcessPhysicalData(l_pRoot);
			else
			{
				int a=0;
			}
		}
		else
		COMPARE_VALUE("AI")
		{
			this->ProcessAIData(l_pRoot);
		}
		else
		COMPARE_VALUE("MonsterData")
		{
			this->ProcessMonsterData(l_pRoot);
		}
		else
		COMPARE_VALUE("Animation")
		{
			this->ProcessAnimation(l_pRoot);
		}
		else
		COMPARE_VALUE("DiedPrizeAnimation")
		{
			ProcessDiedPrizeAnimation(l_pRoot);
		}
		else
		COMPARE_VALUE("ShowProbability")
		{
			ProcessShowProbability(l_pRoot,e_strFishName,e_iFishID);
		}
		l_pRoot = l_pRoot->NextSiblingElement();
	}
	sSphere	l_Sphere;
	l_Sphere.fRadius = this->m_fRadius;
	cBound	l_Bound;
	l_Bound.SetSphere(l_Sphere);
	this->m_pAIMachine->GetSrcCharacter()->SetLocalBound(&l_Bound);
}


cNormalFish::cNormalFish(cNormalFish*e_pNormalFish):cMonster(e_pNormalFish)
{
	m_fAngle = 0.f;
	m_pCurrentMPDI = 0;
	m_pStruggleMPDI = 0;
	m_iCollisionMPDITarget = e_pNormalFish->m_iCollisionMPDITarget;
	this->m_fRadius = e_pNormalFish->m_fRadius;
	m_pAliveMPDI = 0;
	m_pMPDIList = 0;
	m_pAIMachine = 0;
	m_pDyingMPDI = 0;
	m_pHittedMPDI = 0;
	m_pDiedShowMPDI = 0;
	if( e_pNormalFish->m_pAIMachine )
	{
		m_pAIMachine = dynamic_cast<cFAIMachine*>(e_pNormalFish->m_pAIMachine->Clone());
	}
	//
	if( e_pNormalFish->m_pAliveMPDI )
	{
		m_pAliveMPDI = dynamic_cast<cMPDI*>(e_pNormalFish->m_pAliveMPDI->Clone());
		//m_pAliveMPDI->SetScale(this->m_fScale);
	}
	if( e_pNormalFish->m_pDyingMPDI )
	{
		m_pDyingMPDI = dynamic_cast<cMPDI*>(e_pNormalFish->m_pDyingMPDI->Clone());
		//m_pDyingMPDI->SetScale(this->m_fScale);
	}
	if( e_pNormalFish->m_pHittedMPDI )
	{
		m_pHittedMPDI = dynamic_cast<cMPDI*>(e_pNormalFish->m_pHittedMPDI->Clone());
		//m_pHittedMPDI->SetScale(this->m_fScale);
	}
	if( e_pNormalFish->m_pDiedShowMPDI )
	{
		m_pDiedShowMPDI = dynamic_cast<cMPDI*>(e_pNormalFish->m_pDiedShowMPDI->Clone());
		//m_pDiedShowMPDI->SetScale(this->m_fScale);
	}
	if( e_pNormalFish->m_pStruggleMPDI )
	{
		m_pStruggleMPDI = dynamic_cast<cMPDI*>(e_pNormalFish->m_pStruggleMPDI->Clone());
		//m_pStruggleMPDI->SetScale(this->m_fScale);
	}
	m_pMPDIList = e_pNormalFish->m_pMPDIList;
	//fuck
	if( e_pNormalFish->m_CollisionData.Count() )
	{
		m_CollisionData.CloneFromList(&e_pNormalFish->m_CollisionData);
	}
}







cNormalFish::~cNormalFish()
{
	SAFE_DELETE(m_pStruggleMPDI);
	SAFE_DELETE(m_pAliveMPDI);
	SAFE_DELETE(m_pHittedMPDI);
	SAFE_DELETE(m_pDyingMPDI);
	SAFE_DELETE(m_pAIMachine);
	SAFE_DELETE(m_pDiedShowMPDI);
	m_CollisionData.Destroy();
}







void	cNormalFish::CollisionDataCheck()
{
	if( m_pCurrentMPDI && m_CollisionData.Count() )
	{
		size_t	l_uiSize = m_pCurrentMPDI->GetObject(this->m_iCollisionMPDITarget)->GetPointDataList()->size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			int	l_iIndex = m_pCurrentMPDI->GetObject(this->m_iCollisionMPDITarget)->GetPointData((int)i)->iImageIndex;
			if( l_iIndex >= m_CollisionData.Count())
			{
				UT::ErrorMsg(L"collision file not match",m_pMPDIList->GetName());
				return;
			}
		} //next
	}
}

void	cNormalFish::MonsterLeaveScene()
{
	this->m_pAIMachine->SetCurrentWorkingObject( L"Leave" );
	assert(this->m_pAIMachine->GetCurrentWorkingObject());
}


void	cNormalFish::ProcessShowProbability(TiXmlElement*e_pElement,const WCHAR*e_strFishName,int e_iFishID)
{
	cFishApp::m_spMonsterManager->m_FishShowProbabilityList.ProcessShowProbability(e_pElement,e_strFishName,e_iFishID);
}

void	cNormalFish::ProcessPhysicalData(TiXmlElement*e_pElement)
{

	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("CollisionFile")
		{
			std::string	l_strFileName = UT::WcharToChar(l_strValue);
			if(!m_CollisionData.Parse(l_strFileName.c_str()))
			{
				UT::ErrorMsg(l_strFileName.c_str(),"parse failed");
			}
		}
		else
		COMPARE_NAME("CollisionSubMPDI")
		{
			this->m_iCollisionMPDITarget = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
}


void	cNormalFish::ProcessAIData(TiXmlElement*e_pElement)
{
	SAFE_DELETE(m_pAIMachine);
	const WCHAR*l_str = e_pElement->Attribute(L"File");
	std::string	l_strFileName = UT::WcharToChar(l_str);
	//WCHAR*l_strAIMachineName = UT::GetFileNameWithoutFullPath(l_str);
	if( l_str )
	{
		cFAIMachine*l_pFAIMachine = cMonsterManager::m_spFAIBehaviorParser->GetFAIMachineByFileName(l_str);
		if( l_pFAIMachine )
		{
			this->m_pAIMachine = dynamic_cast<cFAIMachine*>(l_pFAIMachine->Clone());
		}
		else
		{
			UT::ErrorMsg(l_str,L"AI error");
		}
	}
}








void	cNormalFish::ProcessAnimation(TiXmlElement*e_pElement)
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("MPDIList")
		{
			m_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
			if( !m_pMPDIList )
			{
				UT::ErrorMsg(L"MPDIList not found",this->GetName());
			}
		}
		else
		COMPARE_NAME("MPDI")
		{
			if( m_pMPDIList )
			{
				cMPDI*l_pMPDI = m_pMPDIList->GetObject(l_strValue);;
				if( l_pMPDI )
				{
					m_pAliveMPDI = new cMPDI(l_pMPDI);
					//m_pAliveMPDI->SetScale(this->m_fScale);
					Vector2	l_vSize = m_pAliveMPDI->GetDrawSize();
					m_fRadius = l_vSize.x>l_vSize.y?l_vSize.x:l_vSize.y;
					m_fRadius /= 2.f;
				}
				else
				{
					UT::ErrorMsg(L"MPDI not found",l_strValue);
				}
			}
		}
		else
		COMPARE_NAME("Dying")
		{
			if( m_pMPDIList )
			{
				cMPDI*l_pMPDI = m_pMPDIList->GetObject(l_strValue);;
				if( l_pMPDI )
				{
					m_pDyingMPDI = new cMPDI(l_pMPDI);
					//m_pDyingMPDI->SetScale(this->m_fScale);
				}
				else
				{
					UT::ErrorMsg(L"MPDI not found",l_strValue);
				}
			}
		}
		else
		COMPARE_NAME("Hitted")
		{
			if( m_pMPDIList )
			{
				cMPDI*l_pMPDI = m_pMPDIList->GetObject(l_strValue);;
				if( l_pMPDI )
				{
					m_pHittedMPDI = new cMPDI(l_pMPDI);
					//m_pHittedMPDI->SetScale(this->m_fScale);
				}
				else
				{
					UT::ErrorMsg(L"MPDI not found",l_strValue);
				}
			}
		}
		else
		COMPARE_NAME("DiedShow")
		{
			if( m_pMPDIList )
			{
				cMPDI*l_pMPDI = m_pMPDIList->GetObject(l_strValue);;
				if( l_pMPDI )
				{
					m_pDiedShowMPDI = new cMPDI(l_pMPDI);
				}
				else
				{
					UT::ErrorMsg(L"MPDI not found",l_strValue);
				}
			}
		}
		else
		COMPARE_NAME("Struggle")
		{
			if( m_pMPDIList )
			{
				cMPDI*l_pMPDI = m_pMPDIList->GetObject(l_strValue);;
				if( l_pMPDI )
				{
					m_pStruggleMPDI = new cMPDI(l_pMPDI);
				}
				else
				{
					UT::ErrorMsg(L"MPDI not found",l_strValue);
				}
			}
		}
	PARSE_NAME_VALUE_END
}
//<DiedPrizeAnimation MPDILIst="aabb.mpdi" MPDI="aa" Count="100" />
void	cNormalFish::ProcessDiedPrizeAnimation(TiXmlElement*e_pElement)
{
	cMPDIList*l_pMPDILIst = 0;
	cMPDI*l_pMPDI = 0;
	int	l_iCount = 100;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("MPDILIst")
		{
			l_pMPDILIst = cGameApp::GetMPDIListByFileName(l_strValue);
			if( !l_pMPDILIst )
			{
				UT::ErrorMsg(l_strValue,L"not exists");
			}
		}
		else
		COMPARE_NAME("MPDI")
		{
			l_pMPDI = l_pMPDILIst->GetObject(l_strValue);
			m_strDiedPrizeAnimationName = l_strValue;
			if( !l_pMPDI )
			{
				UT::ErrorMsg(l_strValue,L"not exists");
			}
		}
		else
		COMPARE_NAME("Count")
		{
			l_iCount = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
	if( !l_pMPDI )
	{
		UT::ErrorMsg(L"AddFishDiedPrizeAnimation",L"failed");
	}
	else
	{
		cFishApp::m_spMonsterManager->AddFishDiedPrizeAnimation(l_pMPDI,l_iCount);
	}
}







bool	cNormalFish::IsVisible()
{
	Vector4	l_vNewWall(-m_fRadius,-m_fRadius,cGameApp::m_spOpenGLRender->m_vGameResolution.x+m_fRadius,cGameApp::m_spOpenGLRender->m_vGameResolution.y+m_fRadius);
	Vector3	l_vPos = this->m_pAIMachine->GetSrcCharacter()->GetWorldPosition();
	return l_vNewWall.CollidePoint(l_vPos.x,l_vPos.y);
}








void cNormalFish::InternalInit()
{
	if( m_pAliveMPDI )
		m_pAliveMPDI->Init();
	if( m_pHittedMPDI )
		m_pHittedMPDI->Init();
	if( m_pDyingMPDI )
		m_pDyingMPDI->Init();
	if( this->m_pDiedShowMPDI )
		m_pDiedShowMPDI->Init();
	if( this->m_pStruggleMPDI )
		m_pStruggleMPDI->Init();
	m_pCurrentMPDI = m_pAliveMPDI;
	//while normal fish is required, AI will be set status.
	if( this->m_pAIMachine )
	{
		m_pAIMachine->Init();
	}
	m_vHittedHintColor = Vector4::One;
	this->SetTransform();
	m_bLeaveScene = false;
}

bool	cNormalFish::IsInFiled()
{
	Vector3	l_vPos = m_pAIMachine->GetSrcCharacter()->GetWorldPosition();
	float	l_fRadius = m_fRadius;
	RECT	l_rc = { (int)(l_vPos.x-l_fRadius),
					 (int)(l_vPos.y-l_fRadius),
					 (int)(l_vPos.x+l_fRadius),
					 (int)(l_vPos.y+l_fRadius) };
	if( l_rc.left>0&&l_rc.top>0&&l_rc.right < cGameApp::m_spOpenGLRender->m_vGameResolution.x && l_rc.bottom < cGameApp::m_spOpenGLRender->m_vGameResolution.y )
		return true;
	return false;
}








void cNormalFish::InternalUpdate( float e_fElpaseTime )
{

	if( this->m_eMonsterStatus == eMS_WAITING_FOR_CLEAN )
	{
		m_pAIMachine->SetCurrentWorkingObject( L"79979" );
		return;
	}
	//
	if( m_eMonsterStatus == eMS_ALIVE  )
	{
		if(this->m_fRestProbabilityValueToDied <=0.8f && m_pDyingMPDI)
		{
			m_pCurrentMPDI = m_pDyingMPDI;
		}
		else
		{
			m_pCurrentMPDI = m_pAliveMPDI;
		}
	}
	else
	if( m_eMonsterStatus == eMS_HITTED && m_pHittedMPDI )
	{
		m_pCurrentMPDI = m_pHittedMPDI;
	}
	else
	if( m_eMonsterStatus == eMS_STRUGGLE && m_pStruggleMPDI )
	{
		m_pCurrentMPDI = m_pStruggleMPDI;
	}
	else
	if( m_eMonsterStatus == eMS_DIED_SHOW )
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
			if( e_fElpaseTime <= 0.f )
				e_fElpaseTime = cGameApp::m_sTimeAndFPS.fElpaseTime;
			if( m_pCurrentMPDI->GetCurrentProgress() > 0.5f )
				m_pCurrentMPDI->Update(e_fElpaseTime*0.3f);
			else
				m_pCurrentMPDI->Update(e_fElpaseTime);
			SetTransform();
			if( m_pDiedShowMPDI->IsAnimationDone() )
			{
				m_eMonsterStatus = eMS_WAITING_FOR_CLEAN;
			}
		}
		//
		return;
	}
	//
//	else
//	if( m_eMonsterStatus == eMS_DYING && m_pDyingMPDI )
//		m_pCurrentMPDI = m_pDyingMPDI;
	if( m_bLeaveScene )
	{
		e_fElpaseTime *= 2.f;
	}
	//
	if( this->m_pAIMachine )
	{
		if(m_eMonsterStatus == eMS_STRUGGLE)
		{
			m_pAIMachine->Update( e_fElpaseTime/3.f );
		}
		else
		{
			m_pAIMachine->Update( e_fElpaseTime );
		}
		//
		if( m_pAIMachine->IsDone() )
		{
			if(this->m_fAliveTime <0.019f)
			{//bug?fish died immediately.	
			}
			//
			if( !this->IsVisible() &&  this->m_eMonsterStatus == eMS_ALIVE)
			{
				this->m_eMonsterStatus = eMS_WAITING_FOR_CLEAN;
			}
			else
			{
				this->m_pAIMachine->SetCurrentWorkingObject( L"Leave" );
				assert(this->m_pAIMachine->GetCurrentWorkingObject());
			}

			
			//
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
		//if( l_fSpeed>0.f && e_fElpaseTime>0.f )
		if( l_fSpeed >= 0.f  )
		{
			e_fElpaseTime *= l_fSpeed;
			
			if( e_fElpaseTime>0.5f )
			{
				e_fElpaseTime = 0.5f;
			}
			else 
			if( e_fElpaseTime<0.008f )
			{
				e_fElpaseTime = 0.008f;
			}
		}
		//
		if(m_eMonsterStatus == eMS_STRUGGLE)
		{
			m_pCurrentMPDI->Update(e_fElpaseTime);
		}
		else
		{
			m_pCurrentMPDI->Update(e_fElpaseTime);
		}

		if( !this->m_HittedHintTC.bTragetTimrReached )
		{
			if( !this->m_pHittedMPDI )
			{
				m_pAliveMPDI->SetCurrentColor(this->m_vHittedHintColor);
			}
		}

		//if(m_pAliveMPDI->GetObject(m_iCollisionMPDITarget)->IsAnimationDone())
		//{
		//	int a=0;
		//}

		SetTransform();
	}
}








void cNormalFish::Render()
{
	if( this->m_pAIMachine )
	{
		m_pAIMachine->Render();
	}
	//
	if( m_pCurrentMPDI )
	{
		//if(!this->IsVisible())
		//if( !this->IsInFiled() )
			m_pCurrentMPDI->Render();
	}
}








void	cNormalFish::DebugRender()
{
	if( this->m_pAIMachine && this->m_pCurrentMPDI )
	{
		Vector2	l_vSize = m_pAliveMPDI->GetDrawSize();
		Vector3 l_vPos = m_pAliveMPDI->GetPos();
		l_vPos.x += l_vSize.x/2.f;
		l_vPos.y += l_vSize.y/2.f;
		std::wstring	l_str = UT::ComposeMsgByFormat(L"%ls,Pos:%d,%d,%d",this->GetName(),(int)l_vPos.x,(int)l_vPos.y,(int)l_vPos.z);
		//cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y,l_str.c_str());
		//m_pAIMachine->DebugRender();
		int	l_iImageIndex = m_pCurrentMPDI->GetObject(m_iCollisionMPDITarget)->GetCurrentPointData()->iImageIndex;
		if( m_CollisionData.Count() > l_iImageIndex && l_iImageIndex != -1 )
		{
			m_CollisionData[l_iImageIndex]->DebugRender();
		}
		//cCurveWithTime*l_pPath = this->GetPath();
		//if(l_pPath )	l_pPath->Render();
		if(this->IsVisible())
		{
			//GLRender::RenderCircle(Vector2(l_vPos.x,l_vPos.y),m_fRadius);
			cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y,this->GetName());
		}
	}
}








void cNormalFish::Destroy()
{

}

//======================================================
//
//
//
//======================================================


bool cNormalFish::IsCollide(cbtShapeCollision*e_pbtShapeCollision)
{
	if( this->m_pCurrentMPDI )
	{
		if( m_pCurrentMPDI->Count() )
		{
			int	l_iImageIndex = m_pCurrentMPDI->GetObject(m_iCollisionMPDITarget)->GetCurrentPointData()->iImageIndex;
			if( l_iImageIndex != -1 && m_CollisionData[l_iImageIndex]->Collide(e_pbtShapeCollision))
			{
				return true;
			}
		}
	}
	return false;
}








void cNormalFish::LeaveScene()
{
	m_bLeaveScene = true;
}








cCurveWithTime*		cNormalFish::GetPath()
{
	cFAIBaseBehave*l_pFAIBaseBehave = m_pAIMachine->GetCurrentWorkingObject();
	if( l_pFAIBaseBehave )
	{
		if( l_pFAIBaseBehave->Type() == cFAIFiledPathBehavior::TypeID)
		{
			return ((cFAIFiledPathBehavior*)l_pFAIBaseBehave)->GetPath();
		}
		else if( l_pFAIBaseBehave->Type() == cFAIPathBehavior::TypeID)
		{
			return ((cFAIPathBehavior*)l_pFAIBaseBehave)->GetPath();	
		}
	}
	return 0;
}








void	cNormalFish::SetPath()
{
	if(this->m_pAIMachine)
	{
		//m_pAIMachine->Init();
		m_pAIMachine->SetCurrentWorkingObject(L"Path");
		//m_pAIMachine->SetCurrentWorkingObject(L"Leave");
		this->m_eMonsterStatus = eMS_ALIVE;
		SetTransform();
	}
}








void	cNormalFish::SetPath(cCurveWithTime*e_pCurveWithTime)
{
	cFAIBaseBehave*l_pFAIBaseBehave = m_pAIMachine->GetCurrentWorkingObject();
	if( l_pFAIBaseBehave )
	{
		if( l_pFAIBaseBehave->Type() == cFAIFiledPathBehavior::TypeID)
		{
			((cFAIFiledPathBehavior*)l_pFAIBaseBehave)->SetPath(e_pCurveWithTime);
			((cFAIFiledPathBehavior*)l_pFAIBaseBehave)->GetPath()->Init();
			return;
		}
		else if( l_pFAIBaseBehave->Type() == cFAIPathBehavior::TypeID)
		{
			((cFAIPathBehavior*)l_pFAIBaseBehave)->SetPath(e_pCurveWithTime);
			((cFAIPathBehavior*)l_pFAIBaseBehave)->GetPath()->Init();
			return;
		}
		else
		{//current AI behavior is not path,why set a path here?
			int a=0;
		}
	}
}








void	cNormalFish::SetPath(Vector3 e_vStart,Vector3 e_vEndPos,float e_fSpeed)
{
	if(this->m_pAIMachine)
	{
		cFAIBaseBehave*l_pFAIBaseBehave = m_pAIMachine->GetObject(L"Walk");
		m_pAIMachine->GetSrcCharacter()->SetWorldPosition(e_vStart);
		m_pAIMachine->GetSrcCharacter()->SetSpeed(e_fSpeed);
		if( l_pFAIBaseBehave )
		{
			cFAIWalkToDestinationAIBehavior*l_pFAIWalkToDestinationAIBehavior = (cFAIWalkToDestinationAIBehavior*)l_pFAIBaseBehave;
			if( l_pFAIWalkToDestinationAIBehavior )
			{
				l_pFAIWalkToDestinationAIBehavior->SetDestPos(e_vEndPos);
				m_pAIMachine->SetCurrentWorkingObject(0);
				SetTransform();
				this->m_eMonsterStatus = eMS_ALIVE;
			}
		}
	}
}








bool	cNormalFish::IsAIDone()
{
	if(this->m_pAIMachine)
	{
		return m_pAIMachine->IsDone();
	}
	return true;
}








void	cNormalFish::SetPos(Vector3 e_vPos)
{
	this->m_pAIMachine->GetSrcCharacter()->SetWorldPosition(e_vPos);
	this->SetTransform();
}

void	cNormalFish::SetAngle(Vector3 e_vAngle)
{
	m_pAliveMPDI->SetAngle( e_vAngle );
	this->SetTransform();
}


Vector3	cNormalFish::GetCenterPos()
{
	//return m_pCurrentMPDI->GetObject(0)->GetRenderMatrix().GetTranslation();
	return m_pCurrentMPDI->GetObject(0)->GetWorldPosition();
}







Vector3	cNormalFish::GetPos()
{
	return this->m_pAIMachine->GetSrcCharacter()->GetWorldPosition();
}








cMatrix44	cNormalFish::GetTransform()
{
	//return m_pCurrentMPDI->GetTransformByImageCenter();
	return m_pCurrentMPDI->GetWorldTransform();
	//return this->m_pAliveMPDI->GetTransformByImageCenter();
}








Vector3		cNormalFish::GetAngle()
{
	return Vector3(0, 0, m_fAngle);
	//return m_pCurrentMPDI->GetAngle();
	//return this->m_pAliveMPDI->GetAngle();
}








void	cNormalFish::RandomPath()
{
	Vector3	l_vStartPos = Vector3(-150,frand(0,cGameApp::m_spOpenGLRender->m_vGameResolution.y),0);
	Vector3	l_vEndPos = Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x+10,frand(0,cGameApp::m_spOpenGLRender->m_vGameResolution.y),0);
	SetPath(l_vStartPos,l_vEndPos,frand(100,300));
	SetTransform();
	this->m_eMonsterStatus = eMS_ALIVE;
}








void	cNormalFish::SetTransform()
{
	if( m_pCurrentMPDI )
	{
		//this->m_pCurrentMPDI->SetPosByImageCenter(m_pAIMachine->GetSrcCharacter()->GetWorldPosition());
		
		assert(m_pCurrentMPDI->Count()>=m_iCollisionMPDITarget);
		if(m_pCurrentMPDI->Count())
		{
			int	l_iImageIndex = m_pCurrentMPDI->GetObject(m_iCollisionMPDITarget)->GetCurrentPointData()->iImageIndex;
			Vector2	l_vSize = m_pCurrentMPDI->GetDrawSize() / 2.f;
			auto l_vPos = m_pAIMachine->GetSrcCharacter()->GetWorldPosition();
			m_pCurrentMPDI->SetDoPositionOffsetToCenter(true);
			cMatrix44 l_mat = cMatrix44::TranslationMatrix(l_vPos) * cMatrix44::RotationMatrix(Vector3(0, 0, m_fAngle)) * cMatrix44::ScaleMatrix(Vector3(this->m_fScale, this->m_fScale, this->m_fScale));
			this->m_pCurrentMPDI->SetLocalTransform(l_mat);
			if( m_CollisionData.Count() <= l_iImageIndex || l_iImageIndex == -1 )
			{
	#ifdef DEBUG
				OutputDebugString(m_pCurrentMPDI->GetName());
				OutputDebugString(L"Collision data not match");
	#endif
			}
			else
			{
				if( this->m_eMonsterStatus == eMS_DIED_SHOW )
				{
					return;
				}
				//if( cBulletManager::m_sbCollideSkip)
				//	return;
				m_CollisionData[l_iImageIndex]->SetTransform(l_mat);
			}
		}
	}
}
















