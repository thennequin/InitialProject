
#ifndef _BuildGraph_HEADER_
#define _BuildGraph_HEADER_

#include <wx/wx.h>

struct BuildEvent
{
	int BuildId;
	int AgentId;
	unsigned long StartTime;
	unsigned long FinishTime;
	wxString Name;
	char Type; //0=Blue, 1=Yellow, 2=Red
	unsigned long Size;
	unsigned long Current;
};

struct Agent
{
	int AgentId;
	wxString Name;
};

WX_DEFINE_ARRAY(BuildEvent*,wxBuildEventArray);
WX_DEFINE_ARRAY(Agent*,wxAgentArray);

class BuildGraph : public wxScrolledWindow
{
	DECLARE_EVENT_TABLE()
public:
	BuildGraph(wxWindow *parent,
                     wxWindowID winid = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxScrolledWindowStyle,
                     const wxString& name = wxPanelNameStr);

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event) { } // Evite le scintillement

	void OnMouseEvent(wxMouseEvent &event);

	void OnTimer(wxTimerEvent &event);

	void AddEvent(int BuildId,unsigned long Time, int AgentIdOrType=0, wxString Name=wxEmptyString);
	void SetEvent(int BuildId,unsigned long Size, unsigned long current);

	void ResetStartTime();
	void Clear();
protected:
	wxTimer Timer;

	wxBuildEventArray Events;

	wxSemaphore EventsAccess;

	unsigned long StartTime;
	//Draw value
	float TimeRatio; // Pixel/sec
	float TimeRatioTarget; // Pixel/sec
	int StartPixelsWidth;
	int StartPixelsHeight;
	int TextSize;
	int BlockHeight;
	int AgentsSpace;


	// Mouse gesture
	bool DragBorder;
};

#endif