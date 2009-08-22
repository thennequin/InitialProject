//---------------------------------------------
//                Initial Engine
//               Layer node Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODELAYER_HEADER_
#define _INODELAYER_HEADER_

#include "Initial/Node/INode.h"

namespace Initial
{
	class INodeLayer : public INode
	{
		DECLARE_OBJECT(INodeLayer,INode)
	public:
		INodeLayer(IDevice *device=NULL);

		virtual Core::IString GetBitmapName() { return "NodeLayer.bmp"; }
	protected:
	};
}

#endif