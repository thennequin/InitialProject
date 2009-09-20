//---------------------------------------------
//                Initial Engine
//          OpenGL Render Device Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IRENDERDRIVEROPENGL_HEADER_
#define _IRENDERDRIVEROPENGL_HEADER_

#include "Initial/Video/IRenderDriver.h"
#include "Initial/3D/ITriangle.h"
#include "Initial/Ressource/ITexture.h"

#include <windows.h>

namespace Initial
{
	class IDevice;
	class IDeviceWin32;

	namespace Video
	{
		class IRenderDriverOpenGL : public IRenderDriver
		{
			friend class IRenderDriver;
			friend class IDevice;
			friend class IDeviceWin32;
		public:

			virtual void SetCurrent();

			virtual void _SetClearColor(Core::IColor color);
			virtual void _ClearBuffer(VideoBuffer buffer);
			virtual void BeginRender(IFrameBuffer *dest=NULL, bool renderDirectly=false, bool clearbuffer=true);
			virtual void EndRender();
			virtual void GetRenderInTexture(ITexture *rgba, ITexture *depth);
			virtual void SwapBuffer();

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

			virtual void _StartTriangleDraw(bool Wireframe);
			virtual void _EndTriangleDraw();
			virtual void _DrawTriangle(ITriangle* polygon);
			virtual void _DrawLine(const Core::IVector3D& pt1,const Core::IVector3D& pt2);
			virtual void _SetColor(Core::IColor color);
			virtual void _SetLineSize(float size=1.0);

			virtual void _DrawQuad(Core::IVector3D a,Core::IVector3D b,Core::IVector3D c,Core::IVector3D d);
			virtual void _DrawQuadPP(float x=0.0, float y=0.0,float w=1.0, float h=1.0);

			virtual void _EnableExt(DeviceExt ext);
			virtual void _DisableExt(DeviceExt Ext);
			virtual void _EnableDepthWrite(bool enable);
			virtual void _EnableColorWrite(bool enable);
			virtual void _EnableDepthTest(bool enable);

			virtual void _StencilFunc(AlphaFunc func, int ref, unsigned int mask);
			virtual void _StencilOp(StencilOp fail, StencilOp zfail, StencilOp zpass);
			virtual void _FrontFace(FrontFaceDirection face);

			virtual void _BlendMode(BlendMode sfactor, BlendMode dfactor);

			virtual unsigned int _NewList();
			virtual void _CallList(unsigned int listid);
			virtual void _StartList(unsigned int listid, bool execute=true);
			virtual void _EndList();
			virtual void _DeleteList(unsigned int listid);

			virtual unsigned int _NewFrameBuffer();
			virtual void _DeleteFrameBuffer(unsigned int id);
			virtual void _UseFrameBuffer(IFrameBuffer *buffer);
			virtual void _AttachTextureToFrameBuffer(int pos, IFrameBuffer *buffer, ITexture *tex);
			virtual void _AttachDepthTextureToFrameBuffer(IFrameBuffer *buffer, ITexture *tex);

			//virtual ITexture* LoadTexture(Core::IString filename);
			virtual IShader* LoadShader(Core::IString vertex, Core::IString fragment);

		protected:
			IRenderDriverOpenGL(IDevice *device, void *WinId, int Width, int Height, int Bits);

			virtual bool InitDriver();
			bool InitGL();
			virtual void _ResizeDriver();
		private:

			HGLRC		hRC;							// Permanent Rendering Context
			HDC			hDC;							// Private GDI Device Context
			HWND		hWnd;							// Holds Our Window Handle
			unsigned int			PixelFormat;					// Holds The Results After Searching For A Match

			long m_lTime;
		};
	}
}

#endif