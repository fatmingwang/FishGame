#include "stdafx.h"
#include "IABProductButton.h"


void	cIABProductButton::Render()
{
	if( m_pItemImage )
	{
		m_pItemImage->Render();
	}
	if( m_pPressedImage && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ) )
	{
		this->m_pPressedImage->Render();
	}
	else
		cBaseImage::Render(); 
}