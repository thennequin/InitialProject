//---------------------------------------------
//                Initial Engine
//         Texture Manager source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Video/ITextureManager.h"
#include "Initial/Video/ITextureVideo.h"

using namespace Initial::Core;
using namespace Initial::Format;

namespace Initial
{
	namespace Video
	{

		ITextureManager::ITextureManager(IRenderDriver *device)
		{
			m_pDevice=device;
			Run();
		}

		ITexture* ITextureManager::NewTexture(ITextureManager* manager) 
		{ 
			return new ITexture(manager); 
		}

		ITexture* ITextureManager::NewVideoTexture(ITextureManager* manager) 
		{ 
			return new ITextureVideo(manager); 
		}

		void ITextureManager::SetTexture(ITexture *tex,void *ptr)
		{
			if (tex)
				tex->m_pTexture=ptr;
		}

		void ITextureManager::SetTextureSize(ITexture *tex,int width, int height)
		{
			if (tex)
			{
				//printf("%d %d\n",width,height);
				tex->m_iWidth=width;
				tex->m_iHeight=height;
			}
		}

		ITexture* ITextureManager::LoadTexture(IString filename)
		{
			ITexture *tex = new ITexture(this);
			tex->m_pTexture = GenTexture();
			IImageITX *itx = IImageITX::LoadITX(filename);
			if (itx)
			{		
				//SetCompressedTextureData(tex,itx);
				tex->m_bCompressed=true;
				tex->m_pTempData=itx;
				tex->SetFilename(filename);
				//delete itx;
				return tex;
			}

			IImage *image = new IImage(filename);
			if (image && image->GetWidth() && image->GetHeight())
			{
				//SetTextureData(tex,image->GetData(),image->GetWidth(),image->GetHeight(),image->GetFormat() == 4 ? ITF_RGBA : ITF_RGB,image->GetFormat());
				tex->m_bCompressed=false;
				tex->m_pTempData=image;
				tex->SetFilename(filename);
				//delete image;
				return tex;				
			}

			return NULL;
		}

		ITexture* ITextureManager::LoadTexture(void *data, int width, int height, int cpp)
		{
			if (data && width>0 && height>0)
			{	
				ITexture *NewTex = CreateTexture(width, height, cpp == 4 ? ITF_RGBA : ITF_RGB);
				if (NewTex)
				{
					if (SetTextureData(NewTex,data,width,height,cpp == 4 ? ITF_RGBA : ITF_RGB,(ImageFormat)cpp))
						return NewTex;		
					else
						delete NewTex;
				}
			}
			return NULL;
		}

		ITexture* ITextureManager::LoadVideo(IString filename)
		{	
			ITexture *NewTex;

			NewTex =  new ITextureVideo(this,filename);
			NewTex->SetFilename(filename);

			return NewTex;
		}

		bool ITextureManager::SetTextureData(ITexture *tex, IImage& image)
		{
			return SetTextureData(tex,image.GetData(),image.GetWidth(),image.GetHeight(),image.GetFormat() == 4 ? ITF_RGBA : ITF_RGB,image.GetFormat());
		}

		ITexture* ITextureManager::CreateTexture(int Width,int Height, TextureFormat Format)
		{
			//IMutexLocker lock(m_mMutexTex);
			if (Width && Height && Format)
			{
				ITexture *NewTex = NewTexture(this);
				if (NewTex)
				{
					void *ptr = GenTexture();
					if (ptr)
					{
						SetTexture(NewTex,ptr);

						ImageFormat CPP2=IIF_GREY;
						//if (Format==ITF_RGB || Format==ITF_RGBA || Format==ITF_RGB32F || Format==ITF_RGBA32F || Format==ITF_RGBA16F)
							CPP2=IIF_RGB;
						/*else */if (Format==ITF_DEPTH || Format==ITF_DEPTH24)
							CPP2=IIF_GREY;

						SetTextureData(NewTex,0,Width,Height,Format,CPP2);
					}else{
						delete NewTex;
						NewTex=NULL;
					}
				}
				return NewTex;
			}
			return NULL;
		}


		void ITextureManager::ThreadFunc()
		{
			return;
			printf("ITextureManager thread Ok\n");
			while (1)
			{
				_sleep(10);
				//IMutexLocker lock2(m_mMutexList);
				for (unsigned int i=0;i<TextureList.Count();i++)
				{
					//TextureList[i]->UpdateImage();
				}
			}
		}
	}
}
