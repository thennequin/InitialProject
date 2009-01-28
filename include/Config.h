//---------------------------------------------
//                Initial Engine
//                Config  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INITIAL_CONFIG_HEADER_
#define _INITIAL_CONFIG_HEADER_

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#pragma warning (disable: 4244)


//#define USE_UNICODE 0

#define USE_BINK 1

#undef IChar
#if USE_UNICODE && UNICODE
	#define IChar wchar_t
#else
	#define IChar char
#endif

#endif