//---------------------------------------------
//                Initial Engine
//            Shader Manager  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISHADERMAANGER_HEADER_
#define _ISHADERMAANGER_HEADER_

#include "Core/IString.h"
#include "Video/IShader.h"

namespace Initial
{
	class IShader;

	class IShaderManager
	{
	public:
		virtual IShader *LoadShader(IString vertex, IString fragment)=0;
		virtual void DeleteShader(IShader *shader)=0;

		virtual bool Compile(IShader *shader)=0;
		IString ReturnError();

		virtual void UseShader(IShader *shader)=0;
		virtual void SetParameterInt(IShader *shader, IString Parameter, int value)=0;
		virtual void SetParameterFloat(IShader *shader, IString Parameter, float value)=0;
		virtual void SetParameter2Float(IShader *shader, IString Parameter, float value1, float value2)=0;
		virtual void SetParameter2Float(IShader *shader, IString Parameter, float value[2])=0;
		virtual void SetParameter3Float(IShader *shader, IString Parameter, float value1, float value2, float value3)=0;
		virtual void SetParameter3Float(IShader *shader, IString Parameter, float value[3])=0;
		virtual void SetParameter4Float(IShader *shader, IString Parameter, float value1, float value2, float value3, float value4)=0;
		virtual void SetParameter4Float(IShader *shader, IString Parameter, float value[4])=0;
	protected:
		IShader *NewShader(IShaderManager *manager);
		unsigned int GetVertexId(IShader *shader);
		unsigned int GetFragmentId(IShader *shader);
		unsigned int GetProgramId(IShader *shader);

		void SetVertexId(IShader *shader, unsigned int id);
		void SetFragmentId(IShader *shader, unsigned int id);
		void SetProgramId(IShader *shader, unsigned int id);

		//Compilation
		IString m_sError;	
	};

}

#endif