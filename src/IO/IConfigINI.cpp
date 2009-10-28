//---------------------------------------------
//                Initial Engine
//               Ini source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/IO/IConfigINI.h"

#include "Initial/IO/IFileStream.h"

using namespace Initial::Core;

namespace Initial
{
	namespace IO
	{
		IConfigINI::IConfigINI(IString Filename)
		{
			m_bNeedSave=false;
			m_sFilename=Filename;
			//Values.Clear();
			Values.clear();
		}

		IConfigINI::~IConfigINI()
		{
			for (UINT i=0;i<Values.size();i++)
			{
				delete Values[i];
			}
		}

		bool IConfigINI::ExistFile()
		{
			IFileStream file(m_sFilename,IFileStream::IOM_READ);
			if (file.IsOk())
				return true;
			return false;
		}

		void IConfigINI::Load()
		{
			Values.clear();

			IFileStream file(m_sFilename,IFileStream::IOM_READ);
			if (file.IsOk())
			{
				IString Name;
				IString Value;

				int State = STATE_NONE;
				char preccar=' ';
				char car=file.GetChar();
				while (!file.EndOfFile())
				{
					switch (State)
					{
					case STATE_NONE:
						if (car=='#')
						{
							Name="";
							State=STATE_COM;
						}else if (car!='\t' && car!='\n' && car!='\r')
						{
							State=STATE_PRENAME;
						}
						break;
					case STATE_COM:
						if (car=='\n' || car=='\r')
						{
							ValueSlot *slot = new ValueSlot();
							slot->Name = Name;
							slot->Value = "";
							slot->IsCom=true;
							Values.push_back(slot);
							State=STATE_NONE;
						}else
							Name+=car;					
						break;
					case STATE_PRENAME:
						if (car=='\t' || car=='\n' || car=='\r')
						{
							State=STATE_NONE;
						}else if (car!=' ')
						{
							State=STATE_NAME;
							Name="";
							Name+=preccar;
							Name+=car;
						}
						break;
					case STATE_NAME:
						if (car==' ' || car=='=')
						{
							State=STATE_PREVALUE;
						}else if (car=='\t' || car=='\n' || car=='\r')
						{
							State=STATE_NONE;
						}else
							Name+=car;
						break;
					case STATE_PREVALUE:
						if (car=='\t' || car=='\n' || car=='\r')
						{
							State=STATE_NONE;
						}else if (car!=' ')
						{
							State=STATE_VALUE;
							Value="";
							Value+=car;
						}
						break;
					case STATE_VALUE:
						if (car=='\t' || car=='\n' || car=='\r' /*|| car==' '*/ || car=='=')
						{
							ValueSlot *slot = new ValueSlot();
							slot->Name = Name;
							slot->Value = Value;
							slot->IsCom=false;
							Values.push_back(slot);
							State=STATE_NONE;
						}else
							Value+=car;
						break;
					}
					preccar=car;
					car = file.GetChar();
				}

				if (State==STATE_VALUE)
				{
					ValueSlot *slot = new ValueSlot();
					slot->Name = Name;
					slot->Value = Value;
					slot->IsCom=false;
					Values.push_back(slot);
				}
			}
		}

		void IConfigINI::Save()
		{
			IFileStream file(m_sFilename,IFileStream::IOM_WRITE);
			if (file.IsOk())
			{
				for (unsigned int i=0;i<Values.size();i++)
				{
					if (Values[i]->IsCom)
						file.Printf("#%s\n",Values[i]->Name.c_str());
					else
						file.Printf("%s=%s\n",Values[i]->Name.c_str(),Values[i]->Value.c_str());
				}
			}			  
			file.Close();
		}

		bool IConfigINI::NeedToBeSave()
		{
			return m_bNeedSave || !ExistFile();
		}

		bool IConfigINI::ValueExist(IString ValueName)
		{
			for (unsigned int i=0;i<Values.size();i++)
			{
				if (Values[i]->IsCom==false && Values[i]->Name == ValueName)
				{
					return true;
				}
			}
			return false;
		}

		IString IConfigINI::ReadStringValue(IString ValueName,IString Default,bool WriteDefaultValue)
		{
			if (ValueExist(ValueName))
			{
				for (unsigned int i=0;i<Values.size();i++)
					if (Values[i]->IsCom==false && Values[i]->Name == ValueName)
						return Values[i]->Value;
			}else if (WriteDefaultValue)
				WriteStringValue(ValueName,Default);

			return Default;
		}

		int IConfigINI::ReadIntValue(IString ValueName,int Default,bool WriteDefaultValue)
		{
			if (ValueExist(ValueName))
				return atoi(ReadStringValue(ValueName).c_str());
			else if (WriteDefaultValue)
				WriteIntValue(ValueName,Default);
			return Default;
		}

		float IConfigINI::ReadFloatValue(IString ValueName,float Default,bool WriteDefaultValue)
		{
			if (ValueExist(ValueName))
				return atof(ReadStringValue(ValueName).c_str());
			else if (WriteDefaultValue)
				WriteFloatValue(ValueName,Default);
			return Default;
		}

		void IConfigINI::WriteStringValue(IString ValueName,IString Value)
		{
			if (ValueExist(ValueName))
			{
				for (unsigned int i=0;i<Values.size();i++)
				{
					if (Values[i]->Name == ValueName)
					{
						Values[i]->Value = Value;
						break;
					}
				}
			}
			else{
				m_bNeedSave=true;
				ValueSlot *slot = new ValueSlot();
				if (slot)
				{
					slot->Name = ValueName;
					slot->Value = Value;
					slot->IsCom=false;
					Values.push_back(slot);
				}
			}
		}

		void IConfigINI::WriteIntValue(IString ValueName,int Value)
		{
			IString text;
			text.Printf("%d",Value);
			WriteStringValue(ValueName,text);
		}

		void IConfigINI::WriteFloatValue(IString ValueName,float Value)
		{
			IString text;
			text.Printf("%f",Value);
			WriteStringValue(ValueName,text);
		}
	}
}
