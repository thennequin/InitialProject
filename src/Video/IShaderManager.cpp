//---------------------------------------------
//                Initial Engine
//          Shader Manager source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/IShaderManager.h"

using namespace Initial;

IShader* IShaderManager::NewShader(IShaderManager *manager)
{
	return new IShader(manager,IString(),IString());
}

IString IShaderManager::ReturnError()
{
	return m_sError;
}

unsigned int IShaderManager::GetVertexId(IShader *shader)
{
	if (shader)
		return shader->m_iVertexShader;
	else return 0;
}

unsigned int IShaderManager::GetFragmentId(IShader *shader)
{
	if (shader)
		return shader->m_iFragmentShader;
	else return 0;
}

unsigned int IShaderManager::GetProgramId(IShader *shader)
{
	if (shader)
		return shader->m_iProgram;
	else return 0;
}

void IShaderManager::SetVertexId(IShader *shader, unsigned int id)
{
	if (shader)
		shader->m_iVertexShader=id;
}

void IShaderManager::SetFragmentId(IShader *shader, unsigned int id)
{
	if (shader)
		shader->m_iFragmentShader=id;
}

void IShaderManager::SetProgramId(IShader *shader, unsigned int id)
{
	if (shader)
		shader->m_iProgram=id;
}