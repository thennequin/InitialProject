//---------------------------------------------
//                Initial Engine
//             Smart Pointer Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISMARTPTR_HEADER_
#define _ISMARTPTR_HEADER_

#include "Initial/IObject.h"

namespace Initial
{
	class IObject;

	template<class T>
	class ISmartPtr
	{
		friend class IObject;
	public:
		ISmartPtr()
		{
			myId=IObject::nextId++;
			m_pPtr=0;
		}

		ISmartPtr(T *ptr)
		{
			myId=IObject::nextId++;
			m_pPtr=0;
			SetPtr(ptr);
		}
		
		~ISmartPtr()
		{
			SetPtr(NULL);
		}

		void operator = (ISmartPtr<T>& ptr)
		{
			SetPtr(ptr.GetPtr());
		}

		void operator = (const T *ptr) const
		{
			SetPtr(ptr);
		}

		void operator = (T *ptr)
		{
			SetPtr(ptr);
		}

		operator T*() const
		{
			return GetPtr();
		}

		operator T*()
		{
			return GetPtr();
		}

		T* operator ->()
		{
			return GetPtr();
		}
		void SetPtr(T *ptr)
		{
			if (m_pPtr)
				m_pPtr->Release((ISmartPtr<IObject>*)this);
			m_pPtr=ptr;
			if (m_pPtr)
				m_pPtr->AddRef((ISmartPtr<IObject>*)this);
		}

		T* GetPtr()
		{
			return m_pPtr;
		}
	protected:		
		T *m_pPtr;

		int myId;
	};
};

#endif