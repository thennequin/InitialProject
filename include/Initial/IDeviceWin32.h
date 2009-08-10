//---------------------------------------------
//                Initial Engine
//                Device  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IDEVICEWIN32_HEADER_
#define _IDEVICEWIN32_HEADER_

#include "Initial/IDevice.h"
#include "Initial/Core/IArray.h"

#include <windows.h>
//#include <winuser.h>

namespace Initial
{	
	class IDeviceWin32 : public IDevice
	{
	public:
		IDeviceWin32(Core::IString Title, int Width, int Height, int Bits, bool Fullscreen, void *winid=NULL);
		~IDeviceWin32();

		virtual bool Run();

		virtual void SetWindowCaption(Core::IString caption);
		virtual void SetSize(int width, int height);
		virtual void SetFullScreen(bool full);
		//virtual void SetResizeAble(bool resize=false);
		
		virtual bool IsFullscreen();
		virtual bool IsWindowActive();
		virtual bool IsWindowFocused();
		virtual bool IsWindowMinimized();

		virtual void OnCustomEvent(EventType type, int val1, int val2);
		//virtual void OnResized()=0;

		int GetWidth();
		int GetHeight();

		virtual int GetMouseX(); // Get Mouse acceleration of mouse if it's lock else mouse position
		virtual int GetMouseY();

		virtual void SetMouseLock(bool lock);
		virtual bool GetMouseLock();

		virtual void SetShowMouse(bool show);
		virtual bool GetShowMouse();

		virtual bool KeyPress(int key);
		virtual bool KeyDown(int key);
		virtual bool MousePress(int button);

		virtual bool CreateRender(VideoDriverType type);
	protected:
		static LRESULT	CALLBACK Callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		HWND HWnd;
		bool m_bExtern;

		bool m_bWinActive;							// Window Active Flag Set To TRUE By Default
		bool m_bWinFocus;

		int m_iMouseAccX;
		int m_iMouseAccY;
		bool m_bLockMouse;
		bool m_bShowMouse;

		bool m_bKeys[256];						// Array Used For The Keyboard Routine
		bool m_bKeysOld[256];
		bool m_bKeysDown[256];
		bool m_bMouse[3];

		class IWin32Instance
		{
		public:
			IDeviceWin32 *Device;
			HWND Handle;
		};

		static Core::IArray<IWin32Instance> Instances;
	};

}

#endif