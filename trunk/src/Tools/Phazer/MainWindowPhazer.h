
#ifndef _MAINWINDOWPHAZER_HEADER_
#define _MAINWINDOWPHAZER_HEADER_

#include <wx/wx.h>
#include <wx/aui/aui.h>

#include "InitialPanel.h"
#include "MaterialGraph.h"
#include "ObjectProperty.h"

class MainWindowPhazer : public wxFrame
{
	DECLARE_EVENT_TABLE()

	enum
	{
		//File menu
		MW_MENU_NEW_FILE = wxID_HIGHEST+1,
		MW_MENU_OPEN_FILE,
		MW_MENU_SAVE_FILE,
		MW_MENU_SAVE_AS_FILE,

		//Render menu
		MW_MENU_RENDER_DEFAULT,
		MW_MENU_RENDER_ALBEDO,
		MW_MENU_RENDER_NORMALMAP,
		MW_MENU_RENDER_EMISSIVE,
		MW_MENU_RENDER_SPECULAR,
		MW_MENU_RENDER_DEPTH,
		MW_MENU_RENDER_BLOOM,

		MW_ADD_MESH,
		MW_DELETE_MESH,
		MW_SET_SKELETON,
	};
public:
	MainWindowPhazer(const wxString& title, const wxPoint& Pos=wxDefaultPosition, const wxSize& size=wxSize(800,600));
	~MainWindowPhazer();	

	void OnOpenFile(wxCommandEvent& event);

	void OnMenu(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);

	bool LoadMaterial(wxString file);
protected:
	wxAuiManager m_mgr;

	wxMenuBar* m_pMenuBar;
		wxMenu *m_pFileMenu;
		wxMenu *m_pRenderMenu;

	InitialPanel *m_pInitial;
		Initial::Video::IMaterial *m_pMaterial;
	MaterialGraph *m_pGraph;
	ObjectProperty *m_pProp;

	wxStatusBar *m_pStatusBar;

	wxString m_sUsedFile;
};

#endif