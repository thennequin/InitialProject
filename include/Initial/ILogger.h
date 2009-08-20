//---------------------------------------------
//                Initial Engine
//                Logger  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ILOGGER_HEADER_
#define _ILOGGER_HEADER_

#include "Initial/Core/IArray.h"
#include "Initial/Core/IString.h"
#include "Initial/IObject.h"
#include "Initial/IO/IFileStream.h"
#include "ISmartPtr.h"

namespace Initial
{
	class ILogger /*: public IObject*/
	{
	public:
		enum LogType
		{
			LT_MESSAGE,
			LT_WARNING,
			LT_ERROR,
			LT_DEBUG,
		};
	public:
		static void Log(LogType type,const IChar *format,...);
		static void Log(LogType type,const IChar *format,va_list ap);

		static void LogMessage(const IChar *format,...);
		static void LogWarning(const IChar *format,...);
		static void LogError(const IChar *format,...);
		static void LogDebug(const IChar *format,...);

		static void RegisterLogger(ILogger *logger);
		static void UnregisterLogger(ILogger *logger);

		virtual void Output(LogType type, Core::IString output)=0;
	protected:
		static Core::IArray<ILogger*> m_pRegistred;
	};

	class ILoggerConsole : public ILogger
	{
	public:
		~ILoggerConsole()
		{printf("destroy\n");}
		virtual void Output(LogType type, Core::IString output);
	};

	class ILoggerDebugger : public ILogger
	{
	public:
		virtual void Output(LogType type, Core::IString output);
	};

	class ILoggerFile : public ILogger
	{
	public:
		ILoggerFile();
		virtual void Output(LogType type, Core::IString output);
	protected:
		IO::IFileStream m_fStdOut;
		IO::IFileStream m_fStdErr;
	};
};

#endif
