//---------------------------------------------
//                Initial Engine
//         OpenGL Texture source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/ILogger.h"
#include "Initial/Video/OpenGLDrv/ITextureManagerOpenGL.h"
#include "Initial/Core/IImage.h"
#include "Initial/Ressource/ITextureVideo.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace Initial::Format;

namespace Initial
{
	namespace Video
	{

		bool isExtensionSupported(const char *extension)
		{
		  const GLubyte *extensions = NULL;
		  const GLubyte *start;
		  GLubyte *where, *terminator;

		  /* Extension names should not have spaces. */
		  where = (GLubyte *) strchr(extension, ' ');
		  if (where || *extension == '\0')
			return 0;
		  extensions = glGetString(GL_EXTENSIONS);
		  //ILogger::LogDebug("OPENGL Ext \n%s\n",extensions);
		  /* It takes a bit of care to be fool-proof about parsing the
			 OpenGL extensions string. Don't be fooled by sub-strings,
			 etc. */
		  start = extensions;
		  for (;;) {
			where = (GLubyte *) strstr((const char *) start, extension);
			if (!where)
			  break;
			terminator = where + strlen(extension);
			if (where == start || *(where - 1) == ' ')
			  if (*terminator == ' ' || *terminator == '\0')
				return 1;
			start = terminator;
		  }
		  return 0;
		}

		ITextureManagerOpenGL::ITextureManagerOpenGL(IRenderDriver *device)
		: ITextureManager(device)
		{

		}

		bool ITextureManagerOpenGL::SetTextureData(ITexture *tex, void *data, int width, int height,  TextureFormat Format, ImageFormat InputCPP)
		{
			//IMutexLocker lock(m_mMutexTex);
			if (tex /*&& data */&& width>0 && height>0)
			{
				SetTextureSize(tex,width,height);
				glBindTexture(GL_TEXTURE_2D, *(GLuint*)tex->GetTexture());

				//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

				int CPP2=GL_DEPTH_COMPONENT;
				bool Error=false;
				if (InputCPP==IIF_RGB)
					CPP2=GL_RGB;
				else if (InputCPP==IIF_RGBA)
					CPP2=GL_RGBA;
				//else 
				//	Error=true;

				//if (Error==false)
				{
					int CPP=GL_RGB;
					bool Error=false;
					if (Format==ITF_RGBA)
						CPP=GL_RGBA;
					else if (Format==ITF_RED)
						CPP=GL_RED;
					else if (Format==ITF_GREEN)
						CPP=GL_GREEN;
					else if (Format==ITF_BLUE)
						CPP=GL_BLUE;
					else if (Format==ITF_ALPHA)
						CPP=GL_RGBA;
					else if (Format==ITF_DEPTH)
						CPP=GL_DEPTH_COMPONENT;
					else if (Format==ITF_DEPTH24)
						CPP=GL_DEPTH_COMPONENT24;
					else if (Format==ITF_DEPTH32)
						CPP=GL_DEPTH_COMPONENT32; // temporary
					else if (Format==ITF_RGB32F)
						CPP=GL_RGB32F_ARB; // GL_RGB32F_ARB
					else if (Format==ITF_RGBA32F)
						CPP=GL_RGBA32F_ARB; // GL_RGBA32F_ARB	/ GL_RGBA_FLOAT32_ATI			
					else if (Format==ITF_RGB16F)
						CPP=GL_RGB16F_ARB;
					else if (Format==ITF_RGBA16F)
						CPP=GL_RGBA16F_ARB;

					/*if (CPP==GL_RGB32F_ARB && isExtensionSupported("GL_RGB32F_ARB")==0)
					{
						ILogger::LogError("GL_RGB32F_ARB not supported\n");
						CPP = GL_RGB;
					}*/


					//if (CPP==GL_RGBA32F_ARB || CPP==GL_RGB32F_ARB || CPP==GL_RGBA_FLOAT32_ATI || CPP==GL_RGBA16F_ARB)
					//	CPP = GL_RGB16F_ARB;
					//if (CPP==GL_DEPTH_COMPONENT32 || CPP==GL_DEPTH_COMPONENT24)
					//	CPP = GL_DEPTH_COMPONENT;

					long inputtype=GL_UNSIGNED_BYTE;
					if (CPP==GL_RGBA32F_ARB || CPP==GL_RGB32F_ARB || CPP==GL_RGBA_FLOAT32_ATI || CPP==GL_RGBA16F_ARB || CPP==GL_RGB16F_ARB)
						inputtype=GL_HALF_FLOAT_ARB;


					glBindTexture(GL_TEXTURE_2D, *(GLuint*)tex->GetTexture());

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					if (CPP2==GL_DEPTH_COMPONENT)
					{
						glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
					}

					//glTexImage2D(GL_TEXTURE_2D, 0, CPP, Width, Height, 0, CPP, GL_FLOAT, 0);
					glTexImage2D(GL_TEXTURE_2D, 0, CPP, width, height, 0, CPP2, inputtype, data);
					//gluBuild2DMipmaps(GL_TEXTURE_2D, CPP, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
					//delete[] data;
					glBindTexture(GL_TEXTURE_2D, NULL);
					//tex->m_iWidth=width;
					//tex->m_iHeight=height;
					return true;
				}
				glBindTexture(GL_TEXTURE_2D, NULL);
				return false;
			}
			return false;
		}

		bool ITextureManagerOpenGL::SetCompressedTextureData(ITexture *tex, IImageITX* itx)
		{
			if (tex && itx)
			{
				glBindTexture(GL_TEXTURE_2D, *(GLuint*)tex->GetTexture());
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

				SetTextureSize(tex,itx->GetWidth(),itx->GetHeight());

				int blockSize=16;
				if (itx->GetFormat()==IImageITX::ITXF_DXT1 || itx->GetFormat()==IImageITX::ITXF_DXT1a || itx->GetFormat()==IImageITX::ITXF_DXT1nm)
					blockSize=8;

				int Format;
				switch (itx->GetFormat())
				{
				/*case IImageITX::ITXF_RGBA:
					Format=GL_COMPRESSED_RGBA_ARB;
					break;*/
				case IImageITX::ITXF_DXT1:
					Format=GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
					break;
				case IImageITX::ITXF_DXT1a:
					Format=GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
					break;
				case IImageITX::ITXF_DXT1nm:
					Format=GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
					break;
				case IImageITX::ITXF_DXT3:
					Format=GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
					break;
				case IImageITX::ITXF_DXT5:
				case IImageITX::ITXF_DXT5nm:
					Format=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
					break;
				case IImageITX::ITXF_ATI1:
				case IImageITX::ITXF_ATI2_3Dc:
					Format=GL_ATI_texture_compression_3dc;
					break;
				}

				int nSize;
				int nOffset = 0;
				int Width=itx->GetWidth();
				int Height=itx->GetHeight();
				int NumMipMaps=itx->GetMipMapCount();

				for( int i = 0; i < NumMipMaps; ++i )
				{
					if( Width  == 0 ) Width  = 1;
					if( Height == 0 ) Height = 1;

					nSize = ((Width+3)/4) * ((Height+3)/4) * blockSize;

					if (itx->GetFormat()==IImageITX::ITXF_RGBA)
					{
						glTexImage2D(GL_TEXTURE_2D, 
							i, 
							GL_RGBA, 
							Width, 
							Height, 
							0, 
							GL_BGRA_EXT, 
							GL_UNSIGNED_BYTE, 
							(unsigned char*)itx->GetData() + nOffset);
					}else{
						glCompressedTexImage2DARB( GL_TEXTURE_2D,
							i,
							Format,
							Width,
							Height,
							0,
							nSize,
							(unsigned char*)itx->GetData() + nOffset );
					}

					nOffset += nSize;

					// Half the image size for the next mip-map level...
					Width  = (Width  / 2);
					Height = (Height / 2);
				}

				glBindTexture(GL_TEXTURE_2D, 0);

				return true;
			}
			return false;
		}

		void ITextureManagerOpenGL::BindTexture(ITexture *tex, int pos)
		{
			//IMutexLocker lock(m_mMutexTex);
			glActiveTexture(GL_TEXTURE0 + pos);
			if (tex)
			{
				//if (!tex->m_bLoaded)
					tex->Load();
				glBindTexture(GL_TEXTURE_2D,*(GLuint*)tex->GetTexture());
				//printf("Bind texture %s %d\n",tex->GetFilename().c_str(),pos);
			}else
				glBindTexture(GL_TEXTURE_2D,NULL);
		}

		void* ITextureManagerOpenGL::GenTexture()
		{
			void *ptr = (void*)(new int);
			if (ptr)
			{
				glGenTextures(1,(GLuint*)ptr);
				if (*(int*)ptr)
					return ptr;
				delete ptr;
			}
			return NULL;
		}

		void ITextureManagerOpenGL::ReleaseTexture(void *ptr)
		{
			if (ptr)
			{
				glDeleteTextures(1,(GLuint*)ptr);
				delete ptr;
			}
		}
	}
}
