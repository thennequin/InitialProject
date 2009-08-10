
#ifndef _INOUTPUT_HEADER_
#define _INOUTPUT_HEADER_

#include "wx/wx.h"
#include "Connection.h"

class InOutput
{
	friend class GraphShape;
	friend class Connection;
public:
	InOutput(wxString name,bool output,int  format=0,wxString strreturn="",bool Passable=false);
	wxString GetName() { return Name; }
	wxString GetOutputReturn() { return OutputReturn; }
	void SetOutputReturn(wxString str) { OutputReturn = str; }
	bool IsPassable() { return Passable; }
	Connection *GetConnection() { return Connect; }

//private:
	wxString Name;
	bool Output;
	//InputType Type;
	int Format;
	//wxArrayConnection Connect;
	Connection *Connect;

	//Render Parameter
	int TextWidth;

	//SGC : Shader Generator Code
	wxString OutputReturn;

	bool Passable;
};

#endif