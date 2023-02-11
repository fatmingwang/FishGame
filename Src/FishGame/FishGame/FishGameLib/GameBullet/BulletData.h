#pragma once


struct	sBulletData
{
	cbtShapeCollision*	m_pBulletCollision;
	cMPDI*				m_pBulletMPDI;
	float				m_fSpeed;
	cbtShapeCollision*	m_pbtWebCollision;
	cMPDI*				m_pWebMPDI;
};