//---------------------------------------------
//                Initial Engine
//             Container node Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODECONTAINER_HEADER_
#define _INODECONTAINER_HEADER_

#include "Initial/Node/INode.h"

namespace Initial
{
	class INodeContainer : public INode
	{
		DECLARE_OBJECT(INodeContainer,INode)
	public:
		INodeContainer(IDevice *device=NULL);

		virtual Core::IString GetBitmapName() { return "NodeContainer.bmp"; }
	protected:
	};
}

#endif