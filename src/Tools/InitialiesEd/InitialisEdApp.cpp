
#include "InitialisEdApp.h"

IMPLEMENT_APP_CONSOLE(InitialisEdApp)

bool InitialisEdApp::OnInit(void)
{
	m_pWindow = new MainWindowInitialisEd("InitialisEd");
	m_pWindow->Show();

	return true;
}
