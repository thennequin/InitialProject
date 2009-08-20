
#ifndef _IASSERT_HEADER_
#define _IASSERT_HEADER_


#include "Initial/Core/IVector3D.h"
#include "Initial/Core/IColor.h"
#include "Initial/Core/IString.h"

namespace Initial
{
	namespace Core
	{
		void myassert(int line,IString file, IString message, IString message2);
	}
}

#define ASSERT_MSG(condition, message) if (!(condition)) Initial::Core::myassert(__LINE__,__FILE__,#condition,message);
#define ASSERT(condition) if (!(condition)) Initial::Core::myassert(__LINE__,__FILE__,#condition,"");

#endif