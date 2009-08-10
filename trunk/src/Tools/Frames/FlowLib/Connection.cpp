
#include "FlowGraph.h"
#include "GraphShape.h"
#include "Connection.h"
#include "InOutput.h"


Connection::Connection()
{
	//ConnectionsList.Add(this);
	InputShape=NULL;
	OutputShape=NULL;
}

Connection::~Connection()
{
	if (Input)
	{
		//wxLogMessage("Delete Input");
		Input->Connect=NULL;
	}

	if (Output)
	{
		//wxLogMessage("Delete Output");
		Output->Connect=NULL;
	}

}

wxPoint _Bezier(wxPoint pt1, wxPoint pt2, wxPoint inter1, wxPoint inter2, float pos)
{
	float pttx,ptty;
	float Q0x,Q0y,Q1x,Q1y,Q2x,Q2y,Q3x,Q3y,Q4x,Q4y;
	Q0x=pt1.x+(inter1.x-pt1.x)*pos;
	Q0y=pt1.y+(inter1.y-pt1.y)*pos;
	Q1x=inter1.x+(inter2.x-inter1.x)*pos;
	Q1y=inter1.y+(inter2.y-inter1.y)*pos;
	Q2x=inter2.x+(pt2.x-inter2.x)*pos;
	Q2y=inter2.y+(pt2.y-inter2.y)*pos;
	Q3x=Q0x+(Q1x-Q0x)*pos;
	Q3y=Q0y+(Q1y-Q0y)*pos;
	Q4x=Q1x+(Q2x-Q1x)*pos;
	Q4y=Q1y+(Q2y-Q1y)*pos;

	pttx=Q3x+(Q4x-Q3x)*pos;
	ptty=Q3y+(Q4y-Q3y)*pos;
	return wxPoint(pttx,ptty);
}

void Connection::DrawConnection(wxDC& dc)
{
	if (InputShape && OutputShape)
	{
		float Zoom = InputShape->GetCanvas() ? InputShape->GetCanvas()->GetZoom() : 1;
		wxPoint pos1 = InputShape->GetInputPos(InputId);
		wxPoint pos2 = OutputShape->GetOutputPos(OutputId);

		DrawConnection(dc,pos1,pos2);
	}
}

void Connection::DrawConnection(wxDC& dc, wxPoint Start, wxPoint End)
{
	int Smooth=10;
	float inc = 1.0/(Smooth*2);
	wxPoint inter1,inter2;
	inter1.y=Start.y;
	inter1.x=Start.x+(End.x-Start.x)/2;

	inter2.y=End.y;
	inter2.x=End.x+(Start.x-End.x)/2;

	dc.SetBrush(wxBrush());
	dc.SetPen(wxPen());
	for (float i=0;i<1;i+=inc)
	{
		wxPoint l1,l2;
		l1=_Bezier(Start,End,inter1,inter2,i);
		l2=_Bezier(Start,End,inter1,inter2,i+inc);
		dc.DrawLine(l1.x,l1.y,l2.x,l2.y);
	}
}