//---------------------------------------------
//                Initial Engine
//                Texture Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ITEXTUREVIDEO_HEADER_
#define _ITEXTUREVIDEO_HEADER_
#include "Initial/Config.h"

#include "Initial/Core/IString.h"
#include "Initial/Video/ITexture.h"
#include "Initial/Core/IImage.h"

#ifdef _ITEXTUREVIDEO_COMPILE_
	#if USE_BINK
#include <bink.h>
	#endif
#endif

namespace Initial
{
	namespace Video
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
			ITextureVideo(ITextureManager *manager, Core::IString filename=Core::IString(""));
		protected:
#ifdef _ITEXTUREVIDEO_COMPILE_
	#if USE_BINK
			HBINK Bink;
	#endif
#endif
			Core::IImage m_pBuffer;
		};
	}
}

#endif
