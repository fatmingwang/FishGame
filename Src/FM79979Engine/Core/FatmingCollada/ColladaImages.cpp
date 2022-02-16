#include "StdAfx.h"
#include "ColladaImages.h"
#include <string>
const wchar_t*           cLibraryImages::TypeID( L"cLibraryImages" );
extern bool	g_bInvertTextureAdress_V;//bmp texture adress V is invert
//
//
//
cLibraryImages::~cLibraryImages()
{
	DELETE_VECTOR(AllLibraryImages,sLibraryImages*);
	//cNamedTypedObjectVector<cBaseImage>*l_pSimpleGLTextureList = &m_ImageList;
	//LIST_DO_FOR_ALL_CHILDREN(l_pSimpleGLTextureList,Release(&m_ImageList));
}

void	cLibraryImages::ParseData(TiXmlElement*e_pTiXmlElement)
{
	assert(strlen(m_strDirectory)&&"forget to set root directory");
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryImagesElement[eLI_image]))
		{
			Parseimage(l_pChildTiXmlElement);
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryImages::Parseimage(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sLibraryImages*l_pLibraryImages = new sLibraryImages;
	m_pCurrentLibraryImages = l_pLibraryImages;
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"name"))
		{
			sprintf(l_pLibraryImages->strName,"%s\0",UT::WcharToChar(l_strValue));
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			sprintf(l_pLibraryImages->strID,"%s\0",WcharToChar(l_strValue));
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	this->AllLibraryImages.push_back(l_pLibraryImages);
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryImagesElement[eLI_image_init_from]))
		{
			Parseimage_init_from( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_scLibraryImagesElement[eLI_image_extra]))
		{
			Parseimage_extra(l_pChildTiXmlElement);
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryImages::Parseimage_init_from(TiXmlElement*e_pTiXmlElement)
{
	//const WCHAR*l_strName = 0;
	//const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	sprintf(m_pCurrentLibraryImages->strFileName,"%s%s\0",m_strDirectory,UT::GetFileNameWithoutFullPath(UT::WcharToChar(l_strText),false));

	if( g_strParsingDataList )
	{
		std::string	l_str;
		l_str.append("read ");
		l_str.append(m_pCurrentLibraryImages->strFileName);
		l_str.append("\n");
		g_strParsingDataList->push_back(l_str);
	}
	//assert(l_strText);
	cBaseImage*l_pBaseImage = 0;
	if( strlen(m_pCurrentLibraryImages->strName) )
		l_pBaseImage = m_ImageList.GetObject(UT::CharToWchar(m_pCurrentLibraryImages->strName));
	else
		l_pBaseImage = m_ImageList.GetObject(UT::CharToWchar(m_pCurrentLibraryImages->strID));
	if( !l_pBaseImage )
	{
		//g_bInvertTextureAdress_V = false;
//#ifdef __IPHONE
		//if(!wcscmp(UT::GetFileExtensionName(m_pCurrentLibraryImages->strFileName),"bmp"))
		//	g_bInvertTextureAdress_V = true;
//#endif
		l_pBaseImage = new cBaseImage(m_pCurrentLibraryImages->strFileName);
		if( strlen(m_pCurrentLibraryImages->strName) )
			l_pBaseImage->SetName(UT::CharToWchar(m_pCurrentLibraryImages->strName));
		else
			l_pBaseImage->SetName(UT::CharToWchar(m_pCurrentLibraryImages->strID));
		assert(wcslen(l_pBaseImage->GetName())&&"failed to assigen texture name!!");
		bool	l_b = m_ImageList.AddObject(l_pBaseImage);
		assert(l_b&&"such texture already add");
	}
}

void	cLibraryImages::Parseimage_extra(TiXmlElement*e_pTiXmlElement)
{
#ifdef WIN32
	OutputDebugString(L"not support image extrac for now,Call fatming");
#endif
}
