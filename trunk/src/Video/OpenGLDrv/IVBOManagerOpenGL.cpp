//---------------------------------------------
//                Initial Engine
//          VBO Manager source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Video/OpenGLDrv/IVBOManagerOpenGL.h"
#include "Initial/Video/IVertexBuffer.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace Initial
{
	namespace Video
	{
		IVBOManagerOpenGL::IVBOManagerOpenGL(IRenderDriver *device)
			: IVBOManager(device)
		{
		}

		void IVBOManagerOpenGL::SetVBOData(IVertexBuffer *vbo,VBOBufferType buffer, void *Data, int Datasize, VBOMode mode)
		{
			if (vbo)
			{
				UINT mode=GL_STREAM_DRAW;
				switch (mode)
				{
				case IVBOM_STREAM:
					mode=GL_STREAM_DRAW;
					break;
				case IVBOM_DYNAMIC:
					mode=GL_DYNAMIC_DRAW;
					break;
				case IVBOM_STATIC:
					mode=GL_STATIC_DRAW;
					break;
				}

				switch (buffer)
				{
				case IVBOBT_VERTEX:
					glBindBuffer(GL_ARRAY_BUFFER, GetVertexBuffer(vbo));
					glBufferData(GL_ARRAY_BUFFER, Datasize, Data, mode);
					glBindBuffer(GL_ARRAY_BUFFER, 0);	
					break;
				case IVBOBT_NORMALS:
					glBindBuffer(GL_ARRAY_BUFFER, GetNormalBuffer(vbo));
					glBufferData(GL_ARRAY_BUFFER, Datasize, Data, mode);
					glBindBuffer(GL_ARRAY_BUFFER, 0);	
					break;
				case IVBOBT_COORD:
					glBindBuffer(GL_ARRAY_BUFFER, GetCoordBuffer(vbo));
					glBufferData(GL_ARRAY_BUFFER, Datasize, Data, mode);
					glBindBuffer(GL_ARRAY_BUFFER, 0);	
					break;
				case IVBOBT_INDEX:
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndexBuffer(vbo));
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Datasize, Data, mode);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	
					break;	
				}			
			}
		}

		void IVBOManagerOpenGL::Draw(IVertexBuffer *vbo)
		{
			if (vbo)
			{
				glBindBuffer(GL_ARRAY_BUFFER, GetVertexBuffer(vbo));
				glVertexPointer(3, GL_FLOAT, 0, 0);

				glBindBuffer(GL_ARRAY_BUFFER, GetNormalBuffer(vbo));
				glNormalPointer(GL_FLOAT, 0, 0);

				glBindBuffer(GL_ARRAY_BUFFER, GetCoordBuffer(vbo));
				glTexCoordPointer(2, GL_FLOAT, 0, 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndexBuffer(vbo));

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glDrawElements(GL_TRIANGLES, GetIndexCount(vbo)*3, GL_UNSIGNED_INT, 0);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}

		UINT IVBOManagerOpenGL::_GenBuffer()
		{
			GLuint buf;
			glGenBuffers(1,&buf);
			return buf;
		}

		void IVBOManagerOpenGL::_DeleteBuffer(UINT buffer)
		{
			glDeleteBuffers(1,&buffer);
		}
	}
}
