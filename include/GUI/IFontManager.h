//---------------------------------------------
//                Initial Engine
//             Font Manager  Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#ifndef _IFONTMANAGER_HEADER_
#define _IFONTMANAGER_HEADER_

#include <ft2build.h> 
#include FT_FREETYPE_H 
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BBOX_H

#include "Video/IRenderDriver.h"
#include "Core/IArray.h"
#include "Core/IString.h"
#include "3D/ILine.h"
#include "3D/IPolygon.h"
#include "GUI/IFont.h"

namespace Initial
{
	namespace GUI
	{
		class IFontManager
		{
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
				IArray<IPolygon> Array;
			};

		public:
			IFontManager();
			~IFontManager();

			void SetRenderDevice(IRenderDriver *device);
			IFont* LoadFont(IString filename, int quality=1);

			void RenderText(IFont *font, IString text, IFontDrawParam param=IFontDrawParam());

		protected:
			IRenderDriver*	m_pDevice;

			FT_Library		m_Library;
			FT_Face			m_Face;
			//static IFontManager
		};
	}

}

#endif