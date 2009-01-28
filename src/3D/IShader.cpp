//---------------------------------------------
//                Initial Engine
//             Shader source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/IShader.h"

using namespace Initial;

IShader::IShader(IShaderManager *manager, IString vertex, IString fragment)
{
	m_sFragmentCode=fragment;
	m_sVertexCode=vertex;
	m_pShaderManager=manager;

	m_iFragmentShader=0;
	m_iVertexShader=0;
	m_iProgram=0;
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
}

void IShader::SetVertexCode(IString Code)
{
	m_sVertexCode=Code;
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
	return m_iProgram;
}

bool IShader::Compile()
{
	if (m_pShaderManager)
		return m_pShaderManager->Compile(this);
	else return false;
}