//---------------------------------------------
//                Initial Engine
//                Object  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IOBJECT_HEADER_
#define _IOBJECT_HEADER_

#include "Initial/ILogger.h"
#include "IObject_MACRO.h"

#include "Initial/Core/IString.h"
#include "Initial/Core/IList.h"
#include "Initial/Core/IArray.h"
#include "ISmartPtr.h"
#include "Initial/IProperty.h"

DECLARE_OBJECT_INITIATOR()

namespace Initial
{
	class IObject
	{
	public:
		static int nextId;
		static int nextIdObj;
		class IObjectInfo
		{
		public:
			virtual IObject *CreateInstance()=0;

			bool IsKindOf(Initial::IObject::IObjectInfo *base);
			Core::IString GetName();
			IObjectInfo* GetBaseClass();
		protected:
			Core::IString m_sClassname;
			IObjectInfo *m_pBaseClass;
		};

		friend class IRessourceManager;
	public:
		IObject();
		virtual ~IObject()/*=0*/;

		void AddRef(ISmartPtr<IObject> *ptr);
		bool Release(ISmartPtr<IObject> *ptr);

		bool IsKindOf(Initial::IObject::IObjectInfo *base);
		static IObjectInfo *GetStaticClass();
		virtual IObjectInfo *GetClass();

		void AddProperty(Core::IString Name,PropertyType Type,void *var=NULL,Core::IList<Core::IString>& choices=Core::IList<Core::IString>());
		IPropertyList& GetPropertyList();
		IProperty *GetProperty(Core::IString name);
		void SetPropertyValue(Core::IString Name,Core::IString Value);
		virtual void OnPropertyChange(Core::IString name);

		IObject::IObjectInfo* GetStaticClassByName(Core::IString classname);
	protected:
		int m_iObjId;
		bool m_bInDestroy;
		unsigned int m_iReferenceCount;
		Core::IArray<ISmartPtr<IObject>*> m_References;

		IPropertyList m_aProperties;
	public:
		static Core::IArray<IObject::IObjectInfo*> ObjectsClass;
	};
};

#endif