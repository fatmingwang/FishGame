#include "../stdafx.h"
#include "../AllImageInclude.h"
#include "Mouse_Touch.h"
#include "AssembleImageButton.h"
#include "../GameplayUT/GameApp.h"
#include "ObjectXMLParser.h"
const wchar_t*     cAssembleImageButton::TypeID( L"cAssembleImageButton" );
cAssembleImageButton::cAssembleImageButton()
{

}

cAssembleImageButton::~cAssembleImageButton()
{

}

cAssembleImageButton::cAssembleImageButton(cAssembleImageButton*e_pAssembleImageButton):cImageButton(e_pAssembleImageButton)
{
	this->CloneFromList(e_pAssembleImageButton);
}

void	cAssembleImageButton::SetTransformInternalData()
{
	float	l_fWidth = this->m_vCollisionRange.Width();	
	float	l_fHeight = this->m_vCollisionRange.Height();
	Vector3	l_vPos = this->GetWorldPosition();
	m_vCollisionRange.x = l_vPos.x;
	m_vCollisionRange.y = l_vPos.y;
	m_vCollisionRange.z = m_vCollisionRange.x+l_fWidth;
	m_vCollisionRange.w = m_vCollisionRange.y+l_fHeight;

}

//void    cAssembleImageButton::MouseMove(int e_iPosX,int e_iPosY)
//{
//
//}
//
//void    cAssembleImageButton::MouseDown(int e_iPosX,int e_iPosY)
//{
//
//}
//
//void    cAssembleImageButton::MouseUp(int e_iPosX,int e_iPosY)
//{
//
//}
void	cAssembleImageButton::Init()
{
	cBehaviorObjectList<cRenderObjectBehavior>::Init();
	cImageButton::Init();
	int	l_iCount = this->Count();
	Vector4	l_vDrawRect(FLT_MAX,FLT_MAX,FLT_MIN,FLT_MIN);
	for( int i=0;i<l_iCount;++i )
	{
		const WCHAR*l_strTypeID = GetObject(i)->Type();
		if( l_strTypeID == cBaseImage::TypeID || l_strTypeID == cPuzzleImageUnit::TypeID || cImageButton::TypeID == l_strTypeID )
		{
			cBaseImage*l_pBaseImage = (cBaseImage*)GetObject(i);
			Vector3	l_vPos = l_pBaseImage->GetWorldPosition();
			Vector4	l_vRect(l_vPos.x,l_vPos.y,l_vPos.x+*l_pBaseImage->GetWidth(),l_vPos.y+*l_pBaseImage->GetHeight());
			l_vDrawRect = GetBiggerRect(l_vDrawRect,l_vRect);
		}
		else
		if( l_strTypeID == cMPDI::TypeID)
		{
			cMPDI*l_pMPDI = (cMPDI*)GetObject(i);
			Vector2	l_vDrawSize = l_pMPDI->GetDrawSize();
			Vector3	l_vPos = l_pMPDI->GetWorldPosition();
			Vector3	l_vLastPos = l_pMPDI->GetLastFramePos();
			l_vPos += l_vLastPos;
			Vector4	l_vRect(l_vPos.x,l_vPos.y,l_vPos.x+l_vDrawSize.x,l_vPos.y+l_vDrawSize.y);
			l_vDrawRect = GetBiggerRect(l_vDrawRect,l_vRect);
		}
		else
		{
			UT::ErrorMsg(L"cAssembleImageButton not support this type",l_strTypeID);
		}
	}
	this->m_vCollisionRange = l_vDrawRect;
	this->m_iWidth = (int)m_vCollisionRange.Width();
	this->m_iHeight = (int)m_vCollisionRange.Height();
}

void	cAssembleImageButton::SetColor(Vector4 e_vColor)
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
		this->GetObject(i)->SetColor(e_vColor);
}

void	cAssembleImageButton::Update(float e_fElpaseTime)
{
	cBehaviorObjectList<cRenderObjectBehavior>::Update(e_fElpaseTime);
	cImageButton::Update(e_fElpaseTime);
}

void	cAssembleImageButton::Render()
{
	if( m_pPressedImage && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ) )
	{
		//m_pPressedImage->SetPos(m_vPos);
		this->m_pPressedImage->Render();
	}
	else
	if( this->IsSatisfiedCondition() && m_pFinishImage )
			m_pFinishImage->Render();
	else
		cBehaviorObjectList<cRenderObjectBehavior>::Render();
}

void	cAssembleImageButton::DebugRender()
{
	Vector3	l_vPos = this->GetWorldPosition();
	Vector4	l_vCollideRect = this->m_vCollisionRange;
	l_vCollideRect.Move(l_vPos);
	GLRender::RenderRectangle(Vector2(l_vCollideRect.x,l_vCollideRect.y),(float)this->m_iWidth,(float)this->m_iHeight,Vector4::One,this->GetRotation());
	int l_iCount = this->Count();
	for(int i=0;i<l_iCount;++i)
	{
		cBaseImage*l_cBaseImage = (cBaseImage*)m_ObjectList[i];
		l_cBaseImage->RenderRect();
		//this->m_ObjectList[i]->DebugRender();
	}    
}
//<cAssembleImageButton Name="aabb">
//	<MPDI....>
//	<PI		  File="" Image=""/>
//	<FinishImage>
//		<MPDI />
//	</FinishImage>
//	<PressImage>
//		<SubMPDI />
//	</PressImage>
//</cAssembleImageButton>

cAssembleImageButton*	cAssembleImageButton::GetMe(TiXmlElement*e_pElement)
{
	cAssembleImageButton*l_pAssembleImageButton = new cAssembleImageButton();
	ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cAssembleImageButton::TypeID);
	const WCHAR*l_strAssembleImageButtonName = e_pElement->Attribute(L"Name");
	if( l_strAssembleImageButtonName )
		l_pAssembleImageButton->SetName(l_strAssembleImageButtonName);
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		if(!l_pAssembleImageButton->ProcessFinishAndPressedImage(e_pElement))
		{
			cRenderObjectBehavior*l_pRenderObjectBehavior = GetRenderObjectBehavior(e_pElement);
			if( l_pRenderObjectBehavior )
			{
				l_pAssembleImageButton->AddObjectNeglectExist(l_pRenderObjectBehavior);
			}
			else
			{
				UT::ErrorMsg(e_pElement->Value(),L"cAssembleImageButton::GetMe not support!");
			}
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
	return l_pAssembleImageButton;
}