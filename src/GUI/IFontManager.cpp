//---------------------------------------------
//                Initial Engine
//          Font Manager source Module
//
//            By Thibault HENNEQUIN
//              December 23th  2008
//---------------------------------------------

#include "Core/IMath.h"
#include "GUI/IFontManager.h"

const int StartChar=32;

using namespace Initial;
using namespace Initial::GUI;

IFontManager::IFontManager()
{
	FT_Error error;
	FT_Init_FreeType( &m_Library );
}

IFontManager::~IFontManager()
{
	printf("~IFontManager\n");
}


void IFontManager::SetRenderDevice(IRenderDriver *device)
{
	m_pDevice=device;
}

IFont* IFontManager::LoadFont(IString filename, int quality)
{
	if (!m_pDevice)
		return NULL;

	FT_Error error;
	error = FT_New_Face( m_Library, filename, 0, &m_Face ); 	
	if (error)
	{
		printf("Erreur de chargement de la font\n");
		return NULL;
	}/*else
		printf("Font Ok\n");*/

	float fontsize=64*50;
	FT_Outline_Funcs handlers;
	DecomposeState state;

	FT_Select_Charmap(m_Face, FT_ENCODING_NONE);
	FT_Set_Char_Size( m_Face, fontsize, fontsize, 64, 64 ); 
	//printf("Char map %d\n",m_Face->num_glyphs);
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

	for (int i=StartChar;i<255;i++)
	{
		error = FT_Load_Char( m_Face, i, FT_LOAD_NO_BITMAP|FT_LOAD_RENDER );
		if (error)
		{
			font->m_aCharMap.Add(IFontChar());
			continue;
			//printf("%c error\n",i);
		}
		//printf("%c ok %d %d\n",i, m_Face->glyph->outline.n_contours,m_Face->glyph->outline.n_points);
		
		IFontChar *car = new IFontChar();
		if (car)
		{
			car->m_iChar=i;
			FT_Outline outline = m_Face->glyph->outline;	
			state.Array.Clear();
			FT_Outline_Decompose(&outline,&handlers,&state);
			car->SetPolygons(state.Array);

			car->m_fWidth=m_Face->glyph->advance.x/fontsize;
		}
		font->m_aCharMap.Add(*car);
	}

	FT_Done_Face(m_Face);
	return font;
}

void IFontManager::RenderText(IFont *font, IString text, IFontDrawParam param)
{
	if (font)
	{
		int i, yc=0;
		if (m_pDevice)
		{
			m_pDevice->UseMaterial(NULL);
			m_pDevice->_PushMatrix();
				m_pDevice->_Scale(param.m_fSize,param.m_fSize);
				//m_pDevice->_DisableExt(EXT_DEPTH_TEST);
				m_pDevice->_EnableExt(EXT_BLEND);			
			
				for (i=0;i<text.Length()-1;i++)
				{
					if (text[i]=='\n')
					{
						yc++;
						m_pDevice->_PopMatrix();
						m_pDevice->_PushMatrix();
						m_pDevice->_Scale(param.m_fSize,param.m_fSize);
						m_pDevice->_Translate(0,-1*yc,0);
					}else if (text[i]=='\t')
					{
						m_pDevice->_Translate(1.5,0,0);
					}else{
						unsigned char car = text[i]-StartChar;
						if (&font->m_aCharMap[car])
						{		
							float space;
							if (param.m_bFixedSpace)
								space = param.m_fFixedSpaceWidth;
							else
								space = font->m_aCharMap[car].m_fWidth;// = RenderChar(text[i],16)

							//Render Shadow
							if (param.m_bShadow)
							{
								m_pDevice->_PushMatrix();							
									m_pDevice->_Translate(param.m_fShadowOffsetX,param.m_fShadowOffsetY,-0.001);
									font->m_aCharMap[car].Render(m_pDevice,param.m_cShadowColor);
								m_pDevice->_PopMatrix();
							}

							if (param.m_bSolid)
								font->m_aCharMap[car].Render(m_pDevice,param.m_cColor);
							if (param.m_bOutline)
							{
								m_pDevice->_PushMatrix();							
									m_pDevice->_Translate(0,0,0.001);
									font->m_aCharMap[car].Render(m_pDevice,param.m_cOutlineColor,true,param.m_fOutlineSize);
								m_pDevice->_PopMatrix();
							}

							float kerning=0.0;
							m_pDevice->_Translate(space+kerning,0,0);
						}
					}
				}

				m_pDevice->_DisableExt(EXT_BLEND);
				//m_pDevice->_EnableExt(EXT_DEPTH_TEST);
			m_pDevice->_PopMatrix();
		}
	}
}

int IFontManager::_MoveTo (FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;

	s->Array.Add(IPolygon());
	s->Array.Last().AddPoint(IVertex(to->x/s->size,to->y/s->size));

    s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}

int IFontManager::_LineTo (FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;

	s->Array.Last().AddPoint(IVertex(to->x/s->size,to->y/s->size));

	s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}

int IFontManager::_ConicTo (FT_Vector *control, FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;


	float quality = 0;
	if (s->curvequality>0)
		quality=s->curvequality*2;
	else
		quality=1;

	for (float i=0;i<=1;i+=1/quality)
	{
		IVertex pt = Math::RenderConicBezier(IVertex(s->current_x,s->current_y),
			IVertex(to->x,to->y),
			IVertex(control->x,control->y),i);

		s->Array.Last().AddPoint(IVertex(pt*(1/s->size)));
	}

	s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}

int IFontManager::_CubicTo (FT_Vector *control1, FT_Vector *control2, FT_Vector *to, void *data)
{
	DecomposeState *s = (DecomposeState*) data;

	float quality = 0;
	if (s->curvequality>0)
		quality=s->curvequality*2;
	else
		quality=1;

	for (float i=0;i<=1;i+=1/quality)
	{
		IVertex pt = Math::RenderCubicBezier(IVertex(s->current_x,s->current_y),
			IVertex(to->x,to->y),
			IVertex(control1->x,control1->y)*(1/s->size),
			IVertex(control2->x,control2->y)*(1/s->size),i);

		s->Array.Last().AddPoint(IVertex(pt*(1/s->size)));
	}

	s->current_x = to->x;
    s->current_y = to->y;
	return 0;
}
