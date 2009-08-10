//---------------------------------------------
//                Initial Engine
//            Shader Manager  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISHADERMAANGER_HEADER_
#define _ISHADERMAANGER_HEADER_

#include "Initial/Core/IArray.h"
#include "Initial/Math/IMatrix.h"
#include "Initial/Core/IString.h"
#include "Initial/Video/IShader.h"

namespace Initial
{
	namespace Video
	{
		class IShader;

		class IShaderManager
		{
		public:
			virtual IShader *LoadShader(Core::IString vertex, Core::IString fragment)=0;
			virtual void DeleteShader(IShader *shader)=0;

			virtual bool Compile(IShader *shader)=0;
			Core::IString ReturnError();

			virtual void UseShader(IShader *shader)=0;
			virtual bool SetParameterInt(IShader *shader, Core::IString Parameter, int value)=0;
			virtual bool SetParameterFloat(IShader *shader, Core::IString Parameter, float value)=0;
			virtual bool SetParameter2Float(IShader *shader, Core::IString Parameter, float value1, float value2)=0;
			virtual bool SetParameter2Float(IShader *shader, Core::IString Parameter, float value[2])=0;
			virtual bool SetParameter3Float(IShader *shader, Core::IString Parameter, float value1, float value2, float value3)=0;
			virtual bool SetParameter3Float(IShader *shader, Core::IString Parameter, float value[3])=0;
			virtual bool SetParameter4Float(IShader *shader, Core::IString Parameter, float value1, float value2, float value3, float value4)=0;
			virtual bool SetParameter4Float(IShader *shader, Core::IString Parameter, float value[4])=0;
			virtual bool SetParameterMatrix22(IShader *shader, Core::IString Parameter, Math::IMatrix& mat)=0;
			virtual bool SetParameterMatrix33(IShader *shader, Core::IString Parameter, Math::IMatrix& mat)=0;
			virtual bool SetParameterMatrix44(IShader *shader, Core::IString Parameter, Math::IMatrix& mat)=0;

			void SendLightsInfo();
		protected:
			IShader *NewShader(IShaderManager *manager);
			unsigned int GetVertexId(IShader *shader);
			unsigned int GetFragmentId(IShader *shader);
			unsigned int GetProgramId(IShader *shader);

			void SetVertexId(IShader *shader, unsigned int id);
			void SetFragmentId(IShader *shader, unsigned int id);
			void SetProgramId(IShader *shader, unsigned int id);

			//Compilation
			Core::IString m_sError;	

			Core::IArray<IShader*> m_pShaderList;
		};
	}
}

#endif