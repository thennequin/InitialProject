
#ifndef _PHAZERAPP_HEADER_
#define _PHAZERAPP_HEADER_

#include <wx/wx.h>
#include "MainWindowPhazer.h"

class PhazerApp : public wxApp
{
public:
	virtual bool OnInit(void);

	MainWindowPhazer *GetWindow() { return m_pWindow; }
protected:
	MainWindowPhazer *m_pWindow;
};

DECLARE_APP(PhazerApp)

#endif