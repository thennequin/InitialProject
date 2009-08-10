
#include "Initial/Video/IFrameBuffer.h"
#include "Initial/Video/IRenderDriver.h"

using namespace Initial::Core;

namespace Initial
{
	namespace Video
	{
		IFrameBuffer::IFrameBuffer(IRenderDriver* driver)
		{
			m_pDriver=driver;
			if (m_pDriver)
				m_iBuffer = m_pDriver->_NewFrameBuffer();
			for (int i=0;i<16;i++)
				m_pTexture[i]=NULL;
			m_pTextureDepth=NULL;
		}

		void IFrameBuffer::AttachTexture(int pos, ITexture *tex)
		{
			if (m_pDriver && pos>=0 && pos<16)
			{
				m_pDriver->_AttachTextureToFrameBuffer(pos,this,tex);
				m_pTexture[pos]=tex;
			}
		}

		void IFrameBuffer::AttachDepthTexture(ITexture *tex)
		{
			if (m_pDriver)
			{
				m_pDriver->_AttachDepthTextureToFrameBuffer(this,tex);
				m_pTextureDepth=tex;
			}
		}

		ITexture* IFrameBuffer::GetTexture(int pos)
		{
			if(pos>=0 && pos<16)
				return m_pTexture[pos];
			return NULL;
		}

		ITexture* IFrameBuffer::GetDepthTexture()
		{
			return m_pTextureDepth;
		}

		unsigned int IFrameBuffer::GetBufferId()
		{
			return m_iBuffer;
		}
	}
}
