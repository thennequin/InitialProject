
#ifndef _PUPPETEERAPP_HEADER_
#define _PUPPETEERAPP_HEADER_

#include <wx/wx.h>
#include "MainWindow.h"

class PuppeteerApp : public wxApp
{
public:
    virtual bool OnInit(void);

	MainWindow *GetWindow() { return m_pWindow; }
protected:
	MainWindow *m_pWindow;
};

DECLARE_APP(PuppeteerApp)

#endif