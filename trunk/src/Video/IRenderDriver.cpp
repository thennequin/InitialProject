//---------------------------------------------
//                Initial Engine
//          Render Device source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include <math.h>

#include "Initial/IEnum.h"
#include "Initial/ILogger.h"
#include "Initial/IDevice.h"
#include "Initial/IRessourceManager.h"
#include "Initial/ISceneManager.h"
#include "Initial/Math/IMath.h"
#include "Initial/IO/IConfigINI.h"
#include "Initial/Video/IRenderDriver.h"
#include "Initial/Video/OpenGLDrv/IRenderDriverOpenGL.h"

#include "Initial/3D/IFrustum.h"

#include "Initial/Node/INode.h"
#include "Initial/Node/INodeLight.h"

#include "Initial/3D/IMesh.h"
#include "Initial/3D/ILight.h"

using namespace Initial::Core;
using namespace Initial::IO;
using namespace Initial::Math;

namespace Initial
{
	namespace Video
	{

		/*IRenderDriver* IRenderDriver::CreateDevice(VideoDriverType type,int Width,int Height,int BitDepth, bool Fullscreen, IString Title, int AntiaAliasing, bool UseShader)
		{
			switch (type)
			{
			case IVDT_OPENGL:
				IRenderDriver *device = new IRenderDriverOpenGL(Width,Height,BitDepth,Fullscreen,Title,AntiaAliasing,UseShader);
				return device;
			};

			return NULL;
		}*/

		//IRenderDriver::IRenderDriver(int Width,int Height,int BitDepth, bool Fullscreen,IString Title, int AntiaAliasing, bool UseShader)
		IRenderDriver::IRenderDriver(IDevice *device, void *WinId)
		{
			m_pDevice=device;
			m_pCamera=NULL;
			m_bInRender=false;
			m_pCurrentMaterial=NULL;

			//m_pScreenTexture=NULL;
			//m_pDepthTexture=NULL;

			/*if (m_iWidth<640)
				m_iWidth=640;

			if (m_iHeight<480)
				m_iHeight=480;

			if (m_iBitDepth<16)
				m_iBitDepth=16;*/

			IConfigINI engineParameter("engine.ini");
			engineParameter.Load();

			m_bBlurEnable = engineParameter.ReadIntValue("BlurEnable",1,true);
			m_iBlurPass = engineParameter.ReadIntValue("BlurPass",4,true);
			m_fBlurPowerMax = engineParameter.ReadFloatValue("BlurPowerMax",3,true);
			m_fBlurPowerMin = engineParameter.ReadFloatValue("BlurPowerMin",1,true);

			m_bDofEnable = engineParameter.ReadIntValue("DofEnable",1,true);
			m_fDofScale = engineParameter.ReadFloatValue("DofScale",100,true);
			m_fDofRange = engineParameter.ReadFloatValue("DofRange",15,true);
			m_fDofFocus = engineParameter.ReadFloatValue("DofFocus",85,true);
			m_bDofRGDebug = engineParameter.ReadIntValue("DofRGDebug",0,true);

			m_bBloomEnable = engineParameter.ReadIntValue("BloomEnable",1,true);
			m_iBloomBlurPass = engineParameter.ReadIntValue("BloomBlurPass",7,true);
			m_fBloomPowerMax = engineParameter.ReadFloatValue("BloomPowerMax",5,true);
			m_fBloomPowerMin = engineParameter.ReadFloatValue("BloomPowerMin",1,true);
			m_fBloomMultMax = engineParameter.ReadFloatValue("BloomMultMax",2,true);
			m_fBloomMultMin = engineParameter.ReadFloatValue("BloomMultMin",1,true);

			m_bGodRayEnable = engineParameter.ReadIntValue("GodRayEnable",1,true);

			m_bShowPass = engineParameter.ReadIntValue("ShowPass",0,true);

			m_fRenderScale = engineParameter.ReadFloatValue("RenderScale",1.0,true);

			m_iShadowMapSize = engineParameter.ReadIntValue("ShadowMapSize",512,true);

			if (engineParameter.NeedToBeSave())
				engineParameter.Save();

			m_bLightEnable=true;
			m_iOutputTexture=0;
		}

		void IRenderDriver::_Inited()
		{
			if (m_pTextureManager)
			{
				m_pFrameBuffer = new IFrameBuffer(this);
					m_pFrameDepthTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_DEPTH24);
					m_pFrameDiffuseTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
					m_pFrameEmissiveTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
					m_pFrameNormalTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
					m_pFrameSpecularTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
				m_pFrameBuffer->AttachDepthTexture(m_pFrameDepthTexture);
				m_pFrameBuffer->AttachTexture(0,m_pFrameDiffuseTexture);
				m_pFrameBuffer->AttachTexture(1,m_pFrameEmissiveTexture);
				m_pFrameBuffer->AttachTexture(2,m_pFrameNormalTexture);
				m_pFrameBuffer->AttachTexture(3,m_pFrameSpecularTexture);

				/*m_pFinalFrameBuffer = new IFrameBuffer(this);
					m_pFinalFrameTexture = m_pTextureManager->CreateTexture(m_iWidth,m_iHeight);
				m_pFinalFrameBuffer->AttachTexture(0,m_pFinalFrameTexture);*/

				m_pDeferredLightBuffer = new IFrameBuffer(this);
					m_pDeferredLightTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
				m_pDeferredLightBuffer->AttachTexture(0,m_pDeferredLightTexture);

				m_pBloomBuffer = new IFrameBuffer(this);
					m_pBloomTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
				m_pBloomBuffer->AttachTexture(0,m_pBloomTexture);

				m_pDofBuffer = new IFrameBuffer(this);
					m_pDofTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
				m_pDofBuffer->AttachTexture(0,m_pDofTexture);

				m_pBlurBuffer = new IFrameBuffer(this);
					m_pBlurTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
				m_pBlurBuffer->AttachTexture(0,m_pBlurTexture);

				m_pTempBuffer = new IFrameBuffer(this);
					m_pTempTexture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGBA);
				m_pTempBuffer->AttachTexture(0,m_pTempTexture);

				m_pTemp2Buffer = new IFrameBuffer(this);
					m_pTemp2Texture = m_pTextureManager->CreateTexture(m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGBA);
				m_pTemp2Buffer->AttachTexture(0,m_pTemp2Texture);

				m_pFrameHalfBuffer = new IFrameBuffer(this);
					m_pFrameHalfTexture = m_pTextureManager->CreateTexture(m_iWidth/2.0,m_iHeight/2.0,ITF_RGB);
				m_pFrameHalfBuffer->AttachTexture(0,m_pFrameHalfTexture);

				m_pFrameQuartBuffer = new IFrameBuffer(this);
					m_pFrameQuartTexture = m_pTextureManager->CreateTexture(m_iWidth/4.0,m_iHeight/4.0,ITF_RGB);
				m_pFrameQuartBuffer->AttachTexture(0,m_pFrameQuartTexture);

				m_pFrameQuart2Buffer = new IFrameBuffer(this);
					m_pFrameQuart2Texture = m_pTextureManager->CreateTexture(m_iWidth/4.0,m_iHeight/4.0,ITF_RGB);
				m_pFrameQuart2Buffer->AttachTexture(0,m_pFrameQuart2Texture);

				m_pShadowMapBuffer = new IFrameBuffer(this);
					m_pShadowMapDepthTexture = m_pTextureManager->CreateTexture(m_iShadowMapSize,m_iShadowMapSize,ITF_DEPTH24);
					m_pShadowMapColorTexture = m_pTextureManager->CreateTexture(m_iShadowMapSize,m_iShadowMapSize,ITF_RGB);
				m_pShadowMapBuffer->AttachDepthTexture(m_pShadowMapDepthTexture);
				m_pShadowMapBuffer->AttachTexture(0,m_pShadowMapColorTexture);
			}
		}

		void IRenderDriver::Resize(int Width, int Height)
		{
			//ILogger::LogDebug("Resize %d %d\n",Width,Height);
			m_iWidth=Width;
			m_iHeight=Height;

			_ResizeDriver();

			m_pTextureManager->SetTextureData(m_pFrameDepthTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_DEPTH24,IIF_GREY);
			m_pTextureManager->SetTextureData(m_pFrameDiffuseTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
			m_pTextureManager->SetTextureData(m_pFrameEmissiveTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
			m_pTextureManager->SetTextureData(m_pFrameNormalTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);
			m_pTextureManager->SetTextureData(m_pFrameSpecularTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);

			m_pTextureManager->SetTextureData(m_pDeferredLightTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB16F);

			m_pTextureManager->SetTextureData(m_pBloomTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pDofTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pBlurTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pTempTexture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pTemp2Texture,NULL,m_iWidth*m_fRenderScale,m_iHeight*m_fRenderScale,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pFrameHalfTexture,NULL,m_iWidth/2.0,m_iHeight/2.0,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pFrameQuartTexture,NULL,m_iWidth/4.0,m_iHeight/4.0,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pFrameQuart2Texture,NULL,m_iWidth/4.0,m_iHeight/4.0,ITF_RGB);
		}

		void IRenderDriver::ResizeTexturesPP(int Width, int Height)
		{
			m_pTextureManager->SetTextureData(m_pFrameDepthTexture,NULL,Width,Height,ITF_DEPTH24,IIF_GREY);
			m_pTextureManager->SetTextureData(m_pFrameDiffuseTexture,NULL,Width,Height,ITF_RGB16F);
			m_pTextureManager->SetTextureData(m_pFrameEmissiveTexture,NULL,Width,Height,ITF_RGB16F);
			m_pTextureManager->SetTextureData(m_pFrameNormalTexture,NULL,Width,Height,ITF_RGB16F);
			m_pTextureManager->SetTextureData(m_pFrameSpecularTexture,NULL,Width,Height,ITF_RGB16F);

			m_pTextureManager->SetTextureData(m_pDeferredLightTexture,NULL,Width,Height,ITF_RGB16F);

			m_pTextureManager->SetTextureData(m_pBloomTexture,NULL,Width,Height,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pDofTexture,NULL,Width,Height,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pBlurTexture,NULL,Width,Height,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pTempTexture,NULL,Width,Height,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pTemp2Texture,NULL,Width,Height,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pFrameHalfTexture,NULL,Width/2.0,Height/2.0,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pFrameQuartTexture,NULL,Width/4.0,Height/4.0,ITF_RGB);
			m_pTextureManager->SetTextureData(m_pFrameQuart2Texture,NULL,Width/4.0,Height/4.0,ITF_RGB);
		}
		
		void IRenderDriver::Render(INode *node, IFrustum* frustum, int flags)
		{
			_SetColor(IColor());
			for (int i=0;i<IMesh::m_aObjects.Count();i++)
			{
				if (IMesh::m_aObjects[i] &&
					IMesh::m_aObjects[i]->GetFlag(IRF_ALWAYS_RENDER) &&
					IMesh::m_aObjects[i]->GetParent()==NULL)
					IMesh::m_aObjects[i]->Render(this,flags);
			}

			if (node)
				node->Render(this,frustum);
			else if (m_pDevice && m_pDevice->GetSceneManager())
					m_pDevice->GetSceneManager()->Render(this,frustum,flags);

			/*for (int i=0;i<INode::m_aObjects.Count();i++)
			{
				if (IMesh::m_aObjects[i] &&
					IMesh::m_aObjects[i]->GetFlag(IRF_ALWAYS_RENDER) &&
					IMesh::m_aObjects[i]->GetParent()==NULL)
					IMesh::m_aObjects[i]->Render();
			}*/
			
			
			//Render shadows
			/*if (shadow)
			{
				for (unsigned int j=0;j<ILight::m_aLights.Count();j++)
				{
					ILight::m_aLights[j]->DrawShadow();
				}
			}*/
		}

		ITextureManager* IRenderDriver::GetTextureManager()
		{
			return m_pTextureManager;
		}

		IShaderManager* IRenderDriver::GetShaderManager()
		{
			return m_pShaderManager;
		}

		IVBOManager* IRenderDriver::GetVBOManager()
		{
			return m_pVBOManager;
		}

		void IRenderDriver::SetCamera(ICamera *camera)
		{
			m_pCamera=camera;
			if (camera && m_bInRender)
			{
				camera->SetRatio((float)m_iWidth/m_iHeight);
				camera->UpdateView(this);
			}
		}

		unsigned int IRenderDriver::PolygonDrawsCount()
		{
			return m_iPolygonCount;
		}

		unsigned int IRenderDriver::LineDrawsCount()
		{
			return m_iLineCount;
		}

		void IRenderDriver::SetLightEnable(bool enable)
		{
			m_bLightEnable=enable;
		}

		bool IRenderDriver::GetLightEnable()
		{
			return m_bLightEnable;
		}

		void IRenderDriver::SetOutputTexture(int tex) // 0 = Light, 1 = albedo, 2 = normal, 3 = diffuse, 4 = spec
		{
			m_iOutputTexture=tex;
		}

		void IRenderDriver::UseMaterial(IMaterial *mat)
		{
			if (m_pCurrentMaterial!=mat)
			{		
				if (mat)
				{
					mat->Lock();
					if (mat->GetParent());
						UseMaterial(mat->GetParent());
					
					if (mat->GetParent()==NULL || mat->GetParent()->GetShader()==NULL)
					{
						IShader *shader = mat->GetShader();
						if (m_pShaderManager)
						{
							m_pShaderManager->UseShader(shader);
							if (shader)
							{
								shader->SetParameterFloat("Time",GetTickCount()/1000.0);
							}
						}
					}
					
					// Bind Texture
					for (int i=0;i<8;i++)
						if (/*mat->TextureIsDefine(i) ||*/ mat->GetParent()==NULL || mat->GetTexture(i))
							m_pTextureManager->BindTexture(mat->GetTexture(i),i);
					
					if (mat->IsTranslucent())
					{
						_EnableExt(IEXT_BLEND);
						_EnableDepthWrite(false);
						//_DisableExt(IEXT_DEPTH);
						//_DisableExt(IEXT_COLOR);
					}else{
						_DisableExt(IEXT_BLEND);
						//_EnableExt(IEXT_DEPTH);
						_EnableDepthWrite(true);
						//_EnableExt(IEXT_COLOR);
					}
					mat->Unlock();
				}else{
					_DisableExt(IEXT_BLEND);
					if (m_pShaderManager)
						m_pShaderManager->UseShader(NULL);
					for (int i=0;i<8;i++)
						m_pTextureManager->BindTexture(NULL,i);
				}
				m_pCurrentMaterial=mat;
			}
		}

		void IRenderDriver::RenderCamera(ICamera *camera, INode *node, IFrameBuffer *dest, IRenderParams params)
		{
			float ratio;
			int w,h;
			if (dest)
			{
				w=dest->GetTexture(0)->GetWidth();
				h=dest->GetTexture(0)->GetHeight();
			}else{
				w=m_iWidth;
				h=m_iHeight;				
			}
			ratio = (float)w/h;

			IMatrix cameraMatrix(4,4);
			if (camera)
			{
				camera->SetRatio(ratio);
				cameraMatrix = camera->GetUpdateViewMatrix();
			}else
				cameraMatrix.MakeIdentity(4);

			IFrustum frustum;
			frustum.SetFrustumFromMatrix(cameraMatrix);

			_SetProjectionMatrix();
			_SetMatrix(cameraMatrix);

			//Resize Render Texture

			IMatrix OrthoMat = GetOrthoMatrix(1,0,1,0,10,1);
			IMaterial *pp=NULL;

			//Render to FrameBuffer
			BeginRender();
				Render(node,&frustum);
			EndRender();

			_DisableExt(IEXT_DEPTH_TEST);
			_EnableDepthWrite(false);

			//if (params.EnableLights)
			ITexture* InputTex = m_pFrameDiffuseTexture;
			//if (m_iOutputTexture==0)
			{
				_Lighting(cameraMatrix,frustum);
				InputTex = m_pDeferredLightTexture;
			}

			//Post process
			InputTex = _Bloom(cameraMatrix,InputTex);

			_EnableDepthWrite(true);
			_EnableExt(IEXT_DEPTH_TEST);

			BeginRender(NULL,true);
				_SetProjectionMatrix();
				_SetMatrix(OrthoMat);
				pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/copy.ipp");
				UseMaterial(NULL);
				if (pp)
					pp->GetShader()->SetParameterFloat("Mult",1);

				if (m_iOutputTexture==0)
					m_pTextureManager->BindTexture(InputTex,0);
				else if (m_iOutputTexture==1)
					m_pTextureManager->BindTexture(m_pFrameDiffuseTexture,0);
				else if (m_iOutputTexture==2)
					m_pTextureManager->BindTexture(m_pFrameNormalTexture,0);
				else if (m_iOutputTexture==3)
					m_pTextureManager->BindTexture(m_pFrameEmissiveTexture,0);
				else if (m_iOutputTexture==4)
					m_pTextureManager->BindTexture(m_pFrameSpecularTexture,0);
				else if (m_iOutputTexture==5)
					m_pTextureManager->BindTexture(m_pFrameDepthTexture,0);
				else if (m_iOutputTexture==6)
					m_pTextureManager->BindTexture(m_pFrameQuart2Texture,0);

				_DrawQuadPP();
			EndRender();
		}

		void IRenderDriver::_Lighting(Math::IMatrix& cameraMatrix, IFrustum &frustum)
		{
			IMatrix halfMatrix(4,4);
			halfMatrix.MakeIdentity();
			halfMatrix[0][0]=0.5;
			halfMatrix[1][1]=0.5;
			halfMatrix[2][2]=0.5;
			
			halfMatrix[3][0]=0.5;
			halfMatrix[3][1]=0.5;
			halfMatrix[3][2]=0.5;

			IMatrix OrthoMat = GetOrthoMatrix(1,0,1,0,10,0.1);
			IMaterial *pp=NULL;
			IMatrix invProj;
			cameraMatrix.Inv(invProj);

			_SetProjectionMatrix();
			_SetMatrix(OrthoMat);
			_SetModelViewMatrix();
			_LoadIdentity();
			//_BlendMode(IBM_ONE,IBM_ONE);
			//_EnableExt(IEXT_BLEND);
		
			pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/defered-light.ipp");
			UseMaterial(pp);
			if (pp)
			{
				pp->GetShader()->SetParameterMatrix44("InvProjection",invProj);

				IArray<INode*> lights = m_pDevice->GetSceneManager()->GetNodeByClass(INodeLight::GetStaticClass(),true);

				_SetClearColor(IColor(0,0,0,0));
				BeginRender(m_pDeferredLightBuffer,false,true);
				EndRender();
				_SetClearColor(IColor(1,1,1,1));
				for (unsigned int j=0;j<lights.Count();j++)
				{
					INodeLight *light = (INodeLight*)lights[j];
					if (light)
					{			
						//Light affect frustum
						if (frustum.SphereInFrustum(light->GetPosition(),light->GetRadius()))
						{
							//Calculate light screen bounding box
							IBBox screenBox(light->GetPosition(),IVector3D(light->GetRadius(),light->GetRadius(),light->GetRadius()));
							screenBox = BBoxToScreenBBox(screenBox,cameraMatrix);

							IVector3D min = screenBox.GetCenter()+screenBox.GetDimension()*IVector3D(-1,-1,1);
							IVector3D max = screenBox.GetCenter()+screenBox.GetDimension()*IVector3D(1,1,1);
							//ILogger::LogDebug("BBox %f %f %f %f\n",min.x,min.y,max.x,max.y);
							//m_iShadowMapSize
							
							for (int l=0;l<6;l++)
							{
								//Render shadow map 
								IMatrix lightView = light->GetProjectionMatrix(l);//GetPerspectiveMatrix(90,1,0.01,light->GetRadius());
								IFrustum frustum;
								frustum.SetFrustumFromMatrix(lightView);
								UseMaterial(NULL);
								_SetColor(IColor(1,1,1,1));
								_EnableDepthWrite(true);
								_EnableColorWrite(false);
								_EnableDepthTest(true);
								BeginRender(m_pShadowMapBuffer);
									_SetProjectionMatrix();
									_PushMatrix();								
										_SetMatrix(lightView);
										_SetModelViewMatrix();
										_LoadIdentity();
											Render(NULL,&frustum,IRF_SHADOW_MAP_RENDER);
										_SetProjectionMatrix();
									_PopMatrix();
								EndRender();

								UseMaterial(pp);
								pp->GetShader()->SetParameter3Float("LightPos",light->GetPosition());
								pp->GetShader()->SetParameter3Float("LightColor",light->GetColor());
								pp->GetShader()->SetParameterFloat("LightRadius",light->GetRadius());
								pp->GetShader()->SetParameterMatrix44("HalfLightProjection",halfMatrix*lightView);
								pp->GetShader()->SetParameterMatrix44("LightProjection",lightView);

								m_pTextureManager->BindTexture(m_pFrameNormalTexture,0);
								m_pTextureManager->BindTexture(m_pFrameSpecularTexture,1);
								m_pTextureManager->BindTexture(m_pFrameDepthTexture,2);
								m_pTextureManager->BindTexture(m_pDeferredLightTexture,3);
								m_pTextureManager->BindTexture(m_pShadowMapDepthTexture,4);
								m_pTextureManager->BindTexture(m_pShadowMapColorTexture,5);
								m_pTextureManager->BindTexture(light->GetProjectedTexture(),6);

								_EnableDepthWrite(false);
								_EnableColorWrite(true);
								_EnableDepthTest(false);
								BeginRender(m_pDeferredLightBuffer,false,false);
									_SetProjectionMatrix();
									_SetMatrix(OrthoMat);
									_DrawQuadPP(min.x,min.y,max.x-min.x,max.y-min.y);
								EndRender();
							}
						}
					}
				}
			}

			//_DisableExt(IEXT_BLEND);
					

			//Mix lights and diffuse
			BeginRender(m_pDeferredLightBuffer,false,false);
				//_SetProjectionMatrix();
				//_SetMatrix(OrthoMat);
				pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/defered-light2.ipp");
				UseMaterial(pp);

				m_pTextureManager->BindTexture(m_pDeferredLightTexture,0);

				m_pTextureManager->BindTexture(m_pFrameDiffuseTexture,1);
				m_pTextureManager->BindTexture(m_pFrameEmissiveTexture,2);
				
				_DrawQuadPP();
			EndRender();
		}

		ITexture* IRenderDriver::_Bloom(Math::IMatrix& mat, ITexture *InputTex)
		{
			IMatrix OrthoMat = GetOrthoMatrix(1,0,1,0,10,1);
			IMaterial *pp=NULL;

			//Bloom Pass 1
			BeginRender(m_pBloomBuffer);
				_SetProjectionMatrix();
				_SetMatrix(OrthoMat);
				pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/bloom_pass1.ipp");
				UseMaterial(pp);
				if (pp)
				{
					//pp->GetShader()->SetParameterInt("ScreenWidth",m_iWidth);
					//pp->GetShader()->SetParameterInt("ScreenHeight",m_iHeight);
				}
				m_pTextureManager->BindTexture(InputTex,0);		
				
				_DrawQuadPP();
			EndRender();

			BeginRender(m_pFrameHalfBuffer);
				_SetProjectionMatrix();
				_SetMatrix(OrthoMat);
				pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/copy.ipp");
				UseMaterial(pp);
				if (pp)
				{
					pp->GetShader()->SetParameterFloat("Mult",1.0);
				}
				m_pTextureManager->BindTexture(m_pBloomTexture,0);		
				
				_DrawQuadPP();
			EndRender();

			//Bloom blur
			for (int i=0;i<m_iBloomBlurPass;i++)
			{
				BeginRender(m_pFrameQuartBuffer,false,true);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/blurV.ipp");
					UseMaterial(pp); // Blur pass 1
					if (pp)
					{				
						pp->GetShader()->SetParameterInt("ScreenHeight",m_iHeight/4.0);
						pp->GetShader()->SetParameterFloat("Power",i>=m_iBloomBlurPass/2 ? m_fBloomPowerMin : m_fBloomPowerMax);
						pp->GetShader()->SetParameterFloat("Mult",i>=m_iBloomBlurPass/2 ? m_fBloomMultMin : m_fBloomMultMax);
					}
					if (i!=0)
						m_pTextureManager->BindTexture(m_pFrameQuart2Texture,0);
					else
						m_pTextureManager->BindTexture(m_pFrameHalfTexture,0);
					
					_DrawQuadPP();
				EndRender();

				BeginRender(m_pFrameQuart2Buffer,false,true);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/blurH.ipp");
					UseMaterial(pp); // Blur pass 1
					if (pp)
					{
						pp->GetShader()->SetParameterInt("ScreenWidth",m_iWidth/4.0);
						pp->GetShader()->SetParameterFloat("Power",i>=m_iBloomBlurPass/2 ? m_fBloomPowerMin : m_fBloomPowerMax);
						pp->GetShader()->SetParameterFloat("Mult",i>=m_iBloomBlurPass/2 ? m_fBloomMultMin : m_fBloomMultMax);
					}
					m_pTextureManager->BindTexture(m_pFrameQuartTexture,0);
					
					_DrawQuadPP();
				EndRender();
			}

			// Bloom Pass 3
			BeginRender(m_pBloomBuffer,false,false);
			//BeginRender(NULL,true);
				_SetProjectionMatrix();
				_SetMatrix(OrthoMat);
				pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/bloom_pass2.ipp");
				UseMaterial(pp); // Final render
				m_pTextureManager->BindTexture(InputTex,0);
				m_pTextureManager->BindTexture(m_pFrameQuart2Texture,1);	
				
				_DrawQuadPP();
			EndRender();

			return m_pBloomTexture;
		}

		void IRenderDriver::_PostProcess(Math::IMatrix& mat, IFrameBuffer *dest)
		{

		}

		void IRenderDriver::PostProcess(Math::IMatrix& Proj)
		{
			//return;
			IMatrix invProj;
			Proj.Inv(invProj);

			SetCamera(NULL);

			unsigned int Time = GetTickCount();
			SetCamera(NULL);
			IMatrix OrthoMat = GetOrthoMatrix(1,0,1,0,10,1);
			// Screen Quad
			ITriangle tri[2];
			tri[0].SetVertex(0,IVector3D(0,1,1));
			tri[0].SetVertex(1,IVector3D(0,0,1));
			tri[0].SetVertex(2,IVector3D(1,0,1));
			tri[0].SetCoord(0,IVector2D(1,1));
			tri[0].SetCoord(1,IVector2D(1,0));
			tri[0].SetCoord(2,IVector2D(0,0));
			tri[1].SetVertex(0,IVector3D(0,1,1));
			tri[1].SetVertex(1,IVector3D(1,0,1));
			tri[1].SetVertex(2,IVector3D(1,1,1));
			tri[1].SetCoord(0,IVector2D(1,1));
			tri[1].SetCoord(1,IVector2D(0,0));
			tri[1].SetCoord(2,IVector2D(0,1));

			ITriangle triBR[2];
			triBR[0].SetVertex(0,IVector3D(0,0.25,1));
			triBR[0].SetVertex(1,IVector3D(0,0,1));
			triBR[0].SetVertex(2,IVector3D(0.25,0,1));
			triBR[0].SetCoord(0,IVector2D(1,1));
			triBR[0].SetCoord(1,IVector2D(1,0));
			triBR[0].SetCoord(2,IVector2D(0,0));
			triBR[1].SetVertex(0,IVector3D(0,0.25,1));
			triBR[1].SetVertex(1,IVector3D(0.25,0,1));
			triBR[1].SetVertex(2,IVector3D(0.25,0.25,1));
			triBR[1].SetCoord(0,IVector2D(1,1));
			triBR[1].SetCoord(1,IVector2D(0,0));
			triBR[1].SetCoord(2,IVector2D(0,1));

			ITriangle triBL[2];
			triBL[0].SetVertex(0,IVector3D(0.75,0.25,1));
			triBL[0].SetVertex(1,IVector3D(0.75,0,1));
			triBL[0].SetVertex(2,IVector3D(1,0,1));
			triBL[0].SetCoord(0,IVector2D(1,1));
			triBL[0].SetCoord(1,IVector2D(1,0));
			triBL[0].SetCoord(2,IVector2D(0,0));
			triBL[1].SetVertex(0,IVector3D(0.75,0.25,1));
			triBL[1].SetVertex(1,IVector3D(1,0,1));
			triBL[1].SetVertex(2,IVector3D(1,0.25,1));
			triBL[1].SetCoord(0,IVector2D(1,1));
			triBL[1].SetCoord(1,IVector2D(0,0));
			triBL[1].SetCoord(2,IVector2D(0,1));

			ITriangle triTL[2];
			triTL[0].SetVertex(0,IVector3D(0.75,1,1));
			triTL[0].SetVertex(1,IVector3D(0.75,0.75,1));
			triTL[0].SetVertex(2,IVector3D(1,0.75,1));
			triTL[0].SetCoord(0,IVector2D(1,1));
			triTL[0].SetCoord(1,IVector2D(1,0));
			triTL[0].SetCoord(2,IVector2D(0,0));
			triTL[1].SetVertex(0,IVector3D(0.75,1,1));
			triTL[1].SetVertex(1,IVector3D(1,0.75,1));
			triTL[1].SetVertex(2,IVector3D(1,1,1));
			triTL[1].SetCoord(0,IVector2D(1,1));
			triTL[1].SetCoord(1,IVector2D(0,0));
			triTL[1].SetCoord(2,IVector2D(0,1));

			ITriangle triTR[2];
			triTR[0].SetVertex(0,IVector3D(0,1,1));
			triTR[0].SetVertex(1,IVector3D(0,0.75,1));
			triTR[0].SetVertex(2,IVector3D(0.25,0.75,1));
			triTR[0].SetCoord(0,IVector2D(1,1));
			triTR[0].SetCoord(1,IVector2D(1,0));
			triTR[0].SetCoord(2,IVector2D(0,0));
			triTR[1].SetVertex(0,IVector3D(0,1,1));
			triTR[1].SetVertex(1,IVector3D(0.25,0.75,1));
			triTR[1].SetVertex(2,IVector3D(0.25,1,1));
			triTR[1].SetCoord(0,IVector2D(1,1));
			triTR[1].SetCoord(1,IVector2D(0,0));
			triTR[1].SetCoord(2,IVector2D(0,1));

			IMaterial *pp=NULL;
			_SetColor(IColor(1,1,1,1));

			//--------------------
			//Light
			//--------------------
			long Time2=GetTickCount();
			if (m_bLightEnable)
			{
				BeginRender(m_pDeferredLightBuffer,false,true);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					_EnableDepthWrite(false);
					_DisableExt(IEXT_DEPTH_TEST);
					//_BlendMode(IBM_ONE,IBM_ONE);
					//_EnableExt(IEXT_BLEND);
				
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/defered-light.ipp");
					UseMaterial(pp);
					if (pp)
					{
						pp->GetShader()->SetParameterMatrix44("InvProjection",invProj);
						
						//m_pTextureManager->BindTexture(m_pFrameDiffuseTexture,0);
						m_pTextureManager->BindTexture(m_pFrameNormalTexture,0);
						m_pTextureManager->BindTexture(m_pFrameSpecularTexture,1);
						m_pTextureManager->BindTexture(m_pFrameDepthTexture,2);
						m_pTextureManager->BindTexture(m_pDeferredLightTexture,3);

						//Create light info texture
						//m_pTextureManager->SetTextureData(LightInfo,lightData,ILight::m_aLights.Count()+1,1,IF_RGBA32F,IIF_RGB);
						
						IArray<INode*> lights = m_pDevice->GetSceneManager()->GetNodeByClass(INodeLight::GetStaticClass(),true);

						for (unsigned int j=0;j<lights.Count();j++)
						{
							INodeLight *light = (INodeLight*)lights[j];
							if (light)
							{
								//m_iShadowMapSize
								pp->GetShader()->SetParameter3Float("LightPos",light->GetPosition());
								pp->GetShader()->SetParameter3Float("LightColor",light->GetColor());
								pp->GetShader()->SetParameterFloat("LightRadius",light->GetRadius());

								_DrawQuadPP();	
							}
						}

						/*for (unsigned int j=0;j<ILight::m_aLights.Count();j++)
						{		
							//Render();
							pp->GetShader()->SetParameter3Float("LightPos",ILight::m_aLights[j]->m_vPosition);
							pp->GetShader()->SetParameter3Float("LightColor",ILight::m_aLights[j]->m_cColor);
							pp->GetShader()->SetParameterFloat("LightRadius",ILight::m_aLights[j]->m_fRadius);

							//Calc affected area.
							//ILight::m_aLights[j]->m_vPosition;

							_DrawQuadPP();			
						}*/
					}

					//_DisableExt(IEXT_BLEND);
				EndRender();				
			}

			//ILogger::LogDebug("Lights time %f\n",(GetTickCount()-Time2)/1000.0);

			BeginRender(m_pDeferredLightBuffer,false,false);
				_SetProjectionMatrix();
				_SetMatrix(OrthoMat);
				pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/defered-light2.ipp");
				UseMaterial(pp);

				if (m_bLightEnable)
					m_pTextureManager->BindTexture(m_pDeferredLightTexture,0);
				else
					m_pTextureManager->BindTexture(m_pFrameDiffuseTexture,0);

				m_pTextureManager->BindTexture(m_pFrameDiffuseTexture,1);
				m_pTextureManager->BindTexture(m_pFrameEmissiveTexture,2);
				
				_DrawQuadPP();
			EndRender();			

			//--------------------
			//Bloom
			//--------------------
			if (m_bBloomEnable)
			{
				//Bloom Pass 1
				BeginRender(m_pBloomBuffer);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/bloom_pass1.ipp");
					UseMaterial(pp);
					if (pp)
					{
						//pp->GetShader()->SetParameterInt("ScreenWidth",m_iWidth);
						//pp->GetShader()->SetParameterInt("ScreenHeight",m_iHeight);
					}
					m_pTextureManager->BindTexture(m_pDeferredLightTexture,0);		
					
					_DrawQuadPP();
				EndRender();

				BeginRender(m_pFrameHalfBuffer);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/copy.ipp");
					UseMaterial(pp);
					if (pp)
					{
						pp->GetShader()->SetParameterFloat("Mult",1.0);
					}
					m_pTextureManager->BindTexture(m_pBloomTexture,0);		
					
					_DrawQuadPP();
				EndRender();

				//Bloom blur
				for (int i=0;i<m_iBloomBlurPass;i++)
				{
					BeginRender(m_pFrameQuartBuffer,false,true);
						_SetProjectionMatrix();
						_SetMatrix(OrthoMat);
						pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/blurV.ipp");
						UseMaterial(pp); // Blur pass 1
						if (pp)
						{				
							pp->GetShader()->SetParameterInt("ScreenHeight",m_iHeight/4.0);
							pp->GetShader()->SetParameterFloat("Power",i>=m_iBloomBlurPass/2 ? m_fBloomPowerMin : m_fBloomPowerMax);
							pp->GetShader()->SetParameterFloat("Mult",i>=m_iBloomBlurPass/2 ? m_fBloomMultMin : m_fBloomMultMax);
						}
						if (i!=0)
							m_pTextureManager->BindTexture(m_pFrameQuart2Texture,0);
						else
							m_pTextureManager->BindTexture(m_pFrameHalfTexture,0);
						
						_DrawQuadPP();
					EndRender();

					BeginRender(m_pFrameQuart2Buffer,false,true);
						_SetProjectionMatrix();
						_SetMatrix(OrthoMat);
						pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/blurH.ipp");
						UseMaterial(pp); // Blur pass 1
						if (pp)
						{
							pp->GetShader()->SetParameterInt("ScreenWidth",m_iWidth/4.0);
							pp->GetShader()->SetParameterFloat("Power",i>=m_iBloomBlurPass/2 ? m_fBloomPowerMin : m_fBloomPowerMax);
							pp->GetShader()->SetParameterFloat("Mult",i>=m_iBloomBlurPass/2 ? m_fBloomMultMin : m_fBloomMultMax);
						}
						m_pTextureManager->BindTexture(m_pFrameQuartTexture,0);
						
						_DrawQuadPP();
					EndRender();
				}

				// Bloom Pass 3
				BeginRender(m_pBloomBuffer,false,false);
				//BeginRender(NULL,true);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/bloom_pass2.ipp");
					UseMaterial(pp); // Final render
					m_pTextureManager->BindTexture(m_pDeferredLightTexture,0);
					m_pTextureManager->BindTexture(m_pFrameQuart2Texture,1);	
					
					_DrawQuadPP();
				EndRender();
			}else{
				BeginRender(m_pBloomBuffer);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/copy.ipp");
					UseMaterial(pp);
					if (pp)
						pp->GetShader()->SetParameterFloat("Mult",1);
					m_pTextureManager->BindTexture(m_pDeferredLightTexture,0);
					
					_DrawQuadPP();
				EndRender();
				if (pp)
					pp->GetShader()->SetParameterFloat("Mult",1);
			}

			//--------------------
			//GOD RAY
			//--------------------
			bool first=false;
			if (m_bGodRayEnable)
			{
				//int j=3;
				for (unsigned int j=0;j<ILight::m_aLights.Count();j++)
				{
					if (ILight::m_aLights[j]->m_bGodRay)
					{
						IVector3D pos = ILight::m_aLights[j]->m_vPosition;
						IMatrix posMat(1,4);
						posMat[0][0]=pos.GetX();
						posMat[0][1]=pos.GetY();
						posMat[0][2]=pos.GetZ();
						posMat[0][3]=1.0;
						posMat = Proj * posMat;
						IVector3D screenPos=IVector3D(posMat[0][0] / posMat[0][3],posMat[0][1] / posMat[0][3],posMat[0][2] / posMat[0][3]);
						/*screenPos.Set(screenPos.GetX()*0.5+0.5,
										screenPos.GetY()*0.5+0.5,
										screenPos.GetZ()*0.5+0.5);*/
						/*printf("%f %f %f\n",screenPos.GetX(),screenPos.GetY(),screenPos.GetZ());*/

						//if (screenPos.GetZ()>0)// Inutile si la lumiére est dérriére la camera
						{
							BeginRender(m_pTempBuffer);
								_SetProjectionMatrix();
								_SetMatrix(OrthoMat);
								pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/godray.ipp");
								UseMaterial(pp);
								if (pp)
								{
									pp->GetShader()->SetParameterMatrix44("Projection",Proj);
									//pp->GetShader()->SetParameter3Float("ScreenLightPos",pos);
									pp->GetShader()->SetParameter3Float("LightPos",pos);
									//pp->GetShader()->SetParameterFloat("LightRadius",ILight::m_aLights[j]->m_fRadius);
								}

								m_pTextureManager->BindTexture(m_pFrameDepthTexture,0);
								
								_DrawQuadPP();
							EndRender();

							for (int i=0;i<3;i++)
							{
								if (i%2==0)
									BeginRender(m_pTemp2Buffer,false,false);	
								else
									BeginRender(m_pTempBuffer,false,false);	

									_SetProjectionMatrix();
									_SetMatrix(OrthoMat);
									pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/godray_radialblur.ipp");
									UseMaterial(pp);
									if (pp)
									{
										pp->GetShader()->SetParameterMatrix44("Projection",Proj);
										pp->GetShader()->SetParameter3Float("LightPos",pos);
										//pp->GetShader()->SetParameter3Float("ScreenLightPos",screenPos);
										pp->GetShader()->SetParameter2Float("Center",0.5*(screenPos.GetX()+1),0.5*(screenPos.GetY()+1));
										pp->GetShader()->SetParameterInt("Samples",8);
										pp->GetShader()->SetParameterFloat("Power",-0.1/(i+1));

										pp->GetShader()->SetParameterInt("Original",0);
									}

									if (i%2==0)
										m_pTextureManager->BindTexture(m_pTempTexture,0);
									else
										m_pTextureManager->BindTexture(m_pTemp2Texture,0);
									
									_DrawQuadPP();
								EndRender();
							}

							BeginRender(m_pBloomBuffer,false,false);	
								_SetProjectionMatrix();
								_SetMatrix(OrthoMat);
								pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/godray2.ipp");
								UseMaterial(pp);
								if (pp)
								{
									pp->GetShader()->SetParameter3Float("LightColor",ILight::m_aLights[j]->m_cColor);
								}

								m_pTextureManager->BindTexture(m_pTemp2Texture,0);
								//m_pTextureManager->BindTexture(m_pFrameDepthTexture,2);
								//if (first==false)
									m_pTextureManager->BindTexture(m_pBloomTexture,1);
								//else
									//m_pTextureManager->BindTexture(m_pTemp2Texture,1);

								_DrawQuadPP();
							EndRender();
							first=true;
						}
					}
				}
			}

			//--------------------
			//Depth of field
			//--------------------
			if (m_bDofEnable)
			{
				/*for (int i=0;i<m_iBlurPass;i++)
				{
					BeginRender(m_pFrameQuartBuffer,false,true);
						_SetProjectionMatrix();
						_SetMatrix(OrthoMat);
						pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/blurV.ipp");
						UseMaterial(pp); // Final render
						if (pp)
						{
							pp->GetShader()->SetParameterInt("ScreenHeight",m_iHeight/4.0);
							pp->GetShader()->SetParameterFloat("Power",i>=m_iBlurPass?m_fBlurPowerMin:m_fBlurPowerMax);
						}
						if (i==0)
							m_pTextureManager->BindTexture(m_pBloomTexture,0);
						else
							m_pTextureManager->BindTexture(m_pFrameQuart2Texture,0);
						
						_DrawQuadPP();
					EndRender();

					BeginRender(m_pFrameQuart2Buffer);
						_SetProjectionMatrix();
						_SetMatrix(OrthoMat);
						pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/blurH.ipp");
						UseMaterial(pp); // Final render
						if (pp)
						{
							pp->GetShader()->SetParameterInt("ScreenWidth",m_iWidth/4.0);
							pp->GetShader()->SetParameterFloat("Power",i>=m_iBlurPass?m_fBlurPowerMin:m_fBlurPowerMax);
						}
						m_pTextureManager->BindTexture(m_pFrameQuartTexture,0);
						
						_DrawQuadPP();
					EndRender();
				}*/
				/*if (KeyDown(VK_NEXT))
					m_fDofFocus+=KeyDown(VK_SHIFT) ? 0.01 : 0.1;
				if (KeyDown(VK_PRIOR))
					m_fDofFocus-=KeyDown(VK_SHIFT) ? 0.01 : 0.1;

				if (KeyDown(VK_END))
					m_fDofRange+=KeyDown(VK_SHIFT) ? 0.01 : 0.1;
				if (KeyDown(VK_HOME))
					m_fDofRange-=KeyDown(VK_SHIFT) ? 0.01 : 0.1;

				if (KeyDown(VK_INSERT))
					m_fDofScale+=KeyDown(VK_SHIFT) ? 0.01 : 0.1;
				if (KeyDown(VK_DELETE))
					m_fDofScale-=KeyDown(VK_SHIFT) ? 0.01 : 0.1;	

				if (KeyPress(VK_RETURN))
					m_bDofRGDebug=m_bDofRGDebug-1;	*/

				//printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%f %f %f",m_fDofFocus,m_fDofRange,m_fDofScale);
				
				/*BeginRender(m_pDofBuffer);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/dof_pass2.ipp");
					UseMaterial(pp); // Final render
					if(pp)
					{
						pp->GetShader()->SetParameterFloat("focus",m_fDofFocus);
						pp->GetShader()->SetParameterFloat("range",m_fDofRange);
						pp->GetShader()->SetParameterFloat("scale",m_fDofScale);
						pp->GetShader()->SetParameterInt("RG",m_bDofRGDebug);
					}
					m_pTextureManager->BindTexture(m_pBloomTexture,0);
					m_pTextureManager->BindTexture(m_pFrameQuart2Texture,1);
					m_pTextureManager->BindTexture(m_pFrameDepthTexture,2);	
					
					_DrawQuadPP();
				EndRender();*/
			}else{
				BeginRender(m_pDofBuffer);
					_SetProjectionMatrix();
					_SetMatrix(OrthoMat);
					pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/copy.ipp");
					UseMaterial(pp);
					if (pp)
						pp->GetShader()->SetParameterFloat("Mult",1);
					m_pTextureManager->BindTexture(m_pBloomTexture,0);
					
					_DrawQuadPP();
				EndRender();
			}

			//--------------------
			//Final render
			//--------------------
			BeginRender(NULL,true);
				_SetProjectionMatrix();
				_SetMatrix(OrthoMat);
				pp = m_pDevice->GetRessourceManager()->LoadMaterial("PostProcess/copy.ipp");
				UseMaterial(NULL);
				if (pp)
					pp->GetShader()->SetParameterFloat("Mult",1);

				// 0 = Light, 1 = Albedo, 2 = Normal, 3 = Diffuse, 4 = Spec, 5 = Depth : 6 = Bloom
				if (m_iOutputTexture==0)
					m_pTextureManager->BindTexture(m_pDofTexture,0);
				else if (m_iOutputTexture==1)
					m_pTextureManager->BindTexture(m_pFrameDiffuseTexture,0);
				else if (m_iOutputTexture==2)
					m_pTextureManager->BindTexture(m_pFrameNormalTexture,0);
				else if (m_iOutputTexture==3)
					m_pTextureManager->BindTexture(m_pFrameEmissiveTexture,0);
				else if (m_iOutputTexture==4)
					m_pTextureManager->BindTexture(m_pFrameSpecularTexture,0);
				else if (m_iOutputTexture==5)
					m_pTextureManager->BindTexture(m_pFrameDepthTexture,0);
				else if (m_iOutputTexture==6)
					m_pTextureManager->BindTexture(m_pFrameQuart2Texture,0);
				
				_DrawQuadPP();

				if (m_bShowPass)
				{
					m_pTextureManager->BindTexture(m_pFrameDepthTexture,0);	
					
					_StartTriangleDraw(false);			
						_DrawTriangle(&triBR[0]);		
						_DrawTriangle(&triBR[1]);
					_EndPolyDraw();

					m_pTextureManager->BindTexture(m_pFrameDiffuseTexture,0);

					_StartTriangleDraw(false);			
						_DrawTriangle(&triBL[0]);		
						_DrawTriangle(&triBL[1]);
					_EndPolyDraw();

					//m_pTextureManager->BindTexture(m_pBlurTexture,0);
					m_pTextureManager->BindTexture(m_pFrameNormalTexture,0);

					_StartTriangleDraw(false);			
						_DrawTriangle(&triTL[0]);		
						_DrawTriangle(&triTL[1]);
					_EndPolyDraw();		

					m_pTextureManager->BindTexture(m_pFrameSpecularTexture,0);
					//m_pTextureManager->BindTexture(m_pFrameEmissiveTexture,0);

					_StartTriangleDraw(false);			
						_DrawTriangle(&triTR[0]);		
						_DrawTriangle(&triTR[1]);
					_EndPolyDraw();
				}
			EndRender();

			//printf("PP time %f\n",(GetTickCount()-Time)/1000.0);
			_EnableExt(IEXT_DEPTH_TEST);
		}
	}
}
