// CallExe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CallExe.h"
#include "windows.h"
#include "Windowsx.h"
#include <string>
#include <vector>
#include "HIDCheck.h"

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

//#pragma comment(lib, "Vfw32.lib")
#pragma comment(lib, "../../../../lib/OpenAL32.lib")
#pragma comment(lib, "../../../../lib/alut.lib")
#ifdef _OGG_
#pragma comment(lib, "../../../../lib/ogg.lib")
#endif
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "../../../../lib/glew32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "../../../../lib/devIL.lib")
#pragma comment(lib, "Urlmon.lib")
#ifdef DEBUG
#pragma comment(lib, "../../../../lib/Debug/BulletCollision.lib")
#pragma comment(lib, "../../../../lib/Debug/BulletDynamics.lib")
#pragma comment(lib, "../../../../lib/Debug/LinearMath.lib")
#else
#pragma comment(lib, "../../../../lib/Release/BulletCollision.lib")
#pragma comment(lib, "../../../../lib/Release/BulletDynamics.lib")
#pragma comment(lib, "../../../../lib/Release/LinearMath.lib")
#endif
#endif//end win32

#define MAX_LOADSTRING 100





// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	std::string	l_strHID = GetEncryptHip();
	//please change this ID
	const char*	l_strResultHID = g_strResultHID;
	//const WCHAR*l_strExeFile = L"123";
	//setup where the exe file is
	WCHAR cCurrentPath[FILENAME_MAX];
	GetCurrentDirectory(FILENAME_MAX,cCurrentPath);
	std::wstring	l_temp = g_strExeWorkingDirecotry;
	std::wstring l_strWorkingDirectory = DecodeStringFromeValue((WCHAR*)l_temp.c_str(),L",").c_str();

	if( strcmp(l_strHID.c_str(),l_strResultHID) || wcscmp(cCurrentPath,l_strWorkingDirectory.c_str()) )
	{
		UT::ErrorMsg(l_strHID.c_str(),g_strResultHID);
		//DeleteAllFiles();
		//Sleep(10000000);
		return 0;
	}
	else
	{
		WCHAR	l_strForStringCommandline[3000];
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		//l_strWorkingDirectory = L"C:/Users/Fatming/Desktop/Fish/shrimp/Fish-2-0-0-87-Lee-GHOST/";
		//swprintf(l_strForStringCommandline,3000,L"%s%s %s",l_strWorkingDirectory.c_str(),L"Fish-Mermaid-Win32.exe",UT::CharToWchar(l_strResultHID));

		swprintf(l_strForStringCommandline,3000,L"%s%s %s",l_strWorkingDirectory.c_str(),g_strExeFileName,UT::CharToWchar(l_strResultHID));
		CreateProcess(0,l_strForStringCommandline,0,0,0,CREATE_NEW_CONSOLE,0,0,&si,&pi);
	}

	return (int) 0;
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALLEXE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CALLEXE);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //hWnd = CreateWindow(L"", L"", 0,
      0, 0, 0, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
