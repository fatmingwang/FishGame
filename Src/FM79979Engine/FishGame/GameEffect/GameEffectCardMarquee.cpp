#include "stdafx.h"
#include "GameEffectBase.h"
#include "GameEffectCardMarquee.h"

const wchar_t*         cGameEffectCardMarqueeManager::TypeID( L"cGameEffectCardMarqueeManager" );


cGameEffectPlayerCardMarquee::sCardData::sCardData()
{
	iValue = 0;
	PuzzleImageUnit = 0;
	QueueCardStatus = eQCS_SHOW;
}
cGameEffectPlayerCardMarquee::sCardData::~sCardData()
{

}

void	cGameEffectPlayerCardMarquee::sCardData::SetData(Vector3	e_vPos,cPuzzleImageUnit*e_pPuzzleImageUnit,float e_fTimeToShow,float e_fTimeToDisappear,int e_iValue,cMatrix44*e_pMat)
{
	pMat = e_pMat;
	vPos = e_vPos;
	iValue = e_iValue;
	PuzzleImageUnit = e_pPuzzleImageUnit;
	TimeToDisappear.SetTargetTime(e_fTimeToShow);
	TimeToShow.SetTargetTime(e_fTimeToDisappear);
	QueueCardStatus = eQCS_SHOW;
}

void	cGameEffectPlayerCardMarquee::sCardData::Update(float e_fElpaseTime,Vector3 e_vMovePos)
{
	vPos += e_vMovePos;
	if( QueueCardStatus == eQCS_SHOW )
	{
		this->TimeToShow.Update(e_fElpaseTime);
	}
	else
	if( QueueCardStatus == eQCS_DISAPPEAR )
	{
		TimeToDisappear.Update(e_fElpaseTime);
	}
}

void	cGameEffectPlayerCardMarquee::sCardData::Render(cPuzzleImage*e_pPI,cNumeralImage*e_pNumerialImage)
{
	cPuzzleImageUnit*l_pPIUnit = PuzzleImageUnit;//e_pPI->GetObject(iPuzzleImageUnitIndex);
	if( l_pPIUnit )
	{
		float	l_f = 1.f;
		if( QueueCardStatus == eQCS_SHOW )
		{
			l_f = TimeToShow.GetLERP();
			if( TimeToShow.bTragetTimrReached )
			{
				l_pPIUnit->SetColor(Vector4::One);
			}
			else
				l_pPIUnit->SetColor(Vector4(l_f,l_f,l_f,l_f));
		}
		else
		{
			l_f = 1-TimeToDisappear.GetLERP();
			if( TimeToDisappear.bTragetTimrReached )
			{
				l_pPIUnit->SetColor(Vector4::One);
			}
			else
				l_pPIUnit->SetColor(Vector4(l_f,l_f,l_f,l_f));
		}
		//l_pPIUnit->SetPos(vPos);
		//Vector3	l_vPos = this->pMat->TransformCoordinate(vPos);
		cMatrix44	l_mat = (*pMat)*cMatrix44::TranslationMatrix(vPos);
		l_pPIUnit->Render(l_mat);
		if( e_pNumerialImage )
		{
			//l_vPos.x += *l_pPIUnit->GetWidth()/2.f;
			int	l_iHeight = *l_pPIUnit->GetHeight()/2;
			l_mat = l_mat*cMatrix44::TranslationMatrix(Vector3(0,(float)l_iHeight,0));
			e_pNumerialImage->SetColor(Vector4(l_f,l_f,l_f,l_f));
			//e_pNumerialImage->DrawOnCenter(iValue,(int)l_vPos.x,(int)l_vPos.y);
			e_pNumerialImage->DrawOnCenter(iValue,0,0,l_mat);
		}
	}
}

cGameEffectPlayerCardMarquee::cGameEffectPlayerCardMarquee(cPuzzleImage*e_pCardPI,int e_iMaximumCount,float e_fTimeToDestination,Vector3 e_vStartPos,Vector3 e_vDestination,float e_fHoldTime,int e_iViewCount,cMatrix44 e_matShowMatrix,cNumeralImage*e_pNumerialImage)
{
	m_pNumerialImage = e_pNumerialImage;
	m_ShowMatrix = e_matShowMatrix;
	m_pCardPI = e_pCardPI;
	m_MoveTC.SetTimeAndPosData(e_vDestination,e_vStartPos,e_fTimeToDestination);
	m_AfterMoveHoldTime.SetTargetTime(e_fHoldTime);
	m_AfterMoveHoldTime.bTragetTimrReached = true;
	m_iViewCount = e_iViewCount;
	m_iMaximumCount = e_iMaximumCount;
	m_MoveTC.Start();
	m_AfterMoveHoldTime.Start();
	//for( std::list<sCardData>::iterator l_CardDataiterator = m_CardDataList.begin();l_CardDataiterator != m_CardDataList.end();++l_CardDataiterator )
	//{
	//	sCardData	l_sCardData = *l_CardDataiterator;
	//	l_sCardData.iValue;
	//}
	//l_b.ne
}
cGameEffectPlayerCardMarquee::~cGameEffectPlayerCardMarquee()
{
	//SAFE_DELETE(m_pNumerialImage);
}
void	cGameEffectPlayerCardMarquee::Init()
{
	m_MoveTC.Start();
	m_AfterMoveHoldTime.Start();
}
void	cGameEffectPlayerCardMarquee::AddData(WCHAR*e_strFishName,int e_iValueForShow)
{
	int	l_iSize = (int)m_CardDataList.size();
	if( l_iSize < m_iMaximumCount )
	{//if voer alow show objects ignore new object
		sCardData l_CardAndPos;
		cPuzzleImageUnit*l_pPuzzleImageUnit = 0;
		if( m_pCardPI )
		{
			//l_pCardAndPos->pImage = m_pCardPI->GetObject(0);
			//l_iImageIndex = rand()%m_pCardPI->Count();
			l_pPuzzleImageUnit = m_pCardPI->GetObject(0);
			//l_iImageIndex = 0;
		}
		//Vector3	l_vPos = m_ShowMatrix.GetTranslation();
		Vector3	l_vPos = Vector3::Zero;
		if( l_iSize > 0 && l_iSize < this->m_iViewCount )
		{
			int	l_iIndex = 0;
			std::list<sCardData>::iterator l_iterator;
			for( l_iterator = m_CardDataList.begin(); l_iterator != m_CardDataList.end() ; ++l_iterator)
			{
				if(l_iIndex == l_iSize-1)
					break;
				++l_iIndex;
			}
			sCardData*l_pCardData = (sCardData*)&(*l_iterator);
			Vector3	l_vGap = m_MoveTC.vEndPos-m_MoveTC.vStartPos;
			l_vPos = l_pCardData->vPos - l_vGap;
		}
		l_CardAndPos.SetData(l_vPos,l_pPuzzleImageUnit,0.5f,0.5f,e_iValueForShow,&this->m_ShowMatrix);
		m_CardDataList.push_back(l_CardAndPos);
		if(l_iSize == 0 )
			m_MoveTC.Start();
	}
}

void	cGameEffectPlayerCardMarquee::ResetCardPos()
{
	int	l_iSize = (int)m_CardDataList.size();
	Vector3	l_vGap = m_MoveTC.vEndPos-m_MoveTC.vStartPos;
	Vector3	l_vStatPos = Vector3::Zero;
	int l_iIndex = 0;
	for( std::list<sCardData>::iterator l_iterator = m_CardDataList.begin(); l_iterator != m_CardDataList.end() ; ++l_iterator)
	{
		sCardData*l_pCardData = (sCardData*)&(*l_iterator);
		l_pCardData->vPos = l_vStatPos;
		l_vStatPos -= l_vGap;
		++l_iIndex;
		if( l_iIndex >= m_iViewCount )
			break;
	}
}

void	cGameEffectPlayerCardMarquee::Update(float e_fElpaseTime)
{
	if( m_CardDataList.begin() != m_CardDataList.end() )
	{
		int	l_iSize = (int)m_CardDataList.size();
		if( l_iSize > this->m_iViewCount )
			e_fElpaseTime *= 2.f;
		std::list<sCardData>::iterator l_iterator = m_CardDataList.begin();
		sCardData*l_pCardData = (sCardData*)&(*l_iterator);
		if( !m_MoveTC.MoveToDestinationTC.bTragetTimrReached )
			m_MoveTC.Update(e_fElpaseTime);
		if( m_MoveTC.MoveToDestinationTC.bTragetTimrReached )
		{
			if( l_pCardData->TimeToShow.bTragetTimrReached &&  l_pCardData->QueueCardStatus == eQCS_SHOW )
				l_pCardData->QueueCardStatus = eQCS_DISAPPEAR;
		}
		Vector3	l_vMovePos = l_pCardData->vPos-m_MoveTC.vTotalMoveDistance;
		if( l_vMovePos.Length() < 1.f )
		{
			m_MoveTC.vCurrentMoveDistance = Vector3::Zero;
		}
		l_pCardData->Update(e_fElpaseTime,m_MoveTC.vCurrentMoveDistance);
		if( l_pCardData->TimeToDisappear.bTragetTimrReached )
		{//reset card position
			m_CardDataList.pop_front();
			ResetCardPos();
			m_MoveTC.Start();
			return;
		}
		++l_iterator;
		int l_iIndex = 1;
		for( ; l_iterator != m_CardDataList.end() ; ++l_iterator)
		{
			l_pCardData = (sCardData*)&(*l_iterator);
			l_pCardData->Update(e_fElpaseTime,m_MoveTC.vCurrentMoveDistance);
			++l_iIndex;
			if( l_iIndex >= m_iViewCount )
				break;
		}
	}
}

void	cGameEffectPlayerCardMarquee::Render()
{
	int l_iIndex = 0;
	for( std::list<sCardData>::iterator l_iterator = m_CardDataList.begin(); l_iterator != m_CardDataList.end() ; ++l_iterator)
	{
		sCardData*l_pCardData = (sCardData*)&(*l_iterator);
		l_pCardData->Render(m_pCardPI,m_pNumerialImage);
		++l_iIndex;
		if( l_iIndex >= 4 )
			break;
	}
}

cGameEffectCardMarqueeManager::cGameEffectCardMarqueeManager(cNumeralImage*e_pNumerialImage)
{
	m_pNumerialImage = e_pNumerialImage;
	this->SetName(cGameEffectCardMarqueeManager::TypeID);
}

cGameEffectCardMarqueeManager::~cGameEffectCardMarqueeManager()
{
	SAFE_DELETE(m_pNumerialImage);
}

void	cGameEffectCardMarqueeManager::InternalInit()
{
	//int	l_iCount = m_GameEffectPlayerCardMarqueeList.Count();
	//for( int i=0;i<l_iCount;++i )
	//{
	//	m_GameEffectPlayerCardMarqueeList[i]->Init();
	//}
}
void	cGameEffectCardMarqueeManager::InternalUpdate(float e_fElpaseTime){}
void	cGameEffectCardMarqueeManager::Update(float e_fElpaseTime)
{
	int	l_iCount = m_GameEffectPlayerCardMarqueeList.Count();
	for( int i=0;i<l_iCount;++i )
	{
		m_GameEffectPlayerCardMarqueeList[i]->Update(e_fElpaseTime);
	}
	m_bStart = false;
}

void	cGameEffectCardMarqueeManager::InternalRender()
{
	int	l_iCount = m_GameEffectPlayerCardMarqueeList.Count();
	for( int i=0;i<l_iCount;++i )
	{
		m_GameEffectPlayerCardMarqueeList[i]->Render();
	}
}

//Fish Name,player ID,Matrix
void	cGameEffectCardMarqueeManager::SetData(void*e_pPlayerBehaviorBase)
{
	sGameEffectInputData*l_pGameEffectInputData = (sGameEffectInputData*)e_pPlayerBehaviorBase;
	cGameEffectPlayerCardMarquee*l_pGameEffectPlayerCardMarquee = m_GameEffectPlayerCardMarqueeList.GetObject(l_pGameEffectInputData->iPlayerID);
	if( l_pGameEffectPlayerCardMarquee )
	{		
		l_pGameEffectPlayerCardMarquee->AddData(l_pGameEffectInputData->strMonstrName,l_pGameEffectInputData->iValue);
	}
}
