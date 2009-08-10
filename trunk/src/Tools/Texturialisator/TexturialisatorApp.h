
#ifndef _TEXTURIALISATORAPP_HEADER_
#define _TEXTURIALISATORAPP_HEADER_

#include <wx/wx.h>
#include "MainWindow.h"

class TexturialisatorApp : public wxApp
{
public:
    virtual bool OnInit(void);

	MainWindow *GetWindow() { return m_pWindow; }
protected:
	MainWindow *m_pWindow;
};

DECLARE_APP(TexturialisatorApp)

#endif