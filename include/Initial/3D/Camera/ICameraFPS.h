//---------------------------------------------
//                Initial Engine
//              Camera FPS  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ICAMERAFPS_HEADER_
#define _ICAMERAFPS_HEADER_

#include "ICamera.h"

namespace Initial
{
	class ICameraFPS : public ICamera
	{
		friend class Video::IRenderDriver;
	public:
		ICameraFPS(float fov=90.0, float ratio=4/3, float zFar=1000, float zNear=0.01);

		virtual Math::IMatrix GetMatrix();

		virtual void Move(float x, float y, float z);
		virtual void RotateX(float angle);
		virtual void RotateY(float angle);
		virtual void RotateZ(float angle);

		virtual Math::IMatrix GetUpdateViewMatrix();
	protected:
		float AngleX;
	};

}

#endif