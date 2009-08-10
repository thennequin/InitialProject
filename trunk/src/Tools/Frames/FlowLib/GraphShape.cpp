#include "GraphShape.h"
#include "FlowGraph.h"

#include "wx/dcbuffer.h"

IMPLEMENT_DYNAMIC_CLASS(GraphShape,wxObject)

GraphShape::GraphShape()
: m_pCanvas(NULL), m_bSelected(false)
{	
	Name="GraphShape";
	Deletable=true;
	Preview=false;

	DrawParam.InOutFormatPreviewSize=0;

	/*Inputs.Add(new InOutput("Input 1",false,0,"",0));
	Inputs.Add(new InOutput("Input 2",false,0,"",0));
	Inputs.Add(new InOutput("Input 3",false,0,"",0));
	Outputs.Add(new InOutput("Output 1",true,0,"",0));
	Outputs.Add(new InOutput("Output 2",true,0,"",0));*/
}

GraphShape::~GraphShape()
{
	for (UINT i=0;i<Outputs.Count();i++)
		if (Outputs[i])
			delete Outputs[i];

	for (UINT i=0;i<Inputs.Count();i++)
		if (Inputs[i])
			delete Inputs[i];
}

FlowGraph* GraphShape::GetCanvas()
{
	return m_pCanvas;
}

void GraphShape::SetCanvas(FlowGraph* canvas)
{
	m_pCanvas=canvas;
}

int GraphShape::GetX()
{ 
	float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
	int w = GetCanvas() ? GetCanvas()->GetSize().x : 0;
	return (m_iPos.x-w/2)+w/2*Zoom; 
}

void GraphShape::ConnectInput(int InId,GraphShape *dest,int destOutId)
{
	if (this==dest)
		return;

	if (dest==NULL) // Destroy connection
	{
		if (Inputs[InId] && Inputs[InId]->Connect)
		{
			delete Inputs[InId]->Connect;
			Inputs[InId]->Connect=NULL;
		}
	}else if (Inputs[InId])
	{		
		/*if (InOutIsLinkable(Inputs[InId],dest->Outputs[destOutId]))*/
		{
			Connection *connect;
			if (Inputs[InId]->Connect)
				connect = Inputs[InId]->Connect;
			else{
				Inputs[InId]->Connect = new Connection();
				connect = Inputs[InId]->Connect;
			}
			connect->InputShape=this;
			connect->Input=Inputs[InId];
			connect->InputId=InId;
			connect->OutputShape=dest;
			connect->Output=dest->Outputs[destOutId];
			connect->OutputId=destOutId;	
		}
	}
}

int GraphShape::GetY()
{ 
	float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
	int h = GetCanvas() ? GetCanvas()->GetSize().y : 0;
	return (m_iPos.y-h/2)+h/2*Zoom; 
}

void GraphShape::SetPos(int x, int y)
{
	m_iPos.x=x;
	m_iPos.y=y;
}

void GraphShape::StartDrag()
{
	m_iDragPos=m_iPos;
}

void GraphShape::MoveTo(int x, int y)
{
	m_iPos.x=m_iDragPos.x+x;
	m_iPos.y=m_iDragPos.y+y;
}

void GraphShape::SetSelected(bool select)
{
	m_bSelected=select;
}

bool GraphShape::Selected()
{
	return m_bSelected;
}

void GraphShape::CalculateRender()
{
	//Setup brush, font, pen an color
	RenderParam.FontInOut.SetPixelSize(wxSize(0,DrawParam.InOutFontHeight));
	RenderParam.FontTitle.SetPixelSize(wxSize(0,DrawParam.TitleFontHeight));		
	RenderParam.FontTitle.SetWeight(wxFONTWEIGHT_BOLD);

	RenderParam.BrushTitleText.SetColour(DrawParam.TitleTextColor);
	RenderParam.BrushTitleBackground.SetColour(DrawParam.TitleBackColor);

	RenderParam.BrushBodyText.SetColour(DrawParam.BodyTextColor);
	RenderParam.BrushBodyBackground.SetColour(DrawParam.BodyBackColor);

	RenderParam.BrushInOutBlock.SetColour(DrawParam.InOutBlockColor);
	RenderParam.BrushInOutBlockOver.SetColour(DrawParam.InOutBlockColorOver);

	RenderParam.PenSelected.SetColour(DrawParam.ShapeSelect);
	RenderParam.PenNotSelected.SetColour(DrawParam.ShapeNotSelect);

	if (GetCanvas())
	{
		wxClientDC dc(GetCanvas());
		GetCanvas()->PrepareDC(dc);

		// Calculate Title width	
		dc.SetFont(RenderParam.FontTitle);
		dc.GetTextExtent(Name,&(RenderParam.TitleWidth),NULL);

		// Calculate Input width
		dc.SetFont(RenderParam.FontInOut);
		RenderParam.InputWidth=0;
		for (UINT i=0;i<Inputs.Count();i++)
		{
			int Temp;
			dc.GetTextExtent(Inputs[i]->GetName(),&Temp,NULL);
			Inputs[i]->TextWidth=Temp;
			if (Temp>RenderParam.InputWidth)
				RenderParam.InputWidth=Temp;
		}

		// Calculate Output width
		RenderParam.OutputWidth=0;
		for (UINT i=0;i<Outputs.Count();i++)
		{
			int Temp;
			dc.GetTextExtent(Outputs[i]->GetName(),&Temp,NULL);
			Outputs[i]->TextWidth=Temp;
			if (Temp>RenderParam.OutputWidth)
				RenderParam.OutputWidth=Temp;
		}

		// Calculate Width
		int BodyWidth = 0; 
		if (Inputs.Count()>0)
			BodyWidth+=RenderParam.InputWidth+DrawParam.InOutSpaceBorder + DrawParam.InOutFormatPreviewSize+DrawParam.InOutSpaceBorder;
		if (Outputs.Count()>0)
			BodyWidth+=RenderParam.OutputWidth+DrawParam.InOutSpaceBorder + DrawParam.InOutFormatPreviewSize+DrawParam.InOutSpaceBorder;

		if (Outputs.Count()>0 && Inputs.Count()>0 && Preview==false)
			BodyWidth+=DrawParam.InOutWidthSpace;
		else{
			if (Preview)
				BodyWidth+=DrawParam.PreviewSize;

			if (Inputs.Count()>0)
				BodyWidth+=DrawParam.InOutSpaceBorder;
			else if (Preview)
				BodyWidth+=DrawParam.PreviewSpaceBorder;

			if (Outputs.Count()>0)
				BodyWidth+=DrawParam.InOutSpaceBorder;
			else if (Preview)
				BodyWidth+=DrawParam.PreviewSpaceBorder;
		}

		if (BodyWidth<RenderParam.TitleWidth+DrawParam.TitleSpaceBorder*2)
			RenderParam.Width = RenderParam.TitleWidth+DrawParam.TitleSpaceBorder*2;
		else
			RenderParam.Width = BodyWidth;	

		if (RenderParam.Width<DrawParam.MinWidth)
			RenderParam.Width=DrawParam.MinWidth;

		// Calculate Height
		int BodyHeight=0;

		int TitleHeight = DrawParam.TitleFontHeight + DrawParam.TitleSpaceBorder*2;

		int MaxInOut=0;
		if(Inputs.Count()>MaxInOut)
			MaxInOut = Inputs.Count();
		if(Outputs.Count()>MaxInOut)
			MaxInOut = Outputs.Count();

		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
		{
			if (MaxInOut>1)
				BodyHeight = DrawParam.InOutSpaceBorder*2 + MaxInOut*DrawParam.InOutFontHeight+(MaxInOut-1)*DrawParam.InOutHeightSpace;
			else
				BodyHeight = DrawParam.InOutSpaceBorder*2 + MaxInOut*DrawParam.InOutFontHeight;
		}else{
			if (MaxInOut>1)
				BodyHeight = DrawParam.InOutSpaceBorder*2 + MaxInOut*DrawParam.InOutFormatPreviewSize+(MaxInOut-1)*DrawParam.InOutHeightSpace;
			else
				BodyHeight = DrawParam.InOutSpaceBorder*2 + MaxInOut*DrawParam.InOutFormatPreviewSize;
		}

		if (Preview)
		{
			if (BodyHeight<DrawParam.PreviewSize+DrawParam.PreviewSpaceBorder*2)
				BodyHeight=DrawParam.PreviewSize+DrawParam.PreviewSpaceBorder*2;
		}
		if (BodyHeight<DrawParam.MinHeight)
			BodyHeight=DrawParam.MinHeight;
		
		RenderParam.Height = TitleHeight + DrawParam.TitleBodySpace + BodyHeight;	

		// Calculate Preview Pos
		if (Preview)
		{
			RenderParam.PreviewY=DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace+DrawParam.PreviewSpaceBorder;

			int MaxInWidth=0;
			int MaxOutWidth=0;
			for (UINT i=0;i<Inputs.Count();i++)
			{
				if (Inputs[i]->TextWidth>MaxInWidth)	
					MaxInWidth=Inputs[i]->TextWidth;
			}
			for (UINT i=0;i<Outputs.Count();i++)
			{
				if (Outputs[i]->TextWidth>MaxOutWidth)	
					MaxOutWidth=Outputs[i]->TextWidth;
			}

			if (Inputs.Count()>0)
				MaxInWidth+=+DrawParam.InOutSpaceBorder*2;

			if (Outputs.Count()>0)
				MaxOutWidth+=+DrawParam.InOutSpaceBorder*2;

			int Space = (RenderParam.Width - (MaxInWidth+MaxOutWidth+DrawParam.InOutSpaceBorder*2))-(DrawParam.PreviewSize+DrawParam.PreviewSpaceBorder*2);

			RenderParam.PreviewX=MaxOutWidth+DrawParam.InOutSpaceBorder*2+Space/2;
		}
	}
	//m_xpos=0;
	//m_ypos=0;
	//SetDefaultRegionSize();
	//SetSize(RenderParam.Width,RenderParam.Height);
}

void GraphShape::Draw(wxDC& dc)
{
	dc.SetTextForeground(wxColour(0,0,0));

	//Select Pen
	if (Selected())
		dc.SetPen(RenderParam.PenSelected);
	else
		dc.SetPen(RenderParam.PenNotSelected);

	float Zoom=GetCanvas()->GetZoom();
	//dc.SetLogicalScale(Zoom,Zoom);
	int DefX = GetX()+DrawParam.InOutBlockSize;
	int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;

	RenderParam.FontInOut.SetPixelSize(wxSize(0,DrawParam.InOutFontHeight/Zoom));
	RenderParam.FontTitle.SetPixelSize(wxSize(0,DrawParam.TitleFontHeight/Zoom));

	//Render Shadow
#if wxUSE_GRAPHICS_CONTEXT == 1
	bool m_bShadow=false;
	if (m_bShadow)
	{
		wxColour ShadowColor = wxColor(64,64,64,128);
		int shadx=5;
		int shady=5;
		wxGraphicsContext *dc2 = wxGraphicsContext::Create(*((wxBufferedPaintDC*)&dc));
		if (dc2)
		{
			dc2->SetPen(wxPen(ShadowColor));
			dc2->SetBrush(wxBrush(ShadowColor));
			dc2->DrawRectangle((DefX+shadx)/Zoom,(GetY()+shady)/Zoom+3,
				RenderParam.Width/Zoom, (DrawParam.TitleFontHeight)/Zoom);
			dc2->DrawRectangle((DefX+shadx)/Zoom, (BodyY+shady)/Zoom,
				RenderParam.Width/Zoom,
				(RenderParam.Height-(DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder+DrawParam.TitleBodySpace))/Zoom);
		}
	}
#endif

	//Render Title
	dc.SetBrush(RenderParam.BrushTitleBackground);
	dc.DrawRectangle(DefX/Zoom,GetY()/Zoom,
		RenderParam.Width/Zoom, (DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2)/Zoom);
	dc.SetBrush(RenderParam.BrushTitleText);
	
	dc.SetFont(RenderParam.FontTitle);
	dc.DrawText(Name,(DefX+(RenderParam.Width-RenderParam.TitleWidth)/2)/Zoom,
		(GetY()+DrawParam.TitleSpaceBorder)/Zoom);

	//Render Body
	dc.SetBrush(RenderParam.BrushBodyBackground);
	dc.DrawRectangle(DefX/Zoom, BodyY/Zoom,
		RenderParam.Width/Zoom,
		(RenderParam.Height-(DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder+DrawParam.TitleBodySpace))/Zoom);
	
	//Render Inputs	
	dc.SetFont(RenderParam.FontInOut);
	
	for (UINT i=0;i<Inputs.Count();i++)
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
			dc.DrawRectangle((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,
				DrawParam.InOutBlockSize/Zoom);
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
			dc.DrawRectangle((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,
				DrawParam.InOutBlockSize/Zoom);
		}
	}

	//Render Outputs
	for (UINT i=0;i<Outputs.Count();i++)
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
			
			dc.SetBrush(RenderParam.BrushInOutBlock);
			dc.DrawRectangle(GetX()/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,DrawParam.InOutBlockSize/Zoom);
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
			
			dc.SetBrush(RenderParam.BrushInOutBlock);
			dc.DrawRectangle(GetX()/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,DrawParam.InOutBlockSize/Zoom);
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
	for (UINT i=0;i<Inputs.Count();i++)
	{
		if (Inputs[i] && Inputs[i]->GetConnection())
		{
			Inputs[i]->GetConnection()->DrawConnection(dc);
		}
	}

	OnDraw(dc);
}

void GraphShape::DrawInputHighlight(int id, wxDC& dc)
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

		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
			dc.DrawRectangle((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,
				DrawParam.InOutBlockSize/Zoom);
		else
			dc.DrawRectangle((DefX+RenderParam.Width)/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,
				DrawParam.InOutBlockSize/Zoom);
	}
}

void GraphShape::DrawOutputHighlight(int id, wxDC& dc)
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

		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
			dc.DrawRectangle(GetX()/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,DrawParam.InOutBlockSize/Zoom);
		else
			dc.DrawRectangle(GetX()/Zoom,
				(BodyY+DrawParam.InOutSpaceBorder+id*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom,
				DrawParam.InOutBlockSize/Zoom,DrawParam.InOutBlockSize/Zoom);
	}
}

bool GraphShape::InShape(int x, int y)
{
	float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
	int DefX = GetX()+DrawParam.InOutBlockSize;
	if (x >= DefX/Zoom
		&& x <= (DefX+RenderParam.Width)/Zoom
		&& y >= GetY()/Zoom
		&& y <= (GetY()+RenderParam.Height)/Zoom)
		return true;
	
	if (HitOutput(x,y)!=-1)
		return true;
	if (HitInput(x,y)!=-1)
		return true;
	return false;
}

bool GraphShape::ContainsShape(int x, int y, int width, int height)
{
	if (width<0)
	{
		x=x+width;
		width=-width;
	}

	if (height<0)
	{
		y=y+height;
		height=-height;
	}

	float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
	int DefX = GetX()+DrawParam.InOutBlockSize;
	if (x <= DefX/Zoom
		&& x+width >= (DefX+RenderParam.Width)/Zoom
		&& y <= GetY()/Zoom
		&& y+height >= (GetY()+RenderParam.Height)/Zoom)
		return true;
	
	if (HitOutput(x,y)!=-1)
		return true;
	if (HitInput(x,y)!=-1)
		return true;
	return false;
}

int GraphShape::HitOutput(double x, double y)
{
	float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
	int DefX = GetX()+DrawParam.InOutBlockSize;
	int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;

	for (UINT i=0;i<Outputs.Count();i++)
	{
		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
		{
			if (x >= GetX()/Zoom
				&& x<= (GetX()+DrawParam.InOutBlockSize)/Zoom
				&& y>= (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom
				&& y<=   (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom)
				return i;
		}else{
			if (x >= GetX()/Zoom
				&& x<= (GetX()+DrawParam.InOutBlockSize)/Zoom
				&& y>= (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom
				&& y<= (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom)
				return i;
		}
	}
	return -1;
}

int GraphShape::HitInput(double x, double y)
{
	float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
	int DefX = GetX()+DrawParam.InOutBlockSize;
	int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;

	for (UINT i=0;i<Inputs.Count();i++)
	{
		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
		{
			if (x >= (DefX+RenderParam.Width)/Zoom
				&& x <= (DefX+RenderParam.Width+DrawParam.InOutBlockSize)/Zoom
				&& y >= (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2)/Zoom
				&& y <= (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom)
				return i;
		}else
		{
			if (x >= (DefX+RenderParam.Width)/Zoom
				&& x <= (DefX+RenderParam.Width+DrawParam.InOutBlockSize)/Zoom
				&& y >= (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2)/Zoom
				&& y <= (BodyY+DrawParam.InOutSpaceBorder+i*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize)/Zoom)
				return i;
		}
	}
	return -1;
}

wxPoint GraphShape::GetInputPos(int InputId)
{
	if (InputId<Inputs.Count())
	{
		float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
		int DefX = GetX()+DrawParam.InOutBlockSize;
		int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;

		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
		{
			return wxPoint((DefX+RenderParam.Width+DrawParam.InOutBlockSize/2)/Zoom,(BodyY+DrawParam.InOutSpaceBorder+InputId*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);
		}else{
			return wxPoint((DefX+RenderParam.Width+DrawParam.InOutBlockSize/2)/Zoom,(BodyY+DrawParam.InOutSpaceBorder+InputId*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);
		}
	}

	return wxPoint();
}

wxPoint GraphShape::GetOutputPos(int OutputId)
{
	if (OutputId<Outputs.Count())
	{
		float Zoom = GetCanvas() ? GetCanvas()->GetZoom() : 1.0;
		int DefX = GetX()+DrawParam.InOutBlockSize;
		int BodyY = GetY()+DrawParam.TitleFontHeight+DrawParam.TitleSpaceBorder*2+DrawParam.TitleBodySpace;
				
		if (DrawParam.InOutFontHeight>DrawParam.InOutFormatPreviewSize)
		{
			return wxPoint((GetX()+DrawParam.InOutBlockSize/2)/Zoom,(BodyY+DrawParam.InOutSpaceBorder+OutputId*(DrawParam.InOutFontHeight+DrawParam.InOutHeightSpace)+(DrawParam.InOutFontHeight-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);
		}else{
			return wxPoint((GetX()+DrawParam.InOutBlockSize/2)/Zoom,(BodyY+DrawParam.InOutSpaceBorder+OutputId*(DrawParam.InOutFormatPreviewSize+DrawParam.InOutHeightSpace)+(DrawParam.InOutFormatPreviewSize-DrawParam.InOutBlockSize)/2+DrawParam.InOutBlockSize/2)/Zoom);
		}
	}

	return wxPoint();
}

GraphShape* GraphShape::GetShapeOnInput(int id)
{
	if (id >=0 && id<Inputs.Count())
	{
		if (Inputs[id] && Inputs[id]->GetConnection())
		{
			return Inputs[id]->GetConnection()->OutputShape;
		}
	}

	return NULL;
}

int GraphShape::GetShapeOutputIdOnInput(int id)
{
	if (id >=0 && id<Inputs.Count())
	{
		if (Inputs[id] && Inputs[id]->GetConnection())
		{
			return Inputs[id]->GetConnection()->OutputId;
		}
	}

	return NULL;
}

int GraphShape::GetInputCount()
{
	return Inputs.Count();
}

int GraphShape::GetOutputCount()
{
	return Outputs.Count();
}

bool GraphShape::IsDeletable()
{
	return Deletable;	
}

void GraphShape::OnDraw(wxDC& dc)
{

}

bool GraphShape::IsPreviewActive()
{
	return Preview;
}
