#include "../stdafx.h"
#include "TabPage.h"
#include "GameApp.h"
#include "Scroller.h"
#include "ImageWithSlider.h"
#include "MultiPage.h"
namespace FATMING_CORE
{
	cTabPage::cTabPage()
	{
		m_pIcon = 0;
		m_pBG = 0;
	}

	cTabPage::cTabPage(Vector4 e_vWorkingZone,cClickMouseBehavior*e_pCancelButton,cBaseImage*e_pIcon,Vector3 *e_pvIconPos,bool e_bForceSetCancelButtonPos):cMouse_TouchWithCancelButton(e_pCancelButton,e_vWorkingZone,e_bForceSetCancelButtonPos)
	{
		m_pIcon = 0;
		m_pBG = 0;
		if( e_pIcon )
		{
			m_pIcon = dynamic_cast<cClickMouseBehavior*>(e_pIcon->Clone());
			if( e_pvIconPos )
			{
				if( m_pIcon->Type() == cImageButton::TypeID )
					((cImageButton*)(m_pIcon))->SetPos(*e_pvIconPos);
			}
		}
	}

	cTabPage::cTabPage(cTabPage*e_pTabPage):cMouse_TouchWithCancelButton(e_pTabPage)
	{
		m_pBG = e_pTabPage->m_pBG;
		m_pIcon = 0;
		if( e_pTabPage->m_pIcon )
			m_pIcon = dynamic_cast<cClickMouseBehavior*>(e_pTabPage->m_pIcon->Clone());
		m_bCheckOutOfWorking = e_pTabPage->m_bCheckOutOfWorking;
	}

	cTabPage::~cTabPage()
	{
		SAFE_DELETE(m_pIcon);
	}
	void	cTabPage::SetIconColor(Vector4 e_vColor)
	{
		if( m_pIcon )
		{
			if( m_pIcon->Type() == cImageButton::TypeID )
				((cImageButton*)m_pIcon)->SetColor(e_vColor);
		}
	}
	void	cTabPage::RenderIcon()
	{
		if( m_pIcon )
		{
			m_pIcon->Render();
		}
	}

	bool	cTabPage::IsCollideIcon(int e_iPosX,int e_iPosY)
	{
		if( m_pIcon )
		{
			return m_pIcon->Collide(e_iPosX,e_iPosY);
			//return m_pIcon->CollideTextureWithTransform(e_iPosX,e_iPosY,false);
		}
		return false;
	}

	void	cTabPage::SetTabPageIcon(cClickMouseBehavior*e_pIcon)
	{
		SAFE_DELETE(m_pIcon);
		m_pIcon = dynamic_cast<cClickMouseBehavior*>(e_pIcon->Clone());
	}

	//add one by one,it might be orther things nit only a image
	//bool	cTabPage::AddObject(cClickMouseBehavior*e_pClickMouseBehavior)
	//{
	//	if( !e_pClickMouseBehavior )
	//		return false;
	//	this->AddObjectNeglectExist(e_pClickMouseBehavior);
	//	return true;
	//}

	bool	cTabPage::AddObject(cPuzzleImage*e_pPuzzleImage,bool e_bClearOldData)
	{
		if( !e_pPuzzleImage )
			return false;
		if( e_bClearOldData )
			this->Destroy();
		int	l_iCount = e_pPuzzleImage->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cPuzzleImageUnit*l_pPIUnit = e_pPuzzleImage->GetObject(i);
			cImageButton*l_pImageButton = new cImageButton(l_pPIUnit,Vector3::Zero);
			this->AddObjectNeglectExist(l_pImageButton);
		}
		return true;
	}
	bool	cTabPage::AddObject(cBaseImage*e_pBaseImage,bool e_bDeleteData)
	{
		if( !e_pBaseImage )
			return false;
		cImageButton*l_pImageButton = new cImageButton(e_pBaseImage,e_pBaseImage->GetPos());
		if( e_bDeleteData )
			delete e_pBaseImage;
		this->AddObjectNeglectExist(l_pImageButton);
		return true;
	}
	bool	cTabPage::AddObject(std::vector<cBaseImage*>*e_pImageList,bool e_bClearOldData)
	{
		if( !e_pImageList )
			return false;
		if( e_bClearOldData )
			this->Destroy();
		int	l_iCount = (int)e_pImageList->size();
		for( int i=0;i<l_iCount;++i )
		{
			cBaseImage*l_pBaseImage = (*e_pImageList)[i];
			cImageButton*l_pImageButton = new cImageButton(l_pBaseImage,Vector3::Zero);
			this->AddObjectNeglectExist(l_pImageButton);
		}
		return true;
	}

	cTabPageControl::cTabPageControl(Vector4 e_vWorkingZone,cImageButton*e_pCancelButton,cCueToStartCurveWithTime*e_pBG,bool e_bForceSetCancelButtonPos):cMouse_TouchWithCancelButton(e_pCancelButton,e_vWorkingZone,e_bForceSetCancelButtonPos)
	{
		m_pBG = 0;
		if( e_pBG )
		{
			m_pBG = dynamic_cast<cRenderObjectBehavior*>(e_pBG->Clone());
			m_pBG->Init();
		}
	}

	cTabPageControl::~cTabPageControl()
	{
		SAFE_DELETE(m_pBG);
	}
	//============================
	//
	//============================
	void    cTabPageControl::MouseDown(int e_iPosX,int e_iPosY)
	{
		cMouse_TouchWithCancelButton::MouseDown(e_iPosX,e_iPosY);
	}
	//============================
	//
	//============================
	void    cTabPageControl::MouseUp(int e_iPosX,int e_iPosY)
	{
		cMouse_TouchWithCancelButton::MouseUp(e_iPosX,e_iPosY);
		float	l_fDownUpDis = this->m_MouseMoveData.DownUpDistance();
		if(l_fDownUpDis<5.f)

		{
			int	l_iCount = this->Count();
			for( int i=0;i<l_iCount;++i )
			{//it is possible not a tabpage
				cTabPage*l_pTabPage = dynamic_cast<cTabPage*>(this->m_ObjectList[i]);
				if( l_pTabPage )
				{
					if(l_pTabPage->IsCollideIcon(e_iPosX,e_iPosY))
					{
						SetWorkingTabPage(i);
						this->m_ObjectList[i]->SetSatisfiedCondition(false);
						return;
					}
				}
			}
		}
		//cMouse_TouchWithCancelButton::MouseUp(e_iPosX,e_iPosY);
	}
	//============================
	//
	//============================
	void    cTabPageControl::Init()
	{
		cMouse_TouchWithCancelButton::Init();
		SetWorkingTabPage(0);
		if( m_pBG )
		{
			m_pBG->Init();
			m_pBG->Update(EPSIONAL);
		}
	}
	//============================
	//
	//============================
	void    cTabPageControl::Update(float e_fElpaseTime)
	{
		this->MiscUpdate(e_fElpaseTime);
		if(this->m_pTopClickMouseBehavior)
		{
			m_pTopClickMouseBehavior->Update(e_fElpaseTime);
			if( m_pTopClickMouseBehavior->IsSatisfiedCondition() )
			{
				cTabPage*l_pTabPage = dynamic_cast<cTabPage*>(m_pTopClickMouseBehavior);
				if( l_pTabPage )
				{
					if( l_pTabPage->GetCurrentWorkingObject() || m_bOutOfWorkingZone )
					{
						this->m_bClickMouseBehaviorSatisfied = true;
					}
				}
				else
				{
					this->m_bClickMouseBehaviorSatisfied = true;
				}
			}
		}
	}
	//============================
	//
	//============================
	void    cTabPageControl::Render()
	{
		if( m_pBG )
			m_pBG->Render();
		if(this->m_pTopClickMouseBehavior)
			m_pTopClickMouseBehavior->Render();
		cTabPage*l_pTabPage = 0;
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			l_pTabPage = dynamic_cast<cTabPage*>(m_ObjectList[i]);
			if( l_pTabPage )
			{
				l_pTabPage->RenderIcon();
				//l_pTabPage->Render();
			}
			else
				m_ObjectList[i]->Render();

		}
		this->MiscRender();
	}
	//============================
	//
	//============================
	void	cTabPageControl::SetWorkingTabPage(int e_iTabPageIndex)
	{
		if( m_pTopClickMouseBehavior )
		{
			if( m_pTopClickMouseBehavior->Type() == cTabPage::TypeID )
			{
				if( dynamic_cast<cTabPage*>(m_pTopClickMouseBehavior)->GetTabPageIcon() )
				{
					dynamic_cast<cTabPage*>(m_pTopClickMouseBehavior)->SetIconColor(Vector4::One);
				}
			}
		}
		if( e_iTabPageIndex > -1 && this->Count() > e_iTabPageIndex )
		{
			this->m_pTopClickMouseBehavior = this->GetObject(e_iTabPageIndex);
			//Vector4	g_vImageButtonClickedColor(0.8f,0.8f,0.f,1);
			Vector4	l_vColor(1.f,1.f,0.f,1);
			cTabPage*l_pTabPage = dynamic_cast<cTabPage*>(m_pTopClickMouseBehavior);
			if( l_pTabPage->GetTabPageIcon() )
				l_pTabPage->SetIconColor(l_vColor);
		}
	}
	//============================
	//
	//============================
	int	cTabPageControl::GetWorkingTabPageIndex()
	{
		if( !m_pTopClickMouseBehavior )
			return -1;
		return this->GetObjectIndexByPointer(m_pTopClickMouseBehavior);
	}
	NamedTypedObject*	cTabPageControl::GetSelectedObject()
	{
		if( !m_pTopClickMouseBehavior ) 
			return 0;
		return dynamic_cast<cTabPage*>(m_pTopClickMouseBehavior)->GetCurrentWorkingObject();
	}

	//============================
	//
	//============================
#define	TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND			L"BG"				//
#define	TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT		L"SliderObject"		//each object's position
#define	TABPAGE_CONTROLLER_DATA_MPDI_MULTIPAGE_OBJECT	L"MultiPageObject"	//each object's position
#define	TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON		L"CB"				//
#define	TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE			L"PriorPage"		//proio page button position
#define	TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE			L"NextPage"			//next page button position
#define	TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS	L"IconObjectPos"	//icon object's position

	cTabPageControl*	ParseFileToSliderBar(char*e_strMPDIFile,char*e_strPIFileName,cMPDIList**e_pMPDIList)
	{
		cTabPageControl*l_pTabPageControl = 0;
		std::wstring	l_strTemp = cMPDIList::FileToMPDIListName(e_strMPDIFile);
		const WCHAR*l_strMPDIName = l_strTemp.c_str();
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIList(e_strMPDIFile,l_strMPDIName);
		if( e_pMPDIList )
		{
			*e_pMPDIList = l_pMPDIList;
		}
		std::wstring	l_strPIName = UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strPIFileName));
		cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(e_strPIFileName,(WCHAR*)l_strPIName.c_str());
		//cMPDI*l_pMPDI = l_pMPDIList->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND);
		//........default only 1 file
		cMPDI*l_pMPDI = l_pMPDIList->GetObject(0);
		return ParseFileToSliderBar(l_pMPDI,l_pPI,e_pMPDIList);
	}

	cTabPageControl*	ParseFileToSliderBar(cMPDI*e_pMPDI,cPuzzleImage*e_pPI,cMPDIList**e_pMPDIList)
	{
		cTabPageControl*l_pTabPageControl = 0;
		cPuzzleImage*l_pPI = e_pPI;
		cMPDI*l_pMPDI = e_pMPDI;
		cCueToStartCurveWithTime*l_pBG = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND);
		if( !l_pBG )
		{
			UT::ErrorMsg("BG is not exist at "," MPDI");
		}
		Vector2	l_vSize = l_pBG->GetPointData(0)->Size/2.f;
		cCueToStartCurveWithTime*l_pFirstObject = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT);
		cCueToStartCurveWithTime*l_pCancelButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON);
		cImageButton*l_pImageButton = 0;
		Vector3	l_vBGPos = l_pBG->GetOriginalPointList()[0];
		if( l_pCancelButton )
		{
			Vector3	l_vPos = l_pCancelButton->GetOriginalPointList()[0];
			Vector2	l_vSize = l_pCancelButton->GetPointData(0)->Size/2.f;
			l_vPos -= l_vSize;
			l_pImageButton = new cImageButton( l_pCancelButton->GetPointData(0)->GetPuzzleImageUnit(),l_vPos );
		}
		Vector4 l_vBGRange = Vector4(l_vBGPos.x-l_vSize.x,l_vBGPos.y-l_vSize.y,l_vBGPos.x+l_vSize.x,l_vBGPos.y+l_vSize.y );
		l_pTabPageControl = new cTabPageControl(l_vBGRange,l_pImageButton,l_pBG,false);
		bool	l_bHorizontal = false;
		if(l_pFirstObject)
		{
			Vector3	l_vFirstPos = l_pFirstObject->GetOriginalPointList()[0];
			l_vBGRange.x = l_vFirstPos.x-l_pFirstObject->GetPointData(0)->Size.x/2.f;
			l_vBGRange.y = l_vFirstPos.y-l_pFirstObject->GetPointData(0)->Size.y/2.f;
			if( l_pFirstObject->GetOriginalPointList().size() == 2 )
			{
				Vector3	l_vSceondPos = l_pFirstObject->GetOriginalPointList()[1];
				if( l_vSceondPos.y == l_vFirstPos.y )
				{
					l_bHorizontal = true;
				}
			}
		}
		cScroller*l_pScroller = new cScroller(l_pPI,l_vBGRange,0,l_bHorizontal);
		l_pTabPageControl->AddObjectNeglectExist(l_pScroller);
		return l_pTabPageControl;
	}
	//============================
	//
	//============================
	cTabPageControl*	ParseFileToSliderBarWithTabPagesController(cMPDI*e_pMPDI,cPuzzleImage*e_pIconPI,std::vector<cPuzzleImage*>*e_pPageIcon)
	{
		if( !e_pIconPI || !e_pMPDI )
			return 0;
		cTabPageControl*l_pTabPageControl = 0;
		cPuzzleImage*l_pIconPI = e_pIconPI;
		cMPDI*l_pMPDI = e_pMPDI;
		cCueToStartCurveWithTime*l_pBG = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND);
		if( !l_pBG )
		{
			UT::ErrorMsg("BG is not exist at "," MPDI");
		}
		Vector2	l_vSize = l_pBG->GetPointData(0)->Size/2.f;
		cCueToStartCurveWithTime*l_pFirstObject = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT);
		cCueToStartCurveWithTime*l_pIconObjectPos = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS);
		cCueToStartCurveWithTime*l_pCancelButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON);
		cImageButton*l_pImageButton = 0;
		if( l_pCancelButton )
		{
			Vector3	l_vPos = l_pCancelButton->GetOriginalPointList()[0];
			Vector2	l_vSize = l_pCancelButton->GetPointData(0)->Size/2.f;
			l_vPos -= l_vSize;
			l_pImageButton = new cImageButton( l_pCancelButton->GetPointData(0)->GetPuzzleImageUnit(),l_vPos );
		}
		Vector3	l_vBGPos = l_pBG->GetOriginalPointList()[0];
		Vector4 l_vBGRange = Vector4(l_vBGPos.x-l_vSize.x,l_vBGPos.y-l_vSize.y,l_vBGPos.x+l_vSize.x,l_vBGPos.y+l_vSize.y );
		l_pTabPageControl = new cTabPageControl(l_vBGRange,l_pImageButton,l_pBG);
		bool	l_bHorizontal = false;
		if(l_pFirstObject)
		{
			Vector3	l_vFirstPos = l_pFirstObject->GetOriginalPointList()[0];
			l_vBGRange.x = l_vFirstPos.x;
			l_vBGRange.y = l_vFirstPos.y;
			if( l_pFirstObject->GetOriginalPointList().size() == 2 )
			{
				Vector3	l_vSceondPos = l_pFirstObject->GetOriginalPointList()[1];
				if( l_vSceondPos.y == l_vFirstPos.y )
				{
					l_bHorizontal = true;
				}
			}
		}
		int	l_iCount = l_pIconPI->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cPuzzleImage*l_pPI = (*e_pPageIcon)[i];
			Vector3	l_vIconPos;
			if(l_pIconObjectPos)
			{
				assert( (int)l_pIconObjectPos->GetOriginalPointList().size() >= l_iCount );
				l_vIconPos = l_pIconObjectPos->GetOriginalPointList()[i];
			}
			cScroller*l_pScroller = new cScroller(l_pPI,l_vBGRange,0,l_bHorizontal,l_pIconPI->GetObject(i),&l_vIconPos);
			l_pTabPageControl->AddObjectNeglectExist(l_pScroller);
		}
		return l_pTabPageControl;	
	}
	cTabPageControl*	ParseFileToSliderBarWithTabPagesController(char*e_strMPDIFile,char*e_strIconPIFileName)
	{
		cTabPageControl*l_pTabPageControl = 0;
		std::wstring	l_strTemp = cMPDIList::FileToMPDIListName(e_strMPDIFile);
		const WCHAR*l_strMPDIName = l_strTemp.c_str();
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIList(e_strMPDIFile,l_strMPDIName);
		std::wstring	l_strIconPIName = UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strIconPIFileName));
		cPuzzleImage*l_pIconPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(e_strIconPIFileName,(WCHAR*)l_strIconPIName.c_str());
		//assume only one MPDI.
		cMPDI*l_pMPDI = l_pMPDIList->GetObject(0);
		cCueToStartCurveWithTime*l_pBG = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND);
		if( !l_pBG )
		{
			UT::ErrorMsg("BG is not exist at "," MPDI");
		}
		Vector2	l_vSize = l_pBG->GetPointData(0)->Size/2.f;
		cCueToStartCurveWithTime*l_pFirstObject = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT);
		cCueToStartCurveWithTime*l_pIconObjectPos = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS);
		cCueToStartCurveWithTime*l_pCancelButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON);
		cImageButton*l_pImageButton = 0;
		Vector3	l_vBGPos = l_pBG->GetOriginalPointList()[0];
		if( l_pCancelButton )
		{
			Vector3	l_vPos = l_pCancelButton->GetOriginalPointList()[0];
			Vector2	l_vSize = l_pCancelButton->GetPointData(0)->Size/2.f;
			l_vPos -= l_vSize;
			l_pImageButton = new cImageButton( l_pCancelButton->GetPointData(0)->GetPuzzleImageUnit(),l_vPos );
		}
		Vector4 l_vBGRange = Vector4(l_vBGPos.x-l_vSize.x,l_vBGPos.y-l_vSize.y,l_vBGPos.x+l_vSize.x,l_vBGPos.y+l_vSize.y );
		l_pTabPageControl = new cTabPageControl(l_vBGRange,l_pImageButton,l_pBG);
		bool	l_bHorizontal = false;
		if(l_pFirstObject)
		{
			Vector3	l_vFirstPos = l_pFirstObject->GetOriginalPointList()[0];
			l_vBGRange.x = l_vFirstPos.x;
			l_vBGRange.y = l_vFirstPos.y;
			if( l_pFirstObject->GetOriginalPointList().size() == 2 )
			{
				Vector3	l_vSceondPos = l_pFirstObject->GetOriginalPointList()[1];
				if( l_vSceondPos.y == l_vFirstPos.y )
				{
					l_bHorizontal = true;
				}
			}
		}
		std::string	l_strPIFileName = UT::GetDirectoryWithoutFileName(e_strIconPIFileName);
		int	l_iCount = l_pIconPI->Count();
		for( int i=0;i<l_iCount;++i )
		{
			const WCHAR*l_strPIName = l_pIconPI->GetObject(i)->GetName();
			std::string	l_strPIFileName = l_strPIFileName.c_str();
			l_strPIFileName += UT::WcharToChar(l_strPIName);
			cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage((char*)l_strPIFileName.c_str(),l_strPIName);
			Vector3	l_vIconPos;
			if(l_pIconObjectPos)
			{
				assert( (int)l_pIconObjectPos->GetOriginalPointList().size() >= l_iCount );
				l_vIconPos = l_pIconObjectPos->GetOriginalPointList()[i];
			}
			cScroller*l_pScroller = new cScroller(l_pPI,l_vBGRange,0,l_bHorizontal,l_pPI->GetObject(i),&l_vIconPos);
			l_pTabPageControl->AddObjectNeglectExist(l_pScroller);
		}
		return l_pTabPageControl;
	}
	//============================
	//
	//============================
	cTabPageControl*	ParseFileToMultiPages(char*e_strMPDIFile,char*e_strPIFileName)
	{
		cTabPageControl*l_pTabPageControl = 0;
		std::wstring	l_strTemp = cMPDIList::FileToMPDIListName(e_strMPDIFile);
		const WCHAR*l_strMPDIName = l_strTemp.c_str();
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIList(e_strMPDIFile,l_strMPDIName);
		std::wstring	l_strPIName = UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strPIFileName));
		cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(e_strPIFileName,(WCHAR*)l_strPIName.c_str());
		cMPDI*l_pMPDI = l_pMPDIList->GetObject(0);
		return ParseFileToMultiPages(l_pMPDI,l_pPI);
	}

	cTabPageControl*	ParseFileToMultiPages(cMPDI*e_pMPDI,cPuzzleImage*e_pPI)
	{
		cTabPageControl*l_pTabPageControl = 0;
		cPuzzleImage*l_pPI = e_pPI;
		cMPDI*l_pMPDI = e_pMPDI;
		cCueToStartCurveWithTime*l_pBG = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND);
		if( !l_pBG )
		{
			UT::ErrorMsg("BG is not exist at "," MPDI");
		}
		Vector2	l_vSize = l_pBG->GetPointData(0)->Size/2.f;
		cCueToStartCurveWithTime*l_pFirstObject = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_MULTIPAGE_OBJECT);
		cCueToStartCurveWithTime*l_pCancelButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON);
		cCueToStartCurveWithTime*l_pNextPageButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE);
		cCueToStartCurveWithTime*l_pPriorPageButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE);
		cImageButton*l_pImageButton = 0;
		Vector3	l_vBGPos = l_pBG->GetOriginalPointList()[0];
		if( l_pCancelButton )
		{
			Vector3	l_vPos = l_pCancelButton->GetOriginalPointList()[0];
			Vector2	l_vSize = l_pCancelButton->GetPointData(0)->Size/2.f;
			l_vPos -= l_vSize;
			l_pImageButton = new cImageButton( l_pCancelButton->GetPointData(0)->GetPuzzleImageUnit(),l_vPos );
		}
		Vector4 l_vBGRange = Vector4(l_vBGPos.x-l_vSize.x,l_vBGPos.y-l_vSize.y,l_vBGPos.x+l_vSize.x,l_vBGPos.y+l_vSize.y );
		l_pTabPageControl = new cTabPageControl(l_vBGRange,l_pImageButton,l_pBG);
		Vector3	l_vFirstPos = l_pFirstObject->GetOriginalPointList()[0];
		int	l_iRow = 1;
		int	l_iColumn = 1;
		std::vector<Vector3>	l_vPositionList = l_pFirstObject->GetOriginalPointList();
		if( l_vPositionList.size()>1 )
		{
			for( int i=1;i<(int)l_vPositionList.size();++i )
			{
				if( l_vFirstPos.x == l_vPositionList[i].x )
				{
					++l_iRow;
				}
				else
				if( l_vFirstPos.y == l_vPositionList[i].y )
				{
					++l_iColumn;
				}
			}
		}
		Vector2	l_vDistance(l_vPositionList[1].x-l_vFirstPos.x,l_vPositionList[l_iColumn].y-l_vFirstPos.y);
		cMultiPages*l_pMultiPages = new cMultiPages(0,l_pPI,l_pNextPageButton->GetPointData(0)->GetPuzzleImageUnit(),
			l_pNextPageButton->GetOriginalPointList()[0]-l_pNextPageButton->GetPointData(0)->Size/2.f,
			l_pPriorPageButton->GetOriginalPointList()[0]-l_pNextPageButton->GetPointData(0)->Size/2.f,
			l_vFirstPos,l_vDistance,l_iRow,l_iColumn,l_vBGRange,0);
		l_pTabPageControl->AddObjectNeglectExist(l_pMultiPages);
		return l_pTabPageControl;	
	}
	//============================
	//
	//============================
	cTabPageControl*	ParseFileToMultiPagesWithTabPagesController(char*e_strMPDIFile,char*e_strIconPIFileName)
	{
		cTabPageControl*l_pTabPageControl = 0;
		std::wstring	l_strTemp = cMPDIList::FileToMPDIListName(e_strMPDIFile);
		const WCHAR*l_strMPDIName = l_strTemp.c_str();
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIList(e_strMPDIFile,l_strMPDIName);
		std::wstring	l_strIconPIName = UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strIconPIFileName));
		cPuzzleImage*l_pIconPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(e_strIconPIFileName,(WCHAR*)l_strIconPIName.c_str());
		cMPDI*l_pMPDI = l_pMPDIList->GetObject(0);
		cCueToStartCurveWithTime*l_pBG = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND);
		if( !l_pBG )
		{
			UT::ErrorMsg("BG is not exist at "," MPDI");
		}
		Vector2	l_vSize = l_pBG->GetPointData(0)->Size/2.f;
		cCueToStartCurveWithTime*l_pFirstObject = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT);
		cCueToStartCurveWithTime*l_pIconObjectPos = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS);
		cCueToStartCurveWithTime*l_pCancelButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON);
		cCueToStartCurveWithTime*l_pNextPageButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE);
		cCueToStartCurveWithTime*l_pPriorPageButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE);
		cImageButton*l_pImageButton = 0;
		Vector3	l_vBGPos = l_pBG->GetOriginalPointList()[0];
		if( l_pCancelButton )
		{
			Vector3	l_vPos = l_pCancelButton->GetOriginalPointList()[0];
			Vector2	l_vSize = l_pCancelButton->GetPointData(0)->Size/2.f;
			l_vPos -= l_vSize;
			l_pImageButton = new cImageButton( l_pCancelButton->GetPointData(0)->GetPuzzleImageUnit(),l_vPos );
		}
		Vector4 l_vBGRange = Vector4(l_vBGPos.x-l_vSize.x,l_vBGPos.y-l_vSize.y,l_vBGPos.x+l_vSize.x,l_vBGPos.y+l_vSize.y );
		l_pTabPageControl = new cTabPageControl(l_vBGRange,l_pImageButton,l_pBG);
		Vector3	l_vFirstPos = l_pFirstObject->GetOriginalPointList()[0];
		int	l_iRow = 1;
		int	l_iColumn = 1;
		std::vector<Vector3>	l_vPositionList = l_pFirstObject->GetOriginalPointList();
		if( l_vPositionList.size()>1 )
		{
			for( int i=1;i<(int)l_vPositionList.size();++i )
			{
				if( l_vFirstPos.x == l_vPositionList[i].x )
				{
					++l_iRow;
				}
				else
				if( l_vFirstPos.y == l_vPositionList[i].y )
				{
					++l_iColumn;
				}
			}
		}
		Vector2	l_vDistance(l_vPositionList[1].x-l_vFirstPos.x,l_vPositionList[l_iColumn].y-l_vFirstPos.y);
		int	l_iCount = l_pIconPI->Count();
		for( int i=0;i<l_iCount;++i )
		{
			const WCHAR*l_strPIName = l_pIconPI->GetObject(i)->GetName();
			std::string	l_strPIFileName = l_strPIFileName.c_str();
			l_strPIFileName += UT::WcharToChar(l_strPIName);
			cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage((char*)l_strPIFileName.c_str(),l_strPIName);

			cMultiPages*l_pMultiPages = new cMultiPages(0,l_pPI,l_pNextPageButton->GetPointData(0)->GetPuzzleImageUnit(),l_pNextPageButton->GetOriginalPointList()[0],
			l_pPriorPageButton->GetOriginalPointList()[0],l_vFirstPos,l_vDistance,l_iRow,l_iColumn,l_vBGRange,0);
			l_pTabPageControl->AddObjectNeglectExist(l_pMultiPages);
		}
		return l_pTabPageControl;
	}
	cTabPageControl*	ParseFileToMultiPagesWithTabPagesController(cMPDIList*e_pMPDIList,cPuzzleImage*e_pIconPI,std::vector<cPuzzleImage*>*e_pPageIcon)
	{
		if( !e_pIconPI || !e_pMPDIList )
			return 0;
		cTabPageControl*l_pTabPageControl = 0;
		cPuzzleImage*l_pIconPI = e_pIconPI;
		cMPDI*l_pMPDI = e_pMPDIList->GetObject(0);
		cCueToStartCurveWithTime*l_pBG = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND);
		if( !l_pBG )
		{
			UT::ErrorMsg("BG is not exist at "," MPDI");
		}
		Vector2	l_vSize = l_pBG->GetPointData(0)->Size/2.f;
		cCueToStartCurveWithTime*l_pFirstObject = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT);
		cCueToStartCurveWithTime*l_pIconObjectPos = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS);
		cCueToStartCurveWithTime*l_pCancelButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON);
		cCueToStartCurveWithTime*l_pNextPageButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE);
		cCueToStartCurveWithTime*l_pPriorPageButton = l_pMPDI->GetObject(TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE);
		cImageButton*l_pImageButton = 0;
		Vector3	l_vBGPos = l_pBG->GetOriginalPointList()[0];
		if( l_pCancelButton )
		{
			Vector3	l_vPos = l_pCancelButton->GetOriginalPointList()[0];
			Vector2	l_vSize = l_pCancelButton->GetPointData(0)->Size/2.f;
			l_vPos -= l_vSize;
			l_pImageButton = new cImageButton( l_pCancelButton->GetPointData(0)->GetPuzzleImageUnit(),l_vPos );
		}
		Vector4 l_vBGRange = Vector4(l_vBGPos.x-l_vSize.x,l_vBGPos.y-l_vSize.y,l_vBGPos.x+l_vSize.x,l_vBGPos.y+l_vSize.y );
		l_pTabPageControl = new cTabPageControl(l_vBGRange,l_pImageButton,l_pBG);
		Vector3	l_vFirstPos = l_pFirstObject->GetOriginalPointList()[0];
		int	l_iRow = 1;
		int	l_iColumn = 1;
		std::vector<Vector3>	l_vPositionList = l_pFirstObject->GetOriginalPointList();
		if( l_vPositionList.size()>1 )
		{
			for( int i=1;i<(int)l_vPositionList.size();++i )
			{
				if( l_vFirstPos.x == l_vPositionList[i].x )
				{
					++l_iRow;
				}
				else
				if( l_vFirstPos.y == l_vPositionList[i].y )
				{
					++l_iColumn;
				}
			}
		}
		Vector2	l_vDistance(l_vPositionList[1].x-l_vFirstPos.x,l_vPositionList[l_iColumn].y-l_vFirstPos.y);
		int	l_iCount = l_pIconPI->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cPuzzleImage*l_pPI = (*e_pPageIcon)[i];
			cMultiPages*l_pMultiPages = new cMultiPages(l_pIconPI->GetObject(i),l_pPI,l_pNextPageButton->GetPointData(0)->GetPuzzleImageUnit(),l_pNextPageButton->GetOriginalPointList()[0],
			l_pPriorPageButton->GetOriginalPointList()[0],l_vFirstPos,l_vDistance,l_iRow,l_iColumn,l_vBGRange,0);
			l_pTabPageControl->AddObjectNeglectExist(l_pMultiPages);
		}
		return l_pTabPageControl;	
	}
//end namespace
}