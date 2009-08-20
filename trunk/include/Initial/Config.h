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

//#pragma warning (disable: 4244)

#ifdef WIN32
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0600
	#endif
#endif

#define INITIAL_EXPORT __declspec(dllexport)
#define INITIAL_IMPORT __declspec(dllimport)

#ifndef USE_MEMORYDEBUG
#define USE_MEMORYDEBUG 0
#endif

#ifndef USE_UNICODE
#define USE_UNICODE 0
#endif

#ifndef USE_BINK
//#define USE_BINK 0
#endif

#undef IChar
#if USE_UNICODE && UNICODE
	typedef wchar_t IChar;
#else
	typedef char IChar;
#endif

typedef int INT;
typedef unsigned int UINT;

#ifndef USE_OPENGL
#define USE_OPENGL 1
#endif

#endif