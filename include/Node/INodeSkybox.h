//---------------------------------------------
//                Initial Engine
//               Skybox Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODESKYBOX_HEADER_
#define _INODESKYBOX_HEADER_

#include "3D/INode.h"

namespace Initial
{
	class INodeSkybox : public INode
	{
	public:
		ISkybox(IRenderDriver *Device,ITexture *textures);

		virtual void Render(IRenderDriver *device)=0;
	protected:
	};
}

#endif