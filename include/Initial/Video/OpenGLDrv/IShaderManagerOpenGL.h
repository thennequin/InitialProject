//---------------------------------------------
//                Initial Engine
//             OpenGL Shader Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISHADERMANAGEROPENGL_HEADER_
#define _ISHADERMANAGEROPENGL_HEADER_

#include "Initial/Video/IShader.h"
#include "Initial/Core/IString.h"
#include "Initial/Math/IMatrix.h"
#include "Initial/Video/IRenderDriver.h"

namespace Initial
{
	namespace Video
	{
		class IShaderManagerOpenGL : public IShaderManager
		{
			friend class IRenderDriverOpenGL;
		public:
			~IShaderManagerOpenGL();

			virtual IShader *LoadShader(Core::IString vertex, Core::IString fragment);
			virtual void DeleteShader(IShader *shader);

			//Compile Shader
			virtual bool Compile(IShader *shader);

			virtual bool UseShader(IShader *shader);
			virtual bool SetParameterInt(IShader *shader, Core::IString Parameter, int value);
			virtual bool SetParameterFloat(IShader *shader, Core::IString Parameter, float value);	
			virtual bool SetParameter2Float(IShader *shader, Core::IString Parameter, float value1, float value2);
			virtual bool SetParameter2Float(IShader *shader, Core::IString Parameter, float value[2]);
			virtual bool SetParameter3Float(IShader *shader, Core::IString Parameter, float value1, float value2, float value3);
			virtual bool SetParameter3Float(IShader *shader, Core::IString Parameter, float value[3]);
			virtual bool SetParameter4Float(IShader *shader, Core::IString Parameter, float value1, float value2, float value3, float value4);
			virtual bool SetParameter4Float(IShader *shader, Core::IString Parameter, float value[4]);
			virtual bool SetParameterMatrix22(IShader *shader, Core::IString Parameter, Math::IMatrix& mat);
			virtual bool SetParameterMatrix33(IShader *shader, Core::IString Parameter, Math::IMatrix& mat);
			virtual bool SetParameterMatrix44(IShader *shader, Core::IString Parameter, Math::IMatrix& mat);
		protected:
			IShaderManagerOpenGL(IRenderDriver *device);
		protected:

			IRenderDriver *m_pDevice;
		};
	}
}

#endif