//---------------------------------------------
//                Initial Engine
//                 Math  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMATH_HEADER_
#define _IMATH_HEADER_

#include "Core/IMatrix.h"
#include "3D/INormal.h"
#include "3D/IVertex.h"
#include "3D/ITriangle.h"

namespace Initial
{
	class IBBox;
	class ICamera;

	namespace Math
	{
		const float I_PIE = 3.1415926535897932384626433832795;
		#define RAD_TO_DEG(rad) 180.0*(rad)/I_PIE
		#define DEG_TO_RAD(deg) I_PIE*(deg)/180.0

		IMatrix TranslationToMatrix(float x, float y, float z);
		IMatrix AngleXToMatrix(float angle);
		IMatrix AngleYToMatrix(float angle);
		IMatrix AngleZToMatrix(float angle);

		IVertex PolygonCenter(ITriangle poly);
		INormal NormalFromPolygon(ITriangle poly);
		void	CalcultatePolygonNormals(ITriangle& poly);

		IVertex RenderCubicBezier(
				IVertex pt1, IVertex pt2,
				IVertex inter1, IVertex inter2,
				double pos);

		IVertex RenderConicBezier(
			IVertex pt1, IVertex pt2,
			IVertex inter,
			double pos);

		bool BBoxVisibleInCameraView(IBBox& box, ICamera& camera, IMatrix boxpos);
	}
}
#endif