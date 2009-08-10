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

#include "Initial/Video/IRenderDriver.h"
#include "Initial/Core/IArray.h"
#include "Initial/Core/IString.h"
#include "Initial/3D/ILine.h"
#include "Initial/3D/IPolygon.h"
#include "Initial/GUI/IFont.h"

//class FT_Vector;

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
				Core::IArray<IPolygon*> Array;
			};
		public:
			IFontManager();
			~IFontManager();

			void SetRenderDevice(Video::IRenderDriver *device);
			IFont* LoadFont(Core::IString filename, int quality=1);

			void RenderText(IFont *font, Core::IString text, IFontDrawParam param=IFontDrawParam());

		protected:
			Video::IRenderDriver*	m_pDevice;

			FT_Library		m_Library;
			FT_Face			m_Face;
			//static IFontManager
		};
	}

}

#endif