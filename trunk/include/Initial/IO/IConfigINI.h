//---------------------------------------------
//                Initial Engine
//                  Ini  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ICONFIGINI_HEADER_
#define _ICONFIGINI_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"

namespace Initial
{
	namespace IO
	{
		class IConfigINI
		{
		private:
			class ValueSlot
			{
			public:
				ValueSlot(Core::IString name="", Core::IString value="",bool Com=false) { Name=name; Value=value; IsCom=Com; }
				Core::IString Name;
				Core::IString Value;
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
			IConfigINI(Core::IString Filename);
			~IConfigINI();

			void		Load();
			void		Save();
			bool		NeedToBeSave();

			bool		ValueExist(Core::IString ValueName);
			Core::IString		ReadStringValue(Core::IString ValueName,Core::IString Default="",bool WriteDefaultValue=true);
			int			ReadIntValue(Core::IString ValueName,int Default=0,bool WriteDefaultValue=true);
			float		ReadFloatValue(Core::IString ValueName,float Default=0,bool WriteDefaultValue=true);

			void		WriteStringValue(Core::IString ValueName,Core::IString Value);
			void		WriteIntValue(Core::IString ValueName,int Value);
			void		WriteFloatValue(Core::IString ValueName,float Value);

			void		SetFilename(Core::IString filename) { m_sFilename=filename; }
			Core::IString		GetFilename() { return m_sFilename; }
			bool		ExistFile();

		private:
			bool m_bNeedSave;
			Core::IString		m_sFilename;

			Core::IArray<ValueSlot*> Values;
		};
	}
}

#endif