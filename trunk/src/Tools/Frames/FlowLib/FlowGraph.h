
#ifndef _FLOWGRAPH_HEADER_
#define _FLOWGRAPH_HEADER_

#include <wx/wx.h>

#include "GraphShape.h"

#include "Initial/Core/IArray.h"

#define KEY_SHIFT 1
#define KEY_CTRL 2

class FlowGraph : public wxScrolledWindow
{
	enum DragType
	{
		DT_SELECT,
		DT_CANVAS,
		DT_SHAPE,
		DT_CONNECTION
	};
public:
	FlowGraph(wxWindow* parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    virtual ~FlowGraph();

	void Redraw(wxDC& dc);

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnResize(wxSizeEvent& event);
    //bool OnCloseCanvas();
    void OnMouse(wxMouseEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);
    //void SetCursorStyle(int x, int y);
	void OnTimer(wxTimerEvent &event);

	virtual void RightClick(wxPoint pos);
	virtual void ShapeSelected(GraphShape* shape);
	virtual void ShapeConnected(GraphShape* shape1, GraphShape* shape2);
	virtual void OnDeleteShape(GraphShape* shape);

	void AddShape(GraphShape* shape);
	bool DeleteShape(GraphShape* shape, bool delData=true);
	void DeleteSelection();
	float GetZoom();
	wxPoint GetMousePos();
	wxList *GetShapesList();

	void ResetSelection();

	DECLARE_EVENT_TABLE()
private:
	wxPoint m_iMousePos;
	float m_fZoom;
	float m_fZoomTo;
	wxTimer m_Timer;

	bool m_bAnimateZoom;

	bool m_bLeftClick;
	bool m_bRightClick;
	bool m_bDrag;
	bool m_bDraged;
	int m_iDragType;
	wxPoint m_iStartDrag;
	wxPoint m_iDragPos;

	GraphShape *m_pDragConnectionShape;
	int m_iDragConnectionId;
	int m_bDragConnectionIsOutput;

	int m_iKeys;

	//wxArrayShape m_aShapes;
	Initial::Core::IArray<GraphShape*> m_aShapes;
};

#endif