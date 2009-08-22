//---------------------------------------------
//                Initial Engine
//          Node Container source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Node/INodeLayer.h"
#include "Initial/IDevice.h"
#include "Initial/Video/IRenderDriver.h"

using namespace Initial::Core;
using namespace Initial::Video;

namespace Initial
{	
	INodeLayer::INodeLayer(IDevice *device)
		: INode(device)
	{
	}
}
