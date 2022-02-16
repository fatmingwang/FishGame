#include "../stdafx.h"
#include "../AllImageInclude.h"
#include "Mouse_TouchWithCancelButton.h"


#define	CANCEL_BUTTON_NAME	L"!@#$"
namespace FATMING_CORE
{
	cMouse_TouchWithCancelButton::cMouse_TouchWithCancelButton()
	{
		m_bCheckOutOfWorking = true;
		m_pCancelButton = 0;
		m_bOutOfWorkingZone = false;
		this->m_vCollisionRange = Vector4::Zero;
	}

	cMouse_TouchWithCancelButton::cMouse_TouchWithCancelButton(cClickMouseBehavior*e_pCancelButton,Vector4 e_vWorkingRectRange,bool e_bForceSetCancelButtonPositionbyWorkingZone)
	{
		m_bCheckOutOfWorking = true;
		m_pCancelButton = 0;
		m_bOutOfWorkingZone = false;
		if( e_pCancelButton )
		{
			m_pCancelButton = e_pCancelButton;
			m_pCancelButton->SetName(CANCEL_BUTTON_NAME);
			if( e_bForceSetCancelButtonPositionbyWorkingZone )
			{
				if( cImageButton::TypeID == m_pCancelButton->Type() )
				{
					((cImageButton*)m_pCancelButton)->SetPosByImageCenter(Vector3(e_vWorkingRectRange.z,e_vWorkingRectRange.y,0.f));
				}
			}
		}
		this->m_vCollisionRange = e_vWorkingRectRange;
	}

	cMouse_TouchWithCancelButton::cMouse_TouchWithCancelButton(cMouse_TouchWithCancelButton*e_pMouse_TouchWithCancelButton):cClickMouseBehaviorList<cClickMouseBehavior>(e_pMouse_TouchWithCancelButton)
	{
		m_pCancelButton = 0;
		m_bOutOfWorkingZone = false;
		m_bCheckOutOfWorking = e_pMouse_TouchWithCancelButton->m_bCheckOutOfWorking;
		if( e_pMouse_TouchWithCancelButton->m_pCancelButton )
		{
			m_pCancelButton = dynamic_cast<cClickMouseBehavior*>(e_pMouse_TouchWithCancelButton->m_pCancelButton->Clone());
			m_pCancelButton->SetName(CANCEL_BUTTON_NAME);
		}
	}

	cMouse_TouchWithCancelButton::~cMouse_TouchWithCancelButton()
	{
		SAFE_DELETE(m_pCancelButton);
	}

	void    cMouse_TouchWithCancelButton::Init()
	{
		m_bOutOfWorkingZone = false;
		cClickMouseBehaviorList<cClickMouseBehavior>::Init();
		m_MiscButtonEventList.Init();
		if( m_pCancelButton )
			m_pCancelButton->Init();
	}

	void    cMouse_TouchWithCancelButton::OutOfWorkingMouseDown(int e_iPosX,int e_iPosY)
	{
		if( m_pCancelButton )
		{
			m_pCancelButton->MouseDown(e_iPosX,e_iPosY);
			//if( m_pCancelButton->IsCollided() )
			{
				//m_bOutOfWorkingZone = false;
			}
		}
		if(!m_bCheckOutOfWorking)
			return;
		m_bOutOfWorkingZone = true;
		if(this->m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY))
			this->m_bOutOfWorkingZone = false;
	}

	void    cMouse_TouchWithCancelButton::OutOfWorkingMouseMove(int e_iPosX,int e_iPosY)
	{
		if( m_pCancelButton&&m_pCancelButton->IsCollided() )
			m_pCancelButton->MouseMove(e_iPosX,e_iPosY);	
	}

	void    cMouse_TouchWithCancelButton::OutOfWorkingMouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_pCancelButton&&m_pCancelButton->IsCollided() )
		{
			m_pCancelButton->MouseUp(e_iPosX,e_iPosY);
		}
		if(!m_bCheckOutOfWorking)
			return;
		if( m_bOutOfWorkingZone && !this->m_pCurrentMouseDownCollidedObject )
		{
			if(!this->m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY))
			{//click out of this menu
				m_bOutOfWorkingZone = true;
				this->m_bClickMouseBehaviorSatisfied = true;
				return;
			}
			else
				m_bOutOfWorkingZone = false;
		}
	}

	void    cMouse_TouchWithCancelButton::MouseDown(int e_iPosX,int e_iPosY)
	{
		m_MiscButtonEventList.MouseDown(e_iPosX,e_iPosY);
		if(m_MiscButtonEventList.IsChildrenCollided() != -1 )
			m_bMiscButtonClicked = true;
		else
		{
			cClickMouseBehaviorList<cClickMouseBehavior>::MouseDown(e_iPosX,e_iPosY);
			m_bMiscButtonClicked = false;
		}
		OutOfWorkingMouseDown(e_iPosX,e_iPosY);
	}

	void    cMouse_TouchWithCancelButton::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( !m_bMiscButtonClicked )
			cClickMouseBehaviorList<cClickMouseBehavior>::MouseMove(e_iPosX,e_iPosY);
		else
			m_MiscButtonEventList.MouseMove(e_iPosX,e_iPosY);
		OutOfWorkingMouseMove(e_iPosX,e_iPosY);
	}

	void    cMouse_TouchWithCancelButton::MouseUp(int e_iPosX,int e_iPosY)
	{
		if( !m_bMiscButtonClicked )
			cClickMouseBehaviorList<cClickMouseBehavior>::MouseUp(e_iPosX,e_iPosY);
		else
			m_MiscButtonEventList.MouseUp(e_iPosX,e_iPosY);
		OutOfWorkingMouseUp(e_iPosX,e_iPosY);
	}

	void    cMouse_TouchWithCancelButton::MiscRender()
	{
		m_MiscButtonEventList.Render();
		if( m_pCancelButton )
			m_pCancelButton->Render();	
	}

	void    cMouse_TouchWithCancelButton::MiscUpdate(float e_fElpaseTime)
	{
		m_MiscButtonEventList.Update(e_fElpaseTime);
		if( m_pCancelButton )
		{
			m_pCancelButton->Update(e_fElpaseTime);
			if( m_pCancelButton->IsSatisfiedCondition() )
			{
				m_bOutOfWorkingZone = true;
				this->m_bClickMouseBehaviorSatisfied = true;
			}
		}
		if( this->m_bClickMouseBehaviorSatisfied && !m_bOutOfWorkingZone )
		{
			if( m_pCancelButton )
			{
				if( m_pCancelButton->IsSatisfiedCondition() )
				{
					m_bOutOfWorkingZone = true;
				}
			}
		}	
	}

	void    cMouse_TouchWithCancelButton::Update(float e_fElpaseTime)
	{
		cClickMouseBehaviorList<cClickMouseBehavior>::Update(e_fElpaseTime);
		MiscUpdate(e_fElpaseTime);
	}
	void    cMouse_TouchWithCancelButton::Render()
	{
		cClickMouseBehaviorList<cClickMouseBehavior>::Render();
		MiscRender();
	}

	bool	cMouse_TouchWithCancelButton::IsChancelButtonSatisfied()
	{
		if( !m_pCancelButton )
			return false;
		return m_pCancelButton->IsSatisfiedCondition();
	}

	void	cMouse_TouchWithCancelButton::SetCancelButton(cBaseImage*e_pImage,Vector3 e_vPos)
	{
		SAFE_DELETE(m_pCancelButton);
		m_pCancelButton = new cImageButton(e_pImage,e_vPos);
	}

	void	cMouse_TouchWithCancelButton::SetCancelButton(cSubMPDI*e_pSubMPDI)
	{
		SAFE_DELETE(m_pCancelButton);
		m_pCancelButton = new cImageButton(e_pSubMPDI);
	}
	void	cMouse_TouchWithCancelButton::SetCancelButton(cMPDI*e_pMPDI)
	{
		SAFE_DELETE(m_pCancelButton);
		m_pCancelButton = new cImageButton(e_pMPDI);
	}
}