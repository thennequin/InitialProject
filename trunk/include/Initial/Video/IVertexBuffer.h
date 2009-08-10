//---------------------------------------------
//                Initial Engine
//                  VBO Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IVERTEXBUFFER_HEADER_
#define _IVERTEXBUFFER_HEADER_

#include "Initial/Config.h"
#include "Initial/IEnum.h"

//#define GL_VERTEX_ARRAY                   0x8074
//#define GL_NORMAL_ARRAY                   0x8075
//#define GL_COLOR_ARRAY                    0x8076
//#define GL_INDEX_ARRAY                    0x8077

//GL_ARRAY_BUFFER

/*GL_STREAM_DRAW*/

/*glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
glBufferData(GL_ARRAY_BUFFER, ColorSize, ColorData, GL_STREAM_DRAW);
glColorPointer(3, GL_UNSIGNED_BYTE, 0, 0);

glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STREAM_DRAW);
glVertexPointer(2, GL_FLOAT, 0, 0);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STREAM_DRAW);

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);

glDrawElements(GL_TRIANGLES, VertexCount, GL_UNSIGNED_INT, 0);

glDisableClientState(GL_COLOR_ARRAY);
glDisableClientState(GL_VERTEX_ARRAY);*/

namespace Initial
{
	namespace Video
	{
		class IRenderDriver;

		//Element array VBO
		class IVertexBuffer
		{
			friend class IVBOManager;
		public:
			IVertexBuffer(IRenderDriver *device);

			void SetBufferData(VBOBufferType buffer,void *Data, int Datasize, VBOMode mode);
			void SetIndexCount(UINT count);
			void Draw();
		protected:
			IRenderDriver *m_pDevice;
			int m_iBufferVertex; // Liste des vertex
			int m_iBufferNormal; // Liste des normals
			int m_iBufferCoord; // Liste des coordonées
			int m_iBufferTangent; // Liste des coordonées
			int m_iBufferIndex; // Index 
			UINT m_iIndexCount;
		};
	}
}

#endif