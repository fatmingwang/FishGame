#include "../stdafx.h"
#include "FontFrame.h"
#include "ImageButton.h"
namespace FATMING_CORE
{
	const wchar_t*         cTextButton::TypeID( L"cTextButton" );

	cFontWithFrame::cFontWithFrame(cGlyphFontRender*e_pGlyphFontRender,const WCHAR*e_strText,cBaseImage*e_pConnectRadianImage,cBaseImage*e_pLineImage)
	{
		m_pstrCurrentText = 0;
		m_pGlyphFontRender = e_pGlyphFontRender;
		m_pConnectRadianImage = e_pConnectRadianImage;
		m_pLineImage = e_pLineImage;
		SetText(e_strText);
		m_pvTextBGColor = 0;
		m_pstrPressedText = 0;
	}

	cFontWithFrame::~cFontWithFrame()
	{
		SAFE_DELETE(m_pstrPressedText);
		SAFE_DELETE(m_pvTextBGColor);
	}

	void	cFontWithFrame::SetPressedText(const WCHAR*e_strText)
	{
		SAFE_DELETE(m_pstrPressedText);
		m_pstrPressedText = new std::wstring;
		*m_pstrPressedText = e_strText;
	}

	void	cFontWithFrame::SetText(const WCHAR*e_strText)
	{
		m_strText = e_strText;
		m_pstrCurrentText = m_strText.c_str();
		m_vRenderSize = Vector2::Zero;
		if( m_pGlyphFontRender )
			m_vRenderSize = this->m_pGlyphFontRender->GetRenderSize(e_strText);
	}

	void	cFontWithFrame::SetTextBGColor(Vector4*e_pvColor)
	{
		if( e_pvColor )
		{
			if( !m_pvTextBGColor )
				m_pvTextBGColor = new Vector4(*e_pvColor);
			else
				*m_pvTextBGColor = *e_pvColor;
		}
	}

	//void	cFontWithFrame::Render()
	//{
	//	Render(this->m_vPos.x,this->m_vPos.y);
	//}

	void	cFontWithFrame::Render(int e_iX,int e_iY)
	{
		if( !m_pvTextBGColor )
		{
			Vector2	l_vLeltUp((float)e_iX,(float)e_iY);
			Vector2	l_vRightUp(e_iX+m_vRenderSize.x,(float)e_iY);
			Vector2	l_vLeltDown((float)e_iX,e_iY+m_vRenderSize.y);
			Vector2	l_vRightDown(e_iX+m_vRenderSize.x,e_iY+m_vRenderSize.y);
		}
		else
		{
			RenderFilledRectangle(Vector2((float)e_iX,(float)e_iY),m_vRenderSize.x,m_vRenderSize.y,*m_pvTextBGColor,0.f);
		}
		if( m_pGlyphFontRender  && m_pstrCurrentText )
		{
			m_pGlyphFontRender->RenderFont(e_iX,e_iY,m_pstrCurrentText);
		}
	}

	cTextButton::cTextButton(Vector2 e_vRenderPos,cGlyphFontRender*e_pGlyphFontRender,Vector4 e_vPressedColor,const WCHAR* e_strText,cBaseImage*e_pConnectRadianImage,cBaseImage*e_pLineImage)
		:cFontWithFrame(e_pGlyphFontRender,e_strText,e_pConnectRadianImage,e_pLineImage)
	{
		m_vFontColor = Vector4::One;
		m_bPressedRepeatMouseUp = false;
		this->SetLocalPosition(Vector3(e_vRenderPos.x,e_vRenderPos.y,0.f));
		m_vCollisionRange = Vector4(0,0,m_vRenderSize.x,m_vRenderSize.y);
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		m_TCForRepeat.SetTargetTime(cImageButton::m_sfRepeatTime);
		this->m_pvTextBGColor = new Vector4(e_vPressedColor);
		m_vPressedColor = g_vImageButtonClickedColor;
	}

	cTextButton::cTextButton(cTextButton*e_pTextButton):cFontWithFrame(e_pTextButton->m_pGlyphFontRender,e_pTextButton->m_strText.c_str(),e_pTextButton->m_pConnectRadianImage,e_pTextButton->m_pLineImage),
		cClickMouseBehavior(e_pTextButton)
	{
		SetLocalPosition(e_pTextButton->GetLocalPosition());
		if( e_pTextButton->m_pstrPressedText )
			this->SetPressedText(e_pTextButton->m_pstrPressedText->c_str());
		m_bPressedRepeatMouseUp = e_pTextButton->m_bPressedRepeatMouseUp;
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		m_TCForRepeat.SetTargetTime(cImageButton::m_sfRepeatTime);
		this->m_pvTextBGColor = new Vector4(*e_pTextButton->m_pvTextBGColor);
		m_vPressedColor = e_pTextButton->m_vPressedColor;
		m_vFontColor = e_pTextButton->m_vFontColor;
	}


	cTextButton::~cTextButton()
	{
	
	}

	void	cTextButton::SetColor(Vector4 e_vColor)
	{
		if(m_pConnectRadianImage)
			m_pConnectRadianImage->SetColor(e_vColor);
		if(m_pLineImage)
			m_pLineImage->SetColor(e_vColor);
		m_vPressedColor = e_vColor;
	}

	void    cTextButton::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		SetColor(*this->m_pvTextBGColor);
		if( this->m_pstrPressedText )
		{
			m_pstrCurrentText = this->m_pstrPressedText->c_str();
		}
	}

	void    cTextButton::InternalMouseLeave(int e_iPosX,int e_iPosY)
	{
		SetColor(Vector4::One);
		m_pstrCurrentText = this->m_strText.c_str();
	}

	void    cTextButton::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		SetColor(this->m_vFontColor);
		m_pstrCurrentText = this->m_strText.c_str();
		if( m_TC.fRestTime >0.00001f )
			m_TC.Start();
	}

	bool    cTextButton::IsSatisfiedCondition()
	{
		if( this->m_bPressedRepeatMouseUp && this->m_eObjectMouseBehavior == eOMB_HORVER )
		{
			return this->m_TCForRepeat.bTragetTimrReached;
		}
		if( cClickMouseBehavior::IsSatisfiedCondition() )
		{
			return this->m_TC.bTragetTimrReached;
		}
		return false;
	}

	void    cTextButton::Init()
	{
		eObjectMouseBehavior	l_eObjectMouseBehavior = m_eObjectMouseBehavior;
		m_TC.Start();
		cClickMouseBehavior::Init();
		InternalMouseUp(0,0);
		if( l_eObjectMouseBehavior == eOMB_HORVER )
			m_eObjectMouseBehavior = eOMB_HORVER;
	}

	void    cTextButton::InitRepeat()
	{

	}

	void    cTextButton::Update(float e_fElpaseTime)
	{
		if( this->m_bPressedRepeatMouseUp && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ))
		{
			if( m_TCForRepeat.bTragetTimrReached )
			{
				m_TCForRepeat.Start();
				m_eObjectMouseBehavior = eOMB_HORVER;
			}
			else
				this->m_TCForRepeat.Update(e_fElpaseTime);
		}
		if( cClickMouseBehavior::IsSatisfiedCondition() )
		{
			if( !m_TC.bTragetTimrReached )
			{
				m_TC.Update(e_fElpaseTime);
				if( m_TC.bTragetTimrReached )
				{
					InternalMouseUp(0,0);
					if(m_pDoButtonGoal_Callback)
						m_pDoButtonGoal_Callback(this);
				}
			}
		}
	}

	void	cTextButton::Render()
	{
		if( m_pGlyphFontRender )
			this->m_pGlyphFontRender->SetFontColor(m_vPressedColor);
		Vector3	l_vPos = GetLocalPosition();
		cFontWithFrame::Render((int)l_vPos.x,(int)l_vPos.y);
		if( m_pGlyphFontRender )
			m_pGlyphFontRender->SetFontColor(Vector4::One);
	}
}