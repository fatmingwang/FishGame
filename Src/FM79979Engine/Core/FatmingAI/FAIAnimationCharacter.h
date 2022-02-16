#ifndef _FAI_ANIMATION_STATUS_H_
#define _FAI_ANIMATION_STATUS_H_

#include "FAICharacterParameter.h"

namespace FATMING_AI
{

	//===============================================
	//auto change animation while character behivor status is not same
	//===============================================
	class	cFAIAnimationCharacter:public cFMWorkingObjectChanger<cFMAnimationRule>,public cFAICharacterInterface
	{
		eFAICharacterBehaviorStatus	m_ePreviousBehaviodStatus;
	public:
		DEFINE_TYPE_INFO();
		cFAIAnimationCharacter(TiXmlElement*e_pElement);
		cFAIAnimationCharacter(cFAIAnimationCharacter*e_pFAIAnimationStatus);
		virtual ~cFAIAnimationCharacter();
		CLONE_MYSELF(cFAIAnimationCharacter);
		virtual	bool	IsCharacterBehaviorStatusLock();
		virtual void*   GetAnimationData();
		virtual	void	Init();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
	};
}

#endif