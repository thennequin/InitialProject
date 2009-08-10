
#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>

#include "Initial/Core/IVector2D.h"
#include "Initial/Core/IVector3D.h"
#include "Initial/Core/IColor.h"
#include "Initial/Core/IString.h"

using namespace Initial;
using namespace Initial::Core;

//typedef Vec2 Initial::Core::IVector2D;

WX_PG_DECLARE_VARIANT_DATA(IVector2DVariantData,IVector2D, wxPG_NO_DECL)
WX_PG_DECLARE_VARIANT_DATA(IVector3DVariantData, IVector3D, wxPG_NO_DECL)
WX_PG_DECLARE_VARIANT_DATA(IColorVariantData, IColor, wxPG_NO_DECL)
WX_PG_DECLARE_VARIANT_DATA(IStringVariantData, IString, wxPG_NO_DECL)

class FloatProperty : public wxFloatProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(MyFloatProperty)
public:

	FloatProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const float& value = 0.0 );

	WX_PG_DECLARE_BASIC_TYPE_METHODS()
protected:
};

class Vec2Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(Vec2Property)
public:

	Vec2Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const IVector2D& value = IVector2D() );

	WX_PG_DECLARE_PARENTAL_METHODS()
};

class Vec3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(Vec3Property)
public:

	Vec3Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const IVector3D& value = IVector3D() );

	WX_PG_DECLARE_PARENTAL_METHODS()
};

class ColorProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(ColorProperty)
public:

	ColorProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const IColor& value = IColor() );

	WX_PG_DECLARE_PARENTAL_METHODS()
	WX_PG_DECLARE_EVENT_METHODS()
};

class ITXProperty : public wxStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(ColorProperty)
public:

	ITXProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const IString& value = IString() );

	//WX_PG_DECLARE_PARENTAL_METHODS()
	WX_PG_DECLARE_BASIC_TYPE_METHODS()
	WX_PG_DECLARE_EVENT_METHODS()
};
