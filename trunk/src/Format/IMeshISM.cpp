//---------------------------------------------
//                Initial Engine
//               ISM Format Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/ILogger.h"
#include "Initial/Format/IMeshISM.h"
#include "Initial/IO/IFileStream.h"

using namespace Initial::IO;

/*
char[3] ISM
char    Version
short   NameSize
char*   Name
long	MeshCount
long	MaterialCount

{MaterialCount
	short  MaterialNameSize
  char* MaterialName
}

{MeshCount
	long	MaterialId	
	long	VerticeCount
	long	NormalsCount
	long	TextureCoordCount
	long	IndiceCount

	{VerticeCount
		float[3] Pos x,y,z
	}

	{NormalsCount
		float[3] Pos x,y,z
	}

	{TextureCoordCount
		float[2] Pos u,v
	}

	{IndiceCount
		long[3] VerticeIndex
		long[3] NormalIndex
		long[3] TextureIndex
	}
}

*/

using namespace Initial::Core;

namespace Initial
{
	namespace Format
	{
		IMeshISM::IMeshISM()
			: IRessource("")
		{
		}

		IMeshISM::~IMeshISM()
		{
			for (UINT i=0;i<m_aMeshs.Count();i++)
			{
				if (m_aMeshs[i])
				{
					delete m_aMeshs[i];
				}
			}
		}

		void IMeshISM::_ClearMeshs()
		{
			for (UINT i=0;i<m_aMeshs.Count();i++)
			{
				if (m_aMeshs[i])
				{
					delete m_aMeshs[i];
				}
			}
			m_aMeshs.Clear();
		}

		bool IMeshISM::Load(IString filename)
		{
			char	Header[3];
			char	Version;
			short	NameSize;
			IString	Name;
			long	MeshCount;
			long	MaterialCount;
				short	MaterialNameSize;
				IString	MaterialName;

			IFileStream file(filename,IFileStream::IOM_READ);
			if (file.IsOk())
			{
				file.Read(Header,1,3);
				if (!(Header[0]=='I' && Header[1]=='S' && Header[2]=='M'))
				{
					file.Close();
					return false;
				}
				file.Read(&Version,1,1);
				if (Version!=1)
				{
					file.Close();
					return false;
				}
				ILogger::LogMessage("ISM OK\n");

				SetFilename(filename);

				file.Read(&NameSize,2,1);
				Name.AllocBuffer(NameSize);
				file.Read(Name.c_str(),NameSize,1);
				Name[(int)NameSize]='\0';
				file.Read(&MeshCount,4,1);
				file.Read(&MaterialCount,4,1);

				m_aMaterials.Clear();
				for (int i=0;i<MaterialCount;i++)
				{
					MaterialName="";
					file.Read(&MaterialNameSize,2,1);
					MaterialName.AllocBuffer(MaterialNameSize);
					file.Read(MaterialName.c_str(),MaterialNameSize,1);
					MaterialName[(int)MaterialNameSize]='\0';
					m_aMaterials.Add(MaterialName);
				}

				_ClearMeshs();
				for (int i=0;i<MeshCount;i++)
				{
					Mesh* mesh = new Mesh;
					file.Read(&mesh->iMaterialId,4,1);
					file.Read(&mesh->iVerticeCount,4,1);
					file.Read(&mesh->iNormalsCount,4,1);
					file.Read(&mesh->iTexCoordCount,4,1);
					file.Read(&mesh->iIndexCount,4,1);
					mesh->pVertices = (float(*)[3])malloc(sizeof(float)*3*mesh->iVerticeCount);
					mesh->pNormals = (float(*)[3])malloc(sizeof(float)*3*mesh->iNormalsCount);
					mesh->pTexCoords = (float(*)[2])malloc(sizeof(float)*2*mesh->iTexCoordCount);
					mesh->pIndex = (MeshIndex *)malloc(sizeof(MeshIndex)*mesh->iIndexCount);
					file.Read(mesh->pVertices,sizeof(float)*3,mesh->iVerticeCount);
					file.Read(mesh->pNormals,sizeof(float)*3,mesh->iNormalsCount);
					file.Read(mesh->pTexCoords,sizeof(float)*2,mesh->iTexCoordCount);
					file.Read(mesh->pIndex,sizeof(MeshIndex),mesh->iIndexCount);
				}
				return true;
			}
			return false;
		}

		bool IMeshISM::Save(IString filename)
		{
			return false;
		}
	}
}
