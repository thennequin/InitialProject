//---------------------------------------------
//                Initial Engine
//            Frustum source  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/IFrustum.h"
#include "Initial/Math/IMathIntersect.h"

using namespace Initial::Core;
using namespace Initial::Math;
using namespace Initial::Math::Intersect;

namespace Initial
{
	IFrustum::IFrustum()
	{
	}

	/*void IFrustum::SetFrustumFromCamera(ICamera* cam)
	{
		if (cam)
		{
			SetFrustumFromMatrix(cam->GetViewUpdateMatrix());
		}
	}*/

	void IFrustum::SetFrustumFromMatrix(IMatrix m)
	{
		pl[IFF_NEARP].SetCoefficients(
					 m(0,2) + m(0,3),
					 m(1,2) + m(1,3),
					 m(2,2) + m(2,3),
					 m(3,2) + m(3,3));
		pl[IFF_FARP].SetCoefficients( 
					-m(0,2) + m(0,3),
					-m(1,2) + m(1,3),
					-m(2,2) + m(2,3),
					-m(3,2) + m(3,3));
		pl[IFF_BOTTOM].SetCoefficients(
					 m(0,1) + m(0,3),
					 m(1,1) + m(1,3),
					 m(2,1) + m(2,3),
					 m(3,1) + m(3,3));
		pl[IFF_TOP].SetCoefficients(  
					-m(0,1) + m(0,3),
					-m(1,1) + m(1,3),
					-m(2,1) + m(2,3),
					-m(3,1) + m(3,3));
		pl[IFF_LEFT].SetCoefficients(  
					 m(0,0) + m(0,3),
					 m(1,0) + m(1,3),
					 m(2,0) + m(2,3),
					 m(3,0) + m(3,3));
		pl[IFF_RIGHT].SetCoefficients(
					-m(0,0) + m(0,3),
					-m(1,0) + m(1,3),
					-m(2,0) + m(2,3),
					-m(3,0) + m(3,3));
	}

	void IFrustum::CalcVertice()
	{
		// near plane vertices
		PlaneIntersect(pl[5],pl[2],pl[1],m_vVertices[0]);
		PlaneIntersect(pl[5],pl[2],pl[0],m_vVertices[1]);
		PlaneIntersect(pl[5],pl[3],pl[0],m_vVertices[2]);
		PlaneIntersect(pl[5],pl[3],pl[1],m_vVertices[3]);

		// far plane vertices
		PlaneIntersect(pl[4],pl[2],pl[1],m_vVertices[4]);
		PlaneIntersect(pl[4],pl[2],pl[0],m_vVertices[5]);
		PlaneIntersect(pl[4],pl[3],pl[0],m_vVertices[6]);
		PlaneIntersect(pl[4],pl[3],pl[1],m_vVertices[7]);
	}

	void IFrustum::DrawFrustum(Video::IRenderDriver *driver)
	{
		if (driver)
		{
			//printf("%f %f %f\n",m_vVertices[0].x,m_vVertices[0].y,m_vVertices[0].z);
			// near plane
			driver->_DrawLine(m_vVertices[0],m_vVertices[1]);
			driver->_DrawLine(m_vVertices[1],m_vVertices[2]);
			driver->_DrawLine(m_vVertices[2],m_vVertices[3]);
			driver->_DrawLine(m_vVertices[3],m_vVertices[0]);

			// left plane
			driver->_DrawLine(m_vVertices[4],m_vVertices[0]);
			driver->_DrawLine(m_vVertices[0],m_vVertices[3]);
			driver->_DrawLine(m_vVertices[3],m_vVertices[7]);
			driver->_DrawLine(m_vVertices[7],m_vVertices[4]);

			// right plane
			driver->_DrawLine(m_vVertices[1],m_vVertices[5]);
			driver->_DrawLine(m_vVertices[5],m_vVertices[6]);
			driver->_DrawLine(m_vVertices[6],m_vVertices[2]);
			driver->_DrawLine(m_vVertices[2],m_vVertices[1]);

			// far plane
			driver->_DrawLine(m_vVertices[4],m_vVertices[5]);
			driver->_DrawLine(m_vVertices[5],m_vVertices[6]);
			driver->_DrawLine(m_vVertices[6],m_vVertices[7]);
			driver->_DrawLine(m_vVertices[7],m_vVertices[4]);
		}
	}

	int IFrustum::PointInFrustum(IVector3D &p) 
	{
		int result = IC_INSIDE;
		for(int i=0; i < 6; i++) {

			if (pl[i].Distance(p) < 0)
				return IC_OUTSIDE;
		}
		return result ;

	}

	int IFrustum::SphereInFrustum(IVector3D &p, float radius) 
	{
		float distance;
		int result = IC_INSIDE;
		
		for(int i=0; i < 6; i++) {
			distance = pl[i].Distance(p);
			if (distance < -radius)
				return IC_OUTSIDE;
			else if (distance < radius)
				result = IC_INTERSECT;
		}
		return result;
	}

	int IFrustum::BoxInFrustum(IBBox &b) 
	{
		int result = IC_INSIDE;
		//for each plane do ...
		for(int i=0; i < 6; i++)
		{
			IVector3D center = b.GetCenter();
			IVector3D dim = b.GetDimension();
			IVector3D Normal = pl[i].GetNormal();

			IVector3D min = center-(dim*0.5);
			IVector3D max = center+(dim*0.5);

			IVector3D P = min;
			if (Normal.GetX() > 0)
				P.SetX(max.GetX());
			if (Normal.GetY() > 0)
				P.SetY(max.GetY());
			if (Normal.GetZ() > 0)
				P.SetZ(max.GetZ());

			// is the positive vertex outside?
			if (pl[i].Distance(P) < 0)
				return IC_OUTSIDE;
			IVector3D N = min;
			if (Normal.GetX() < 0)
				N.SetX(max.GetX());
			if (Normal.GetY() < 0)
				N.SetY(max.GetY());
			if (Normal.GetZ() < 0)
				N.SetZ(max.GetZ());

			// is the negative vertex outside?	
			if (pl[i].Distance(N) < 0)
				result = IC_INTERSECT;
		}
		return result;
	}
}
