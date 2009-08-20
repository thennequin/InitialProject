
#ifndef _MAINWINDOWINITIALISED_HEADER_
#define _MAINWINDOWINITIALISED_HEADER_

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/treectrl.h>

#include "InitialPanel.h"
#include "ObjectProperty.h"
#include "NodeTree.h"

class MainWindowInitialisEd : public wxFrame
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

		//Window menu
		MW_MENU_WINDOW_TREE,
		MW_MENU_WINDOW_PROPERTY,
		
		//Toolbar
		MW_TB_SELECT,
		MW_TB_MOVE,
		MW_TB_ROTATE,
		MW_TB_SCALE,
		MW_TB_TRANS_MODE,
		MW_TB_MAGNET,
	};
public:
	MainWindowInitialisEd(const wxString& title, const wxPoint& Pos=wxDefaultPosition, const wxSize& size=wxSize(800,600));
	~MainWindowInitialisEd();	

	void OnOpenFile(wxCommandEvent& event);

	void OnMenu(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnTool(wxCommandEvent& event);
	void OnTrandModeChoice(wxCommandEvent& event);
protected:
	wxAuiManager m_mgr;

	wxMenuBar* m_pMenuBar;
		wxMenu *m_pFileMenu;
		wxMenu *m_pRenderMenu;

	wxAuiToolBar *m_pToolbar;
		wxChoice *m_pToolbarMode;

	wxTreeCtrl* m_pTree;

	InitialPanel *m_pInitial;
	ObjectProperty *m_pProp;

	wxStatusBar *m_pStatusBar;
	NodeTree *m_pNodeTree;
};

#endif