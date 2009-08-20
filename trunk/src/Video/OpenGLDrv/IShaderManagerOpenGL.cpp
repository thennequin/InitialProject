//---------------------------------------------
//                Initial Engine
//          OpenGL Shader source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Video/OpenGLDrv/IShaderManagerOpenGL.h"

#include <windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace Initial::Core;
using namespace Initial::Math;

namespace Initial
{
	namespace Video
	{
		/*IShaderOpenGL::IShaderOpenGL(IString vertex, IString fragment)
		: IShader(vertex, fragment)
		{
			m_iFragmentShader=0;
			m_iVertexShader=0;
			m_iProgram=0;
		}

		IShaderOpenGL::~IShaderOpenGL()
		{
			if (m_iFragmentShader)
			{
				glDeleteShader(m_iFragmentShader);
			}

			if (m_iVertexShader)
			{
				glDeleteShader(m_iVertexShader);
			}

			if (m_iProgram)
			{
				glDeleteProgram(m_iProgram);
			}
		}*/



		IShaderManagerOpenGL::IShaderManagerOpenGL(IRenderDriver *device)
		{
			m_pDevice=device;
		}

		IShaderManagerOpenGL::~IShaderManagerOpenGL()
		{

		}

		IShader* IShaderManagerOpenGL::LoadShader(IString vertex, IString fragment)
		{
			if (vertex!="" || fragment!="")
			{
				IShader *shader = NewShader(this);
				shader->SetFragmentCode(fragment);
				shader->SetVertexCode(vertex);
				/*if (!Compile(shader))
					printf("%s\n",ReturnError());*/
				return shader;
			}
			return NULL;
		}

		void IShaderManagerOpenGL::DeleteShader(IShader *shader)
		{
			if (shader)
			{
				if (GetFragmentId(shader))
				{
					glDeleteShader(GetFragmentId(shader));
					SetFragmentId(shader,0);
				}

				if (GetVertexId(shader))
				{
					glDeleteShader(GetVertexId(shader));
					SetVertexId(shader,0);
				}

				if (GetProgramId(shader))
				{
					glDeleteProgram(GetProgramId(shader));
					SetProgramId(shader,0);
				}
			}
		}

		bool IShaderManagerOpenGL::Compile(IShader *shader)
		{
			if (!shader)
				return false;
			GLint result;
			char szBuffer[1024];
			unsigned int FragmentId = GetFragmentId(shader);
			unsigned int VertexId = GetVertexId(shader);
			unsigned int ProgramId = GetProgramId(shader);
			bool OK=true;

			//Vertex Shader
			if (!VertexId && shader->GetVertexCode()!="")
			{
				VertexId = glCreateShader(GL_VERTEX_SHADER);
				SetVertexId(shader,VertexId);
			}

			if (VertexId)
			{
				glShaderSource(VertexId, 1, (const GLchar**)&(shader->GetVertexCode()), NULL);	
				glCompileShader(VertexId);

				glGetObjectParameterivARB(VertexId, GL_OBJECT_COMPILE_STATUS_ARB, &result);
				if (result == GL_FALSE)
				{
					glGetInfoLogARB (VertexId, sizeof (szBuffer), 0, szBuffer);
					m_sError.Printf("Vertex Error : \n %s", szBuffer);
					OK=false;
				}
			}

			//Fragment shader
			if (!FragmentId && shader->GetFragmentCode()!="")
			{
				FragmentId = glCreateShader(GL_FRAGMENT_SHADER);
				SetFragmentId(shader,FragmentId);
			}

			if (FragmentId)
			{
				//printf("%s\n",shader->GetFragmentCode().c_str());
				glShaderSource(FragmentId, 1, (const GLchar**)&(shader->GetFragmentCode()), NULL);
				glCompileShader(FragmentId);

				glGetObjectParameterivARB(FragmentId, GL_OBJECT_COMPILE_STATUS_ARB, &result);
				
				if (result == GL_FALSE)
				{
					glGetInfoLogARB (FragmentId, sizeof (szBuffer), 0, szBuffer);
					m_sError.Printf("%s\nFragment Error :\n %s",m_sError.c_str(), szBuffer);
					OK=false;
				}
			}

			//Program Shader
			if (!ProgramId && (VertexId || FragmentId))
			{
				ProgramId = glCreateProgram();
				SetProgramId(shader,ProgramId);
			}

			if (ProgramId)
			{
				if (VertexId)
					glAttachObjectARB(ProgramId,VertexId);

				if (FragmentId)
					glAttachObjectARB(ProgramId,FragmentId);

				glLinkProgram(ProgramId);

				glGetObjectParameterivARB(ProgramId, GL_OBJECT_LINK_STATUS_ARB, &result);
				if (result == GL_FALSE)
				{
					glGetInfoLogARB (ProgramId, sizeof (szBuffer), 0, szBuffer);
					m_sError.Printf("%s\nProgram Error : \n %s", m_sError.c_str(), szBuffer);
					//glDeleteShader(shader->shaderFrag);
					//glDeleteShader(shader->shaderVert);
					OK=false;
				}

				if (OK)
				{
					glUseProgramObjectARB(ProgramId);
					IString Var;
					GLint uniform_location;
					for (int i=0;i<8;i++)
					{
						Var.Printf("Texture%d",i);
						uniform_location = glGetUniformLocationARB(ProgramId, Var.c_str());
						if (uniform_location!=-1)
							glUniform1iARB(uniform_location, i);
					}

					uniform_location = glGetUniformLocationARB(ProgramId, "ScreenDepth");
					if (uniform_location!=-1)
						glUniform1iARB(uniform_location, 8);
					uniform_location = glGetUniformLocationARB(ProgramId, "ScreenColor");
					if (uniform_location!=-1)
						glUniform1iARB(uniform_location, 9);

					/*IString temp;
					temp.AllocBuffer(1024);
					glGetShaderSource(FragmentId,1024,NULL,temp.c_str());
					printf("%s\n",temp.c_str());*/
				}
			}

			return OK;
		}

		bool IShaderManagerOpenGL::UseShader(IShader *shader)
		{
			if (shader)
			{
				bool Ok=true;
				if (!shader->IsCompiled())
					Ok = shader->Compile();
				glUseProgramObjectARB(shader->GetProgram());
				return Ok;
			}else
				glUseProgramObjectARB(0);
			return true;
		}

		bool IShaderManagerOpenGL::SetParameterInt(IShader *shader, IString Parameter, int value)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniform1iARB(uniform_location, value);
				return true;
			}
			return false;
		}

		bool IShaderManagerOpenGL::SetParameterFloat(IShader *shader, IString Parameter, float value)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniform1fARB(uniform_location, value);
				return true;
			}
			return false;
		}

		bool IShaderManagerOpenGL::SetParameter2Float(IShader *shader, IString Parameter, float value1, float value2)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniform2fARB(uniform_location, value1, value2);
				return true;
			}
			return false;
		}

		bool IShaderManagerOpenGL::SetParameter2Float(IShader *shader, IString Parameter, float value[2])
		{
			return SetParameter2Float(shader, Parameter, value[0], value[1]);
		}

		bool IShaderManagerOpenGL::SetParameter3Float(IShader *shader, IString Parameter, float value1, float value2, float value3)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniform3fARB(uniform_location, value1, value2, value3);
				return true;
			}
			return false;
		}

		bool IShaderManagerOpenGL::SetParameter3Float(IShader *shader, IString Parameter, float value[3])
		{
			return SetParameter3Float(shader, Parameter, value[0], value[1], value[2]);
		}

		bool IShaderManagerOpenGL::SetParameter4Float(IShader *shader, IString Parameter, float value1, float value2, float value3, float value4)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniform4fARB(uniform_location, value1, value2, value3, value4);
				return true;
			}
			return false;
		}

		bool IShaderManagerOpenGL::SetParameter4Float(IShader *shader, IString Parameter, float value[4])
		{
			return SetParameter4Float(shader, Parameter, value[0], value[1], value[2], value[3]);
		}


		bool IShaderManagerOpenGL::SetParameterMatrix22(IShader *shader, IString Parameter, IMatrix& mat)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniformMatrix2fvARB(uniform_location, 1, false, mat.GetFloatValues());
				return true;
			}
			return false;
		}

		bool IShaderManagerOpenGL::SetParameterMatrix33(IShader *shader, IString Parameter, IMatrix& mat)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniformMatrix3fvARB(uniform_location, 1, false, mat.GetFloatValues());
				return true;
			}
			return false;
		}

		bool IShaderManagerOpenGL::SetParameterMatrix44(IShader *shader, IString Parameter, IMatrix& mat)
		{
			if (shader)
			{
				glUseProgramObjectARB(shader->GetProgram());
				GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter.c_str());
				if (uniform_location==-1) return false;
				glUniformMatrix4fvARB(uniform_location, 1, false, mat.GetFloatValues());
				return true;
			}
			return false;
		}
	}
}
