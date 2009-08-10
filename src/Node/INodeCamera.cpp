//---------------------------------------------
//                Initial Engine
//          Node skybox source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Node/INodeCamera.h"
#include "Initial/Math/IMath.h"

using namespace Initial::Core;
using namespace Initial::Math;
using namespace Initial::Video;

namespace Initial
{	
	INodeCamera::INodeCamera(IDevice *device)
		: INode(device)
	{
	}	

	Math::IMatrix INodeCamera::GetUpdateViewMatrix()
	{
		return GetPerspectiveMatrix(m_fFov,m_fRatio,m_fzFar,m_fzNear)/**GetMatrix()*/;
	}	
}
