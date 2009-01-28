//---------------------------------------------
//                Initial Engine
//                  Ini  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ICONFIGINI_HEADER_
#define _ICONFIGINI_HEADER_

#include "Core/IString.h"
#include "Core/IArray.h"

namespace Initial
{

	class IConfigINI
	{
	private:
		class ValueSlot
		{
		public:
			ValueSlot(IString name="", IString value="",bool Com=false) { Name=name; Value=value; IsCom=Com; }
			IString Name;
			IString Value;
			bool IsCom;
		};

		enum
		{
			STATE_NONE,
			STATE_COM,
			STATE_PRENAME,
			STATE_NAME,
			STATE_PREVALUE,
			STATE_VALUE,
			STATE_MAX
		};

	public:
		IConfigINI(IString Filename);

		void		Load();
		void		Save();

		bool		ValueExist(IString ValueName);
		IString		ReadStringValue(IString ValueName,IString Default="",bool WriteDefaultValue=true);
		int			ReadIntValue(IString ValueName,int Default=0,bool WriteDefaultValue=true);
		float		ReadFloatValue(IString ValueName,float Default=0,bool WriteDefaultValue=true);

		void		WriteStringValue(IString ValueName,IString Value);
		void		WriteIntValue(IString ValueName,int Value);
		void		WriteFloatValue(IString ValueName,float Value);

		void		SetFilename(IString filename) { mFilename=filename; }
		IString		GetFilename() { return mFilename; }
		bool		ExistFile();

	private:
		IString		mFilename;

		IArray<ValueSlot*> Values;
	};

}

#endif