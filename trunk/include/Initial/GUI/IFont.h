//---------------------------------------------
//                Initial Engine
//                 Font  Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#ifndef _IFONT_HEADER_
#define _IFONT_HEADER_

#include "Initial/Core/IArray.h"
#include "Initial/Core/IString.h"
#include "Initial/3D/ITriangle.h"
#include "Initial/3D/IPolygon.h"
#include "Initial/Video/IRenderDriver.h"

namespace Initial
{
	namespace GUI
	{
		class IFontDrawParam
		{
		public:
			IFontDrawParam()
				: m_bSolid(true), m_bOutline(false), m_bShadow(true), m_bFixedSpace(false),
				m_cShadowColor(0.25,0.25,0.25,0.25), m_cOutlineColor(0,0,0), m_cColor(1,1,1), 
				m_fOutlineSize(1.0f), m_fShadowOffsetX(0.05f), m_fShadowOffsetY(-0.05f), m_fSize(1.0f), m_fFixedSpaceWidth(1.0f)
			{
			}

			bool m_bSolid;
			bool m_bOutline;
			bool m_bShadow;
			bool m_bFixedSpace;
			Core::IColor m_cShadowColor;
			Core::IColor m_cOutlineColor;
			Core::IColor m_cColor;
			float m_fOutlineSize;

			float m_fShadowOffsetX;
			float m_fShadowOffsetY;
			float m_fSize;
			float m_fFixedSpaceWidth;
		};
		
		class IFontChar
		{
			//friend class Core::IArray;
			friend class IFontManager;
		public:
			IFontChar();
			~IFontChar();
			void SetPolygons(Core::IArray<IPolygon*>& pols);
			void Render(Video::IRenderDriver *device, Core::IColor color, bool outline=false, float outlinesize=1.0);
		protected:
			char						m_iChar;
			Core::IArray<ITriangle*>	m_aFilled;
			Core::IArray<IPolygon*>		m_aOutline;
			float						m_fWidth;
			unsigned int				m_iDisplayListFilled;
			unsigned int				m_iDisplayListOutline;
		};

		class IFont
		{
			friend class IFontManager;
		public:
			~IFont() {};
		protected:		
			IFont() {};
		protected:
			Core::IString				m_sFontName;
			Core::IArray<IFontChar>	m_aCharMap;
			Video::IRenderDriver*	m_pDevice;
		};
	}

}

#endif