//---------------------------------------------
//                Initial Engine
//               3DS Loader Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _I3DLoad_HEADER_
#define _I3DLoad_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/3D/IMesh.h"
#include "Initial/Video/IRenderDriver.h"

namespace Initial
{
	class I3DLoad : public IMeshLoader
	{
	public:
		static IMesh* Load(Core::IString filename, Video::IRenderDriver *device);	
	};

}

#endif