
#ifndef _GRAPHSHAPE_HEADER_
#define _GRAPHSHAPE_HEADER_

#include <wx/wx.h>

#include "ShapeDrawParameter.h"
#include "ShapeRenderParameter.h"
#include "InOutput.h"
#include "Initial/Core/IArray.h"

class FlowGraph;

class GraphShape;

//WX_DEFINE_ARRAY_PTR(InOutput*, wxArrayInOutput);
//WX_DEFINE_ARRAY_PTR(GraphShape*,wxArrayShape);

enum InputFormat
{
	IF_ONE = 0x0000000F, // One canal (R, G, B or A) (float)
	IF_RED = IF_ONE - 1,
	IF_GREEN = IF_ONE - 2,
	IF_BLUE = IF_ONE - 3,
	IF_ALPHA = IF_ONE - 4,
	IF_TWO = 0x000000F0, // Two canal (x,y coord) (vec2, float)
	IF_THREE = 0x00000F00, // Three canal (RGB) (vec3, float)
	IF_FOUR = 0x0000F000, // Four canal (RGBA) (vec4, float)
	IF_MAX
};


class GraphShape : public wxObject
{
	DECLARE_DYNAMIC_CLASS(GraphShape)

	friend class FlowGraph;
	friend class ShaderGenerator;
public:
	GraphShape();
	~GraphShape();

	wxString GetName() { return Name; }
	//wxString GetCategorie() { return Categorie; }

	virtual void CalculateRender();
	virtual void Draw(wxDC& dc);
	virtual void DrawInputHighlight(int id, wxDC& dc);
	virtual void DrawOutputHighlight(int id, wxDC& dc);
	virtual bool InShape(int x, int y);
	virtual bool ContainsShape(int x, int y, int width, int height);
	virtual int HitOutput(double x, double y);
	virtual int HitInput(double x, double y);
	virtual void OnDraw(wxDC& dc);

	void StartDrag();
	void MoveTo(int x, int y);

	void SetPos(int x, int y);
	int GetX();
	int GetY();

	void SetSelected(bool select);
	bool Selected();

	wxPoint GetInputPos(int id);
	wxPoint GetOutputPos(int id);
	GraphShape* GetShapeOnInput(int id);
	int GetShapeOutputIdOnInput(int id);

	int GetInputCount();
	int GetOutputCount();

	void ConnectInput(int InId,GraphShape *dest,int destOutId);

	FlowGraph *GetCanvas();
	void SetCanvas(FlowGraph* canvas);

	bool IsDeletable();
	bool IsPreviewActive();
protected:
	FlowGraph *m_pCanvas;
	wxPoint m_iPos;
	wxPoint m_iDragPos;

	//wxArrayInOutput Outputs;
	//wxArrayInOutput Inputs;
	Initial::Core::IArray<InOutput*> Outputs;
	Initial::Core::IArray<InOutput*> Inputs;

	wxString Name;
	//wxString Categorie;
	bool Preview; 	

	bool Deletable;
	//Render
	ShapeDrawParameter DrawParam;
	ShapeRenderParameter RenderParam;

	wxPoint DragStart;

	bool m_bSelected;

	wxImage PreviewImage;
};

#endif