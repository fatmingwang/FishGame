#include "stdafx.h"
#include "Resource.h"
#include "Windowsx.h"


#include "../FishGameLib/GameApp/GameApp.h"
//
#define MAX_LOADSTRING 100
//
// Global Variables:
HINSTANCE			hInst;								// current instance
TCHAR				szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR				szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND				g_hWnd;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HHOOK MouseHook;
bool				g_bLeave = false;
cGameApp			*g_pGameApp = 0;

#include <direct.h>






int APIENTRY _tWinMain( HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPTSTR    lpCmdLine,
                        int       nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	//HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	swprintf( szTitle, 100, L"Ver.[%d]", GetTickCount() );
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	} // end if
	g_pGameApp = new cFishApp( g_hWnd, cGameApp::m_spOpenGLRender->m_vGameResolution, Vector2(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y) );
	g_pGameApp->Init();
	SetTimer( g_hWnd, 0, 0, NULL ) ;

	HWND hWindow = FindWindow(NULL,_T("系統設定變更"));
	if( hWindow )
	{
		PostMessage(hWindow,WM_COMMAND,7,0);
	}
    //MouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInstance,0);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)&&!g_bLeave)
	//while(1)
	{
		//g_pGameApp->Run();
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//UnhookWindowsHookEx(MouseHook);
	SAFE_DELETE(g_pGameApp);
	_CrtDumpMemoryLeaks();
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//







ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//







BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
	bool	l_bFullScreen = false;
	cNodeISAX	l_NodeISAX;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.z = 1024.;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.w = 768.f;
	cFishApp::ResoluctionParse2( "FishSetup.xml" );

	DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
	if(cGameApp::m_sbFullScreen)
		l_dwFlag = WS_VISIBLE | WS_POPUP |	WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height(), NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
	  return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
POINT g_MousePosition;
bool	g_bLeftMouseDown = false;
bool	g_bRightMouseDown = false;
short	g_cMouseWhellDelta = 0;
bool	g_bTimerDone = false;







LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_cMouseWhellDelta = 0;
	float   l_fScaleX = cGameApp::m_spOpenGLRender->m_vGameResolution.x/ cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
	float   l_fScaleY = cGameApp::m_spOpenGLRender->m_vGameResolution.y/ cGameApp::m_spOpenGLRender->m_vViewPortSize.y;
	switch (message)
	{
	case  WM_SIZE:
		cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)LOWORD(lParam);
		cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)HIWORD(lParam);
		break;
	case WM_TIMER:
		if( !g_bLeave && g_pGameApp && g_bTimerDone == false )
		{
			g_bTimerDone = true;
			g_pGameApp->Run();
			cFishApp*l_pFishApp = (cFishApp*)g_pGameApp;
			g_bTimerDone = false;
		}
		//else
		//	SAFE_DELETE(g_pGameApp);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//case WM_KEYFIRST:
	//	if( g_pGameApp )
	//	{
	//		g_pGameApp->KeyDown(LOWORD(wParam));
	//	}
	//	break;

	case WM_KEYDOWN:
		if( g_pGameApp )
		{
			g_pGameApp->KeyDown((char)LOWORD(wParam));
		}
		switch(LOWORD(wParam))
		{
			case VK_RETURN:
			{

			}
			break;
			case VK_ESCAPE:
				if(g_pGameApp->m_sbDebugFunctionWorking)
				{
					g_bLeave = true;
				}
				break;
			break;
		}
		break;
	case WM_KEYUP:
		if( g_pGameApp )
		{
			g_pGameApp->KeyUp((char)LOWORD(wParam));
		}
		break;
	//case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
		g_cMouseWhellDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_LBUTTONDOWN:
		g_bLeftMouseDown = true;
		g_bRightMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseDown(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_LBUTTONUP:
		g_bLeftMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseUp(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_RBUTTONDOWN:
		g_bRightMouseDown = true;
		g_bLeftMouseDown = false;
			return 0;
	case WM_RBUTTONUP:
		g_bRightMouseDown = false;
			return 0;
	case WM_MOUSEMOVE:
		g_MousePosition.x = (int)(GET_X_LPARAM(lParam));
		g_MousePosition.y = (int)(GET_Y_LPARAM(lParam));
		g_pGameApp->m_sMousePosition = g_MousePosition;
		if( g_pGameApp )
			g_pGameApp->MouseMove(g_MousePosition.x,g_MousePosition.y);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}








