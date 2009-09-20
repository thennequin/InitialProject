//---------------------------------------------
//                Initial Engine
//             Texture Manager Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITEXTUREMANAGER_HEADER_
#define _ITEXTUREMANAGER_HEADER_

#include "Initial/IEnum.h"
#include "Initial/Core/IString.h"
#include "Initial/Core/IImage.h"
#include "Initial/Ressource/ITexture.h"
#include "Initial/Video/IRenderDriver.h"
#include "Initial/Format/IImageITX.h"

#include "Initial/Thread/IThread.h"

namespace Initial
{
	namespace Video
	{
		class ITextureManager : public IThread
		{
		public:
			ITextureManager(IRenderDriver *device);

			virtual ITexture* LoadTexture(Core::IString filename);
			virtual ITexture* LoadTexture(void *data, int width, int height, int cpp);
			virtual ITexture* LoadVideo(Core::IString filename);

			virtual ITexture* CreateTexture(int Width,int Height, TextureFormat Format=ITF_RGBA);

			virtual void BindTexture(ITexture *tex, int pos)=0;
			virtual bool SetTextureData(ITexture *tex, void *data, int width, int height,  TextureFormat Format, ImageFormat InputCPP=IIF_RGB)=0;
			virtual bool SetCompressedTextureData(ITexture *tex, Format::IImageITX* itx)=0;
			virtual bool SetTextureData(ITexture *tex, Core::IImage& image);

			virtual void ThreadFunc();
			
		protected:
			ITexture* NewTexture(ITextureManager* manager);
			ITexture* NewVideoTexture(ITextureManager* manager);

			void SetTexture(ITexture *tex, void *ptr);
			void SetTextureSize(ITexture *tex,int width, int height);
			virtual void *GenTexture()=0;
			virtual void ReleaseTexture(void *ptr)=0;
		protected:
			IRenderDriver *m_pDevice;

			IMutex m_mMutexList;
			IMutex m_mMutexTex;

			Core::IArray<ITexture*> TextureList;
		};
	}
}

#endif