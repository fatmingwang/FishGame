#ifndef _ASSEMBLE_IMAGE_H_
#define _ASSEMBLE_IMAGE_H_
#include "../AllImageInclude.h"
#include "ImageButton.h"

	//multiple image but a button.
	class	cAssembleImageButton:public cBehaviorObjectList<cRenderObjectBehavior>,public cImageButton
	{
		void					SetTransformInternalData();
		virtual	void			AddObjectNotification(cRenderObjectBehavior* e_t){ if(e_t)this->AddChild(e_t); }
		virtual	void			RemoveObjectNotification(cRenderObjectBehavior* e_t){ if(e_t)e_t->SetParent(0); }
	public:
		DEFINE_TYPE_INFO();
		cAssembleImageButton();
		~cAssembleImageButton();
		cAssembleImageButton(cAssembleImageButton*e_pAssembleImageButton);
		CLONE_MYSELF(cAssembleImageButton);
		static	cAssembleImageButton*	GetMe(TiXmlElement*e_pElement);
		virtual void					Init();
		virtual	void					SetColor(Vector4 e_vColor);
		virtual	void					Update(float e_fElpaseTime);
		virtual	void					Render();
		virtual void					DebugRender();
	};

#endif