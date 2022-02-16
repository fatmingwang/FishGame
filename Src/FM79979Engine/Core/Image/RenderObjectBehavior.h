#ifndef _RENDER_OBJECT_BEHAVIOR_H
#define _RENDER_OBJECT_BEHAVIOR_H

#include "../Math/Frame.h"

namespace FATMING_CORE
{
	class	cRenderObjectBehavior:virtual public Frame
	{
		//just a rotiton reference
		Vector3			m_vRotation;
		//while set transform is called we might need some data update.ex:rotation
		//if warning is appear override as below
		//virtual	void	SetTransformInternalData(){ cRenderObjectBehavior::SetTransformInternalData(); }
	protected:
		virtual	void	SetTransformInternalData();
		GET_SET_DEC(bool,m_bUpdateRotation,IsUpdateRotation,SetUpdateRotation);
	protected:
		//local position
		Vector3*		m_pvPos;
	public:
		cRenderObjectBehavior();
		cRenderObjectBehavior(cRenderObjectBehavior*e_pRenderObjectBehavior);
		virtual ~cRenderObjectBehavior();
		Vector3			GetPos();
		void			SetPos(Vector3 e_vPos);
		//radian
		float			GetAngle();
		//degree
		float			GetDegreeAngle();
		//radian not degree.
		Vector3			GetRotation(){ return m_vRotation; }
		void			SetAngle(float e_fAngle);
		virtual	void	Init() = 0;
		virtual	void	Update(float e_fElpaseTime) = 0;
		virtual	void	Render() = 0;
		virtual	void	Destroy() = 0;
		//virtual	bool	IsDone(){ return false; }
		virtual	void	SetColor(Vector4 e_vColor){}
		//virtual	int		GetWidth() = 0;
		//virtual	int		GetHeight() = 0;
	};

//end namespace
}

#endif