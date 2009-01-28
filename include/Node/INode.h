//---------------------------------------------
//                Initial Engine
//                 Node  Module
//
//            By Thibault HENNEQUIN
//              January 18th 2008
//---------------------------------------------

#ifndef _INODE_HEADER_
#define _INODE_HEADER_

#include "Video/IRenderDriver.h"

//using namespace Initial::Math;

namespace Initial
{

	class INode
	{
	public:
		virtual bool IsVisible()=0;
		virtual void Render()=0;

		void SetRenderDevice(IRenderDriver *device);
		IRenderDriver *GetRenderDevice();
	protected:
		IRenderDriver *m_pDevice;

		IArray<INode*>	m_aChildren;
	};

}

#endif