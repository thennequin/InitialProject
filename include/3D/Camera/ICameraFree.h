//---------------------------------------------
//                Initial Engine
//              Camera Free Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ICAMERAFREE_HEADER_
#define _ICAMERAFREE_HEADER_

#include "ICamera.h"

namespace Initial
{

	class IRenderDriver;

	class ICameraFree : public ICamera
	{
		friend class IRenderDriver;
	public:
		ICameraFree(float fov=90.0, float ratio=4/3, float zFar=10000, float zNear=0.1);

		virtual void Move(float x, float y, float z);
		virtual void RotateX(float angle);
		virtual void RotateY(float angle);
		virtual void RotateZ(float angle);

		void UpdateView(IRenderDriver *device);
	protected:
		float AngleX;
	};

}

#endif