
#ifndef _IFILESTREAM_HEADER_
#define _IFILESTREAM_HEADER_

#include "Initial/Config.h"
#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"

#include <stdio.h>

namespace Initial
{
	namespace IO
	{
		class IFileStream
		{
		public:
			enum OpenMode
			{
				IOM_READ,
				IOM_WRITE,
				IOM_READ_WRITE,
				IOM_APPEND,
				IOM_READ_APPEND,
			};

			enum SeekMode
			{
				ISM_SET,
				ISM_CURRENT,
				ISM_END,
			};
		public:
			IFileStream();
			IFileStream(Core::IString filename, OpenMode mode=IOM_READ_WRITE);
			~IFileStream();

			bool Open(Core::IString filename, OpenMode mode);
			void Close();

			bool IsOk();

			UINT Read(void *dst, UINT size, UINT count);
			unsigned char GetChar();

			UINT Write(void *src, UINT size, UINT count);
			UINT Printf(const char *Format, ...);

			int Seek(long pos, SeekMode mode=ISM_SET);
			UINT Tell();
			bool EndOfFile();
			UINT Size();

			unsigned char operator[] (int i);
		protected:
			Core::IString m_sFilename;
			FILE *m_pFile;
			OpenMode m_iMode;
		};
	}
}

#endif