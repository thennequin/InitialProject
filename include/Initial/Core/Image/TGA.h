
#include "Initial/Core/IString.h"
#include "Initial/Core/IImage.h"

namespace Initial
{
	namespace Image
	{
		bool LoadTGA(Core::IString filename, Core::IImage *image);
		bool SaveTGA(Core::IString filename, Core::IImage *image);
	}
}
