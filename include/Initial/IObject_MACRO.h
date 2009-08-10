//---------------------------------------------
//                Initial Engine
//             Object Macro  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IOBJECTMACRO_HEADER_
#define _IOBJECTMACRO_HEADER_

#define DECLARE_OBJECT_INITIATOR()																	\
	namespace Initial {																				\
		void InitObject(); }																		\
	class ObjectInitiator																			\
	{ public:																						\
		virtual void Init(){};																		\
		static Initial::Core::IArray<ObjectInitiator*> Objects;										\
	};

#define IMPLEMENT_OBJECT_INITIATOR()																\
	Initial::Core::IArray<ObjectInitiator*> ObjectInitiator::Objects;								\
	Initial::Core::IArray<Initial::IObject::IObjectInfo*> Initial::IObject::ObjectsClass;			\
	void Initial::InitObject()																		\
	{																								\
		/*if (Inited==false)*/{																			\
			int count = ObjectInitiator::Objects.Count();											\
			for (int i=0;i<count;i++)																\
				ObjectInitiator::Objects[i]->Init();												\
		}																							\
	}

#define IMPLEMENT_OBJECT_INIT(ObjectClass)															\
	class /*Initial::*/ObjectClass::ObjectClass##_Init : public ObjectInitiator						\
	{																								\
	public:																							\
	ObjectClass##_Init() { ObjectInitiator::Objects.Add(this); }									\
		void Init() { /*Initial::*/ObjectClass::_Init(); }											\
	};																								\
	const /*Initial::*/ObjectClass::ObjectClass##_Init ObjectClass##_Global;


//----------------------------------------------------------------
// Declare object class
//----------------------------------------------------------------
#define DECLARE_OBJECT(ObjectClass, baseClass)																	\
	friend class ObjectInitiator;																	\
	friend class ObjectClass##_Init;																\
	typedef baseClass BaseClass;																	\
	public:																							\
	static Initial::IObject::IObjectInfo *m_pObjectInfo;											\
	class IObjectInfo_##ObjectClass;																\
	class ObjectClass##_Init;																		\
	static Initial::IObject::IObjectInfo *GetStaticClass();											\
	virtual Initial::IObject::IObjectInfo *GetClass();												\
	protected:																						\
	static void _Init();

//----------------------------------------------------------------
// Object class
//----------------------------------------------------------------
#define IMPLEMENT_OBJECT_INFO(ObjectClass, BaseClass)												\
	class ObjectClass::IObjectInfo_##ObjectClass : public Initial::IObject::IObjectInfo				\
	{																								\
	public:																							\
		IObjectInfo_##ObjectClass ()																\
		{																							\
			m_sClassname = #ObjectClass ;															\
			m_pBaseClass = BaseClass::GetStaticClass();												\
		}																							\
		virtual Initial::IObject *CreateInstance()													\
		{																							\
			return new /*Initial::*/ObjectClass ;													\
		}																							\
	};

#define IMPLEMENT_OBJECT(ObjectClass, BaseClass)													\
	IMPLEMENT_OBJECT_INFO(ObjectClass, BaseClass)													\
	Initial::IObject::IObjectInfo* /*Initial::*/ObjectClass::m_pObjectInfo=NULL;					\
	void /*Initial::*/ObjectClass::_Init()															\
	{																								\
		if (m_pObjectInfo==NULL)																	\
			m_pObjectInfo = new IObjectInfo_##ObjectClass ;											\
		Initial::IObject::ObjectsClass.Add(ObjectClass::m_pObjectInfo);								\
	}																								\
	Initial::IObject::IObjectInfo */*Initial::*/ObjectClass::GetStaticClass()						\
	{																								\
		return m_pObjectInfo;																		\
	}																								\
	Initial::IObject::IObjectInfo */*Initial::*/ObjectClass::GetClass()								\
	{																								\
		return m_pObjectInfo;																		\
	}																								\
	IMPLEMENT_OBJECT_INIT(ObjectClass)

//----------------------------------------------------------------
// Abstract object class
//----------------------------------------------------------------
#define IMPLEMENT_ABSTRACT_OBJECT_INFO(ObjectClass, BaseClass)										\
	class ObjectClass::IObjectInfo_##ObjectClass : public Initial::IObject::IObjectInfo				\
	{																								\
	public:																							\
		IObjectInfo_##ObjectClass ()																\
		{																							\
			m_sClassname = #ObjectClass ;															\
			m_pBaseClass = BaseClass::GetStaticClass();												\
		}																							\
		virtual Initial::IObject *CreateInstance()													\
		{																							\
			return NULL;																			\
		}																							\
	};

#define IMPLEMENT_ABSTRACT_OBJECT(ObjectClass, BaseClass)											\
	IMPLEMENT_ABSTRACT_OBJECT_INFO(ObjectClass, BaseClass)											\
	Initial::IObject::IObjectInfo* /*Initial::*/ObjectClass::m_pObjectInfo=NULL;					\
	void /*Initial::*/ObjectClass::_Init()															\
	{																								\
		if (m_pObjectInfo==NULL)																	\
		m_pObjectInfo = new IObjectInfo_##ObjectClass ;												\
		Initial::IObject::ObjectsClass.Add(ObjectClass::m_pObjectInfo);								\
	}																								\
	Initial::IObject::IObjectInfo */*Initial::*/ObjectClass::GetStaticClass()						\
	{																								\
		return m_pObjectInfo;																		\
	}																								\
	Initial::IObject::IObjectInfo */*Initial::*/ObjectClass::GetClass()								\
	{																								\
		return m_pObjectInfo;																		\
	}																								\
	IMPLEMENT_OBJECT_INIT(ObjectClass)

#endif