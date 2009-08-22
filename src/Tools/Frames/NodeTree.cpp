
#include "NodeTree.h"

BEGIN_EVENT_TABLE(NodeTree, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(wxID_ANY,NodeTree::OnSelect)
	EVT_TREE_SEL_CHANGING(wxID_ANY,NodeTree::OnSelect)
	EVT_TREE_BEGIN_DRAG(wxID_ANY, NodeTree::OnBeginDrag)
    EVT_TREE_END_DRAG(wxID_ANY, NodeTree::OnEndDrag)
	EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, NodeTree::OnRightClick)
	EVT_MENU(wxID_ANY,NodeTree::OnMenu)
	EVT_TREE_DELETE_ITEM(wxID_ANY,NodeTree::OnDelete)
	EVT_CHAR(NodeTree::OnChar)
	EVT_TREE_BEGIN_LABEL_EDIT(wxID_ANY,NodeTree::OnLabelEditBegin)
	EVT_TREE_END_LABEL_EDIT(wxID_ANY,NodeTree::OnLabelEditEnd)
END_EVENT_TABLE()

NodeTree::NodeTree(wxWindow *parent, wxWindowID id,
               const wxPoint& pos,
               const wxSize& size)
: wxTreeCtrl(parent,id,pos,size, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_EDIT_LABELS | wxTR_MULTIPLE), m_Images(16,16)
{
	m_pManager=NULL;
	m_pProperty=NULL;
	m_pContextMenu=NULL;
	m_draggedItem=NULL;

	SetBackgroundColour(wxColour(192,192,192));

	SetImageList(&m_Images);
	AddImage("NodeObject.bmp");
	int rootImage = AddImage("NodeRoot.bmp");
	m_tRoot = AddRoot("Root",rootImage);
}

int NodeTree::AddImage(wxString file)
{
	if (file!="")
	{
		for (unsigned int i=0;i<m_ImagesName.Count();i++)
		{
			if (m_ImagesName[i]==file)
				return i;
		}
	}else{
		return 0;
	}
	int id = m_Images.Add(wxBitmap("res/"+file,wxBITMAP_TYPE_BMP));
	m_ImagesName.Add(file);
	return id;
}

void NodeTree::LinkNodeManager(Initial::ISceneManager *manager)
{
	m_pManager=manager;
}

void NodeTree::LinkProperty(ObjectProperty *Property)
{
	m_pProperty=Property;
}

void NodeTree::RefreshList()
{
	DeleteChildren(m_tRoot);
	RefreshItem(m_tRoot);
	ExpandAll();
}

void NodeTree::RefreshItem(wxTreeItemId& item)
{
	TreeNode *itemData=(TreeNode*)GetItemData(item);
	Initial::INode *node = NULL;
	Initial::INode *subNode = NULL;
	if (itemData)
		node=itemData->m_pNode;
	if (node==NULL)
		node=m_pManager;
	
	if (node)
	{
		UINT count = node->GetNodeCount();
		for (UINT i=0;i<count;i++)
		{
			subNode = node->GetNode(i);
			if (subNode)
			{				
				wxTreeItemId newitem = InsertItem(item,item,
					subNode->GetName().c_str(),
					AddImage(subNode->GetBitmapName().c_str()),-1,
					new TreeNode(subNode));
				RefreshItem(newitem);
			}
		}
	}
}

void NodeTree::OnSelect(wxTreeEvent& event)
{
	printf("Select\n");
	if (m_pProperty)
	{
		wxArrayTreeItemIds selected;
		GetSelections(selected);
		if (selected.Count()==1)
		{
			Initial::INode *node=NULL;
			TreeNode *itemData=(TreeNode*)GetItemData(event.GetItem());
			if (itemData)
				node=itemData->m_pNode;
			m_pProperty->GenerateFromObject(node);
		}else
			m_pProperty->GenerateFromObject(NULL);

		m_pManager->ResetSelection();
		for (size_t i=0;i<selected.Count();i++)
		{
			TreeNode *itemData=(TreeNode*)GetItemData(selected[i]);
			if (itemData && itemData->m_pNode)
				itemData->m_pNode->Select(true);
		}
	}
}

void NodeTree::OnBeginDrag(wxTreeEvent& event)
{
	GetSelections(m_aItemSelected);

	TreeNode *itemData=(TreeNode*)GetItemData(event.GetItem());
	if (itemData)
	{
		event.Allow();
	}
}

void NodeTree::OnEndDrag(wxTreeEvent& event)
{
	Initial::INode *dstNode;
	TreeNode *itemData=(TreeNode*)GetItemData(event.GetItem());
	if (itemData)
		dstNode=itemData->m_pNode;
	else
		dstNode=m_pManager;

	for (size_t i=0;i<m_aItemSelected.Count();i++)
	{
		itemData=(TreeNode*)GetItemData(m_aItemSelected[i]);

		if (itemData && itemData->m_pNode)
		{
			itemData->m_pNode->SetParent(dstNode);
		}		
	}
	RefreshList();
}

void NodeTree::OnRightClick(wxTreeEvent& event)
{
	if (CreateMenu())
	{
		TreeNode *itemData=(TreeNode*)GetItemData(event.GetItem());
		m_draggedItem=itemData;
		PopupMenu(m_pContextMenu,event.GetPoint());
	}else{
		wxLogError("Error on menu creation");
	}
}

bool NodeTree::CreateMenu()
{
	if (m_pContextMenu)
	{
		return true;
	}else{
		m_pContextMenu = new wxMenu();
		if (m_pContextMenu)
		{
			wxMenu *newMenu = new wxMenu();
			m_pContextMenu->AppendSubMenu(newMenu,"New");
			m_pContextMenu->AppendSeparator();

			wxArrayString Categories;

			int Count = Initial::IObject::ObjectsClass.Count();
			//Generate sub menu
			for (int i=0;i<Count;i++)
			{
				if (Initial::IObject::ObjectsClass[i]->IsKindOf(Initial::INode::GetStaticClass()))
				{
					Initial::INode *shape = (Initial::INode*)Initial::IObject::ObjectsClass[i]->CreateInstance();

					if (shape)
					{
						if (shape->GetCategorie()!="")
						{
							bool Exist=false;
							for (UINT j=0;j<Categories.Count();j++)
							{
								if (shape->GetCategorie()==Categories[j].c_str())
								{
									Exist=true;
									break;
								}
							}
							if (Exist==false)
								Categories.Add(shape->GetCategorie().c_str());
						}
						delete shape;
					}
				}
			}

			Categories.Sort();

			for (UINT i=0;i<Categories.Count();i++)
			{
				wxMenu *sub = new wxMenu;
				if (sub)
				{
					for (int j=0;j<Count;j++)
					{
						if (Initial::IObject::ObjectsClass[j]->IsKindOf(Initial::INode::GetStaticClass()))
						{
							Initial::INode *shape = (Initial::INode*)Initial::IObject::ObjectsClass[j]->CreateInstance();

							if (shape)
							{
								if (shape->GetCategorie()==Categories[i].c_str())
								{
									sub->Append(6000+j,shape->GetName().c_str());
								}
								delete shape;
							}	
						}
					}
					newMenu->AppendSubMenu(sub,Categories[i]);
				}
			}

			for (int i=0;i<Count;i++)
			{
				if (Initial::IObject::ObjectsClass[i]->IsKindOf(Initial::INode::GetStaticClass()))
				{
					Initial::INode *shape = (Initial::INode*)Initial::IObject::ObjectsClass[i]->CreateInstance();

					if (shape)
					{
						if (shape->GetCategorie()=="")
							newMenu->Append(6000+i,shape->GetClass()->GetName().c_str());
						delete shape;
					}
				}
			}
			return true;
		}
	}
	return false;
}

void NodeTree::OnMenu(wxCommandEvent& event)
{
	int Count = Initial::IObject::ObjectsClass.Count();
	if (event.GetId()>=6000 && event.GetId()<6000+Count)
	{
		Initial::INode *node;
		node = (Initial::INode*)Initial::IObject::ObjectsClass[event.GetId()-6000]->CreateInstance();
		if (node)
		{
			Initial::INode *parent=NULL;
			if (m_draggedItem)
				parent = m_draggedItem->m_pNode;
			else
				parent = m_pManager;
			node->SetDevice(m_pManager->GetDevice());
			node->InitNode();
			node->SetParent(parent);			
			RefreshList();
		}
	}
}

void NodeTree::OnDelete(wxTreeEvent& event)
{
	//DeleteSelected();
}

void NodeTree::OnChar(wxKeyEvent& event)
{
    switch ( event.GetKeyCode() )
    {
    case WXK_DELETE:
        DeleteSelected();
        return;
	case WXK_F2:
		{
			GetSelections(m_aItemSelected);
			EditLabel(m_aItemSelected[0]);
		}
		return;
	}

    event.Skip();
}

void NodeTree::OnLabelEditBegin(wxTreeEvent& event)
{
	TreeNode *itemData=(TreeNode*)GetItemData(event.GetItem());
	if (!itemData)
	{
		event.Veto();
	}
}

void NodeTree::OnLabelEditEnd(wxTreeEvent& event)
{
	if (!event.IsEditCancelled())
	{
		for (size_t i=0;i<m_aItemSelected.Count();i++)
		{
			TreeNode *itemData=(TreeNode*)GetItemData(m_aItemSelected[i]);
			if (itemData && itemData->m_pNode)
			{
				SetItemText(itemData->GetId(),event.GetLabel());
				itemData->m_pNode->SetName(event.GetLabel().c_str());
				if (m_aItemSelected.Count()==1)
					m_pProperty->GenerateFromObject(itemData->m_pNode);
			}
		}
	}
}

void NodeTree::DeleteSelected()
{
	wxArrayTreeItemIds items;
	GetSelections(items);

	for (size_t i=0;i<items.Count();i++)
	{
		TreeNode *itemData=(TreeNode*)GetItemData(items[i]);
		if (itemData)
		{
			if (itemData->m_pNode)
			{
				itemData->m_pNode->SetParent(NULL);
				delete itemData->m_pNode;
			}
		}
	}
	RefreshList();
}