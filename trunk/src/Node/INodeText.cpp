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
		
		AddProperty("Fixed space",IPT_BOOL,&(m_Params.m_bFixedSpace));
		AddProperty("Fixed space width",IPT_FLOAT,&(m_Params.m_fFixedSpaceWidth));

		AddProperty("COD4 style",IPT_BOOL,&m_bCOD4);		
		AddProperty("Char change time",IPT_FLOAT,&m_fcharTime);
		AddProperty("Char change count",IPT_INTEGER,&m_icharChange);
		m_bCOD4=false;
		m_fcharTime=0.25;
		m_icharChange=4;

		m_fState=0.0;
		m_lLastTime=GetTickCount();
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
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/fable2.ttf",2);
			m_pFont = m_pDevice->GetFontManager()->LoadFont("Fonts/Bank Gothic Medium BT.ttf",1);
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("c:/windows/fonts/arial.ttf",1);
			//m_pFont = m_pDevice->GetFontManager()->LoadFont("c:/windows/fonts/comic.ttf",1);
		}
		m_sMessage="Text";
	}

	void INodeText::Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags=0)
	{
		if (!m_bShow)
			return;

		driver->_SetModelViewMatrix();
		driver->_PushMatrix();

			driver->_MultMatrix(GetPositionMatrix());

			driver->UseMaterial(m_pMaterial);

			IString newmsg=m_sMessage;

			Initial::GUI::IFontDrawParam tempParams=m_Params;

			if (m_bCOD4)
			{
				//const float m_fcharTime=0.15;
				//const int m_icharChange=3;
				float time = (GetTickCount()-m_lLastTime)/1000.0;
				m_lLastTime=GetTickCount();
				float sequenceLength=newmsg.Length()*m_fcharTime*2.0+2.0;
				int strLen=newmsg.Length();
				printf("-------------\n");
				for (int i=0;i<strLen;i++)
				{
					//int newi=((strLen/2)+(2*i)*(2*i)*(2*i))%strLen;
					int newi=i;
					printf("%d\t%d\n",i,newi);

					if (newmsg[i]!='\n') // No change line return
					{
						float secondTransTime=strLen*m_fcharTime+2.0;
						
						//Blank char
						if (m_fState<newi*m_fcharTime || m_fState>=secondTransTime+(/*strLen-*/newi+1)*m_fcharTime )
						{
							newmsg[i]=' ';
						}//First transform
						else if (m_fState>=newi*m_fcharTime && m_fState<(newi+1)*m_fcharTime)
						{
							float tempState = (m_fState-newi*m_fcharTime);

							newmsg[i]=(newmsg[i]-32+(int)(tempState*(m_icharChange/m_fcharTime)))%223+32;
						}//Second transform
						else if (m_fState>=secondTransTime+(/*strLen-*/newi)*m_fcharTime && m_fState<secondTransTime+(/*strLen-*/newi+1)*m_fcharTime)				
						{
							float tempState = (m_fState-(secondTransTime+(/*strLen-*/newi)*m_fcharTime));

							newmsg[i]=(newmsg[i]-32+(int)(tempState*(m_icharChange/m_fcharTime)))%223+32;
						}//Else no change
					}
				}
				m_fState+=time;
				if (m_fState>=sequenceLength)
					m_fState=0;
				//tempParams.m_bFixedSpace=true;
			}
			
			m_pDevice->GetFontManager()->RenderText(m_pFont,newmsg,tempParams);


			//m_pDevice->GetFontManager()->RenderText(m_pFont,m_sMessage,m_Params);

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
