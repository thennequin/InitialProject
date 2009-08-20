
#ifndef _PUPPETEERAPP_HEADER_
#define _PUPPETEERAPP_HEADER_

#include <wx/wx.h>
#include "MainWindowPuppeteer.h"

class PuppeteerApp : public wxApp
{
public:
    virtual bool OnInit(void);

	MainWindowPuppeteer *GetWindow() { return m_pWindow; }
protected:
	MainWindowPuppeteer *m_pWindow;
};

DECLARE_APP(PuppeteerApp)

#endif