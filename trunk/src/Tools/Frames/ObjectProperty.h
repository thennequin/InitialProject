
#ifndef _OBJECTPROPERTY_HEADER_
#define _OBJECTPROPERTY_HEADER_

#include <wx/propgrid/propgrid.h>

#include "Initial/IObject.h"


class ObjectProperty : public wxPropertyGrid
{
	DECLARE_EVENT_TABLE()
public:
	ObjectProperty(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxPG_DEFAULT_STYLE,
		const wxChar* name = wxPropertyGridNameStr);

	void GenerateFromObject(Initial::IObject *object);

	void OnPropertyChange( wxPropertyGridEvent& event );
protected:
	Initial::IObject *m_pObject;
};

#endif
