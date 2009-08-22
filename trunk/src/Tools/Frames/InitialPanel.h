
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
	enum TransMode
	{
		TM_WORLD,
		TM_LOCAL,
		TM_VIEW,
	};
	enum AxisSelect
	{
		AS_NONE,
		AS_X,
		AS_Y,
		AS_Z,
		AS_XY,
		AS_XZ,
		AS_YZ
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

	void SetTransMode(TransMode mode);
	TransMode GetTransMode();

	void Start();
	void Stop();
protected:
	void DrawAxis();
	void CheckAxis(Initial::Core::IVector3D& start, Initial::Core::IVector3D& dir);
	void DrawArrow(float arrowheight, float width);

	void DrawRotateAxis();
	void CheckRotateAxis(Initial::Core::IVector3D& start, Initial::Core::IVector3D& dir);

	size_t GetSelectionCenter(Initial::Core::IVector3D& center);
	void SetSelectionPosition(Initial::Core::IVector3D pos);
	void GetSelectionAngle(Initial::Math::IMatrix& res);
	void StartDrag();
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
	TransMode m_iTransMode;

	float m_fAxisScale;
	AxisSelect m_iAxisSelected;

	bool m_bLeftClick;
	Initial::Core::IVector3D m_MouseStartDrag;
	Initial::Core::IVector3D m_MouseStartDragDir;

	Initial::Math::IMatrix m_mProjectionMatrix;
	Initial::Math::IMatrix m_mInvProjectionMatrix;
	Initial::Math::IMatrix m_mModelViewMatrix;
	Initial::Math::IMatrix m_mInvModelViewMatrix;

	Initial::Math::IMatrix m_mTransMatrix;

	Initial::ISmartPtr<Initial::Video::IMaterial> m_EmissiveVertexAlpha;
};

#endif