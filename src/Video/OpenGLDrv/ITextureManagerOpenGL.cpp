//---------------------------------------------
//                Initial Engine
//         OpenGL Texture source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/OpenGLDrv/ITextureManagerOpenGL.h"
#include "Core/IImage.h"
#include "Video/ITextureVideo.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace Initial;

ITextureManagerOpenGL::ITextureManagerOpenGL(IRenderDriver *device)
: ITextureManager(device)
{

}

bool ITextureManagerOpenGL::SetTextureData(ITexture *tex, void *data, int width, int height, int cpp)
{
	//IMutexLocker lock(m_mMutexTex);
	if (tex && data && width>0 && height>0)
	{
		glBindTexture(GL_TEXTURE_2D, tex->GetTextureId());

		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

		int CPP=0;
		bool Error=false;
		if (cpp==3)
			CPP=GL_RGB;
		else if (cpp==4)
			CPP=GL_RGBA;
		else 
			Error=true;

		if (Error==false)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, CPP, GL_UNSIGNED_BYTE, data);
			//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, CPP, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, NULL);

			return true;
		}
		glBindTexture(GL_TEXTURE_2D, NULL);
		return false;
	}
}

ITexture* ITextureManagerOpenGL::CreateTexture(int Width,int Height, TextureFormat Format)
{
	//IMutexLocker lock(m_mMutexTex);
	if (Width && Height && Format)
	{
		ITexture *NewTex = NewTexture(this);
		if (NewTex)
		{
			unsigned int TextureId;
			glGenTextures(1,&TextureId);
			if (TextureId)
			{
				SetTextureId(NewTex,TextureId);

				int CPP=GL_RGB;
				bool Error=false;
				if (Format==TF_RGBA)
					CPP=GL_RGBA;
				else if (Format==TF_RED)
					CPP=GL_RED;
				else if (Format==TF_GREEN)
					CPP=GL_GREEN;
				else if (Format==TF_BLUE)
					CPP=GL_BLUE;
				else if (Format==TF_ALPHA)
					CPP=GL_RGBA;
				else if (Format==TF_DEPTH)
					CPP=GL_DEPTH_COMPONENT;

				GLuint *data = new GLuint[Width*Height*4];

				glBindTexture(GL_TEXTURE_2D, TextureId);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, CPP, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				//gluBuild2DMipmaps(GL_TEXTURE_2D, CPP, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
				delete[] data;
			}else{
				delete NewTex;
				NewTex=NULL;
			}
		}
		return NewTex;
	}
}

void ITextureManagerOpenGL::BindTexture(ITexture *tex, int pos)
{
	//IMutexLocker lock(m_mMutexTex);
	glActiveTexture(GL_TEXTURE0 + pos);
	if (tex)
	{
		glBindTexture(GL_TEXTURE_2D,tex->GetTextureId());
		//printf("Bind texture %s %d\n",tex->GetFilename().c_str(),pos);
	}else
		glBindTexture(GL_TEXTURE_2D,NULL);
}
