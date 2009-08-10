
#ifndef _SHAPERENDERPARAMETER_HEADER_
#define _SHAPERENDERPARAMETER_HEADER_

#include "wx/wx.h"

struct ShapeRenderParameter
{
	wxFont FontTitle;
	wxFont FontInOut;
	wxBrush BrushTitleText;
	wxBrush BrushTitleBackground;
	wxBrush BrushBodyText;
	wxBrush BrushBodyBackground;
	wxBrush BrushInOutBlock;	
	wxBrush BrushInOutBlockOver;	
	wxPen PenSelected;
	wxPen PenNotSelected;

	int Width;
	int Height;

	int TitleWidth;
	int PreviewX;
	int PreviewY;

	int InputWidth;
	int OutputWidth;
};

#endif