
#include "Initial/IDevice.h"
#include "Initial/IObject.h"
#include "Initial/IRessourceManager.h"
#include "Initial/ISceneManager.h"

namespace Initial
{
	IDevice::IDevice()
	{
		Initial::InitObject();
		m_pRenderDriver = NULL;
		//m_pRessourceManager = IRessourceManager::GetRessourceManager();
		m_pRessourceManager = new IRessourceManager(this);
		m_pSceneManager = new ISceneManager(this);
		m_pFontManager = new GUI::IFontManager(this);
		m_pRessourceManager->Run();
	}

	Video::IRenderDriver *IDevice::GetRenderDriver()
	{
		return m_pRenderDriver;
	}

	IRessourceManager *IDevice::GetRessourceManager()
	{
		return m_pRessourceManager;
	}

	ISceneManager *IDevice::GetSceneManager()
	{
		return m_pSceneManager;
	}	

	GUI::IFontManager *IDevice::GetFontManager()
	{
		return m_pFontManager;
	}

	int IDevice::GetWidth()
	{
		return m_iWidth;
	}

	int IDevice::GetHeight()
	{
		return m_iHeight;
	}

}
