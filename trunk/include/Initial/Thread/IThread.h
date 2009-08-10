//---------------------------------------------
//                Initial Engine
//                Thread  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITHREAD_HEADER_
#define _ITHREAD_HEADER_

//#ifdef _ITHREAD_COMPILE_
#include <boost/thread.hpp>
//#endif

#include "Initial/Core/IArray.h"

namespace boost
{
	class thread;
}

namespace Initial
{

	class IThread
	{
	friend class Thread_fake;
	public:
		IThread();
		~IThread();

		bool Run();
		bool Stop();

		bool IsRunning();
		bool IsFinnish();
	protected:
		virtual void ThreadFunc()=0;
	private:
		bool m_bFinish;
		bool m_bRunning;
		int _temp;

		static Core::IArray<IThread*> m_aThreads;

		boost::thread m_pThread;
	};

	class IMutex
	{
		friend class IMutexLocker;
	public:
		IMutex();

		void Lock();
		void Unlock();

		bool TryLock();
	private:
//#ifdef _ITHREAD_COMPILE_
		boost::mutex m_Mutex;
		boost::mutex::scoped_lock m_Lock;

		//boost::mutex m_InternMutex;
//#endif
	};

	class IMutexLocker
	{
	public:
		IMutexLocker(IMutex& mutex);
		~IMutexLocker();

	protected:
//#ifdef _ITHREAD_COMPILE_
		boost::mutex::scoped_lock m_Lock;
//#endif
	};
}

#endif
