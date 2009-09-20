
#ifdef WIN32
#include <windows.h>
#endif

#include "Initial/Core/IUtil.h"

namespace Initial
{
	namespace Core
	{
		#define CPUID __asm __emit 0fh __asm __emit 0a2h
		#define RDTSC __asm __emit 0fh __asm __emit 031h

		unsigned __int64 timercall()
		{
			unsigned cycles_low, cycles_high;
			__asm {
			pushad
			CPUID
			RDTSC
			mov cycles_high, edx
			mov cycles_low, eax
			popad }
			return ((unsigned __int64)cycles_high << 32) | cycles_low;
		}



		IString TrimZeroFloat(IString str)
		{
			IString temp;
			str.Replace(",",".");

			int pos = str.Length()-1;

			while (str[pos-1]=='0')
				pos--;
			if (str[pos-1]=='.')
				pos++;
			temp=str.Left(pos);
			return temp;
		}

		double Iatof(IString str)
		{
			/*double temp;
			str.ToDouble(&temp);
			return temp;*/
			//---------------
			double final=0.0;
			double dot=1.0;
			double val=0;
			int i=0;
			bool dec=false;
			bool OK=true;
			bool neg=false;
			bool negpossible=true;
			bool start=false;

			while (str.Length()>(UINT)i && OK)
			{
				switch (str[i])
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					val = (double)str[i]-(double)'0';
					if (dec==false)
						final=final*10+val;
					else{
						dot=dot*10;
						final=final+val/dot;
					}
					if (negpossible)
						negpossible=false;
					if (start==false)
						start=true;
					break;
				case '.':
				case ',':
					if (dec==false)
						dec=true;
					else
						OK=false;
					break;
				case '-':
					if (negpossible)
						neg=true;
					else
						OK=false;
					break;
				default:
					if (start || str[i]!=' ')
					OK=false;
				}
				i++;
			}

			if (neg)
				final*=-1;
			return final;
		}

		IString Iftoa(float val)
		{
			IString str;
			str.Printf("%f",val);
			return str;
		}

		IVector2D Vector2DFromString(IString str)
		{
			IString x,y;
			x=str.BeforeFirst(";");
			y=str.AfterFirst(";");
			return IVector2D(Iatof(x),Iatof(y));
		}

		IVector3D Vector3DFromString(IString str)
		{
			IString x,y,z,temp;
			x=str.BeforeFirst(";");
			temp=str.AfterFirst(";");
			y=temp.BeforeFirst(";");
			z=temp.AfterFirst(";");
			return IVector3D(Iatof(x),Iatof(y),Iatof(z));
		}

		IColor ColorFromString(IString str)
		{
			IString r,g,b,a,temp;
			r=str.BeforeFirst(";");
			temp=str.AfterFirst(";");
			g=temp.BeforeFirst(";");
			temp=temp.AfterFirst(";");
			b=temp.BeforeFirst(";");
			a=temp.AfterFirst(";");
			return IColor(Iatof(r),Iatof(g),Iatof(b),Iatof(a));
		}

		bool BoolFromString(IString str)
		{
			str=str.ToLower();
			if (str==IString(""))
				return false;
			else if (str==IString("0"))
				return false;
			else if (str==IString("false"))
				return false;
			else 
				return true;
		}

		IString GetFullPath(IString File)
		{
			char *p;
			long len = GetFullPathName(File.c_str(), 0,0, &p);
			IString fileFull;
			fileFull.AllocBuffer(len);
			GetFullPathName(File.c_str(), fileFull.AllocatedSize(), fileFull.c_str(), &p);
			return fileFull;
		}

		IString FilePath(IString file)
		{
			//file.Find(
			return "";
		}

		IString RelativeToPath(IString filepath, IString file)
		{
			int pos = filepath.Last('/');
			int pos2 = filepath.Last('\\');
			if (pos2>pos)
				pos=pos2;

			IString path = filepath.Left(pos);
			if (file.Find(path)!=-1)
				file = file.Right(path.Length()+1);

			return file;
		}

		IString PathToFile(IString filepath, IString file)
		{
			int pos = filepath.Last('/');
			int pos2 = filepath.Last('\\');
			if (pos2>pos)
				pos=pos2;

			IString path = filepath.Left(pos2);

			return path+"\\"+file;
		}

		IString AppPath()
		{
			char buffer[2048];
			GetCurrentDirectory(2048,buffer);
			return IString(buffer)+"\\";
		}
	}
}