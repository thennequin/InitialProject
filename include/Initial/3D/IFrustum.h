//---------------------------------------------
//                Initial Engine
//                IFrustum Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IFRUSTUM_HEADER_
#define _IFRUSTUM_HEADER_

#include "Initial/Core/IVector3D.h"
#include "Initial/3D/IBBox.h"
#include "Initial/3D/IPlane.h"
#include "Initial/Video/IRenderDriver.h"

namespace Initial
{
	class IFrustum
	{
	protected:
		enum FrustumFace
		{
			IFF_TOP = 0, 
			IFF_BOTTOM,
			IFF_LEFT,
			IFF_RIGHT,
			IFF_NEARP,
			IFF_FARP,
		};
	public:
		enum Clip
		{
			IC_OUTSIDE,
			IC_INTERSECT,
			IC_INSIDE,
		};
	public:
		IFrustum();

		//void SetFrustumFromCamera(ICamera* cam);
		void SetFrustumFromMatrix(Math::IMatrix mat);
		int PointInFrustum(Core::IVector3D &p);
		int SphereInFrustum(Core::IVector3D &p, float radius);
		int BoxInFrustum(IBBox &b);

		void CalcVertice();
		void DrawFrustum(Video::IRenderDriver *driver);
	protected:
		IPlane pl[6];

		/*Core::IVector3D ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
		float nearD, farD, ratio, angle,tang;
		float nw,nh,fw,fh;*/

		Core::IVector3D m_vVertices[8];
	};
}

#endif
