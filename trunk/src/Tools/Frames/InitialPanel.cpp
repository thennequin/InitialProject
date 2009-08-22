
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
const float TOLERANCE = 0.05;

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
	m_bLeftClick=false;

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
				IVector3D center;
				if (GetSelectionCenter(center)>0)
				{
					float ratio=ratio = (float)GetSize().x/GetSize().y;

					if (m_pCamera)
					{
						m_pCamera->SetRatio(ratio);
						m_mProjectionMatrix = m_pCamera->GetUpdateViewMatrix();
						m_mModelViewMatrix = m_pCamera->GetMatrix();
					}else{
						m_mProjectionMatrix.MakeIdentity(4);
						m_mModelViewMatrix.MakeIdentity(4);
					}

					m_mProjectionMatrix.Inv(m_mInvProjectionMatrix);
					m_mModelViewMatrix.Inv(m_mInvModelViewMatrix);

					m_pDevice->GetRenderDriver()->_SetProjectionMatrix();
					m_pDevice->GetRenderDriver()->_SetMatrix(m_mProjectionMatrix);

					IVector2D viewport(GetSize().x, GetSize().y);
					IVector3D camPos = m_mInvProjectionMatrix*IVector3D(0,0,0);
					IVector3D dist = center-camPos;
					m_fAxisScale= dist.Length()/(m_pCamera->GetFov()/SCALE_FOV); // 36 = 180/10 * 2
					m_fAxisScale = m_fAxisScale<SCALE_MIN ? SCALE_MIN : m_fAxisScale;

					//m_pDevice->GetRenderDriver()->_PushMatrix();
					m_pDevice->GetRenderDriver()->_Translate(center.x,center.y,center.z);
					if (m_iTransMode==TM_LOCAL)
					{
						m_mTransMatrix.MakeIdentity(4);
						GetSelectionAngle(m_mTransMatrix);
					}else if (m_iTransMode==TM_VIEW)
					{
						m_mTransMatrix=m_mInvModelViewMatrix;
						m_mTransMatrix(3,0)=0;
						m_mTransMatrix(3,1)=0;
						m_mTransMatrix(3,2)=0;
						m_mTransMatrix(0,3)=0;
						m_mTransMatrix(1,3)=0;
						m_mTransMatrix(2,3)=0;
					}else if (m_iTransMode==TM_WORLD)
					{
						m_mTransMatrix.MakeIdentity(4);
					}
					m_pDevice->GetRenderDriver()->_MultMatrix(m_mTransMatrix);
					m_pDevice->GetRenderDriver()->_Scale(m_fAxisScale,m_fAxisScale,m_fAxisScale);

					m_pDevice->GetRenderDriver()->_UseFrameBuffer(0);
					m_pDevice->GetRenderDriver()->_ClearBuffer(IVB_DEPTH);

					switch (m_iTool)
					{
					case TT_MOVE:
						DrawAxis();
						break;
					case TT_ROTATE:
						DrawRotateAxis();
						break;
					case TT_SCALE:
						break;
					}
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

	//Tool
	if (m_iTool!=TT_NONE)
	{
		IVector3D lineStart, lineDir;
		IMatrix cameraProj=m_pCamera->GetUpdateViewMatrix();
		Math::MakeRayFromMouseOnScreen(IVector2D(event.GetPosition().x,event.GetPosition().y),
			cameraProj,
			IVector2D(GetSize().x,GetSize().y),
			lineStart,lineDir);

		if (event.ButtonIsDown(1)==false)
		{
			if (m_iTool==TT_MOVE)
				CheckAxis(lineStart,lineDir);
			else if (m_iTool==TT_ROTATE)
				CheckRotateAxis(lineStart,lineDir);
		}

		if (event.ButtonIsDown(1))
		{
			IVector3D center;
			if (GetSelectionCenter(center))
			{
				if (m_iTool==TT_MOVE)	
				{
					if (m_iAxisSelected!=AS_NONE)
					{
						if (m_bLeftClick==false)
						{
							m_bLeftClick=true;
							Math::MakeRayFromMouseOnScreen(IVector2D(event.GetPosition().x,event.GetPosition().y),
								cameraProj,
								IVector2D(GetSize().x,GetSize().y),
								m_MouseStartDrag,m_MouseStartDragDir);
							StartDrag();
						}else{
							IVector3D axisX, axisY, axisZ;
							axisX=m_mTransMatrix*IVector3D(1,0,0)*m_fAxisScale;
							axisY=m_mTransMatrix*IVector3D(0,1,0)*m_fAxisScale;
							axisZ=m_mTransMatrix*IVector3D(0,0,1)*m_fAxisScale;
							IVector3D dir1,dir2;
							if (m_iAxisSelected==AS_X)
								dir1=axisX;
							else if (m_iAxisSelected==AS_Y)
								dir1=axisY;
							else if (m_iAxisSelected==AS_Z)
								dir1=axisZ;
							else if (m_iAxisSelected==AS_XY)
							{
								dir1=axisX;
								dir2=axisY;
							}else if (m_iAxisSelected==AS_XZ)
							{
								dir1=axisX;
								dir2=axisZ;
							}else if (m_iAxisSelected==AS_YZ)
							{
								dir1=axisY;
								dir2=axisZ;
							}

							IVector3D dragEnd, dragEndDir;
							Math::MakeRayFromMouseOnScreen(IVector2D(event.GetPosition().x,event.GetPosition().y),
								cameraProj,
								IVector2D(GetSize().x,GetSize().y),
								dragEnd,dragEndDir);

							if (m_iAxisSelected==AS_XY || m_iAxisSelected==AS_XZ || m_iAxisSelected==AS_YZ)
							{
								IVector3D hitStart, hitEnd, temp;
								Math::Intersect::RayIntersectPlane(m_MouseStartDrag, m_MouseStartDragDir, center, dir1, dir2, hitStart);
								Math::Intersect::RayIntersectPlane(dragEnd, dragEndDir, center, dir1, dir2, hitEnd);
								SetSelectionPosition(hitEnd-hitStart);
							}else{
								IVector3D hitStart, hitEnd, temp;
								Math::Intersect::RayIntersectRay(center,dir1,m_MouseStartDrag, m_MouseStartDragDir, hitStart,temp);
								Math::Intersect::RayIntersectRay(center,dir1,dragEnd, dragEndDir, hitEnd,temp);
								SetSelectionPosition(hitEnd-hitStart);
							}
						}
					}
				}else if (m_iTool==TT_ROTATE)	
				{
					if (m_iAxisSelected!=AS_NONE)
					{
						if (m_bLeftClick==false)
						{
							m_bLeftClick=true;
							Math::MakeRayFromMouseOnScreen(IVector2D(event.GetPosition().x,event.GetPosition().y),
								cameraProj,
								IVector2D(GetSize().x,GetSize().y),
								m_MouseStartDrag,m_MouseStartDragDir);
							StartDrag();
						}else{
							IVector3D axisX, axisY, axisZ;
							axisX=m_mTransMatrix*IVector3D(1,0,0)*m_fAxisScale;
							axisY=m_mTransMatrix*IVector3D(0,1,0)*m_fAxisScale;
							axisZ=m_mTransMatrix*IVector3D(0,0,1)*m_fAxisScale;
							IVector3D dir1,dir2;
							if (m_iAxisSelected==AS_X)
							{
								dir1=axisY;
								dir2=axisZ;
							}else if (m_iAxisSelected==AS_Y)
							{
								dir1=axisX;
								dir2=axisZ;
							}else if (m_iAxisSelected==AS_Z)
							{
								dir1=axisX;
								dir2=axisY;
							}

							/*IVector3D dragEnd, dragEndDir;
							Math::MakeRayFromMouseOnScreen(IVector2D(event.GetPosition().x,event.GetPosition().y),
								cameraProj,
								IVector2D(GetSize().x,GetSize().y),
								dragEnd,dragEndDir);

							if (m_iAxisSelected==AS_XY || m_iAxisSelected==AS_XZ || m_iAxisSelected==AS_YZ)
							{
								IVector3D hitStart, hitEnd, temp;
								Math::Intersect::RayIntersectPlane(m_MouseStartDrag, m_MouseStartDragDir, center, dir1, dir2, hitStart);
								Math::Intersect::RayIntersectPlane(dragEnd, dragEndDir, center, dir1, dir2, hitEnd);
								SetSelectionPosition(hitEnd-hitStart);
							}else{
								IVector3D hitStart, hitEnd, temp;
								Math::Intersect::RayIntersectRay(center,dir1,m_MouseStartDrag, m_MouseStartDragDir, hitStart,temp);
								Math::Intersect::RayIntersectRay(center,dir1,dragEnd, dragEndDir, hitEnd,temp);
								SetSelectionPosition(hitEnd-hitStart);
							}*/
						}
					}
				}
			}
		}else if (m_bLeftClick)
		{
			m_bLeftClick=false;
		}
	}

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

void InitialPanel::SetTransMode(TransMode mode)
{
	m_iTransMode=mode;
}

InitialPanel::TransMode InitialPanel::GetTransMode()
{
	return m_iTransMode;
}

size_t InitialPanel::GetSelectionCenter(Initial::Core::IVector3D& center)
{
	IList<INode*> selectedNode = m_pDevice->GetSceneManager()->GetSelectedNode();
	size_t count=0;
	FOREACH(INode*,ite,selectedNode)
	{
		if (ite.GetData())
		{
			center=center+ite.GetData()->GetPosition();
			count++;
		}
	}
	if (count>0)
		center=center/count;
	return count;
}

void InitialPanel::SetSelectionPosition(Initial::Core::IVector3D pos)
{
	IList<INode*> selectedNode = m_pDevice->GetSceneManager()->GetSelectedNode();
	FOREACH(INode*,ite,selectedNode)
	{
		if (ite.GetData())
		{
			INode *node=ite.GetData();
			node->SetPosition(node->GetDragPosition()+pos);
		}
	}
}

void InitialPanel::GetSelectionAngle(Initial::Math::IMatrix& res)
{
	IList<INode*> selectedNode = m_pDevice->GetSceneManager()->GetSelectedNode();
	FOREACH(INode*,ite,selectedNode)
	{
		if (ite.GetData())
		{
			INode *node=ite.GetData();
			res = node->GetRotationMatrix4x4();
			break;
		}
	}
}

void InitialPanel::StartDrag()
{
	IList<INode*> selectedNode = m_pDevice->GetSceneManager()->GetSelectedNode();
	FOREACH(INode*,ite,selectedNode)
	{
		if (ite.GetData())
		{
			ite.GetData()->StartDrag();
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
			if (m_iAxisSelected==AS_X)
				render->_SetColor(IColor(1,1,0));
			else
				render->_SetColor(IColor(1,0,0));
			DrawArrow(AXIS_HEIGHT,0.05);

			if (m_iAxisSelected==AS_Y)
				render->_SetColor(IColor(1,1,0));
			else
				render->_SetColor(IColor(0,1,0));
			render->_PushMatrix();
			render->_RotateZ(90);
			DrawArrow(AXIS_HEIGHT,0.05);
			render->_PopMatrix();

			if (m_iAxisSelected==AS_Z)
				render->_SetColor(IColor(1,1,0));
			else
				render->_SetColor(IColor(0,0,1));
			render->_PushMatrix();
			render->_RotateY(-90);
			DrawArrow(AXIS_HEIGHT,0.05);
			render->_PopMatrix();

			ITriangle tri;
			tri.SetVertex(0,IVector3D(0,0,0));
			//Draw plan
			render->_EnableExt(IEXT_BLEND);
			if (m_iAxisSelected==AS_XZ)
				render->_SetColor(IColor(1,1,0,0.5));
			else
				render->_SetColor(IColor(0,1,0,0.5));
			render->_DrawLine(IVector3D(AXIS_HEIGHT,0,0),IVector3D(0,0,AXIS_HEIGHT));
			tri.SetVertex(1,IVector3D(AXIS_HEIGHT,0,0));
			tri.SetVertex(2,IVector3D(0,0,AXIS_HEIGHT));
			render->_StartTriangleDraw(false);
			render->_DrawTriangle(&tri);
			render->_EndTriangleDraw();			

			if (m_iAxisSelected==AS_XY)
				render->_SetColor(IColor(1,1,0,0.5));
			else
				render->_SetColor(IColor(0,0,1,0.5));
			render->_DrawLine(IVector3D(AXIS_HEIGHT,0,0),IVector3D(0,AXIS_HEIGHT,0));
			tri.SetVertex(1,IVector3D(AXIS_HEIGHT,0,0));
			tri.SetVertex(2,IVector3D(0,AXIS_HEIGHT,0));
			render->_StartTriangleDraw(false);
			render->_DrawTriangle(&tri);
			render->_EndTriangleDraw();

			if (m_iAxisSelected==AS_YZ)
				render->_SetColor(IColor(1,1,0,0.5));
			else
				render->_SetColor(IColor(1,0,0,0.5));
			render->_DrawLine(IVector3D(0,AXIS_HEIGHT,0),IVector3D(0,0,AXIS_HEIGHT));
			tri.SetVertex(1,IVector3D(0,AXIS_HEIGHT,0));
			tri.SetVertex(2,IVector3D(0,0,AXIS_HEIGHT));
			render->_StartTriangleDraw(false);
			render->_DrawTriangle(&tri);
			render->_EndTriangleDraw();
			render->_DisableExt(IEXT_BLEND);
		}
	}
}

void InitialPanel::CheckAxis(Initial::Core::IVector3D& start, Initial::Core::IVector3D& dir)
{
	m_iAxisSelected=AS_NONE;
	IVector3D center;
	if (GetSelectionCenter(center))
	{
		//Test axis
		IVector3D res1, res2, len, axisDir;
		IVector3D axisX, axisY, axisZ;
		axisX=m_mTransMatrix*IVector3D(1,0,0)*m_fAxisScale;
		axisY=m_mTransMatrix*IVector3D(0,1,0)*m_fAxisScale;
		axisZ=m_mTransMatrix*IVector3D(0,0,1)*m_fAxisScale;

		Math::Intersect::RayIntersectRay(start,dir,center,axisX,res1,res2);
		len=res1-res2;
		if (len.Length()<=TOLERANCE*m_fAxisScale && Math::Intersect::PointOnRay(res2,center,center+axisX))
		{
			m_iAxisSelected=AS_X;
		}
		
		Math::Intersect::RayIntersectRay(start,dir,center,axisY,res1,res2);
		len=res1-res2;
		if (len.Length()<=TOLERANCE*m_fAxisScale && Math::Intersect::PointOnRay(res2,center,center+axisY))
		{
			m_iAxisSelected=AS_Y;
		}
		
		Math::Intersect::RayIntersectRay(start,dir,center,axisZ,res1,res2);
		len=res1-res2;
		if (len.Length()<=TOLERANCE*m_fAxisScale && Math::Intersect::PointOnRay(res2,center,center+axisZ))
		{
			m_iAxisSelected=AS_Z;
		}

		//Test plane
		if (m_iAxisSelected==AS_NONE)
		{
			ITriangle tri;
			tri.SetVertex(0,center);

			tri.SetVertex(1,center+axisX);
			tri.SetVertex(2,center+axisY);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res1,false))
			{
				m_iAxisSelected=AS_XY;
			}

			tri.SetVertex(1,center+axisX);
			tri.SetVertex(2,center+axisZ);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res1,false))
			{
				m_iAxisSelected=AS_XZ;
			}

			tri.SetVertex(1,center+axisY);
			tri.SetVertex(2,center+axisZ);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res1,false))
			{
				m_iAxisSelected=AS_YZ;
			}
		}
	}
}

void InitialPanel::DrawRotateAxis()
{
	if (m_pDevice)
	{
		Video::IRenderDriver *render = m_pDevice->GetRenderDriver();
		if (render)
		{			
			render->UseMaterial(m_EmissiveVertexAlpha);
			ITriangle tri;
			render->_StartTriangleDraw(false);

			if (m_iAxisSelected==AS_X)
				render->_SetColor(IColor(1,1,0,0.5));
			else
				render->_SetColor(IColor(1,0,0,0.5));
			for (int i=0;i<=CIRCLE_QUALITY;i++)
			{
				tri.SetVertex(0,IVector3D(0,cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(1,IVector3D(0,CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(2,IVector3D(0,cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY))));
				render->_DrawTriangle(&tri);

				tri.SetVertex(0,IVector3D(0,CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(1,IVector3D(0,cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(2,IVector3D(0,CIRCLE_HEIGHT*cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY))));
				render->_DrawTriangle(&tri);
			}

			if (m_iAxisSelected==AS_Y)
				render->_SetColor(IColor(1,1,0,0.5));
			else
				render->_SetColor(IColor(0,1,0,0.5));
			for (int i=0;i<=CIRCLE_QUALITY;i++)
			{
				tri.SetVertex(0,IVector3D(cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0,sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(1,IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0,CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(2,IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0,sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY))));
				render->_DrawTriangle(&tri);

				tri.SetVertex(0,IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0,CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(1,IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0,sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY))));
				tri.SetVertex(2,IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0,CIRCLE_HEIGHT*sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY))));
				render->_DrawTriangle(&tri);
			}

			if (m_iAxisSelected==AS_Z)
				render->_SetColor(IColor(1,1,0,0.5));
			else
				render->_SetColor(IColor(0,0,1,0.5));
			for (int i=0;i<=CIRCLE_QUALITY;i++)
			{
				tri.SetVertex(0,IVector3D(cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0));
				tri.SetVertex(1,IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0));
				tri.SetVertex(2,IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0));
				render->_DrawTriangle(&tri);

				tri.SetVertex(0,IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0));
				tri.SetVertex(1,IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0));
				tri.SetVertex(2,IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0));
				render->_DrawTriangle(&tri);
			}

			render->_EndTriangleDraw();
		}
	}
}

void InitialPanel::CheckRotateAxis(Initial::Core::IVector3D& start, Initial::Core::IVector3D& dir)
{
	m_iAxisSelected=AS_NONE;
	IVector3D center;
	if (GetSelectionCenter(center))
	{
		//Test axis
		IVector3D res;
		ITriangle tri;
		for (int i=0;i<=CIRCLE_QUALITY;i++)
		{
			tri.SetVertex(0,center+m_mTransMatrix*IVector3D(0,cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(1,center+m_mTransMatrix*IVector3D(0,CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(2,center+m_mTransMatrix*IVector3D(0,cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res,false))
			{
				m_iAxisSelected=AS_X;
				return;
			}

			tri.SetVertex(0,center+m_mTransMatrix*IVector3D(0,CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(1,center+m_mTransMatrix*IVector3D(0,cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(2,center+m_mTransMatrix*IVector3D(0,CIRCLE_HEIGHT*cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res,false))
			{
				m_iAxisSelected=AS_X;
				return;
			}
		}

		for (int i=0;i<=CIRCLE_QUALITY;i++)
		{
			tri.SetVertex(0,center+m_mTransMatrix*IVector3D(cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0,sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(1,center+m_mTransMatrix*IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0,CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(2,center+m_mTransMatrix*IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0,sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res,false))
			{
				m_iAxisSelected=AS_Y;
				return;
			}

			tri.SetVertex(0,center+m_mTransMatrix*IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0,CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(1,center+m_mTransMatrix*IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0,sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			tri.SetVertex(2,center+m_mTransMatrix*IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0,CIRCLE_HEIGHT*sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)))*m_fAxisScale);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res,false))
			{
				m_iAxisSelected=AS_Y;
				return;
			}
		}

		for (int i=0;i<=CIRCLE_QUALITY;i++)
		{
			tri.SetVertex(0,center+m_mTransMatrix*IVector3D(cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0)*m_fAxisScale);
			tri.SetVertex(1,center+m_mTransMatrix*IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0)*m_fAxisScale);
			tri.SetVertex(2,center+m_mTransMatrix*IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0)*m_fAxisScale);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res,false))
			{
				m_iAxisSelected=AS_Z;
				return;
			}

			tri.SetVertex(0,center+m_mTransMatrix*IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD(i*360.0/CIRCLE_QUALITY)),0)*m_fAxisScale);
			tri.SetVertex(1,center+m_mTransMatrix*IVector3D(cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0)*m_fAxisScale);
			tri.SetVertex(2,center+m_mTransMatrix*IVector3D(CIRCLE_HEIGHT*cos(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),CIRCLE_HEIGHT*sin(DEG_TO_RAD((i+1)*360.0/CIRCLE_QUALITY)),0)*m_fAxisScale);
			if (Math::Intersect::RayIntersectTriangle(tri,start,start+dir,res,false))
			{
				m_iAxisSelected=AS_Z;
				return;
			}
		}
	}
}
