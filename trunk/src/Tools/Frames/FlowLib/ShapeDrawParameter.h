
#ifndef _SHAPEDRAWPARAMETER_HEADER_
#define _SHAPEDRAWPARAMETER_HEADER_

#include "wx/wx.h"

struct ShapeDrawParameter
{
	int MinWidth;
	int MinHeight;
	int PreviewSize; // 96
	int PreviewSpaceBorder; // 5

	int InOutFormatPreviewSize;
	int InOutFontHeight; // 16
	int InOutWidthSpace; // 10 : if no preview, space between input and output text
	int InOutHeightSpace; // 3
	int InOutSpaceBorder; // 2
	int InOutBlockSize; // 5
	wxColor InOutBlockColor;
	wxColor InOutBlockColorOver;

	int TitleFontHeight; // 20
	int TitleSpaceBorder; // 2
	int TitleBodySpace; // 1
	wxColor TitleTextColor; // 0,0,0
	wxColor TitleBackColor; // 133,133,133

	wxColor BodyBackColor; // 159,159,159
	wxColor BodyTextColor; // 0,0,0

	//Border
	wxColor ShapeSelect; // 255,255,0
	wxColor ShapeNotSelect; // 0,0,0
	ShapeDrawParameter();
};

#endif
