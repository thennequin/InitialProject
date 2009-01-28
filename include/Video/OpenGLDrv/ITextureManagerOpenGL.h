//---------------------------------------------
//                Initial Engine
//            OpenGL Texture  Module
//
//            By Thibault HENNEQUIN
//              January 12th  2008
//---------------------------------------------

#ifndef _ITEXTUREMANAGEROPENGL_HEADER_
#define _ITEXTUREMANAGEROPENGL_HEADER_

#include "Video/ITexture.h"
#include "Video/ITextureManager.h"
#include "IRenderDriverOpenGL.h"

namespace Initial
{

	class ITextureManagerOpenGL : public ITextureManager
	{
		friend IRenderDriverOpenGL;
	public:
		virtual ITexture* CreateTexture(int Width,int Height, TextureFormat Format=TF_RGBA);
		virtual void BindTexture(ITexture *tex, int pos);
		virtual bool SetTextureData(ITexture *tex, void *data, int width, int height, int cpp);
	protected:
		ITextureManagerOpenGL(IRenderDriver *device=NULL);
	protected:
		IRenderDriver *m_pDevice;
	};

}

#endif
