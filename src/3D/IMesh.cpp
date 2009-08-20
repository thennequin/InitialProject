//---------------------------------------------
//                Initial Engine
//            3D Object source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/IMesh.h"
#include "Initial/Math/IMath.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;

IArray<IMesh*> IMesh::m_aObjects;

IMesh::IMesh(IRenderDriver *Device, IMesh* parent)
{
	_Init(Device, parent);
}

IMesh::IMesh(IRenderDriver *Device, IArray<ITriangle> polygons, IMesh* parent)
{
	_Init(Device, parent);
	m_aPolygons = polygons;
}

void IMesh::_Init(IRenderDriver *Device, IMesh* parent)
{
	m_pParent=parent;
	m_pDevice = Device;	
	m_aObjects.Add(this);
	m_mPosition.MakeIdentity(4);
	m_mOrigPosition.MakeIdentity(4);
	m_fScale=1.0;
	m_iDisplayListSolid=m_iDisplayListWireframe=m_iDisplayListNormal=0;
	m_bUpdated=false;
	m_iFlags=IRF_ALWAYS_RENDER|IRF_WHITE_WIREFRAME; // Default flags
	m_pMaterial=NULL;
}

IMesh::~IMesh()
{
	m_aObjects.DeleteItem(this);
	m_pDevice->_DeleteList(m_iDisplayListSolid);
	m_pDevice->_DeleteList(m_iDisplayListWireframe);
	m_pDevice->_DeleteList(m_iDisplayListNormal);
}

void IMesh::AddChild(IMesh *child)
{
	if (child)
		m_aChildren.Add(child);
}

int IMesh::GetChildCount()
{
	return m_aChildren.Count();
}

IMesh* IMesh::GetChild(int i)
{
	if (i<m_aChildren.Count())
		return m_aChildren[i];
	return NULL;
}

IMesh* IMesh::GetParent()
{
	return m_pParent;
}

void IMesh::SetParent(IMesh* parent)
{
	m_pParent=parent;
}

void IMesh::Render(IRenderDriver *Device, unsigned int Flags)
{
	IRenderDriver *dev;
	if (Device)
		dev=Device;
	else
		dev=m_pDevice;

	unsigned int flags=m_iFlags;
	if (Flags)
		m_iFlags=Flags;	

	if (dev)
	{
		/*RF_ALWAYS_RENDER
		RF_SHOW_BOUNDING_BOX
		RF_SHOW_NORMAL
		RF_WIREFRAME
		RF_WIREFRAME_ONLY*/
		dev->_SetModelViewMatrix();

		dev->_PushMatrix();

			IMatrix ScaleMat(4);
			ScaleMat.MakeIdentity();
			ScaleMat[0][0]=ScaleMat[1][1]=ScaleMat[2][2]=m_fScale;

			dev->_MultMatrix(m_mOrigPosition);
			dev->_MultMatrix(m_mPosition);
			dev->_MultMatrix(ScaleMat);			

			//Solid
			if (!GetFlag(IRF_WIREFRAME_ONLY))
			{
				if (!GetFlag(IRF_SHADOW_MAP_RENDER))
				{
					dev->_SetColor(m_cColor);
					dev->UseMaterial(m_pMaterial);
				}
				if (m_iDisplayListSolid)
				{
					dev->_CallList(m_iDisplayListSolid);
				}else{
					if (m_iDisplayListSolid==0)
						m_iDisplayListSolid = dev->_NewList();

					dev->_StartList(m_iDisplayListSolid,true);
						for (int i=0;i<m_aPolygons.Count();i++)
						{
							dev->_StartTriangleDraw(false);
								dev->_DrawTriangle(&m_aPolygons[i]);
							dev->_EndTriangleDraw();
						}
					dev->_EndList();
				}
			}
			
			//Wireframe
			if (GetFlag(IRF_WIREFRAME|IRF_WIREFRAME_ONLY))
			{
				if (GetFlag(IRF_WHITE_WIREFRAME))
				{
					dev->_SetColor(IColor(1,1,1));
					dev->UseMaterial(NULL);
				}else{
					dev->UseMaterial(m_pMaterial);
					dev->_SetColor(m_cColor);
				}
				dev->_SetLineSize(1.0);

				if (m_iDisplayListWireframe)
				{
					dev->_CallList(m_iDisplayListWireframe);
				}else{
					if (m_iDisplayListWireframe==0)
						m_iDisplayListWireframe = dev->_NewList();

					dev->_StartList(m_iDisplayListWireframe,true);
						for (int i=0;i<m_aPolygons.Count();i++)
						{							
							dev->_StartTriangleDraw(true);
								dev->_DrawTriangle(&m_aPolygons[i]);
							dev->_EndTriangleDraw();
						}
					dev->_EndList();
				}
			}

			//Normals / Tangent / Binormal
			if (GetFlag(IRF_SHOW_NORMAL))
			{
				dev->_SetLineSize(1.0);
				if (m_iDisplayListNormal)
				{
					dev->_CallList(m_iDisplayListNormal);
				}else{
					if (m_iDisplayListNormal==0)
						m_iDisplayListNormal = dev->_NewList();

					dev->_StartList(m_iDisplayListNormal,true);
						dev->UseMaterial(NULL);
						IVector3D *vertex;
						IVector3D *normal;
						for (int i=0;i<m_aPolygons.Count();i++)
						{
							vertex=m_aPolygons[i].GetVertex();
							normal=m_aPolygons[i].GetNormals();
							IVector3D center = PolygonCenter(m_aPolygons[i]);
							center = m_aPolygons[i].GetVertex()[0];
							dev->_SetColor(IColor(1,0,0));
							dev->_DrawLine(center,center+normal[0]);
							/*dev->_SetColor(IColor(0,1,0));
							dev->_DrawLine(center,center+normal[1]);
							dev->_SetColor(IColor(0,0,1));
							dev->_DrawLine(center,center+normal[2]);*/
						}
					dev->_EndList();
				}
			}

			// Bounding box
			if (GetFlag(IRF_SHOW_BOUNDING_BOX))
			{
				dev->_SetLineSize(1.0);
				dev->UseMaterial(NULL);
				m_BBox.Render(dev);
			}

			unsigned int flagsChild=m_iFlags;
			if (!GetFlag(IRF_SHOW_CHILDREN_BOUNDING_BOX))
				flagsChild=flagsChild&~IRF_SHOW_BOUNDING_BOX;

			for (int i=0;i<m_aChildren.Count();i++)
				m_aChildren[i]->Render(dev,flagsChild);
		
		dev->_PopMatrix();
	}
	m_iFlags=flags;
}

void IMesh::SetFlags(unsigned int flag,bool enable) 
{ 
	m_iFlags = enable ? m_iFlags|flag : m_iFlags&~flag; 
}

bool IMesh::GetFlag(unsigned int flag) 
{ 
	return m_iFlags&flag ? true : false; 
}

void IMesh::AddPolygon(ITriangle pol) 
{
	m_aPolygons.Add(pol);
}

void IMesh::SetMaterial(IMaterial *mat, bool ToChildren)
{
	m_pMaterial = mat;
	if (ToChildren)
	{
		for (int i=0;i<m_aChildren.Count();i++)
			if (m_aChildren[i])
				m_aChildren[i]->SetMaterial(mat,true);
	}
}

void IMesh::SetColor(IColor color, bool ToChildren)
{
	m_cColor=color;
	if (ToChildren)
	{
		for (int i=0;i<m_aChildren.Count();i++)
			if (m_aChildren[i])
				m_aChildren[i]->SetColor(color);
	}
}

IMaterial* IMesh::GetMaterial()
{
	return m_pMaterial;
}

IColor IMesh::GetColor()
{
	return m_cColor;
}

void IMesh::Translate(float x, float y, float z)
{
	m_mPosition*=TranslationToMatrix(x,y,z);;
}

void IMesh::RotateX(float x)
{
	m_mPosition*=AngleXToMatrix4x4(x);
}

void IMesh::RotateY(float y)
{
	m_mPosition*=AngleYToMatrix4x4(y);
}

void IMesh::RotateZ(float z)
{
	m_mPosition*=AngleZToMatrix4x4(z);
}

void IMesh::InitPosition()
{
	m_mPosition.MakeIdentity();
}

void IMesh::TranslateOrig(float x, float y, float z)
{
	m_mOrigPosition*=TranslationToMatrix(x,y,z);
}

void IMesh::RotateOrigX(float x)
{
	m_mOrigPosition*=AngleXToMatrix4x4(x);
}

void IMesh::RotateOrigY(float y)
{
	m_mOrigPosition*=AngleYToMatrix4x4(y);
}

void IMesh::RotateOrigZ(float z)
{
	m_mOrigPosition*=AngleZToMatrix4x4(z);
}

void IMesh::InitOrigPosition()
{
	m_mPosition.MakeIdentity();
}

void IMesh::SetScale(float scale)
{
	m_fScale=scale;
}

void IMesh::CalculateBBox(bool WithChild)
{
	float minx,miny,minz,maxx,maxy,maxz;
	if (m_aPolygons.Count())
	{
		minx=(m_aPolygons[0].GetVertex()[0])[0];
		miny=(m_aPolygons[0].GetVertex()[0])[1];
		minz=(m_aPolygons[0].GetVertex()[0])[2];

		maxx=(m_aPolygons[0].GetVertex()[0])[0];
		maxy=(m_aPolygons[0].GetVertex()[0])[1];
		maxz=(m_aPolygons[0].GetVertex()[0])[2];
		
		for (int i=0;i<m_aPolygons.Count();i++)
		{
			for (int j=0;j<3;j++)
			{
				if (minx>(m_aPolygons[i].GetVertex()[j])[0])
					minx=(m_aPolygons[i].GetVertex()[j])[0];
				if (miny>(m_aPolygons[i].GetVertex()[j])[1])
					miny=(m_aPolygons[i].GetVertex()[j])[1];
				if (minz>(m_aPolygons[i].GetVertex()[j])[2])
					minz=(m_aPolygons[i].GetVertex()[j])[2];

				if (maxx<(m_aPolygons[i].GetVertex()[j])[0])
					maxx=(m_aPolygons[i].GetVertex()[j])[0];
				if (maxy<(m_aPolygons[i].GetVertex()[j])[1])
					maxy=(m_aPolygons[i].GetVertex()[j])[1];
				if (maxz<(m_aPolygons[i].GetVertex()[j])[2])
					maxz=(m_aPolygons[i].GetVertex()[j])[2];
			}
		}
		m_BBox.SetBBox(minx,miny,minz,maxx,maxy,maxz);
	}

	if (WithChild)
	{
		if (m_aChildren.Count())
		{
			for (int i=0;i<m_aChildren.Count();i++)
				m_aChildren[i]->CalculateBBox(true);

			IBBox maxbox;
			int start=0;
			if (m_aPolygons.Count()==0)
			{
				maxbox=m_aChildren[0]->GetBBox();
				start=1;
			}else
				maxbox=m_BBox;
				
			for (int i=start;i<m_aChildren.Count();i++)
				maxbox=IBBox::MaxBBox(maxbox,m_aChildren[i]->GetBBox());

			m_BBox=maxbox;
		}
	}
}

IBBox IMesh::GetBBox()
{
	return m_BBox;
}

void IMesh::CalcNormals()
{
	int i;
	IVector3D l_vect1,l_vect2,l_vect3,l_vect_b1,l_vect_b2,l_normal;
	//IArray<int> l_connections_qty;

	for (i=0; i<m_aPolygons.Count(); i++)
	{
		l_vect1 = m_aPolygons[i].GetVertex()[0];
		l_vect2 = m_aPolygons[i].GetVertex()[1];
		l_vect3 = m_aPolygons[i].GetVertex()[2];
		
		l_vect_b1=(l_vect3-l_vect2);
		l_vect_b1.Normalize();
		l_vect_b2=(l_vect1-l_vect2);
		l_vect_b2.Normalize();

		//VectDotProduct (&l_vect_b1, &l_vect_b2, &l_normal);

		l_normal = IVector3D::Dot(l_vect_b1,l_vect_b2);
		//VectNormalize (&l_normal);
		l_normal.Normalize();

		//m_aPolygons[i].SetNormal(0,l_normal);
		//m_aPolygons[i].SetNormal(1,l_normal);
		//m_aPolygons[i].SetNormal(2,l_normal);
	} 

	/*for (i=0; i<p_object->vertices_qty; i++)
	{
		if (l_connections_qty[i]>0)
		{
			p_object->normal[i].x /= l_connections_qty[i];
			p_object->normal[i].y /= l_connections_qty[i];
			p_object->normal[i].z /= l_connections_qty[i];
		}
	}*/

}

