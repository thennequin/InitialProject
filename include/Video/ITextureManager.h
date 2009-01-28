//---------------------------------------------
//                Initial Engine
//             Texture Manager Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITEXTUREMANAGER_HEADER_
#define _ITEXTUREMANAGER_HEADER_

#include "IEnum.h"
#include "Video/ITexture.h"
#include "Video/IRenderDriver.h"
#include "Core/IImage.h"

#include "Thread/IThread.h"

namespace Initial
{

	class ITextureManager : public IThread
	{
	public:
		ITextureManager(IRenderDriver *device);

		virtual ITexture* LoadTexture(IString filename);
		virtual ITexture* LoadTexture(void *data, int width, int height, int cpp);
		virtual ITexture* LoadVideo(IString filename);

		virtual ITexture* CreateTexture(int Width,int Height, TextureFormat Format=TF_RGBA)=0;
		virtual void BindTexture(ITexture *tex, int pos)=0;
		virtual bool SetTextureData(ITexture *tex, void *data, int width, int height, int cpp)=0;

		virtual bool SetTextureData(ITexture *tex, IImage& image);

		unsigned int GetTextureCount();
		ITexture* GetTexture(unsigned int id);
		ITexture* GetTexture(IString filename);
		void AddTexture(ITexture* tex);

		virtual void ThreadFunc();
		
	protected:
		ITexture* NewTexture(ITextureManager* manager);
		ITexture* NewVideoTexture(ITextureManager* manager);

		void SetTextureId(ITexture *tex,int Id);
		void SetTextureFilename(ITexture *tex, IString filename);
	protected:
		IRenderDriver *m_pDevice;

		IMutex m_mMutexList;
		IMutex m_mMutexTex;

		IArray<ITexture*> TextureList;
	};

}

#endif