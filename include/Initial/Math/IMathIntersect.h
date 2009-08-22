//---------------------------------------------
//                Initial Engine
//                 Math  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMATHINTERSECT_HEADER_
#define _IMATHINTERSECT_HEADER_

#include "Initial/Math/IMatrix.h"
#include "Initial/Math/IMath.h"
#include "Initial/3D/IPlane.h"

namespace Initial
{
	namespace Math
	{
		namespace Intersect
		{
			void RayIntersectRay(Core::IVector3D start1, Core::IVector3D dir1, Core::IVector3D start2, Core::IVector3D dir2, Core::IVector3D& res1, Core::IVector3D& res2);
			bool RayIntersectPlane(Core::IVector3D startray, Core::IVector3D raydir, Core::IVector3D planeorig, Core::IVector3D planedir1, Core::IVector3D planedir2, Core::IVector3D& res);
			bool RayIntersectTriangle(Core::IVector3D tri[3], Core::IVector3D linestart, Core::IVector3D lineend, Core::IVector3D& result, bool allray=false);
			bool RayIntersectTriangle(ITriangle& tri, Core::IVector3D linestart, Core::IVector3D lineend, Core::IVector3D& result, bool allray=false);
			bool RayIntersectBox(IBBox box, Core::IVector3D linestart, Core::IVector3D lineend, Core::IVector3D& result, bool allray=false);

			bool PlaneIntersect(IPlane a, IPlane b, IPlane c, Core::IVector3D& res);

			bool PointOnRay(Core::IVector3D point, Core::IVector3D start, Core::IVector3D end);
		}
	}
}

#endif