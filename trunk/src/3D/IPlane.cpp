//---------------------------------------------
//                Initial Engine
//             Plane source  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/IPlane.h"

using namespace Initial::Core;

namespace Initial
{
	IPlane::IPlane()
	{
		m_vNormal.Set(0,0,1);
	}

	IPlane::IPlane(IVector3D &v1,  IVector3D &v2,  IVector3D &v3)
	{
		Set3Points(v1,v2,v3);
	}

	void IPlane::Set3Points(IVector3D &v1,  IVector3D &v2,  IVector3D &v3)
	{
		IVector3D aux1, aux2;

		aux1 = v1 - v2;
		aux2 = v3 - v2;

		m_vNormal = aux2 * aux1;

		m_vNormal.Normalize();
		m_vPoint = v2;
		m_fD = -Dot(m_vNormal,m_vPoint);
	}

	void IPlane::SetNormalAndPoint(IVector3D &normal, IVector3D &point)
	{
		m_vNormal = normal;
		m_vNormal.Normalize();
		m_fD = -Dot(m_vNormal,point);
	}

	void IPlane::SetCoefficients(float a, float b, float c, float d)
	{
		// set the normal vector
		m_vNormal.Set(a,b,c);
		//m_fD = d/m_vNormal.Length();
		//m_vNormal.Normalize();
		float l = m_vNormal.Length();
		// normalize the vector
		m_vNormal.Set(a/l,b/l,c/l);
		// and divide d by th length as well
		m_fD = d/l;
	}
		
	float IPlane::Distance(IVector3D &p) {

		return (m_fD + Dot(m_vNormal,p));
	}

	IVector3D IPlane::GetPoint()
	{
		return m_vPoint;
	}

	IVector3D IPlane::GetNormal()
	{
		return m_vNormal;
	}

	float IPlane::GetD()
	{
		return m_fD;
	}
}
