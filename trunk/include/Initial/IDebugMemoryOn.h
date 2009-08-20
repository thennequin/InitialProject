
#ifndef _IDEBUGMEMORYON_HEADER_
#define _IDEBUGMEMORYON_HEADER_

#include "Initial/Config.h"

#if USE_MEMORYDEBUG

	#include "Initial/IMemoryManager.h"

	//new
	inline void* operator new(std::size_t Size, const char* File, int Line)
	{
		return Initial::IMemoryManager::Instance().Allocate(Size, File, Line, false);
	}

	inline void* operator new[](std::size_t Size, const char* File, int Line)
	{
		return Initial::IMemoryManager::Instance().Allocate(Size, File, Line, true);
	}

	//delete
	inline void operator delete(void* Ptr)
	{
		Initial::IMemoryManager::Instance().Free(Ptr, false);
	}

	inline void operator delete(void* Ptr, const char* File, int Line)
	{
		Initial::IMemoryManager::Instance().NextDelete(File, Line);
		Initial::IMemoryManager::Instance().Free(Ptr, false);
	}

	inline void operator delete[](void* Ptr)
	{
		Initial::IMemoryManager::Instance().Free(Ptr, true);
	}

	#ifndef new
		#define new new(__FILE__, __LINE__)
		#define delete Initial::IMemoryManager::Instance().NextDelete(__FILE__, __LINE__), delete
	#endif

#endif

#endif
