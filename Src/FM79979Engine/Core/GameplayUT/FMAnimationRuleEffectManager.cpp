#include "../stdafx.h"
#include "FMAnimationRuleEffectManager.h"
#include "FMAnimationRuleEffectQueuePlayer.h"
namespace FATMING_CORE
{
	//cFMAnimationRule::cFMAnimationRule(TiXmlElement*e_pXmlElement)
	//{
	//	m_fPlayingTime = 0.f;
	//}

	//cFMAnimationRule::~cFMAnimationRule()
	//{

	//}

	cFMAnimationRuleManager::cFMAnimationRuleManager()
	{
		m_pFatmingAnimationRuleParseFunction = 0;
	}

	cFMAnimationRuleManager::~cFMAnimationRuleManager()
	{

	}
	//<cFMAnimationRuleManager>
	//	<cFMARQueuePlayer />
	//</cFMAnimationRuleManager>
	//void	cFMAnimationRuleManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	//{
	//	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	//	COMPARE_VALUE("FatmingAnimationRule")
	//	{
	//		ProvessFatmingAnimationRule(e_pTiXmlElement);
	//	}
	//	else
	//	COMPARE_VALUE_WITH_DEFINE(cFMARQueuePlayer::TypeID)
	//	{
	//		ProcessFMARQueuePlayer(e_pTiXmlElement);
	//	}
	//}

	void	cFMAnimationRuleManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		TiXmlElement*l_pTiXmlElement = e_pTiXmlElement;
		cFMAnimationRule*l_pFatmingAnimationRule = 0;
		const WCHAR*l_strValue = l_pTiXmlElement->Value();
		const WCHAR*l_strType = ATTRIBUT_VALUE(l_pTiXmlElement,"Type");
		if( l_strType && !wcscmp(l_strType,cFMARQueuePlayer::TypeID) )
		{
			l_pFatmingAnimationRule = new cFMARQueuePlayer(l_pTiXmlElement->FirstChildElement());
		}
		else
		if( m_pFatmingAnimationRuleParseFunction )
		{
			l_pFatmingAnimationRule = (*m_pFatmingAnimationRuleParseFunction)(l_pTiXmlElement->FirstChildElement());
		}
		//else
		//{
		//	UT::ErrorMsg(l_strValue,L"not suport");
		//}
		if( l_pFatmingAnimationRule )
		{
			const WCHAR*l_strCount = ATTRIBUT_VALUE(l_pTiXmlElement,"Count");
			const WCHAR*l_strAlwaysRun = ATTRIBUT_VALUE(l_pTiXmlElement,"AlwaysRun");
			if( l_strAlwaysRun )
			{
				l_pFatmingAnimationRule->Init();
				this->m_AlwaysRunFatmingAnimationRuleList.AddObjectNeglectExist(l_pFatmingAnimationRule);
			}
			else
			{
				cNamedTypedObjectVector< cFMAnimationRule >*l_pData = new cNamedTypedObjectVector< cFMAnimationRule >;
				l_pData->SetName(l_strValue);
				l_pData->AddObject(l_pFatmingAnimationRule);
				m_AllFatmingAnimationRuleList.AddObject(l_pData);
				if( l_strCount )
				{
					int	l_iCount = GetInt(l_strCount)-1;
					for( int i=0;i<l_iCount;++i )
					{
						l_pData->AddObject(dynamic_cast<cFMAnimationRule*>(l_pFatmingAnimationRule->Clone()));
					}
				}
			}
		}
	}

	//after call this please call cFMAnimationRule::SetData
	cFMAnimationRule* cFMAnimationRuleManager::RequirePrizeEffect( const WCHAR* e_strEffectTypeName,void*e_pData,bool e_bPushIntoTempContainer)
	{
		cNamedTypedObjectVector< cFMAnimationRule >*l_pObject = m_AllFatmingAnimationRuleList.GetObject(e_strEffectTypeName);
		if( l_pObject )
		{
			int	l_iCount = l_pObject->Count();
			if( l_iCount )
			{
				for( int i=0;i<l_iCount;++i )
				{
					cFMAnimationRule*l_pFatmingAnimationRule = l_pObject->GetObject(i);
					if(!l_pFatmingAnimationRule->IsStart())
					{
						if( e_pData )
							l_pFatmingAnimationRule->SetData(e_pData);
						if( m_WorkingFatmingAnimationRuleTempContainer.m_iNumWorking < m_WorkingFatmingAnimationRuleTempContainer.m_iCount )
						{
							
							l_pFatmingAnimationRule->Init();
							if( e_bPushIntoTempContainer )
							{
								m_WorkingFatmingAnimationRuleTempContainer.m_ppObjects[m_WorkingFatmingAnimationRuleTempContainer.m_iNumWorking] = l_pFatmingAnimationRule;
								++m_WorkingFatmingAnimationRuleTempContainer.m_iNumWorking;
							}
						}
						return l_pFatmingAnimationRule;
					}
				}
			}
		}
		else
		{
			cFMAnimationRule*l_pFatmingAnimationRule = m_AlwaysRunFatmingAnimationRuleList.GetObject(e_strEffectTypeName);
			if( l_pFatmingAnimationRule )
			{
				if(!l_pFatmingAnimationRule->IsStart())
				{
					l_pFatmingAnimationRule->Init();
					if( e_pData )
						l_pFatmingAnimationRule->SetData(e_pData);
					return l_pFatmingAnimationRule;
				}			
			}
		}
		return 0;
	}

	void cFMAnimationRuleManager::Init(char*e_strFileName)
	{
		PARSE_FAILED_MESSAGE_BOX(this,e_strFileName);
		int	l_iCount = m_AllFatmingAnimationRuleList.Count();
		int	l_iTotalCount = 0;
		for( int i=0;i<l_iCount;++i )
		{
			l_iTotalCount += m_AllFatmingAnimationRuleList[i]->Count();
		}
		m_WorkingFatmingAnimationRuleTempContainer.SetCount(l_iTotalCount);
	}

	void cFMAnimationRuleManager::Render()
	{
		for(int i=0;i<m_WorkingFatmingAnimationRuleTempContainer.m_iNumWorking;++i)
		{
			m_WorkingFatmingAnimationRuleTempContainer.m_ppObjects[i]->Render();
		}
		int	l_iCount = m_AlwaysRunFatmingAnimationRuleList.Count();
		for(int i=0;i<l_iCount;++i)
		{
			m_AlwaysRunFatmingAnimationRuleList[i]->Render();
		}
		
	}

	void cFMAnimationRuleManager::Update( float e_fElapseTime )
	{
		for(int i=0;i<m_WorkingFatmingAnimationRuleTempContainer.m_iNumWorking;++i)
		{
			m_WorkingFatmingAnimationRuleTempContainer.m_ppObjects[i]->Update(e_fElapseTime);
			if(m_WorkingFatmingAnimationRuleTempContainer.m_ppObjects[i]->IsAnimationDone())
			{
				--m_WorkingFatmingAnimationRuleTempContainer.m_iNumWorking;
				m_WorkingFatmingAnimationRuleTempContainer.m_ppObjects[i] = m_WorkingFatmingAnimationRuleTempContainer.m_ppObjects[m_WorkingFatmingAnimationRuleTempContainer.m_iNumWorking];
			}
		}
		int	l_iCount = m_AlwaysRunFatmingAnimationRuleList.Count();
		for(int i=0;i<l_iCount;++i)
		{
			m_AlwaysRunFatmingAnimationRuleList[i]->Update(e_fElapseTime);
		}
	}
}