#ifndef _PLATFORM_DEFINE_H_
#define _PLATFORM_DEFINE_H_

//===============================================
// Cross-platform type definitions
#ifdef WIN32
    typedef signed char int8;
    typedef short int16;
    typedef long int32;
    typedef __int64 int64;
    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned long uint32;
    typedef unsigned __int64 uint64;
    //#define	MYWCSTOK(y,z)	wcstok(y,z);
#else // For LINUX and IOS
	#pragma GCC diagnostic ignored "-Wformat"
	//#pragma GCC diagnostic ignored "-Wformat-contain-nul"
	//#pragma GCC diagnostic ignored "-Wformat-extra-args"
	#pragma GCC diagnostic ignored "-Wformat-zero-length"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wwrite-strings"
    //windows type
	#define CONST				const
    #define VOID void
    typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
    typedef int                 INT;
    typedef unsigned int        UINT;
	typedef unsigned long       DWORD;
    typedef unsigned char       BYTE;
    typedef unsigned short      WORD;
    typedef float               FLOAT;
    typedef unsigned int        UINT;
    typedef char                CHAR;
    typedef long                LONG;
    typedef unsigned short      WORD;
    typedef short				SHORT;
#ifndef IOS
//	typedef bool				BOOL;
#endif
    typedef int8_t				int8;
    typedef int16_t				int16;
    typedef int32_t				int32;
    typedef int64_t				int64;
    typedef unsigned char		uint8;
    typedef unsigned int		uint16;
    typedef unsigned long int	uint32;
    typedef unsigned long long int uint64;
	typedef uint64				UINT64;
    typedef uint8 byte;
	#define	 TRUE				1
	#define	 FALSE				0
	//#define	 NULL				0

    typedef struct tagRECT
    {
        LONG    left;
        LONG    top;
        LONG    right;
        LONG    bottom;
    } RECT;
    typedef struct tagPOINT
    {
        LONG  x;
        LONG  y;
    } POINT;


    static WCHAR* g_pStringForWCSTOK = 0;
    //#define	MYWCSTOK(y,z)   wcstok(y,z,&g_pStringForWCSTOK)
	#define	wcstok(y,z)   wcstok(y,z,&g_pStringForWCSTOK)
	#define	_atoi64	atoll//for string to uint64
    #ifndef _CLIMITS_
    #include <climits>
    #endif // _CLIMITS_
#endif // PLATFORMS
//===============================================
//math
#ifndef D3DXToRadian
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))
#endif
//===============================================
#ifndef D3DXToDegree
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))
#endif
//===============================================
#ifndef D3DX_PI
#define D3DX_PI    (3.14159265358979323846f)
#endif
//===============================================
#if defined WIN32
#define GL_COLOR_ATTACHMENT0                            GL_COLOR_ATTACHMENT0_EXT
#define GL_DEPTH_ATTACHMENT                             GL_DEPTH_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_COMPLETE                         GL_FRAMEBUFFER_COMPLETE_EXT
#define GL_FRAMEBUFFER_UNSUPPORTED                      GL_FRAMEBUFFER_UNSUPPORTED_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT    GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT            GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS            GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#define GL_FRAMEBUFFER                                  GL_FRAMEBUFFER_EXT
#define GL_RENDERBUFFER                                 GL_RENDERBUFFER_EXT
#define glGenFramebuffers                               glGenFramebuffersEXT
#define glBindFramebuffer                               glBindFramebufferEXT
#define glBindRenderbuffer                              glBindRenderbufferEXT
#define glCheckFramebufferStatus                        glCheckFramebufferStatusEXT
#define glFramebufferRenderbuffer                       glFramebufferRenderbufferEXT
#define glFramebufferTexture2D                          glFramebufferTexture2DEXT
#define glGenRenderbuffers                              glGenRenderbuffersEXT
#define glRenderbufferStorage                           glRenderbufferStorageEXT
#define glDeleteFramebuffers                            glDeleteFramebuffersEXT
#define glDeleteRenderbuffers                           glDeleteRenderbuffersEXT
#define glClearDepthf                                   glClearDepth

#endif
//===============================================
#ifdef WIN32
#pragma warning(disable:4702)
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#else
#ifdef IOS
#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include <string>
#include <wchar.h>
#include <stdint.h>
#else // IOS
#if defined(LINUX) || defined(__PPU__)
#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include <string>
#include <wchar.h>
#include <stdarg.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#else // OTHER...                 only for linux and windows!!
//#error "Unsupported platform."
#endif // LINUX || __PPU__
#endif // IOS
#endif // WIN32
//===============================================
#ifndef MAX_PATH
#define MAX_PATH			260
#endif


#define IPAD_RESOLUTION_WIDTH		768
#define IPAD_RESOLUTION_HEIGHT		1024

#define NEW_IPAD_RESOLUTION_WIDTH   1536
#define NEW_IPAD_RESOLUTION_HEIGHT  2048

#define IPHONE_RESOLUTION_WIDTH		320
#define IPHONE_RESOLUTION_HEIGHT	480

#define IPHONE_4_RESOLUTION_WIDTH   640
#define IPHONE_4_RESOLUTION_HEIGHT  960

#define IPHONE_5_RESOLUTION_WIDTH   640
#define IPHONE_5_RESOLUTION_HEIGHT  1136

#define	FULL_HD_RESOLUTION_WIDTH	1920
#define	FULL_HD_RESOLUTION_HEIGHT	1080

#define	DATA_TYPE	L"DataType"

enum	eDataType
{
	eDT_BYTE = 0,
	eDT_FLOAT,
	eDT_DOUBLE,
	eDT_INT,
	eDT_STRING, //string(std::string)
	eDT_WSTRING,//wstring(std::wstring)
	eDT_VECTOR2,
	eDT_VECTOR3,
	eDT_VECTOR4,
	eDT_POINT,
	eDT_VOID,	//could be any type
	eDT_INT64,
	eDT_UINT64,
	eDT_MAX,
};

static int	g_iDataTypeSize[eDT_MAX] = {
										sizeof(char),//byte
										sizeof(float),//float
										sizeof(double),//double
										sizeof(int),//int
										-1,//string
										-1,//wstring
										sizeof(float)*2,//vector2
										sizeof(float)*3,//vector3
										sizeof(float)*4,//vector4
										sizeof(long)*2,//point
										-1,//void
										sizeof(int64),//int64
										sizeof(uint64)//UINT64
										};

#endif