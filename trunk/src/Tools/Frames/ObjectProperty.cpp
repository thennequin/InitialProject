
#include "ObjectProperty.h"
#include "PropType.h"

BEGIN_EVENT_TABLE(ObjectProperty,wxPropertyGrid)
	EVT_PG_CHANGED( wxID_ANY, ObjectProperty::OnPropertyChange )
	//EVT_PG_CHANGED( wxID_ANY, ObjectProperty::OnPropertyGridChange )	
	/*EVT_MENU(wxID_ANY, MainWindow::OnToolLeftClick)
	EVT_PG_HIGHLIGHTED( wxID_ANY, MainWindow::OnPropertyGridHighlight )*/
END_EVENT_TABLE()


ObjectProperty::ObjectProperty(wxWindow *parent, wxWindowID id,
			   const wxPoint& pos,
			   const wxSize& size,
			   long style,
			   const wxChar* name)
 : wxPropertyGrid(parent,id,pos,size,style,name)
{
	m_pObject=NULL;
}

void ObjectProperty::GenerateFromObject(Initial::IObject *object)
{
	wxString str;
	Delete("Object Property");
	m_pObject=object;
	if (m_pObject)
	{
		AppendCategory("Object Property");
		IPropertyList& props = m_pObject->GetPropertyList();
		for (int i=0;i<props.Count();i++)
		{
			if (props.GetProperty(i))
			{
				switch (props.GetProperty(i)->GetType())
				{
				case IPT_BOOL:
					Append(new wxBoolProperty(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						*(bool*)props.GetProperty(i)->GetVar()))->SetAttribute(wxPG_BOOL_USE_CHECKBOX,true);
					break;
				case IPT_INTEGER:
					Append(new wxIntProperty(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						*(int*)props.GetProperty(i)->GetVar()));
					break;
				case IPT_FLOAT:
					Append(new FloatProperty(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						*(float*)props.GetProperty(i)->GetVar()));
					break;
				case IPT_VEC2:
					Append(new Vec2Property(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						*(IVector2D*)props.GetProperty(i)->GetVar()));
					break;
				case IPT_VEC3:
					Append(new Vec3Property(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						*(IVector3D*)props.GetProperty(i)->GetVar()));
					break;
				case IPT_COLOR:
					Append(new ColorProperty(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						*(IColor*)props.GetProperty(i)->GetVar()));
					break;
				case IPT_STRING:
					Append(new wxStringProperty(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetString().c_str()));
					break;
				case IPT_LONG_STRING:
					Append(new wxLongStringProperty(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetString().c_str()))->SetFlagRecursively(wxPG_PROP_NO_ESCAPE,true);
					break;
				case IPT_CHOICE:
					{
						wxArrayString choices;
						for (Core::IList<Core::IString>::Iterator ite=props.GetProperty(i)->GetChoices().Ite();ite;ite++)					
							choices.Add(ite.GetData().c_str());
						Append(new wxEnumProperty(props.GetProperty(i)->GetName().c_str(),
							props.GetProperty(i)->GetName().c_str(),
							choices,wxArrayInt(),props.GetProperty(i)->GetFloat()));
					}
					break;
				case IPT_RESSOURCE_ITX:
					Append(new ITXProperty(props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetName().c_str(),
						props.GetProperty(i)->GetString().c_str()));
					break;
				}
			}
		}
	}
	Refresh();
}

void ObjectProperty::OnPropertyChange( wxPropertyGridEvent& event )
{
	ILogger::LogDebug("Changed %d %d %s\n",m_pObject,event.GetProperty(),event.GetProperty()->GetValueAsString().c_str());
	if (m_pObject && event.GetProperty())
	{
		if (event.GetProperty()->IsKindOf(&wxEnumProperty::ms_classInfo))
		{
			//We return integer value for Enum property
			wxString val;
			val.Printf("%d",event.GetProperty()->GetValue().GetInteger());
			m_pObject->SetPropertyValue(event.GetProperty()->GetName().c_str(),val.c_str());
		}else
			m_pObject->SetPropertyValue(event.GetProperty()->GetName().c_str(),event.GetProperty()->GetValueAsString().c_str());
	}
}
