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
#include "Core/IArray.h"
#include "IVertex.h"

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

		void AddPoint(const IVertex& vert)
		{
			m_pVertex.Add(vert);
		}

		unsigned int Count()
		{
			return m_pVertex.Count();
		}

		IVertex& GetPoint(unsigned int id)
		{
			return m_pVertex[id];
		}

		void operator=(const IPolygon& poly)
		{
			m_pVertex=poly.m_pVertex;
		}

	protected:
		IArray<IVertex>	m_pVertex;
	};

}

#endif
