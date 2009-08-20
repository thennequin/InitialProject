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

#include "Initial/Core/IList.h"
#include "Initial/3D/IPolygon.h"
#include "Initial/3D/ITriangle.h"
#include "Initial/Core/IVector3D.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <vector>

namespace Initial
{

	class ITriangulator
	{
	public:
		ITriangulator(std::vector<std::vector<Core::IVector3D>>& points);
		~ITriangulator();
		
		void Triangulate(std::vector<ITriangle>& ret);
	protected:
		static void CALLBACK combineCallback(GLdouble coords[3], Core::IVector3D *vertex_data[4], GLfloat weight[4], Core::IVector3D **dataOut );
		static void CALLBACK vertexCallback(GLvoid *vertex);
		static void CALLBACK beginCallback(GLenum type);
		
		//static Core::IList<ITriangle> m_aTriangle;
		static std::vector<ITriangle> m_aTriangle;
		static int m_iType;
		static int m_iPos;
		static Core::IVector3D m_vVert[2];
	protected:
		//Core::IList<IPolygon> m_aContours;
		std::vector<std::vector<Core::IVector3D>> m_aContours;
	};

}

#endif