#ifndef	ALL_POLICY_NAME_LIST_H
#define ALL_POLICY_NAME_LIST_H
namespace FATMING_CORE
{
//for editor use
	class	cPrtAllClassName
	{
	public:
		//all policy name
		static	const char*	m_strAllParticlePolicy[];
		//color
		static const WCHAR*	m_sstrColorInitNameList[];
		static const WCHAR*	m_sstrColorActNameList[];
		//life
		static const WCHAR*	m_sstrLifeInitNameList[];
		static const WCHAR*	m_sstrLifeActNameList[];
		//rotate
		static const WCHAR*	m_sstrRotateInitNameList[];
		static const WCHAR*	m_sstrRotateActNameList[];
		//size
		static const WCHAR*	m_sstrSizeInitNameList[];
		static const WCHAR*	m_sstrSizeActNameList[];
		//start position
		static const WCHAR*	m_sstrStartPositionInitNameList[];
		static const WCHAR*	m_sstrStartPositionActNameList[];
		//velocity
		static const WCHAR*	m_sstrVelocityInitNameList[];
		static const WCHAR*	m_sstrVelocityActNameList[];
		//texture
		static const WCHAR*	m_sstrTextureInitNameList[];
		static const WCHAR*	m_sstrTextureActNameList[];
		//all
		static const WCHAR**m_sstrAllActNameList[];
		static const WCHAR**m_sstrAllInitNameList[];
	};
//end namespace FATMING_CORE
}
#endif