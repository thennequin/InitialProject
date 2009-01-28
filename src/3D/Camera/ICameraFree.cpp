//---------------------------------------------
//                Initial Engine
//             Camera source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/Camera/ICameraFree.h"
#include "Video/IRenderDriver.h"
#include "Core/IMath.h"
#include <math.h>

using namespace Initial;
using namespace Math;

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
	m_mPosition=AngleXToMatrix(angle)*m_mPosition;
}

void ICameraFree::RotateY(float angle)
{
	m_mPosition=AngleYToMatrix(angle)*m_mPosition;
}

void ICameraFree::RotateZ(float angle)
{
	m_mPosition=AngleZToMatrix(angle)*m_mPosition;
}