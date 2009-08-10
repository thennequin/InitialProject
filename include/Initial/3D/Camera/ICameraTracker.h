//---------------------------------------------
//                Initial Engine
//            Camera Tracker  Module
//
//            By Thibault HENNEQUIN
//              January 10th  2009
//---------------------------------------------

#ifndef _ICAMERATRACKER_HEADER_
#define _ICAMERATRACKER_HEADER_

#include "Initial/Math/IMatrix.h"

namespace Initial
{
	class IRenderDriver;

	class ICameraTracker : public ICamera
	{
		friend class IRenderDriver;
	public:
		ICameraTracker(float fov=90.0);
		void SetFov(float fov);
		float GetFov();

		void SetMatrix(Math::IMatrix mat);
		virtual Math::IMatrix GetMatrix();

		virtual void Move(float x, float y, float z)=0;
		virtual void RotateX(float angle)=0;
		virtual void RotateY(float angle)=0;
		virtual void RotateZ(float angle)=0;

		virtual void UpdateView(IRenderDriver *device)=0;
	protected:
		float m_fFov;
		Math::IMatrix m_mPosition;
	};

}

#endif