
#include "InitialPanel.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;

BEGIN_EVENT_TABLE(InitialPanel, wxWindow)
	EVT_SIZE(InitialPanel::OnSize)
    EVT_PAINT(InitialPanel::OnPaint)
	EVT_ERASE_BACKGROUND(InitialPanel::OnEraseBackground)

	EVT_MOUSE_EVENTS(InitialPanel::OnMouseEvent)
	EVT_KEY_UP(InitialPanel::OnKeyUp)
	EVT_KEY_DOWN(InitialPanel::OnKeyDown)

	EVT_TIMER(wxID_ANY,InitialPanel::OnTimer)
END_EVENT_TABLE()

InitialPanel::InitialPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size)
: wxWindow(parent,winid,pos,size)
{
	/*IMatrix test(4,4);
	test.MakeIdentity();
	test(0,0)=5.0;
	test(1,0)=7.0;
	test(0,3)=3.0;
	IMatrix test2(4,4);
	test2.MakeIdentity();
	test2(0,0)=7.0;
	test2(1,0)=3.0;
	test2(0,3)=5.0;
	IMatrix test3(4,4);
	test3.MakeIdentity();
	test3(0,0)=4.0;
	test3(1,1)=2.0;
	test3(0,3)=9.0;

	(test3*test*test2).Print();
	printf("\n");
	(test3*(test*test2)).Print();*/

	m_pDevice = NULL;
	WXWidget handle =  GetHandle();
	m_pDevice = CreateDeviceEx(IVDT_OPENGL,32,&handle);

	/*class MyOutput : public Format::IImageITX::ITXOutput
	{
	public:
		MyOutput()
		{
			Time=GetTickCount();
		}

		virtual void Progress(int current)
		{
			if (GetTickCount()-Time>30)
			{
				printf("\b\b\b\b\b\b\b\b%.2f",((float)current/Total)*100.0);
				Time=GetTickCount();
			}
		}

		virtual void SetTotal(int totalSize)
		{
			Total=totalSize;
		}

		int Total;
		long Time;
	};
	
	Format::IImageITX image;
	Format::IImageITX::ITXParameter params;
	params.output = new MyOutput;
	//params.format=Format::IImageITX::DXTF_RGBA;
	params.format=Format::IImageITX::DXTF_DXT5;
	params.quality=Format::IImageITX::OQ_FAST;
	params.mipmaps=false;

	/*image.CreateFromImage("TextureSrc/brick.png",params);
	image.Save("Textures/brick_temp.itx");
	IImage myimage = image.GetImage();
	myimage.Save("test.tga");

	IImage myimage2("image.bmp");
	myimage2.Save("image2.tga");*/

	//Format::IImageITX *imagebak=Format::IImageITX::LoadITX("marcus.itx");

	/*printf("Object count %d\n",Initial::IObject::ObjectsClass.Count());
	for (UINT i=0;i<IObject::ObjectsClass.Count();i++)
		printf("%s\n",IObject::ObjectsClass[i]->GetName());*/

	//m_pDevice->GetRenderDriver()->SetCurrent();
	//m_pDevice->SetShowMouse(false);
	//m_pDevice->SetMouseLock(true);

	m_pCamera = new ICameraFPS(90,m_pDevice->GetWidth()/(float)m_pDevice->GetHeight());
	m_pCamera->Move(0,0,-2);

	Timer.SetOwner(this,wxID_ANY);
	//Timer.Start(15);
	Start();

	m_lTime=0;
	m_fTime=0;
	m_bPlay=true;
}

void InitialPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	if (m_pDevice)
	{
		m_pDevice->Run();
		m_pDevice->GetRenderDriver()->SetCurrent();

		/*m_pDevice->GetRenderDriver()->BeginRender();
		m_pDevice->GetRenderDriver()->SetCamera(m_pCamera);
			m_pDevice->GetRenderDriver()->Render(NULL,NULL);
			animObject->Render(m_pDevice->GetRenderDriver());
		m_pDevice->GetRenderDriver()->EndRender();*/


		/*long lTime = GetTickCount();
		long diff = lTime - m_lTime;
		m_lTime=lTime;
		if (m_bPlay)
			m_fTime +=diff/1000.0;

		m_pLight->SetPosition(IVector3D(-5.8+3,3+2.6,0.7)+IVector3D(2*cos(m_fTime),0,2*sin(m_fTime)));*/

		m_pDevice->GetRenderDriver()->RenderCamera(m_pCamera);

		m_pDevice->GetRenderDriver()->SwapBuffer();
		//SwapBuffers((HDC) ::GetDC((HWND) GetHWND()));
	}
}

void InitialPanel::OnMouseEvent(wxMouseEvent &event)
{
	SetFocus();
	if (m_pDevice)
	{
		//m_pDevice->OnCustomEvent(IDevice::IET_FOCUS,1,0);

		if (event.GetEventType()==wxEVT_RIGHT_DOWN)
		{
			m_pDevice->SetShowMouse(false);
			m_pDevice->SetMouseLock(true);
		}
		if (event.GetEventType()==wxEVT_RIGHT_UP)
		{
			m_pDevice->SetShowMouse(true);
			m_pDevice->SetMouseLock(false);
		}
	
		if (event.GetEventType()==wxEVT_LEFT_DOWN)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_CLICK,0,1);
		if (event.GetEventType()==wxEVT_LEFT_UP)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_CLICK,0,0);
		if (event.GetEventType()==wxEVT_MIDDLE_DOWN)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_CLICK,2,1);
		if (event.GetEventType()==wxEVT_MIDDLE_UP)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_CLICK,2,0);
		if (event.GetEventType()==wxEVT_RIGHT_DOWN)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_CLICK,1,1);
		if (event.GetEventType()==wxEVT_RIGHT_UP)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_CLICK,1,0);
		if (event.GetEventType()==wxEVT_MOTION)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_MOVE,event.GetPosition().x,event.GetPosition().y);
		if (event.GetEventType()==wxEVT_MOUSEWHEEL)
			m_pDevice->OnCustomEvent(IDevice::IET_MOUSE_CLICK,0,1);
	}
}

int GetKeyCode(int keycode)
{
	if (keycode==WXK_DOWN)
		return VK_DOWN;
	else if (keycode==WXK_UP)
		return VK_UP;
	else if (keycode==WXK_LEFT)
		return VK_LEFT;
	else if (keycode==WXK_RIGHT)
		return VK_RIGHT;

	else if (keycode==WXK_SPACE)
		return VK_SPACE;
	else if (keycode==WXK_CONTROL)
		return VK_CONTROL;
	else if (keycode==WXK_NUMPAD0)
		return VK_NUMPAD0;

	return 0;
}

void InitialPanel::OnKeyUp(wxKeyEvent& event)
{
	/*if (event.GetKeyCode()==WXK_F5)
		m_bPlay=1-m_bPlay;*/

	int keycode = GetKeyCode(event.GetKeyCode());		
	m_pDevice->OnCustomEvent(IDevice::IET_KEY_PUSH,keycode,0);
}

void InitialPanel::OnKeyDown(wxKeyEvent& event)
{
	int keycode = GetKeyCode(event.GetKeyCode());
	m_pDevice->OnCustomEvent(IDevice::IET_KEY_PUSH,keycode,1);
}

void InitialPanel::OnTimer(wxTimerEvent &event)
{
	//Camera
	int speed=1;
	if (m_pDevice->KeyDown(VK_UP))
		m_pCamera->Move(0,0,0.1*speed);
	if (m_pDevice->KeyDown(VK_DOWN))
		m_pCamera->Move(0,0,-0.1*speed);
	if (m_pDevice->KeyDown(VK_LEFT))
		m_pCamera->Move(0.1*speed,0,0);
	if (m_pDevice->KeyDown(VK_RIGHT))
		m_pCamera->Move(-0.1*speed,0,0);

	if (m_pDevice->KeyDown(VK_NUMPAD0))
		m_pCamera->Move(0,0.1*speed,0);
	if (m_pDevice->KeyDown(VK_CONTROL))
		m_pCamera->Move(0,-0.1*speed,0);

	if (m_pDevice->GetMouseLock())
	{
		m_pCamera->RotateY(m_pDevice->GetMouseX()/100.0);
		m_pCamera->RotateX(m_pDevice->GetMouseY()/100.0);
	}

	if (m_pDevice->KeyPress(VK_SPACE))
		m_pDevice->GetRenderDriver()->SetLightEnable(!m_pDevice->GetRenderDriver()->GetLightEnable());
	

	Refresh();
}

void InitialPanel::OnSize(wxSizeEvent& event)
{
	//wxWindow::OnSize(event);
	//printf("Size\n");
	m_pDevice->OnCustomEvent(IDevice::IET_RESIZE,event.GetSize().x,event.GetSize().y);
}

IDevice* InitialPanel::GetDevice()
{
	return m_pDevice;
}

void InitialPanel::Start()
{
	Timer.Start(15);
}

void InitialPanel::Stop()
{
	Timer.Stop();
}
