//---------------------------------------------
//                Initial Engine
//          Shader Manager source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Video/IShaderManager.h"

#include "Initial/3D/ILight.h"

using namespace Initial::Core;

namespace Initial
{
	namespace Video
	{
		IShader* IShaderManager::NewShader(IShaderManager *manager)
		{
			IShader *shader = new IShader(manager,IString(),IString());
			m_pShaderList.Add(shader);
			return shader;
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

		void IShaderManager::SendLightsInfo()
		{
			return;
			IString str;
			//printf("light count %d\n",ILight::m_aLights.Count());
			for (unsigned int i=0;i<m_pShaderList.Count();i++)
			{
				if (m_pShaderList[i] && m_pShaderList[i]->IsCompiled())
				{
					//m_pShaderList[i]->UseShader();
					int ok = m_pShaderList[i]->SetParameterInt("m_nLightsCount",ILight::m_aLights.Count());
					//printf("Send light count %d\n",ok);
					if (ok)
						for (unsigned int j=0;j<ILight::m_aLights.Count();j++)
						{
							//Send color
							str.Printf("m_v3LightsColor[%d]",j);
							ok=m_pShaderList[i]->SetParameter3Float(str,ILight::m_aLights[j]->m_cColor);
							//Send position
							str.Printf("m_v3LightsPos[%d]",j);
							ok=m_pShaderList[i]->SetParameter3Float(str,ILight::m_aLights[j]->m_vPosition);
							//Send radius
							str.Printf("m_fLightsRadius[%d]",j);
							ok=m_pShaderList[i]->SetParameterFloat(str,ILight::m_aLights[j]->m_fRadius);
						}
				}
			}	
		}
	}
}