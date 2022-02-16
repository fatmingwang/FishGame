#include "../stdafx.h"
#include "Sliderbar.h"
#include "GameApp.h"
#include "../Image/SimplePrimitive.h"
namespace FATMING_CORE
{
	const wchar_t*         cSliderBar::TypeID( L"cSliderBar" );

	cSliderBar::cSliderBar(Vector3	e_vPos,cBaseImage*e_pBar,cBaseImage*e_pSlider)
	{
		this->m_eOrientation = eO_HORIZONTAL;
		m_pBar = 0;
		m_pSlider = 0;
		if( e_pBar )
		{
			m_pBar = new cBaseImage(e_pBar);
			m_pSlider = new cBaseImage(e_pSlider);
			this->m_vCollisionRange = Vector4(0,0,(float)*m_pBar->GetWidth(),(float)*m_pBar->GetHeight());
		}
		else
		{
			m_vCollisionRange = Vector4(0,0,100,20);
		}
		m_fMaxValue = 0.f;
		m_fMinValue = 100.f;
		//m_fBarLength = 100.f;
		m_fCurrentValue = 0.f;
		if( m_eOrientation == eO_HORIZONTAL )
			this->m_fCurrentBarValue = this->m_vCollisionRange.Width();
		else
			this->m_fCurrentBarValue = this->m_vCollisionRange.Height();
		this->SetPos(e_vPos);
	}

	cSliderBar::~cSliderBar()
	{
		SAFE_DELETE(m_pBar);
		SAFE_DELETE(m_pSlider);
	}

	void	cSliderBar::Update(float e_fElpaseTime)
	{

	}

	void	cSliderBar::Render()
	{
		if( this->m_pBar )
		{
			this->m_pSlider->Render();
			m_pBar->Render();
		}
		float	l_fHeight = cGameApp::m_spGlyphFontRender->GetGlyphReader()->GetCharInfo((int)0).fHeight;
		cGameApp::m_spGlyphFontRender->RenderFont(this->m_vCollisionRange.x,
			this->m_vCollisionRange.y-l_fHeight-10.f,ValueToStringW((int)this->m_fCurrentValue));
	}

	void	cSliderBar::DebugRender()
	{
		GLRender::RenderRectangle(Vector2(m_vCollisionRange.x,m_vCollisionRange.y),m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4::One);
		//if( m_pSlider )
		{
			Vector3	l_vPos(this->m_vCollisionRange.x,m_vCollisionRange.y,0.f);
			float	l_fWidth = 100.f;
			float	l_fHeight = 100.f;
			if( this->m_eOrientation == eO_HORIZONTAL )
			{
				l_vPos.x = m_vCollisionRange.z-m_fCurrentBarValue;
				l_fWidth = m_vCollisionRange.Width()/(this->m_fMaxValue-this->m_fMinValue);
				l_fHeight = m_vCollisionRange.Height();
			}
			else
			{
				l_vPos.y = m_vCollisionRange.w-m_fCurrentBarValue;
				l_fWidth = m_vCollisionRange.Width();
				l_fHeight = m_vCollisionRange.Height()/(this->m_fMaxValue-this->m_fMinValue);
			}
			GLRender::RenderRectangle(Vector2(l_vPos.x,l_vPos.y),l_fWidth,l_fHeight,Vector4::One);
		}
	}

	void	cSliderBar::SetValue(float e_fValue)
	{
		if(e_fValue>this->m_fMaxValue)
		{
			e_fValue = m_fMaxValue;
		}
		else
		if(e_fValue<this->m_fMinValue)
		{
			e_fValue = m_fMinValue;
		}
		this->m_fCurrentValue = e_fValue;
		assert(m_fMaxValue>m_fMinValue);
		float	l_fPercentage = e_fValue/(m_fMaxValue-m_fMinValue);
		if( this->m_eOrientation == eO_HORIZONTAL )
		{
			this->m_fCurrentBarValue = UT::LERP<float>(0,this->m_vCollisionRange.z-this->m_vCollisionRange.x,l_fPercentage);
			//if in debug render comment this.......?
			m_fCurrentBarValue = m_vCollisionRange.z -m_fCurrentBarValue;
			if(this->m_pSlider)
				m_pSlider->SetPos(Vector2(m_fCurrentBarValue,m_pSlider->GetLocalPositionPointer()->y));
		}
		else
		{
			this->m_fCurrentBarValue = UT::LERP<float>(0,this->m_vCollisionRange.w-this->m_vCollisionRange.y,l_fPercentage);
			//if in debug render comment this.......?
			m_fCurrentBarValue = m_vCollisionRange.w -m_fCurrentBarValue;
			if(this->m_pSlider)
				m_pSlider->SetPos(Vector2(m_pSlider->GetLocalPositionPointer()->x,m_fCurrentBarValue));
		}
	}
	void	cSliderBar::Slide(int e_iPosX,int e_iPosY)
	{
		if( this->m_eOrientation == eO_HORIZONTAL )
		{
			this->m_fCurrentBarValue = this->m_vCollisionRange.z-e_iPosX;
		}
		else
		{
			this->m_fCurrentBarValue = this->m_vCollisionRange.w-e_iPosY;
		}
		float	l_fPercentage;
		if( this->m_eOrientation == eO_HORIZONTAL )
			l_fPercentage = m_fCurrentBarValue/(m_vCollisionRange.Width());
		else
			l_fPercentage = m_fCurrentBarValue/(m_vCollisionRange.Height());
		this->m_fCurrentValue = UT::LERP<float>(this->m_fMinValue,this->m_fMaxValue,l_fPercentage);	
		if(this->m_pSlider)
		{
			if( this->m_eOrientation == eO_HORIZONTAL )
				m_pSlider->SetPos(Vector2((float)e_iPosX,m_pSlider->GetLocalPositionPointer()->y));
			else
				m_pSlider->SetPos(Vector2(m_pSlider->GetLocalPositionPointer()->x,(float)e_iPosY));
		}
	}

	void    cSliderBar::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		Slide(e_iPosX,e_iPosY);
	}

	void    cSliderBar::InternalMouseMove(int e_iPosX,int e_iPosY)
	{
		Slide(e_iPosX,e_iPosY);
	}

	void    cSliderBar::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		Slide(e_iPosX,e_iPosY);
	}

	void	cSliderBar::SetPos(Vector3 e_vPos)
	{
		if( m_pBar )
		{
			Vector3	l_vOffsetPos = *m_pBar->GetLocalPositionPointer()-*m_pSlider->GetLocalPositionPointer();
			this->m_pBar->SetPos(e_vPos);
			if(this->m_eOrientation == eO_HORIZONTAL)
				l_vOffsetPos.y-=(*m_pBar->GetHeight())/2.f;
			else
				l_vOffsetPos.x-=(*m_pBar->GetWidth())/2.f;
			this->m_pSlider->SetPos(l_vOffsetPos+e_vPos);
		}
		this->m_vCollisionRange.x+= e_vPos.x;
		this->m_vCollisionRange.y+= e_vPos.y;
		this->m_vCollisionRange.z+= e_vPos.x;
		this->m_vCollisionRange.w+= e_vPos.y;
	}
//end namespace
}