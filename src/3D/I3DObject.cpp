//---------------------------------------------
//                Initial Engine
//            3D Object source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/I3DObject.h"
#include "Core/IMath.h"

using namespace Initial;

IArray<I3DObject*> I3DObject::m_aObjects;

I3DObject::I3DObject(IRenderDriver *Device, I3DObject* parent)
{
	_Init(Device, parent);
}

I3DObject::I3DObject(IRenderDriver *Device, IArray<ITriangle> polygons, I3DObject* parent)
{
	_Init(Device, parent);
	m_aPolygons = polygons;
}

void I3DObject::_Init(IRenderDriver *Device, I3DObject* parent)
{
	m_pParent=parent;
	m_pDevice = Device;	
	m_aObjects.Add(this);
	m_mPosition.MakeIdentity(4);
	m_mOrigPosition.MakeIdentity(4);
	m_fScale=1.0;
	m_iDisplayListSolid=m_iDisplayListWireframe=m_iDisplayListNormal=0;
	m_bUpdated=false;
	m_iFlags=RF_ALWAYS_RENDER|RF_WHITE_WIREFRAME; // Default flags
	m_pMaterial=NULL;
}

I3DObject::~I3DObject()
{
	m_aObjects.DeleteItem(this);
	m_pDevice->_DeleteList(m_iDisplayListSolid);
	m_pDevice->_DeleteList(m_iDisplayListWireframe);
	m_pDevice->_DeleteList(m_iDisplayListNormal);
}

void I3DObject::AddChild(I3DObject *child)
{
	if (child)
		m_aChildren.Add(child);
}

I3DObject* I3DObject::GetParent()
{
	return m_pParent;
}

void I3DObject::SetParent(I3DObject* parent)
{
	m_pParent=parent;
}

void I3DObject::Render(IRenderDriver *Device, unsigned int Flags)
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
			if (!GetFlag(RF_WIREFRAME_ONLY))
			{		
				dev->_SetColor(m_cColor);
				dev->UseMaterial(m_pMaterial);
				if (m_iDisplayListSolid)
				{
					dev->_CallList(m_iDisplayListSolid);
				}else{
					if (m_iDisplayListSolid==0)
						m_iDisplayListSolid = dev->_NewList();

					dev->_StartList(m_iDisplayListSolid,true);
						for (int i=0;i<m_aPolygons.Count();i++)
						{
							CalcultatePolygonNormals(m_aPolygons[i]);
							dev->_StartPolyDraw(false);
								dev->_DrawPolygon(&m_aPolygons[i]);
							dev->_EndPolyDraw();
						}
					dev->_EndList();
				}
			}
			
			//Wireframe
			if (GetFlag(RF_WIREFRAME|RF_WIREFRAME_ONLY))
			{
				if (GetFlag(RF_WHITE_WIREFRAME))
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
							dev->_StartPolyDraw(true);
								dev->_DrawPolygon(&m_aPolygons[i]);
							dev->_EndPolyDraw();
						}
					dev->_EndList();
				}
			}

			//Normals / Tangent / Binormal
			if (GetFlag(RF_SHOW_NORMAL))
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
						IVertex *vertex;
						IVertex *normal;
						for (int i=0;i<m_aPolygons.Count();i++)
						{
							vertex=m_aPolygons[i].GetVertex();
							normal=m_aPolygons[i].GetNormals();
							IVertex center = PolygonCenter(m_aPolygons[i]);
							center = m_aPolygons[i].GetVertex()[0];
							dev->_SetColor(IColor(1,0,0));
							dev->_DrawLine(center,center+normal[0]);
							dev->_SetColor(IColor(0,1,0));
							dev->_DrawLine(center,center+normal[1]);
							dev->_SetColor(IColor(0,0,1));
							dev->_DrawLine(center,center+normal[2]);
						}
					dev->_EndList();
				}
			}

			// Bounding box
			if (GetFlag(RF_SHOW_BOUNDING_BOX))
			{
				dev->_SetLineSize(1.0);
				dev->UseMaterial(NULL);
				m_BBox.Render(dev);
			}

			unsigned int flagsChild=m_iFlags;
			if (!GetFlag(RF_SHOW_CHILDREN_BOUNDING_BOX))
				flagsChild=flagsChild&~RF_SHOW_BOUNDING_BOX;

			for (int i=0;i<m_aChildren.Count();i++)
				m_aChildren[i]->Render(dev,flagsChild);
		
		dev->_PopMatrix();
	}
	m_iFlags=flags;
}

void I3DObject::SetFlags(unsigned int flag,bool enable) 
{ 
	m_iFlags = enable ? m_iFlags|flag : m_iFlags&~flag; 
}

bool I3DObject::GetFlag(unsigned int flag) 
{ 
	return m_iFlags&flag ? true : false; 
}

void I3DObject::AddPolygon(ITriangle pol) 
{
	m_aPolygons.Add(pol);
}

void I3DObject::SetMaterial(IMaterial *mat, bool ToChildren)
{
	m_pMaterial = mat;
	if (ToChildren)
	{
		for (int i=0;i<m_aChildren.Count();i++)
			if (m_aChildren[i])
				m_aChildren[i]->SetMaterial(mat,true);
	}
}

void I3DObject::SetColor(IColor color, bool ToChildren)
{
	m_cColor=color;
	if (ToChildren)
	{
		for (int i=0;i<m_aChildren.Count();i++)
			if (m_aChildren[i])
				m_aChildren[i]->SetColor(color);
	}
}

IMaterial* I3DObject::GetMaterial()
{
	return m_pMaterial;
}

IColor I3DObject::GetColor()
{
	return m_cColor;
}

void I3DObject::Translate(float x, float y, float z)
{
	m_mPosition*=TranslationToMatrix(x,y,z);;
}

void I3DObject::RotateX(float x)
{
	m_mPosition*=AngleXToMatrix(x);
}

void I3DObject::RotateY(float y)
{
	m_mPosition*=AngleYToMatrix(y);
}

void I3DObject::RotateZ(float z)
{
	m_mPosition*=AngleZToMatrix(z);
}

void I3DObject::InitPosition()
{
	m_mPosition.MakeIdentity();
}

void I3DObject::TranslateOrig(float x, float y, float z)
{
	m_mOrigPosition*=TranslationToMatrix(x,y,z);
}

void I3DObject::RotateOrigX(float x)
{
	m_mOrigPosition*=AngleXToMatrix(x);
}

void I3DObject::RotateOrigY(float y)
{
	m_mOrigPosition*=AngleYToMatrix(y);
}

void I3DObject::RotateOrigZ(float z)
{
	m_mOrigPosition*=AngleZToMatrix(z);
}

void I3DObject::InitOrigPosition()
{
	m_mPosition.MakeIdentity();
}

void I3DObject::SetScale(float scale)
{
	m_fScale=scale;
}

void I3DObject::CalculateBBox(bool WithChild)
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

IBBox I3DObject::GetBBox()
{
	return m_BBox;
}