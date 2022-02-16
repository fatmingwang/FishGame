#include "stdafx.h"
#include "NumerialRollingEffect.h"

cFixedRollingObject::cFixedRollingObject(cFixedRollingObjectManager*e_pNumerialObjectManager,cBaseImage*e_pBaseImage,Vector3 e_vPos,float e_fCurrentPos)
:cBaseImage(e_pBaseImage)
{
	m_pNumerialObjectManager = e_pNumerialObjectManager;
	m_fCurrentPos = e_fCurrentPos;
	*m_pvPos = e_vPos;
	m_pSpinSound = 0;
	//m_pSpinSound = cGameApp::m_spSoundParser->GetObject(SPIN_SOUND_NAME);
}

cFixedRollingObject::~cFixedRollingObject()
{

}

void	cFixedRollingObject::Update(float e_fElpaseTime)
{
	RelativeMove(this->m_pNumerialObjectManager->m_fCurrentSpeed*e_fElpaseTime);
}

void	cFixedRollingObject::Render()
{
	cBaseImage::Render();
}

void	cFixedRollingObject::DebugRender()
{

}

bool	cFixedRollingObject::IsAtShowArea()
{
	Vector2	l_vPos1(m_pNumerialObjectManager->m_vShowArea.x,m_fCurrentPos);
	Vector2	l_vPos2(m_pNumerialObjectManager->m_vShowArea.x,m_fCurrentPos+*this->GetHeight());
	if(m_pNumerialObjectManager->m_vShowArea.CollidePoint(l_vPos1.x,l_vPos1.y)||
		m_pNumerialObjectManager->m_vShowArea.CollidePoint(l_vPos2.x,l_vPos2.y))
	{
		//this->m_vPos.y = m_fCurrentPos;
		return true;
	}
	//return false;
	return true;
}

void	cFixedRollingObject::RelativeMove(float e_fDis)
{
	float	l_fNewPos = m_fCurrentPos+e_fDis;

	float	l_fRlativePosToEnd = l_fNewPos-this->m_pNumerialObjectManager->m_fAllSpinObjectLength;
	if( l_fRlativePosToEnd>0.f )
	{
		SetCurrentPos(l_fRlativePosToEnd);
		//if( m_pSpinSound )
		//	m_pSpinSound->Play(true);
	}
	else
	{
		SetCurrentPos(l_fNewPos);
	}
}

void	cFixedRollingObject::SetCurrentPos(float e_fPos)
{
	m_fCurrentPos = e_fPos;
	this->m_pvPos->y = m_pNumerialObjectManager->m_vShowArea.y+m_fCurrentPos-m_pNumerialObjectManager->m_fSpinObjectIndex_0_OffsetStartPos;
}

void	cFixedRollingObject::ChangeImage(cBaseImage*e_pBaseImage)
{
	this->SetName(e_pBaseImage->GetName());
	SetUV(e_pBaseImage->GetUV());
}

int	cFixedRollingObjectManager::m_siResultSpinObjectIndex = 1;
cFixedRollingObjectManager::cFixedRollingObjectManager(Vector4 e_vShowArea,float	e_fSpinTime,float	e_fLeastSpinDis)
{
	m_pAllRollingObjectPI = 0;
	m_fLeastSpinDis = e_fLeastSpinDis;
	m_fSpinObjectIndex_0_OffsetStartPos = 0.f;
	m_vShowArea = e_vShowArea;
	m_SpinTime.SetTargetTime(e_fSpinTime);
	m_fTargetSpinDis = 0.f;
	memset(m_ShowObjectIndex,-1,sizeof(int)*MAX_VISBILE_OBJECT);
	m_iResultIndexToStopAtTopShow = -1;
}

cFixedRollingObjectManager::~cFixedRollingObjectManager()
{
}

void	cFixedRollingObjectManager::SetupData(LineData e_SlotLineData,cPuzzleImage*e_pPI)
{
	m_pAllRollingObjectPI = e_pPI;
	//========================================================
	this->Destroy();
	//from 0 to 9
	int	l_iCount = 10;
	if( l_iCount )
	{
		float	l_fImageHeight = (float)*e_pPI->GetObject(0)->GetHeight();
		m_fSpinObjectIndex_0_OffsetStartPos = l_fImageHeight;
		m_fAllSpinObjectLength = (l_iCount)*l_fImageHeight;
		Vector3	l_vPos(m_vShowArea.x,m_vShowArea.y,0.f);
		for( int i=0;i<l_iCount;++i )
		{
			l_vPos.y = m_vShowArea.y+(l_fImageHeight*i)-m_fSpinObjectIndex_0_OffsetStartPos;
			const WCHAR*l_strPIUnitName = e_SlotLineData[i].c_str();
			cFixedRollingObject*l_p = new cFixedRollingObject(this,e_pPI->GetObject(l_strPIUnitName),l_vPos,l_vPos.y);
			this->AddObjectNeglectExist(l_p);
		}
	}
}

void	cFixedRollingObjectManager::StartSpinData(LineData e_strResult)
{
	int	l_iSize = (int)e_strResult.size();
	int	l_iCount = this->Count();
	if(l_iCount == 0 )
		return;
	const int	l_iOffsetIndex = 2;
	assert(l_iCount-l_iSize>=l_iSize&&"slot object's count is too small");
	for( int i=0;i<l_iSize;++i )
	{
		const WCHAR*l_strNmae = e_strResult[i].c_str();
		int	l_iTargetIndex = UT::GetLoopIndex(this->m_iResultIndexToStopAtTopShow-i-l_iOffsetIndex,l_iCount);
		cFixedRollingObject*l_pOriginalSlotObject = this->GetObject(l_iTargetIndex);
		if( wcscmp(this->GetObject(l_iTargetIndex)->GetName(),e_strResult[i].c_str()) )
		{
			Vector3 l_vPos = l_pOriginalSlotObject->GetPos();
			float	l_fCurrentPos = l_pOriginalSlotObject->GetCurrentPos();
			cFixedRollingObject*l_pSlotObject = new cFixedRollingObject(this,m_pAllRollingObjectPI->GetObject(l_strNmae),l_vPos,l_fCurrentPos);
			this->m_ObjectList[l_iTargetIndex] = l_pSlotObject;
			delete l_pOriginalSlotObject;
		}
	}
	int	l_iResultIndexToStopAtTopShow = UT::GetLoopIndex(this->m_iResultIndexToStopAtTopShow-l_iSize-1,l_iCount);
	StartSpinData(l_iResultIndexToStopAtTopShow);
}

void	cFixedRollingObjectManager::StartSpinData(int e_iStopWorkingIndex)
{
	int	l_iCount = this->Count();
	if(l_iCount == 0 )
		return;
	float	l_fOneImageHeight = (float)*this->GetObject(0)->GetHeight();
	float	l_fResultPos = m_siResultSpinObjectIndex*l_fOneImageHeight;
	float	l_fCurrentResultPos = GetObject(e_iStopWorkingIndex)->GetCurrentPos();
	float	l_fNormalSpinPos = l_fCurrentResultPos+this->m_fLeastSpinDis;
	l_fNormalSpinPos = UT::GetFloatModulus(l_fNormalSpinPos,m_fAllSpinObjectLength);
	float	l_fOffset = l_fResultPos-l_fNormalSpinPos;
	if( l_fOffset<0.f )
		this->m_fTargetSpinDis = m_fLeastSpinDis+(m_fAllSpinObjectLength-l_fNormalSpinPos+l_fResultPos);
	else
	{
		this->m_fTargetSpinDis = m_fLeastSpinDis+(l_fResultPos-l_fNormalSpinPos);
	}
	m_iResultIndexToStopAtTopShow = e_iStopWorkingIndex;
	this->m_fCurrentSpeed = this->m_fTargetSpinDis/this->m_SpinTime.fTargetTime;
	m_SpinTime.Start();
}

void	cFixedRollingObjectManager::AdjustOffsetPosition()
{
	int		l_iCount = this->Count();
	if( l_iCount <1 )
		return;
	int	l_iWorkingCount = 0;
	float	l_fImageHeight = (float)*GetObject(0)->GetHeight();
	int	l_iStartIndex = UT::GetLoopIndex(m_iResultIndexToStopAtTopShow-1,l_iCount);
	for( int i=0;i<l_iCount;++i )
	{
		int	l_iTargetIndex = UT::GetLoopIndex(l_iStartIndex,l_iCount);
		this->m_ObjectList[l_iTargetIndex]->SetCurrentPos(l_fImageHeight*i);
		if(this->m_ObjectList[l_iTargetIndex]->IsAtShowArea())
		{
			m_ShowObjectIndex[l_iWorkingCount] = l_iTargetIndex;
			++l_iWorkingCount;
		}
		++l_iStartIndex;
	}
}

void	cFixedRollingObjectManager::UpdateSpinVisibleResult(float e_fElpaseTime)
{
	memset(m_ShowObjectIndex,-1,sizeof(int)*MAX_VISBILE_OBJECT);
	int	l_iWorkingCount = 0;
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		this->m_ObjectList[i]->Update(e_fElpaseTime);
		if(this->m_ObjectList[i]->IsAtShowArea())
		{
			m_ShowObjectIndex[l_iWorkingCount] = i;
			++l_iWorkingCount;
		}
	}
}

void	cFixedRollingObjectManager::Init()
{
	AdjustOffsetPosition();
}

void	cFixedRollingObjectManager::Update(float e_fElpaseTime)
{
	m_SpinTime.Update(e_fElpaseTime);
	if(!m_SpinTime.bTragetTimrReached)
	{
		UpdateSpinVisibleResult(e_fElpaseTime);
	}
	else
	{//adjust position to correct to minus position error
		AdjustOffsetPosition();
		//UpdateSpinVisibleResult(e_fElpaseTime);
	}
}

void	cFixedRollingObjectManager::Render()
{
	Vector4	l_vScissor = ViewRectToOpenGLScissor(m_vShowArea);
	glScissor((int)l_vScissor.x,(int)l_vScissor.y,(int)l_vScissor.z,(int)l_vScissor.w);
	int	l_iWorkingCount = 0;
	while( m_ShowObjectIndex[l_iWorkingCount] != -1 )
	{
		if( m_ShowObjectIndex[l_iWorkingCount] == this->m_iResultIndexToStopAtTopShow )
		{
			this->m_ObjectList[m_ShowObjectIndex[l_iWorkingCount]]->Render();
		}
		else
			this->m_ObjectList[m_ShowObjectIndex[l_iWorkingCount]]->Render();
		++l_iWorkingCount;
	}
}

void	cFixedRollingObjectManager::DebugRender()
{

}

bool	cFixedRollingObjectManager::IsSpinedDone()
{
	return m_SpinTime.bTragetTimrReached;
}

float	cFixedRollingObjectManager::GetRestTime()
{
	return m_SpinTime.fRestTime;
}

void	cFixedRollingObjectManager::ReplaceObject(int e_iIndex,WCHAR*e_strNewObjectName)
{
	cFixedRollingObject*l_pOriginalSlotObject = this->GetObject(e_iIndex);
	if( wcscmp(this->GetObject(e_iIndex)->GetName(),e_strNewObjectName) )
	{
		Vector3 l_vPos = l_pOriginalSlotObject->GetPos();
		float	l_fCurrentPos = l_pOriginalSlotObject->GetCurrentPos();
		cFixedRollingObject*l_pSlotObject = new cFixedRollingObject(this,m_pAllRollingObjectPI->GetObject(e_strNewObjectName),l_vPos,l_fCurrentPos);
		this->m_ObjectList[e_iIndex] = l_pSlotObject;
		delete l_pOriginalSlotObject;
	}
}

const int	g_iNumNumerial = 10;

cFixedRollingObjectManagerList::cFixedRollingObjectManagerList(char*e_strPIName,Vector2 e_vRenderPos)
{
	std::wstring	l_str = UT::CharToWchar(e_strPIName);
	m_pAllRollingObjectPI = cGameApp::GetPuzzleImageByFileName(l_str.c_str());
	if( !m_pAllRollingObjectPI )
	{
		UT::ErrorMsg(L"JP pi is not exist",L"Error");
		return;
	}
	cPuzzleImageUnit*l_pPIUnit = m_pAllRollingObjectPI->GetObject(0);
	int	l_iHeight = *l_pPIUnit->GetHeight();
	int	l_iWidth = *l_pPIUnit->GetWidth();
	e_vRenderPos.x += g_iNumNumerial*l_iWidth/2;
	LineData	l_NumerialOrder;
	for(int i=0;i<g_iNumNumerial;++i)
	{
		std::wstring	l_strUnit = ValueToStringW(i);
		l_NumerialOrder.push_back(l_strUnit);
	}
	for(int i=0;i<g_iNumNumerial;++i)
	{
		Vector4	l_vShowRect(e_vRenderPos.x,e_vRenderPos.y,e_vRenderPos.x+l_iWidth,e_vRenderPos.y+l_iHeight);
		cFixedRollingObjectManager*l_pFixedRollingObjectManager = new cFixedRollingObjectManager(l_vShowRect,1.f+(i*0.5f),l_iHeight*g_iNumNumerial+(i*g_iNumNumerial/2.f*l_iHeight));
		l_pFixedRollingObjectManager->SetupData(l_NumerialOrder,m_pAllRollingObjectPI);
		e_vRenderPos.x -= l_iWidth;
		this->AddObjectNeglectExist(l_pFixedRollingObjectManager);
	}
}

void	cFixedRollingObjectManagerList::Render()
{
	if( this->Count() )
	{
		glEnable(GL_SCISSOR_TEST);
		cBehaviorObjectList<cFixedRollingObjectManager>::Render();
		glDisable(GL_SCISSOR_TEST);
	}
}

void	cFixedRollingObjectManagerList::SetStopResult(int e_iValue)
{
	for(int i=0;i<g_iNumNumerial;++i)
	{
		cFixedRollingObjectManager*l_pFixedRollingObjectManager = this->GetObject(i);
		l_pFixedRollingObjectManager->StartSpinData(0);
	}
	std::string	l_str = ValueToString(e_iValue);
	int	l_iLength = strlen(l_str.c_str());
	for(int i=0;i<l_iLength;++i)
	{
		int	l_iIndex = l_str[l_iLength-i-1]-'0';
		cFixedRollingObjectManager*l_pFixedRollingObjectManager = this->GetObject(i);
		l_pFixedRollingObjectManager->StartSpinData(l_iIndex);
	}
	//this->Init();
}