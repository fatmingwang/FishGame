#include "stdafx.h"
#include "BankNegativeMode.h"
//








void	cNegativeBankMode::DebugRender( int e_iPosX,int e_iPosY )
{
	//
	std::wstring	l_str = 
		UT::ComposeMsgByFormat( L"NegR:%d, In:%d, Out:%d",
			(int)this->m_i64Revenue,
			(int)this->m_i64CurrentIn,
			this->m_i64CurrentOut );
	cGameApp::m_spGlyphFontRender->RenderFont(e_iPosX,e_iPosY,l_str.c_str());	
}









//<NegativeMode BigPrizeToOut="20" FetchPercentageToRevenueFromBullet="0.3" BulletDuration="6"/>
cNegativeBankMode::cNegativeBankMode(TiXmlElement*e_pTiXmlElement):cBankMode(e_pTiXmlElement)
{
	m_iBigPrizeToOut = Vector2(10,300);
	m_iThisRoundBigPrizeTarget = 50;
	m_i64StoredBigMoneyToOut = 0;
	m_fFetchPercentageToRevenueFromBullet = Vector2(0.3f,0.5f);
	const WCHAR*l_strBigPrizeToOut = e_pTiXmlElement->Attribute(L"BigPrizeToOut");
	const WCHAR*l_strFetchPercentageToRevenueFromBullet = e_pTiXmlElement->Attribute(L"FetchPercentageToRevenueFromBullet");
	if( l_strBigPrizeToOut )
	{
		m_iBigPrizeToOut = GetVector2(l_strBigPrizeToOut);
	}
	if( l_strFetchPercentageToRevenueFromBullet )
	{
		m_fFetchPercentageToRevenueFromBullet = GetVector2(l_strFetchPercentageToRevenueFromBullet);
	}
}








cNegativeBankMode::~cNegativeBankMode()
{

}








void	cNegativeBankMode::AddRevenue(int64 e_i64Value)
{
	BulletDataUpdate(e_i64Value);

	int64	l_i64FetchMoney = (int64)(e_i64Value*m_fFetchPercentageToRevenueFromBullet.Rand());
	int64	l_i64StoreRevenue = e_i64Value-(int64)(e_i64Value*m_TargetProbability);
	if( l_i64FetchMoney +l_i64StoreRevenue > e_i64Value )
	{
		l_i64StoreRevenue = 0;
	}
	m_i64StoredBigMoneyToOut += l_i64FetchMoney;

	e_i64Value -= l_i64FetchMoney;
	e_i64Value -= l_i64StoreRevenue;

	m_i64CurrentIn += e_i64Value;
	m_i64Revenue += e_i64Value;
}








bool	cNegativeBankMode::IsModeSatisfied()
{//ensure big money is out and revenue is positive
	if ( this->m_i64Revenue > 0  )
		return true;
	return false;
}








void	cNegativeBankMode::ResetDataInit()
{
	m_iThisRoundBigPrizeTarget = m_iBigPrizeToOut.Rand();
	if( m_i64StoredBigMoneyToOut < 0 )
	{
		m_i64StoredBigMoneyToOut = 0;
	}
}








bool	cNegativeBankMode::IsBigMoneyOkOkay(int e_iMoney,int e_iPayRate)
{
	if( this->m_i64StoredBigMoneyToOut >= e_iMoney )
	{
		int	l_iFishPrize = e_iMoney/e_iPayRate;
		if( l_iFishPrize >= this->m_iThisRoundBigPrizeTarget || 
			this->m_iNumRevenueAddAndNoRevenueMinus > 10 && l_iFishPrize <= 10 )
		{
			this->m_i64Revenue += e_iMoney;
			m_i64StoredBigMoneyToOut -= e_iMoney;
			this->m_i64CurrentOut -= e_iMoney;
			return true;
		}
	}
	//if( m_iNumRevenueAddAndNoRevenueMinus >= 5 )
	{
		m_iThisRoundBigPrizeTarget = this->m_iBigPrizeToOut.Rand();
	}
	return false;
}








bool	cNegativeBankMode::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	if( IsBigMoneyOkOkay(e_iMoney,e_iPayRate) )
		return true;	
	return cBankMode::IsMoneyOutOk(e_iMoney,e_iPayRate);
}








