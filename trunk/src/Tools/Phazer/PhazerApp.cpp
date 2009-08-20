
#include "PhazerApp.h"

IMPLEMENT_APP_CONSOLE(PhazerApp)

bool PhazerApp::OnInit(void)
{
	wxFileName file(argv[0]);
	chdir(file.GetPath());
	m_pWindow = new MainWindowPhazer("Phazer");
	if (argc==2)
		m_pWindow->LoadMaterial(argv[1]);
	m_pWindow->Show();


	return true;
}
