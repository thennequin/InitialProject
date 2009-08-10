//---------------------------------------------
//                Initial Engine
//               IEM Format Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMESHIEM_HEADER_
#define _IMESHIEM_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"
#include "Initial/IRessource.h"

#include "cal3d/cal3d.h"

namespace Initial
{
	namespace Format
	{
		class IMeshIEM : public IRessource
		{
		public:
			IMeshIEM();
			~IMeshIEM();

			virtual void Reload();

			virtual bool Load(Core::IString filename);
			virtual bool Save(Core::IString filename);

			int GetMaterialCount();
			Core::IString GetMaterial(int id);
			
			Core::IString GetError();

			bool	SetSkeleton(Core::IString filename);
			int		AddMesh(Core::IString filename);
			bool	DeleteMesh(int i);
			bool	SetMaterial(int id, Core::IString filename);

			void	SetName(Core::IString name);
		protected:
			int		GetMaxMaterial();
		protected:
			Core::IString m_sName;
			Core::IArray<Core::IString> m_aMaterials;

			CalCoreModel* m_calCoreModel;
		};
	}
}

#endif
