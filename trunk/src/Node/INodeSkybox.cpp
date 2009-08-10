//---------------------------------------------
//                Initial Engine
//          Node skybox source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Node/INodeSkybox.h"
#include "Initial/IDevice.h"
#include "Initial/Video/IRenderDriver.h"

using namespace Initial::Core;
using namespace Initial::Video;

namespace Initial
{	
	INodeSkybox::INodeSkybox(IDevice *device)
		: INode(device)
	{
		m_pMaterial = m_pDevice->GetRessourceManager()->LoadMaterial("skybox.ima");
		//m_tFace
	}	

	void INodeSkybox::Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags=0)
	{
		driver->_EnableDepthWrite(false);

		driver->UseMaterial(m_pMaterial);
		driver->GetTextureManager()->BindTexture(m_pTexture[0],0);
			driver->_StartTriangleDraw(false);
			driver->_DrawTriangle(&m_tFace[0][0]);
			driver->_DrawTriangle(&m_tFace[0][1]);
			driver->_EndPolyDraw();
		driver->GetTextureManager()->BindTexture(m_pTexture[1],0);
			driver->_StartTriangleDraw(false);
			driver->_DrawTriangle(&m_tFace[1][0]);
			driver->_DrawTriangle(&m_tFace[1][1]);
			driver->_EndPolyDraw();
		driver->GetTextureManager()->BindTexture(m_pTexture[2],0);
			driver->_StartTriangleDraw(false);
			driver->_DrawTriangle(&m_tFace[2][0]);
			driver->_DrawTriangle(&m_tFace[2][1]);
			driver->_EndPolyDraw();
		driver->GetTextureManager()->BindTexture(m_pTexture[3],0);
			driver->_StartTriangleDraw(false);
			driver->_DrawTriangle(&m_tFace[3][0]);
			driver->_DrawTriangle(&m_tFace[3][1]);
			driver->_EndPolyDraw();
		driver->GetTextureManager()->BindTexture(m_pTexture[4],0);
			driver->_StartTriangleDraw(false);
			driver->_DrawTriangle(&m_tFace[4][0]);
			driver->_DrawTriangle(&m_tFace[4][1]);
			driver->_EndPolyDraw();
		driver->GetTextureManager()->BindTexture(m_pTexture[5],0);
			driver->_StartTriangleDraw(false);
			driver->_DrawTriangle(&m_tFace[5][0]);
			driver->_DrawTriangle(&m_tFace[5][1]);
			driver->_EndPolyDraw();

		//driver->GetTextureManager()->BindTexture(NULL,0);
		driver->UseMaterial(NULL);

		driver->_EnableDepthWrite(true);
	}
}
