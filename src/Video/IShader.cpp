//---------------------------------------------
//                Initial Engine
//             Shader source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Video/IShader.h"

using namespace Initial::Core;
using namespace Initial::Math;

namespace Initial
{
	namespace Video
	{
		IShader::IShader(IShaderManager *manager, IString vertex, IString fragment)
		{
			m_sFragmentCode=fragment;
			m_sVertexCode=vertex;
			m_pShaderManager=manager;

			m_iFragmentShader=0;
			m_iVertexShader=0;
			m_iProgram=0;

			m_bNeedCompile=true;
		}

		IShader::~IShader()
		{
			if (m_pShaderManager)
				m_pShaderManager->DeleteShader(this);
		}

		IString IShader::GetFragmentCode()
		{
			return m_sFragmentCode;
		}

		IString IShader::GetVertexCode()
		{
			return m_sVertexCode;
		}

		void IShader::SetFragmentCode(IString Code)
		{
			m_sFragmentCode=Code;
			m_bNeedCompile=true;
		}

		void IShader::SetVertexCode(IString Code)
		{
			m_sVertexCode=Code;
			m_bNeedCompile=true;
		}

		IString IShader::ReturnError()
		{
			if (m_pShaderManager)
				return m_pShaderManager->ReturnError();
			else return "";
		}

		int IShader::GetProgram() 
		{
			return m_iProgram; 
		}

		bool IShader::IsCompiled()
		{
			return m_iProgram && !m_bNeedCompile;
		}

		bool IShader::Compile()
		{
			if (m_pShaderManager)
			{
				m_bNeedCompile=false;
				bool OK = m_pShaderManager->Compile(this);
				if (!OK)
					ILogger::LogError("%s\n",ReturnError().c_str());
				return OK;
			}
			else return false;
		}

		void IShader::UseShader()
		{
			if (m_pShaderManager)
			{
				m_pShaderManager->UseShader(this);
			}
		}

		bool IShader::SetParameterInt(IString Parameter, int value)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameterInt(this, Parameter, value);
			return false;
		}

		bool IShader::SetParameterFloat(IString Parameter, float value)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameterFloat(this, Parameter, value);
			return false;
		}

		bool IShader::SetParameter2Float(IString Parameter, float value1, float value2)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameter2Float(this, Parameter, value1, value2);
			return false;
		}

		bool IShader::SetParameter2Float(IString Parameter, float value[2])
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameter2Float(this, Parameter, value);
			return false;
		}

		bool IShader::SetParameter3Float(IString Parameter, float value1, float value2, float value3)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameter3Float(this, Parameter, value1, value2, value3);
			return false;
		}

		bool IShader::SetParameter3Float(IString Parameter, float value[3])
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameter3Float(this, Parameter, value);
			return false;
		}

		bool IShader::SetParameter4Float(IString Parameter, float value1, float value2, float value3, float value4)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameter4Float(this, Parameter, value1, value2, value3, value4);
			return false;
		}

		bool IShader::SetParameter4Float(IString Parameter, float value[4])
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameter4Float(this, Parameter, value);
			return false;
		}

		bool IShader::SetParameterMatrix22(IString Parameter, IMatrix& mat)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameterMatrix22(this, Parameter, mat);
			return false;
		}

		bool IShader::SetParameterMatrix33(IString Parameter, IMatrix& mat)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameterMatrix33(this, Parameter, mat);
			return false;
		}

		bool IShader::SetParameterMatrix44(IString Parameter, IMatrix& mat)
		{
			if (m_pShaderManager)
				return m_pShaderManager->SetParameterMatrix44(this, Parameter, mat);
			return false;
		}
	}
}
