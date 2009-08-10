//---------------------------------------------
//                Initial Engine
//          Triangulator source Module
//               ITriangulator.cpp
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/ITriangulator.h"

using namespace Initial;
using namespace Initial::Core;

IArray<ITriangle> ITriangulator::m_aTriangle;
int ITriangulator::m_iType;
int ITriangulator::m_iPos;
IVector3D ITriangulator::m_vVert[2];

ITriangulator::ITriangulator(IArray<IPolygon>& points)
{
	for (UINT i=0;i<points.Count();i++)
	{
		m_aContours.Add(IPolygon());
		for (UINT j=0;j<points[i].Count();j++)
		{
			m_aContours[i].AddPoint(points[i].GetPoint(j));
		}
	}	
}

ITriangulator::~ITriangulator()
{
}

void ITriangulator::Triangulate(IArray<ITriangle*>& ret)
{
	//printf("Begin triangulation\n");
	GLUtesselator *tobj = gluNewTess();

	for (UINT i=0;i<m_aTriangle.Count();i++)
		//if (&m_aTriangle[i])
			//delete m_aTriangle[i];

	m_aTriangle.Clear();

	if (tobj)
	{
		gluTessProperty(tobj, GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_ODD);
		// Set callback functions
		gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK*) ( )) &vertexCallback);
		gluTessCallback(tobj, GLU_TESS_BEGIN,  (void (CALLBACK*) ( )) &beginCallback);
		gluTessCallback(tobj, GLU_TESS_COMBINE,  (void (CALLBACK*) ( )) &combineCallback);

		GLdouble coord[6];
		coord[3]=coord[4]=coord[5]=1.0;
		gluTessBeginPolygon(tobj,NULL);
		//printf("contours %d\n",m_aContours.Count());
		for (unsigned int i=0;i<m_aContours.Count();i++)
		{
			//gluNextContour(tobj,GLU_CCW);
			//printf("	points %d\n",m_aContours[i].Count());
			gluTessBeginContour(tobj);
			for (unsigned int j=0;j<m_aContours[i].Count();j++)
			{
				coord[0]=m_aContours[i].GetPoint(j).GetX();
				coord[1]=m_aContours[i].GetPoint(j).GetY();
				coord[2]=m_aContours[i].GetPoint(j).GetZ();
				gluTessVertex(tobj,coord,m_aContours[i].GetPoint(j));
			}
			gluTessEndContour(tobj);
		}
		gluTessEndPolygon(tobj);
		gluDeleteTess(tobj);
	}

	
	for (UINT j=0;j<m_aTriangle.Count();j++)
	{
		ITriangle *tri=new ITriangle();
		tri->GetVertex()[0].Set(m_aTriangle[j].GetVertex()[0]);
		tri->GetVertex()[1].Set(m_aTriangle[j].GetVertex()[1]);
		tri->GetVertex()[2].Set(m_aTriangle[j].GetVertex()[2]);
		ret.Add(tri);
		delete tri;
	}
}

void CALLBACK ITriangulator::vertexCallback(GLvoid *vertex)
{
	IVector3D vert(*(IVector3D*)vertex);
	if (vert==NULL)
		return;

	//printf("	vertex callback %f %f %f\n",vert[0],vert[1],vert[2]);

	//GL_TRIANGLES
	if (m_iType==GL_TRIANGLES) //4
	{
		if (m_iPos==0)
		{
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[0].Set(vert);
			m_iPos++;
		}else if (m_iPos==1)
		{
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[1].Set(vert);
			m_iPos++;
		}else{
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[2].Set(vert);
			m_aTriangle.Add(ITriangle());
			m_iPos=0;
		}
	}else if (m_iType==GL_TRIANGLE_STRIP) //5
	{
		if (m_iPos==0)
		{
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[0].Set(vert);
			m_iPos++;
		}else if (m_iPos==1)
		{
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[1].Set(vert);
			m_vVert[0].Set(vert);
			m_iPos++;
		}else if (m_iPos==2){
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[2].Set(vert);
			m_vVert[1].Set(vert);
			m_iPos++;
		}else{
			m_aTriangle.Add(ITriangle());
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[0].Set(m_vVert[0]);
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[1].Set(m_vVert[1]);

			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[2].Set(vert);
			m_vVert[0]=m_vVert[1];
			m_vVert[1].Set(vert);
		}
	}else if (m_iType==GL_TRIANGLE_FAN) //6 BUG
	{
		if (m_iPos==0)
		{
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[0].Set(vert);
			m_vVert[0].Set(vert);
			m_iPos++;
		}else if (m_iPos==1)
		{
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[1].Set(vert);
			m_iPos++;
		}else if (m_iPos==2){
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[2].Set(vert);
			m_vVert[1].Set(vert);
			m_iPos++;
		}else{
			m_aTriangle.Add(ITriangle());
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[0].Set(m_vVert[0]);
			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[1].Set(m_vVert[1]);

			m_aTriangle[m_aTriangle.Count()-1].GetVertex()[2].Set(vert);
			m_vVert[1].Set(vert);	
		}
	}
}

void CALLBACK ITriangulator::beginCallback(GLenum type)
{
	//printf("begin callback %d\n",type);
	m_iType=type;
	m_aTriangle.Add(ITriangle());
	m_iPos=0;
}

void CALLBACK ITriangulator::combineCallback(GLdouble coords[3], IVector3D *vertex_data[4], GLfloat weight[4], IVector3D **dataOut )
{
	//printf("combine callback\n");
	IVector3D *vertex=new IVector3D();
	vertex->Set(coords[0], coords[1], coords[2]);
	*dataOut = vertex;
}