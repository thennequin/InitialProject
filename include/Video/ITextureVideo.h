//---------------------------------------------
//                Initial Engine
//                Texture Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITEXTUREVIDEO_HEADER_
#define _ITEXTUREVIDEO_HEADER_
#include "Config.h"

#include "Core/IString.h"
#include "Video/ITexture.h"
#include "Core/IImage.h"

#if USE_BINK
#include <bink.h>
#endif

namespace Initial
{
	class ITextureVideo : public ITexture
	{
		friend class ITextureManager;
	public:
		~ITextureVideo();

		virtual void UpdateImage();

		void Play();
		void Pause();
		void Stop();
		void SetPos(float time);
		float GetLength();
		float GetPos();		
	protected:
		ITextureVideo(ITextureManager *manager, IString filename=IString(""));
	protected:
#if USE_BINK
		HBINK Bink;
#endif
		IImage m_pBuffer;
	};
}

#endif
