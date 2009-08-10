//---------------------------------------------
//                Initial Engine
//                Object  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IRESSOURCE_HEADER_
#define _IRESSOURCE_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"
#include "Initial/IObject.h"
#include "Initial/Thread/IThread.h"

namespace Initial
{
	class IRessource : public IObject
	{
		friend class IRessourceManager;
	public:
		IRessource(Core::IString filename);
		virtual ~IRessource()/*=0*/;

		Core::IString GetFilename();
		void SetFilename(Core::IString filename);

		virtual void Reload()=0;

		void Lock();
		void Unlock();
	protected:
		Core::IString m_sFileName;
		IMutex m_mMutex;
	};
};

#endif