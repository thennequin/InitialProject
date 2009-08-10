//---------------------------------------------
//                Initial Engine
//       Ressource Manager Module Source
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Config.h"
#include "Initial/ISceneManager.h"
#include "Initial/IDevice.h"
#include "Initial/ILogger.h"
#include "Initial/Core/IUtil.h"
#include "Initial/Node/INode.h"

using namespace Initial::Core;
using namespace Initial::Video;

namespace Initial
{
	ISceneManager::ISceneManager(IDevice *device)
		: INode(device)
	{
	}

	ISceneManager::~ISceneManager()
	{
	}
};
