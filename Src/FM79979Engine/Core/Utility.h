#ifndef UTILITY_H
#define UTILITY_H

#include "NamedTypedObject.h"
#include <map>
#include "Math/Collision.h"
#include "Glh.h"
#include "TimeUT.h"
#include "Android/nv_file.h"
#include "TimeUT.h"
#if defined(WIN32)
	#pragma warning( disable : 4996 )
#endif

namespace UT
{
	//extern char		g_sstrForStringToStructure[TEMP_SIZE];
	//extern WCHAR	g_sstrForStringToStructureW[TEMP_SIZE];
	void					MyGlErrorTest();
#ifdef WIN32
	HGLRC					InitOpenGL(HWND e_pHwnd,bool e_bInitGlewInit,HDC e_HdcMV);
	bool 					InitMultisample(HINSTANCE hInstance,HWND hWnd);
	bool 					IsExtensionSupported( char* szTargetExtension );
#endif
	eDataType				GetDataType(const WCHAR*e_str);
	eDataType				GetDataType(const char*e_str);
	//for loading data to ad root directory(Media/)
	inline char*			StringAddRootDirectory(char* e_str){ static char l_strTemp[TEMP_SIZE]; sprintf(l_strTemp,"Media/%s\0",e_str); return l_strTemp; }
	inline char*			ConstCharToChar(const char* e_str){ static char l_strTemp[TEMP_SIZE]; sprintf(l_strTemp,"%s\0",e_str);return l_strTemp; }
	std::wstring			ComposeMsgByFormat(WCHAR*e_strErrMsg1,...);
	std::string				ComposeMsgByFormat(char*e_strErrMsg1,...);
	void					ErrorMsg(const wchar_t*e_strErrMsg1,const wchar_t*e_strErrMsg2);
	void					ErrorMsg(std::wstring  e_strErrMsg1,std::wstring e_strErrMsg2);
	void					ErrorMsg(const wchar_t*  e_strErrMsg1,std::wstring e_strErrMsg2);
	void					ErrorMsg(std::wstring e_strErrMsg1,const wchar_t*  e_strErrMsg2);

	void					ErrorMsg(char*e_strErrMsg1,char*e_strErrMsg2);
	void					ErrorMsg(const char*e_strErrMsg1,char*e_strErrMsg2);
	void					ErrorMsg(char*e_strErrMsg1,const char*e_strErrMsg2);
	void					ErrorMsg(const char*e_strErrMsg1,const char*e_strErrMsg2);	
	void					ErrorMsgByFormat(char*e_strErrMsg1,...);
	void					ErrorMsgByFormat(WCHAR*e_strErrMsg1,...);
	//
	void					SystemErrorCheck();
	WCHAR*					GetFileNameWithoutFullPath(const WCHAR*e_pString,bool e_bStripExtensionFileName = true);
	WCHAR*					GetFileNameWithoutFullPath(std::wstring e_pString,bool e_bStripExtensionFileName = true);
	char*					GetFileNameWithoutFullPath(const char*e_pString,bool e_bStripExtensionFileName = true);
	char*					GetFileNameWithoutFullPath(std::string e_pString,bool e_bStripExtensionFileName = true);

	WCHAR*					GetFileExtensionName(const WCHAR*e_pString);
	char*					GetFileExtensionName(const char*e_pString);
	char*					GetDirectoryWithoutFileName(const char*e_str);
	//for cross platform
	NvFile*					MyFileOpen( const char* FileName,const char*e_strMode  );
	bool					IsFileExists( const char* FileName );
	bool					MyRemoveFile( const char* FileName );
	bool					MyRemoveDirectory( const char* e_strDirectoryName );
	bool					MyCreateDirectory( const char* e_strDirectoryName );
	//ex:  view rang is 100,100,200,200
	//                  4 factor represent x,y,x,y coordinate
	Vector4					ViewRectToOpenGLScissor(Vector4 e_v2DViewRange,Vector2 e_vViewPortSize,Vector2 e_vGameResolution,eDeviceDirection e_eDeviceDirection);
	//x,y showpos,zw width height
	Vector4					ViewRectToOpenGLScissor(Vector4 e_v2DViewRange);
	//============
	//only support 4 characters
	int						HexadecimalStringToDecimal(char*e_strValue);
	int						DecimalStringToHexadcimal(char*e_strValue);	
	int						FindChacterIndexFromBackBuffer(const char*e_String,char e_char);
	int						FindChacterIndexFromBackBuffer(const WCHAR*e_String,WCHAR e_char);
	int						FindChacterIndexFromStartBuffer(const WCHAR*e_String,WCHAR e_char,int e_iLookForIndex = 0);
	//
	long					GetFileSize(NvFile*e_pFile);
	//if e_bCN is true return chinese format or return western format
	std::wstring			GetSystemTimeForFile(bool e_bCN);
	std::wstring			GetSystemTime(bool e_bCN);
	//====================
	//this is good for immediately to get pointer,but becareful!
	//because here using static variable
	//====================
	void					WcharToChar(const wchar_t *e_strSrc,char*e_strDest);
	void					WcharToChar(wchar_t *e_strSrc,char*e_strDest);
	std::string				WcharToChar(wchar_t *e_strWchar);
	std::string				WcharToChar(const wchar_t *e_strWchar);
	std::string				WcharToChar(std::wstring e_strWchar);
	void					CharToWchar(const char *e_strSrc,WCHAR*e_strDest);
	void					CharToWchar(char *e_strSrc,WCHAR*e_strDest);
	std::wstring			CharToWchar(char *e_strChar);
	std::wstring			CharToWchar(const char *e_strChar);
	std::wstring			CharToWchar(std::string	e_strChar);
	//a vector contain a bunch data,now input a index it is possible bigger than last index or smaller 0,
	//so we have to find the proper one if we want it be loop
	int						GetLoopIndex(int e_iIndex,int e_iTotalCount);

	template <class T>inline	int		GetLoopIndex(int e_iIndex,std::vector<T>*e_pVector){return GetLoopIndex(e_iIndex,(int)e_pVector->size());}
	//is index legal in the vector range
	template <class T>inline	bool	IsLegalIndexInVector(std::vector<T>*e_pVector,int e_iIndex){if( e_iIndex<0 )return false;if( e_iIndex>=(int)e_pVector->size() )return false;return true;	}
	inline	POINT				GetScreenResolution();
	//==============================
	//move to destination from a to b with a time and get current move distance.
	//==============================
    template<class T>struct  sMoveToDestTimeCounter
    {
        T    				vCurrentMoveDistance;
		T    				vTotalMoveDistance;
        T    				vCurrentPos;
        T    				vEndPos;
		T    				vStartPos;
        UT::sTimeCounter    MoveToDestinationTC;
		void				SetTimeAndPosData(T e_Distination,T e_StartPos,float e_fTime)
		{
			MoveToDestinationTC.SetTargetTime(e_fTime);
			vCurrentPos = vStartPos = e_StartPos;
			vEndPos = e_Distination;
			this->vTotalMoveDistance = vStartPos-vStartPos;
			vCurrentMoveDistance = vTotalMoveDistance;
		}
		void    			Start()
		{
			MoveToDestinationTC.Start();
			vCurrentPos = vStartPos;
			this->vTotalMoveDistance = vStartPos-vStartPos;
			vCurrentMoveDistance = vTotalMoveDistance;
		}

        void    			Update(float e_fElpaseTime)
        {
            MoveToDestinationTC.Update(e_fElpaseTime);
            if(!this->MoveToDestinationTC.bTragetTimrReached)
            {
				vCurrentMoveDistance = (e_fElpaseTime/MoveToDestinationTC.fTargetTime)*(vEndPos-vStartPos);
                vTotalMoveDistance += vCurrentMoveDistance;
                vCurrentPos += vTotalMoveDistance;
            }
            else
            {
				vCurrentMoveDistance = vEndPos-vStartPos-vTotalMoveDistance;
				vTotalMoveDistance = vEndPos-vStartPos;
                vCurrentPos = vEndPos;
            }
        }
		bool	IsDone(){ return MoveToDestinationTC.bTragetTimrReached; }
    };
//end namespace Utility
}
using namespace UT;
#endif