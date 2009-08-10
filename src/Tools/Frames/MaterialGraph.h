
#ifndef _MATERIALGRAPH_HEADER_
#define _MATERIALGRAPH_HEADER_

#include "FlowLib/FlowGraph.h"
#include "Initial/Video/IMaterial.h"
#include "Initial/Video/IMaterialExpression.h"

#include "ObjectProperty.h"

class MaterialGraph : public FlowGraph
{
	DECLARE_EVENT_TABLE()
public:
	MaterialGraph(wxWindow* parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~MaterialGraph();

	void LinkMaterial(Initial::Video::IMaterial *material);
	Initial::Video::IMaterial *GetLinkedMaterial();
	void LinkProperty(ObjectProperty *prop);

	//void SetPropertyGrid(wxPropertyGrid* grid);
	//void PropertyChange(wxPGProperty* prop);

	virtual void RightClick(wxPoint pos);
	virtual void ShapeSelected(GraphShape* shape);
	virtual void ShapeConnected(GraphShape* shape1, GraphShape* shape2);
	void OnDeleteShape(GraphShape* shape);

	void OnMenu(wxCommandEvent& event);

	void CreateMenu();
	wxMenu *GetMenu() { return ContextMenu; }

	//void Clear();
	//wxString Read(wxString File); // Return Error, if returnString=="" then 'no error'
	//void Save(wxString File);

	//wxString GenerateShaderCode();
protected:
	void Clear();
protected:
	//wxPropertyGrid* m_pPropGrid;
	wxMenu *ContextMenu;
	wxPoint Position;

	ObjectProperty *m_pProperty;
	//ComponentShape *m_pShaderShape;
	Initial::Video::IMaterialExpression* m_pExprFinal;
	Initial::Video::IMaterial *m_pMaterial;
};


#endif