//---------------------------------------------
//                Initial Engine
//               Text node Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODETEXT_HEADER_
#define _INODETEXT_HEADER_

#include "Initial/Node/INode.h"
#include "Initial/Ressource/IFont.h"

namespace Initial
{
	class INodeText : public INode
	{
		DECLARE_OBJECT(INodeText,INode)
	public:
		INodeText(IDevice *device=NULL);

		virtual Core::IString GetBitmapName() { return "NodeText.bmp"; }

		virtual void _InitNode();

		virtual void Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags);
		void SetText(Core::IString msg);
		Initial::GUI::IFontDrawParam& GetParams();
		void SetMaterial(Video::IMaterial* material);
	protected:
		ISmartPtr<Video::IMaterial> m_pMaterial;
		Initial::GUI::IFont* m_pFont;
		Initial::GUI::IFontDrawParam m_Params;
		Core::IString m_sMessage;

		bool m_bCOD4;
		float m_fcharTime;
		int m_icharChange;
		float m_fState;
		long m_lLastTime;
	};
}

#endif