
#ifndef _NODETREE_HEADER_
#define _NODETREE_HEADER_

#include <wx/wx.h>
#include <wx/treectrl.h>

#include "ObjectProperty.h"
#include "Initial/ISceneManager.h"

class NodeTree : public wxTreeCtrl
{
	DECLARE_EVENT_TABLE()

	class TreeNode : public wxTreeItemData
	{
	public:
		TreeNode(Initial::INode *node)
		{
			m_pNode=node;
		}
	public:
		Initial::INode *m_pNode;
	};
public:
	NodeTree(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize);

	void LinkNodeManager(Initial::ISceneManager *manager);
	void LinkProperty(ObjectProperty *Property);

	void RefreshList();

	void OnSelect(wxTreeEvent& event);
	void OnBeginDrag(wxTreeEvent& event);
	void OnEndDrag(wxTreeEvent& event);
	void OnRightClick(wxTreeEvent& event);
	void OnMenu(wxCommandEvent& event);
	void OnDelete(wxTreeEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnLabelEditBegin(wxTreeEvent& event);
	void OnLabelEditEnd(wxTreeEvent& event);

	void DeleteSelected();
protected:
	void RefreshItem(wxTreeItemId& item);
	int AddImage(wxString file);
	bool CreateMenu();
protected:
	wxImageList m_Images;
	wxArrayString m_ImagesName;
	Initial::ISceneManager *m_pManager;
	ObjectProperty *m_pProperty;

	wxMenu *m_pContextMenu;

	wxTreeItemId m_tRoot;

	TreeNode    *m_draggedItem;       // the item being dragged
	wxArrayTreeItemIds m_aItemSelected;
};

#endif
