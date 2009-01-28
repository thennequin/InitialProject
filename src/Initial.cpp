//---------------------------------------------
//                Initial Engine
//             Initial Engine source
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial.h"

namespace Initial
{

	IRenderDriver *CreateDevice(VideoDriverType type,int Width,int Height,int BitDepth, bool Fullscreen, IString Title, int AntiAliasing, bool UseShader)
	{
#ifdef WIN32
		//new IDeviceWin32();
		//return NULL;
		return IRenderDriver::CreateDevice(type,Width,Height,BitDepth,Fullscreen,Title,AntiAliasing,UseShader);
#else
		return NULL;
#endif
	}

}