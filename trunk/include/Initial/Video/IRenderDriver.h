//---------------------------------------------
//                Initial Engine
//             Render driver Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IRENDERDRIVER_HEADER_
#define _IRENDERDRIVER_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Math/IMatrix.h"
#include "Initial/3D/ITriangle.h"
#include "Initial/3D/Camera/ICamera.h"
#include "Initial/Video/ITextureManager.h"
#include "Initial/Video/IShaderManager.h"
#include "Initial/Video/IVBOManager.h"
#include "Initial/Video/IFrameBuffer.h"
#include "Initial/Thread/IThread.h"
#include "Initial/Node/INode.h"
//#include "Initial/3D/IFrustum.h"

namespace Initial
{
	class IDevice;
	class ICamera;
	class IDevice;
	class IDeviceWin32;
	class IFrustum;

	namespace Video
	{
		class IRenderDriver
		{
			friend class IDevice;
			friend class IDeviceWin32;
		public:
			struct IRenderParams
			{
				IRenderParams()
				{
					EnableLights=true;
				}
				bool EnableLights;
				//bool CustomPostProcess;
				//bool EnableBloom;
			};

			enum RenderMode
			{
				IRM_NORMAL,			// With light
				IRM_NO_LIGHTING,	// Show Albedo
				IRM_WIREFRAME,		// Show wireframe scene
				IRM_SKETCH			// Show sketch scene
				//IRM_INFRARED, //For example
			};
		public:
			ITextureManager *GetTextureManager();
			IShaderManager *GetShaderManager();
			Video::IVBOManager *GetVBOManager();
			Core::IString GetError() { return m_sError; }
			
			void Render(INode *node, IFrustum *frustum, int flags=0);

			void SetCamera(ICamera *camera);
			ICamera* GetCamera() { return m_pCamera; }

			unsigned int PolygonDrawsCount();
			unsigned int LineDrawsCount();

			void PostProcess(Math::IMatrix& proj);

			void UseMaterial(IMaterial *mat);

			void Resize(int Width, int Height);

			void ResizeTexturesPP(int Width, int Height);

			void SetLightEnable(bool enable);
			bool GetLightEnable();

			void RenderCamera(ICamera *camera, INode *node=NULL, IFrameBuffer *dest=NULL, IRenderParams params=IRenderParams());

			// 0 = Light, 1 = Albedo, 2 = Normal, 3 = Diffuse, 4 = Spec, 5 = Depth : 6 = Bloom
			void SetOutputTexture(int tex=0);

			virtual void SetCurrent()=0;

			virtual void _SetClearColor(Core::IColor color)=0;
			virtual void _ClearBuffer(VideoBuffer buffer)=0;
			virtual void BeginRender(IFrameBuffer *dest=NULL, bool renderDirectly=false, bool clearbuffer=true)=0;
			virtual void EndRender()=0;
			virtual void SwapBuffer()=0;
			virtual void GetRenderInTexture(ITexture *rgba, ITexture *depth)=0;

			// Matrix operations
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
			
			//3D Operation on current matrix
			virtual void _Translate(float x, float y, float z)=0;
			virtual void _RotateX(float x)=0;
			virtual void _RotateY(float y)=0;
			virtual void _RotateZ(float z)=0;	
			virtual void _Scale(float x=1.0, float y=1.0, float z=1.0)=0;

			virtual void _StartTriangleDraw(bool Wireframe)=0;
			virtual void _EndTriangleDraw()=0;
			virtual void _DrawTriangle(ITriangle* polygon/*, bool NoColor=false,Core::IColor ColorPower=Core::IColor(1,1,1)*/)=0;
			virtual void _DrawLine(const Core::IVector3D& pt1,const Core::IVector3D& pt2)=0;
			virtual void _SetColor(Core::IColor color)=0;
			virtual void _SetLineSize(float size=1.0)=0;

			virtual void _DrawQuad(Core::IVector3D a=Core::IVector3D(0,0,1),Core::IVector3D b=Core::IVector3D(1,0,1),Core::IVector3D c=Core::IVector3D(1,1,1),Core::IVector3D d=Core::IVector3D(0,1,1))=0;
			virtual void _DrawQuadPP(float x=0.0, float y=0.0,float w=1.0, float h=1.0)=0;
			
			virtual void _EnableExt(DeviceExt ext)=0;
			virtual void _DisableExt(DeviceExt Ext)=0;
			virtual void _EnableDepthWrite(bool enable)=0;
			virtual void _EnableColorWrite(bool enable)=0;
			virtual void _EnableDepthTest(bool enable)=0;

			virtual void _StencilFunc(AlphaFunc func, int ref, unsigned int mask)=0;
			virtual void _StencilOp(StencilOp fail, StencilOp zfail, StencilOp zpass)=0;
			virtual void _FrontFace(FrontFaceDirection face)=0;

			virtual void _BlendMode(BlendMode sfactor, BlendMode dfactor)=0;

			virtual unsigned int _NewList()=0;
			virtual void _CallList(unsigned int listid)=0;
			virtual void _StartList(unsigned int listid, bool execute=false)=0;
			virtual void _EndList()=0;
			virtual void _DeleteList(unsigned int listid)=0;

			virtual unsigned int _NewFrameBuffer()=0;
			virtual void _DeleteFrameBuffer(unsigned int id)=0;
			virtual void _UseFrameBuffer(IFrameBuffer *buffer)=0;
			virtual void _AttachTextureToFrameBuffer(int pos, IFrameBuffer *buffer, ITexture *tex)=0;
			virtual void _AttachDepthTextureToFrameBuffer(IFrameBuffer *buffer, ITexture *tex)=0;

			//virtual ITexture* LoadTexture(Core::IString filename)=0;
			virtual IShader* LoadShader(Core::IString vertex, Core::IString fragment)=0;
		protected:
			IRenderDriver(IDevice *device, void *WinId);
			virtual bool InitDriver()=0;
			void _Inited();
			virtual void _ResizeDriver()=0;

			void _Lighting(Math::IMatrix& cameraMatrix, IFrustum &frustum);
			void _PostProcess(Math::IMatrix& cameraMatrix, IFrameBuffer *dest);

			ITexture *_Bloom(Math::IMatrix& cameraMatrix, ITexture *InputTex);
		protected:
			IDevice*		m_pDevice;
			int				m_iWidth;
			int				m_iHeight;
			int				m_iBitDepth;
			Core::IString	m_sError;

			ITextureManager		*m_pTextureManager;
			IShaderManager		*m_pShaderManager;
			Video::IVBOManager	*m_pVBOManager;
			
			ICamera *m_pCamera;
			bool	m_bInRender;

			unsigned int	m_iPolygonCount;
			unsigned int	m_iLineCount;

			IMaterial*		m_pCurrentMaterial;

			bool			m_bRenderToScreen;
			bool			m_bLightEnable;
			int				m_iOutputTexture;

			float			m_fRenderScale;

			IFrameBuffer*	m_pFrameBuffer;
				ITexture*		m_pFrameDiffuseTexture;
				ITexture*		m_pFrameEmissiveTexture;
				ITexture*		m_pFrameDepthTexture;
				ITexture*		m_pFrameNormalTexture;
				ITexture*		m_pFrameSpecularTexture;

			IFrameBuffer*	m_pDeferredLightBuffer;
				ITexture*		m_pDeferredLightTexture;

			IFrameBuffer*	m_pBloomBuffer;
				ITexture*		m_pBloomTexture;

			IFrameBuffer*	m_pDofBuffer;
				ITexture*		m_pDofTexture;

			IFrameBuffer*	m_pBlurBuffer;
				ITexture*		m_pBlurTexture;

			IFrameBuffer*	m_pTempBuffer;
				ITexture*		m_pTempTexture;

			IFrameBuffer*	m_pTemp2Buffer;
				ITexture*		m_pTemp2Texture;

			IFrameBuffer*	m_pFrameHalfBuffer;
				ITexture*		m_pFrameHalfTexture;

			IFrameBuffer*	m_pFrameQuartBuffer;
				ITexture*		m_pFrameQuartTexture;

			IFrameBuffer*	m_pFrameQuart2Buffer;
				ITexture*		m_pFrameQuart2Texture;

			IMaterial*		m_pPostProcessBloom;

			IFrameBuffer*	m_pShadowMapBuffer;
				ITexture*		m_pShadowMapDepthTexture;
				ITexture*		m_pShadowMapColorTexture;

			//Post process parameter

			int m_vCurrentBufferWidth;
			int m_vCurrentBufferHeight;

			bool m_bBlurEnable;
			int m_iBlurPass;
			float m_fBlurPowerMax;
			float m_fBlurPowerMin;

			bool m_bDofEnable;
			float m_fDofScale;
			float m_fDofRange;
			float m_fDofFocus;
			bool m_bDofRGDebug;

			bool m_bBloomEnable;
			int m_iBloomBlurPass;
			float m_fBloomPowerMax;
			float m_fBloomPowerMin;
			float m_fBloomMultMax;
			float m_fBloomMultMin;

			bool m_bGodRayEnable;

			bool m_bShadowEnable;
			int m_iShadowMapSize;

			bool m_bShowPass;
		};
	}
}

#endif