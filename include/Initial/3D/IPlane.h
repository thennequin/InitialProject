//---------------------------------------------
//                Initial Engine
//                 Plane Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IPLANE_HEADER_
#define _IPLANE_HEADER_

#include "Initial/Core/IVector3D.h"

namespace Initial
{
	class IPlane  
	{
	public:
		IPlane();
		IPlane(Core::IVector3D &v1,  Core::IVector3D &v2,  Core::IVector3D &v3);

		void Set3Points( Core::IVector3D &v1,  Core::IVector3D &v2,  Core::IVector3D &v3);
		void SetNormalAndPoint(Core::IVector3D &normal, Core::IVector3D &point);
		void SetCoefficients(float a, float b, float c, float d);
		float Distance(Core::IVector3D &p);

		Core::IVector3D GetPoint();
		Core::IVector3D GetNormal();
		float GetD();
	protected:
		Core::IVector3D m_vNormal, m_vPoint;
		float m_fD;
	};
}


#endif
