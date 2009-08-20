//---------------------------------------------
//                Initial Engine
//          Node Text source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Node/INodeText.h"
#include "Initial/IDevice.h"
#include "Initial/Video/IRenderDriver.h"

using namespace Initial::Core;
using namespace Initial::Video;

namespace Initial
{	
	INodeText::INodeText(IDevice *device)
		: INode(device)
	{		
		m_pFont=NULL;

		AddProperty("Message",IPT_LONG_STRING,&m_sMessage);

		AddProperty("Solid",IPT_BOOL,&(m_Params.m_bSolid));
		AddProperty("Outline",IPT_BOOL,&(m_Params.m_bOutline));
		AddProperty("Shadow",IPT_BOOL,&(m_Params.m_bShadow));

		AddProperty("Font color",IPT_COLOR,&(m_Params.m_cColor));
		AddProperty("Outline color",IPT_COLOR,&(m_Params.m_cOutlineColor));
		AddProperty("Shadow color",IPT_COLOR,&(m_Params.m_cShadowColor));

		AddProperty("Size",IPT_FLOAT,&(m_Params.m_fSize));
		AddProperty("Shadow Offset",IPT_VEC2,&(m_Params.m_vShadowOffset));
		
		//AddProperty("Fixed space",IPT_BOOL,&(m_Params.m_bFixedSpace));
		//AddProperty("Fixed space width",IPT_FLOAT,&(m_Params.m_fFixedSpaceWidth));
	}	

	void INodeText::_InitNode()
	{
		if (m_pDevice)
		{
			m_pMaterial = m_pDevice->GetRessourceManager()->LoadMaterial("materials/engine/text.ima");

			//m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/DS-DIGI.TTF",0);
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/african.ttf",0);
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/ALGER.TTF");
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/SLNTHLN.TTF");
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/SFOR.ttf");
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/BRADHITC.TTF");
			m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/fable2.ttf",2);
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("c:/windows/fonts/arial.ttf",1);
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("c:/windows/fonts/comic.ttf",1);
		}
		m_sMessage="Message";
	}

	void INodeText::Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags=0)
	{
		if (!m_bShow)
			return;

		driver->_SetModelViewMatrix();
		driver->_PushMatrix();

			driver->_MultMatrix(GetPositionMatrix());

			driver->UseMaterial(m_pMaterial);

			m_pDevice->GetFontManager()->RenderText(m_pFont,m_sMessage,m_Params);			

			BaseClass::Render(driver,frustum,flags);

		driver->_PopMatrix();
	}

	Initial::GUI::IFontDrawParam& INodeText::GetParams()
	{
		return m_Params;
	}

	void INodeText::SetText(Core::IString msg)
	{
		m_sMessage=msg;
	}

	void INodeText::SetMaterial(Video::IMaterial* material)
	{
		m_pMaterial=material;
	}
}
