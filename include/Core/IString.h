//---------------------------------------------
//                Initial Engine
//                String  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISTRING_HEADER_
#define _ISTRING_HEADER_
#include "Config.h"

#include "stdio.h"
#include "stdlib.h"

namespace Initial
{

	class IString
	{
	public:
		IString();
		IString(const IChar* str);
		IString(IString& str);
		IString(int val);
		IString(double val);

		~IString();

		bool AllocBuffer(size_t nLen);
		unsigned int Length();

		bool Cat(const IChar *str);
		bool Printf(const IChar *format,...);

		IChar* c_str() { return m_pData; }

		int Find(const IChar *str);

		IString Left(size_t pos);
		IString Right(size_t pos);

		IString Before(IString str);
		IString After(IString str);

		IString ToLower();

		int Cmp(IString str);

		bool operator!() const;

		void operator=(IString& str);
		void operator=(IChar *str);
		void operator=(IChar car);
		void operator=(int val);
		void operator=(float val);

		const IString operator+(IString& str);

		void operator+=(IString& str);
		void operator+=(const IChar *str);
		void operator+=(IChar car);
		void operator+=(int val);
		void operator+=(float val);

		bool operator==(IString& str);
		bool operator!=(IString& str);
		IChar operator[](int n);

		operator IChar*() const;
		operator const IChar*() const;

	private:
		void _Init();
		bool _SetString(IChar *str,size_t max=-1);		

	private:
		IChar *m_pData;
		size_t m_iLen;
	};

	const IString operator+(const IChar *str1, IString& str2);

	/*operator char*(wchar_t *str) const;
	operator wchar_t*(char *str) const;*/
}

#endif
