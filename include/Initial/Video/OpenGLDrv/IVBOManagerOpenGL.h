//---------------------------------------------
//                Initial Engine
//              VBO Manager Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IVBOMANAGEROPENGL_HEADER_
#define _IVBOMANAGEROPENGL_HEADER_

#include "Initial/Config.h"
#include "Initial/Video/IVBOManager.h"

namespace Initial
{
	namespace Video
	{
		//Element array VBO
		class IVBOManagerOpenGL : public IVBOManager
		{
			friend class IRenderDriver;
			friend class IVertexBuffer;
		public:
			//IVertexBuffer(IDevice *device);

			virtual void SetVBOData(IVertexBuffer *vbo,VBOBufferType buffer, void *Data, int Datasize, VBOMode mode);
			virtual void Draw(IVertexBuffer *vbo);
		//protected:
			IVBOManagerOpenGL(IRenderDriver *device);
		protected:
			virtual UINT	_GenBuffer();
			virtual void	_DeleteBuffer(UINT buffer);
		};
	}
}

#endif