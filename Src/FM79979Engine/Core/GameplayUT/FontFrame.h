#ifndef FONT_FRAME_H
#define FONT_FRAME_H

#include "../AllImageInclude.h"
#include "Mouse_Touch.h"
namespace FATMING_CORE
{
	class	cFontWithFrame
	{
	protected:
		GET_SET_DEC(cGlyphFontRender*,m_pGlyphFontRender,GetGlyphFontRender,SetGlyphFontRender);
		cBaseImage*		m_pConnectRadianImage;
		cBaseImage*		m_pLineImage;
		Vector4			m_vFrameRange;
		std::wstring	m_strText;
		std::wstring*	m_pstrPressedText;
		const WCHAR*	m_pstrCurrentText;
		Vector4*		m_pvTextBGColor;
		Vector2			m_vRenderSize;
	public:
		cFontWithFrame(cGlyphFontRender*e_pGlyphFontRender,const WCHAR* e_strText,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		virtual ~cFontWithFrame();
		const WCHAR*	GetTxt(){ return m_strText.c_str(); }
		void			SetText(const WCHAR*e_strText);
		void			SetPressedText(const WCHAR*e_strText);
		void			Render(int e_iX,int e_iY);
		//void			Render();
		Vector4*		GetTextBGColor(){ return m_pvTextBGColor; }
		void			SetTextBGColor(Vector4*e_pvColor);
	};
	//sample
	//cFontWithFrame	l_FontWithFrame(cGameApp::m_spGlyphFontRender,L"cFontWithFrame Test");
	//Vector4	l_vBGColor(0,0,0,1);
	//l_FontWithFrame.SetTextBGColor(&l_vBGColor);
	//l_FontWithFrame.Render(500,500);

	//cTextButton just reference from cImageButton
	class	cTextButton:public cFontWithFrame,public cClickMouseBehavior
	{
		UT::sTimeCounter	m_TC;
        virtual void		InternalMouseDown(int e_iPosX,int e_iPosY);
		virtual void		InternalMouseLeave(int e_iPosX,int e_iPosY);
        virtual void		InternalMouseUp(int e_iPosX,int e_iPosY);
		//
		UT::sTimeCounter	m_TCForRepeat;
		GET_SET_DEC(bool,m_bPressedRepeatMouseUp,IsPressedRepeatMouseUp,SetPressedRepeatMouseUp);
		Vector4				m_vPressedColor;
		void				SetColor(Vector4 e_vColor);
		GET_SET_DEC(Vector4,m_vFontColor,GetFontColor,SetFontColor);
	public:
		DEFINE_TYPE_INFO();
		cTextButton(Vector2 e_vRenderPos,cGlyphFontRender*e_pGlyphFontRender,Vector4 e_vPressedColor,const WCHAR* e_strText,cBaseImage*e_pConnectRadianImage = 0,cBaseImage*e_pLineImage = 0);
		cTextButton(cTextButton*e_pTextButton);
		CLONE_MYSELF(cTextButton);
		virtual ~cTextButton();
		virtual bool		IsSatisfiedCondition();
		virtual void		Init();
		virtual void		InitRepeat();
		virtual void		Update(float e_fElpaseTime);
		virtual	void		Render();
	};
	//sample
	//cTextButton	*g_pTextButton = 0;
	//SAFE_DELETE(g_pTextButton);
	//g_pTextButton = new cTextButton(Vector2(200,200),cGameApp::m_spGlyphFontRender,Vector4(0.5,0.5,0.5,1),L"cFontWithFrame Test");
	//g_pTextButton->Update(e_fElpaseTime);
	//Vector4	l_vBGColor(0,0.5,0.5,1);
	//g_pTextButton->SetTextBGColor(&l_vBGColor);
	//g_pTextButton->Render();
	//g_pTextButton->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	//g_pTextButton->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	//g_pTextButton->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
//end namespace FATMING_CORE
}
#endif