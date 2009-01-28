//---------------------------------------------
//                Initial Engine
//                Device  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IDEVICEWIN32_HEADER_
#define _IDEVICEWIN32_HEADER_

#include "Core/IDevice.h"
#include <windows.h>
//#include <winuser.h>

namespace Initial
{	
	class IDeviceWin32 : public IDevice
	{
	public:
		IDeviceWin32();
		~IDeviceWin32();

		virtual bool Run();
		virtual void SetWindowCaption(IString caption);
		virtual bool IsFullscreen();
		virtual bool IsWindowActive();
		virtual bool IsWindowFocused();
		virtual bool IsWindowMinimized();
		virtual void OnEvent();
		//void OnResized();

		//virtual void SetResizeAble(bool resize=false);

		IRenderDriver *GetRenderDriver();

	protected:
		HWND HWnd;
	};

}

#endif