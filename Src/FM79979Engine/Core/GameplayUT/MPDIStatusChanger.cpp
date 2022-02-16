#include "../stdafx.h"
#include "MPDIStatusChanger.h"

namespace FATMING_CORE
{
	//===============
	//<StatusObjectChanger>
	//	<Status Name="" >
	//		<MPDI File="" MPDIList="" MPDI="" >
	//		<MPDI File="" MPDIList="" MPDI="" >
	//	</Status>
	//	<ParticleFile="" prt="" >
	//	<ParticleGroup  File="" prtg="" >
	//</StatusObjectChanger>
	//===============

	//void	cStatusObjectChanger::ProcessMPDI(TiXmlElement*e_pElement)
	//{
	//	PARSE_NAME_VALUE_START
	//		COMPARE_NAME("File")
	//		{
	//		}
	//		//else
	//	PARSE_NAME_VALUE_END
	//}

	//void	cStatusObjectChanger::ProcessParticle(TiXmlElement*e_pElement)
	//{
	//
	//}

	//void	cStatusObjectChanger::ProcessParticleGroup(TiXmlElement*e_pElement)
	//{
	//
	//}

	//bool	cStatusObjectChanger::MyParse(TiXmlElement*e_pRoot)
	//{
	//	const WCHAR*l_strValue = e_pRoot->Value();
	//	if( wcscmp(l_strValue,L"StatusObjectChanger") )
	//	{
	//		this->m_strErrorMsg = L"this is not a StatusObjectChanger file";
	//		return false;
	//	}
	//	e_pRoot = e_pRoot->FirstChildElement();
	//	while( e_pRoot )
	//	{
	//		m_pCurrentStatusContainer = 0;
	//		this->m_pCurrentTiXmlElement = e_pRoot;
	//		l_strValue = e_pRoot->Value();
	//		if( !wcscmp(l_strValue,L"MPDI") )
	//		{
	//			ProcessMPDI(e_pRoot);
	//		}
	//		else
	//		if( !wcscmp(l_strValue,L"ParticleFile") )
	//		{

	//		}
	//		else
	//		if( !wcscmp(l_strValue,L"ParticleGroup") )
	//		{

	//		}
	//		e_pRoot = e_pRoot->NextSiblingElement();
	//	}
	//	//	<Data Type="MPDI" File="" MPDIList="" MPDI="" >	
	//	return true;
	//}

	//bool	cStatusObjectChangerExport(char*e_strFileName)
	//{
	//	return true;
	//}

//end FATMING_CORE
}
