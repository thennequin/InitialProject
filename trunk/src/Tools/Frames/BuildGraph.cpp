

#include "BuildGraph.h"
#include "wx/dcbuffer.h"

BEGIN_EVENT_TABLE(BuildGraph, wxScrolledWindow)
    EVT_PAINT(BuildGraph::OnPaint)
	EVT_ERASE_BACKGROUND(BuildGraph::OnEraseBackground)
	EVT_MOUSE_EVENTS(BuildGraph::OnMouseEvent)
	EVT_TIMER(wxID_ANY,BuildGraph::OnTimer)
END_EVENT_TABLE()

const int RefreshTime=30;
const int RefreshTimeIdle=30;

BuildGraph::BuildGraph(wxWindow *parent,
                     wxWindowID winid,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
	: wxScrolledWindow(parent,winid,pos,size,style,name)
{
	
	StartPixelsWidth=70;
	StartPixelsHeight=15;

	TextSize=16;
	BlockHeight=18;

	//TimeRatio=200.0*BlockWidth/100+10;
	TimeRatio=100;
	TimeRatioTarget=TimeRatio;

	AgentsSpace=5;

	StartTime=GetTickCount();

	DragBorder=false;

	EventsAccess.Post();
	Timer.SetOwner(this,wxID_ANY);
	Timer.Start(RefreshTimeIdle);
}

void BuildGraph::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

    PrepareDC(dc);

    dc.SetBackground(wxBrush(wxColor(255,255,255), wxSOLID));
    dc.Clear();

	int MaxWidth=GetSize().x;
	int MaxHeight=0;

	int ScrollPosH=GetScrollPos(wxHORIZONTAL);
	int ScrollPosV=GetScrollPos(wxVERTICAL);

	wxFont font = dc.GetFont();
	font.SetWeight(0);
	font.SetUnderlined(false);
	dc.SetFont(font);
	dc.SetPen(wxPen(wxColor(192,192,192)));
	//dc.SetBrush(wxBrush(wxColor(192,192,192)));
	dc.SetBrush(wxBrush(wxColor(0,0,0,0),wxTRANSPARENT));

	wxString TriDot="...";
	long TriDotSize;
	dc.GetTextExtent(TriDot,&TriDotSize,0);

	int AgentNum=1;
	EventsAccess.Wait();
	for (unsigned int i=0;i<Events.Count();i++)
	{
		
		if (Events[i]->AgentId+1>AgentNum)
			AgentNum=Events[i]->AgentId+1;

		float sec = (float)(Events[i]->StartTime-StartTime)/1000.0;

		wxColour blockcolour;
		wxColour blockcolourdest;

		wxString Text;	

		float FinishTime;
		bool Finish;
		if (Events[i]->StartTime>Events[i]->FinishTime)
		{
			blockcolour = wxColour(172,172,255);
			blockcolourdest = wxColour(232,232,255);
			FinishTime=GetTickCount();
			Finish=false;
		}else{
			if (Events[i]->Type==0) // Blue
			{
				blockcolour = wxColour(80,255,255);
				blockcolourdest = wxColour(192,255,255);
			}else if (Events[i]->Type==1) // Green
			{
				blockcolour = wxColour(80,255,80);
				blockcolourdest = wxColour(200,255,200);
			}else if (Events[i]->Type==2) // Yellow
			{
				blockcolour = wxColour(255,255,64);
				blockcolourdest = wxColour(255,255,184);
			}else{ // Red
				blockcolour = wxColour(255,48,48);
				blockcolourdest = wxColour(255,172,172);
			}
			FinishTime=Events[i]->FinishTime;
			Finish=true;
		}
		float BlockWidth = (FinishTime-Events[i]->StartTime)*TimeRatio/1000.0;
		float BlockWidth2 = BlockWidth; 
		if (BlockWidth<0)
			BlockWidth=0;

		if (Finish==false)
			Text.Printf("%s (%.2f%%)",Events[i]->Name,100.0*Events[i]->Current/Events[i]->Size);
		else
			Text.Printf("%s (%.2f)",Events[i]->Name,(FinishTime-Events[i]->StartTime)/1000.0);

		long size;
		dc.GetTextExtent(Text,&size,NULL);

		if (Finish==false)
		{
			BlockWidth = BlockWidth>size+6?BlockWidth:size+6;
			//BlockWidth = Events[i]->Size/((long)Events[i]->Current+1)*(FinishTime-Events[i]->StartTime)*TimeRatio/1000.0;
		}

		int cornerL,cornerR;
		if (StartPixelsWidth+TimeRatio*sec<ScrollPosH)
			cornerL=-1;
		else if (StartPixelsWidth+TimeRatio*sec>ScrollPosH+GetSize().x)
			cornerL=1;
		else
			cornerL=0;

		if (StartPixelsWidth+TimeRatio*sec+BlockWidth<ScrollPosH)
			cornerR=-1;
		else if (StartPixelsWidth+TimeRatio*sec+BlockWidth>ScrollPosH+GetSize().x)
			cornerR=1;
		else
			cornerR=0;

		if (BlockWidth>=0.5 && ((StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId>ScrollPosV && 
			StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId<ScrollPosV+GetSize().y)
			|| (StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId+BlockHeight>ScrollPosV && 
			StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId+BlockHeight<ScrollPosV+GetSize().y))
			&&
			(cornerR==0 || cornerL==0 || (cornerL==-1 && cornerR==1)) )
		{
			dc.DrawRectangle(StartPixelsWidth+TimeRatio*sec+1,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId,BlockWidth-1,BlockHeight);

			dc.GradientFillLinear(wxRect(StartPixelsWidth+TimeRatio*sec+2,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId+1,BlockWidth2-3,BlockHeight-2),blockcolour,blockcolourdest,wxNORTH);

			if (size<=BlockWidth-7 || Finish==false)
				dc.DrawText(Text,StartPixelsWidth+TimeRatio*sec+4,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId+3);
			else{
				if (TriDotSize<=BlockWidth-7)
				{
					//	dc.DrawText(TriDot,StartPixelsWidth+TimeRatio*sec+3,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId+3);

					wxString Temp;
					for (int j=Text.Len()-TriDot.Len();j>=0;j--)
					{
						Temp=Text.Left(j);
						dc.GetTextExtent(Temp,&size,NULL);
						if (size+TriDotSize<=BlockWidth-7)
						{
							dc.DrawText(Temp+TriDot,StartPixelsWidth+TimeRatio*sec+4,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId+3);
							break;
						}
					}
				}
			}
		}

		if (MaxWidth < StartPixelsWidth+TimeRatio*sec+3+BlockWidth+5)
			MaxWidth = StartPixelsWidth+TimeRatio*sec+3+BlockWidth+5;
	}
	EventsAccess.Post();

	//Draw time
	font.SetPointSize(10);
	dc.SetFont(font);
	dc.SetPen(wxPen(wxColour()));
	int itenum=(MaxWidth-StartPixelsWidth)/TimeRatio;

	int modul=1;
	if (TimeRatio!=0)
		modul = 50.0/TimeRatio;

	int i=(ScrollPosH-StartPixelsWidth)/TimeRatio;
	if (i<1)
		i=1;
	int j=modul;
	for (;i<itenum;i++)
	{
		if (j==0)
		{
			int x=StartPixelsWidth+i*TimeRatio;
			if (x>=ScrollPosH)
			{
				dc.DrawLine(x,0,x,5);
				wxString str;
				
				str.Printf("%d:%02d",i/60,i-60*(i/60));
				long size;
				dc.GetTextExtent(str,&size,NULL);
				dc.DrawText(str,x-size/2,5);
			}
			j=modul;
			if (x>GetSize().x+ScrollPosH)
				break;
		}else
			j--;
	}

	//Draw Agents

	dc.SetBrush(wxBrush(wxColour(255,255,255)));
	dc.SetPen(wxPen(wxColour(255,255,255)));
	dc.DrawRectangle(ScrollPosH,ScrollPosV,StartPixelsWidth-10,GetSize().y);
	dc.SetPen(wxPen(wxColour(192,192,192),1,wxDOT));

	for (int i=0;i<AgentNum;i++)
	{
		wxString str;
		str.Printf("Agent %d",i+1);
		
		dc.DrawText(str,5+ScrollPosH,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*i+3);

		dc.DrawLine(ScrollPosH,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*i+BlockHeight,ScrollPosH+StartPixelsWidth-10,StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*i+BlockHeight);
	}

	dc.SetPen(wxPen());
	dc.DrawLine(ScrollPosH+StartPixelsWidth-10,GetScrollPos(wxVERTICAL),ScrollPosH+StartPixelsWidth-10,GetSize().y+GetScrollPos(wxVERTICAL));

	MaxHeight=AgentsSpace*2+(AgentNum+1)*(AgentsSpace+BlockHeight);

	//Deplacement auto de la scrollbar horizontal

	int sh = /*GetScrollPageSize(wxHORIZONTAL)*/0;
	//MaxWidth+=sh;

	//test.Printf("%d %d", GetScrollPos(wxHORIZONTAL),GetScrollLines(wxHORIZONTAL)-GetSize().x);
	int HorizPos;
	if (GetScrollPos(wxHORIZONTAL)>=GetScrollLines(wxHORIZONTAL)-GetSize().x /*&& GetScrollPageSize(wxHORIZONTAL)>GetSize().x*/) 
		HorizPos=MaxWidth;
	else
		HorizPos=GetScrollPos(wxHORIZONTAL);

	//if (GetScrollLines(wxHORIZONTAL)<GetSize().x)
		HorizPos=GetScrollPos(wxHORIZONTAL);

	SetScrollbars(1,1,MaxWidth,MaxHeight,HorizPos,GetScrollPos(wxVERTICAL),true);
}

void BuildGraph::AddEvent(int BuildId,unsigned long Time, int AgentIdOrType, wxString Name)
{
	EventsAccess.Wait();

	for (unsigned int i=0;i<Events.Count();i++)
	{
		if (Events[i]->BuildId==BuildId)
		{
			Events[i]->FinishTime=Time;
			Events[i]->Type=AgentIdOrType;
			EventsAccess.Post();
			//Refresh();
			return;
		}
	}

	BuildEvent *New = new BuildEvent();
	New->BuildId=BuildId;
	New->AgentId=AgentIdOrType;
	New->StartTime=Time;
	New->FinishTime=New->StartTime-1;
	New->Name=Name;
	New->Size=1;
	New->Current=0;
	Events.Add(New);

	EventsAccess.Post();
}

void BuildGraph::SetEvent(int BuildId,unsigned long Size, unsigned long current)
{
	EventsAccess.Wait();

	for (unsigned int i=0;i<Events.Count();i++)
	{
		if (Events[i]->BuildId==BuildId)
		{
			if (Size!=0)
				Events[i]->Size=Size;
			Events[i]->Current=current;
			EventsAccess.Post();
			return;
		}
	}

	EventsAccess.Post();
}

void BuildGraph::OnMouseEvent(wxMouseEvent &event)
{
	//SetToolTip("My tips");
	if (event.ButtonDown())
		SetFocus();

	if (event.GetPosition().x>=StartPixelsWidth-12/*-GetScrollPos(wxHORIZONTAL)*/ && event.GetPosition().x<=StartPixelsWidth-8/*-GetScrollPos(wxHORIZONTAL)*/)
	{
		SetCursor(wxCURSOR_SIZEWE);
		if (event.ButtonDown(wxMOUSE_BTN_LEFT))
			DragBorder=true;
	}else{
		SetCursor(NULL);
	}

	//ToolTip
	EventsAccess.Wait();
	int posX=event.GetPosition().x;
	int posY=event.GetPosition().y;
	bool OverEvent=false;
	for (unsigned int i=0;i<Events.Count();i++)
	{
		if (Events[i] && Events[i]->StartTime<Events[i]->FinishTime)
		{
			float sec = (float)(Events[i]->StartTime-StartTime)/1000.0;
			float BlockWidth = (Events[i]->FinishTime-Events[i]->StartTime)*TimeRatio/1000.0 - 1;
			float x=StartPixelsWidth+TimeRatio*sec+1;
			float y=StartPixelsHeight+AgentsSpace+(AgentsSpace+BlockHeight)*Events[i]->AgentId;
			if (posX>=x && 
				posY>=y &&
				posX<=x+BlockWidth && posY<=y+BlockHeight)
			{
				SetToolTip(Events[i]->Name);
				OverEvent=true;
				break;
			}
		}
	}
	if (OverEvent==false)
	{
		UnsetToolTip();
	}
	EventsAccess.Post();

	if (DragBorder==false)
	{
		if (event.GetWheelRotation()!=0) // Zoom
		{
			int sign=(abs(event.GetWheelRotation())/event.GetWheelRotation());
			//TimeRatio+=sign*TimeRatio/10;
			TimeRatioTarget=TimeRatioTarget+sign*TimeRatio/10;
			if (TimeRatioTarget<0.001)
				TimeRatioTarget=0.001f;
			Timer.Start(RefreshTime);

			Refresh();
		}
	}else{
		if (event.ButtonUp(wxMOUSE_BTN_LEFT))
			DragBorder=false;
		else{
			StartPixelsWidth=event.GetPosition().x+10/*+GetScrollPos(wxHORIZONTAL)*/;
			if (StartPixelsWidth<70)
				StartPixelsWidth=70;
			Refresh();
		}
	}
}


void BuildGraph::OnTimer(wxTimerEvent &event)
{
	if (TimeRatio!=TimeRatioTarget)
	{
		float val = (TimeRatioTarget-TimeRatio)/10.0;
		if (abs(val)<0.00001)
		{
			val=0.00001*abs(val)/val;
		}
		TimeRatio+=val;
		if ((val<0 && TimeRatio-TimeRatioTarget<0)
			|| (val>0 && TimeRatio-TimeRatioTarget>0))
		{
			TimeRatio=TimeRatioTarget;
			Timer.Start(RefreshTimeIdle);
		}
	}
	Refresh();
}

void BuildGraph::ResetStartTime()
{
	StartTime=GetTickCount();
}


void BuildGraph::Clear()
{
	for (unsigned int i=0;i<Events.Count();i++)
	{
		delete Events[i];
	}
	Events.Clear();
}
