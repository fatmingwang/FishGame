#ifndef _FM_ANIMATION_RULE_MANAGER_H_
#define _FM_ANIMATION_RULE_MANAGER_H_

#include "../Image/FMAnimationRule.h"

//=========================================
//this is a FMAnimationRule parser
//what ever it is,just require your data and it will auto update
//
//=========================================

namespace FATMING_CORE
{
	typedef cFMAnimationRule*    (*FatmingAnimationRuleParseFunction)(TiXmlElement*e_pElement);
	class cFMAnimationRuleManager : public cNamedTypedObjectVector< cFMAnimationRule >,public ISAXCallback
	{
		//
		FatmingAnimationRuleParseFunction	m_pFatmingAnimationRuleParseFunction;
		//
		virtual	void						HandleElementData(TiXmlElement*e_pTiXmlElement);
		void								ProcessFMARQueuePlayer(TiXmlElement*e_pTiXmlElement);
		void								ProcessFatmingAnimationRule(TiXmlElement*e_pTiXmlElement);
								
		cTempContainer<cFMAnimationRule>										m_WorkingFatmingAnimationRuleTempContainer;
		cNamedTypedObjectVector< cNamedTypedObjectVector< cFMAnimationRule > >	m_AllFatmingAnimationRuleList;
		//not wait for calling,it's a always running game effect
		cNamedTypedObjectVector<cFMAnimationRule>								m_AlwaysRunFatmingAnimationRuleList;
	public:

		cFMAnimationRuleManager();
		virtual ~cFMAnimationRuleManager();
		//after call this please call cFMAnimationRule::SetData
		cFMAnimationRule* RequirePrizeEffect( const WCHAR* e_strEffectTypeName,void*e_pData = 0,bool e_bPushIntoTempContainer = true);

		void Init(char*e_strFileName);
		void Render();
		void Update( float ElapseTime );
	};

//end namespace FATMING_CORE
}

#endif