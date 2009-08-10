//---------------------------------------------
//                Initial Engine
//             Initial Engine source
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Initial.h"

using namespace Initial::Core;

namespace Initial
{
	IDevice *CreateDevice(VideoDriverType type,int Width,int Height,int BitDepth, bool Fullscreen, IString Title)
	{
#ifdef WIN32
		IDevice *device = new IDeviceWin32(Title,Width,Height,BitDepth,Fullscreen);
		if (device)
		{
			if (device->CreateRender(type))
				return device;
			else 
				delete device;
		}
#else
		//Other
#endif
		return NULL;
	}

	IDevice *CreateDeviceEx(VideoDriverType type, int BitDepth, void *WinId)
	{
#ifdef WIN32
		IDevice *device = new IDeviceWin32("",0,0,BitDepth,false,WinId);
		if (device)
		{
			if (device->CreateRender(type))
				return device;
			else 
				delete device;
		}
#else
		//Other
#endif
		return NULL;
	}

}