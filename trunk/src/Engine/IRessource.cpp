
//---------------------------------------------
//                Initial Engine
//             Object source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/ILogger.h"
#include "Initial/IRessource.h"
#include "Initial/IRessourceManager.h"

using namespace Initial::Core;

namespace Initial
{
	
	IRessource::IRessource(IString filename)
	{
		//IRessourceManager.
		m_sFileName=filename;

		//FindFirstChangeNotification
		//FindCloseChangeNotification;

		//filename.
		//ReadDirectoryChangesW
		//m_pHandle = FindFirstChangeNotification (filename, false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        //if (INVALID_HANDLE_VALUE == _handle)
         //   throw WinException ("Cannot create change notification handle");
	}

	IRessource::~IRessource()
	{

	}

	Core::IString IRessource::GetFilename()
	{
		//printf("GetFilename %s\n",m_sFileName);
		return m_sFileName;
	}

	void IRessource::SetFilename(IString filename)
	{
		printf("SetFilename %s\n",filename);
		m_sFileName=filename;
	}

	void IRessource::Lock()
	{
		m_mMutex.Lock();
	}

	void IRessource::Unlock()
	{
		m_mMutex.Unlock();
	}
};
