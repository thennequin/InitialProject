
//---------------------------------------------
//                Initial Engine
//             Object source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/ILogger.h"
#include "Initial/IObject.h"

//#include "Initial/Core/IRessourceManager.h"

IMPLEMENT_OBJECT_INITIATOR()

namespace Initial
{
	int IObject::nextId=0;
	int IObject::nextIdObj=0;

	IObject::IObject()
		: m_iReferenceCount(0), m_bInDestroy(false)
	{
		m_iObjId=nextIdObj++;
	}

	IObject::~IObject() /*= 0;*/
	{
		m_bInDestroy=true;
		for (UINT i=0;i<m_References.Count();i++)
		{
			if (m_References[i])
			{
				m_References[i]->SetPtr(NULL);
			}
		}
		//printf("Destroy object\n");
		m_iReferenceCount;
		//ILogger::LogWarning("Destroy Object\n");
	}
	
	void IObject::AddRef(ISmartPtr<IObject> *ptr)
	{
		//ILogger::LogDebug("AddRef %d %d\n",m_iObjId, ptr->myId);
		/*if (GetClass() && GetClass()!=IObject::GetStaticClass())
			ILogger::LogDebug("%s\n",GetClass()->GetName().c_str());*/

		if (ptr!=NULL && m_bInDestroy==false)
		{
			m_References.Add(ptr);
			m_iReferenceCount++;
			//ILogger::LogDebug("\tAddRef Ok %d\n",m_iReferenceCount);
		}
	}

	bool IObject::Release(ISmartPtr<IObject> *ptr)
	{
		//ILogger::LogDebug("ReleaseRef %d %d\n",m_iObjId,ptr->myId);
		if (m_bInDestroy==false)
		{
			if (m_References.DeleteItem(ptr))
			{
				m_iReferenceCount--;
				//ILogger::LogDebug("\tReleaseRef Ok %d\n",m_iReferenceCount);

				if (m_iReferenceCount==0)
				{
					//delete this;
					return true;
				}
			}
			return false;
		}
		return true;
	}

	IObject::IObjectInfo *IObject::GetStaticClass()
	{
		return NULL;
	}

	IObject::IObjectInfo *IObject::GetClass()
	{
		return NULL;
	}

	bool IObject::IsKindOf(IObject::IObjectInfo *base)
	{
		return base!=0 && (GetClass() == base ||
					(GetClass() && GetClass()->GetBaseClass() && 
					 GetClass()->GetBaseClass()->IsKindOf(base) ) );
	}

	void IObject::AddProperty(Core::IString Name,PropertyType Type,void *var, Core::IList<Core::IString>& choices)
	{
		m_aProperties.AddProperty(Name,Type,var,choices);
	}

	IPropertyList& IObject::GetPropertyList()
	{ 
		return m_aProperties;
	}

	void IObject::SetPropertyValue(Core::IString Name,Core::IString Value)
	{
		IProperty *prop = m_aProperties.GetProperty(Name);

		if (prop)
		{
			prop->SetValueFromString(Value);
			OnPropertyChange(Name);
		}
	}

	void IObject::OnPropertyChange(Core::IString name)
	{

	}

	IObject::IObjectInfo* IObject::GetStaticClassByName(Core::IString classname)
	{
		for (UINT i=0;i<IObject::ObjectsClass.Count();i++)
		{
			if (IObject::ObjectsClass[i]
			&& IObject::ObjectsClass[i]->GetName()==classname)
			{
				return IObject::ObjectsClass[i];
			}
		}
		return NULL;
	}

	//-----------------------------------------------------------------------
	// IObjectInfo
	//-----------------------------------------------------------------------
	bool IObject::IObjectInfo::IsKindOf(IObject::IObjectInfo *base)
	{
		return base!=0 && (this == base ||
					(m_pBaseClass && m_pBaseClass->IsKindOf(base) ) );
	}

	Core::IString IObject::IObjectInfo::GetName()
	{
		return m_sClassname;
	}

	IObject::IObjectInfo* IObject::IObjectInfo::GetBaseClass()
	{
		return m_pBaseClass;
	}
	
};
