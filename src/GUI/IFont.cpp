//---------------------------------------------
//                Initial Engine
//              Font source Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#include "GUI/IFont.h"
#include "Core/ITriangulator.h"

using namespace Initial;
using namespace GUI;

IFontChar::IFontChar() 
{
	m_iChar=0;
	m_fWidth=0;
	m_iDisplayListFilled=0;
	m_iDisplayListOutline=0;
}

IFontChar::~IFontChar()
{
	if (m_iDisplayListFilled);
}

void IFontChar::SetPolygons(IArray<IPolygon>& pols)
{
	IArray<ITriangle> triangle;
	ITriangulator triangulate(pols);
	triangulate.Triangulate(triangle);	

	//printf("%c %d\n",i,state.Array->Count());
	for (int i=0;i<triangle.Count();i++)
		m_aFilled.Add(triangle[i]);

	for (int i=0;i<pols.Count();i++)
		m_aOutline.Add(pols[i]);
}

void IFontChar::Render(IRenderDriver *device, IColor color, bool outline, float outlinesize)
{
	if (device)
	{
		device->_SetColor(color);
		device->_SetLineSize(outlinesize);

		if (outline)
		{
			if (m_iDisplayListOutline)
				device->_CallList(m_iDisplayListOutline);
			else{
				m_iDisplayListOutline=device->_NewList();
				device->_StartList(m_iDisplayListOutline,true);
					for (unsigned int i=0;i<m_aOutline.Count();i++)
					{
						for (unsigned int j=0;j<m_aOutline[i].Count();j++)
						{					
							if (j==m_aOutline[i].Count()-1)
								device->_DrawLine(m_aOutline[i].GetPoint(j),m_aOutline[i].GetPoint(0));
							else
								device->_DrawLine(m_aOutline[i].GetPoint(j),m_aOutline[i].GetPoint(j+1));
						}
					}
				device->_EndList();
			}
		}else{			
			if (m_iDisplayListFilled)
				device->_CallList(m_iDisplayListFilled);
			else{				
				m_iDisplayListFilled=device->_NewList();
				device->_StartList(m_iDisplayListFilled,true);
					device->_StartPolyDraw(false);
					for (unsigned int i=0;i<m_aFilled.Count();i++)
					{
						device->_DrawPolygon(&m_aFilled[i]);
					}
					device->_EndPolyDraw();
				device->_EndList();
			}				
		}
	}
}