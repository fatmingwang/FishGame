#include "stdafx.h"
#include "IODataEditor.h"

//cIOSMDriverInterface*					g_pIOSMDriverInterface = 0;
cIOSMIOManager*							g_pIOSMIOManager = 0;
cIOSMBaseDevice*						g_pIOSMBaseDevice = 0;
cProbableIO*							g_pProbableIO = 0;
cActiveIOTimesWithSingnalCircle*		g_pActiveIOTimesWithSingnalCircle = 0;
cListenMultiSingnalToActiveIO*			g_pListenMultiSingnalToActiveIO = 0;
cSendHighSingnal*						g_pSendHighSingnal = 0;
cSendLowSingnal*						g_pSendLowSingnal = 0;
#pragma comment(lib, "../../../lib/Devil.lib")
#pragma comment(lib, "../../../lib/OpenAL32.lib")
#pragma comment(lib, "../../../lib/alut.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "../../../lib/FPGAWinXPLib.lib")

#ifdef _DEBUG
	#pragma comment(lib, "../../../lib/Debug/BulletCollision.lib")
	//#pragma comment(lib, "../../../lib/Debug/IOSimulationMachine.lib")
	#pragma comment(lib, "../../../lib/Debug/Core.lib")
	#pragma comment(lib, "../../../lib/Debug/LinearMath.lib")
	#pragma comment(lib, "../../../lib/Debug/BulletDynamics.lib")
#else

	#pragma comment(lib, "../../../lib/Release/Core.lib")
	//#pragma comment(lib, "../../../lib/Release/IOSimulationMachine.lib")
	#pragma comment(lib, "../../../lib/Release/BulletCollision.lib")
	#pragma comment(lib, "../../../lib/Release/LinearMath.lib")
	#pragma comment(lib, "../../../lib/Release/BulletDynamics.lib")
#endif
#pragma comment(lib, "../../../lib/ogg.lib")//I have build my self OGG lib here
#pragma comment(lib, "../../../lib/glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "Vfw32.lib")
#pragma comment(lib, "Setupapi.lib")
using namespace IODevice;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// 建立任何控制項之前，先啟用 Windows XP 視覺化效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 建立主視窗並執行
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	return 0;
}
