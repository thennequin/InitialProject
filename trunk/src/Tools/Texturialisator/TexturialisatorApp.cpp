
#include "TexturialisatorApp.h"

IMPLEMENT_APP_CONSOLE(TexturialisatorApp)

bool TexturialisatorApp::OnInit(void)
{
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
#if wxUSE_TGA
	wxImage::AddHandler(new wxTGAHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif

	m_pWindow = new MainWindow("Texturialisator");
	m_pWindow->Show();

	return true;
}
