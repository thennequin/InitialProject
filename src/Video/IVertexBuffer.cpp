//---------------------------------------------
//                Initial Engine
//          VBO Manager source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Video/IVertexBuffer.h"
#include "Initial/Video/IRenderDriver.h"


namespace Initial
{
	namespace Video
	{
		IVertexBuffer::IVertexBuffer(IRenderDriver *device)
		{
			m_pDevice=device;
			if (m_pDevice && m_pDevice->GetVBOManager())
			{
				m_iBufferVertex = m_pDevice->GetVBOManager()->_GenBuffer();
				m_iBufferNormal = m_pDevice->GetVBOManager()->_GenBuffer();
				m_iBufferCoord = m_pDevice->GetVBOManager()->_GenBuffer();
				m_iBufferIndex = m_pDevice->GetVBOManager()->_GenBuffer();
				m_iBufferTangent = m_pDevice->GetVBOManager()->_GenBuffer();
			}
		}

		void IVertexBuffer::SetBufferData(VBOBufferType buffer,void *Data, int Datasize, VBOMode mode)
		{
			if (m_pDevice && m_pDevice->GetVBOManager())
			{
				m_pDevice->GetVBOManager()->SetVBOData(this,buffer,Data,Datasize,mode);
			}
		}

		void IVertexBuffer::Draw()
		{
			if (m_pDevice && m_pDevice->GetVBOManager())
			{
				m_pDevice->GetVBOManager()->Draw(this);
			}
		}

		void IVertexBuffer::SetIndexCount(UINT count)
		{
			m_iIndexCount=count;
		}
	}
}
