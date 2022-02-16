#include "../stdafx.h"
#include "BankFormat.h"
//
#include "BankRunner.h"
//
//<Root>
//	<SubRepositoryBank Range="100,70">
//		<PossitiveBankMode >
//			<FakeRevenue FakeRevenue="2000" FakeRevenueCircle="30,1000" HappyRevenue="100,10000" FakeRevenueFetchProbability="0.3" FakeRevenueGiveDivision="4" HappyTimeScale="1"/>
//		</PossitiveBankMode>
//		<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.5" />
//	</SubRepositoryBank>
//	<SubRepositoryBank Range="69,31">
//		<PossitiveBankMode >
//			<FakeRevenue FakeRevenue="500" FakeRevenueCircle="30,1000" HappyRevenue="100,1000" FakeRevenueFetchProbability="0.3" FakeRevenueGiveDivision="4" HappyTimeScale="1"/>
//		</PossitiveBankMode>
//		<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.2" />
//	</SubRepositoryBank>
//	<SubRepositoryBank Range="30,0">
//		<PossitiveBankMode >
//			<FakeRevenue FakeRevenue="300" FakeRevenueCircle="30,1000" HappyRevenue="10,100" FakeRevenueFetchProbability="0.3" FakeRevenueGiveDivision="4" HappyTimeScale="1"/>
//		</PossitiveBankMode>
//		<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.1" />
//	</SubRepositoryBank>
//</Root>







bool	cBankRunnerManager::MyParse(TiXmlElement*e_pRoot)
{
	m_iCurrentWorkingIndex = 0;
	e_pRoot = e_pRoot->FirstChildElement();
	while(e_pRoot)
	{
		const WCHAR*l_strValue = e_pRoot->Value();
		COMPARE_VALUE("SubRepositoryBank")
		{
			cBankRunner*l_pSubRepositoryBank = new cBankRunner(e_pRoot);
			this->AddObjectNeglectExist(l_pSubRepositoryBank);
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}








int		cBankRunnerManager::GetIndexByPerenctage(float e_fPercentage,int e_iPayRate,int e_iPlayerID)
{
	e_fPercentage *= 100;
	e_fPercentage = (float)(int)e_fPercentage;
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cBankRunner*l_pSubRepositoryBank = this->GetObject(i);
		if(l_pSubRepositoryBank->m_vTargetPercentValueRange.x>=e_fPercentage&&
			l_pSubRepositoryBank->m_vTargetPercentValueRange.y<=e_fPercentage)
		{
			if( l_pSubRepositoryBank->IsLeastMoneyOk(e_iPayRate) )
			{
				m_iCurrentWorkingIndex = i;
				return i;
			}
			for( ; i<l_iCount ; ++i )
			{
				l_pSubRepositoryBank = this->GetObject(i);
				if( l_pSubRepositoryBank->IsLeastMoneyOk(e_iPayRate) )
				{
					m_iCurrentWorkingIndex = i;
					return i;
				}
			}
			return -1;
		}
	}
	m_iCurrentWorkingIndex = -1;
	assert(0&&"GetIndexByPerenctage(float e_fPercentage,int e_iPayRate)");
	return 0;
}








void	cBankRunnerManager::Update(float e_fElpaseTime)
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cBankRunner*l_pSubRepositoryBank = this->GetObject(i);
		l_pSubRepositoryBank->Update(e_fElpaseTime);
	}
}








void	cBankRunnerManager::DebugRender( int e_nPosX, int e_nPosY )
{
	//int	l_iCount = this->Count();
	//for( int i=0;i<l_iCount;++i )
	//{
	//	cBankRunner*l_pSubRepositoryBank = this->GetObject(i);
	//	l_pSubRepositoryBank->DebugRender(50,100+50*i);
	//}

	std::wstring	l_str = UT::ComposeMsgByFormat( L"pbMode: %d", m_iCurrentWorkingIndex );
	cGameApp::m_spGlyphFontRender->RenderFont( e_nPosX, e_nPosY, l_str.c_str() );
	//
	cBankRunner*	l_pSubRepositoryBank = this->GetObject( m_iCurrentWorkingIndex );
	l_pSubRepositoryBank->DebugRender( e_nPosX + 250, e_nPosY );
}








