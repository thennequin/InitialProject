
#ifndef _INITIALPANEL_HEADER_
#define _INITIALPANEL_HEADER_

#include <wx/wx.h>

#include "Initial/Initial.h"

class InitialPanel : public wxWindow
{
	DECLARE_EVENT_TABLE()
public:
	enum ToolType
	{
		TT_NONE,
		TT_SELECT,
		TT_MOVE,
		TT_ROTATE,
		TT_SCALE,
	};
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

	void SetTool(ToolType tool);
	ToolType GetTool();

	void Start();
	void Stop();
protected:
	void DrawAxis();
	void DrawArrow(float arrowheight, float width);
protected:
	wxTimer Timer;

	bool m_bRender;
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

	ToolType m_iTool;

	Initial::ISmartPtr<Initial::Video::IMaterial> m_EmissiveVertexAlpha;
};

#endif