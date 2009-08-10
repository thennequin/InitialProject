//---------------------------------------------
//                Initial Engine
//            OpenGL Texture  Module
//
//            By Thibault HENNEQUIN
//              January 12th  2008
//---------------------------------------------

#ifndef _ITEXTUREMANAGEROPENGL_HEADER_
#define _ITEXTUREMANAGEROPENGL_HEADER_

#include "Initial/Video/ITexture.h"
#include "Initial/Video/ITextureManager.h"
#include "IRenderDriverOpenGL.h"

namespace Initial
{
	namespace Video
	{
		class ITextureManagerOpenGL : public ITextureManager
		{
			friend IRenderDriverOpenGL;
		public:
			virtual void BindTexture(ITexture *tex, int pos);
			virtual bool SetTextureData(ITexture *tex, void *data, int width, int height,  TextureFormat Format, ImageFormat ImageFormat=IIF_RGB);
			virtual bool SetCompressedTextureData(ITexture *tex, Format::IImageITX* itx);
		protected:
			ITextureManagerOpenGL(IRenderDriver *device=NULL);
			virtual void *GenTexture();
			virtual void ReleaseTexture(void *ptr);
		protected:
			IRenderDriver *m_pDevice;
		};
	}
}

#endif