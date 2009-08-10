//---------------------------------------------
//                Initial Engine
//             String source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Initial/Core/IString.h"

//#pragma warning( disable : 4311)

namespace Initial
{
	namespace Core
	{
		IString::IString()
		{
			_Init();
			_SetString("");
		}

		IString::IString(const IChar* str)
		{
			_Init();
			_SetString((IChar*)str);
		}

		IString::IString(const IString& str)
		{
			_Init();
			_SetString(str.c_str());
		}

		IString::IString(int val)
		{
			_Init();
			Printf("%d",val);
		}

		IString::IString(double val)
		{
			_Init();
			Printf("%f",val);
		}

		IString::~IString()
		{
			if (m_pData)
				free(m_pData);
		}

		void IString::_Init()
		{
			m_iSize = 8;
			m_pData = (IChar*)malloc(sizeof(IChar)*(m_iSize+1));
			//m_pData=NULL;
			//AllocBuffer(8);
			m_iLen=0; 
			m_pData[m_iLen]='\0';
		}

		bool IString::AllocBuffer(UINT nLen)
		{
			if (nLen>=0)
			{
				if (nLen>m_iSize)
				{
					int newSize = m_iSize;
					while (newSize<nLen)
						newSize*=2;

					IChar *_temp = (IChar*) realloc(m_pData,sizeof(IChar)*(newSize+1));
					if (_temp)
					{
						m_iSize=newSize;
						m_pData=_temp;
						return true;
					}
					return false;
				}
				return true;
			}
			return false;
		}

		UINT IString::Length() const
		{
			return strlen(m_pData);
			//return m_iLen;
		}

		UINT IString::AllocatedSize()
		{
			return m_iSize;
		}

		bool IString::_SetString(IChar *str,UINT max)
		{
			if (str)
			{
				UINT Size = (UINT)strlen(str);
				if (max>=0 && Size>max)
					Size=max;
				if (AllocBuffer(Size) && m_pData)
				{
					memcpy(m_pData,str,Size);
					m_iLen=Size;
					m_pData[Size]='\0';
					return true;
				}
			}
			return false;
		}

		bool IString::Cat(const IChar *str)
		{
			if (str)
			{
				UINT Len=(UINT)strlen(str);
				UINT OldLen=strlen(m_pData);
				UINT Size=Len+OldLen;		
				if (AllocBuffer(Size))
				{
					memcpy(m_pData+OldLen,str,Len);
					m_iLen=Size;
					m_pData[Size]='\0';
					return true;
				}
			}
			return false;
		}

		bool IString::Printf(const IChar *format,...)
		{
			va_list ap;
			va_start(ap, format);

			IString temp;
			int Size = vsnprintf(NULL,0,format,ap);
			if (AllocBuffer(Size))
			{
				vsnprintf_s(m_pData,Size+1,-1,format,ap);
				m_iLen=Size;
				//m_pData[Size]='\0';
				//va_end(ap);
				return true;
			}
			va_end(ap);
			return false;
		}

		bool IString::vPrintf(const IChar *format,va_list ap)
		{
			IString temp;
			int Size = vsnprintf(NULL,0,format,ap);
			if (AllocBuffer(Size))
			{
				vsnprintf_s(m_pData,Size+1,-1,format,ap);
				m_iLen=Size;
				//m_pData[Size]='\0';
				//va_end(ap);
				return true;
			}
			return false;
		}

		int IString::Cmp(const IString& str) const
		{ 
			return strcmp(m_pData,str.m_pData);
		}

		int IString::Find(const IString str)
		{
			char *pos = strstr(m_pData,str.c_str());
			if (pos)
			{
				int newpos = pos - m_pData;
				return newpos;
			}
			return -1;
		}

		IString IString::Left(UINT pos)
		{
			if (pos>=0)
			{
				IString temp;
				temp._SetString(c_str(),pos);
				return IString(temp);
			}
			return IString(*this);
		}

		IString IString::Right(UINT pos)
		{
			if (pos>=0)
			{
				IString temp;
				temp._SetString(c_str()+pos);
				return IString(temp);
			}
			return IString(*this);
		}

		IString IString::Before(IString str)
		{
			int pos = Find(str.c_str());
			if (pos>=0)
				return Left(pos);
			return IString();
		}

		IString IString::After(IString str)
		{
			int pos = Find(str.c_str());
			if (pos>=0)
				return Right(pos+1);
			return IString();
		}

		IString IString::ToLower()
		{
			//return IString(_strlwr_s(c_str(),Length()));
			return strlwr(c_str());
		}

		void IString::Replace(IString strToReplace, IString byStr)
		{

		}

		IString IString::BeforeFirst(IString val)
		{
			return Before(val);
		}

		IString IString::AfterFirst(IString val)
		{
			return After(val);
		}

		int IString::Last(IString val)
		{
			return 0;
		}

		void IString::operator=(const IString& str)
		{ 
			_SetString(str.m_pData);
		}

		void IString::operator=(IChar *str)
		{ 
			_SetString(str);
		}

		const IString IString::operator+(const IString& str) const
		{
			IString Str=c_str();
			Str.Cat(str.c_str());
			return Str;
		}

		void IString::operator+=(const IString& str)
		{ 
			Cat(str.c_str()); 
		}

		void IString::operator+=(const IChar* str)
		{ 
			Cat(str);
		}

		void IString::operator=(IChar car)
		{
			//Printf("%c",car);
			(*this)=" ";
			(*this)[0]=car;
		}

		void IString::operator=(int val)
		{
			Printf("%d",val);
		}

		void IString::operator=(float val)
		{
			Printf("%f",val);
		}

		void IString::operator+=(IChar car)
		{
			IString temp;
			temp=car;
			Cat(temp.c_str());
		}

		void IString::operator+=(int val)
		{
			IString temp;
			temp=val;
			Cat(temp.c_str());
		}

		void IString::operator+=(float val)
		{
			IString temp;
			temp=val;
			Cat(temp.c_str());
		}

		IChar& IString::operator[](int n)
		{
			if (n>=0 && n<=(int)m_iSize)
				return m_pData[n];
			return _temp;
		}

		/*IString::operator IChar*() const 
		{ 
			return m_pData; 
		}*/

		/*IString::operator const IChar*() const
		{ 
			return m_pData; 
		}*/

		const IString operator+(const IChar *str1, IString& str2)
		{
			IString Str=str1;
			Str.Cat(str2.c_str());
			return Str;
		}

		//Initial::operator char*(wchar_t *str) const
		//{
		//	strlen(str);
		//	//mbstowcs_s(&convertedChars, wcstring, origsize, orig, _TRUNCATE);
		//	return NULL;
		//}
		//
		//Initial::operator wchar_t*(char *str) const
		//{
		//	return NULL;
		//}
	}
}
