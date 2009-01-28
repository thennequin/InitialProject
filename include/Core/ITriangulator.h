//---------------------------------------------
//                Initial Engine
//             Triangulator  Module
//                ITriangulator.h
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITRIANGULATOR_HEADER_
#define _ITRIANGULATOR_HEADER_

#include "Core/IArray.h"
#include "3D/IPolygon.h"
#include "3D/ITriangle.h"
#include "3D/IVertex.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace Initial
{

	class ITriangulator
	{
	public:
		ITriangulator(IArray<IPolygon>& points);
		~ITriangulator();

		void Triangulate(IArray<ITriangle>& ret);
	protected:
		static void CALLBACK combineCallback(GLdouble coords[3], IVertex *vertex_data[4], GLfloat weight[4], IVertex **dataOut );
		static void CALLBACK vertexCallback(GLvoid *vertex);
		static void CALLBACK beginCallback(GLenum type);
		
		static IArray<ITriangle> m_aTriangle;
		static int m_iType;
		static int m_iPos;
		static IVertex m_vVert[2];
	protected:
		IArray<IPolygon> m_aContours;
	};

}

#endif