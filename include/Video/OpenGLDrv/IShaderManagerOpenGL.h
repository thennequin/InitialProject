//---------------------------------------------
//                Initial Engine
//             OpenGL Shader Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISHADERMANAGEROPENGL_HEADER_
#define _ISHADERMANAGEROPENGL_HEADER_

#include "Core/IArray.h"
#include "Video/IShader.h"
#include "Core/IString.h"
#include "Video/IRenderDriver.h"

namespace Initial
{
	class IShaderManagerOpenGL : public IShaderManager
	{
		friend class IRenderDriverOpenGL;
	public:
		~IShaderManagerOpenGL();

		virtual IShader *LoadShader(IString vertex, IString fragment);
		virtual void DeleteShader(IShader *shader);

		//Compile Shader
		virtual bool Compile(IShader *shader);

		virtual void UseShader(IShader *shader);
		virtual void SetParameterInt(IShader *shader, IString Parameter, int value);
		virtual void SetParameterFloat(IShader *shader, IString Parameter, float value);	
		virtual void SetParameter2Float(IShader *shader, IString Parameter, float value1, float value2);
		virtual void SetParameter2Float(IShader *shader, IString Parameter, float value[2]);
		virtual void SetParameter3Float(IShader *shader, IString Parameter, float value1, float value2, float value3);
		virtual void SetParameter3Float(IShader *shader, IString Parameter, float value[3]);
		virtual void SetParameter4Float(IShader *shader, IString Parameter, float value1, float value2, float value3, float value4);
		virtual void SetParameter4Float(IShader *shader, IString Parameter, float value[4]);
	protected:
		IShaderManagerOpenGL(IRenderDriver *device);
	protected:
		IArray<IShader*> m_pShaderList;

		IRenderDriver *m_pDevice;
	};
}

#endif