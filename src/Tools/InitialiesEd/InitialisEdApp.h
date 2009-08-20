
#ifndef _INITIALISEDAPP_HEADER_
#define _INITIALISEDAPP_HEADER_

#include <wx/wx.h>
#include "MainWindowInitialisEd.h"

class InitialisEdApp : public wxApp
{
public:
    virtual bool OnInit(void);

	MainWindowInitialisEd *GetWindow() { return m_pWindow; }
protected:
	MainWindowInitialisEd *m_pWindow;
};

DECLARE_APP(InitialisEdApp)

#endif