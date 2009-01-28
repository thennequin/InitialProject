//---------------------------------------------
//                Initial Engine
//             Render driver Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IRENDERDRIVER_HEADER_
#define _IRENDERDRIVER_HEADER_

#include "Core/IString.h"
#include "Core/IMatrix.h"
#include "3D/ITriangle.h"
#include "3D/Camera/ICamera.h"
#include "Video/ITextureManager.h"
#include "Video/IShaderManager.h"
#include "Thread/IThread.h"

namespace Initial
{
	
	class ICamera;

	class IRenderDriver
	{
	public:
		static IRenderDriver* CreateDevice(VideoDriverType type,int Width,int Height,int BitDepth, bool Fullscreen,IString Title, int AntiaAliasing=0, bool UseShader=true);

		ITextureManager *GetTextureManager();
		IShaderManager *GetShaderManager();
		IString GetError() { return m_sError; }
		void Render();

		int GetWidth();
		int GetHeight();

		void SetCamera(ICamera *camera);
		ICamera* GetCamera() { return m_pCamera; }

		int GetMouseAccX(); // Get Mouse acceleration of mouse is lock else is mouse position
		int GetMouseAccY();
		void SetMouseLock(bool lock);
		bool GetMouseLock();
		void SetShowMouse(bool show);
		bool GetShowMouse();

		unsigned int PolygonDrawsCount();
		unsigned int LineDrawsCount();

		virtual bool KeyPress(int key)=0;
		virtual bool KeyDown(int key)=0;
		virtual bool MousePress(int button)=0;

		virtual void SetSize(int width, int height)=0;
		virtual void SetFullScreen(bool full)=0;
		virtual void SetTitle(IString title)=0;

		virtual bool UpdateDevice()=0;

		virtual void BeginRender()=0;
		virtual void EndRender(ITexture *destRGBA=NULL, ITexture *destDepth=NULL)=0;

		// 3D operations
		virtual void _SetModelViewMatrix()=0;
		virtual void _SetProjectionMatrix()=0;
		virtual void _SetTextureMatrix()=0;
		virtual void _PushMatrix()=0;
		virtual void _PopMatrix()=0;
		virtual void _LoadIdentity()=0;
		virtual void _MultMatrix(Math::IMatrix mat)=0;
		virtual void _SetMatrix(Math::IMatrix mat)=0;
		virtual Math::IMatrix _GetMatrix()=0;
		virtual Math::IMatrix _GetProjectionMatrix()=0;
		virtual Math::IMatrix _GetModelViewMatrix()=0;
		virtual Math::IMatrix _GetTextureMatrix()=0;
		
		virtual void _Translate(float x, float y, float z)=0;
		virtual void _RotateX(float x)=0;
		virtual void _RotateY(float y)=0;
		virtual void _RotateZ(float z)=0;	
		virtual void _Scale(float x=1.0, float y=1.0, float z=1.0)=0;

		virtual void _StartPolyDraw(bool Wireframe)=0;
		virtual void _EndPolyDraw()=0;
		virtual void _DrawPolygon(ITriangle* polygon/*, bool NoColor=false,IColor ColorPower=IColor(1,1,1)*/)=0;
		virtual void _DrawLine(const IVertex& pt1,const IVertex& pt2)=0;
		virtual void _SetColor(IColor color)=0;
		virtual void _SetLineSize(float size=1.0)=0;
		
		virtual void _EnableExt(DeviceExt ext)=0;
		virtual void _DisableExt(DeviceExt Ext)=0;

		virtual unsigned int _NewList()=0;
		virtual void _CallList(unsigned int listid)=0;
		virtual void _StartList(unsigned int listid, bool execute=false)=0;
		virtual void _EndList()=0;
		virtual void _DeleteList(unsigned int listid)=0;


		virtual ITexture* LoadTexture(IString filename)=0;
		virtual IShader* LoadShader(IString vertex, IString fragment)=0;

		void UseMaterial(IMaterial *mat);

		virtual void SetAntialising(int power)=0;

	protected:
		IRenderDriver(int Width,int Height,int BitDepth, bool Fullscreen, IString Title, int AntiAliasingPower, bool UseShader);

	protected:
		int		m_iWidth;
		int		m_iHeight;
		int		m_iBitDepth;
		bool	m_bFullscreen;
		IString	m_sTitle;
		IString	m_sError;

		ITextureManager	*m_pTextureManager;
		IShaderManager	*m_pShaderManager;
		
		ICamera *m_pCamera;
		bool	m_bInRender;

		int				m_iMouseAccX;
		int				m_iMouseAccY;
		bool			m_bLockMouse;
		bool			m_bShowMouse;

		int				m_iAntiAliasing;
		bool			m_bAntiAliasingEnable;
		bool			m_bUseShader;

		unsigned int	m_iPolygonCount;
		unsigned int	m_iLineCount;

		IMaterial*		m_pCurrentMaterial;

		ITexture*		m_pScreenTexture;
		ITexture*		m_pDepthTexture;
	};

}

#endif