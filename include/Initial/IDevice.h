//---------------------------------------------
//                Initial Engine
//                Device  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IDEVICE_HEADER_
#define _IDEVICE_HEADER_

#include "Initial/IEnum.h"
#include "Initial/IRessourceManager.h"
#include "Initial/ISceneManager.h"
#include "Initial/gui/IFontManager.h"
#include "Initial/Core/IString.h"

namespace Initial
{	
	namespace Video
	{
		class IRenderDriver;
	}

	class IDevice
	{
	public:
		enum EventType
		{
			IET_MOUSE_MOVE, //X,Y
			IET_MOUSE_CLICK, // ButtonId, Down
			IET_MOUSE_WHELL, // Delta, none
			IET_KEY_PUSH, // KeyId, Down
			IET_RESIZE, //Width, Height
			IET_ACTIVE, // Active, none
			IET_FOCUS, // Focus, none
		};
	public:
		int GetWidth();
		int GetHeight();

		virtual bool Run()=0;

		virtual void SetWindowCaption(Core::IString caption)=0;
		virtual void SetSize(int width, int height)=0;
		virtual void SetFullScreen(bool full)=0;
		//virtual void SetResizeAble(bool resize=false);
		
		virtual bool IsFullscreen()=0;
		virtual bool IsWindowActive()=0;
		virtual bool IsWindowFocused()=0;
		virtual bool IsWindowMinimized()=0;

		virtual void OnCustomEvent(EventType type, int val1, int val2)=0;
		//virtual void OnResized()=0;

		virtual int GetMouseX()=0; // Get Mouse acceleration of mouse if it's lock else mouse position
		virtual int GetMouseY()=0;

		virtual void SetMouseLock(bool lock)=0;
		virtual bool GetMouseLock()=0;

		virtual void SetShowMouse(bool show)=0;
		virtual bool GetShowMouse()=0;

		virtual bool KeyPress(int key)=0;
		virtual bool KeyDown(int key)=0;
		virtual bool MousePress(int button)=0;

		Video::IRenderDriver *GetRenderDriver();
		IRessourceManager *GetRessourceManager();
		ISceneManager *GetSceneManager();
		GUI::IFontManager *GetFontManager();

		virtual bool CreateRender(VideoDriverType type)=0;
	protected:
		IDevice();
		//~IDevice();
	protected:
		Video::IRenderDriver*	m_pRenderDriver;
		IRessourceManager*		m_pRessourceManager;
		ISceneManager*			m_pSceneManager;
		GUI::IFontManager*		m_pFontManager;

		int m_iWidth;
		int m_iHeight;
		int m_iBits;
	};

}

#endif