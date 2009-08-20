//---------------------------------------------
//                Initial Engine
//                Logger  Module
//
//            By Thibault HENNEQUIN
//              june 6  2009
//---------------------------------------------

#include "Initial/ILogger.h"
#include "windows.h"

using namespace Initial::Core;

namespace Initial
{
	IArray<ILogger*> ILogger::m_pRegistred;

	//void ILogger::Log(IString message,LogType type)
	void ILogger::Log(LogType type,const IChar *format,...)
	{
		va_list ap;
		va_start(ap, format);
		Log(type,format,ap);
		//va_end(ap);
	}

	void ILogger::Log(LogType type,const IChar *format,va_list ap)
	{
		IString temp;
		temp.vPrintf(format,ap);	

		if (m_pRegistred.Count()==0)
		{
			RegisterLogger(new ILoggerConsole);
			RegisterLogger(new ILoggerDebugger);
			RegisterLogger(new ILoggerFile);
		}

		for (UINT i=0;i<m_pRegistred.Count();i++)
		{
			if (m_pRegistred[i])
				m_pRegistred[i]->Output(type,temp);
		}
	}

	void ILogger::LogMessage(const IChar *format,...)
	{
		va_list ap;
		va_start(ap, format);
		Log(LT_MESSAGE,format,ap);
		//va_end(ap);
	}

	void ILogger::LogWarning(const IChar *format,...)
	{
		va_list ap;
		va_start(ap, format);
		Log(LT_WARNING,format,ap);
		//va_end(ap);
	}

	void ILogger::LogError(const IChar *format,...)
	{
		va_list ap;
		va_start(ap, format);
		Log(LT_ERROR,format,ap);
		//va_end(ap);
	}

	void ILogger::LogDebug(const IChar *format,...)
	{
		va_list ap;
		va_start(ap, format);
		Log(LT_DEBUG,format,ap);
		//va_end(ap);
	}

	void ILogger::RegisterLogger(ILogger *logger)
	{
		m_pRegistred.Add(logger);
	}

	void ILogger::UnregisterLogger(ILogger *logger)
	{
		m_pRegistred.DeleteItem(logger);
	}

	void ILoggerConsole::Output(LogType type, IString output)
	{
		//Change color
		if (type==LT_WARNING)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		else if (type==LT_ERROR)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_INTENSITY);
		else if (type==LT_DEBUG)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);

		printf("%s",output.c_str());
		
		//Default color
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	}

	void ILoggerDebugger::Output(LogType type, IString output)
	{
		if (IsDebuggerPresent())
			OutputDebugString(output.c_str());

		//if (type==LT_ERROR && IsDebuggerPresent())
		//	DebugBreak();
	}

	ILoggerFile::ILoggerFile() :
		m_fStdOut("Log.txt",IO::IFileStream::IOM_WRITE),
		m_fStdErr("LogError.txt",IO::IFileStream::IOM_WRITE)
	{
	}

	void ILoggerFile::Output(LogType type, IString output)
	{		
		switch (type)
		{
		case LT_MESSAGE:
		case LT_WARNING:
		case LT_DEBUG:
			m_fStdOut.Printf("%s",output.c_str());
			break;
		case LT_ERROR:
			m_fStdErr.Printf("%s",output.c_str());
			break;
		}
	}
};