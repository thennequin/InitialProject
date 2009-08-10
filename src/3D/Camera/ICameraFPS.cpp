//---------------------------------------------
//                Initial Engine
//             Camera source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/Camera/ICameraFPS.h"
#include "Initial/Video/IRenderDriver.h"
#include "Initial/Math/IMath.h"
#include <math.h>

using namespace Initial;
using namespace Math;

ICameraFPS::ICameraFPS(float fov, float ratio, float zFar, float zNear)
: ICamera(fov,ratio,zFar,zNear)
{
	AngleX=0;
}

Math::IMatrix ICameraFPS::GetUpdateViewMatrix()
{
	return GetPerspectiveMatrix()*GetMatrix();
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
	m_mPosition=AngleYToMatrix4x4(angle)*m_mPosition;
}

void ICameraFPS::RotateZ(float angle)
{
}

IMatrix ICameraFPS::GetMatrix()
{
	return IMatrix(AngleXToMatrix4x4(AngleX)*m_mPosition);
}