//---------------------------------------------
//                Initial Engine
//       Ressource Manager Module Source
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Config.h"
#include "Initial/IRessourceManager.h"
#include "Initial/IDevice.h"
#include "Initial/ILogger.h"
#include "Initial/Core/IUtil.h"

using namespace Initial::Core;
using namespace Initial::Video;

namespace Initial
{
	IRessourceManager::IRessourceManager(IDevice *device)
	{
		m_pDevice=device;
	}

	IRessourceManager::~IRessourceManager()
	{
	}

	IMaterial*		IRessourceManager::LoadMaterial(IString filename)
	{
		IMaterial *mat;
		filename=GetFullPath(filename);
		mat = (IMaterial*)GetRessource(filename);
		if (mat)
			return mat;

		mat = new IMaterial(m_pDevice);
		bool OK = mat->Load(filename);
		if (OK==false)
			OK = mat->LoadV2(filename);
		AddRessource(mat);
		return mat;
	}

	ITexture*			IRessourceManager::LoadTexture(IString filename)
	{
		ITexture *tex;
		filename=GetFullPath(filename);
		tex = (ITexture*)GetRessource(filename);
		if (tex)
			return tex;

		//tex = new ITexture(m_pDevice);
		tex = m_pDevice->GetRenderDriver()->GetTextureManager()->LoadTexture(filename);
		AddRessource(tex);
		return tex;
	}

	IMesh*			IRessourceManager::LoadMesh(IString filename)
	{
		return (IMesh*)NULL;
	}

	/*ISmartPtr<IAnimatedMesh>	IRessourceManager::LoadAnimatedMesh(IString filename)
	{
		for (UINT i=0;i<m_aRessources.Count();i++)
		{
			if (m_aRessources[i]->m_sFileName==filename)
				return (IAnimatedMesh*)m_aRessources[i];
		}

		return new IAnimatedMesh(NULL,filename);
	}*/

	Format::IMeshIEM*		IRessourceManager::LoadAnimatedMesh(Core::IString filename)
	{
		for (UINT i=0;i<m_aRessources.Count();i++)
		{
			if (filename!="" && m_aRessources[i]->m_sFileName==filename)
				return (Format::IMeshIEM*)m_aRessources[i];
		}
		Format::IMeshIEM *iem = new Format::IMeshIEM;
		iem->Load(filename);
		AddRessource(iem);
		return iem;
	}

	IMaterial* IRessourceManager::CreateNewMaterial(IString name)
	{		
		IMaterial *mat = new IMaterial(m_pDevice);
		mat->CompileV2();
		mat->SetFilename(":"+name);
		AddRessource(mat);
		return mat;
	}

	ITexture* IRessourceManager::CreateNewTexture(IString name)
	{
		ITexture *tex =m_pDevice->GetRenderDriver()->GetTextureManager()->CreateTexture(1,1,ITF_RGBA);
		AddRessource(tex);
		tex->SetFilename("#"+name);
		return tex;
	}

	void IRessourceManager::AddRessource(IRessource *ptr)
	{
		m_mMutex.Lock();
		if (ptr)
			m_aRessources.Add(ptr);
		m_mMutex.Unlock();
	}

	void IRessourceManager::DeleteRessource(IRessource *ptr)
	{
		m_mMutex.Lock();
		if (ptr)
			m_aRessources.DeleteItem(ptr);
		m_mMutex.Unlock();
	}

	IRessource *IRessourceManager::GetRessource(IString& filename)
	{
		m_mMutex.Lock();
		IRessource *res=NULL;
		for (UINT i=0;i<m_aRessources.Count();i++)
		{
			if (m_aRessources[i])
			{
				//printf("%s\n", m_aRessources[i]->m_sFileName.c_str());
				if (filename!="" && m_aRessources[i]->m_sFileName==filename)
				{
					res = m_aRessources[i];
					break;
				}
			}
		}
		m_mMutex.Unlock();
		return res;
	}

	void IRessourceManager::ThreadFunc()
	{
		for (UINT i=0;i<m_aRessources.Count();i++)
		{
			m_aRessources[i]->m_sFileName;
		}

		HANDLE dwChangeHandles; 
		 
		dwChangeHandles = FindFirstChangeNotification( 
			"./",
			true,
			FILE_NOTIFY_CHANGE_LAST_WRITE);
		 
		if (dwChangeHandles == INVALID_HANDLE_VALUE) 
			return;
			//ExitProcess(GetLastError()); 
		 
		bool bContinue = true;
		while (bContinue)
		{
			DWORD fileHandle = WaitForSingleObject(dwChangeHandles, INFINITE);
			if ( fileHandle - WAIT_OBJECT_0 == 0)
			{
				const int MAX_BUFFER = 1024;
				BYTE Buffer[MAX_BUFFER];
				DWORD dwBytesReturned = 0;

				OVERLAPPED over;

				ReadDirectoryChangesW(dwChangeHandles,
					Buffer,
					MAX_BUFFER,
					TRUE,
					FILE_NOTIFY_CHANGE_LAST_WRITE,
					&dwBytesReturned,
					0,
					0);
				IString file;
				file.AllocBuffer((dwBytesReturned-12)/2);
				int pos=0;
				for (int i=12;i<dwBytesReturned;)
				{
					file[pos]=Buffer[i];
					pos++;
					i+=2;
				}
				file[pos]='\0';
				
				//ILogger::LogDebug("File change %d\n",(int)dwBytesReturned);
				file = GetFullPath(file);
				printf("File change %d %s\n",file.Length(), file.c_str());				
				printf("Ressource count %d\n",m_aRessources.Count());
				for (UINT i=0;i<m_aRessources.Count();i++)
				{
					if (m_aRessources[i]->m_sFileName==file)
					{
						m_aRessources[i]->Reload();
						break;
					}
				}

				FindNextChangeNotification(dwChangeHandles);
			}
			else // exit loop, as m_event has became signalled
				bContinue = false;
		}

		FindCloseChangeNotification(dwChangeHandles);
	}
};
