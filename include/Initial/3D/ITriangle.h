//---------------------------------------------
//                Initial Engine
//               Triangle  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITRIANGLE_HEADER_
#define _ITRIANGLE_HEADER_

#include "Initial/Core/IArray.h"
#include "Initial/Core/IColor.h"
#include "Initial/Core/IVector2D.h"
#include "Initial/Core/IVector3D.h"
#include "Initial/Video/IMaterial.h"

namespace Initial
{

	class ITriangle // triangle & quad
	{
	public:
		ITriangle()
		{ /*m_pMaterial=NULL;*/
			m_pNormal[0]=Core::IVector3D(0,1,0);
			m_pNormal[1]=m_pNormal[0];
			m_pNormal[2]=m_pNormal[0]; 
		}

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
		void SetVertex(int index, Core::IVector3D vertex) { m_pVertex[index]=vertex; }
		void SetNormal(int index, Core::IVector3D normal) { m_pNormal[index]=normal; }
		void SetCoord(int index, Core::IVector2D coord) { m_pCoord[index]=coord; }
		//void SetMaterial(IMaterial *material) { m_pMaterial=material; }

		//IColor* GetColors() { return m_pColor; }
		Core::IVector3D* GetVertex() { return m_pVertex; }
		Core::IVector3D* GetNormals() { return m_pNormal; }
		Core::IVector2D* GetCoords() { return m_pCoord; }
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
		Core::IVector3D	m_pVertex[3];		
		Core::IVector3D	m_pNormal[3]; // 0;Normal 1;Tangent 2;Birnomal
		Core::IVector2D	m_pCoord[3];		
		//IMaterial *m_pMaterial;

	};

}

#endif