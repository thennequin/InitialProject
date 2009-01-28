//---------------------------------------------
//                Initial Engine
//          OpenGL Shader source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/OpenGLDrv/IShaderManagerOpenGL.h"

#include <windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace Initial;

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
		if (!Compile(shader))
			printf("%s\n",ReturnError());
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
	if (!VertexId && shader->GetVertexCode().Cmp(""))
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
	if (!FragmentId && shader->GetFragmentCode().Cmp(""))
	{
		FragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		SetFragmentId(shader,FragmentId);
	}

	if (FragmentId)
	{
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
				uniform_location = glGetUniformLocationARB(ProgramId, Var);
				if (uniform_location!=-1)
					glUniform1iARB(uniform_location, i);
			}
		}
	}

	return OK;
}

void IShaderManagerOpenGL::UseShader(IShader *shader)
{
	if (shader)
	{
		//printf("Use shader\n");
		if (!shader->IsCompiled())
			shader->Compile();
		glUseProgramObjectARB(shader->GetProgram());
	}else
		glUseProgramObjectARB(0);
}

void IShaderManagerOpenGL::SetParameterInt(IShader *shader, IString Parameter, int value)
{
	if (shader)
	{
		glUseProgramObjectARB(shader->GetProgram());
		GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter);
		glUniform1iARB(uniform_location, value);
	}
}

void IShaderManagerOpenGL::SetParameterFloat(IShader *shader, IString Parameter, float value)
{
	if (shader)
	{
		glUseProgramObjectARB(shader->GetProgram());
		GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter);
		glUniform1fARB(uniform_location, value);
	}
}

void IShaderManagerOpenGL::SetParameter2Float(IShader *shader, IString Parameter, float value1, float value2)
{
	if (shader)
	{
		glUseProgramObjectARB(shader->GetProgram());
		GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter);
		glUniform2fARB(uniform_location, value1, value2);
	}
}

void IShaderManagerOpenGL::SetParameter2Float(IShader *shader, IString Parameter, float value[2])
{
	SetParameter2Float(shader, Parameter, value[0], value[1]);
}

void IShaderManagerOpenGL::SetParameter3Float(IShader *shader, IString Parameter, float value1, float value2, float value3)
{
	if (shader)
	{
		glUseProgramObjectARB(shader->GetProgram());
		GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter);
		glUniform3fARB(uniform_location, value1, value2, value3);
	}
}

void IShaderManagerOpenGL::SetParameter3Float(IShader *shader, IString Parameter, float value[3])
{
	SetParameter3Float(shader, Parameter, value[0], value[1], value[2]);
}

void IShaderManagerOpenGL::SetParameter4Float(IShader *shader, IString Parameter, float value1, float value2, float value3, float value4)
{
	if (shader)
	{
		glUseProgramObjectARB(shader->GetProgram());
		GLint uniform_location = glGetUniformLocationARB(shader->GetProgram(), Parameter);
		glUniform4fARB(uniform_location, value1, value2, value3, value4);
	}
}

void IShaderManagerOpenGL::SetParameter4Float(IShader *shader, IString Parameter, float value[4])
{
	SetParameter4Float(shader, Parameter, value[0], value[1], value[2], value[3]);
}
