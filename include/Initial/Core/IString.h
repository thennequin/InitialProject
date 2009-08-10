//---------------------------------------------
//                Initial Engine
//                String  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISTRING_HEADER_
#define _ISTRING_HEADER_
#include "Initial/Config.h"

#include "stdio.h"
#include "stdlib.h"

namespace Initial
{
	namespace Core
	{
		class IString
		{
		public:
			IString();
			IString(const IChar* str);
			IString(const IString& str);
			IString(int val);
			IString(double val);

			~IString();

			bool AllocBuffer(UINT nLen);
			UINT Length() const;
			UINT AllocatedSize();

			bool Cat(const IChar *str);
			bool Printf(const IChar *format,...);
			bool vPrintf(const IChar *format,va_list ap);

			IChar* c_str() const { return m_pData; }

			int Find(const IString str);

			IString Left(UINT pos);
			IString Right(UINT pos);

			IString Before(IString str);
			IString After(IString str);

			IString ToLower();

			void Replace(IString strToReplace, IString byStr);

			IString BeforeFirst(IString val);
			IString AfterFirst(IString val);

			int Last(IString val);

			int Cmp(const IString& str) const;

			void operator=(const IString& str);
			void operator=(IChar *str);
			void operator=(IChar car);
			void operator=(int val);
			void operator=(float val);

			const IString operator+(const IString& str) const;

			void operator+=(const IString& str);
			void operator+=(const IChar *str);
			void operator+=(IChar car);
			void operator+=(int val);
			void operator+=(float val);

			IChar& operator[](int n);

			//operator IChar*() const;
			//operator const IChar*() const;

		private:
			void _Init();
			bool _SetString(IChar *str,UINT max=-1);		

		private:
			IChar *m_pData;
			UINT m_iLen; // String length
			UINT m_iSize; // Data allocate
			IChar _temp;
		};

		const IString operator+(const IChar *str1, IString& str2);

		inline bool operator==(const IString& s1, const IString& s2)
		{ return (s1.Length() == s2.Length()) && (s1.Cmp(s2) == 0); }
		inline bool operator==(const IString& s1, const IChar& s2)
		{ return (s1.Cmp(s2) == 0); }
		inline bool operator!=(const IString& s1, const IString& s2)
		{ return (s1.Length() != s2.Length()) || (s1.Cmp(s2) != 0); }
		inline bool operator!=(const IString& s1, const IChar& s2)
		{ return (s1.Cmp(s2) != 0); }

		/*operator char*(wchar_t *str) const;
		operator wchar_t*(char *str) const;*/
	}
}

#endif
