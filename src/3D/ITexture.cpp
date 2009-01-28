//---------------------------------------------
//                Initial Engine
//             Texture source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/ITexture.h"
#include "Video/ITextureManager.h"

using namespace Initial;

ITexture::ITexture(ITextureManager *manager, IString filename)
{
	m_pTextureManager=manager;
	m_sFilename=filename;

	if (manager)
		manager->AddTexture(this);
}

IString ITexture::GetFilename()
{
	return m_sFilename; 
}

unsigned int ITexture::GetTextureId()
{
	return m_iTextureId;
}

ITextureManager* ITexture::GetTextureManager()
{
	return m_pTextureManager;
}

void ITexture::UpdateImage()
{
}