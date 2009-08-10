
#include "InOutput.h"

InOutput::InOutput(wxString name,bool output,int format,wxString strreturn,bool passable)
{
	Name=name;
	Output=output;
	Format=format;
	Connect=NULL;
	OutputReturn=strreturn;
	Passable=passable;
}