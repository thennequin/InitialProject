//---------------------------------------------
//                Initial Engine
//           Frame buffer (FBO) Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IFRAMEBUFFER_HEADER_
#define _IFRAMEBUFFER_HEADER_

#include "Initial/Core/IArray.h"
#include "Initial/Ressource/ITexture.h"

namespace Initial
{
	namespace Video
	{
		class IRenderDriver;

		class IFrameBuffer
		{
		public:
			IFrameBuffer(IRenderDriver* driver);

			void AttachTexture(int pos,ITexture *tex);
			void AttachDepthTexture(ITexture *tex);
			ITexture *GetTexture(int pos);
			ITexture *GetDepthTexture();

			unsigned int GetBufferId();
		protected:
			
		protected:
			unsigned int m_iBuffer;

			IRenderDriver *m_pDriver;

			ITexture *m_pTextureDepth;
			ITexture *m_pTexture[16];
		};
	}
}

#endif
