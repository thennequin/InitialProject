//---------------------------------------------
//                Initial Engine
//                Texture Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITEXTURE_HEADER_
#define _ITEXTURE_HEADER_

#include "Core/IString.h"

namespace Initial
{
	class ITexture
	{
		friend class ITextureManager;
	public:
		virtual void UpdateImage();

		unsigned int GetTextureId();
		IString GetFilename();
		ITextureManager *GetTextureManager();
	protected:
		ITexture(ITextureManager *manager, IString filename=IString(""));
	protected:
		IString m_sFilename;
		unsigned int m_iTextureId;

		ITextureManager *m_pTextureManager;
	};
}

#endif
