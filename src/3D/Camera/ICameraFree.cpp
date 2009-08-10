//---------------------------------------------
//                Initial Engine
//             Camera source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/Camera/ICameraFree.h"
#include "Initial/Video/IRenderDriver.h"
#include "Initial/Math/IMath.h"
#include <math.h>

using namespace Initial;
using namespace Initial::Math;
using namespace Initial::Video;

ICameraFree::ICameraFree(float fov, float ratio, float zFar, float zNear)
: ICamera(fov,ratio,zFar,zNear)
{
}

void ICameraFree::UpdateView(IRenderDriver *device)
{
	if (device)
	{
		device->_SetProjectionMatrix();
		device->_LoadIdentity();
		device->_MultMatrix(GetPerspectiveMatrix());
		device->_MultMatrix(GetMatrix());
	}
}

void ICameraFree::Move(float x, float y, float z)
{
	m_mPosition=TranslationToMatrix(x,-y,z)*m_mPosition;
}

void ICameraFree::RotateX(float angle)
{
	m_mPosition=AngleXToMatrix4x4(angle)*m_mPosition;
}

void ICameraFree::RotateY(float angle)
{
	m_mPosition=AngleYToMatrix4x4(angle)*m_mPosition;
}

void ICameraFree::RotateZ(float angle)
{
	m_mPosition=AngleZToMatrix4x4(angle)*m_mPosition;
}