//---------------------------------------------
//                Initial Engine
//               IEM Format Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/ILogger.h"
#include "Initial/Format/IMeshIEM.h"
#include "Initial/IO/IFileStream.h"

using namespace Initial::IO;

/*
char[3] IEM
char    Version = 1
short    NameSize
char*   Name
short   MeshCount
short   MaterialCount

{MaterialCount
  short  MaterialNameSize
  char* MaterialName
}

long    SkeletalZipSize
char*   SkeletalZipData

{MeshCount
  short    MeshNameSize
  char*   MeshName
  long    MeshZipSize
  char*   MeshZipData
}
*/

using namespace Initial::Core;

namespace Initial
{
	namespace Format
	{
		IMeshIEM::IMeshIEM()
			: IRessource("")
		{
			m_calCoreModel = new CalCoreModel("dummy");
		}

		IMeshIEM::~IMeshIEM()
		{
			delete m_calCoreModel;
		}

		void IMeshIEM::Reload()
		{
			Load(m_sFileName);
		}

		bool IMeshIEM::Load(IString filename)
		{
			char	Header[3];
			char	Version;
			short	NameSize;
			IString	Name;
			short	MeshCount;
			short	MaterialCount;
				short	MaterialNameSize;
				IString	MaterialName;
			long    SkeletalZipSize;
			void*	SkeletalZipData;
				short	MeshNameSize;
				IString	MeshName;
				long	MeshZipSize;
				void*	MeshZipData;


			IFileStream file(filename,IFileStream::IOM_READ);
			if (file.IsOk())
			{
				file.Read(Header,1,3);
				if (!(Header[0]=='I' && Header[1]=='E' && Header[2]=='M'))
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
				ILogger::LogMessage("IEM OK\n");

				SetFilename(filename);

				file.Read(&NameSize,2,1);
				Name.AllocBuffer(NameSize);
				file.Read(Name.c_str(),NameSize,1);
				Name[(int)NameSize]='\0';
				file.Read(&MeshCount,2,1);
				file.Read(&MaterialCount,2,1);
				ILogger::LogMessage("Size %d\nName %s\n",NameSize,Name.c_str());
				ILogger::LogMessage("Mesh %d\nMaterial : %d\n",MeshCount,MaterialCount);
				m_sName=Name;

				m_aMaterials.Clear();
				for (int i=0;i<MaterialCount;i++)
				{
					MaterialName="";
					file.Read(&MaterialNameSize,2,1);
					MaterialName.AllocBuffer(MaterialNameSize);
					file.Read(MaterialName.c_str(),MaterialNameSize,1);
					MaterialName[(int)MaterialNameSize]='\0';
					m_aMaterials.Add(MaterialName);
					ILogger::LogMessage("%d %s\n",MaterialNameSize,MaterialName.c_str());
				}

				file.Read(&SkeletalZipSize,4,1);
				SkeletalZipData = malloc(SkeletalZipSize);
				file.Read(SkeletalZipData,SkeletalZipSize,1);
				CalCoreSkeletonPtr skeleton = CalLoader::loadCoreSkeleton(SkeletalZipData);
				m_calCoreModel->setCoreSkeleton(skeleton.get());
				free(SkeletalZipData);

				for (int i=0;i<MeshCount;i++)
				{
					file.Read(&MeshNameSize,2,1);
					MeshName.AllocBuffer(MeshNameSize);
					file.Read(MeshName.c_str(),MeshNameSize,1);
					MeshName[(int)MeshNameSize]='\0';
					file.Read(&MeshZipSize,4,1);
					MeshZipData = malloc(MeshZipSize);
					file.Read(MeshZipData,MeshZipSize,1);
					CalCoreMeshPtr mesh = CalLoader::loadCoreMesh(MeshZipData);
					mesh->setName(MeshName.c_str());
					m_calCoreModel->addCoreMesh(mesh.get());
					free(MeshZipData);
				}
				return true;
			}
			return false;
		}

		bool IMeshIEM::Save(IString filename)
		{
			IFileStream file(filename,IFileStream::IOM_WRITE);
			if (file.IsOk())
			{
				IString filenameTemp=filename+".temp_";

				char Version=1;
				short NameSize;
				short MeshCount;
				short MaterialCount;
				file.Write("IEM",1,3);
				file.Write(&Version,1,1);
				NameSize=m_sName.Length();
				file.Write(&NameSize,2,1);
				file.Write(m_sName.c_str(),NameSize,1);
				MeshCount = m_calCoreModel->getCoreMeshCount();
				printf("MeshCount %d\n",MeshCount);
				file.Write(&MeshCount,2,1);
				MaterialCount = m_calCoreModel->getCoreMaterialCount();
				MaterialCount=GetMaxMaterial();
				file.Write(&MaterialCount,2,1);

				for (int i=0;i<MaterialCount;i++)
				{
					IString Material;
					if (i<m_aMaterials.Count() && m_aMaterials.Count()!=0)
						Material=m_aMaterials[i];
					short MaterialNameSize=Material.Length();
					file.Write(&MaterialNameSize,2,1);
					file.Write(Material.c_str(),MaterialNameSize,1);
				}

				m_calCoreModel->saveCoreSkeleton(filenameTemp.c_str());
				IFileStream fileTemp(filename,IFileStream::IOM_READ);
				long SkeletonSize;
				void *SkeletonData=NULL;
				if (fileTemp.IsOk())
				{
					SkeletonSize = fileTemp.Size();
					SkeletonData = malloc(SkeletonSize);
					fileTemp.Read(SkeletonData,SkeletonSize,1);
					fileTemp.Close();

					file.Write(&SkeletonSize,4,1);
					file.Write(SkeletonData,SkeletonSize,1);
					free(SkeletonData);
				}

				for (int i=0;i<MeshCount;i++)
				{
					IString MeshName = m_calCoreModel->getCoreMesh(i)->getName().c_str();
					short MeshNameSize = MeshName.Length();
					file.Write(&MeshNameSize,2,1);
					file.Write(MeshName.c_str(),MeshNameSize,1);
					
					m_calCoreModel->saveCoreMesh(filenameTemp.c_str(),i);
					fileTemp.Open(filenameTemp,IFileStream::IOM_READ);
					long MeshSize;
					void *MeshData=NULL;
					if (fileTemp.IsOk())
					{
						MeshSize = fileTemp.Size();
						MeshData = malloc(MeshSize);
						fileTemp.Read(MeshData,MeshSize,1);
						fileTemp.Close();

						file.Write(&MeshSize,4,1);
						file.Write(MeshData,MeshSize,1);
						free(MeshData);
					}
				}
				remove(filenameTemp.c_str()); // Delete temp file
				file.Close();
				return true;
			}

			return false;
		}

		bool IMeshIEM::SetSkeleton(IString filename)
		{
			return m_calCoreModel->loadCoreSkeleton(filename.c_str());
		}

		int IMeshIEM::AddMesh(IString filename)
		{
			return m_calCoreModel->loadCoreMesh(filename.c_str());
		}

		bool IMeshIEM::DeleteMesh(int i)
		{
			return m_calCoreModel->unloadCoreMesh(i);
		}

		bool IMeshIEM::SetMaterial(int id, IString filename)
		{
			while (id>=m_aMaterials.Count())
				m_aMaterials.Add("");
			m_aMaterials[id]=filename;
			//m_calCoreModel->loadCoreMaterial(
			return true;
		}

		void IMeshIEM::SetName(IString name)
		{
			m_sName=name;
		}

		int IMeshIEM::GetMaxMaterial()
		{
			int MaterialCount=1;
			for(int meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
			{					
				CalCoreMesh *mesh = m_calCoreModel->getCoreMesh(meshId);
				if (mesh)
				for(int submeshId = 0; submeshId <  mesh->getCoreSubmeshCount(); submeshId++)
				{
					CalCoreSubmesh *sub = mesh->getCoreSubmesh(submeshId);
					if (sub)
						if (MaterialCount<sub->getCoreMaterialThreadId()+1)
							MaterialCount=sub->getCoreMaterialThreadId()+1;
				}
			}
			return MaterialCount;
		}
	}
}
