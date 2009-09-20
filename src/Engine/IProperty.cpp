
#include "Initial/IProperty.h"

#include "Initial/Core/IUtil.h"
#include "Initial/Core/IVector2D.h"
#include "Initial/Core/IVector3D.h"
#include "Initial/Core/IColor.h"
//#include "Initial/Format/IImageITX.h"
#include "Initial/Ressource/ITexture.h"

using namespace Initial::Core;
using namespace Initial::Video;

namespace Initial
{
	//------------------------------------------------------------------
	// IProperty
	//------------------------------------------------------------------
	IProperty::IProperty(IString name, PropertyType type, void *affected, bool editable, Core::IList<Core::IString>& choices)
	{
		m_bEditable=editable;
		m_sName=name;
		m_iType=type;
		m_pVar=affected;
		m_pUserData=NULL;
		m_bInternalVar=false;
		m_aChoices=choices;
		if (m_pVar==NULL)
		{
			m_bInternalVar=true;
			switch (m_iType)
			{
			case IPT_BOOL:
				m_pVar = new bool(false);
				break;
			case IPT_INTEGER:
				m_pVar = new int(0);
				break;
			case IPT_FLOAT:
				m_pVar = new float(0.0);
				break;
			case IPT_VEC2:
				m_pVar = new IVector2D;
				break;
			case IPT_VEC3:
				m_pVar = new IVector3D;
				break;
			case IPT_COLOR:
				m_pVar = new IColor;
				break;
			case IPT_STRING:
			case IPT_LONG_STRING:
				m_pVar = new IString("");
				break;
			case IPT_CHOICE:	
				m_pVar = new int(0);
				break;
			case IPT_RESSOURCE_ITX: // Texture
				//m_pVar = new ISmartPtr<ITexture>;
				m_pVar = new IString("");
				break;
			case IPT_RESSOURCE_IMA: // Material
				break;
			case IPT_RESSOURCE_IEM: // Animated mesh
				break;
			}
		}
	}

	IProperty::~IProperty()
	{
		if (m_bInternalVar && m_pVar)
		{
			delete m_pVar;
		}
	}

	PropertyType IProperty::GetType()
	{
		return m_iType;
	}

	IString IProperty::GetName()
	{
		return m_sName;
	}

	IString IProperty::GetString()
	{
		if (m_pVar)
		{
			switch (m_iType)
			{
			case IPT_BOOL:
				return *((bool*)m_pVar)==true?"true":"false";
				break;
			case IPT_INTEGER:
				return IString(*((int*)m_pVar));
				break;
			case IPT_FLOAT:
				return TrimZeroFloat(IString(*((float*)m_pVar)));
				break;
			case IPT_VEC2:
				{
					IString str;
					IVector2D *vect = ((IVector2D*)m_pVar);
					str.Printf("%f;%f",vect->GetU(),vect->GetV());
					return str;
				}
				break;
			case IPT_VEC3:
				{
					IString str;
					IVector3D *vect = ((IVector3D*)m_pVar);
					str.Printf("%f;%f;%f",vect->GetX(),vect->GetY(),vect->GetZ());
					return str;
				}
				break;
			case IPT_COLOR:
				{
					IString str;
					IColor *color = ((IColor*)m_pVar);
					str.Printf("%f;%f;%f;%f",color->GetRed(),color->GetGreen(),color->GetBlue(),color->GetAlpha());
					return str;
				}
				break;
			case IPT_STRING:
			case IPT_LONG_STRING:
				return *((IString*)m_pVar);
				break;
			case IPT_CHOICE:
					return *((int*)m_pVar);
				//return IString(*((int*)m_pVar));
				break;
			case IPT_RESSOURCE_ITX: // Texture
				//if (((ISmartPtr<ITexture>*)m_pVar))
					//return (*(ISmartPtr<ITexture>*)m_pVar)->GetFilename();
				//return "";
				
				//return *((IString*)m_pVar);
				return Core::RelativeToPath(Core::AppPath(),*((IString*)m_pVar));
				break;
			case IPT_RESSOURCE_IMA: // Material
				break;
			case IPT_RESSOURCE_IEM: // Animated mesh
				break;
			}
		}
		return "";
	}

	float IProperty::GetFloat()
	{
		if (m_pVar)
		{
			switch (m_iType)
			{
			case IPT_BOOL:
				return *((bool*)m_pVar);
				break;
			case IPT_INTEGER:
				return *((int*)m_pVar);
				break;
			case IPT_FLOAT:
				return *((float*)m_pVar);
				break;
			case IPT_VEC2:
				return ((IVector2D*)m_pVar)->x;
				break;
			case IPT_VEC3:
				return ((IVector3D*)m_pVar)->x;
				break;
			case IPT_COLOR:
				return ((IColor*)m_pVar)->r;
			case IPT_CHOICE:
				return *((int*)m_pVar);
			}
		}
		return 0.0;
	}

	Core::IColor IProperty::GetColor()
	{
		if (m_pVar)
		{
			float temp;
			switch (m_iType)
			{
			case IPT_BOOL:
				temp = *((bool*)m_pVar);
				return IColor(temp,temp,temp,temp);
			case IPT_INTEGER:
				temp = *((int*)m_pVar);
				return IColor(temp,temp,temp,temp);
			case IPT_FLOAT:
				temp = *((float*)m_pVar);
				return IColor(temp,temp,temp,temp);
			case IPT_VEC2:
				{
					IVector2D temp = (*(IVector2D*)m_pVar);
					return IColor(temp.x,temp.y,0,0);
				}
			case IPT_VEC3:
				{
					IVector3D temp = (*(IVector3D*)m_pVar);
					return IColor(temp.x,temp.y,temp.z,0);
				}
			case IPT_COLOR:
				return (*(IColor*)m_pVar);
			case IPT_CHOICE:
				return IColor(*((int*)m_pVar),0,0,0);
			}
		}
		return IColor(0,0,0,0);
	}

	void *IProperty::GetVar()
	{
		return m_pVar;
	}

	Core::IList<Core::IString>& IProperty::GetChoices()
	{
		return m_aChoices;
	}

	void IProperty::SetValueFromString(IString value)
	{
		if (m_pVar)
		{
			switch (m_iType)
			{
			case IPT_BOOL:
				*((bool*)m_pVar)=BoolFromString(value);
				break;
			case IPT_INTEGER:
				*((int*)m_pVar)=Iatof(value);
				break;
			case IPT_FLOAT:
				*((float*)m_pVar)=Iatof(value);
				break;
			case IPT_VEC2:
				*((IVector2D*)m_pVar)=Vector2DFromString(value);
				break;
			case IPT_VEC3:
				*((IVector3D*)m_pVar)=Vector3DFromString(value);
				break;
			case IPT_COLOR:
				*((IColor*)m_pVar)=ColorFromString(value);
				break;
			case IPT_STRING:
			case IPT_LONG_STRING:
				*((IString*)m_pVar)=value;
				break;
			case IPT_CHOICE:			
				*((int*)m_pVar)=Iatof(value);
				break;
			case IPT_RESSOURCE_ITX: // Texture
				//if (m_pMaterial)
				//m_pObject->getde
				//(ISmartPtr<ITexture>*)m_pVar)
				//(ISmartPtr<ITexture>*)m_pVar) = m_pDevice->getress
				//((Format::IImageITX*)m_pVar);
				*((IString*)m_pVar)=value;
				break;
			case IPT_RESSOURCE_IMA: // Material
				break;
			case IPT_RESSOURCE_IEM: // Animated mesh
				break;
			}
		}
	}

	//------------------------------------------------------------------
	// IPropertyList
	//------------------------------------------------------------------
	IPropertyList::~IPropertyList()
	{
		for (UINT i=0;i<m_aProperties.Count();i++)
		{
			if (m_aProperties[i])
				delete m_aProperties[i];
		}
	}

	void IPropertyList::AddProperty(Core::IString& Name,PropertyType Type,void *var, Core::IList<Core::IString>& choices)
	{
		m_aProperties.Add(new IProperty(Name,Type,var,true,choices));
	}

	IProperty *IPropertyList::GetProperty(Core::IString Name)
	{
		for (UINT i=0;i<m_aProperties.Count();i++)
		{
			if (m_aProperties[i] && m_aProperties[i]->GetName()==Name)
				return m_aProperties[i];
		}
		return NULL;
	}

	int IPropertyList::Count()
	{ 
		return m_aProperties.Count(); 
	}

	IProperty *IPropertyList::GetProperty(int i) 
	{ 
		return m_aProperties[i];
	}

	//void GeneratePropertyGrid(wxPropertyGrid *grid);
	Core::IArray<IProperty*> m_aProperties;
}
