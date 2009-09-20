//---------------------------------------------
//                Initial Engine
//             Font Manager  Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#ifndef _IFONTMANAGER_HEADER_
#define _IFONTMANAGER_HEADER_

#ifdef _IFONTMANAGER_COMPILE_
	#include <ft2build.h> 
	#include FT_FREETYPE_H 
	#include FT_GLYPH_H
	#include FT_OUTLINE_H
	#include FT_BBOX_H
#endif

#include "Initial/Video/IRenderDriver.h"
#include "Initial/Core/IList.h"
#include "Initial/Core/IString.h"
#include "Initial/3D/ILine.h"
#include "Initial/3D/IPolygon.h"
#include "Initial/Ressource/IFont.h"

#include <vector>
//class FT_Vector;

namespace Initial
{
	namespace GUI
	{
		class IFontManager
		{
#ifdef _IFONTMANAGER_COMPILE_
			static int _MoveTo (FT_Vector *to, void *data);
			static int _LineTo (FT_Vector *to, void *data);
			static int _ConicTo (FT_Vector *control, FT_Vector *to, void *data);
			static int _CubicTo (FT_Vector *control1, FT_Vector *control2, FT_Vector *to,
								 void *data);

			class DecomposeState
			{
			public:
				float current_x, current_y;
				//IRenderDriver *device;
				float size;
				int curvequality;
				//Core::IList<IPolygon> Array;
				std::vector<std::vector<Core::IVector3D>> Array;
			};
#endif
		public:
			IFontManager(IDevice *device=NULL);
			~IFontManager();

			void SetDevice(IDevice *device);
			IFont* LoadFont(Core::IString filename, int quality=1);

			void RenderText(IFont *font, Core::IString& text, IFontDrawParam param=IFontDrawParam());

		protected:
			IDevice*	m_pDevice;
#ifdef _IFONTMANAGER_COMPILE_
			FT_Library		m_Library;
			FT_Face			m_Face;
#endif
		};
	}

}

#endif