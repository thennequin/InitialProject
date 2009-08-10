
#ifndef _INITIALPANEL_HEADER_
#define _INITIALPANEL_HEADER_

#include <wx/wx.h>

#include "Initial/Initial.h"

class InitialPanel : public wxWindow
{
	DECLARE_EVENT_TABLE()
public:
	InitialPanel(wxWindow *parent,
                     wxWindowID winid = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize);

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event) { } // Evite le scintillement

	void OnSize(wxSizeEvent& event);

	void OnMouseEvent(wxMouseEvent &event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);

	void OnTimer(wxTimerEvent &event);

	Initial::IDevice *GetDevice();

	void Start();
	void Stop();

protected:
	wxTimer Timer;

	Initial::IDevice *m_pDevice;
	Initial::ICameraFPS *m_pCamera;
	/*Initial::IAnimatedMesh *animObject;

	Initial::IMesh *_3ds;
	Initial::IMesh *_3ds2;
	Initial::IMesh *_3ds3;

	Initial::INodeLight *m_pLight;*/
	long m_lTime;
	float m_fTime;
	bool m_bPlay;
};

#endif