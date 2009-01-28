//---------------------------------------------
//                Initial Engine
//          OpenGL Render Device Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IRENDERDRIVEROPENGL_HEADER_
#define _IRENDERDRIVEROPENGL_HEADER_

#include "Video/IRenderDriver.h"
#include "3D/ITriangle.h"
#include "Video/ITexture.h"

#include <windows.h>

namespace Initial
{

	class IRenderDriverOpenGL : public IRenderDriver
	{
		friend class IRenderDriver;
	public:
		virtual bool KeyPress(int key);
		virtual bool KeyDown(int key);
		virtual bool MousePress(int button);

		virtual void SetSize(int width, int height);
		virtual void SetFullScreen(bool full);
		virtual void SetTitle(IString title);

		virtual bool UpdateDevice();

		virtual void BeginRender();
		virtual void EndRender(ITexture *destRGBA=NULL, ITexture *destDepth=NULL);

		// 3D operations if in render
		virtual void _SetModelViewMatrix();
		virtual void _SetProjectionMatrix();
		virtual void _SetTextureMatrix();
		virtual void _SetIdentityMatrix();
		virtual void _PushMatrix();
		virtual void _PopMatrix();
		virtual void _LoadIdentity();
		virtual void _MultMatrix(Math::IMatrix mat);
		virtual void _SetMatrix(Math::IMatrix);
		virtual Math::IMatrix _GetMatrix();
		virtual Math::IMatrix _GetProjectionMatrix();
		virtual Math::IMatrix _GetModelViewMatrix();
		virtual Math::IMatrix _GetTextureMatrix();
		
		virtual void _Translate(float x, float y, float z);
		virtual void _RotateX(float x);
		virtual void _RotateY(float y);
		virtual void _RotateZ(float z);
		virtual void _Scale(float x=1.0, float y=1.0, float z=1.0);

		virtual void _StartPolyDraw(bool Wireframe);
		virtual void _EndPolyDraw();
		virtual void _DrawPolygon(ITriangle* polygon);
		virtual void _DrawLine(const IVertex& pt1,const IVertex& pt2);
		virtual void _SetColor(IColor color);
		virtual void _SetLineSize(float size=1.0);

		virtual void _EnableExt(DeviceExt ext);
		virtual void _DisableExt(DeviceExt Ext);

		virtual unsigned int _NewList();
		virtual void _CallList(unsigned int listid);
		virtual void _StartList(unsigned int listid, bool execute=true);
		virtual void _EndList();
		virtual void _DeleteList(unsigned int listid);


		virtual ITexture* LoadTexture(IString filename);
		virtual IShader* LoadShader(IString vertex, IString fragment);

		virtual void SetAntialising(int power);

	protected:
		IRenderDriverOpenGL(int Width,int Height,int BitDepth, bool Fullscreen, IString Title, int AntiAliasingPower, bool UseShader);

		bool InitPixelFormat(int AntiAliasingPower);
		bool CreateGLWindow();
		bool KillGLWindow();
		bool InitGL();
		bool ResizeGL(int width, int height);
		static LRESULT	CALLBACK Callback(HWND, UINT, WPARAM, LPARAM);

		void Destroy();
	private:
		int WinId;

		HGLRC		hRC;							// Permanent Rendering Context
		HDC			hDC;							// Private GDI Device Context
		HWND		hWnd;							// Holds Our Window Handle
		HINSTANCE	hInstance;						// Holds The Instance Of The Application
		//PIXELFORMATDESCRIPTOR	pfd;
		unsigned int			PixelFormat;					// Holds The Results After Searching For A Match
		
		bool		keys[256];						// Array Used For The Keyboard Routine
		bool		previousKeys[256];
		bool		keysDown[256];						// Array Used For The Keyboard Routine
		bool		mouse[3];
		bool		active;							// Window Active Flag Set To TRUE By Default
		bool		focus;

		class IInstance
		{
		public:
			IRenderDriverOpenGL *Device;
			HWND Handle;
		};
		static IArray<IInstance> Instances;
	};

}

#endif