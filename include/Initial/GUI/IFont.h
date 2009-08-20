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

#include <vector>

namespace Initial
{
	namespace GUI
	{
		class IFontDrawParam
		{
		public:
			IFontDrawParam()
				: m_bSolid(true), m_bOutline(true), m_bShadow(true), m_bFixedSpace(false),
				m_cShadowColor(0.25,0.25,0.25,0.25), m_cOutlineColor(0,0,0), m_cColor(1,1,1), 
				m_fOutlineSize(1.0f), m_vShadowOffset(0.05f,-0.05f), m_fSize(1.0f), m_fFixedSpaceWidth(1.0f)
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

			Core::IVector2D m_vShadowOffset;
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
			void SetPolygons(std::vector<std::vector<Core::IVector3D>>& pols);
			void Render(Video::IRenderDriver *device, Core::IColor color, bool outline=false, float outlinesize=1.0);
		protected:
			char						m_iChar;
			std::vector<ITriangle>						m_aFilled;
			std::vector<std::vector<Core::IVector3D>>	m_aOutline;
			float						m_fWidth;
			unsigned int				m_iDisplayListFilled;
			unsigned int				m_iDisplayListOutline;
		};

		class IFont //: public IRessource
		{
			friend class IFontManager;
		public:
			~IFont() {};
		protected:		
			IFont() {};
			IFont(const IFont& font)
			{
				m_sFontName=font.m_sFontName;
				m_aCharMap=font.m_aCharMap;
				m_pDevice=font.m_pDevice;
			};
		protected:
			Core::IString			m_sFontName;
			std::vector<IFontChar>	m_aCharMap;
			Video::IRenderDriver*	m_pDevice;
		};
	}

}

#endif