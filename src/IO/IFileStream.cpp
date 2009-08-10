
#include <stdarg.h>

#include "Initial/IO/IFileStream.h"

using namespace Initial::Core;

namespace Initial
{
	namespace IO
	{
		IFileStream::IFileStream()
		{
			m_pFile=NULL;
		}

		IFileStream::IFileStream(Core::IString filename, OpenMode mode)
		{
			m_pFile=NULL;
			Open(filename,mode);
		}

		IFileStream::~IFileStream()
		{
			Close();
		}

		bool IFileStream::Open(Core::IString filename, OpenMode mode)
		{
			Close();
			
			IString modeStr;
			switch (mode)
			{
			default:
			case IOM_READ:
				modeStr="rb";
				break;
			case IOM_WRITE:
				modeStr="wb";
				break;
			case IOM_READ_WRITE:
				modeStr="wb+";
				break;
			case IOM_APPEND:
				modeStr="ab";
				break;
			case IOM_READ_APPEND:
				modeStr="ab+";
				break;
			}

			fopen_s(&m_pFile,filename.c_str(),modeStr.c_str());
			if (m_pFile)
			{
				m_sFilename=filename;
				return true;
			}
			return false;
		}

		void IFileStream::Close()
		{
			if (m_pFile)
			{
				fclose(m_pFile);
				m_pFile=NULL;
			}
		}

		bool IFileStream::IsOk()
		{
			return m_pFile!=NULL ? true : false;
		}

		UINT IFileStream::Read(void *dst, UINT size, UINT count)
		{
			if (m_pFile)
				return fread(dst,size,count,m_pFile);
			return 0;
		}

		UINT IFileStream::Write(void *src, UINT size, UINT count)
		{
			if (m_pFile)
				return fwrite(src,size,count,m_pFile);
			return 0;
		}

		UINT IFileStream::Printf(const char *Format, ...)
		{
			if (m_pFile)
			{
				va_list ap;
				va_start(ap, Format);
				return vfprintf(m_pFile,Format,ap);
			}
			return 0;
		}

		unsigned char IFileStream::GetChar()
		{
			if (m_pFile)
			{
				unsigned char car;
				fread(&car,1,1,m_pFile);
				return car;
			}
			return 0;
		}

		int IFileStream::Seek(long pos, SeekMode mode)
		{
			if (m_pFile)
			{
				int mod;
				switch (mode)
				{
				default:
				case ISM_SET:
					mod=SEEK_SET;
					break;
				case ISM_CURRENT:
					mod=SEEK_CUR;
					break;
				case ISM_END:
					mod=SEEK_END;
					break;
				}
				return fseek(m_pFile,pos,mod);
			}
			return 0;
		}

		UINT IFileStream::Tell()
		{
			if (m_pFile)
				return ftell(m_pFile);
			return 0;
		}

		bool IFileStream::EndOfFile()
		{
			if (m_pFile)
				return feof(m_pFile);
			return true;
		}

		UINT IFileStream::Size()
		{
			if (m_pFile)
			{				
				UINT current = Tell();
				Seek(0,ISM_END);
				UINT size = Tell();
				Seek(current,ISM_SET);
				return size;
			}
			return 0;
		}

		unsigned char IFileStream::operator[] (int i)
		{
			if (m_pFile)
			{
				long current = Tell();
				Seek(i,ISM_SET);
				unsigned char car;
				if (Read(&car,1,1)!=1)
					car=0;
				Seek(current,ISM_SET);
				return car;
			}
			return 0;
		}
	}
}