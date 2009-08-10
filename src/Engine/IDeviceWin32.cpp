//---------------------------------------------
//                Initial Engine
//             Windows Device source
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/IDeviceWin32.h"
#include "Initial/ILogger.h"

#include "Initial/Video/OpenGLDrv/IRenderDriverOpenGL.h"

using namespace Initial::Core;

namespace Initial
{
	IArray<IDeviceWin32::IWin32Instance> IDeviceWin32::Instances;

	LRESULT	CALLBACK IDeviceWin32::Callback( 
								HWND	hWnd,			// Handle For This Window
								UINT	uMsg,			// Message For This Window
								WPARAM	wParam,			// Additional Message Information
								LPARAM	lParam)			// Additional Message Information
	{
		IDeviceWin32 *device=NULL;
		for (int i=0;i<IDeviceWin32::Instances.Count();i++)
		{
			if (hWnd==Instances[i].Handle)
			{
				device=Instances[i].Device;
				break;
			}
		}

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
			break;

		case WM_MOUSEWHEEL:
			break;

		case WM_LBUTTONDOWN:
			if (device)
				device->m_bMouse[0]=true;
			return 0;

		case WM_LBUTTONUP:
			if (device)
				device->m_bMouse[0]=false;
			return 0;

		case WM_RBUTTONDOWN:
			if (device)
				device->m_bMouse[1]=true;
			return 0;

		case WM_RBUTTONUP:
			if (device)
				device->m_bMouse[1]=false;
			return 0;

		case WM_MBUTTONDOWN:
			if (device)
				device->m_bMouse[2]=true;
			return 0;

		case WM_MBUTTONUP:
			if (device)
				device->m_bMouse[2]=false;
			return 0;

		case WM_MOUSEMOVE:
			return 0;

		case WM_KEYDOWN:
			if (device)
			{
				device->m_bKeysDown[wParam] = true;
			}
			return 0;
		case WM_KEYUP:
			if (device)
			{
				device->m_bKeysDown[wParam] = false;
			}
			return 0;

		case WM_SIZE:
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

		case WM_ACTIVATE:							// Watch For Window Activate Message
			if (!HIWORD(wParam))					// Check Minimization State
			{
				if (device)
					device->m_bWinActive=true;				
			}else{
				if (device)
					device->m_bWinActive=false;
			}
			return 0;								// Return To The Message Loop
		
		case WM_SETFOCUS:
			if (device)
				device->m_bWinFocus=true;
			return 0;

		case WM_KILLFOCUS:
			if (device)
				device->m_bWinFocus=false;
			return 0;

		case WM_USER:
			return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	IDeviceWin32::IDeviceWin32(Core::IString Title, int Width, int Height, int Bits, bool Fullscreen, void *WinId)
	{
		ILogger::LogDebug("%d\n",WinId);
		if (!WinId)
		{
			m_bExtern=false;
			ILogger::LogDebug("Create Windows\n");
			m_iWidth=Width;
			m_iHeight=Height;

			const char *Classname = "IDeviceWin32";
			HINSTANCE hInstance = GetModuleHandle(0);

			WNDCLASSEX wcex;
			wcex.cbSize		= sizeof(WNDCLASSEX);
			wcex.style		= CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	= (WNDPROC)IDeviceWin32::Callback;
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

			HWnd = CreateWindow( Classname, Title.c_str(), style, windowLeft, windowTop,
						realWidth, realHeight, NULL, NULL, hInstance, NULL);
			//CreationParams.WindowId = HWnd;

			ShowWindow(HWnd, SW_SHOW);
			UpdateWindow(HWnd);

			// fix ugly ATI driver bugs. Thanks to ariaci
			MoveWindow(HWnd, windowLeft, windowTop, realWidth, realHeight, TRUE);
		}else{
			m_bExtern=true;
			ILogger::LogDebug("Use existing Windows\n");
			// attach external window
			HWnd = *((HWND*)WinId);
			RECT rect;
			GetWindowRect(HWnd, &rect);
			m_iWidth = rect.right - rect.left;
			m_iHeight = rect.bottom - rect.top;
		}

		for (int i=0;i<256;i++)
		{
			m_bKeys[i]=false;					// Array Used For The Keyboard Routine
			m_bKeysOld[i]=false;
			m_bKeysDown[i]=false;
		}
		for (int i=0;i<3;i++)
			m_bMouse[i]=false;

		m_bWinActive=true;
		m_bWinFocus=true;
		m_bShowMouse=true;
		m_bLockMouse=false;

		m_iMouseAccX=0;
		m_iMouseAccY=0;

		IWin32Instance inst;
		inst.Device=this;
		inst.Handle=HWnd;
		Instances.Add(inst);
	}

	IDeviceWin32::~IDeviceWin32()
	{
		for (UINT i=0;i<Instances.Count();i++)
		{
			if (Instances[i].Device==this)
			{
				Instances.Delete(i);
				break;
			}
		}
	}

	bool IDeviceWin32::Run()
	{
		bool quit = false;

		if (m_bExtern==false)
		{
			MSG msg;			

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
		}

		//Update key state
		for (int i=0;i<256;i++)
		{
			m_bKeysOld[i]=m_bKeys[i];
			m_bKeys[i]=m_bKeysDown[i];
		}
		
		POINT point;
		GetCursorPos(&point);
		if (m_bLockMouse)
		{
			if (m_bWinFocus)
			{
				tagRECT rect;
				GetWindowRect(HWnd,&rect);
				int BorderX = rect.left;
				int BorderY = rect.top;
				m_iMouseAccX=point.x-BorderX-m_iWidth/2;
				m_iMouseAccY=point.y-BorderY-m_iHeight/2;
				SetCursorPos(BorderX+m_iWidth/2,BorderY+m_iHeight/2);
			}else{
				m_iMouseAccX=0;
				m_iMouseAccY=0;
			}
		}else{
			m_iMouseAccX=point.x;
			m_iMouseAccY=point.y;
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
		return m_bWinFocus;
	}

	bool IDeviceWin32::IsWindowMinimized()
	{
		return m_bWinActive;
	}

	void IDeviceWin32::OnCustomEvent(EventType type, int val1, int val2)
	{
		switch (type)
		{
		case IET_MOUSE_MOVE: //X,Y
			if (!m_bLockMouse)
			{
				m_iMouseAccX=val1;
				m_iMouseAccY=val2;
			}
			break;
		case IET_MOUSE_CLICK: // ButtonId, Down
			m_bMouse[val1]=val2;
			break;
		case IET_MOUSE_WHELL: // Delta, none
			break;
		case IET_KEY_PUSH: // KeyId, Down
			m_bKeysDown[val1]=val2;
			break;
		case IET_RESIZE: //Width, Height
			m_iWidth=val1;
			m_iHeight=val2;
			GetRenderDriver()->Resize(val1,val2);
			break;
		case IET_ACTIVE: // Active, none
			m_bWinActive=val1;
			break;
		case IET_FOCUS: // Focus, none
			m_bWinFocus=val1;
			break;
		}
	}

	bool IDeviceWin32::CreateRender(VideoDriverType type)
	{
		if (type==IVDT_OPENGL)
			m_pRenderDriver = new Video::IRenderDriverOpenGL(this,&HWnd,m_iWidth,m_iHeight,m_iBits);
			
		if (m_pRenderDriver)
			m_pRenderDriver->InitDriver();
		return true;
	}

	void IDeviceWin32::SetFullScreen(bool full)
	{
	}

	void IDeviceWin32::SetSize(int width, int height)
	{
	}

	int IDeviceWin32::GetMouseX()
	{
		return m_iMouseAccX;
	}

	int IDeviceWin32::GetMouseY()
	{
		return m_iMouseAccY;
	}

	void IDeviceWin32::SetMouseLock(bool lock)
	{
		m_bLockMouse=lock;
		if (lock)
		{
			m_iMouseAccX=0;
			m_iMouseAccY=0;
			tagRECT rect;
			GetWindowRect(HWnd,&rect);
			int BorderX = rect.left;
			int BorderY = rect.top;
			SetCursorPos(BorderX+m_iWidth/2,BorderY+m_iHeight/2);
		}
	}

	bool IDeviceWin32::GetMouseLock()
	{
		return m_bLockMouse;
	}

	void IDeviceWin32::SetShowMouse(bool show)
	{
		ShowCursor(show);
		m_bShowMouse=show;
	}

	bool IDeviceWin32::GetShowMouse()
	{
		return m_bShowMouse;
	}

	bool IDeviceWin32::KeyPress(int key)
	{
		if (key>=0 && key<256)
		{
			return m_bKeysOld[key]!=m_bKeys[key] && m_bKeys[key]==true;
		}
		return false;
	}

	bool IDeviceWin32::KeyDown(int key)
	{
		if (key>=0 && key<256)
			return m_bKeys[key];
		return false;
	}

	bool IDeviceWin32::MousePress(int button)
	{
		return m_bMouse[button];
	}
}
