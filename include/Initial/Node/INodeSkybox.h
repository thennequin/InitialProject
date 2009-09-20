//---------------------------------------------
//                Initial Engine
//                Skybox  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODESKYBOX_HEADER_
#define _INODESKYBOX_HEADER_

#include "Initial/Node/INode.h"
#include "Initial/Ressource/ITexture.h"

namespace Initial
{
	class INodeSkybox : public INode
	{
		DECLARE_OBJECT(INodeSkybox,INode)
	public:
		INodeSkybox(IDevice *device=NULL);

		void SetTexuture(int pos, Video::ITexture *tex);
		virtual void Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags);
	protected:
		ISmartPtr<Video::IMaterial> m_pMaterial;
		ISmartPtr<Video::ITexture> m_pTexture[6];
		ITriangle m_tFace[6][2];
	};
}

#endif