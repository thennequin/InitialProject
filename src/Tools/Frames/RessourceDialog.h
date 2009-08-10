
#ifndef _RESSOURCEDIALOG_HEADER_
#define _RESSOURCEDIALOG_HEADER_

#include "wx/wx.h"
#include "wx/dialog.h"

#include "RessourcePanel.h"

class RessourceDialog : public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	RessourceDialog(wxWindow *parent=NULL);
	//virtual int ShowModal();

	RessourcePanel *GetRessourcePanel();

	virtual void OnSelected(wxCommandEvent& event);

	wxArrayString& GetSelected();

protected:
	RessourcePanel *m_pPanel;
	wxArrayString m_aSelected;
};

#endif