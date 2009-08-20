
#include "InitialPanel.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;
using namespace Initial::GUI;

const float AXIS_HEIGHT = 0.7;
const float CIRCLE_HEIGHT = 0.9; // to 1
const float CUBE_HEIGHT = 0.8; // to 1
const int CIRCLE_QUALITY = 32;
const float SCALE_FOV = 42.0;
const float SCALE_MIN = 0.1;

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
	m_pDevice = NULL;
	m_lTime=0;
	m_fTime=0;
	m_bPlay=true;
	m_iTool=TT_NONE;

	WXWidget handle = GetHandle();
	m_pDevice = CreateDeviceEx(IVDT_OPENGL,32,&handle);
	m_bRender=false;
	//ASSERT(m_pDevice);
	if (!m_pDevice)
		wxLogError("Error for creating render");

	if (m_pDevice)
		m_EmissiveVertexAlpha = m_pDevice->GetRessourceManager()->LoadMaterial("Materials/engine/diffuse-vertex-alpha.ima");

	m_pCamera = new ICameraFPS(90,m_pDevice->GetWidth()/(float)m_pDevice->GetHeight());
	m_pCamera->Move(0,0,-2);

	Timer.SetOwner(this,wxID_ANY);
	Start();
}

void InitialPanel::OnPaint(wxPaintEvent& event)
{
	if (m_bRender)
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

			if (m_iTool!=TT_NONE && m_iTool!=TT_SELECT)
			{
				float ratio=ratio = (float)GetSize().x/GetSize().y;

				IMatrix cameraMatrix(4,4);
				if (m_pCamera)
				{
					m_pCamera->SetRatio(ratio);
					cameraMatrix = m_pCamera->GetUpdateViewMatrix();
				}else
					cameraMatrix.MakeIdentity(4);

				m_pDevice->GetRenderDriver()->_SetProjectionMatrix();
				m_pDevice->GetRenderDriver()->_SetMatrix(cameraMatrix);

				IVector3D center;// = selectedObj->GetPosition();

				IMatrix projMatInv;
				cameraMatrix.Inv(projMatInv);
				IVector2D viewport(GetSize().x, GetSize().y);
				IVector3D camPos = projMatInv*IVector3D(0,0,0);
				IVector3D dist = center-camPos;
				float scale = dist.Length()/(m_pCamera->GetFov()/SCALE_FOV); // 36 = 180/10 * 2
				scale = scale<SCALE_MIN ? SCALE_MIN : scale;

				//m_pDevice->GetRenderDriver()->_PushMatrix();
				m_pDevice->GetRenderDriver()->_Translate(center.x,center.y,center.z);
				//Rotate
				m_pDevice->GetRenderDriver()->_Scale(scale,scale,scale);

				m_pDevice->GetRenderDriver()->_UseFrameBuffer(0);
				m_pDevice->GetRenderDriver()->_ClearBuffer(IVB_DEPTH);
				//m_pDevice->GetRenderDriver()->BeginRender();
				//DrawAxis();
				//m_pDevice->GetRenderDriver()->EndRender();

				switch (m_iTool)
				{
				case TT_MOVE:
					DrawAxis();
					break;
				case TT_ROTATE:
					break;
				case TT_SCALE:
					break;
				}
			}

			m_pDevice->GetRenderDriver()->SwapBuffer();
			//SwapBuffers((HDC) ::GetDC((HWND) GetHWND()));
		}
	}
}

void InitialPanel::OnMouseEvent(wxMouseEvent &event)
{
	if (event.GetButton())
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
	m_bRender=true;
}

void InitialPanel::Stop()
{
	m_bRender=false;
	Timer.Stop();
}

void InitialPanel::SetTool(ToolType tool)
{
	m_iTool=tool;
}

InitialPanel::ToolType InitialPanel::GetTool()
{
	return m_iTool;
}

void InitialPanel::DrawAxis()
{
	if (m_pDevice)
	{
		Video::IRenderDriver *render = m_pDevice->GetRenderDriver();
		if (render)
		{			
			render->UseMaterial(m_EmissiveVertexAlpha);
			//render->UseMaterial(NULL);
			//Draw axis 
			render->_SetColor(IColor(1,0,0));
			DrawArrow(AXIS_HEIGHT,0.05);

			render->_SetColor(IColor(0,1,0));
			render->_PushMatrix();
			render->_RotateZ(90);
			DrawArrow(AXIS_HEIGHT,0.05);
			render->_PopMatrix();

			render->_SetColor(IColor(0,0,1));
			render->_PushMatrix();
			render->_RotateY(-90);
			DrawArrow(AXIS_HEIGHT,0.05);
			render->_PopMatrix();

			ITriangle tri;
			tri.SetVertex(0,IVector3D(0,0,0));
			//Draw plan
			render->_SetColor(IColor(0,1,0));
			render->_DrawLine(IVector3D(AXIS_HEIGHT,0,0),IVector3D(0,0,AXIS_HEIGHT));
			tri.SetVertex(1,IVector3D(AXIS_HEIGHT,0,0));
			tri.SetVertex(2,IVector3D(0,0,AXIS_HEIGHT));
			render->_StartTriangleDraw(false);
			render->_DrawTriangle(&tri);
			render->_EndTriangleDraw();			

			render->_SetColor(IColor(0,0,1));
			render->_DrawLine(IVector3D(AXIS_HEIGHT,0,0),IVector3D(0,AXIS_HEIGHT,0));
			tri.SetVertex(1,IVector3D(AXIS_HEIGHT,0,0));
			tri.SetVertex(2,IVector3D(0,AXIS_HEIGHT,0));
			render->_StartTriangleDraw(false);
			render->_DrawTriangle(&tri);
			render->_EndTriangleDraw();

			render->_SetColor(IColor(1,0,0));
			render->_DrawLine(IVector3D(0,AXIS_HEIGHT,0),IVector3D(0,0,AXIS_HEIGHT));
			tri.SetVertex(1,IVector3D(0,AXIS_HEIGHT,0));
			tri.SetVertex(2,IVector3D(0,0,AXIS_HEIGHT));
			render->_StartTriangleDraw(false);
			render->_DrawTriangle(&tri);
			render->_EndTriangleDraw();

			//DrawArrow(AXIS_HEIGHT,0.05);
		}
	}
}

void InitialPanel::DrawArrow(float arrowheight,float width)
{
	m_pDevice->GetRenderDriver()->_SetLineSize(2.0);
	m_pDevice->GetRenderDriver()->_DrawLine(IVector3D(0,0,0),IVector3D(arrowheight,0,0));
	m_pDevice->GetRenderDriver()->_SetLineSize(1.0);

	ITriangle tri;
	m_pDevice->GetRenderDriver()->_StartTriangleDraw(false);
		tri.SetVertex(0,IVector3D(arrowheight,width,-width));
		tri.SetVertex(1,IVector3D(arrowheight,-width,-width));
		tri.SetVertex(2,IVector3D(1,0,0));
		m_pDevice->GetRenderDriver()->_DrawTriangle(&tri);

		tri.SetVertex(0,IVector3D(arrowheight,width,width));
		tri.SetVertex(1,IVector3D(arrowheight,-width,width));
		m_pDevice->GetRenderDriver()->_DrawTriangle(&tri);

		tri.SetVertex(0,IVector3D(arrowheight,width,-width));
		tri.SetVertex(1,IVector3D(arrowheight,width,width));
		m_pDevice->GetRenderDriver()->_DrawTriangle(&tri);

		tri.SetVertex(0,IVector3D(arrowheight,-width,-width));
		tri.SetVertex(1,IVector3D(arrowheight,-width,width));
		m_pDevice->GetRenderDriver()->_DrawTriangle(&tri);
	m_pDevice->GetRenderDriver()->_EndTriangleDraw();
}

