//---------------------------------------------
//                Initial Engine
//                Shader  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISHADER_HEADER_
#define _ISHADER_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Math/IMatrix.h"
#include "Initial/Ressource/ITexture.h"
#include "Initial/Video/IShaderManager.h"

namespace Initial
{
	namespace Video
	{
		class IShaderManager;

		class IShader
		{
			friend class IShaderManager;
		public:		
			~IShader();

			Core::IString GetFragmentCode();
			Core::IString GetVertexCode();

			void SetFragmentCode(Core::IString Code);
			void SetVertexCode(Core::IString Code);

			int GetProgram();

			//Compile Shader
			bool IsCompiled();
			bool Compile();
			Core::IString ReturnError();

			void UseShader();
			bool SetParameterInt(Core::IString Parameter, int value);
			bool SetParameterFloat(Core::IString Parameter, float value);
			bool SetParameter2Float(Core::IString Parameter, float value1, float value2);
			bool SetParameter2Float(Core::IString Parameter, float value[2]);
			bool SetParameter2Float(Core::IString Parameter, Core::IVector2D& value);
			bool SetParameter3Float(Core::IString Parameter, float value1, float value2, float value3);
			bool SetParameter3Float(Core::IString Parameter, float value[3]);
			bool SetParameter3Float(Core::IString Parameter, Core::IVector3D& value);
			bool SetParameter4Float(Core::IString Parameter, float value1, float value2, float value3, float value4);
			bool SetParameter4Float(Core::IString Parameter, float value[4]);
			bool SetParameterMatrix22(Core::IString Parameter, Math::IMatrix& mat);
			bool SetParameterMatrix33(Core::IString Parameter, Math::IMatrix& mat);
			bool SetParameterMatrix44(Core::IString Parameter, Math::IMatrix& mat);
		protected:
			IShader(IShaderManager *manager, Core::IString vertex, Core::IString fragment);
		protected:
			Core::IString m_sShaderName;

			// Shader Code
			Core::IString m_sFragmentCode;
			Core::IString m_sVertexCode;

			// Shader
			int m_iFragmentShader;
			int m_iVertexShader;
			int m_iProgram;

			bool m_bNeedCompile;
			IShaderManager *m_pShaderManager;
		};
	}
}

#endif