//---------------------------------------------
//                Initial Engine
//             Camera source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/Camera/ICamera.h"
#include "Initial/Video/IRenderDriver.h"
#include "Initial/Math/IMath.h"
#include <math.h>

using namespace Initial;
using namespace Initial::Math;
using namespace Initial::Video;

ICamera::ICamera(float fov, float ratio, float zFar, float zNear)
{
	m_fFov=fov;
	m_fRatio=ratio;
	m_fzNear=zNear;
	m_fzFar=zFar;
	m_mPosition.MakeIdentity(4);
}

void ICamera::SetFov(float fov)
{
	m_fFov=fov;
}

float ICamera::GetFov()
{
	return m_fFov;
}

void ICamera::SetMatrix(IMatrix mat)
{
	m_mPosition=mat;
}

IMatrix ICamera::GetMatrix()
{
	return IMatrix(m_mPosition);
}

Math::IMatrix ICamera::GetPerspectiveMatrix()
{
	/*double f = 1.0/tan(DEG_TO_RAD(m_fFov)/2.0);
	double a=(m_fzFar+m_fzNear)/(m_fzNear-m_fzFar);
	double b=(2*m_fzFar*m_fzNear)/(m_fzNear-m_fzFar);

	IMatrix PerspectiveMat(4);
	PerspectiveMat.MakeZero();
	PerspectiveMat[0][0]=f/m_fRatio;
	PerspectiveMat[1][1]=f;
	PerspectiveMat[2][2]=a;
	PerspectiveMat[3][2]=b;
	PerspectiveMat[2][3]=-1;
	return PerspectiveMat;*/
	return Math::GetPerspectiveMatrix(m_fFov,m_fRatio,m_fzFar,m_fzNear);
}

Math::IMatrix ICamera::GetOrthoMatrix()
{
	float right=0;
	float left=1;
	float top=1;
	float bottom=0;

	double tx = - (right + left)/(right - left);
	double ty = - (top + bottom)/(top - bottom);
	double tz = - (m_fzFar + m_fzNear)/(m_fzFar - m_fzNear);

	IMatrix OrthoMat(4);
	OrthoMat.MakeZero();
	OrthoMat[0][0]=2.0/(right - left);
	OrthoMat[1][1]=2.0/(top - bottom);
	OrthoMat[2][2]=2.0/(m_fzFar - m_fzNear);
	OrthoMat[3][0]=tx;
	OrthoMat[3][1]=ty;
	OrthoMat[3][2]=tz;
	OrthoMat[3][3]=1.0;

	return OrthoMat;
}

Math::IMatrix ICamera::GetFrustumMatrix()
{
	float right=0;
	float left=1;
	float top=1;
	float bottom=0;

	double A = (right + left)/(right - left);
	double B = (top + bottom)/(top - bottom);
	double C = (m_fzFar + m_fzNear)/(m_fzFar - m_fzNear);
	double D = (2 * m_fzFar * m_fzNear)/(m_fzFar - m_fzNear);

	IMatrix FrustumMat(4);
	FrustumMat.MakeZero();
	FrustumMat[2][0]=A;
	FrustumMat[2][1]=B;
	FrustumMat[2][2]=C;
	FrustumMat[3][2]=D;
	FrustumMat[2][3]=-1;
	FrustumMat[0][0]=(2.0 * m_fzNear)/(right - left);
	FrustumMat[1][1]=(2.0 * m_fzNear)/(top - bottom);
	return FrustumMat;
}

void ICamera::SetRatio(float ratio)
{
	m_fRatio=ratio;
}

void ICamera::SetzFar(float z)
{
	m_fzFar=z;
}

void ICamera::SetzNear(float z)
{
	m_fzNear=z;
}

void ICamera::UpdateView(IRenderDriver *device)
{
	if (device)
	{
		device->_SetProjectionMatrix();
		//device->_PushMatrix();
		device->_LoadIdentity();	
		device->_SetMatrix(GetUpdateViewMatrix());
		//device->_MultMatrix(GetMatrix());
	}
}