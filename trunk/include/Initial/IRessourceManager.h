//---------------------------------------------
//                Initial Engine
//           Ressource Manager Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IRESSOURCEMANAGER_HEADER_
#define _IRESSOURCEMANAGER_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"
#include "Initial/IObject.h"
#include "ISmartPtr.h"

#include "Initial/Ressource/IAnimatedMesh.h"
#include "Initial/Ressource/IMesh.h"
#include "Initial/Ressource/IMaterial.h"
#include "Initial/Video/IShader.h"
#include "Initial/Ressource/ITexture.h"
#include "Initial/Format/IMeshIEM.h"

#include "Initial/Thread/IThread.h"

//#include <list>


namespace Initial
{
	class IDevice;

	class IRessourceManager : public IThread
	{
		friend class IObject;
		friend class IDevice;
	public:
		~IRessourceManager();

		Video::IMaterial*				LoadMaterial(Core::IString filename);
		Video::ITexture*				LoadTexture(Core::IString filename);
		IMesh*							LoadMesh(Core::IString filename);
		//ISmartPtr<IAnimatedMesh>		LoadAnimatedMesh(Core::IString filename);
		Format::IMeshIEM*				LoadAnimatedMesh(Core::IString filename);

		Video::IMaterial*				CreateNewMaterial(Core::IString name);
		Video::ITexture*				CreateNewTexture(Core::IString name);

	protected:
		IRessourceManager(IDevice *device);

		void AddRessource(IRessource *ptr);
		void DeleteRessource(IRessource *ptr);
		IRessource *GetRessource(Core::IString& filename);

		virtual void ThreadFunc();
	protected:
		IDevice *m_pDevice;

		Core::IList<IRessource*>	m_aRessources;
		//std::list<IRessource*>	m_aRessources;

		IMutex m_mMutex;
	};
};

#endif