#include "stdafx.h"
#include "FishDiedEffect.h"


sFishDiedEffectList::sFishDiedEffectList(cMPDI*e_pMPDI,int e_iCount)
{
	iCount = e_iCount;
	ppWorkingList = new cMPDI*[e_iCount];
	for( int i=0;i<iCount;++i )
	{
		ppWorkingList[i] = 0;
		cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(e_pMPDI->Clone());
		l_pMPDI->SetName(e_pMPDI->GetName());
		l_pMPDI->SetCurrentProgress(1.1f);
		FishDiedEffectList.AddObjectNeglectExist(l_pMPDI);
	}
	iNumWorking = 0;
}

sFishDiedEffectList::~sFishDiedEffectList()
{
	for( int i=0;i<iCount;++i )
	{
		ppWorkingList[i] = 0;
	}
	SAFE_DELETE_ARRAY(ppWorkingList);
}

void	sFishDiedEffectList::Update(float e_fElpseTime)
{
	for( int i=0;i<iNumWorking;++i )
	{
		ppWorkingList[i]->Update(e_fElpseTime);
		if(ppWorkingList[i]->IsAnimationDone())
		{
			ppWorkingList[i] = ppWorkingList[iNumWorking-1];
			--iNumWorking;
			--i;
		}
	}
}

void	sFishDiedEffectList::Render()
{
	for( int i=0;i<iNumWorking;++i )
	{
		ppWorkingList[i]->Render();
	}
}

void	sFishDiedEffectList::Show(Vector3 e_vPos,float e_fAngle)
{
	if( iNumWorking +1 >= iCount)
	{//force to add ont
		return;
	}
	for( int i=0;i<iCount;++i )
	{
		if(FishDiedEffectList[i]->IsAnimationDone())
		{
			ppWorkingList[iNumWorking] = FishDiedEffectList[i];
			ppWorkingList[iNumWorking]->Init();
			ppWorkingList[iNumWorking]->SetPos(e_vPos);
			ppWorkingList[iNumWorking]->SetAngle(e_fAngle);
			++iNumWorking;
			return;
		}
	}
}


cFishDiedEffect::cFishDiedEffect()
{

}

cFishDiedEffect::~cFishDiedEffect()
{
	DELETE_VECTOR(m_FishDiedEffectList,sFishDiedEffectList*);
}

void	cFishDiedEffect::AddMPDI(cMPDI*e_pMPDI,int e_iCount)
{
	for(size_t i=0;i<m_FishDiedEffectList.size();++i)
	{
		if(m_FishDiedEffectList[i]->FishDiedEffectList[0]->IsSameName(e_pMPDI))
			return;
	}
	sFishDiedEffectList*l_pFishDiedEffectList = new sFishDiedEffectList(e_pMPDI,e_iCount);
	m_FishDiedEffectList.push_back(l_pFishDiedEffectList);
}

void	cFishDiedEffect::Update(float e_fElpseTime)
{
	for(size_t i=0;i<m_FishDiedEffectList.size();++i)
	{
		m_FishDiedEffectList[i]->Update(e_fElpseTime);
	}
}

void	cFishDiedEffect::Render()
{
	for(size_t i=0;i<m_FishDiedEffectList.size();++i)
	{
		m_FishDiedEffectList[i]->Render();
	}
}

void	cFishDiedEffect::Show(const WCHAR*e_strName,Vector3 e_vPos,float e_fAngle)
{
	for(size_t i=0;i<m_FishDiedEffectList.size();++i)
	{
		if(!wcscmp(m_FishDiedEffectList[i]->FishDiedEffectList[0]->GetName(),e_strName))
		{
			m_FishDiedEffectList[i]->Show(e_vPos,e_fAngle);
			return;
		}
	}
	UT::ErrorMsg(e_strName,L"not exist!!");
}