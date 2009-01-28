//---------------------------------------------
//                Initial Engine
//             Windows Device source
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Core/IDeviceWin32.h"

using namespace Initial;

LRESULT	CALLBACK Callback( 
							HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL 0x020A
	#endif
	#ifndef WHEEL_DELTA
	#define WHEEL_DELTA 120
	#endif

	switch (uMsg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		return 0;

	case WM_ERASEBKGND:
		return 0;

	case WM_SETCURSOR:
	{
	}
		break;

	case WM_MOUSEWHEEL:
		break;

	case WM_LBUTTONDOWN:
		return 0;

	case WM_LBUTTONUP:
		return 0;

	case WM_RBUTTONDOWN:
		return 0;

	case WM_RBUTTONUP:
		return 0;

	case WM_MBUTTONDOWN:
		return 0;

	case WM_MBUTTONUP:
		return 0;

	case WM_MOUSEMOVE:
		return 0;

	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			return 0;
		}

	case WM_SIZE:
		{
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		// prevent screensaver or monitor powersave mode from starting
		if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
			(wParam & 0xFFF0) == SC_MONITORPOWER)
			return 0;
		break;

	case WM_USER:
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

IDeviceWin32::IDeviceWin32()
{
	bool Fullscreen=false;
	int Width=640;
	int Height=480;

	const char *Classname = "IDeviceWin32";
	HINSTANCE hInstance = GetModuleHandle(0);

	WNDCLASSEX wcex;
	wcex.cbSize		= sizeof(WNDCLASSEX);
	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)Callback;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= Classname;
	wcex.hIconSm		= 0;

	RegisterClassEx(&wcex);

	RECT clientSize;
	clientSize.top = 0;
	clientSize.left = 0;
	clientSize.right = Width;
	clientSize.bottom = Height;

	DWORD style = WS_POPUP;

	if (!Fullscreen)
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	AdjustWindowRect(&clientSize, style, FALSE);

	const int realWidth = clientSize.right - clientSize.left;
	const int realHeight = clientSize.bottom - clientSize.top;

	int windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	int windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

	if (Fullscreen)
	{
		windowLeft = 0;
		windowTop = 0;
	}

	HWnd = CreateWindow( Classname, "", style, windowLeft, windowTop,
				realWidth, realHeight, NULL, NULL, hInstance, NULL);
	//CreationParams.WindowId = HWnd;

	ShowWindow(HWnd, SW_SHOW);
	UpdateWindow(HWnd);

	// fix ugly ATI driver bugs. Thanks to ariaci
	MoveWindow(HWnd, windowLeft, windowTop, realWidth, realHeight, TRUE);


	m_pRenderDriver = NULL;
	//m_pRenderDevice = new IRenderDriverOpenGL;
}

IDeviceWin32::~IDeviceWin32()
{

}

bool IDeviceWin32::Run()
{
	MSG msg;

	bool quit = false;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);

		//if (ExternalWindow && msg.hwnd == HWnd)
			//WndProc(HWnd, msg.message, msg.wParam, msg.lParam);
		//else
			DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			quit = true;
	}

	return !quit;
}

void IDeviceWin32::SetWindowCaption(IString caption)
{
	DWORD dwResult;
	SendMessageTimeout(HWnd, WM_SETTEXT, 0,
		reinterpret_cast<LPARAM>(caption.c_str()),
		SMTO_ABORTIFHUNG, 2000, &dwResult);
}

bool IDeviceWin32::IsFullscreen()
{
	return false;
}

bool IDeviceWin32::IsWindowActive()
{
	return false;
}

bool IDeviceWin32::IsWindowFocused()
{
	return false;
}

bool IDeviceWin32::IsWindowMinimized()
{
	return false;
}

void IDeviceWin32::OnEvent()
{

}
