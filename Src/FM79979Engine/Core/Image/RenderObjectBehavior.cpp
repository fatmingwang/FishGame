#include "../stdafx.h"

#include "RenderObjectBehavior.h"

namespace FATMING_CORE
{
	cRenderObjectBehavior::cRenderObjectBehavior()
	{
		m_vRotation = Vector3::Zero;
		m_pvPos = Frame::GetLocalPositionPointer();
		m_bUpdateRotation = false;
	}

	cRenderObjectBehavior::cRenderObjectBehavior(cRenderObjectBehavior*e_pRenderObjectBehavior):Frame(e_pRenderObjectBehavior)
	{
		this->SetName(e_pRenderObjectBehavior->GetName());
		m_vRotation = e_pRenderObjectBehavior->m_vRotation;
		m_pvPos = this->GetLocalPositionPointer();
		this->SetLocalTransform(e_pRenderObjectBehavior->GetLocalTransform());
		m_bUpdateRotation = e_pRenderObjectBehavior->m_bUpdateRotation;
	}

	cRenderObjectBehavior::~cRenderObjectBehavior()
	{
	}

	void	cRenderObjectBehavior::SetTransformInternalData()
	{
		if( !m_bUpdateRotation )
			return;
		Vector3	l_vScale,l_vTranslation;
		float	l_fInvert;
		this->GetLocalTransform().Decompose(l_vScale,m_vRotation,l_vTranslation,l_fInvert);
		//m_pvRotation->x = D3DXToDegree(m_pvRotation->x);
		//m_pvRotation->y = D3DXToDegree(m_pvRotation->y);
		//m_pvRotation->z = D3DXToDegree(m_pvRotation->z);
	}

	Vector3	cRenderObjectBehavior::GetPos()
	{
		return this->GetLocalPosition();
	}

	void	cRenderObjectBehavior::SetPos(Vector3 e_vPos)
	{
		Frame::SetLocalPosition(e_vPos);
	}

	float	cRenderObjectBehavior::GetAngle()
	{
		return this->GetRotation().z;
	}

	float	cRenderObjectBehavior::GetDegreeAngle()
	{
		return D3DXToDegree(GetAngle());
	}

	void	cRenderObjectBehavior::SetAngle(float e_fAngle)
	{
		SetLocalRotation(Vector3(0.f,0.f,e_fAngle));
	}
}