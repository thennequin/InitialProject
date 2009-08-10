//---------------------------------------------
//                Initial Engine
//               ISM Format Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMESHISM_HEADER_
#define _IMESHISM_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"
#include "Initial/IRessource.h"

namespace Initial
{
	namespace Format
	{
		class IMeshISM : public IRessource
		{
		public:
			#pragma pack(push, 1)
			struct MeshIndex
			{
				unsigned long VerticeIndex[3];
				unsigned long NormalIndex[3];
				unsigned long TextureIndex[3];
			};
			#pragma pack(pop)

			struct Mesh
			{
				Mesh();
				~Mesh();
				unsigned long iMaterialId;
				unsigned long iVerticeCount;
				unsigned long iNormalsCount;
				unsigned long iTexCoordCount;
				unsigned long iIndexCount;

				float (*pVertices)[3];
				float (*pNormals)[3];
				float (*pTexCoords)[2];

				MeshIndex* pIndex;
			};
		public:
			IMeshISM();
			~IMeshISM();

			virtual void Reload();

			virtual bool Load(Core::IString filename);
			virtual bool Save(Core::IString filename);

			int GetMaterialCount();
			Core::IString GetMaterial(int id);

			void SetName(Core::IString name);

			long MeshCount();
			Mesh *GetMesh(long id);

			void _AddMesh(Mesh *mesh);
			void _DeleteMesh(long id);
		protected:
			void _ClearMeshs();
		protected:
			Core::IString m_sName;
			Core::IArray<Core::IString> m_aMaterials;
			Core::IArray<Mesh*> m_aMeshs;
		};
	}
}

#endif
