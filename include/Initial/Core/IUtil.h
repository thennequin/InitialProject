
#ifndef _IUTIL_HEADER_
#define _IUTIL_HEADER_

#include "Initial/Core/IVector2D.h"
#include "Initial/Core/IVector3D.h"
#include "Initial/Core/IColor.h"
#include "Initial/Core/IString.h"

namespace Initial
{
	namespace Core
	{
		unsigned __int64 timercall();

		IString TrimZeroFloat(IString str);

		double Iatof(IString str);
		IString Iftoa(float val);

		IVector2D Vector2DFromString(IString str);
		IVector3D Vector3DFromString(IString str);
		IColor ColorFromString(IString str);
		bool BoolFromString(IString str);

		IString GetFullPath(IString File);
		IString FilePath(IString file); // return folder of file
		IString RelativeToPath(IString filepath, IString file);
		IString PathToFile(IString filepath, IString file);
		IString AppPath();
	}
}

#endif