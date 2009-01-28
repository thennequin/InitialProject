//---------------------------------------------
//                Initial Engine
//               Triangle  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITRIANGLE_HEADER_
#define _ITRIANGLE_HEADER_

#include "Core/IArray.h"
#include "Core/IColor.h"
#include "IVertex.h"
#include "INormal.h"
#include "ICoord.h"
#include "Video/IMaterial.h"

namespace Initial
{

	class ITriangle // triangle & quad
	{
	public:
		ITriangle() { /*m_pMaterial=NULL;*/ }

		ITriangle(const ITriangle& tri)
		{
			/*m_pColor[0]=tri.m_pColor[0];
			m_pColor[1]=tri.m_pColor[1];
			m_pColor[2]=tri.m_pColor[2];*/

			m_pVertex[0]=tri.m_pVertex[0];
			m_pVertex[1]=tri.m_pVertex[1];
			m_pVertex[2]=tri.m_pVertex[2];

			m_pNormal[0]=tri.m_pNormal[0];
			m_pNormal[1]=tri.m_pNormal[1];
			m_pNormal[2]=tri.m_pNormal[2];

			m_pCoord[0]=tri.m_pCoord[0];
			m_pCoord[1]=tri.m_pCoord[1];
			m_pCoord[2]=tri.m_pCoord[2];

			//m_pMaterial=tri.m_pMaterial;
		}

		//void SetColor(int index, IColor color) { m_pColor[index]=color; }
		void SetVertex(int index, IVertex vertex) { m_pVertex[index]=vertex; }
		void SetNormal(int index, IVertex normal) { m_pNormal[index]=normal; }
		void SetCoord(int index, ICoord coord) { m_pCoord[index]=coord; }
		//void SetMaterial(IMaterial *material) { m_pMaterial=material; }

		//IColor* GetColors() { return m_pColor; }
		IVertex* GetVertex() { return m_pVertex; }
		IVertex* GetNormals() { return m_pNormal; }
		ICoord* GetCoords() { return m_pCoord; }
		//IMaterial* GetMaterial() { return m_pMaterial; }

		//void CalcNormal() { m_pNormal[0] = NormalFromPolygon(*this); }

		void operator= (const ITriangle& tri)
		{
			/*m_pColor[0]=tri.m_pColor[0];
			m_pColor[1]=tri.m_pColor[1];
			m_pColor[2]=tri.m_pColor[2];*/

			m_pVertex[0]=tri.m_pVertex[0];
			m_pVertex[1]=tri.m_pVertex[1];
			m_pVertex[2]=tri.m_pVertex[2];

			m_pNormal[0]=tri.m_pNormal[0];
			m_pNormal[1]=tri.m_pNormal[1];
			m_pNormal[2]=tri.m_pNormal[2];

			m_pCoord[0]=tri.m_pCoord[0];
			m_pCoord[1]=tri.m_pCoord[1];
			m_pCoord[2]=tri.m_pCoord[2];

			//m_pMaterial=tri.m_pMaterial;
		}

	private:
		//IColor	m_pColor[3];
		IVertex	m_pVertex[3];		
		IVertex	m_pNormal[3]; // 0;Normal 1;Tangent 2;Birnomal
		ICoord	m_pCoord[3];		
		//IMaterial *m_pMaterial;

	};

}

#endif