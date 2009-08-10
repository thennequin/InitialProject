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

#include "Initial/Core/IArray.h"
#include "Initial/3D/IPolygon.h"
#include "Initial/3D/ITriangle.h"
#include "Initial/Core/IVector3D.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace Initial
{

	class ITriangulator
	{
	public:
		ITriangulator(Core::IArray<IPolygon>& points);
		~ITriangulator();

		void Triangulate(Core::IArray<ITriangle*>& ret);
	protected:
		static void CALLBACK combineCallback(GLdouble coords[3], Core::IVector3D *vertex_data[4], GLfloat weight[4], Core::IVector3D **dataOut );
		static void CALLBACK vertexCallback(GLvoid *vertex);
		static void CALLBACK beginCallback(GLenum type);
		
		static Core::IArray<ITriangle> m_aTriangle;
		static int m_iType;
		static int m_iPos;
		static Core::IVector3D m_vVert[2];
	protected:
		Core::IArray<IPolygon> m_aContours;
	};

}

#endif