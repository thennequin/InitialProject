//---------------------------------------------
//                Initial Engine
//              Font source Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#include "Initial/Ressource/IFont.h"
#include "Initial/3D/ITriangulator.h"
#include "Initial/Math/IMath.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;
using namespace Initial::GUI;
using namespace Initial::Math;
//using namespace Initial::3D;

IFontChar::IFontChar() 
{
	m_iChar=0;
	m_fWidth=0;
	m_iDisplayListFilled=0;
	m_iDisplayListOutline=0;
}

IFontChar::~IFontChar()
{
	//if (m_iDisplayListFilled);
}

void IFontChar::SetPolygons(std::vector<std::vector<Core::IVector3D>>& pols)
{
	m_aOutline=pols;
	ITriangulator triangulate(pols);
	triangulate.Triangulate(m_aFilled);	
}

void IFontChar::Render(IRenderDriver *device, IColor color, bool outline, float outlinesize)
{
	if (device)
	{
		device->_SetColor(color);
		//device->_SetLineSize(outlinesize);

		if (outline)
		{
			if (m_iDisplayListOutline)
				device->_CallList(m_iDisplayListOutline);
			else{
				m_iDisplayListOutline=device->_NewList();
				device->_StartList(m_iDisplayListOutline,true);
					//printf("Font Outline %d\n",m_aOutline.size());
					//device->_SetColor(IColor(1,1,1,1));
					/*for (unsigned int i=0;i<m_aOutline.size();i++)
					{
						for (unsigned int j=0;j<m_aOutline[i].size();j++)
						{	
							//printf("%f %f %f\n",m_aOutline[i][j].x,m_aOutline[i][j].y,m_aOutline[i][j].z);
							if (j==m_aOutline[i].size()-1)
								device->_DrawLine(m_aOutline[i][j],m_aOutline[i][0]);
							else
								device->_DrawLine(m_aOutline[i][j],m_aOutline[i][j+1]);
						}
					}*/
					
					IVector3D dir1,dir2, dir11,dir12,dir21,dir22;
					IVector3D pt1,pt2,pt3,pt4;
					ITriangle tri;
					device->_StartTriangleDraw(false);
					
					for (unsigned int i=0;i<m_aOutline.size();i++)
					{
						std::vector<IVector3D> dirs;
						std::vector<IVector3D> lindirs;
						//Get Direction
						for (unsigned int j=0;j<m_aOutline[i].size()-1;j++)
						{	
							IVector3D dir;
							if (j==m_aOutline[i].size()-2)
								dir=m_aOutline[i][0]-m_aOutline[i][j];
							else
								dir=m_aOutline[i][j+1]-m_aOutline[i][j];
							dir.Normalize();
							dirs.push_back(dir);
						}

						//Lineariz direction
						for (unsigned int j=0;j<m_aOutline[i].size()-1;j++)
						{
							IVector3D lin;
							if (j==0)
								lin=dirs[j]+dirs[m_aOutline[i].size()-2];
							else
								lin=dirs[j]+dirs[j-1];
							lin.Normalize();
							lindirs.push_back(lin);
						}

						//Render
						ITriangle tri;
						for (unsigned int j=0;j<m_aOutline[i].size()-1;j++)
						{
							IVector3D pt1,pt2,pt3,pt4;
							int next;
							if (j==m_aOutline[i].size()-2)
								next=0;
							else
								next=j+1;
				
							pt1=lindirs[j];
							pt2=lindirs[j];
							pt3=lindirs[next];
							pt4=lindirs[next];
							pt1.Set(-pt1.y,pt1.x,0);
							pt2.Set(pt2.y,-pt2.x,0);
							pt3.Set(-pt3.y,pt3.x,0);
							pt4.Set(pt4.y,-pt4.x,0);
							/*pt1.Normalize();
							pt2.Normalize();
							pt3.Normalize();
							pt4.Normalize();*/

							pt1=pt1*outlinesize+m_aOutline[i][j];
							pt2=pt2*outlinesize+m_aOutline[i][j];
							pt3=pt3*outlinesize+m_aOutline[i][next];
							pt4=pt4*outlinesize+m_aOutline[i][next];
							tri.SetVertex(0,pt1);
							tri.SetVertex(1,pt2);
							tri.SetVertex(2,pt3);
							//device->_StartTriangleDraw(true);
							device->_DrawTriangle(&tri);
							//device->_EndTriangleDraw();
							tri.SetVertex(0,pt4);
							tri.SetVertex(1,pt2);
							tri.SetVertex(2,pt3);
							//device->_StartTriangleDraw(true);
							device->_DrawTriangle(&tri);
							//device->_EndTriangleDraw();

							//device->_SetColor(IColor(1,0,0,1));
							//device->_DrawLine(pt1,pt2);
						}						
					}
					device->_EndTriangleDraw();
					
					
				device->_EndList();
			}
		}else{			
			if (m_iDisplayListFilled)
				device->_CallList(m_iDisplayListFilled);
			else{				
				m_iDisplayListFilled=device->_NewList();
				device->_StartList(m_iDisplayListFilled,true);
					device->_StartTriangleDraw(false);
					//printf("Font Filled %d\n",m_aFilled.size());
					for (unsigned int i=0;i<m_aFilled.size();i++)
					{
						device->_DrawTriangle(&m_aFilled[i]);
					}
					device->_EndTriangleDraw();
				device->_EndList();
			}				
		}
	}
}