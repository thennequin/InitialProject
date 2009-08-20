
#ifndef _MAINWINDOWPUPPETEER_HEADER_
#define _MAINWINDOWPUPPETEER_HEADER_

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/treectrl.h>

#include "InitialPanel.h"

#include "ObjectProperty.h"

class MainWindowPuppeteer : public wxFrame
{
	DECLARE_EVENT_TABLE()

	enum
	{
		//File menu
		MW_MENU_OPEN_FILE = wxID_HIGHEST+1,
		MW_MENU_SAVE_FILE,

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
	MainWindowPuppeteer(const wxString& title, const wxPoint& Pos=wxDefaultPosition, const wxSize& size=wxSize(800,600));
	~MainWindowPuppeteer();	

	void OnOpenFile(wxCommandEvent& event);

	void OnMenu(wxCommandEvent& event);
protected:
	void ClearTree();
protected:
	wxAuiManager m_mgr;

	wxMenuBar* m_pMenuBar;
		wxMenu *m_pFileMenu;
		wxMenu *m_pRenderMenu;

	wxTreeCtrl* m_pTree;

	InitialPanel *m_pInitial;
	ObjectProperty *m_pProp;

	wxStatusBar *m_pStatusBar;
};

#endif