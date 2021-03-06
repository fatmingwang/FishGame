#ifndef ALL_CORE_INCLUDE
#define	ALL_CORE_INCLUDE

//if platfor is android command line make u has set (--rtti -fexceptions )
#ifndef ANDROID
//#define ANDROID
//lot of undefined reference to ... errors
//Application.mk  file LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
//http://stackoverflow.com/questions/7209971/ndk-build-undefined-reference-to-errors-when-statically-linking-to-libxml-a
#endif

#if defined(WIN32)
#pragma warning( disable : 4996 )
#include "windows.h"
#include <math.h>
#include "XML/AtgXmlWriter.h"
#include "../../Include/glew.h"
#include "../../Include/glext.h"
#elif defined(LINUX)
#include "glew.h"
#include "glext.h"
#elif defined(ANDROID)
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#elif defined(IOS)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif

//<string.h> is for c, <string> is for c++
#include <string>
#include <vector>
#include "CommonDefine.h"
#include "GameplayUT/GameUTDefine.h"
#include "NamedTypedObject.h"
#include "TemplateClass.h"
#include "SmartPointer.h"
#include "TwoDimensionDataContainer.h"
#include "MapCellDataContainer.h"
#include "AllMathInclude.h"
#include "XML/StringToStructure.h"
#include "Utility.h"
#include "Math/MathUtility.h"
#include "XML/XMLLoader.h"
#include "AllCameraInclude.h"
#include "AllImageInclude.h"
#include "AllMathInclude.h"
//#ifndef IOS
#include "Sound/SoundManager.h"
//#endif
//#ifndef IOS
//#include "AVIPlayer/AVIPlayer.h"
//#endif
//
#ifdef  WIN32
#include "GLSL/GLSLUiniform.h"
#endif

#ifdef  OPENGLES_2_X//iphone 3g only support openGLES 1.5,android up to 2.0
#include "GLSL/GLSLUiniform.h"
#endif

#include "GLSL/Shader.h"

#include "GameplayUT/AllGamePlayUTInclude.h"
//#include "AllPhysicInclude.h"
//#include "Particle/AllParticleInclude.h"
using namespace FATMING_CORE;

#endif