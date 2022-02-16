#include "stdafx.h"
#include "SlotObject.h"
#include "SlotBetBehavior.h"
#include "SlotGamePhase.h"
#include "SlotRule.h"

sLoopMovingObject::sLoopMovingObject()
{
	bRender = true;
	bOverEndPos = false;
	iImageIndex = -1;
	pSpinSound = 0;
	pSpinSound = cGameApp::m_spSoundParser->GetObject(SPIN_OBJECT_STOP_SOUND_NAME);
}
sLoopMovingObject::~sLoopMovingObject()
{

}
void	sLoopMovingObject::SetData(Vector3 e_vStartPos,Vector3 e_vEndPos)
{
	vStartPos = vCurrentPos = e_vStartPos;
	vEndPos = e_vEndPos;	
}
void    sLoopMovingObject::Update(Vector3 e_vDistance)
{
    vCurrentPos += e_vDistance;
	bOverEndPos = false;
	//set offset
	while( vCurrentPos.y>vEndPos.y  )
	{
		bOverEndPos = true;
		vCurrentPos = vCurrentPos-vEndPos+vStartPos;
		if( pSpinSound )
		{
			if(!pSpinSound->IsPlay())
				pSpinSound->Play(true);
		}
	}
	while( vCurrentPos.y<this->vStartPos.y )
	{
		vCurrentPos = vCurrentPos-vStartPos+vEndPos;
	}
}

void    sLoopMovingObject::Update(float e_fElpaseTime,Vector3 e_vSpeed)
{
    Vector3 l_vCurrentMoveDistance = e_fElpaseTime*e_vSpeed;
	Update(l_vCurrentMoveDistance);
}

Vector3	sLoopMovingObject::DisToStartPos()
{
	return vStartPos-vCurrentPos;
}


cSlotObjectManager::cSlotObjectManager(Vector4 e_vShowArea,float	e_fSpinTime,float	e_fLeastSpinDis,cPuzzleImage*e_pAllSpinObjectPI,int e_iRowCount,int e_iNormalImageRange)
{
	m_iRowCount = e_iRowCount+1;
	m_iNormalImageRange = e_iNormalImageRange;
	m_pAllSpinObjectPI = e_pAllSpinObjectPI;
	m_fLeastSpinDis = e_fLeastSpinDis;
	m_fImageHeight = (float)*e_pAllSpinObjectPI->GetObject(0)->GetHeight();
	m_fImageWidth =  (float)*e_pAllSpinObjectPI->GetObject(0)->GetWidth();
	m_vShowArea = e_vShowArea;
	m_SpinTime.SetTargetTime(e_fSpinTime);
	m_pLoopMovingObject = new sLoopMovingObject[m_iRowCount];
	m_vStartPos = Vector3(this->m_vShowArea.x,this->m_vShowArea.y-this->m_fImageHeight,0.f);
	m_vEndPos = m_vStartPos;
	//+1 for image end position invisible offset.
	m_vEndPos.y = m_vStartPos.y+m_fImageHeight*this->m_iRowCount;
	m_pShowSpindResultPlayer = new cShowSpindResultPlayer(this);
	SetupResult(0,true);
}

cSlotObjectManager::~cSlotObjectManager()
{
	SAFE_DELETE(m_pShowSpindResultPlayer);
	SAFE_DELETE_ARRAY(m_pLoopMovingObject);
}


void	cSlotObjectManager::AdjustOffsetPosition(bool e_bRandomImage)
{
	//set last start position to the first object's above
	int	l_iLastOneSpinObjectIndex = m_iRowCount-1;
	//set as order,but skip last one
	if( m_pLoopMovingObject )
	{
		for( int i=0;i<m_iRowCount-1;++i )
		{
			m_pLoopMovingObject[i].SetData(m_vStartPos,m_vEndPos);
			m_pLoopMovingObject[i].vCurrentPos.y = (i+1)*m_fImageHeight+m_vStartPos.y;
			m_pLoopMovingObject[i].bRender = true;
			if( e_bRandomImage )
				m_pLoopMovingObject[i].iImageIndex = GetNormalImageWithRandom();
		}
		m_pLoopMovingObject[l_iLastOneSpinObjectIndex].SetData(m_vStartPos,m_vEndPos);
		m_pLoopMovingObject[l_iLastOneSpinObjectIndex].vCurrentPos.y = m_vStartPos.y;
			if( e_bRandomImage )
				m_pLoopMovingObject[l_iLastOneSpinObjectIndex].iImageIndex = GetNormalImageWithRandom();

	}
}

int		cSlotObjectManager::GetNormalImageWithRandom()
{
	return rand()%this->m_iNormalImageRange;
}

void	cSlotObjectManager::Init()
{
	AdjustOffsetPosition(false);
}

void	cSlotObjectManager::SetupResult(std::vector<std::wstring>*e_pResultName,bool e_bWithRandomImage)
{
	if( m_iRowCount < 1 )
		return;
	AdjustOffsetPosition(e_bWithRandomImage);
	if( e_pResultName )
	{
		assert(this->GetRowCount() == e_pResultName->size());
		if( m_pShowSpindResultPlayer )
			m_pShowSpindResultPlayer->SetResultData(e_pResultName);
	}
	if( m_pShowSpindResultPlayer )
		m_pShowSpindResultPlayer->Init();
	m_SpinTime.Start();
}

void	cSlotObjectManager::Update(float e_fElpaseTime)
{
	m_SpinTime.Update(e_fElpaseTime);
	if(!m_SpinTime.bTragetTimrReached)
	{
		float	l_fSpeed = m_fLeastSpinDis/m_SpinTime.fTargetTime;
		if( m_pLoopMovingObject )
		for( int i=0;i<m_iRowCount;++i )
		{
			m_pLoopMovingObject[i].Update(e_fElpaseTime,Vector3(0,l_fSpeed,0));
			if(m_pLoopMovingObject[i].bOverEndPos)
			{
				m_pLoopMovingObject[i].iImageIndex = GetNormalImageWithRandom();
			}
		}
	}
	else
	{
		if( m_pShowSpindResultPlayer )
			m_pShowSpindResultPlayer->Update(e_fElpaseTime);
		//adjust position to correct to minus position error
		//AdjustOffsetPosition();
	}
}

void	cSlotObjectManager::Render()
{
	Vector4	l_vMyScissor( this->m_vStartPos.x,this->m_vStartPos.y+m_fImageHeight,this->m_vStartPos.x+m_fImageWidth,this->m_vStartPos.y+((this->m_iRowCount)*this->m_fImageHeight) );
	Vector4	l_vScissor = ViewRectToOpenGLScissor(l_vMyScissor);

	float	l_fWidth = l_vMyScissor.Width();
	float	l_fHeight = l_vMyScissor.Height();

	float	l_fWidth2 = l_vScissor.z;
	float	l_fHeight2 = l_vScissor.w;
	glScissor((int)l_vScissor.x,(int)l_vScissor.y,(int)l_vScissor.z,(int)l_vScissor.w);
	m_pAllSpinObjectPI->ApplyImage();
	if( m_pAllSpinObjectPI->IsVisible() )
	{
		int		l_iImageWidth = *this->m_pAllSpinObjectPI->GetObject(0)->GetWidth();
		if( m_pLoopMovingObject )
		for( int i=0;i<m_iRowCount;++i )
		{
			sLoopMovingObject*l_pLoopMovingObject = &m_pLoopMovingObject[i];
			if(l_pLoopMovingObject->bRender)
				GLRender::DrawQuadWithTextureAndColorAndCoordinate(l_pLoopMovingObject->vCurrentPos.x,l_pLoopMovingObject->vCurrentPos.y,l_pLoopMovingObject->vCurrentPos.z,l_iImageWidth,(int)this->m_fImageHeight,Vector4::One,m_pAllSpinObjectPI->GetObject(l_pLoopMovingObject->iImageIndex)->GetUV(),0);
		}
	}
}

void	cSlotObjectManager::DebugRender()
{

}

bool	cSlotObjectManager::IsSpinedDone()
{
	float	l_fResTime = GetRestTime();
	bool	l_b = m_pShowSpindResultPlayer->IsDone();
	if( l_b )
	{
		int a=0;

	}
	return l_b;
}

float	cSlotObjectManager::GetRestTime()
{
	float	l_fRestTime = this->m_pShowSpindResultPlayer->GetRestTime();
	return m_SpinTime.fRestTime+l_fRestTime;
}

Vector3	cSlotObjectManager::IndexToResultShowPos(int e_iIndex)
{
	Vector3	l_vPos = this->m_vShowArea;
	l_vPos.y += m_fImageHeight*e_iIndex;
	return l_vPos;
}

const WCHAR*	cSlotObjectManager::GetNameByResultIndex(int e_iIndex)
{
	return this->m_pAllSpinObjectPI->GetObject(m_pLoopMovingObject[e_iIndex].iImageIndex)->GetName();
}

cShowSpindResultPlayer::cShowSpindResultPlayer(cSlotObjectManager*e_pSlotObjectManager)
{
	m_iCountForAllResultSetDone = 0;
	m_bFirstTimeDataCheck = false;
	m_pSlotObjectManager = e_pSlotObjectManager;
	m_piResultForShow = new int[m_pSlotObjectManager->GetRowCount()];
	m_pSpinObjectAnimatedMovingBehaviorMPDIList = cGameApp::GetMPDIListByFileName(ANIMATED_MOVE_SPIN_OBJECT_MPDILIST_FILE_NAME);
	m_pSpinObjectAnimatedMovingBehaviorSubMPDI = new cSubMPDI(m_pSpinObjectAnimatedMovingBehaviorMPDIList->GetObject(ANIMATED_MOVE_SOIN_OBJECT_NAME)->GetObject(0));
	m_fScaleForSpin = 1.f;
	for( int i=0;i<m_pSlotObjectManager->GetRowCount();++i )
		m_piResultForShow[i] = rand()%m_pSlotObjectManager->m_iNormalImageRange;
	m_siFreeGameImageCount = 0;
	//
}
cShowSpindResultPlayer::~cShowSpindResultPlayer()
{
	SAFE_DELETE(m_piResultForShow);
	SAFE_DELETE(m_pSpinObjectAnimatedMovingBehaviorSubMPDI);
}
void	cShowSpindResultPlayer::CheckResultImage()
{
	for( int i=m_iCountForAllResultSetDone;i<m_pSlotObjectManager->GetRowCount();++i )
	{
		sLoopMovingObject*l_pLoopMovingObject = &m_pSlotObjectManager->m_pLoopMovingObject[i];
		if(l_pLoopMovingObject->bOverEndPos)
		{
			l_pLoopMovingObject->iImageIndex = m_piResultForShow[i];
		}
	}
}

void	cShowSpindResultPlayer::SetResultData(std::vector<std::wstring>*e_pResultName)
{
	m_iCountForAllResultSetDone = 0;
	assert(this->m_pSlotObjectManager->GetRowCount() == e_pResultName->size());
	int	l_iSize = (int)e_pResultName->size();
	for( int i=0;i<l_iSize;++i )
	{
		m_piResultForShow[i] = this->m_pSlotObjectManager->m_pAllSpinObjectPI->GetObjectIndexByName(e_pResultName->at(i).c_str());
		if(m_piResultForShow[i] == -1)
		{
			int a=0;
		}
	}
}

int		cShowSpindResultPlayer::m_siFreeGameImageCount = 0;
void	cShowSpindResultPlayer::Init()
{
	m_bFirstTimeDataCheck = false;
	m_pSpinObjectAnimatedMovingBehaviorSubMPDI->Init();
	m_siFreeGameImageCount = 0;
}

void	cShowSpindResultPlayer::Update(float e_fElpaseTime)
{
	if( IsDone() )
	{
		return;
	}
	//e_fElpaseTime/=5.f;
	if( !m_bFirstTimeDataCheck )
	{
		m_bFirstTimeDataCheck = true;
		SAFE_DELETE(m_pSpinObjectAnimatedMovingBehaviorSubMPDI);
		m_pSpinObjectAnimatedMovingBehaviorSubMPDI = new cSubMPDI(m_pSpinObjectAnimatedMovingBehaviorMPDIList->GetObject(ANIMATED_MOVE_SOIN_OBJECT_NAME)->GetObject(0));
		float	l_fDis = GetDistanceFromCurrentToStartOfLoop(m_pSlotObjectManager->m_vStartPos.y,
			m_pSlotObjectManager->m_vEndPos.y,
			m_pSlotObjectManager->m_pLoopMovingObject[0].vCurrentPos.y,
			m_pSlotObjectManager->m_vShowArea.y);
		//now we knoe how far we still need to spin.
		m_vRestDisToDestination = Vector3(0,l_fDis,0);
		//do a little trick to spin one more circle so we could perform  a nice stop as MPDI
		//and ajust mpdi's CurveWithTime data
		m_vRestDisToDestination += m_pSlotObjectManager->m_vEndPos-m_pSlotObjectManager->m_vStartPos;

		Vector3	l_vDis = m_pSpinObjectAnimatedMovingBehaviorSubMPDI->GetLastPoint()-m_pSpinObjectAnimatedMovingBehaviorSubMPDI->GetOriginalPointList()[0];
		float	l_fDistance = l_vDis.Length();
		m_fScaleForSpin = m_vRestDisToDestination.Length()/l_fDistance;
		m_pSpinObjectAnimatedMovingBehaviorSubMPDI->Scale(m_fScaleForSpin);
		m_pSpinObjectAnimatedMovingBehaviorSubMPDI->Init();
		l_fDistance = m_pSpinObjectAnimatedMovingBehaviorSubMPDI->GetTotalDistance();
		
	}
	//Vector3	l_vPos1 = m_pSpinObjectAnimatedMovingBehaviorSubMPDI->GetCurrentPosition();
	m_pSpinObjectAnimatedMovingBehaviorSubMPDI->Update(e_fElpaseTime);
	//Vector3	l_vPos2 = m_pSpinObjectAnimatedMovingBehaviorSubMPDI->GetCurrentPosition();
	//Vector3	l_vTargetSpeed = (l_vPos2-l_vPos1);
	Vector3	l_vSpeed = m_pSpinObjectAnimatedMovingBehaviorSubMPDI->GetMovedCoordinate();
	for( int i=0;i<m_pSlotObjectManager->m_iRowCount;++i )
	{
		m_pSlotObjectManager->m_pLoopMovingObject[i].Update(l_vSpeed);
	}
	if(IsDone())
	{
		//m_pSlotObjectManager->AdjustOffsetPosition();		
		int	l_iIndex = m_pSlotObjectManager->m_pAllSpinObjectPI->GetObjectIndexByName(g_pSlotGamePhase->m_pSlotRuleData->m_strFreeGameImageName.c_str());
		for( int i=0;i<m_pSlotObjectManager->m_iRowCount-1;++i )
		{
			if(m_pSlotObjectManager->m_pLoopMovingObject[i].iImageIndex == l_iIndex )
			{
				++m_siFreeGameImageCount;
				std::wstring	l_strName = PLAY_FREE_GAME_SOUND_NAME;
				l_strName += ValueToStringW(m_siFreeGameImageCount);
				cGameApp::SoundPlay(l_strName.c_str(),true);
				break;
			}
		}
	}
	CheckResultImage();
}
bool	cShowSpindResultPlayer::IsDone()
{
	return this->m_pSpinObjectAnimatedMovingBehaviorSubMPDI->IsAnimationDone();
}

float	cShowSpindResultPlayer::GetRestTime()
{
	return m_pSpinObjectAnimatedMovingBehaviorSubMPDI->GetRestTime();
}

cSlotObjectManagerList::cSlotObjectManagerList()
{
}
cSlotObjectManagerList::~cSlotObjectManagerList(){}
void	cSlotObjectManagerList::DoScissor()
{
	cSlotObjectManager*l_pSlotObjectManager = this->GetObject(0);
	if( l_pSlotObjectManager )
	{
		Vector4	l_vScissor = ViewRectToOpenGLScissor(l_pSlotObjectManager->m_vShowArea);
		glEnable(GL_SCISSOR_TEST);
		glScissor((int)l_vScissor.x,(int)l_vScissor.y,(int)l_vScissor.z,(int)l_vScissor.w);	
	}
}
void	cSlotObjectManagerList::UnDoScissor()
{
	glDisable(GL_SCISSOR_TEST);
}
void	cSlotObjectManagerList::Render()
{
	if( this->Count() )
	{
		glEnable(GL_SCISSOR_TEST);
		cBehaviorObjectList<cSlotObjectManager>::Render();
		glDisable(GL_SCISSOR_TEST);
	}
}
bool	cSlotObjectManagerList::IsSpinedDone()
{
	int l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		if(!this->m_ObjectList[i]->IsSpinedDone())
			return false;
	}
	return true;
}
int		cSlotObjectManagerList::GetNameCount(const WCHAR*e_strName)
{
	int	l_iNum = 0;
    int l_iCount = this->Count();
	if( l_iCount )
	{
		cSlotObjectManager*l_pSlotObjectManager = this->m_ObjectList[0];
		cPuzzleImage*l_pPI = l_pSlotObjectManager->GetAllSpinObjectPI();
		for(int i=0;i<l_iCount;++i)
		{
			l_pSlotObjectManager = this->m_ObjectList[i];
			int	l_iRowCount = l_pSlotObjectManager->GetRowCount();
			for(int j=0;j<l_iRowCount;++j)
			{
				int	l_iImageIndex = l_pSlotObjectManager->GetLoopMovingObject(j)->iImageIndex;
				if( l_iImageIndex != -1 )
				{
					const WCHAR*l_strImageName = l_pPI->GetObject(l_iImageIndex)->GetName();
					if( !wcscmp(e_strName,l_strImageName) )
					{
						++l_iNum;
					}
				}
			}
		}
	}
	return l_iNum;
}
std::wstring		GetSpinObjectNameByIndex(int e_iIndex)
{
	POINT	l_Index = g_pSlotGamePhase->m_pSlotRuleData->SpinObjectIndexToRowColumn(e_iIndex);
	std::wstring	l_str;
	cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(l_Index.x);
	l_str = l_pSlotObjectManager->GetNameByResultIndex(l_Index.y);
	return l_str;
}

sLoopMovingObject*	GetLoopMovingObjectByIndex(int e_iIndex)
{
	POINT	l_Index = g_pSlotGamePhase->m_pSlotRuleData->SpinObjectIndexToRowColumn(e_iIndex);
	return g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(l_Index.x)->GetLoopMovingObject(l_Index.y);
	
}