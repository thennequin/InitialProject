//---------------------------------------------
//                Initial Engine
//             Texture source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Ressource/ITexture.h"
#include "Initial/Video/ITextureManager.h"

using namespace Initial::Core;

namespace Initial
{
	namespace Video
	{
		ITexture::ITexture(ITextureManager *manager, IString filename)
		: IRessource(filename)
		{
			m_pTextureManager=manager;
			//m_sFilename=filename;
			m_pTexture=NULL;

			m_bLoaded=false;
			m_bCompressed=false;
			m_pTempData=NULL;
		}

		void ITexture::Load()
		{
			if (!m_bLoaded && m_pTextureManager)
			{
				if (m_bCompressed)
				{
					m_pTextureManager->SetCompressedTextureData(this,(Initial::Format::IImageITX*)m_pTempData);
				}else{
					IImage *image = (IImage*)m_pTempData;
					if (image)
						m_pTextureManager->SetTextureData(this,image->GetData(),image->GetWidth(),image->GetHeight(),image->GetFormat() == 4 ? ITF_RGBA : ITF_RGB,image->GetFormat());
					/*else
						m_pTextureManager->SetTextureData(this,NULL,m_iWidth,m_iHeight,ITF_RGB,IIF_RGB);*/
				}

				delete m_pTempData;
				m_bLoaded=true;
			}
		}

		void ITexture::Reload()
		{

		}

		void* ITexture::GetTexture()
		{
			return m_pTexture;
		}

		int ITexture::GetWidth()
		{
			return m_iWidth;
		}

		int ITexture::GetHeight()
		{
			return m_iHeight;
		}

		ITextureManager* ITexture::GetTextureManager()
		{
			return m_pTextureManager;
		}

		void ITexture::UpdateImage()
		{
		}
	}
}
