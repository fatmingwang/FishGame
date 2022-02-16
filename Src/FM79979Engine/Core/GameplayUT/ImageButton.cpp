#include "../stdafx.h"
#include "../AllImageInclude.h"
#include "Mouse_Touch.h"
#include "ImageButton.h"
#include "../GameplayUT/GameApp.h"
#include "ObjectXMLParser.h"
namespace FATMING_CORE
{
	const wchar_t*     cImageButton::TypeID( L"cImageButton" );
	Vector4	g_vImageButtonClickedColor(0.5f,0.5f,0.5f,1);
	cImageButton::cImageButton():cBaseImage((char*)0)
	{
		m_pPressedImage = 0;
		m_pFinishImage = 0;	
	}

	cImageButton::cImageButton(cPuzzleImageUnit*e_pImage,Vector3 e_vPos):cBaseImage(e_pImage)
	{
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		Frame::SetLocalPosition(e_vPos);
	}

	cImageButton::cImageButton(cBaseImage*e_pImage,Vector3 e_vPos):cBaseImage(e_pImage)
	{
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		Frame::SetLocalPosition(e_vPos);
		//Vector2	l_vLeftPos(this->m_OffsetPos.x+e_vPos.x,this->m_OffsetPos.y+e_vPos.y);
		//m_vCollisionRange = Vector4(l_vLeftPos.x,l_vLeftPos.y,(float)m_iWidth+l_vLeftPos.x,(float)m_iHeight+l_vLeftPos.y);
	}

	cImageButton::cImageButton(cImageButton*e_pImageButton):cBaseImage(e_pImageButton),cClickMouseBehavior(e_pImageButton)
	{
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		if( e_pImageButton->m_pPressedImage )
			m_pPressedImage = dynamic_cast<cRenderObjectBehavior*>(e_pImageButton->m_pPressedImage->Clone());
		if( e_pImageButton->m_pFinishImage )
			m_pFinishImage = dynamic_cast<cRenderObjectBehavior*>(e_pImageButton->m_pFinishImage->Clone());
		m_TC.SetTargetTime(e_pImageButton->m_TC.fTargetTime);
		Frame::SetLocalPosition(e_pImageButton->GetPos());
		//this->SetParent(e_pImageButton->GetParent());
	}

	void	SubMPDIAssignData(cSubMPDI*e_pSubMPDI,cImageButton*e_pImageButton)
	{
		e_pSubMPDI->Init();
		e_pSubMPDI->Update(0.000001f);
		Vector3	l_vPos = e_pSubMPDI->GetOriginalPointList()[0];
		Vector4	l_vCollisionRange;
		POINT*l_pOffset = e_pImageButton->GetOffsetPos();
		if(e_pSubMPDI->GetRenderData(g_fMPDIOptmizeRenderVertices,g_fMPDIOptmizeRenderUV,g_fMPDIOptmizeRenderColor))
		{
			//left up
			l_vCollisionRange.x = g_fMPDIOptmizeRenderVertices[6];
			l_vCollisionRange.y = g_fMPDIOptmizeRenderVertices[7];
			//right down
			l_vCollisionRange.z = g_fMPDIOptmizeRenderVertices[12];
			l_vCollisionRange.w = g_fMPDIOptmizeRenderVertices[13];
			l_vPos.x = l_vCollisionRange.x;
			l_vPos.y= l_vCollisionRange.y;
			e_pImageButton->SetPos(l_vPos);
		}
		else
		{
			l_vPos -= e_pSubMPDI->GetPointData(0)->Size/2.f;
			e_pImageButton->SetPos(l_vPos);
			l_vCollisionRange = Vector4(l_vPos.x+l_pOffset->x,l_vPos.y+l_pOffset->y,l_vPos.x+*e_pImageButton->GetWidth()+l_pOffset->x,l_vPos.y+*e_pImageButton->GetHeight()+l_pOffset->y);
		}
		e_pImageButton->SetCollisionRange(l_vCollisionRange);
	}

	cImageButton::cImageButton(cSubMPDI*e_pSubMPDI,DoButtonGoal_Callback e_pDoButtonGoal_Callback):cBaseImage(e_pSubMPDI->GetPointData(0)->GetPuzzleImageUnit())
	{
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		SubMPDIAssignData(e_pSubMPDI,this);
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		m_pDoButtonGoal_Callback = e_pDoButtonGoal_Callback;
	}

	cImageButton::cImageButton(cMPDI*e_pMPDI,DoButtonGoal_Callback e_pDoButtonGoal_Callback):cBaseImage(e_pMPDI->GetObject(0)->GetPointData(0)->GetPuzzleImageUnit())
	{
		m_pPressedImage = 0;
		m_pFinishImage = 0;
		SubMPDIAssignData(e_pMPDI->GetObject(0),this);
		m_TC.SetTargetTime(MOUSE_UP_TIME_OFFSET);
		m_pDoButtonGoal_Callback = e_pDoButtonGoal_Callback;
	}
	bool	cImageButton::ProcessFinishAndPressedImage(TiXmlElement*e_pTiXmlelement)
	{
		const WCHAR*l_strElementValue = e_pTiXmlelement->Value();
		cRenderObjectBehavior*l_pImage = 0;
		if( !wcscmp(l_strElementValue,L"FinishImage") )
		{
			l_pImage = GetRenderObjectBehavior(e_pTiXmlelement->FirstChildElement());
			if( l_pImage )
				this->SetFinishImage(l_pImage);
		}
		else
		if( !wcscmp(l_strElementValue,L"PressedImage") )
		{
			l_pImage = GetRenderObjectBehavior(e_pTiXmlelement->FirstChildElement());
			if( l_pImage )
				this->SetPressedImage(l_pImage);
		}
		if( l_pImage )
			return true;
		return false;
	}
	//<cImageButton Name="TestButton" PI="Image/Dice/Dice.pi" Image="Call" Pos="" />
	//<cImageButton Name="TestButton" MPDIList="Image/Dice/Dice.mpdi" MPDI="aabb" />
	//<cImageButton Name="TestButton" MPDIList="Image/Dice/Dice.mpdi" MPDI="aabb" SubMPDI="" />
	//<cImageButton Name="TestButton" PI="Image/Dice/Dice.pi" Image="Call" Pos="">
	//	<FinishImage>
	//		<MPDI>
	//	</FinishImage>
	//	<PressedImage>
	//		<MPDI>
	//	</PressedImage>
	//</cImageButton>
	cImageButton*		cImageButton::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cImageButton::TypeID);
		cImageButton*l_pImageButton = 0;
		const WCHAR*l_strPI = e_pElement->Attribute(L"PI");
		if( l_strPI )
		{
			cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(l_strPI);
			const WCHAR*l_strPIUnit = e_pElement->Attribute(L"Image");
			const WCHAR*l_strPos = e_pElement->Attribute(L"Pos");
			if( l_pPI && l_strPIUnit && l_strPos )
			{
				Vector3	l_vPos = GetVector3(l_strPos);
				cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(l_strPIUnit);
				l_pImageButton = new cImageButton(l_pPIUnit,l_vPos);
			}
		}
		else
		{
			const WCHAR*l_strMPDIList = e_pElement->Attribute(L"MPDIList");
			if( l_strMPDIList )
			{
				cMPDIList*l_pMPDILIst = cGameApp::GetMPDIListByFileName(l_strMPDIList);
				const WCHAR*l_strMPDI = e_pElement->Attribute(L"MPDI");
				const WCHAR*l_strSubMPDI = e_pElement->Attribute(L"SubMPDI");
				if( l_pMPDILIst && l_strMPDI )
				{
					cMPDI*l_pMPDI = l_pMPDILIst->GetObject(l_strMPDI);
					if( l_pMPDI )
					{
						if( !l_strSubMPDI )
							l_pImageButton = new cImageButton(l_pMPDI);
						else
						{
							cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(l_strSubMPDI);
							l_pImageButton = new cImageButton(l_pSubMPDI);
						}
					}
				}
			}
		}
		if( !l_pImageButton )
		{
			UT::ErrorMsg(L"cImageButton::GetMe",L"xml data error");
		}
		else
		{
			const WCHAR*l_strName = e_pElement->Attribute(L"Name");
			if( l_strName )
			{
				l_pImageButton->SetName(l_strName);
			}
			e_pElement = e_pElement->FirstChildElement();
			while( e_pElement )
			{
				if(!l_pImageButton->ProcessFinishAndPressedImage(e_pElement))
				{
					UT::ErrorMsg(L"cImageButton::GetMe not finis or pressed image element",e_pElement->Value());
				}
			}
		}
		return l_pImageButton;
	}

	cImageButton::~cImageButton()
	{
		SAFE_DELETE(m_pPressedImage);
		SAFE_DELETE(m_pFinishImage);
	}

	void	cImageButton::SetTransformInternalData()
	{
		cRenderObjectBehavior::SetTransformInternalData();
		Vector2	l_vLeftPos(this->m_OffsetPos.x+this->m_pvPos->x,this->m_OffsetPos.y+m_pvPos->y);
		m_vCollisionRange = Vector4(l_vLeftPos.x,l_vLeftPos.y,(float)m_iWidth+l_vLeftPos.x,(float)m_iHeight+l_vLeftPos.y);
		//if(this->m_pFinishImage)
			//m_pFinishImage->SetPos(this->GetPos());
		//if(this->m_pPressedImage)
			//m_pPressedImage->SetPos(this->GetPos());
	}
	//bool    cImageButton::InternalCollide(int e_iPosX,int e_iPosY)
	//{
	//	e_iPosX -= (int)m_vPos.x;
	//	e_iPosY	-= (int)m_vPos.y;
	//	return this->m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY);
	//	//return this->CollideTextureWithTransform(e_iPosX,e_iPosY,false);
	//}

    void    cImageButton::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		SetColor(g_vImageButtonClickedColor);
	}

    void    cImageButton::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_TC.fRestTime >0.00001f )
			m_TC.Start();
	}

    void    cImageButton::InternalMouseLeave(int e_iPosX,int e_iPosY)
	{
		SetColor(Vector4::One);
	}

	void    cImageButton::InitRepeat()
	{

	}

	bool	cImageButton::InternalCollide(int e_iPosX,int e_iPosY)
	{
		return this->m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY);
	}

	void    cImageButton::Init()
	{
		bool	l_bCollided = this->m_bCollided;
		bool	l_bKeyDown = this->m_bKeyDown;
		eObjectMouseBehavior	l_eObjectMouseBehavior = m_eObjectMouseBehavior;
		m_TC.Start();
		cClickMouseBehavior::Init();
		SetColor(Vector4::One);
		//make sure repeat works
		if( l_eObjectMouseBehavior == eOMB_HORVER )
		{
			m_eObjectMouseBehavior = eOMB_HORVER;
			this->m_bCollided = l_bCollided;
			this->m_bKeyDown = l_bKeyDown;
		}
	}

	void	cImageButton::SetFinishImage(cRenderObjectBehavior*e_pImage)
	{
		SAFE_DELETE(m_pFinishImage);
		m_pFinishImage = dynamic_cast<cRenderObjectBehavior*>(e_pImage->Clone());
		//m_pFinishImage->Init();
		//m_pFinishImage->Update(EPSIONAL);
		m_pFinishImage->SetParent(this);
	}

	void	cImageButton::SetPressedImage(cRenderObjectBehavior*e_pImage)
	{
		SAFE_DELETE(m_pPressedImage);
		m_pPressedImage = dynamic_cast<cRenderObjectBehavior*>(e_pImage->Clone());
		//m_pPressedImage->Init();
		//m_pPressedImage->Update(EPSIONAL);
		m_pPressedImage->SetParent(this);
	}


	bool    cImageButton::IsSatisfiedCondition()
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

	void    cImageButton::Update(float e_fElpaseTime)
	{
		cClickMouseBehavior::Update(e_fElpaseTime);
		if( cClickMouseBehavior::IsSatisfiedCondition() )
		{
			if( !m_TC.bTragetTimrReached )
			{
				m_TC.Update(e_fElpaseTime);
				if( m_TC.bTragetTimrReached )
				{
					SetColor(Vector4::One);
					if(m_pDoButtonGoal_Callback)
						m_pDoButtonGoal_Callback(this);
				}
			}
		}
	}

	void	cImageButton::Render()
	{
		//if( cClickMouseBehavior::IsSatisfiedCondition() && m_pPressedImage )
		if( m_pPressedImage && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ) )
		{
			//m_pPressedImage->SetPos(m_vPos);
			this->m_pPressedImage->Render();
		}
		else
		{
			if( this->IsSatisfiedCondition() && m_pFinishImage )
				m_pFinishImage->Render();
			else
				cBaseImage::Render((int)this->m_pvPos->x,(int)this->m_pvPos->y);
		}
	}

	//void	cImageButton::SetPosByImageCenter(Vector3 e_vPos)
	//{
	//	cTexData::SetPosByImageCenter(e_vPos);
	//	//this->m_vCollisionRange.x = e_vPos.x-this->m_iWidth/2.f;
	//	//this->m_vCollisionRange.y = e_vPos.y-this->m_iWidth/2.f;
	//	//this->m_vCollisionRange.z = e_vPos.x+this->m_iWidth/2.f;
	//	//this->m_vCollisionRange.w = e_vPos.y+this->m_iWidth/2.f;
	//}

	//void	cImageButton::SetPos(Vector3 e_vPos)
	//{
	//	cTexData::SetPos(e_vPos);
	//	//Vector2	l_vSize = m_vCollisionRange.Size();
	//	//this->m_vCollisionRange.x = e_vPos.x;
	//	//this->m_vCollisionRange.y = e_vPos.y;
	//	//this->m_vCollisionRange.z = e_vPos.x+l_vSize.x;
	//	//this->m_vCollisionRange.w = e_vPos.y+l_vSize.y;
	//}
//================================================
//
//================================================

cYesNoDialog::cYesNoDialog(cBaseImage*e_pYesImageButton,cBaseImage*e_pNoImageButton,cBaseImage*e_pBK,WCHAR*e_strDescription,Vector3 e_vYesPos,Vector3 e_vNoPos,Vector3 e_vBKPos,Vector3 e_vFontPos)
{
	this->m_vFontPos = e_vFontPos;
	m_vPos = Vector3::Zero;
	m_pYesImageButton = new cImageButton(e_pYesImageButton,e_vYesPos);
	m_pNoImageButton = new cImageButton(e_pNoImageButton,e_vNoPos);
	m_pBK = new cBaseImage(e_pBK);
	m_pBK->SetPos(e_vBKPos);
	m_strDescription = e_strDescription;
	m_eYesNoDialogResult = eYNDR_NONE;
}
#define		YES_NO_DIALOG_YES_BUTTON	L"Yes"
#define		YES_NO_DIALOG_NO_BUTTON		L"No"
#define		YES_NO_DIALOG_BG			L"BG"
#define		YES_NO_DIALOG_FONT			L"Font"

cYesNoDialog::cYesNoDialog(cMPDI*e_pMPDI,WCHAR*e_strDescription)
{
	cSubMPDI*l_pYesSubMPDI = e_pMPDI->GetObject(YES_NO_DIALOG_YES_BUTTON);
	cSubMPDI*l_pNoSubMPDI = e_pMPDI->GetObject(YES_NO_DIALOG_NO_BUTTON);
	cSubMPDI*l_pFontSubMPDI = e_pMPDI->GetObject(YES_NO_DIALOG_FONT);
	cSubMPDI*l_pBGSubMPDI = e_pMPDI->GetObject(YES_NO_DIALOG_BG);
	int	l_iImageIndex = (*l_pYesSubMPDI->GetPointDataList())[0]->iImageIndex;
	cPuzzleImageUnit*l_pPIUnit = (*l_pYesSubMPDI->GetPointDataList())[0]->pPI->GetObject(l_iImageIndex);
	m_pYesImageButton = new cImageButton(l_pPIUnit,l_pYesSubMPDI->GetOriginalPointList()[0]);
	l_iImageIndex = (*l_pNoSubMPDI->GetPointDataList())[0]->iImageIndex;
	l_pPIUnit = (*l_pNoSubMPDI->GetPointDataList())[0]->pPI->GetObject(l_iImageIndex);
	m_pNoImageButton = new cImageButton(l_pPIUnit,l_pNoSubMPDI->GetOriginalPointList()[0]);
	l_iImageIndex = (*l_pBGSubMPDI->GetPointDataList())[0]->iImageIndex;
	l_pPIUnit = (*l_pBGSubMPDI->GetPointDataList())[0]->pPI->GetObject(l_iImageIndex);
	m_pBK = new cBaseImage(l_pPIUnit);
	Vector2	l_vSize = l_pBGSubMPDI->GetPointData(0)->Size;
	m_pBK->SetWidth((int)l_vSize.x);
	m_pBK->SetHeight((int)l_vSize.y);
	m_pBK->SetPos(l_pBGSubMPDI->GetOriginalPointList()[0]);
	m_vPos = Vector3::Zero;
	m_strDescription = e_strDescription;
	this->m_vFontPos = l_pFontSubMPDI->GetOriginalPointList()[0];
	m_eYesNoDialogResult = eYNDR_NONE;
}

cYesNoDialog::cYesNoDialog(cYesNoDialog*e_pYesNoDialog)
{
	m_pYesImageButton = 0;
	m_pNoImageButton = 0;
	m_pBK = 0;
	this->m_vFontPos = e_pYesNoDialog->m_vFontPos;
	m_vPos = e_pYesNoDialog->m_vPos;
	if( e_pYesNoDialog->m_pYesImageButton )
		m_pYesImageButton = new cImageButton(e_pYesNoDialog->m_pYesImageButton);
	if(e_pYesNoDialog->m_pNoImageButton)
		m_pNoImageButton = new cImageButton(e_pYesNoDialog->m_pNoImageButton);
	if( e_pYesNoDialog->m_pBK )
	{
		m_pBK = new cBaseImage(e_pYesNoDialog->m_pBK);
	}
	m_strDescription = e_pYesNoDialog->m_strDescription;
	m_eYesNoDialogResult = eYNDR_NONE;
}

cYesNoDialog::~cYesNoDialog()
{
	SAFE_DELETE(m_pYesImageButton);
	SAFE_DELETE(m_pNoImageButton);
	SAFE_DELETE(m_pBK);
}

void    cYesNoDialog::Init()
{
	m_pYesImageButton->Init();
	m_pNoImageButton->Init();
	m_eYesNoDialogResult = eYNDR_NONE;
}
void    cYesNoDialog::Update(float e_fElpaseTime)
{
	m_pYesImageButton->Update(e_fElpaseTime);
	m_pNoImageButton->Update(e_fElpaseTime);
	if( m_pYesImageButton->IsSatisfiedCondition() )
	{
		m_eYesNoDialogResult = eYNDR_YES;
	}
	else
	if( m_pNoImageButton->IsSatisfiedCondition() )
	{
		m_eYesNoDialogResult = eYNDR_NO;
	}
}

void	cYesNoDialog::Render()
{
	m_pBK->Render();
	m_pYesImageButton->Render();
	m_pNoImageButton->Render();
	if( m_strDescription.length() )
		cGameApp::m_spGlyphFontRender->RenderFont(m_vFontPos.x,m_vFontPos.y,(WCHAR*)m_strDescription.c_str());
}

void    cYesNoDialog::MouseMove(int e_iPosX,int e_iPosY)
{
	m_pYesImageButton->MouseMove(e_iPosX,e_iPosY);
	m_pNoImageButton->MouseMove(e_iPosX,e_iPosY);
}

//first time into
void    cYesNoDialog::MouseDown(int e_iPosX,int e_iPosY)
{
	m_pYesImageButton->MouseDown(e_iPosX,e_iPosY);
	m_pNoImageButton->MouseDown(e_iPosX,e_iPosY);
}

//
void    cYesNoDialog::MouseUp(int e_iPosX,int e_iPosY)
{
	m_pYesImageButton->MouseUp(e_iPosX,e_iPosY);
	m_pNoImageButton->MouseUp(e_iPosX,e_iPosY);
}
//end namespace namespace FATMING_CORE
}