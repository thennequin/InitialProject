//---------------------------------------------
//                Initial Engine
//          Font Manager source Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#define _IFONTMANAGER_COMPILE_

#include "Initial/Core/IAssert.h"
#include "Initial/ILogger.h"
#include "Initial/Math/IMath.h"
#include "Initial/GUI/IFontManager.h"
#include "Initial/IDevice.h"

const int StartChar=32;

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;
using namespace Initial::GUI;

IFontManager::IFontManager(IDevice *device)
{
	FT_Error error;
	FT_Init_FreeType( &m_Library );
	m_pDevice=device;
}

IFontManager::~IFontManager()
{
	ILogger::LogDebug("~IFontManager\n");
	FT_Done_FreeType( m_Library );
}

void IFontManager::SetDevice(IDevice *device)
{
	m_pDevice=device;
}

IFont* IFontManager::LoadFont(IString filename, int quality)
{
	if (!m_pDevice)
		return NULL;

	FT_Error error;
	error = FT_New_Face( m_Library, filename.c_str(), 0, &m_Face ); 	
	if (error)
	{
		ILogger::LogError("Erreur de chargement de la font\n");
		return NULL;
	}

	float fontsize=64*50;
	FT_Outline_Funcs handlers;
	DecomposeState state;

	FT_Select_Charmap(m_Face, FT_ENCODING_UNICODE);
	FT_Set_Char_Size( m_Face, fontsize, fontsize, 64, 64 ); 
	printf("Char map %d\n",m_Face->num_glyphs);
	//FT_Set_Charmap(m_Face, FT_ENCODING_UNICODE);

	IFont *font = new IFont();
	if (!font)
	{
		FT_Done_Face(m_Face);
		return NULL;
	}
	font->m_sFontName=m_Face->family_name;

	handlers.move_to = (FT_Outline_MoveToFunc)_MoveTo;
	handlers.line_to = (FT_Outline_LineToFunc)_LineTo;
	handlers.conic_to = (FT_Outline_ConicToFunc)_ConicTo;
	handlers.cubic_to = (FT_Outline_CubicToFunc)_CubicTo;
	handlers.shift = 0;
	handlers.delta = 0;

	state.size=fontsize;
	state.curvequality=quality;

	font->m_aCharMap.resize(255-StartChar);
	for (int i=StartChar;i<255;i++)
	{
		error = FT_Load_Char( m_Face, i, FT_LOAD_NO_BITMAP );
		if (error)
		{			
			continue;
			//printf("%c error\n",i);
		}
		
		IFontChar& car=font->m_aCharMap[i-StartChar];
		car.m_iChar=i;
		FT_Outline outline = m_Face->glyph->outline;	
		state.Array.clear();
		FT_Outline_Decompose(&outline,&handlers,&state);
		car.SetPolygons(state.Array);

		car.m_fWidth=m_Face->glyph->advance.x/fontsize;
	}

	FT_Done_Face(m_Face);
	ILogger::LogMessage("Font loaded\n");
	return font;
}

void IFontManager::RenderText(IFont *font, IString& text, IFontDrawParam param)
{
	//printf("Render Text1\n");
	if (font)
	{
		//printf("Render Text2\n");
		int i, yc=0;
		if (m_pDevice && m_pDevice->GetRenderDriver())
		{
			//printf("Render Text3\n");
			IRenderDriver *driver = m_pDevice->GetRenderDriver();
			//driver->UseMaterial(NULL);
			driver->_PushMatrix();
				driver->_Scale(param.m_fSize,param.m_fSize);
				//driver->_DisableExt(IEXT_DEPTH_TEST);
				//driver->_EnableExt(IEXT_BLEND);			
			
				for (i=0;i<text.Length();i++)
				{
					if (text[i]=='\n')
					{
						yc++;
						driver->_PopMatrix();
						driver->_PushMatrix();
						driver->_Scale(param.m_fSize,param.m_fSize);
						driver->_Translate(0,-1*yc,0);
					}else if (text[i]=='\t')
					{
						driver->_Translate(1.5,0,0);
					}else{
						unsigned char car = text[i]-StartChar;
						if (&font->m_aCharMap[car])
						{		
							float space;
							if (param.m_bFixedSpace)
							{
								//space = param.m_fFixedSpaceWidth;
								space = (param.m_fFixedSpaceWidth - font->m_aCharMap[car].m_fWidth)/2.0;
								driver->_Translate(space,0,0);
								space = space + font->m_aCharMap[car].m_fWidth;
							}
							else
								space = font->m_aCharMap[car].m_fWidth;// = RenderChar(text[i],16)

							

							//Render Shadow
							if (param.m_bShadow)
							{
								driver->_PushMatrix();							
									driver->_Translate(param.m_vShadowOffset.x,param.m_vShadowOffset.y,-0.01);
									font->m_aCharMap[car].Render(driver,param.m_cShadowColor);
								driver->_PopMatrix();
							}

							if (param.m_bSolid)
								font->m_aCharMap[car].Render(driver,param.m_cColor);
							if (param.m_bOutline)
							{
								driver->_PushMatrix();							
									driver->_Translate(0,0,-0.005);
									font->m_aCharMap[car].Render(driver,param.m_cOutlineColor,true,param.m_fOutlineSize/100.0);
								driver->_PopMatrix();
							}

							float kerning=0.0;
							driver->_Translate(space+kerning,0,0);
						}
					}
				}

				//driver->_DisableExt(IEXT_BLEND);
				//m_pDevice->_EnableExt(EXT_DEPTH_TEST);
			driver->_PopMatrix();
		}
	}
}

int IFontManager::_MoveTo (FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;
	ASSERT(s)

	std::vector<IVector3D> temp;
	s->Array.push_back(temp);
	ASSERT(!s->Array.empty())
	IVector3D val(to->x/s->size,to->y/s->size);
	s->Array.back().push_back(val);

    s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}

int IFontManager::_LineTo (FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;
	ASSERT(s)

	ASSERT(!s->Array.empty())
	IVector3D val(to->x/s->size,to->y/s->size);
	s->Array.back().push_back(val);

	s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}

int IFontManager::_ConicTo (FT_Vector *control, FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;
	ASSERT(s)

	float quality = 0;
	if (s->curvequality>0)
		quality=s->curvequality*2;
	else
		quality=1;

	ASSERT(!s->Array.empty())
	for (float i=1/quality;i<=1;i+=1/quality)
	{
		IVector3D pt = Math::RenderConicBezier(IVector3D(s->current_x,s->current_y),
			IVector3D(to->x,to->y),
			IVector3D(control->x,control->y),i);

		pt = pt*(1.0/s->size);
		s->Array.back().push_back(pt);
	}

	s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}

int IFontManager::_CubicTo (FT_Vector *control1, FT_Vector *control2, FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;
	ASSERT(s)

	float quality = 0;
	if (s->curvequality>0)
		quality=s->curvequality*2;
	else
		quality=1;

	ASSERT(!s->Array.empty())
	for (float i=1/quality;i<=1;i+=1/quality)
	{
		IVector3D pt = Math::RenderCubicBezier(IVector3D(s->current_x,s->current_y),
			IVector3D(to->x,to->y),
			IVector3D(control1->x,control1->y)*(1/s->size),
			IVector3D(control2->x,control2->y)*(1/s->size),i);

		pt = pt*(1.0/s->size);
		s->Array.back().push_back(pt);
	}

	s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}
