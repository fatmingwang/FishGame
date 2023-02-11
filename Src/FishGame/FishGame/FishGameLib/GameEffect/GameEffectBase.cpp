#include "stdafx.h"
#include "GameEffectBase.h"
cGameEffectBase::cGameEffectBase()
{

}

cGameEffectBase::~cGameEffectBase()
{

}

cGameEffectManager::cGameEffectManager()
{
	m_pGameEffectParseFunction = 0;
}

cGameEffectManager::~cGameEffectManager()
{

}

//after call this please call cGameEffectBase::SetData
cGameEffectBase* cGameEffectManager::RequirePrizeEffect( const WCHAR* e_strEffectTypeName,void*e_pData, bool e_bForceRequire)
{
	cNamedTypedObjectVector< cGameEffectBase >*l_pObject = m_AllGameEffectBaseList.GetObject(e_strEffectTypeName);
	if( l_pObject )
	{
		int	l_iCount = l_pObject->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cGameEffectBase*l_pGameEffectBase = l_pObject->GetObject(i);
			if(!l_pGameEffectBase->IsStart())
			{
				if( e_pData )
					l_pGameEffectBase->SetData(e_pData);
				if( m_WorkingGameEffectBase.m_iNumWorking < m_WorkingGameEffectBase.m_iCount )
				{
					m_WorkingGameEffectBase.m_ppObjects[m_WorkingGameEffectBase.m_iNumWorking] = l_pGameEffectBase;
					l_pGameEffectBase->Init();
					++m_WorkingGameEffectBase.m_iNumWorking;
				}
				return l_pGameEffectBase;
			}
		}
	}
	else
	{
		cGameEffectBase*l_pGameEffectBase = m_AlwaysRunGameEffectBaseList.GetObject(e_strEffectTypeName);
		if( l_pGameEffectBase )
		{
			if(!l_pGameEffectBase->IsStart())
			{
				l_pGameEffectBase->Init();
				if( e_pData )
					l_pGameEffectBase->SetData(e_pData);
				return l_pGameEffectBase;
			}			
		}
	}
	return 0;
}

void cGameEffectManager::Init()
{
	PARSE_FAILED_MESSAGE_BOX(this,"Fish/GameEffect/GameEffect.xml");
	int	l_iCount = m_AllGameEffectBaseList.Count();
	int	l_iTotalCount = 0;
	for( int i=0;i<l_iCount;++i )
	{
		l_iTotalCount += m_AllGameEffectBaseList[i]->Count();
	}
	m_WorkingGameEffectBase.SetCount(l_iTotalCount);
}

void cGameEffectManager::Render()
{
	for(int i=0;i<m_WorkingGameEffectBase.m_iNumWorking;++i)
	{
		m_WorkingGameEffectBase.m_ppObjects[i]->Render();
	}
	int	l_iCount = m_AlwaysRunGameEffectBaseList.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_AlwaysRunGameEffectBaseList[i]->Render();
	}
	
}

void cGameEffectManager::Update( float e_fElapseTime )
{
	for(int i=0;i<m_WorkingGameEffectBase.m_iNumWorking;++i)
	{
		m_WorkingGameEffectBase.m_ppObjects[i]->Update(e_fElapseTime);
		if(m_WorkingGameEffectBase.m_ppObjects[i]->IsAnimationDone())
		{
			--m_WorkingGameEffectBase.m_iNumWorking;
			m_WorkingGameEffectBase.m_ppObjects[i] = m_WorkingGameEffectBase.m_ppObjects[m_WorkingGameEffectBase.m_iNumWorking];
		}
	}
	int	l_iCount = m_AlwaysRunGameEffectBaseList.Count();
	for(int i=0;i<l_iCount;++i)
	{
		m_AlwaysRunGameEffectBaseList[i]->Update(e_fElapseTime);
	}
}