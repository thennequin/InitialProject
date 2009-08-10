#include "GraphShapeSky.h"
#include "FlowGraph.h"

#include "wx/dcbuffer.h"

GraphShapeSky::GraphShapeSky()
{
	DrawParam.BodyBackColor = wxColor(183,226,255);
	DrawParam.TitleBackColor = wxColor(150,202,255);
	DrawParam.ShapeNotSelect = wxColor(128,128,128);
	DrawParam.InOutBlockColor = DrawParam.ShapeNotSelect;
	DrawParam.TitleBodySpace=0;
}

void GraphShapeSky::CalculateRender()
{
	GraphShape::CalculateRender();
	//RenderParam.PenSelected.SetWidth(1);
	//RenderParam.PenNotSelected.SetWidth(1);
}

void GraphShapeSky::Draw(wxDC& dc)
{
	dc.SetTextForeground(wxColour(0,0,0));

	//Select Pen
	if (Selected())
		dc.SetPen(RenderParam.PenSelected);
	else
		dc.SetPen(RenderParam.PenNotSelected);

	float Zoom=GetCanvas()->GetZoom();
	int DefX = GetX()+DrawParam.InOutBlockSize;
	int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;

	RenderParam.FontInOut.SetPixelSize(wxSize(0,DrawParam.InOutFontHeight/Zoom));
	RenderParam.FontTitle.SetPixelSize(wxSize(0,DrawParam.TitleFontHeight/Zoom));		

		//Render Shadow
#if wxUSE_GRAPHICS_CONTEXT == 1
	bool m_bShadow=false;
	if (m_bShadow)
	{
		wxColour ShadowColor = wxColor(16,16,16,128);
		int shadx=5;
		int shady=5;
		wxGraphicsContext *dc2 = wxGraphicsContext::Create(*((wxBufferedPaintDC*)&dc));
		if (dc2)
		{
			dc2->SetPen(wxPen(ShadowColor));
			dc2->SetBrush(wxBrush(ShadowColor));
			dc2->DrawRoundedRectangle((DefX+shadx)/Zoom, (GetY()+shady)/Zoom,
				RenderParam.Width/Zoom,
				RenderParam.Height/Zoom,10/Zoom);
		}
	}
#endif

	//Render Body
	dc.SetBrush(RenderParam.BrushBodyBackground);
	dc.DrawRoundedRectangle(DefX/Zoom, GetY()/Zoom,
		RenderParam.Width/Zoom,
		RenderParam.Height/Zoom,10/Zoom);

	//Render Title
	dc.SetBrush(RenderParam.BrushTitleBackground);
	dc.DrawRoundedRectangle(DefX/Zoom,GetY()/Zoom,
		RenderParam.Width/Zoom, (DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2)/Zoom,10/Zoom);
	dc.SetBrush(RenderParam.BrushTitleText);
	
	dc.SetFont(RenderParam.FontTitle);
	dc.DrawText(Name,(DefX+(RenderParam.Width-RenderParam.TitleWidth)/2)/Zoom,
		(GetY()+DrawParam.TitleSpaceBorder)/Zoom);
	
	//Render Inputs	
	dc.SetFont(RenderParam.FontInOut);
	
	for (unsigned int i=0;i<Inputs.Count();i++)
	{
		dc.SetBrush(RenderParam.BrushBodyText);
		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
		{
			dc.DrawText(Inputs[i]->Name,
				(DefX+RenderParam.Width-DrawParam.InOutSpaceBorder*2-Inputs[i]->TextWidth-DrawParam.InOutFormatPreviewSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace))/Zoom);
			/*wxBitmap *bit = FormatBitmap[Inputs[i]->Format];
			if (bit)
			{
				wxImage Img=bit->ConvertToImage();
				Img.Rescale(DrawParam.InOutFormatPreviewSize,DrawParam.InOutFormatPreviewSize);
				dc.DrawBitmap(wxBitmap(Img),DefX+RenderParam.Width-DrawParam.InOutSpaceBorder-DrawParam.InOutFormatPreviewSize,BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace));
			}*/

			dc.SetBrush(RenderParam.BrushInOutBlock);
			if (Selected())
				dc.SetPen(RenderParam.PenSelected);
			else
				dc.SetPen(RenderParam.PenNotSelected);

			dc.SetBrush(RenderParam.BrushInOutBlock);

			wxPoint pts[3];
						
			pts[0] = wxPoint((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom);

			pts[1] = wxPoint((DefX+RenderParam.Width+DrawParam.InOutBlockSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);

			pts[2] = wxPoint((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom);

			dc.DrawPolygon(3,pts);
		}else{
			dc.DrawText(Inputs[i]->Name,
				(DefX+RenderParam.Width-DrawParam.InOutSpaceBorder*2-Inputs[i]->TextWidth-DrawParam.InOutFormatPreviewSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutFontHeight)/2)/Zoom);
			/*wxBitmap *bit = FormatBitmap[Inputs[i]->Format];
			if (bit)
			{
				wxImage Img=bit->ConvertToImage();
				Img.Rescale(DrawParam.InOutFormatPreviewSize,DrawParam.InOutFormatPreviewSize);
				dc.DrawBitmap(wxBitmap(Img),DefX+RenderParam.Width-DrawParam.InOutSpaceBorder-DrawParam.InOutFormatPreviewSize,BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace));
			}*/

			dc.SetBrush(RenderParam.BrushInOutBlock);

			if (Selected())
				dc.SetPen(RenderParam.PenSelected);
			else
				dc.SetPen(RenderParam.PenNotSelected);

			dc.SetBrush(RenderParam.BrushInOutBlock);

			wxPoint pts[3];
						
			pts[0] = wxPoint((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom);

			pts[1] = wxPoint((DefX+RenderParam.Width+DrawParam.InOutBlockSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);

			pts[2] = wxPoint((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom);
			dc.DrawPolygon(3,pts);
		}
	}

	//Render Outputs
	for (unsigned int i=0;i<Outputs.Count();i++)
	{
		dc.SetBrush(RenderParam.BrushBodyText);
		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
		{
			dc.DrawText(Outputs[i]->Name,
				(DefX+DrawParam.InOutSpaceBorder*2+DrawParam.InOutFormatPreviewSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace))/Zoom);
			/*wxBitmap *bit = FormatBitmap[Outputs[i]->Format];
			if (bit)
			{
				wxImage Img=bit->ConvertToImage();
				Img.Rescale(DrawParam.InOutFormatPreviewSize,DrawParam.InOutFormatPreviewSize);
				dc.DrawBitmap(wxBitmap(Img),DefX+DrawParam.InOutSpaceBorder,BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace));
			}*/
			
			if (Selected())
				dc.SetPen(RenderParam.PenSelected);
			else
				dc.SetPen(RenderParam.PenNotSelected);

			dc.SetBrush(RenderParam.BrushInOutBlock);

			wxPoint pts[3];
						
			pts[0] = wxPoint((GetX()+DrawParam.InOutBlockSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom);

			pts[1] = wxPoint(GetX()/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);

			pts[2] = wxPoint((GetX()+DrawParam.InOutBlockSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom);

			dc.DrawPolygon(3,pts);
		}else{
			dc.DrawText(Outputs[i]->Name,
				(DefX+DrawParam.InOutSpaceBorder*2+DrawParam.InOutFormatPreviewSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutFontHeight)/2)/Zoom);
			/*wxBitmap *bit = FormatBitmap[Outputs[i]->Format];
			if (bit)
			{
				wxImage Img=bit->ConvertToImage();
				Img.Rescale(DrawParam.InOutFormatPreviewSize,DrawParam.InOutFormatPreviewSize);
				dc.DrawBitmap(wxBitmap(Img),DefX+DrawParam.InOutSpaceBorder,BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace));
			}*/
			
			if (Selected())
				dc.SetPen(RenderParam.PenSelected);
			else
				dc.SetPen(RenderParam.PenNotSelected);

			dc.SetBrush(RenderParam.BrushInOutBlock);

			wxPoint pts[3];
						
			pts[0] = wxPoint((GetX()+DrawParam.InOutBlockSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom);

			pts[1] = wxPoint(GetX()/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);

			pts[2] = wxPoint((GetX()+DrawParam.InOutBlockSize)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom);

			dc.DrawPolygon(3,pts);
		}
	}

	//Render Preview
	if (Preview)
	{
		wxBitmap temp = wxBitmap(PreviewImage.Scale(DrawParam.PreviewSize/Zoom, DrawParam.PreviewSize/Zoom));
		dc.DrawBitmap(temp, (DefX+RenderParam.PreviewX)/Zoom,
			(GetY()+RenderParam.PreviewY)/Zoom);		
	}

	//Render Connection
	for (unsigned int i=0;i<Inputs.Count();i++)
	{
		if (Inputs[i] && Inputs[i]->GetConnection())
		{
			Inputs[i]->GetConnection()->DrawConnection(dc);
		}
	}

	OnDraw(dc);
}

void GraphShapeSky::DrawInputHighlight(int id, wxDC& dc)
{
	if (id>=0 && id<Inputs.Count())
	{
		float Zoom=GetCanvas()->GetZoom();
		int DefX = GetX()+DrawParam.InOutBlockSize;
		int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;

		if (Selected())
			dc.SetPen(RenderParam.PenSelected);
		else
			dc.SetPen(RenderParam.PenNotSelected);

		dc.SetBrush(RenderParam.BrushInOutBlockOver);

		wxPoint pts[3];
					
		pts[0] = wxPoint((DefX+RenderParam.Width)/Zoom,
			(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom);

		pts[1] = wxPoint((DefX+RenderParam.Width+DrawParam.InOutBlockSize)/Zoom,
			(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);

		pts[2] = wxPoint((DefX+RenderParam.Width)/Zoom,
			(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom);

		dc.DrawPolygon(3,pts);
	}
}

void GraphShapeSky::DrawOutputHighlight(int id, wxDC& dc)
{
	if (id>=0 && id<Outputs.Count())
	{
		float Zoom=GetCanvas()->GetZoom();
		int DefX = GetX()+DrawParam.InOutBlockSize;
		int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;

		if (Selected())
			dc.SetPen(RenderParam.PenSelected);
		else
			dc.SetPen(RenderParam.PenNotSelected);

		dc.SetBrush(RenderParam.BrushInOutBlockOver);
		
		wxPoint pts[3];
					
		pts[0] = wxPoint((GetX()+DrawParam.InOutBlockSize)/Zoom,
			(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom);

		pts[1] = wxPoint(GetX()/Zoom,
			(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);

		pts[2] = wxPoint((GetX()+DrawParam.InOutBlockSize)/Zoom,
			(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom);

		dc.DrawPolygon(3,pts);
	}
}
