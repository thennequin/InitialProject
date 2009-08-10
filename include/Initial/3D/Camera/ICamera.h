//---------------------------------------------
//                Initial Engine
//                Camera Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ICAMERA_HEADER_
#define _ICAMERA_HEADER_

//#include "Initial/Video/IRenderDriver.h"
#include "Initial/Math/IMatrix.h"
//#include "Initial/Core/IVector3D.h"


namespace Initial
{
	namespace Video
	{
		class IRenderDriver;
	}

	class ICamera
	{
		friend class Video::IRenderDriver;
	public:
		ICamera(float fov=90.0, float ratio=4/3, float zFar=1000, float zNear=0.01);
		void SetFov(float fov);
		float GetFov();

		void SetMatrix(Math::IMatrix mat);
		virtual Math::IMatrix GetMatrix();

		Math::IMatrix GetPerspectiveMatrix();
		Math::IMatrix GetOrthoMatrix();
		Math::IMatrix GetFrustumMatrix();

		virtual void Move(float x, float y, float z)=0;
		virtual void RotateX(float angle)=0;
		virtual void RotateY(float angle)=0;
		virtual void RotateZ(float angle)=0;

		virtual void UpdateView(Video::IRenderDriver *device);
		virtual Math::IMatrix GetUpdateViewMatrix()=0;

		void SetRatio(float ratio);
		void SetzFar(float z);
		void SetzNear(float z);
	protected:
		float m_fFov;
		float m_fRatio;
		float m_fzFar;
		float m_fzNear;
		Math::IMatrix m_mPosition;
	};

}

#endif