//---------------------------------------------
//                Initial Engine
//               Ini source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include <stdio.h>
#include "Core/IConfigINI.h"

using namespace Initial;

IConfigINI::IConfigINI(IString Filename)
{
	mFilename=Filename;
	Values.Clear();
}

bool IConfigINI::ExistFile()
{
	FILE *file ;
	fopen_s(&file, mFilename.c_str(), "rb");
	if (file)
	{
		fclose(file);
		return true;
	}
	return false;
}

void IConfigINI::Load()
{
	Values.Clear();

	FILE *file;
	fopen_s(&file, mFilename.c_str(), "r");
	if (file)
	{
		IString Name;
		IString Value;

		int State = STATE_NONE;
		char preccar=' ';
		char car=fgetc(file);
		if (file)
		{
			while (!feof(file))
			{
				switch (State)
				{
				case STATE_NONE:
					if (car=='#')
					{
						Name="";
						State=STATE_COM;
					}else if (car!='\t' || car!='\n' || car!='\r')
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
						Values.Add(slot);
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
						Values.Add(slot);
						State=STATE_NONE;
					}else
						Value+=car;
					break;
				}
				preccar=car;
				car = fgetc(file);
			}

			fclose(file);

			if (State==STATE_VALUE)
			{
				ValueSlot *slot = new ValueSlot();
				slot->Name = Name;
				slot->Value = Value;
				slot->IsCom=false;
				Values.Add(slot);
			}
		}
	}
}

void IConfigINI::Save()
{
	FILE *file;
	fopen_s(&file, mFilename.c_str(), "w");
	if (file)
	{
		for (unsigned int i=0;i<Values.Count();i++)
		{
			if (Values[i]->IsCom)
				fprintf(file,"#%s\n",Values[i]->Name.c_str());
			else
				fprintf(file,"%s=%s\n",Values[i]->Name.c_str(),Values[i]->Value.c_str());
		}
	}			  
	fclose(file);
}


bool IConfigINI::ValueExist(IString ValueName)
{
	for (unsigned int i=0;i<Values.Count();i++)
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
		for (unsigned int i=0;i<Values.Count();i++)
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
		for (unsigned int i=0;i<Values.Count();i++)
		{
			if (Values[i]->Name == ValueName)
			{
				Values[i]->Value = Value;
				break;
			}
		}
	}
	else{
		ValueSlot *slot = new ValueSlot();
		if (slot)
		{
			slot->Name = ValueName;
			slot->Value = Value;
			slot->IsCom=false;
			Values.Add(slot);
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