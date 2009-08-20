
#include "PuppeteerApp.h"

IMPLEMENT_APP_CONSOLE(PuppeteerApp)

bool PuppeteerApp::OnInit(void)
{
	m_pWindow = new MainWindowPuppeteer("Puppeteer");
	m_pWindow->Show();

	return true;
}
