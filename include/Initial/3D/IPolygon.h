//---------------------------------------------
//                Initial Engine
//               Polygon  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IPOLYGON_HEADER_
#define _IPOLYGON_HEADER_

#include "stdio.h"
#include "Initial/Core/IArray.h"
#include "Initial/Core/IVector3D.h"

namespace Initial
{

	class IPolygon
	{
	public:
		IPolygon() 
		{
		}

		~IPolygon() 
		{
		}

		void AddPoint(const Core::IVector3D& vert)
		{
			m_pVertex.Add(vert);
		}

		UINT Count()
		{
			return m_pVertex.Count();
		}

		Core::IVector3D& GetPoint(unsigned int id)
		{
			return m_pVertex[id];
		}

		void operator=(const IPolygon& poly)
		{
			m_pVertex=poly.m_pVertex;
		}

	protected:
		Core::IArray<Core::IVector3D>	m_pVertex;
	};

}

#endif
