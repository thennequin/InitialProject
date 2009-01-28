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

#include "Video/OpenGLDrv/IRenderDriverOpenGL.h"
#include "Video/OpenGLDrv/ITextureManagerOpenGL.h"
#include "Video/OpenGLDrv/IShaderManagerOpenGL.h"
#include "Video/IMaterial.h"

#include "Core/IMath.h"

using namespace Initial;
using namespace Math;

unsigned long GLext[] = {
	GL_TEXTURE_2D,
	GL_DEPTH_TEST,
	GL_BLEND,
	GL_DEPTH,
	};

IArray<IRenderDriverOpenGL::IInstance> IRenderDriverOpenGL::Instances;

IRenderDriverOpenGL::IRenderDriverOpenGL(int Width,int Height,int BitDepth, bool Fullscreen, IString Title, int AntiAliasingPower, bool UseShader)
 : IRenderDriver(Width,Height,BitDepth,Fullscreen,Title, AntiAliasingPower, UseShader)
{
	hRC=NULL;							// Permanent Rendering Context
	hDC=NULL;							// Private GDI Device Context
	hWnd=NULL;							// Holds Our Window Handle
	hInstance;							// Holds The Instance Of The Application
	PixelFormat=0;

	for (int i=0;i<256;i++)
	{
		keys[i]=false;					// Array Used For The Keyboard Routine
		previousKeys[i]=false;
		keysDown[i]=false;
	}

	for (int i=0;i<3;i++)
		mouse[i]=false;		

	active=true;						// Window Active Flag Set To true By Default
	focus=true;

	CreateGLWindow();
	SetCursorPos(m_iWidth/2,m_iHeight/2);

	IInstance *inst=new IInstance;
	inst->Device=this;
	inst->Handle=hWnd;
	Instances.Add(*inst);

	m_pTextureManager = new ITextureManagerOpenGL(this);
	if (UseShader)
		m_pShaderManager = new IShaderManagerOpenGL(this);
	else
		m_pShaderManager=NULL;

	if (m_pTextureManager)
	m_pScreenTexture = m_pTextureManager->CreateTexture(1024,1024);
}

LRESULT	CALLBACK IRenderDriverOpenGL::Callback( 
							HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	IRenderDriverOpenGL *device=NULL;
	for (int i=0;i<Instances.Count();i++)
	{
		if (hWnd==Instances[i].Handle)
		{
			device=Instances[i].Device;
			break;
		}
	}

	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				if (device)
				{
					device->active=true;						// Program Is Active
				}
			}
			else
			{
				if (device)
				{
					device->active=false;						// Program Is No Longer Active
				}
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SETFOCUS:
			if (device)
				device->focus=true;
			return 0;

		case WM_KILLFOCUS:
			if (device)
				device->focus=false;
			return 0;

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			/*if (device)
					device->Destroy();*/
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			if (device)
			{
					device->keys[wParam] = true;	// If So, Mark It As true
			}
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			if (device)
					device->keys[wParam] = false;					// If So, Mark It As false
			return 0;								// Jump Back
		}

		case WM_LBUTTONUP:
		{
			if (device)
				device->mouse[0]=false;
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			if (device)
				device->mouse[0]=true;
			return 0;
		}

		case WM_RBUTTONUP:
		{
			if (device)
				device->mouse[1]=false;
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			if (device)
				device->mouse[1]=true;
			return 0;
		}

		case WM_MBUTTONUP:
		{
			if (device)
				device->mouse[3]=false;
			return 0;
		}

		case WM_MBUTTONDOWN:
		{
			if (device)
				device->mouse[3]=true;
			return 0;
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			if (device)
					device->ResizeGL(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

bool IRenderDriverOpenGL::InitPixelFormat(int AntiAliasingPower)
{
	if (wglChoosePixelFormatARB)
	{
		printf("Choose Pixel Format\n");
		int pixelFormat=PixelFormat;
		bool valid;
		UINT numFormats;
		float fAttributes[] = {0,0};
		int iAttributes[] = { WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,24,
			WGL_ALPHA_BITS_ARB,8,
			WGL_DEPTH_BITS_ARB,16,
			WGL_STENCIL_BITS_ARB,0,
			WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
			WGL_SAMPLES_ARB,4,
			0,0};

		// First We Check To See If We Can Get A Pixel Format For 4 Samples
		valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);

		iAttributes[19] = AntiAliasingPower;
		while (iAttributes[19]>1)
		{
			valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
			if (valid && numFormats >= 1)
			{
				m_iAntiAliasing=iAttributes[19];
				printf("Antialiasing %dx ok\n",iAttributes[19]);
				PixelFormat=pixelFormat;
				return true;
			}
			iAttributes[19]/=2;
		}
	 
		if (AntiAliasingPower>0)
			printf("Antialiasing not available\n");
	}
	return false;
}

bool IRenderDriverOpenGL::CreateGLWindow()
{
	printf("Create GL window\n");

	bool PixelFormatInit=PixelFormat;
	
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)m_iWidth;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)m_iHeight;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) Callback;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		m_sError="Failed To Register The Window Class.";
		//MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return false
	}
	
	if (m_bFullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= m_iWidth;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= m_iHeight;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= m_iBitDepth;			// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			m_bFullscreen=false;
		}
	}

	if (m_bFullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(false);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								m_sTitle,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		m_sError="Window Creation Error.";
		//MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		m_iBitDepth,								// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	//pfd=pfd1;
	//WGL_SAMPLE_BUFFERS_ARB = $2041;
  //WGL_SAMPLES_ARB
	//wglChoosePixelFormatARB
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		m_sError="Can't Create A GL Device Context.";
		//MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if (PixelFormat==0)
	{
		//PixelFormatInit=true;
		printf("Create PixelFormat\n");
		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			m_sError="Can't Find A Suitable PixelFormat.";
			//MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return false
		}
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		m_sError="Can't Set The PixelFormat.";
		//MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		m_sError="Can't Create A GL Rendering Context.";
		//MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		m_sError="Can't Activate The GL Rendering Context.";
		//MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window

	//WINDOWPLACEMENT pos;
	//SetWindowPlacement(hWnd,);					// Center window

	GLenum err = glewInit();						// Init GLEW
	if (GLEW_OK != err)
	{
		printf("Error: %s\n", glewGetErrorString(err));
	}
	ResizeGL(m_iWidth, m_iHeight);					// Set Up Our Perspective GL Screen

	if (!PixelFormatInit && InitPixelFormat(m_iAntiAliasing))
	{
		Destroy();
		return CreateGLWindow();
	}

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		m_sError="Initialization Failed.";
		//MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	return true;		
}

bool IRenderDriverOpenGL::KillGLWindow()
{
	if (m_bFullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(true);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			m_sError="Release Of DC And RC Failed.";
			//MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			m_sError="Release Rendering Context Failed.";
			//MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		m_sError="Release Device Context Failed.";
		//MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		m_sError="Could Not Release hWnd.";
		//MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		m_sError="Could Not Unregister Class.";
		//MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
	return true;
}

bool IRenderDriverOpenGL::InitGL()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_FRAGMENT_PROGRAM_ARB);

	/*glEnable( GL_POLYGON_SMOOTH );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	glEnable(GL_POINT_SMOOTH );
	glEnable(GL_LINE_SMOOTH );
	glEnable(GL_POLYGON_SMOOTH );*/
	//glBlendFunc( GL_ONE, GL_ONE );
	//glDepthFunc( GL_EQUAL );
	return true;
}

bool IRenderDriverOpenGL::ResizeGL(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,10000.0f); //default fov is 90
	_GetMatrix().Print();
	m_iWidth=width;
	m_iHeight=height;

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	return true;
}

void IRenderDriverOpenGL::Destroy()
{
	KillGLWindow();
}

bool IRenderDriverOpenGL::KeyPress(int key)
{
	if (key>=0 && key<256)
		return keysDown[key];
	return false;
}

bool IRenderDriverOpenGL::KeyDown(int key)
{
	if (key>=0 && key<256)
		return keys[key];
	return false;
}

bool IRenderDriverOpenGL::MousePress(int button)
{
	if (button>=0 && button<3)
		return mouse[button];
	return false;
}


bool IRenderDriverOpenGL::UpdateDevice()
{
	MSG	msg;								// Windows Message Structure
	BOOL done=true;							// Bool Variable To Exit Loop

	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
	{
		if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
		{
			done=false;					// If So done=true
		}
		else							// If Not, Deal With Window Messages
		{
			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
		}
	}
	else								// If There Are No Messages
	{
		// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
		if (active)						// Program Active?
		{
			ShowCursor(m_bShowMouse);
			//SetCurrent
			//DrawGLScene();				// Draw The Scene
			//SwapBuffers(hDC);			// Swap Buffers (Double Buffering)
		}
	}
	
	if (focus)
	{
		POINT point;
		GetCursorPos(&point);
		//SetCursorPos(640/2, 480/2);
		if (m_bLockMouse)
		{
			m_iMouseAccX=point.x-m_iWidth/2;
			m_iMouseAccY=point.y-m_iHeight/2;
			SetCursorPos(m_iWidth/2,m_iHeight/2);
		}else{
			m_iMouseAccX=point.x;
			m_iMouseAccY=point.y;
		}

		//Update Keys

		for (int i=0;i<256;i++)
		{
			keysDown[i]=(keys[i] && keys[i]!=previousKeys[i]);	
			previousKeys[i]=keys[i];
		}
	}else{
		m_iMouseAccX=m_iMouseAccY=0;
	}

	return done;
}

void IRenderDriverOpenGL::BeginRender()
{
	m_bInRender=true;
	m_iPolygonCount=0;
	m_iLineCount=0;
	//m_pCurrentMaterial=NULL;
	//if (m_bAntiAliasingEnable)
		//glEnable(GL_MULTISAMPLE_ARB);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	_SetProjectionMatrix();
	//_PushMatrix();
	_LoadIdentity();
	if (m_pCamera)
		m_pCamera->UpdateView(this);
	_SetModelViewMatrix();
	_LoadIdentity();
}

void IRenderDriverOpenGL::EndRender(ITexture *destRGBA, ITexture *destDepth)
{
	if (destDepth)
	{
		glBindTexture(GL_TEXTURE_2D,destDepth->GetTextureId());
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, m_iWidth, m_iHeight, 0);
		glBindTexture(GL_TEXTURE_2D,0);
	}
	
	if (destRGBA)
	{
		glBindTexture(GL_TEXTURE_2D,destRGBA->GetTextureId());
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_iWidth, m_iHeight, 0);
		glBindTexture(GL_TEXTURE_2D,0);
	}else{
		glFlush();
		SwapBuffers(hDC);
	}
	//glEnable(GL_MULTISAMPLE_ARB);
	m_bInRender=false;
}

void IRenderDriverOpenGL::SetSize(int width, int height)
{
	KillGLWindow();
	int oldwidth=m_iWidth, oldheight=m_iHeight;
	m_iWidth=width;
	m_iHeight=height;
	if (!CreateGLWindow())
	{
		m_iWidth=oldwidth;
		m_iHeight=oldheight;
		CreateGLWindow();
	}
}

void IRenderDriverOpenGL::SetFullScreen(bool full)
{
	KillGLWindow();
	bool oldfullscreen=m_bFullscreen;
	m_bFullscreen=full;
	if (!CreateGLWindow())
	{
		m_bFullscreen=oldfullscreen;
		CreateGLWindow();
	}
}

void IRenderDriverOpenGL::SetTitle(IString title)
{
	
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

void IRenderDriverOpenGL::_DrawPolygon(ITriangle* polygon)
{	
	if (polygon)
	{
		m_iPolygonCount++;

		IVertex *vertex = polygon->GetVertex();
		ICoord *coord = polygon->GetCoords();
		IVertex *normal = polygon->GetNormals();

		//Normal
		//glNormal3f(normal[0][0],normal[0][1],normal[0][2]);
		//glMultiTexCoord3fvARB(GL_TEXTURE1_ARB,normal[0]);
		//Tangent
		//glMultiTexCoord3fvARB(GL_TEXTURE2_ARB,normal[1]);
		//BiNormal
		//glMultiTexCoord2fARB(GL_TEXTURE3_ARB,normal[2][0],normal[2][1],normal[2][2]);
		for (int i=0;i<3;i++)
		{
			//glNormal3f(normal[0][0],normal[0][1],normal[0][2]);
			//glMultiTexCoord2fARB(GL_TEXTURE0_ARB,coord[i].GetU(),coord[i].GetV());
			glMultiTexCoord2fvARB(GL_TEXTURE0_ARB,coord[i]);
			//glVertex3f(vertex[i][0],vertex[i][1],vertex[i][2]);
			glVertex3fv(vertex[i]);
		}
	}
}

void IRenderDriverOpenGL::_DrawLine(const IVertex& pt1,const IVertex& pt2)
{
	m_iLineCount++;
	glBegin(GL_LINES);
		glVertex3fv(pt1);
		glVertex3fv(pt2);
	glEnd();
}

void IRenderDriverOpenGL::_SetColor(IColor color)
{
	glColor4f(color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
}

void IRenderDriverOpenGL::_SetLineSize(float size)
{
	glLineWidth(size);
}

void IRenderDriverOpenGL::_StartPolyDraw(bool Wireframe)
{
	glLineWidth( Wireframe ? 2 : 1 );
	glBegin(Wireframe ? GL_LINE_LOOP : GL_TRIANGLES );
}

void IRenderDriverOpenGL::_EndPolyDraw()
{
	glEnd();
}

void IRenderDriverOpenGL::_EnableExt(DeviceExt ext)
{
	if (ext>=0 && ext<EXT_MAX)
		glEnable(GLext[ext]);
}

void IRenderDriverOpenGL::_DisableExt(DeviceExt ext)
{
	if (ext>=0 && ext<EXT_MAX)
		glDisable(GLext[ext]);
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

ITexture* IRenderDriverOpenGL::LoadTexture(IString filename)
{
	if (m_pTextureManager)
		return m_pTextureManager->LoadTexture(filename);
	else
		return NULL;
}

IShader* IRenderDriverOpenGL::LoadShader(IString vertex, IString fragment)
{
	if (m_pShaderManager)
		return m_pShaderManager->LoadShader(vertex,fragment);
	else
		return NULL;
}

void IRenderDriverOpenGL::SetAntialising(int power)
{
	/*if (power)
	{
		glEnable(GL_MULTISAMPLE_ARB);
	}else{
		glDisable(GL_MULTISAMPLE_ARB);
	}*/
	//Destroy();
	m_iAntiAliasing=power;
	//CreateGLWindow();
	/*if (power!=0)
		m_bAntiAliasingEnable=true;
	else
		m_bAntiAliasingEnable=false;*/
}