//---------------------------------------------
//                Initial Engine
//                Texture Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITEXTURE_HEADER_
#define _ITEXTURE_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/IRessource.h"

namespace Initial
{
	namespace Video
	{
		class ITexture : public IRessource
		{
			friend class ITextureManager;
		public:
			virtual void Load();
			virtual void Reload();

			virtual void UpdateImage();

			void *GetTexture();
			int GetWidth();
			int GetHeight();

			ITextureManager *GetTextureManager();
		protected:
			ITexture(ITextureManager *manager, Core::IString filename=Core::IString(""));
		protected:
			void *m_pTexture; // int for OpenGL, and LPDIRECT3DTEXTURE9 for Direct3D
			//unsigned int m_iTextureId;
			int m_iWidth;
			int m_iHeight;
			
			bool m_bLoaded;
			bool m_bCompressed;
			void *m_pTempData;
			//For empty

			ITextureManager *m_pTextureManager;
		};
	}
}

#endif
