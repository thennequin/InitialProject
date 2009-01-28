//---------------------------------------------
//                Initial Engine
//                 Font  Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#ifndef _IFONT_HEADER_
#define _IFONT_HEADER_

#include "Core/IArray.h"
#include "Core/IString.h"
#include "3D/ITriangle.h"
#include "3D/IPolygon.h"
#include "Video/IRenderDriver.h"

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
				m_fOutlineSize(1.0), m_fShadowOffsetX(0.05), m_fShadowOffsetY(-0.05), m_fSize(1.0), m_fFixedSpaceWidth(1.0)
			{
			}

			bool m_bSolid;
			bool m_bOutline;
			bool m_bShadow;
			bool m_bFixedSpace;
			IColor m_cShadowColor;
			IColor m_cOutlineColor;
			IColor m_cColor;
			float m_fOutlineSize;

			float m_fShadowOffsetX;
			float m_fShadowOffsetY;
			float m_fSize;
			float m_fFixedSpaceWidth;
		};
		
		class IFontChar
		{
			//friend class IArray;
			friend class IFontManager;
		public:
			IFontChar();
			~IFontChar();
			void SetPolygons(IArray<IPolygon>& pols);
			void Render(IRenderDriver *device, IColor color, bool outline=false, float outlinesize=1.0);
		protected:
			char				m_iChar;
			IArray<ITriangle>	m_aFilled;
			IArray<IPolygon>	m_aOutline;
			float				m_fWidth;
			unsigned int		m_iDisplayListFilled;
			unsigned int		m_iDisplayListOutline;
		};

		class IFont
		{
			friend class IFontManager;
		public:
			~IFont() {};
		protected:		
			IFont() {};
		protected:
			IString				m_sFontName;
			IArray<IFontChar>	m_aCharMap;
			IRenderDriver*	m_pDevice;
		};
	}

}

#endif