#include "stdafx.h"
#include "QucikInfo.h"
#include "GameApp.h"
#include "../ControlPanel/ControlPanel.h"
cQuickInfo::cQuickInfo(Vector3 e_vStartDrawPos,unsigned char e_WakeUpKey)
{
	m_bShowInfo = false;
	m_ShowInfoKey.SetKeyData(e_WakeUpKey);
	m_ShowInfoKey.Init();
	m_vStartDrawPos = e_vStartDrawPos;
}

cQuickInfo::~cQuickInfo()
{

}

void	cQuickInfo::Update()
{
	m_ShowInfoKey.SingnalProcess();
	if( m_ShowInfoKey.IsSatisfiedCondition() )
	{
		m_ShowInfoKey.Init();
		m_bShowInfo = !m_bShowInfo;
	}
}

void	cQuickInfo::Render()
{
	if( m_bShowInfo )
	{
		RenderFilledRectangle(Vector2(this->m_vStartDrawPos.x,this->m_vStartDrawPos.y),400,400,Vector4(1,0,1,1),0);
		float	l_fHeightGap = 30.f;
		Vector2	l_vDistance(0,0);
		int	l_iCoinToScore = cFishApp::m_spControlPanel->GetCoinToScore();
		int64	l_i64TotalRevenue		= *cFishApp::m_spControlPanel->m_pi64LatestRevenue/l_iCoinToScore;
		int64	l_i64TotalToastCoin		= *cFishApp::m_spControlPanel->m_pi64LatestCoinIn/l_iCoinToScore;
		int64	l_i64TotalRefudCoin		= *cFishApp::m_spControlPanel->m_pi64LatestCoinOut/l_iCoinToScore;
		int64	l_i64TotalExchangeIn	= *cFishApp::m_spControlPanel->m_pi64LatestExchangeIn/l_iCoinToScore;
		int64	l_i64TotalExchangeOut	= *cFishApp::m_spControlPanel->m_pi64LatestExchangeOut/l_iCoinToScore;
		int64	l_i64TotalTicket		= *cFishApp::m_spControlPanel->m_pi64LatestTicket/l_iCoinToScore;
		int64	l_i64LatestRevenue		= *cFishApp::m_spControlPanel->m_pi64LatestRevenue/l_iCoinToScore;

		l_i64TotalRevenue		+=	*cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalRevenue")->GetInt64();
		l_i64TotalToastCoin		+=	*cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalToastCoin")->GetInt64();
		l_i64TotalRefudCoin		+=	*cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalRefudCoin")->GetInt64();
		l_i64TotalExchangeIn	+=	*cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalExchangeIn")->GetInt64();
		l_i64TotalExchangeOut	+=	*cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalExchangeOut")->GetInt64();
		l_i64TotalTicket		+=	*cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalTicket")->GetInt64();	
		



		std::wstring	l_strDescription;
		l_strDescription = L"全部總盈餘(幣) :"; l_strDescription+= ValueToStringW(l_i64TotalRevenue);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		l_strDescription = L"本次報帳週期盈餘(幣) :"; l_strDescription+= ValueToStringW(l_i64LatestRevenue);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		l_strDescription = L"全部投幣(幣) :"; l_strDescription+= ValueToStringW(l_i64TotalToastCoin);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		l_strDescription = L"全部退幣(幣) :"; l_strDescription+= ValueToStringW(l_i64TotalRefudCoin);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		l_strDescription = L"全部上分(幣) :"; l_strDescription+= ValueToStringW(l_i64TotalExchangeIn);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		l_strDescription = L"全部下分(幣) :"; l_strDescription+= ValueToStringW(l_i64TotalExchangeOut);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		l_strDescription = L"全部彩票(幣) :"; l_strDescription+= ValueToStringW(l_i64TotalTicket);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		
		int64	l_i64RestTimeToReport = cFishApp::m_spControlPanel->GetRestTimeToReport();
		int	l_iDay = (int)(l_i64RestTimeToReport/60/60/24);
		int	l_iHour = (int)(l_i64RestTimeToReport/60/60)-(l_iDay*24);
		int	l_iMin = (int)(l_i64RestTimeToReport/60)-(l_iDay*24*60+l_iHour*60);
		l_strDescription = L"剩餘運行時間:"; l_strDescription += UT::ComposeMsgByFormat(L"%d 天 %d 時 %d分",l_iDay,l_iHour,l_iMin);
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;

		l_strDescription = L"線號 :"; l_strDescription += cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"LineNumber")?cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"LineNumber")->ConvertToString():L"5168";
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;

		l_strDescription = L"版本號 :"; l_strDescription += cFishApp::m_spControlPanel->m_wchVersion.c_str();
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
		
		l_strDescription = L"機器號碼 :"; l_strDescription += cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable( L"MachineNumber" )->ConvertToString();
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vStartDrawPos.x+l_vDistance.x,this->m_vStartDrawPos.y+l_vDistance.y,l_strDescription.c_str());l_vDistance.y += l_fHeightGap;
	}
}