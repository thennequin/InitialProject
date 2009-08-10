
#ifndef _GRAPHSHAPESKY_HEADER_
#define _GRAPHSHAPESKY_HEADER_

#include "GraphShape.h"

class GraphShapeSky : public GraphShape
{
public:
	GraphShapeSky();

	virtual void CalculateRender();
	virtual void Draw(wxDC& dc);
	virtual void DrawInputHighlight(int id, wxDC& dc);
	virtual void DrawOutputHighlight(int id, wxDC& dc);
};

#endif