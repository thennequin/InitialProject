//---------------------------------------------
//                Initial Engine
//             Camera source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/Camera/ICameraFPS.h"
#include "Video/IRenderDriver.h"
#include "Core/IMath.h"
#include <math.h>

using namespace Initial;
using namespace Math;

ICameraFPS::ICameraFPS(float fov, float ratio, float zFar, float zNear)
: ICamera(fov,ratio,zFar,zNear)
{
	AngleX=0;
}

void ICameraFPS::UpdateView(IRenderDriver *device)
{
	if (device)
	{
		device->_SetProjectionMatrix();
		//device->_PushMatrix();
		device->_LoadIdentity();	
		device->_MultMatrix(GetPerspectiveMatrix()*GetMatrix());
		//device->_MultMatrix(GetMatrix());
	}
}

void ICameraFPS::Move(float x, float y, float z)
{
	m_mPosition=TranslationToMatrix(x,-y,z)*m_mPosition;
}

void ICameraFPS::RotateX(float angle)
{
	AngleX+=angle;
	AngleX = AngleX>DEG_TO_RAD(90) ? DEG_TO_RAD(90) : AngleX;
	AngleX = AngleX<DEG_TO_RAD(-90) ? DEG_TO_RAD(-90) : AngleX;
}

void ICameraFPS::RotateY(float angle)
{
	m_mPosition=AngleYToMatrix(angle)*m_mPosition;
}

void ICameraFPS::RotateZ(float angle)
{
}

IMatrix ICameraFPS::GetMatrix()
{
	return IMatrix(AngleXToMatrix(AngleX)*m_mPosition);
}