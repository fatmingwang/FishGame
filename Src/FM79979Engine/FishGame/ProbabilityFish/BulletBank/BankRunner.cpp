#include "../stdafx.h"
#include "BankRunner.h"
#include "BankPossitiveMode.h"
#include "BankNegativeMode.h"
#include "../FishProbability.h"
#include "../../GameApp/GameApp.h"


cBankRunner::cBankRunner(TiXmlElement*e_pXmlElement)
{
	m_i64LeastMoney = 0;
	m_vTargetPercentValueRange = Vector2(0,1);
	m_pPossitiveBankMode = 0;
	m_pNegativeBankMode = 0;
	const WCHAR*l_strValue = e_pXmlElement->Value();
	COMPARE_VALUE("SubRepositoryBank")
	{
		const WCHAR*l_strRange = e_pXmlElement->Attribute(L"Range");
		const WCHAR*l_strLeastMoney = e_pXmlElement->Attribute(L"LeastMoney");
		if( l_strRange )
		{
			m_vTargetPercentValueRange = GetVector2(l_strRange);
		}
		if(l_strLeastMoney)
		{
			m_i64LeastMoney = GetInt64(l_strLeastMoney)*cFishApp::m_spControlPanel->GetCoinToScore();
		}
		e_pXmlElement = e_pXmlElement->FirstChildElement();
		while( e_pXmlElement )
		{
			l_strValue = e_pXmlElement->Value();
			COMPARE_VALUE("PossitiveBankMode")
			{
				m_pPossitiveBankMode = new cPossitiveBankMode(e_pXmlElement);
			}
			else
			COMPARE_VALUE("NegativeMode")
			{
				m_pNegativeBankMode = new cNegativeBankMode(e_pXmlElement);
			}
			e_pXmlElement = e_pXmlElement->NextSiblingElement();
		}
	}
	assert(m_pPossitiveBankMode);
	assert(m_pNegativeBankMode);
	m_pPossitiveBankMode->SetOwner(this);
	m_pNegativeBankMode->SetOwner(this);
	m_pCurrentMode = m_pPossitiveBankMode;
}








cBankRunner::~cBankRunner()
{
	SAFE_DELETE(m_pPossitiveBankMode);
	SAFE_DELETE(m_pNegativeBankMode);
}








void	cBankRunner::Update( float e_fElpaseTime )
{
	m_pCurrentMode->Update(e_fElpaseTime);
	bool	l_bSatisfied = m_pCurrentMode->IsModeSatisfied();
	float l_fProbability = cFishApp::m_spProbabilityFish->GetOffsetProbability();
	//if( !l_bSatisfied )
	//{
	//	if( m_pCurrentMode == m_pNegativeBankMode )
	//	{
	//		if(l_fProbability)
	//		{
	//			float	l_f = (float)m_i64LeastMoney/cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
	//			if( frand(0,1) <= l_f/2.f )
	//				l_bSatisfied = true;
	//		}
	//	}
	//}
	if( l_bSatisfied )
	{
		if( l_fProbability > cFishApp::m_spProbabilityFish->m_fSafeToGiveProbabilityOffset )
		if( m_pCurrentMode == m_pPossitiveBankMode&&rand()%2 == 0 )
		{
			m_pCurrentMode->ModeAssign(m_pCurrentMode);
			return ;
		} 
		//
		if( m_pCurrentMode == m_pPossitiveBankMode )
		{
			m_pCurrentMode = m_pNegativeBankMode;
			m_pCurrentMode->ModeAssign(m_pPossitiveBankMode);
		}
		else
		{
			m_pCurrentMode = m_pPossitiveBankMode;
			m_pCurrentMode->ModeAssign(m_pNegativeBankMode);		
		} 
	} 
}








void	cBankRunner::DebugRender( int e_iPosX,int e_iPosY )
{
	m_pCurrentMode->DebugRender(e_iPosX,e_iPosY);
}








void	cBankRunner::AddRevenue( int64 e_iValue )
{
	++m_pCurrentMode->m_iNumRevenueAddAndNoRevenueMinus;
	m_pCurrentMode->AddRevenue(e_iValue);
}








void	cBankRunner::MinusRevenue(int64 e_iValue)
{
	m_pCurrentMode->m_iNumRevenueAddAndNoRevenueMinus = 0;
	m_pCurrentMode->MinusRevenue(e_iValue);
}








bool	cBankRunner::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	return m_pCurrentMode->IsMoneyOutOk(e_iMoney,e_iPayRate);
}








bool	cBankRunner::IsFakeMoneyEnoughToPay(int e_iMoney)
{
	return m_pCurrentMode->IsFakeMoneyEnoughToPay(e_iMoney);
}








bool	cBankRunner::IsLeastMoneyOk( int64 e_i64Money )
{
	return e_i64Money>=m_i64LeastMoney?true:false;
}








