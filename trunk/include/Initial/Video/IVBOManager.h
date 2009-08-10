//---------------------------------------------
//                Initial Engine
//                  VBO Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IVBOMANAGER_HEADER_
#define _IVBOMANAGER_HEADER_

#include "Initial/Config.h"
#include "Initial/IEnum.h"

#include "Initial/Video/IVertexBuffer.h"

namespace Initial
{
	namespace Video
	{
		class IRenderDriver;

		//Element array VBO
		class IVBOManager
		{
			friend class IVertexBuffer;
		public:
			//IVertexBuffer(IDevice *device);

			virtual void SetVBOData(IVertexBuffer *vbo,VBOBufferType buffer, void *Data, int Datasize, VBOMode mode)=0;
			virtual void Draw(IVertexBuffer *vbo)=0;
		protected:
			IVBOManager(IRenderDriver *device) { m_pDevice=device; }
			virtual UINT	_GenBuffer()=0;
			virtual void	_DeleteBuffer(UINT buffer)=0;

			UINT GetVertexBuffer(IVertexBuffer *vbo) { return vbo->m_iBufferVertex; }
			UINT GetNormalBuffer(IVertexBuffer *vbo) { return vbo->m_iBufferNormal; }
			UINT GetCoordBuffer(IVertexBuffer *vbo) { return vbo->m_iBufferCoord; }
			UINT GetIndexBuffer(IVertexBuffer *vbo) { return vbo->m_iBufferIndex; }
			UINT GetIndexCount(IVertexBuffer *vbo) { return vbo->m_iIndexCount; }
		protected:
			IRenderDriver *m_pDevice;
		};
	}
}

#endif