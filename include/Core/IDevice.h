//---------------------------------------------
//                Initial Engine
//                Device  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IDEVICE_HEADER_
#define _IDEVICE_HEADER_

#include "Core/IString.h"

namespace Initial
{	
	class IRenderDriver;

	class IDevice
	{
	public:
		//IDevice();
		//~IDevice();

		virtual bool Run()=0;
		virtual void SetWindowCaption(IString caption)=0;
		virtual bool IsFullscreen()=0;
		virtual bool IsWindowActive()=0;
		virtual bool IsWindowFocused()=0;
		virtual bool IsWindowMinimized()=0;
		virtual void OnEvent()=0;
		//void OnResized();

		//virtual void SetResizeAble(bool resize=false);
		//virtual void Set

		IRenderDriver *GetRenderDriver();

	protected:
		IRenderDriver*	m_pRenderDriver;
	};

}

#endif