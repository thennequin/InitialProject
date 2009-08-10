//---------------------------------------------
//                Initial Engine
//          OpenGL Device source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

//#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

#include <math.h>

#include "Initial/Video/OpenGLDrv/IRenderDriverOpenGL.h"
#include "Initial/Video/OpenGLDrv/ITextureManagerOpenGL.h"
#include "Initial/Video/OpenGLDrv/IShaderManagerOpenGL.h"
#include "Initial/Video/OpenGLDrv/IVBOManagerOpenGL.h"
#include "Initial/Video/IMaterial.h"

#include "Initial/ILogger.h"
#include "Initial/Math/IMath.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Math;
using namespace Initial::Video;

unsigned long GLext[] = {
	GL_TEXTURE_2D,
	GL_DEPTH_TEST,
	GL_BLEND,
	GL_DEPTH,
	GL_COLOR,
	GL_STENCIL_TEST,
};

unsigned long GLbuffer[] = {
	GL_COLOR_BUFFER_BIT,
	GL_DEPTH_BUFFER_BIT,
	GL_STENCIL_BUFFER_BIT,
};

unsigned long GLalphaFunc[] = {
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS,
};

unsigned long GLstencilOp[] = {
	GL_KEEP,
	GL_REPLACE,
	GL_INCR,
	GL_DECR,
};

unsigned long GLfrontFaceDirection[] = {
	GL_CW,
	GL_CCW,
};

unsigned long GLblendMode[] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA_SATURATE
};

IRenderDriverOpenGL::IRenderDriverOpenGL(IDevice *device, void *WinId, int Width, int Height, int Bits)
: IRenderDriver(device, WinId)
{
	hRC=NULL;							// Permanent Rendering Context
	hDC=NULL;							// Private GDI Device Context
	hWnd=*(HWND*)WinId;					// Holds Our Window Handle
	PixelFormat=0;

	//GetModuleHandle
	m_iWidth=Width;
	m_iHeight=Height;
	m_iBitDepth=Bits;
}

bool IRenderDriverOpenGL::InitDriver()
{
	// Set up ixel format descriptor with desired parameters
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,				// Version Number
		PFD_DRAW_TO_WINDOW |		// Format Must Support Window
		PFD_SUPPORT_OPENGL |		// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,		// Must Support Double Buffering
		PFD_TYPE_RGBA,			// Request An RGBA Format
		m_iBitDepth,				// Select Our Color Depth
		0, 0, 0, 0, 0, 0,		// Color Bits Ignored
		0,				// No Alpha Buffer
		0,				// Shift Bit Ignored
		0,				// No Accumulation Buffer
		0, 0, 0, 0,			// Accumulation Bits Ignored
		24,				// Z-Buffer (Depth Buffer)
		1,	// Stencil Buffer Depth
		0,				// No Auxiliary Buffer
		PFD_MAIN_PLANE,			// Main Drawing Layer
		0,				// Reserved
		0, 0, 0				// Layer Masks Ignored
	};

	GLuint PixelFormat;

	// get hdc
	hDC=GetDC(hWnd);
	if (!hDC)
	{
		ILogger::LogError("Cannot create a GL device context.\n");
		return false;
	}

	PixelFormat = ChoosePixelFormat(hDC, &pfd);

	// set pixel format
	if(!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		ILogger::LogError("Cannot set the pixel format.\n");
		return false;
	}

	// create rendering context
	hRC=wglCreateContext(hDC);
	if (!hRC)
	{
		ILogger::LogError("Cannot create a GL rendering context.\n");
		return false;
	}

	// activate rendering context
	if(!wglMakeCurrent(hDC, hRC))
	{
		ILogger::LogError("Cannot activate GL rendering context.\n");
		wglDeleteContext(hRC);
		return false;
	}

	GLenum err = glewInit();						// Init GLEW
	if (GLEW_OK != err)
	{
		ILogger::LogError("Error : %s\n", glewGetErrorString(err));
	}
	
	m_pTextureManager = new ITextureManagerOpenGL(this);
	m_pShaderManager = new IShaderManagerOpenGL(this);
	m_pVBOManager = new IVBOManagerOpenGL(this);

	SetCurrent();

	InitGL();

	_Inited();

	SwapBuffer();

	ILogger::LogDebug("OpenGL render created.\n");
	return true;
}

void IRenderDriverOpenGL::_ResizeDriver()
{
	SetCurrent();
	glViewport(0,0,m_iWidth,m_iHeight);
}

bool IRenderDriverOpenGL::InitGL()
{
	//glShadeModel(GL_SMOOTH);							// Enable Smooth Shading

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_FRAGMENT_PROGRAM_ARB);

	//glEnable( GL_POLYGON_SMOOTH );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA );

	/*glEnable(GL_POINT_SMOOTH );
	glEnable(GL_LINE_SMOOTH );
	glEnable(GL_POLYGON_SMOOTH );*/
	//glBlendFunc( GL_ONE, GL_ONE );
	//glDepthFunc( GL_EQUAL );

	//glEnable(GL_MINMAX);
	//glMinmax(GL_MINMAX, GL_RGB16F_ARB, GL_FALSE);
	//glMinmax(GL_MINMAX, GL_RGB32F_ARB, GL_FALSE);
	glDisable(GL_MINMAX);

	glViewport(0,0,m_iWidth,m_iHeight);
	return true;
}

void IRenderDriverOpenGL::SetCurrent()
{
	wglMakeCurrent(hDC,hRC);
}

void IRenderDriverOpenGL::_SetClearColor(Core::IColor color)
{
	glClearColor(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
}

void IRenderDriverOpenGL::_ClearBuffer(VideoBuffer buffer)
{
	if (buffer>=0 && buffer<IVB_MAX)
	{
		unsigned int Buffer = GLbuffer[buffer];
		glClear(Buffer);	
	}
}

void IRenderDriverOpenGL::BeginRender(IFrameBuffer *dest, bool renderDirectly, bool clearbuffer)
{
	m_lTime=GetTickCount();
	m_bInRender=true;
	m_iPolygonCount=0;
	m_iLineCount=0;
	//m_pCurrentMaterial=NULL;
	//if (m_bAntiAliasingEnable)
		//glEnable(GL_MULTISAMPLE_ARB);

	if (renderDirectly)
	{
		_UseFrameBuffer(0); //Screen
	}else if (dest)
	{
		_UseFrameBuffer(dest);
	}else{
		_UseFrameBuffer(m_pFrameBuffer); //FrameBuffer
	}

	if (clearbuffer)
	{
		//glClearColor((sin(GetTickCount()/1000.0)+1)*0.5,0.3,0.3,0);
		glClearColor(0,0,0,0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	}

	//glEnable(GL_NORMALIZE);
	/*_SetProjectionMatrix();
	//_PushMatrix();
	_LoadIdentity();
	if (m_pCamera)
		m_pCamera->UpdateView(this);
	_SetModelViewMatrix();
	_LoadIdentity();*/

	//if (m_pShaderManager)
		//m_pShaderManager->SendLightsInfo();
}

void IRenderDriverOpenGL::EndRender()
{
	long Time = GetTickCount();
	//if (m_pCamera)
	//	PostProcess(m_pCamera->GetUpdateViewMatrix());

	//glEnable(GL_MULTISAMPLE_ARB);
	m_bInRender=false;
	//ILogger::LogDebug("Render time %f\n",(GetTickCount()-m_lTime)/1000.0);
}

void IRenderDriverOpenGL::GetRenderInTexture(ITexture *rgba, ITexture *depth)
{
	if (depth)
	{
		glBindTexture(GL_TEXTURE_2D,*(GLuint*)depth->GetTexture());
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, m_iWidth, m_iHeight, 0);
		glBindTexture(GL_TEXTURE_2D,0);
	}
	
	if (rgba)
	{
		glBindTexture(GL_TEXTURE_2D,*(GLuint*)rgba->GetTexture());
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_iWidth, m_iHeight, 0);
		glBindTexture(GL_TEXTURE_2D,0);
	}
}

void IRenderDriverOpenGL::SwapBuffer()
{
	glFlush();
	SwapBuffers(hDC);
}

void IRenderDriverOpenGL::_SetModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
}

void IRenderDriverOpenGL::_SetProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
}

void IRenderDriverOpenGL::_SetTextureMatrix()
{
	glMatrixMode(GL_TEXTURE);
}

void IRenderDriverOpenGL::_SetIdentityMatrix()
{
	glLoadIdentity();
}

void IRenderDriverOpenGL::_PushMatrix()
{
	glPushMatrix();
}

void IRenderDriverOpenGL::_PopMatrix()
{
	glPopMatrix();
}

void IRenderDriverOpenGL::_LoadIdentity()
{
	glLoadIdentity();
}

void IRenderDriverOpenGL::_MultMatrix(IMatrix mat)
{
	glMultMatrixd(mat.GetValues());
}

IMatrix IRenderDriverOpenGL::_GetMatrix()
{
	GLint MatrixMode;
	glGetIntegerv(GL_MATRIX_MODE,&MatrixMode);

	if (MatrixMode==GL_PROJECTION)
		return _GetProjectionMatrix();
	else if (MatrixMode==GL_MODELVIEW)
		return _GetModelViewMatrix();
	else if (MatrixMode==GL_TEXTURE)
		return _GetTextureMatrix();
	
	return IMatrix(4,4);
}

Math::IMatrix IRenderDriverOpenGL::_GetProjectionMatrix()
{
	float matrix[4*4];
	glGetFloatv(GL_PROJECTION_MATRIX, (float*)matrix);
	return IMatrix(4,4,matrix).Transpose();
}

Math::IMatrix IRenderDriverOpenGL::_GetModelViewMatrix()
{
	float matrix[4*4];
	glGetFloatv(GL_MODELVIEW_MATRIX, (float*)matrix);
	return IMatrix(4,4,matrix).Transpose();
}

Math::IMatrix IRenderDriverOpenGL::_GetTextureMatrix()
{
	float matrix[4*4];
	glGetFloatv(GL_TEXTURE_MATRIX, (float*)matrix);
	return IMatrix(4,4,matrix).Transpose();
}

void IRenderDriverOpenGL::_SetMatrix(IMatrix mat)
{
	glLoadMatrixd(mat.GetValues());
}

void IRenderDriverOpenGL::_Translate(float x, float y, float z)
{
	glTranslatef(x,y,z);
}

void IRenderDriverOpenGL::_RotateX(float x)
{
	glRotatef(x,1,0,0);
}

void IRenderDriverOpenGL::_RotateY(float y)
{
	glRotatef(y,0,1,0);
}

void IRenderDriverOpenGL::_RotateZ(float z)
{
	glRotatef(z,0,0,1);
}

void IRenderDriverOpenGL::_Scale(float x, float y, float z)
{
	glScalef(x,y,z);
}

void IRenderDriverOpenGL::_DrawTriangle(ITriangle* polygon)
{	
	if (polygon)
	{
		m_iPolygonCount++;

		IVector3D *vertex = polygon->GetVertex();
		IVector2D *coord = polygon->GetCoords();
		IVector3D *normal = polygon->GetNormals();

		for (int i=0;i<3;i++)
		{
			glNormal3fv(normal[i]);
			glMultiTexCoord3fARB(GL_TEXTURE0_ARB,coord[i].GetU(),coord[i].GetV(),0);
			//glVertex3f(vertex[i][0],vertex[i][1],vertex[i][2]);
			//glMultiTexCoord2fvARB(GL_TEXTURE0_ARB,coord[i]);
			glVertex3fv(vertex[i]);
		}
	}
}

void IRenderDriverOpenGL::_DrawLine(const IVector3D& pt1,const IVector3D& pt2)
{
	m_iLineCount++;
	glBegin(GL_LINES);
		glVertex3fv(pt1);
		glVertex3fv(pt2);
	glEnd();
}

void IRenderDriverOpenGL::_SetColor(IColor color)
{
	//glColor4f(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
	glColor4f(color.GetRed()/1.0,color.GetGreen()/1.0,color.GetBlue()/1.0,color.GetAlpha());
}

void IRenderDriverOpenGL::_SetLineSize(float size)
{
	glLineWidth(size);
}

void IRenderDriverOpenGL::_StartTriangleDraw(bool Wireframe)
{
	//glLineWidth( Wireframe ? 2 : 1 );
	glBegin(Wireframe ? GL_LINE_LOOP : GL_TRIANGLES );
}

void IRenderDriverOpenGL::_EndPolyDraw()
{
	glEnd();
}

void IRenderDriverOpenGL::_DrawQuad(IVector3D a,IVector3D b,IVector3D c,IVector3D d)
{
	glBegin(GL_QUADS);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,1,0);
		glVertex3fv(a);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,0,0);
		glVertex3fv(b);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,0,1);
		glVertex3fv(c);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,1,1);
		glVertex3fv(d);
	glEnd();
}

void IRenderDriverOpenGL::_DrawQuadPP(float x, float y,float w, float h)
{
	/*glBegin(GL_QUADS);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,x+w,y);
		glVertex3s(x,y,1);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,x,y);
		glVertex3s(x+w,y,1);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,x,y+h);
		glVertex3s(x+w,y+h,1);
		glMultiTexCoord2sARB(GL_TEXTURE0_ARB,x+w,y+h);
		glVertex3s(x,y+h,1);
	glEnd();*/

	glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x,y);
		glVertex3f(x,y,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x+w,y);
		glVertex3f(x+w,y,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x+w,y+h);
		glVertex3f(x+w,y+h,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x,y+h);
		glVertex3f(x,y+h,1);
	glEnd();

	/*glBegin(GL_TRIANGLES);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x,y);
		glVertex3f(x,y,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x+w,y);
		glVertex3f(x+w,y,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x,y+h);
		glVertex3f(x,y+h,1);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x,y+h);
		glVertex3f(x,y+h,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x+w,y+h);
		glVertex3f(x+w,y+h,1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x+w,y);
		glVertex3f(x+w,y,1);
	glEnd();*/
}

void IRenderDriverOpenGL::_EnableExt(DeviceExt ext)
{
	if (ext>=0 && ext<IEXT_MAX)
		glEnable(GLext[ext]);
}

void IRenderDriverOpenGL::_DisableExt(DeviceExt ext)
{
	if (ext>=0 && ext<IEXT_MAX)
		glDisable(GLext[ext]);
}

void IRenderDriverOpenGL::_EnableDepthWrite(bool enable)
{
	glDepthMask(enable ? GL_TRUE : GL_FALSE);
}

void IRenderDriverOpenGL::_EnableColorWrite(bool enable)
{
	int val = enable ? GL_TRUE : GL_FALSE;
	glColorMask(val,val,val,val);
}

void IRenderDriverOpenGL::_EnableDepthTest(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void IRenderDriverOpenGL::_StencilFunc(AlphaFunc func, int ref, unsigned int mask)
{
	if (func>=0 && func <IAF_MAX)
	{
		glStencilFunc(GLalphaFunc[func],128, 0xffffffff);
	}
}

void IRenderDriverOpenGL::_StencilOp(StencilOp fail, StencilOp zfail, StencilOp zpass)
{
	if (fail>=0 && fail<ISO_MAX &&
		zfail>=0 && zfail<ISO_MAX && 
		zpass>=0 && zpass<ISO_MAX)
	{
		glStencilOp(GLstencilOp[fail],GLstencilOp[zfail],GLstencilOp[zpass]);
	}
}

void IRenderDriverOpenGL::_FrontFace(FrontFaceDirection face)
{
	if (face>=0 && face<IFFD_MAX)
	{
		glFrontFace(GLfrontFaceDirection[face]);
	}
}

void IRenderDriverOpenGL::_BlendMode(BlendMode sfactor, BlendMode dfactor)
{
	glBlendFunc(GLblendMode[sfactor],GLblendMode[dfactor]);
}

unsigned int IRenderDriverOpenGL::_NewList()
{
	//return 0;
	return glGenLists(1);
}

void IRenderDriverOpenGL::_CallList(unsigned int listid)
{
	glCallList(listid);
}

void IRenderDriverOpenGL::_StartList(unsigned int listid, bool execute)
{
	glNewList(listid, execute ? GL_COMPILE_AND_EXECUTE : GL_COMPILE );
}

void IRenderDriverOpenGL::_EndList()
{
	glEndList();
}

void IRenderDriverOpenGL::_DeleteList(unsigned int listid)
{
	glDeleteLists(listid, 1);
}

unsigned int IRenderDriverOpenGL::_NewFrameBuffer()
{
	unsigned int id;
	glGenFramebuffersEXT(1, &id);
	return id;
}

void IRenderDriverOpenGL::_DeleteFrameBuffer(unsigned int id)
{
	glDeleteFramebuffersEXT(1,&id);
}

void IRenderDriverOpenGL::_UseFrameBuffer(IFrameBuffer *buffer)
{
	if (buffer)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, buffer->GetBufferId());
		unsigned int Buffers[17];
		int Count=0;

		/*if (buffer->GetDepthTexture())
		{
			Buffers[Count]=GL_DEPTH_ATTACHMENT_EXT;
			Count++;
		}*/

		for (int i=0;i<16;i++)
		{
			if (buffer->GetTexture(i))
			{
				Buffers[Count]=GL_COLOR_ATTACHMENT0_EXT+i;
				Count++;
			}
		}

		if (buffer->GetTexture(0))
			glViewport(0,0,buffer->GetTexture(0)->GetWidth(),buffer->GetTexture(0)->GetHeight());

		glDrawBuffers(Count,Buffers);
		//glReadBuffers(Count,Buffers);
		//glReadBuffer(buffer->GetBufferId());
	}else{
		glViewport(0,0,m_iWidth,m_iHeight);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);		
	}
}

void IRenderDriverOpenGL::_AttachTextureToFrameBuffer(int pos, IFrameBuffer *buffer, ITexture *tex)
{
	_UseFrameBuffer(buffer);
	if (tex)
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + pos, GL_TEXTURE_2D, *(GLuint*)tex->GetTexture(), 0);
	else
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + pos, GL_TEXTURE_2D, 0, 0);
}

void IRenderDriverOpenGL::_AttachDepthTextureToFrameBuffer(IFrameBuffer *buffer, ITexture *tex)
{
	_UseFrameBuffer(buffer);
	if (tex)
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, *(GLuint*)tex->GetTexture(), 0);
	else
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);
}

/*ITexture* IRenderDriverOpenGL::LoadTexture(IString filename)
{
	if (m_pTextureManager)
		return m_pTextureManager->LoadTexture(filename);
	else
		return NULL;
}*/

IShader* IRenderDriverOpenGL::LoadShader(IString vertex, IString fragment)
{
	if (m_pShaderManager)
		return m_pShaderManager->LoadShader(vertex,fragment);
	else
		return NULL;
}

