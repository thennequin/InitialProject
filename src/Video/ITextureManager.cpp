//---------------------------------------------
//                Initial Engine
//         Texture Manager source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/ITextureManager.h"
#include "Video/ITextureVideo.h"

using namespace Initial;
ITextureManager::ITextureManager(IRenderDriver *device)
{
	m_pDevice=device;
	Run();
}

unsigned int ITextureManager::GetTextureCount()
{
	IMutexLocker lock(m_mMutexList);
	return TextureList.Count();
}

ITexture* ITextureManager::GetTexture(unsigned int id)
{
	IMutexLocker lock(m_mMutexList);
	return TextureList[id];
}

ITexture* ITextureManager::GetTexture(IString filename)
{
	IMutexLocker lock(m_mMutexList);
	ITexture *tex=NULL;
	for (unsigned int i=0;i<TextureList.Count();i++)
	{
		if (TextureList[i])
		{
			if (TextureList[i]->GetFilename()==filename)
				return TextureList[i];
		}
	}
	return NULL;
}

void ITextureManager::AddTexture(ITexture* tex)
{
	IMutexLocker lock(m_mMutexList);
	if (tex)
		TextureList.Add(tex);
}

ITexture* ITextureManager::NewTexture(ITextureManager* manager) 
{ 
	return new ITexture(manager); 
}

ITexture* ITextureManager::NewVideoTexture(ITextureManager* manager) 
{ 
	return new ITextureVideo(manager); 
}


void ITextureManager::SetTextureId(ITexture *tex,int Id)
{
	if (tex)
		tex->m_iTextureId=Id;
}

void ITextureManager::SetTextureFilename(ITexture *tex, IString filename)
{
	if (tex)
		tex->m_sFilename=filename;
}

ITexture* ITextureManager::LoadTexture(IString filename)
{
	ITexture *NewTex=(ITexture*)GetTexture(filename);
	if (NewTex)
		return (ITexture*)NewTex;

	IImage image(filename);
	if (image.GetWidth() && image.GetHeight())
	{
		NewTex = LoadTexture(image.GetData(), image.GetWidth(), image.GetHeight(), image.GetFormat());
		SetTextureFilename(NewTex, filename);
		return NewTex;
	}else
		return NULL;
}

ITexture* ITextureManager::LoadTexture(void *data, int width, int height, int cpp)
{
	if (data && width>0 && height>0)
	{	
		ITexture *NewTex = CreateTexture(width, height, cpp == 4 ? TF_RGBA : TF_RGB);
		if (NewTex)
		{
			if (SetTextureData(NewTex,data,width,height,cpp))
				return NewTex;		
			else
				delete NewTex;
		}
	}
	return NULL;
}

ITexture* ITextureManager::LoadVideo(IString filename)
{	
	ITexture *NewTex=(ITexture*)GetTexture(filename);
	if (NewTex)
		return (ITexture*)NewTex;

	NewTex =  new ITextureVideo(this,filename);
	//SetTextureFilename(NewTex, filename);

	return NewTex;
}

bool ITextureManager::SetTextureData(ITexture *tex, IImage& image)
{
	return SetTextureData(tex,image.GetData(),image.GetWidth(),image.GetHeight(),image.GetFormat());
}


void ITextureManager::ThreadFunc()
{
	printf("ITextureManager thread Ok\n");
	while (1)
	{
		_sleep(10);
		IMutexLocker lock2(m_mMutexList);
		for (unsigned int i=0;i<TextureList.Count();i++)
		{
			//TextureList[i]->UpdateImage();
		}
	}
}