#include "../stdafx.h"
#include "ImageWithSlider.h"
#include "GameApp.h"
#include "../Image/MPDI.h"
namespace FATMING_CORE
{
	cImageSlider::cImageSlider(cPuzzleImageUnit*e_pIcon,cPuzzleImage*e_pPuzzleImage,Vector4 e_vTotalViewRect,Vector3 e_vIconPos,cImageButton*e_pCancelButton,bool e_bHorizontal)
	:cScroller(e_pPuzzleImage,e_vTotalViewRect,e_pCancelButton,e_bHorizontal,e_pIcon,&e_vIconPos)
	{
	}

	cImageSlider::~cImageSlider()
	{
		SAFE_DELETE(m_pIcon);
	}

	void    cImageSlider::Init()
	{
		if( m_pIcon )
		{
			if( m_pIcon->Type() == cImageButton::TypeID )
				dynamic_cast<cImageButton*>(m_pIcon)->SetColor(Vector4::One);
		}
		cScroller::Init();
	}

	void    cImageSlider::Update(float e_fElpaseTime)
	{
		cScroller::Update(e_fElpaseTime);
	}

	void    cImageSlider::Render()
	{
		cScroller::Render();
		this->m_pIcon->Render();
		int	l_iCount = Count();
		cImageButton*l_pImage = 0;
		Vector3	l_vPos;
		if(this->m_bDirectionIsHorizontal)
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pImage = (cImageButton*)this->m_ObjectList[i];
				l_vPos = l_pImage->GetPos();
				if(l_vPos.x+*l_pImage->GetWidth()>=this->m_vCollisionRange.x)
				{
					if( l_vPos.x>this->m_vCollisionRange.z )
					{
						break;
					}
					cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y+*l_pImage->GetHeight(),L"Cost:100");
				}
			}
		}
		else
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pImage = (cImageButton*)this->m_ObjectList[i];
				l_vPos = l_pImage->GetPos();
				if(l_vPos.y+*l_pImage->GetHeight()>=this->m_vCollisionRange.y)
				{
					if( l_vPos.y>this->m_vCollisionRange.w )
					{
						break;
					}
					cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y+*l_pImage->GetHeight(),L"Cost:100");
				}
			}
		}
		if(this->m_pCancelButton)
			this->m_pCancelButton->Render();
	}

	void    cImageSlider::DebugRender()
	{
		cScroller::DebugRender();
	}


	cImageSliderTabPages::cImageSliderTabPages(Vector4 e_vWorkingZone,cMultiPathDynamicImage*e_pSliderBarBK,cImageButton*e_pCancelButton)
	:cTabPageControl(e_vWorkingZone,e_pCancelButton)
	{
		m_pSliderBarBK = e_pSliderBarBK;
		if( m_pSliderBarBK )
		{
			m_pSliderBarBK->Init();
			m_pSliderBarBK->Update(EPSIONAL);
		}
	}

	cImageSliderTabPages::~cImageSliderTabPages()
	{

	}

	//void    cImageSliderTabPages::MouseMove(int e_iPosX,int e_iPosY)
	//{
	//	this->m_pTopClickMouseBehavior->MouseMove(e_iPosX,e_iPosY);
	//}
	//
	//void    cImageSliderTabPages::MouseDown(int e_iPosX,int e_iPosY)
	//{
	//	this->m_pTopClickMouseBehavior->MouseDown(e_iPosX,e_iPosY);
	//}

	//void    cImageSliderTabPages::MouseUp(int e_iPosX,int e_iPosY)
	//{
	//	if(this->m_MouseMoveData.DownUpDistance()<5.f)
	//	{
	//		if( !m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY) )
	//		{
	//			this->m_bClickMouseBehaviorSatisfied = true;
	//			m_pTopClickMouseBehavior = 0;
	//			return;
	//		}
	//		else
	//		{
	//			int	l_iCount = this->Count();
	//			for( int i=0;i<l_iCount;++i )
	//			{
	//				if(this->m_ObjectList[i]->IsCollideIcon(e_iPosX,e_iPosY))
	//				{
	//					SetWorkingTabPage(i);
	//					return;
	//				}
	//			}
	//		}
	//	}
	//	if( m_pTopClickMouseBehavior )
	//		this->m_pTopClickMouseBehavior->MouseUp(e_iPosX,e_iPosY);
	//}

	void    cImageSliderTabPages::Update(float e_fElpaseTime)
	{
		cTabPageControl::Update(e_fElpaseTime);
	}

	void    cImageSliderTabPages::Render()
	{
		m_pSliderBarBK->Render();
		cTabPageControl::Render();
		//if(this->m_pTopClickMouseBehavior)
		//	m_pTopClickMouseBehavior->Render();
		//cImageSlider*l_pImageSlider = 0;
		//int	l_iCount = this->Count();
		//for( int i=0;i<l_iCount;++i )
		//{
		//	l_pImageSlider = this->m_ObjectList[i];
		//	if( l_pImageSlider )
		//		l_pImageSlider->RenderIcon();
		//}
	}

	void	cImageSliderTabPages::SetFocusItem(int e_iTabPageIndex,cPuzzleImageUnit*e_pPIUnit)
	{
		SetWorkingTabPage(e_iTabPageIndex);
	}
	//
	//void	cImageSliderTabPages::SetWorkingTabPage(int e_iTabPageIndex)
	//{
	//	if( m_pTopClickMouseBehavior )
	//	{
	//		m_pTopClickMouseBehavior->GetIcon()->SetColor(Vector4::One);
	//	}
	//	this->m_pTopClickMouseBehavior = this->GetObject(e_iTabPageIndex);
	//	Vector4	l_vColor = Vector4::One/2.f;
	//	m_pTopClickMouseBehavior->GetIcon()->SetColor(l_vColor);
	//}
	//
	//NamedTypedObject*	cImageSliderTabPages::GetSelectedObject()
	//{
	//	if( !m_pTopClickMouseBehavior ) 
	//		return 0;
	//	return m_pTopClickMouseBehavior->GetObject(m_pTopClickMouseBehavior->GetSelectedIndex()); 
	//}
//end namespace FATMING_CORE
}