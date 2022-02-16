#ifndef _sBulletData_H_
#define _sBulletData_H_








struct	sBulletData
{
	cbtShapeCollision*	m_pBulletCollision;
	cMPDI*				m_pBulletMPDI;
	float				m_fSpeed;
	cbtShapeCollision*	m_pbtWebCollision;
	cMPDI*				m_pWebMPDI;
};

#endif