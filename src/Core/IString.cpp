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

#include "Core/IString.h"

using namespace Initial;

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

IString::IString(IString& str)
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
	m_pData = (IChar*)0; 
	m_iLen=0; 
}

bool IString::AllocBuffer(size_t nLen)
{
	if (nLen>0)
	{
		if (m_pData==NULL)
		{
			m_pData = (IChar*)malloc(sizeof(IChar)*nLen);
			if (m_pData)
			{
				m_iLen=nLen;
				return true;
			}
		}else{
			IChar *_temp;
			_temp = (IChar*)realloc(m_pData,sizeof(IChar)*nLen);
			if (_temp)
			{
				m_iLen=nLen;
				m_pData=_temp;
				return true;
			}
		}
	}
	return false;
}

unsigned int IString::Length()
{
	return m_iLen;
}

bool IString::_SetString(IChar *str,size_t max)
{
	if (str)
	{
		size_t Size = strlen(str)+1;
		if (max>=0 && Size>max)
			Size=max+1;
		if (AllocBuffer(Size))
		{
			memcpy(m_pData,str,Size);
			m_pData[Size-1]='\0';
			//strcpy_s(m_pData,Size-max,);
			return true;
		}
	}
	return false;
}

bool IString::Cat(const IChar *str)
{
	if (str)
	{
		size_t Size=strlen(str)+m_iLen;
		if (AllocBuffer(Size))
		{
			strcat_s(m_pData,Size,str);
			//m_pData[Size-1]='\0';
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
	//vsnprintf_s(NULL,0,1,format,ap);
	size_t size = vsnprintf(NULL,0,format,ap)+1;
	if (temp.AllocBuffer(size))
	{
		vsnprintf_s(temp.m_pData,size,-1,format,ap);
		_SetString(temp.c_str());
		//va_end(ap);
		return true;
	}
	va_end(ap);
	return false;
}

int IString::Cmp(IString str) 
{ 
	return strcmp(this->c_str(),str.c_str());
}

int IString::Find(const IChar *str)
{
	if (str)
	{
		size_t pos = (size_t)strstr(m_pData,str);
		if (pos)
		{
			int newpos = pos - (int)m_pData;
			return newpos;
		}
	}
	return -1;
}

IString IString::Left(size_t pos)
{
	if (pos>=0)
	{
		IString temp;
		temp._SetString(c_str(),pos);
		return IString(temp);
	}
	return IString(*this);
}

IString IString::Right(size_t pos)
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
	return IString(strlwr(c_str()));
}
		

bool IString::operator!() const 
{ 
	return m_iLen!=0 ? false : true; 
}

void IString::operator=(IString& str)
{ 
	_SetString(str.c_str()); 
}

void IString::operator=(IChar *str)
{ 
	_SetString(str); 
}

const IString IString::operator+(IString& str)
{
	IString Str=*this;
	Str.Cat(str);
	return Str;
}

void IString::operator+=(IString& str)
{ 
	Cat(str.c_str()); 
}

void IString::operator+=(const IChar* str)
{ 
	Cat(str); 
}

void IString::operator=(IChar car)
{
	IString temp;
	temp.Printf("%c",car);
	_SetString(temp.c_str());
}

void IString::operator=(int val)
{
	IString temp;
	temp.Printf("%d",val);
	_SetString(temp.c_str());
}

void IString::operator=(float val)
{
	IString temp;
	temp.Printf("%f",val);
	_SetString(temp.c_str());
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

bool IString::operator==(IString& str)
{
	return Cmp(str)==0 ? true : false;
}

bool IString::operator!=(IString& str)
{
	return Cmp(str)==0 ? false : true;
}

IChar IString::operator[](int n)
{
	if (n>=0 && n<m_iLen)
		return m_pData[n];
	return 0;
}

IString::operator IChar*() const 
{ 
	return m_pData; 
}

IString::operator const IChar*() const
{ 
	return m_pData; 
}

const IString Initial::operator+(const IChar *str1, IString& str2)
{
	IString Str(str1);
	Str.Cat(str2);
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