#ifdef WIN32
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "Glu32.lib")
//#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "User32.lib")
//#pragma comment(lib, "vfw32.lib")
//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "kernel32.lib")
//#pragma comment(lib, "user32.lib")
//#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "winspool.lib")
//#pragma comment(lib, "comdlg32.lib")
//#pragma comment(lib, "advapi32.lib")
//#pragma comment(lib, "shell32.lib ")
//#pragma comment(lib, "ole32.lib ")
//#pragma comment(lib, "oleaut32.lib")
//#pragma comment(lib, "uuid.lib ")
//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
//#pragma comment(lib, "../../../lib/glew.lib"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Psapi.lib")

#pragma comment(lib, "../../../lib/Devil.lib")
#pragma comment(lib, "../../../lib/OpenAL32.lib")
#pragma comment(lib, "../../../lib/alut.lib")
#pragma comment(lib, "../../../lib/ogg.lib")
#pragma comment(lib, "../../../lib/glew32.lib")
#ifdef _DEBUG
//debug
	#ifdef OPENGLES_2_X
		#pragma comment(lib, "../../../lib/Debug/Core.lib")
	#else
		#pragma comment(lib, "../../../lib/DebugFixedPipeline/Core.lib")
	#endif
	#pragma comment(lib, "../../../lib/Debug/BulletCollision.lib")
	#pragma comment(lib, "../../../lib/Debug/BulletDynamics.lib")
	#pragma comment(lib, "../../../lib/Debug/LinearMath.lib")
#else
//release
	#ifdef OPENGLES_2_X
		#pragma comment(lib, "../../../lib/Release/Core.lib")
	#else
		#pragma comment(lib, "../../../lib/ReleaseFixedPipeline/Core.lib")
	#endif
	#pragma comment(lib, "../../../lib/Release/BulletCollision.lib")
	#pragma comment(lib, "../../../lib/Release/BulletDynamics.lib")
	#pragma comment(lib, "../../../lib/Release/LinearMath.lib")
	#endif
#endif//end win32