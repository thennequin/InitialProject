
#include "Initial/3D/ILight.h"
#include "Initial/Math/IMath.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;

IArray<ILight*> ILight::m_aLights;

ILight::ILight(IColor color, IVector3D position, float radius,IRenderDriver *driver)
{
	m_aLights.Add(this);
	m_cColor=color;
	m_vPosition=position;
	m_fRadius=radius;
	m_bGodRay=false;

	m_pRenderDriver = driver;
	if (m_pRenderDriver)
	{
		//m_pFrameBuffer = new IFrameBuffer(m_pRenderDriver);
		//m_pShadowTexture = m_pRenderDriver->GetTextureManager()->CreateTexture(m_pRenderDriver->GetWidth(),m_pRenderDriver->GetHeight(),ITF_RED);
		//m_pFrameBuffer->AttachTexture(0,m_pShadowTexture);
	}
}

ILight::~ILight()
{
	m_aLights.DeleteItem(this);
}

void ILight::SetPosition(IVector3D pos)
{
	m_vPosition=pos;
}

IVector3D ILight::GetPosition()
{
	return m_vPosition;
}

void ILight::DrawShadow()
{
	//printf("Render Shadow\n");
	if (m_pRenderDriver)
	{
		//IMaterial *pp = IMaterial::LoadMaterial(m_pRenderDriver,"PostProcess/shadowvolume.ipp");
		//m_pRenderDriver->UseMaterial(pp);

		//m_pRenderDriver->_ClearBuffer(IVB_DEPTH);
		//m_pRenderDriver->_ClearBuffer(IVB_COLOR);
		//m_pRenderDriver->Render(false);

		/*m_pRenderDriver->_EnableDepthWrite(false);
		m_pRenderDriver->_EnableColorWrite(false);

		m_pRenderDriver->_EnableExt(IEXT_STENCIL_TEST);
		m_pRenderDriver->_ClearBuffer(IVB_STENCIL);
		m_pRenderDriver->_StencilFunc(IAF_ALWAYS,128, 0xffffffff);

		m_pRenderDriver->_FrontFace(IFFD_CW);
		m_pRenderDriver->_StencilOp(ISO_KEEP,ISO_KEEP,ISO_INCR);
		//RenderShadow
		m_pRenderDriver->Render(NULL);

		m_pRenderDriver->_FrontFace(IFFD_CCW);
		m_pRenderDriver->_StencilOp(ISO_KEEP,ISO_KEEP,ISO_DECR);
		//RenderShadow
		m_pRenderDriver->Render(NULL);

		m_pRenderDriver->_EnableColorWrite(true);
		//Draw Shadow rect
		m_pRenderDriver->_SetProjectionMatrix();
		m_pRenderDriver->_PushMatrix();
		m_pRenderDriver->_SetMatrix(Math::GetOrthoMatrix(0,1,1,0,10,1));			
		m_pRenderDriver->_SetColor(IColor(1,1,1));
			//m_pRenderDriver->_DrawQuad();
		m_pRenderDriver->_PopMatrix();

		m_pRenderDriver->_DisableExt(IEXT_STENCIL_TEST);
		m_pRenderDriver->_EnableDepthWrite(true);*/
	}
}
