
#include "PhazerApp.h"

IMPLEMENT_APP_CONSOLE(PhazerApp)

bool PhazerApp::OnInit(void)
{
	m_pWindow = new MainWindowPhazer("Phazer");
	m_pWindow->Show();

	return true;
}
