//---------------------------------------------
//                Initial Engine
//             Scene Manager Module
//
//            By Thibault HENNEQUIN
//---------------------------------------------

#ifndef _ISCENEMANAGER_HEADER_
#define _ISCENEMANAGER_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"
#include "Initial/IObject.h"
#include "ISmartPtr.h"

#include "Initial/Ressource/IAnimatedMesh.h"
#include "Initial/Ressource/IMesh.h"
#include "Initial/Ressource/IMaterial.h"
#include "Initial/Video/IShader.h"
#include "Initial/Ressource/ITexture.h"

#include "Initial/Thread/IThread.h"


namespace Initial
{
	class IDevice;
	class INode;

	class ISceneManager : public INode
	{
		friend class IDevice;
	public:
		//static IRessourceManager *GetRessourceManager();
		~ISceneManager();
	protected:
		ISceneManager(IDevice *device);	
	};
};

#endif