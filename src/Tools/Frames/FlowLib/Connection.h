
#ifndef _CONECTION_HEADER_
#define _CONECTION_HEADER_

#include "wx/wx.h"

class Connection;
class GraphShape;
class InOutput;

WX_DEFINE_ARRAY_PTR(Connection*, wxArrayConnection);

class Connection
{
public:
	Connection();
	~Connection();

	void DrawConnection(wxDC& dc);
	static void DrawConnection(wxDC& dc, wxPoint Start, wxPoint End);

	GraphShape *InputShape;
	GraphShape *OutputShape;

	int InputId;
	InOutput *Input;
	int OutputId;
	InOutput *Output;

	//static wxArrayConnection ConnectionsList;
};

#endif