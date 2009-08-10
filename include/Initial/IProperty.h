
#ifndef _IPROPERTY_HEADER_
#define _IPROPERTY_HEADER_

#include "Initial/Core/IArray.h"
#include "Initial/Core/IList.h"
#include "Initial/Core/IString.h"
#include "Initial/Core/IVector2D.h"
#include "Initial/Core/IVector3D.h"
#include "Initial/Core/IColor.h"
#include "Initial/IEnum.h"

namespace Initial
{
	class IProperty
	{
	public:
		
	public:
		IProperty(Core::IString name, PropertyType type, void *affected, bool editable=true, Core::IList<Core::IString>& choices=Core::IList<Core::IString>());
		~IProperty();

		//wxPGProperty *Generate(wxPropertyGrid *grid);

		//void *OnGridChange(wxPGProperty *prop);
		//void OnVarChange();

		PropertyType GetType();
		Core::IString GetName();
		Core::IString GetString();
		float GetFloat();
		Core::IColor GetColor();
		void *GetVar();
		Core::IList<Core::IString>& GetChoices();

		void SetValueFromString(Core::IString value);

	protected:
		bool m_bEditable;
		Core::IString m_sName;
		PropertyType m_iType;
		void *m_pVar;
		bool m_bInternalVar;
		Core::IList<Core::IString> m_aChoices;

		void *m_pUserData; //wxPGProperty *m_pProperty;
	};

	class IPropertyList
	{
	public:
		~IPropertyList();
		void AddProperty(	Core::IString& Name,
							PropertyType Type,
							void *var=NULL,
							Core::IList<Core::IString>& choices=Core::IList<Core::IString>());

		IProperty *GetProperty(Core::IString Name);
		int Count();
		IProperty *GetProperty(int i);

		//void GeneratePropertyGrid(wxPropertyGrid *grid);
	protected:
		Core::IArray<IProperty*> m_aProperties;
	};
}

#endif
