//---------------------------------------------
//                Initial Engine
//                 Math  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMATH_HEADER_
#define _IMATH_HEADER_

#include "Initial/Math/IMatrix.h"
#include "Initial/Core/IVector3D.h"
#include "Initial/3D/ITriangle.h"

namespace Initial
{
	class IBBox;
	class ICamera;

	namespace Math
	{
		const double I_PIE = 3.1415926535897932384626433832795;
		#define RAD_TO_DEG(rad) 180.0*(rad)/I_PIE
		#define DEG_TO_RAD(deg) I_PIE*(deg)/180.0

		IMatrix TranslationToMatrix(float x, float y, float z);
		IMatrix ScaleToMatrix(float x, float y, float z);
		IMatrix TranslationToMatrix(Core::IVector3D vec);
		IMatrix ScaleToMatrix(Core::IVector3D vec);

		IMatrix AngleXToMatrix4x4(float angle);
		IMatrix AngleYToMatrix4x4(float angle);
		IMatrix AngleZToMatrix4x4(float angle);

		IMatrix AngleXToMatrix3x3(float angle);
		IMatrix AngleYToMatrix3x3(float angle);
		IMatrix AngleZToMatrix3x3(float angle);		

		Core::IVector3D GetDegreesFromMatrix3x3(IMatrix& mat);
		IMatrix GetMatrix3x3FromDegrees(Core::IVector3D vect);
		IMatrix GetInverseMatrix3x3FromDegrees(Core::IVector3D vect);

		Core::IVector3D operator* (IMatrix& mat, Core::IVector3D& vec);
		Core::IVector3D operator* (Core::IVector3D& vec, IMatrix& mat);

		Core::IVector3D PolygonCenter(ITriangle poly);
		Core::IVector3D NormalFromPolygon(ITriangle poly);

		Core::IVector3D RenderCubicBezier(
				Core::IVector3D pt1, Core::IVector3D pt2,
				Core::IVector3D inter1, Core::IVector3D inter2,
				double pos);

		Core::IVector3D RenderConicBezier(
			Core::IVector3D pt1, Core::IVector3D pt2,
			Core::IVector3D inter,
			double pos);

		/* Convert 3D position to 2D screen position */
		//bool Project(IMatrix projection_4x4, IMatrix modelview_4x4, Core::IVector2D viewport, Core::IVector3D vert, Core::IVector3D& res);
		/* Convert 2D screen position to 3D position */
		//bool UnProject(IMatrix projection_4x4, IMatrix modelview_4x4, Core::IVector2D viewport, Core::IVector3D winpos, Core::IVector3D& res);

		bool BBoxVisibleInCameraView(IBBox& box, ICamera& camera, IMatrix boxpos);

		IBBox BBoxToScreenBBox(IBBox src,IMatrix projection);

		Math::IMatrix GetOrthoMatrix(float right=1, float left=0, float top=1, float bottom=0, float m_fzFar=1000, float m_fzNear=0.1);
		Math::IMatrix GetPerspectiveMatrix(float fov, float ratio, float Far, float Near);
	}
}
#endif