
#include "PropType.h"
#include "Initial/Core/IUtil.h"

#include <wx/colordlg.h>
#include "RessourceDialog.h"

//using namespace Initial;
//using namespace Initial::Core;

WX_PG_IMPLEMENT_VARIANT_DATA(IVector2DVariantData, IVector2D)
WX_PG_IMPLEMENT_VARIANT_DATA(IVector3DVariantData, IVector3D)
WX_PG_IMPLEMENT_VARIANT_DATA(IColorVariantData, IColor)
WX_PG_IMPLEMENT_VARIANT_DATA(IStringVariantData, IString)

WX_PG_IMPLEMENT_PROPERTY_CLASS(FloatProperty,wxFloatProperty,float,const float&,TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(Vec2Property,wxPGProperty, IVector2D, const IVector2D&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(Vec3Property,wxPGProperty, IVector3D, const IVector3D&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(ColorProperty,wxPGProperty, IColor, const IColor&, TextCtrlAndButton)
WX_PG_IMPLEMENT_PROPERTY_CLASS(ITXProperty,wxStringProperty, IString, const IString&, TextCtrlAndButton)

//--------------------------------------------------------------
// FloatProperty
//--------------------------------------------------------------

FloatProperty::FloatProperty( const wxString& label,
								 const wxString& name,
								 const float& value )
								 : wxFloatProperty(label,name,value)
{
}

wxString FloatProperty::GetValueAsString( int argFlags ) const
{
	return wxFloatProperty::GetValueAsString(argFlags);
}

bool FloatProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	variant=Iatof(text.c_str());
	return true;
}

//--------------------------------------------------------------
// Vec2Property
//--------------------------------------------------------------

Vec2Property::Vec2Property( const wxString& label, const wxString& name, const IVector2D& value )
: wxPGProperty(label,name)
{
	IVector2D temp = value;
	SetValue( IVector2DToVariant(value) );
	AddChild( new FloatProperty(wxT("X"),wxPG_LABEL,temp.x) );
	AddChild( new FloatProperty(wxT("Y"),wxPG_LABEL,temp.y) );
}

void Vec2Property::RefreshChildren()
{
	if ( !GetCount() ) return;
	IVector2D& vector = IVector2DFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
}

void Vec2Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	IVector2D& vector = IVector2DFromVariant(thisValue);
	switch ( childIndex )
	{
	case 0: vector.SetU(childValue.GetDouble()); break;
	case 1: vector.SetV(childValue.GetDouble()); break;
	}
}

//--------------------------------------------------------------
// Vec3Property
//--------------------------------------------------------------

Vec3Property::Vec3Property( const wxString& label, const wxString& name, const IVector3D& value )
: wxPGProperty(label,name)
{
	IVector3D temp = value;
	SetValue( IVector3DToVariant(value) );
	AddChild( new FloatProperty(wxT("X"),wxPG_LABEL,temp.x) );
	AddChild( new FloatProperty(wxT("Y"),wxPG_LABEL,temp.y) );
	AddChild( new FloatProperty(wxT("Z"),wxPG_LABEL,temp.z) );
}

void Vec3Property::RefreshChildren()
{
	if ( !GetCount() ) return;
	IVector3D& vector = IVector3DFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
	Item(2)->SetValue( vector.z );
}

void Vec3Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	IVector3D& vector = IVector3DFromVariant(thisValue);
	switch ( childIndex )
	{
	case 0: vector.SetX(childValue.GetDouble()); break;
	case 1: vector.SetY(childValue.GetDouble()); break;
	case 2: vector.SetZ(childValue.GetDouble()); break;
	}
}

//--------------------------------------------------------------
// ColorProperty
//--------------------------------------------------------------
//wxSystemColourProperty

ColorProperty::ColorProperty( const wxString& label, const wxString& name, const IColor& value )
: wxPGProperty(label,name)
{
	IColor temp = value;
	SetValue( IColorToVariant(value) );
	AddChild( new FloatProperty(wxT("R"),wxPG_LABEL,temp.rgba[0]) );
	AddChild( new FloatProperty(wxT("G"),wxPG_LABEL,temp.rgba[1]) );
	AddChild( new FloatProperty(wxT("B"),wxPG_LABEL,temp.rgba[2]) );
	AddChild( new FloatProperty(wxT("A"),wxPG_LABEL,temp.rgba[3]) );
}

void ColorProperty::RefreshChildren()
{
	if ( !GetCount() ) return;
	IColor& vector = IColorFromVariant(m_value);
	Item(0)->SetValue( vector.rgba[0] );
	Item(1)->SetValue( vector.rgba[1] );
	Item(2)->SetValue( vector.rgba[2] );
	Item(3)->SetValue( vector.rgba[3] );
}

void ColorProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	IColor& vector = IColorFromVariant(thisValue);
	switch ( childIndex )
	{
	case 0: vector.SetRed(childValue.GetDouble()); break;
	case 1: vector.SetGreen(childValue.GetDouble()); break;
	case 2: vector.SetBlue(childValue.GetDouble()); break;
	case 3: vector.SetAlpha(childValue.GetDouble()); break;
	}
}

bool ColorProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
	{
		IColor temp = IColorFromVariant(GetValue());
		wxColourData data;
		data.SetChooseFull(true);
		data.SetColour(wxColour(temp.GetRed()*255,temp.GetGreen()*255,temp.GetBlue()*255));
		wxColourDialog dialog(propgrid,&data);
		if (dialog.ShowModal())
		{
			wxColour color = dialog.GetColourData().GetColour();
			SetValueInEvent(IColorToVariant(IColor(color.Red()/255.0,color.Green()/255.0,color.Blue()/255.0,temp.GetAlpha())));
		}
		return true;
	}
	return false;
}

//--------------------------------------------------------------
// ITXProperty
//--------------------------------------------------------------
ITXProperty::ITXProperty( const wxString& label,const wxString& name,const IString& value )
: wxStringProperty(label,name,value.c_str())
{
	SetValue(IStringToVariant(value));
}

wxString ITXProperty::GetValueAsString( int argFlags ) const
{
	return wxString(IStringFromVariant(GetValue()).c_str());
}

bool ITXProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	variant = IStringToVariant(IString(text.c_str()));
	return true;
}


bool ITXProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
	{
		RessourceDialog dialog;
		dialog.GetRessourcePanel()->SetRessourceType(RessourceItem::RT_Image);
		dialog.GetRessourcePanel()->SetDirectory("Textures/");
		if (dialog.ShowModal())
		{
			wxArrayString& list = dialog.GetSelected();
			if (list.Count())
				SetValueInEvent(IStringToVariant(IString(list[0].c_str())));
			RefreshEditor();
			return true;
		}
	}
	return false;
	//return wxStringProperty::OnEvent(propgrid, primary, event);
}
