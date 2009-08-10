//---------------------------------------------
//                Initial Engine
//             Thread source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

//#define _ITHREAD_COMPILE_

#include "Initial/Thread/IThread.h"

using namespace Initial::Core;

namespace Initial
{
	IArray<IThread*> IThread::m_aThreads;

	class Thread_fake
	{
	public:
		Thread_fake(IThread* thread)
		{
			m_pThread=thread;
		}

		Thread_fake(Thread_fake& fake)
		{
			*this=fake;
		}

		void operator =(Thread_fake& fake)
		{
			m_pThread=fake.m_pThread;
		}

		void operator()()
		{
			if (m_pThread)
			{
				m_pThread->ThreadFunc();
				m_pThread->m_bFinish=true;
			}
		}
	protected:
		IThread* m_pThread;
	};

	IThread::IThread()
	{
		//m_pThread=NULL;
		m_bFinish=false;
		m_bRunning=false;
	}

	IThread::~IThread()
	{
		Stop();
	}

	bool IThread::Run()
	{
		if (/*m_pThread==NULL &&*/ m_bFinish==false && m_bRunning==false)
		{
			//m_aThreads.Add(this);
			m_bRunning=true;
			Thread_fake fake(this);
			m_pThread = boost::thread(fake);
			//m_pThread->join();
			return true;
		}
		return false;
	}

	// TO FIX
	bool IThread::Stop()
	{
		//if (m_pThread)
		if (m_bRunning && m_bFinish==false)
		{
			//m_pThread->interrupt();
			//m_pThread->detach();
			//delete m_pThread;
			//m_pThread=NULL;
			m_pThread = boost::thread();
			m_bFinish=true;
			//m_aThreads.DeleteItem(this);
			return true;
		}
		return false;
	}

	bool IThread::IsRunning()
	{
		return m_bRunning && !m_bFinish;
	}

	bool IThread::IsFinnish()
	{
		return m_bFinish;
	}

	void IThread::ThreadFunc()
	{
		printf("ThreadFunc\n");
	}

	//------------------------


	IMutex::IMutex()
	{
	}

	void IMutex::Lock()
	{
		m_Lock = boost::mutex::scoped_lock(m_Mutex);
	}

	void IMutex::Unlock()
	{
		m_Lock = boost::mutex::scoped_lock();
	}

	bool IMutex::TryLock()
	{
		//m_Lock = boost::mutex::scoped_try_lock();
		
		return false;
	}

	IMutexLocker::IMutexLocker(IMutex& mutex)
	{
		m_Lock = boost::mutex::scoped_lock(mutex.m_Mutex);
	}

	IMutexLocker::~IMutexLocker()
	{
		//delete m_Lock;
	}
}
