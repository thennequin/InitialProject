
//#include "MainWindow.h"
#include "MaterialGraph.h"
/*#include "EditorShape.h"
#include "ComponentsShape.h"
#include "ShaderGenerator.h"
#include "Texture.h"*/

//#include "irrXML.h"
//#include "Util.h"

//using namespace irr;
//using namespace io;

BEGIN_EVENT_TABLE(MaterialGraph, FlowGraph)
EVT_MENU(wxID_ANY,MaterialGraph::OnMenu)
END_EVENT_TABLE()

MaterialGraph::MaterialGraph(wxWindow* parent, const wxPoint& pos, const wxSize& size)
: FlowGraph (parent,pos,size)
{
	ContextMenu=NULL;
	m_pProperty=NULL;
	m_pMaterial=NULL;

	//CreateMenu();

	/*m_pExprFinal = new Initial::Video::IMaterialExpressionFinal();
	m_pExprFinal->SetPos (50,50);
	AddShape(m_pExprFinal);*/

	ShapeSelected(NULL);
}

MaterialGraph::~MaterialGraph()
{
	//Timer.Stop();
	//Clear();
}

void MaterialGraph::LinkMaterial(Initial::Video::IMaterial *material)
{
	m_pMaterial=material;
	Clear();
	if (m_pMaterial)
	{
		FOREACH(Initial::Video::IMaterialExpression*,ite,m_pMaterial->GetExpressions())
		{
			AddShape(ite.GetData());
			printf("%s\n",ite.GetData()->GetClass()->GetName().c_str());
		}

		//m_pMaterial->GetExpressions().First()->GetData();
		//StartDrag()
		//Center shapes
	}
	ShapeSelected(NULL);
}

Initial::Video::IMaterial* MaterialGraph::GetLinkedMaterial()
{
	return m_pMaterial;
}

void MaterialGraph::LinkProperty(ObjectProperty *prop)
{
	m_pProperty=prop;
}

/*void MaterialGraph::SetPropertyGrid(wxPropertyGrid* grid)
{
	m_pPropGrid=grid;
}*/

void MaterialGraph::RightClick(wxPoint pos)
{
	//wxLogMessage("Right CLick") ;

	if (ContextMenu)
	{
		Position=pos;
		PopupMenu(ContextMenu,pos);
	}else
		CreateMenu();
}

void MaterialGraph::CreateMenu()
{
	ContextMenu = new wxMenu();
	if (ContextMenu)
	{
		wxArrayString Categories;

		int Count = Initial::IObject::ObjectsClass.Count();
		//wxLogMessage("Expression count %d",Count);
		//Generate sub menu
		for (int i=0;i<Count;i++)
		{
			//printf("%s\n",Initial::IObject::ObjectsClass[i]->GetName());
			if (Initial::IObject::ObjectsClass[i]->IsKindOf(Initial::Video::IMaterialExpression::GetStaticClass())
				&& Initial::IObject::ObjectsClass[i]!=Initial::Video::IMaterialExpressionFinal::GetStaticClass())
			{
				Initial::Video::IMaterialExpression *shape = (Initial::Video::IMaterialExpression*)Initial::IObject::ObjectsClass[i]->CreateInstance();

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
					if (Initial::IObject::ObjectsClass[j]->IsKindOf(Initial::Video::IMaterialExpression::GetStaticClass())
						&& Initial::IObject::ObjectsClass[j]!=Initial::Video::IMaterialExpressionFinal::GetStaticClass())
					{
						Initial::Video::IMaterialExpression *shape = (Initial::Video::IMaterialExpression*)Initial::IObject::ObjectsClass[j]->CreateInstance();

						if (shape)
						{
							if (shape->GetCategorie()==Categories[i].c_str())
							{
								sub->Append(6000+j,shape->GetName());
							}
							delete shape;
						}	
					}
				}
				ContextMenu->AppendSubMenu(sub,Categories[i]);
			}
		}

		for (int i=0;i<Count;i++)
		{
			if (Initial::IObject::ObjectsClass[i]->IsKindOf(Initial::Video::IMaterialExpression::GetStaticClass())
				&& Initial::IObject::ObjectsClass[i]!=Initial::Video::IMaterialExpressionFinal::GetStaticClass())
			{
				Initial::Video::IMaterialExpression *shape = (Initial::Video::IMaterialExpression*)Initial::IObject::ObjectsClass[i]->CreateInstance();

				if (shape)
				{
					if (shape->GetCategorie()=="")
						ContextMenu->Append(6000+i,shape->GetName());
					delete shape;
				}
			}
		}
	}
}

void MaterialGraph::OnMenu(wxCommandEvent& event)
{
	int Count = Initial::IObject::ObjectsClass.Count();
	if (event.GetId()>=6000 && event.GetId()<6000+Count)
	{
		Initial::Video::IMaterialExpression *shape;
		shape = (Initial::Video::IMaterialExpression*)Initial::IObject::ObjectsClass[event.GetId()-6000]->CreateInstance();
		if (shape)
		{
			int x = (Position.x-GetSize().x/2)*GetZoom()+GetSize().x/2;
			int y = (Position.y-GetSize().y/2)*GetZoom()+GetSize().y/2;
			shape->SetPos(x,y);
			shape->m_iId=m_pMaterial->NextId++;
			shape->m_pMaterial=m_pMaterial;
			m_pMaterial->m_aExpressions.PushBack(shape);
			AddShape(shape);
			ResetSelection();
			shape->SetSelected(true);
			ShapeSelected(shape);

			Refresh();
		}
	}
}

void MaterialGraph::ShapeSelected(GraphShape* shape)
{
	//wxLogMessage("Shape selected");

	Initial::Video::IMaterialExpression* comp = (Initial::Video::IMaterialExpression*)shape;

	if (m_pProperty)
	{
		if (comp)
			m_pProperty->GenerateFromObject(comp);
		else
			m_pProperty->GenerateFromObject(m_pMaterial);
	}
}

void MaterialGraph::ShapeConnected(GraphShape* shape1, GraphShape* shape2)
{
	//Initial::Video::IMaterialExpression* comp1 = (Initial::Video::IMaterialExpression*)shape1;
	//Initial::Video::IMaterialExpression* comp2 = (Initial::Video::IMaterialExpression*)shape2;

	if (m_pMaterial)
		m_pMaterial->CompileV2();
}

void MaterialGraph::OnDeleteShape(GraphShape* shape)
{
	if (m_pMaterial && shape)
	{
		//m_pMaterial->m_aExpressions.DeleteItem((Initial::Video::IMaterialExpression*)shape);
		for (Initial::Core::IList<Initial::Video::IMaterialExpression*>::Iterator ite = m_pMaterial->m_aExpressions.Ite();ite;ite++)
		{
			if (ite.GetData()==shape)
				m_pMaterial->m_aExpressions.DeleteNode(ite);
		}
		delete shape;

		m_pMaterial->CompileV2();
	}
}

void MaterialGraph::Clear()
{
	wxList *shapes = GetShapesList();
	if (shapes)
	{
		wxObjectList::compatibility_iterator node = shapes->GetFirst();
		while (node)
		{
			Initial::Video::IMaterialExpression *expr = (Initial::Video::IMaterialExpression*)node->GetData();
			//if (expr && expr->IsDeletable())
			{
				DeleteShape(expr,false);
			}	
			node = node->GetNext();
		}
	}
	Refresh();
}
