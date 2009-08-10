#include "FlowGraph.h"
#include "Connection.h"

#include "wx/dcbuffer.h"

BEGIN_EVENT_TABLE(FlowGraph, wxScrolledWindow)
	EVT_PAINT(FlowGraph::OnPaint)
	EVT_ERASE_BACKGROUND(FlowGraph::OnEraseBackground)
	EVT_SIZE(FlowGraph::OnResize)

	EVT_MOUSE_EVENTS(FlowGraph::OnMouse)
	EVT_KEY_UP(FlowGraph::OnKeyUp)
	EVT_KEY_DOWN(FlowGraph::OnKeyDown)
	EVT_TIMER(0,FlowGraph::OnTimer)
END_EVENT_TABLE()

FlowGraph::FlowGraph(wxWindow* parent, const wxPoint& pos, const wxSize& size) 
: wxScrolledWindow(parent, wxID_ANY, pos, size),
m_fZoom(1.0), m_fZoomTo(1.0), m_bDrag(false), m_bDraged(false), m_iDragType(DT_CANVAS), m_bRightClick(false), m_bLeftClick(false), m_bAnimateZoom(true), m_iKeys(0)
{
	m_Timer.SetOwner(this,0);
	if (m_bAnimateZoom)
		m_Timer.Start(10);
	else
		m_Timer.Start(1000);
}

FlowGraph::~FlowGraph()
{

}

void FlowGraph::OnKeyUp(wxKeyEvent& event)
{
	if (event.GetKeyCode()==WXK_SHIFT)
		m_iKeys = m_iKeys&~KEY_SHIFT;
	
	if (event.GetKeyCode()==WXK_CONTROL)
		m_iKeys = m_iKeys&~KEY_CTRL;
}

void FlowGraph::OnKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode()==WXK_SHIFT)
		m_iKeys = m_iKeys|KEY_SHIFT;
	
	if (event.GetKeyCode()==WXK_CONTROL)
		m_iKeys = m_iKeys|KEY_CTRL;

	if (event.GetKeyCode()==WXK_DELETE)
		DeleteSelection();
}

void FlowGraph::OnMouse(wxMouseEvent& event)
{	
	if (event.GetWheelRotation()!=0) // Zoom
	{
		m_fZoomTo-=(abs(event.GetWheelRotation())/event.GetWheelRotation())/10.0;

		if (m_fZoomTo<1)
			m_fZoomTo=1;

		if (m_fZoomTo>10)
			m_fZoomTo=10;
	}

	bool Mouse2=false;
	if (event.ButtonIsDown(2))
		Mouse2=true;

	if (event.ButtonIsDown(1) || Mouse2)
	{
		SetFocus();
		if (m_bDrag==false)
		{
			m_bLeftClick=true;
			m_bDrag=true;
			if ((m_iKeys&KEY_CTRL) || Mouse2)
				m_iDragType=DT_CANVAS;
			else
				m_iDragType=DT_SELECT;
			
			//Search if mouse is on shape
			if (m_aShapes.Count()>0 && m_iDragType!=DT_CANVAS)
			{
				for (int i=m_aShapes.Count()-1;i>=0;i--)
				{
					int test = m_aShapes.Count()-1;
					if (m_aShapes[i])
					{
						if (m_aShapes[i]->InShape(event.GetX(),event.GetY()))
						{
							int id;
							if ((id=m_aShapes[i]->HitInput(event.GetX(),event.GetY()))!=-1)
							{
								m_iDragType=DT_CONNECTION;
								m_pDragConnectionShape=m_aShapes[i];
								m_iDragConnectionId=id;
								m_bDragConnectionIsOutput=false;
							}else if ((id=m_aShapes[i]->HitOutput(event.GetX(),event.GetY()))!=-1)
							{
								m_iDragType=DT_CONNECTION;
								m_pDragConnectionShape=m_aShapes[i];
								m_iDragConnectionId=id;
								m_bDragConnectionIsOutput=true;
							}else if (m_aShapes[i]->Selected())
								m_iDragType=DT_SHAPE;
							break;
						}
					}
				}
			}

			if (m_iDragType==DT_CANVAS)
			{
				wxGetMousePosition(&(m_iStartDrag.x),&(m_iStartDrag.y));
				m_iDragPos=wxPoint(0,0);
			}else
				m_iStartDrag=event.GetPosition();

			// Set drag mode on all shape
			for (unsigned int i=0;i<m_aShapes.Count();i++)
			{
				if (m_aShapes[i])
					m_aShapes[i]->StartDrag();
			}
		}else{
			bool HideCursor=false;
			if (m_bDraged==false)
				HideCursor=true;
			m_bDraged=true;				

			if (m_iDragType==DT_CANVAS)
			{		
				if (HideCursor)
					ShowCursor(false);

				int MouseX,MouseY;
				wxGetMousePosition(&MouseX,&MouseY);
				m_iDragPos+=m_iStartDrag-wxPoint(MouseX,MouseY);
				SetCursorPos(m_iStartDrag.x,m_iStartDrag.y);
				
				for (unsigned int i=0;i<m_aShapes.Count();i++)
					if (m_aShapes[i])
						m_aShapes[i]->MoveTo(-m_iDragPos.x*m_fZoom,-m_iDragPos.y*m_fZoom);
				Refresh();
			}else if (m_iDragType==DT_SHAPE)
			{
				m_iDragPos=m_iStartDrag-event.GetPosition();

				for (unsigned int i=0;i<m_aShapes.Count();i++)
					if (m_aShapes[i] && m_aShapes[i]->Selected())
						m_aShapes[i]->MoveTo(-m_iDragPos.x*m_fZoom,-m_iDragPos.y*m_fZoom);
				Refresh();
			}else if (m_iDragType==DT_CONNECTION)
			{
			}else if (m_iDragType==DT_SELECT)
			{
			}
			
		}
	}else if (m_bLeftClick==true)
	{
		if (m_bDraged==false)
		{
			bool InShape=false;
			//Search if mouse is on shape
			if (m_aShapes.Count()>0)
				for (int i=m_aShapes.Count()-1;i>=0;i--)
				{
					if (m_aShapes[i])
						if (m_aShapes[i]->InShape(event.GetX(),event.GetY()))
						{
							if (!(m_iKeys&KEY_SHIFT))
							{
								ResetSelection();
								m_aShapes[i]->SetSelected(true);
							}else							
								m_aShapes[i]->SetSelected(!m_aShapes[i]->Selected());
							
							InShape=true;
							ShapeSelected(m_aShapes[i]);
							break;
						}
				}
			// If not click on shape of shift not down, deselect all shape
			
			if (!InShape && !(m_iKeys&KEY_SHIFT))
			{
				ResetSelection();
				ShapeSelected(NULL);	
			}
		}else if (m_iDragType==DT_CANVAS)
			ShowCursor(true);
		else if (m_iDragType==DT_CONNECTION)
		{
			if (m_aShapes.Count()>0)
				for (int i=m_aShapes.Count()-1;i>=0;i--)
				{
					if (m_aShapes[i])
					{
						if (m_aShapes[i]->InShape(event.GetX(),event.GetY()))
						{
							if (m_bDragConnectionIsOutput)
							{
								int id;
								if ((id=m_aShapes[i]->HitInput(event.GetX(),event.GetY()))!=-1)
								{
									m_aShapes[i]->ConnectInput(id,m_pDragConnectionShape,m_iDragConnectionId);
									ShapeConnected(m_aShapes[i],m_pDragConnectionShape);
									//Connect
								}
							}else{
								int id;
								if ((id=m_aShapes[i]->HitOutput(event.GetX(),event.GetY()))!=-1)
								{
									m_pDragConnectionShape->ConnectInput(m_iDragConnectionId,m_aShapes[i],id);
									ShapeConnected(m_pDragConnectionShape,m_aShapes[i]);
									//Connect
								}
							}
						}
					}
				}
		}else if (m_iDragType==DT_SELECT)
		{
			if (!(m_iKeys&KEY_SHIFT))
				ResetSelection();
			for (int i=m_aShapes.Count()-1;i>=0;i--)
			{
				if (m_aShapes[i])
				{
					if (m_aShapes[i]->ContainsShape(m_iStartDrag.x,m_iStartDrag.y,m_iMousePos.x-m_iStartDrag.x,m_iMousePos.y-m_iStartDrag.y))
						m_aShapes[i]->SetSelected(true);
				}
			}

		}

		m_bLeftClick=false;
		m_bDrag=false;
		m_bDraged=false;		
	}

	if (event.Button(3))
	{
		//wxLogMessage("RightClick");
		bool Hit=false;
		for (unsigned int i=0;i<m_aShapes.Count();i++)
		{
			int id;
			if (m_aShapes[i])
				if (m_aShapes[i]->InShape(event.GetX(),event.GetY()))
				{
					if ((id=m_aShapes[i]->HitOutput(event.GetX(),event.GetY()))!=-1)
					{
						Hit=true;
						for (unsigned int j=0;j<m_aShapes.Count();j++)
						{
							if (m_aShapes[j])
							{
								for (unsigned int k=0;k<m_aShapes[j]->Inputs.Count();k++)
								{
									if (m_aShapes[j]->Inputs[k]->GetConnection())
									{
										if (m_aShapes[j]->Inputs[k]->GetConnection()->OutputShape==m_aShapes[i]
											&& m_aShapes[j]->Inputs[k]->GetConnection()->OutputId==id)
											{
												m_aShapes[j]->ConnectInput(k,NULL,0);
												ShapeConnected(m_aShapes[j],NULL);
											}
									}
								}
							}
						}
						// Delete Connection

						break;
					}else if ((id=m_aShapes[i]->HitInput(event.GetX(),event.GetY()))!=-1)
					{
						Hit=true;
						m_aShapes[i]->ConnectInput(id,NULL,0);
						ShapeConnected(m_aShapes[i],NULL);
					}
				}
		}
		if (!Hit)
		{
			// Context Menu
			RightClick(event.GetPosition ());
		}
	}

	m_iMousePos = event.GetPosition();
	Refresh();
}

void FlowGraph::RightClick(wxPoint pos)
{

}

void FlowGraph::ShapeSelected(GraphShape* shape)
{

}

void FlowGraph::ShapeConnected(GraphShape* shape1, GraphShape* shape2)
{

}

void FlowGraph::OnDeleteShape(GraphShape* shape)
{

}

void FlowGraph::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
    PrepareDC(dc);

	dc.SetBackground(wxBrush(GetBackgroundColour(), wxSOLID));
    dc.Clear();

//	wxGraphicsContext *dc2 = wxGraphicsContext::Create(dc);
	Redraw(dc);
	//Redraw(*((wxDC*)dc2));
}

void FlowGraph::Redraw(wxDC& dc)
{
	int w,h;
	GetSize(&w,&h);

	//dc.SetBrush(wxBrush(wxColour(216,255,255)));
	//dc.DrawRectangle(0,0,w,h);
	//wxString str;
	//str.Printf("Drag %d DragShape %d",m_bDrag,m_iDragType);
	//dc.DrawText(str,10,10);

	//Draw Grid
	/*int LineSpace=5;
	for (int i=0;i<GetSize().x/LineSpace;i++)
		dc.DrawLine(wxPoint(i*LineSpace,0),wxPoint(i*LineSpace,GetSize().y));*/

	for (unsigned int i=0;i<m_aShapes.Count();i++)
	{
		if (m_aShapes[i])
			m_aShapes[i]->Draw(dc);
	}

	// Draw current Connection creation
	if (m_bDrag && m_iDragType==DT_CONNECTION)
	{
		if (m_pDragConnectionShape)
		{
			if (m_bDragConnectionIsOutput)
			{
				Connection::DrawConnection(dc,m_pDragConnectionShape->GetOutputPos(m_iDragConnectionId),m_iMousePos);
				int id;
				for (unsigned int i=0;i<m_aShapes.Count();i++)
				{
					if (m_aShapes[i] && m_aShapes[i]!=m_pDragConnectionShape)
						if ((id=m_aShapes[i]->HitInput(m_iMousePos.x, m_iMousePos.y))!=-1)
						{
							m_aShapes[i]->DrawInputHighlight(id, dc);
							break;
						}
				}
			}else{
				Connection::DrawConnection(dc,m_pDragConnectionShape->GetInputPos(m_iDragConnectionId),m_iMousePos);
				int id;
				for (unsigned int i=0;i<m_aShapes.Count();i++)
				{
					if (m_aShapes[i] && m_aShapes[i]!=m_pDragConnectionShape)
						if ((id=m_aShapes[i]->HitOutput(m_iMousePos.x, m_iMousePos.y))!=-1)
						{
							m_aShapes[i]->DrawOutputHighlight(id, dc);
							break;
						}
				}
			}
		}
	}
	else if (m_bDrag && m_iDragType==DT_SELECT)
	{
#if wxUSE_GRAPHICS_CONTEXT == 1
		wxGraphicsContext *dc2 = wxGraphicsContext::Create(*((wxBufferedPaintDC*)&dc));
		if  (dc2)
		{
			dc2->SetPen(wxPen(wxColour(109,182,255),1,wxSHORT_DASH));
			dc2->SetBrush(wxBrush(wxColour(182,226,255,64),wxSOLID));
			int x,y,w,h;
			x=m_iStartDrag.x;
			y=m_iStartDrag.y;
			w=m_iMousePos.x-m_iStartDrag.x;
			h=m_iMousePos.y-m_iStartDrag.y;
			if (w<0)
			{
				x=x+w;
				w=-w;
			}
			if (h<0)
			{
				y=y+h;
				h=-h;
			}
			dc2->DrawRectangle(x,y,w,h);
			delete dc2;
		}
#else
		dc.SetPen(wxPen(wxColour(109,182,255),1,wxSHORT_DASH));
		dc.SetBrush(wxBrush(wxColour(182,226,255,64),wxTRANSPARENT));
		dc.DrawRectangle(m_iStartDrag.x,m_iStartDrag.y,m_iMousePos.x-m_iStartDrag.x,m_iMousePos.y-m_iStartDrag.y);
#endif
	}
}

void FlowGraph::OnEraseBackground(wxEraseEvent& event) // Evite le scintillement
{
}

void FlowGraph::OnResize(wxSizeEvent& event)
{
	Refresh();
}

void FlowGraph::AddShape(GraphShape* shape)
{
	if (shape)
	{
		m_aShapes.Add(shape);
		shape->SetCanvas(this);
		shape->CalculateRender();
	}
}

bool FlowGraph::DeleteShape(GraphShape* shape, bool delData)
{
	if (shape)
	{
		for (unsigned int i=0;i<m_aShapes.Count();i++)
		{
			if (m_aShapes[i])
			{
				for (unsigned int j=0;j<m_aShapes[i]->Inputs.Count();j++)
				{
					if (m_aShapes[i]->Inputs[j]->Connect)
					{
						if (m_aShapes[i]->Inputs[j]->Connect->OutputShape==shape)
						{
							m_aShapes[i]->ConnectInput(j,NULL,0);
						}
					}
				}
			}
		}

		for (unsigned int j=0;j<shape->Inputs.Count();j++)
		{
			shape->ConnectInput(j,NULL,0);
		}

		OnDeleteShape(shape);
		//m_aShapes.Remove(shape);
		m_aShapes.DeleteItem(shape);
		if (delData)
			delete shape;
		return true;
	}
	return false;
}

void FlowGraph::DeleteSelection()
{
	for (unsigned int i=0;i<m_aShapes.Count();i++)
		if (m_aShapes[i])
			if (m_aShapes[i]->Selected() && m_aShapes[i]->IsDeletable())
			{
				DeleteShape(m_aShapes[i],false);
				i--;
			}
	ShapeConnected(NULL,NULL);
	Refresh();
}

float FlowGraph::GetZoom()
{
	return m_bAnimateZoom ? m_fZoom : m_fZoomTo;
}

wxPoint FlowGraph::GetMousePos()
{
	return m_iMousePos;
}

void FlowGraph::ResetSelection()
{
	for (unsigned int i=0;i<m_aShapes.Count();i++)
		if (m_aShapes[i])
			m_aShapes[i]->SetSelected(false);
}

wxList* FlowGraph::GetShapesList()
{
	wxList *list=new wxList;
	if (list)
	{
		list->Clear();
		for (unsigned int i=0;i<m_aShapes.Count();i++)
			if (m_aShapes[i])
				list->push_back(m_aShapes[i]);
	}
	return list;
}

void FlowGraph::OnTimer(wxTimerEvent &event)
{
	if (m_fZoom!=m_fZoomTo)
	{
		if (m_bAnimateZoom)
		{
			if (abs(m_fZoom-m_fZoomTo)<=0.01)
			{
				m_fZoom=m_fZoomTo;
			}else{
				float speed = abs(m_fZoom-m_fZoomTo)/10.0;
				speed = speed<0.005 ? 0.005 : speed;
				if (m_fZoom<m_fZoomTo)
					m_fZoom+=speed;
				else if (m_fZoom>m_fZoomTo)
					m_fZoom-=speed;
			}
		}else{
			m_fZoom=m_fZoomTo;
		}
		Refresh();
	}
}