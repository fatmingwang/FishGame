#include "stdafx.h"
#include "FallDownNewSpinObject.h"
#include "SlotObject.h"


cFallDownNewSpinObject::cFallDownNewSpinObject(int e_iNumObject,Vector3 e_vEndPos,float	e_fSpeed,float	e_fObjectGap)
{
	m_iCount = e_iNumObject;
	m_pMovingObjects = new sImageWithPos[e_iNumObject];
	m_vEndPos = e_vEndPos;
	m_fSpeed = e_fSpeed;
	m_fObjectGap = e_fObjectGap;
}

cFallDownNewSpinObject::~cFallDownNewSpinObject()
{
	SAFE_DELETE_ARRAY(m_pMovingObjects);
}

void	cFallDownNewSpinObject::SetupData(int e_iIndex,Vector3 e_vStartPos,cBaseImage*e_pImage)
{
	m_pMovingObjects[e_iIndex].vPos = e_vStartPos;
	assert(e_pImage);
	m_pMovingObjects[e_iIndex].pImage = e_pImage;
}

void	cFallDownNewSpinObject::Update(float e_fElpaseTime)
{
	float	l_fSpeed = e_fElpaseTime*m_fSpeed;
	for(int i=m_iCount-1;i>-1;--i)
	{
		if( i==0 && m_pMovingObjects[0].vPos.y + m_fObjectGap >= m_pMovingObjects[1].vPos.y )
		{
			m_bAllFallDown  =true;
		}
		m_pMovingObjects[i].vPos.y += l_fSpeed;
		//last one
		if( i == m_iCount-1 )
		{
			if(m_pMovingObjects[i].vPos.y > m_vEndPos.y)
			{
				m_pMovingObjects[i].vPos.y = m_vEndPos.y;
			}
		}
		else
		{
			if(m_pMovingObjects[i].vPos.y+m_fObjectGap > m_pMovingObjects[i+1].vPos.y )
				m_pMovingObjects[i].vPos.y = m_pMovingObjects[i+1].vPos.y-m_fObjectGap;
		}
	}
}

void	cFallDownNewSpinObject::Render()
{
	for(int i=0;i<this->m_iCount;++i)
	{
		m_pMovingObjects[i].pImage->SetPos(m_pMovingObjects[i].vPos);
		//m_pMovingObjects[i].pImage->SetVisible(true);
		m_pMovingObjects[i].pImage->Render();
		//m_pMovingObjects[i].pImage->SetVisible(false);
	}
}

void	cFallDownNewSpinObject::Init()
{
	m_bAllFallDown = false; 
}

bool	cFallDownNewSpinObject::IsDone()
{
	return m_bAllFallDown; 
}
