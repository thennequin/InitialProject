
#include "RessourcePanel.h"
#include <wx/dcbuffer.h>

#include "Initial/Initial.h"

using namespace Initial;

void IImageToWxImage(Initial::Core::IImage& src, wxImage& dst)
{
	unsigned long Time=GetTickCount();
	if (src.GetFormat())
	{
		dst.Create(src.GetWidth(),src.GetHeight());

		for (int y=0;y<src.GetHeight();y++)
		{
			for (int x=0;x<src.GetWidth();x++)
			{
				if (src.GetFormat()==Initial::IIF_RGBA)
				{
					memcpy(	dst.GetData()+(y*src.GetWidth()+x)*3,
							src.GetData()+(y*src.GetWidth()+x)*4,
							3);
					dst.SetAlpha(x,y, ((unsigned char*)src.GetData())[(y*src.GetWidth()+x)*4+3]);
				}else if (src.GetFormat()==Initial::IIF_RGB)
				{
					memcpy(	dst.GetData()+(y*src.GetWidth()+x)*3,
							src.GetData()+(y*src.GetWidth()+x)*3,
							3);
				}
			}
		}
	}
	//printf("Time %f %d\n",(GetTickCount()-Time)/1000.0,dst.GetData());
}

//----------------------------------------------
// RessourceItem
//----------------------------------------------

wxString RessourceItem::GetPath()
{
	wxFileName file(m_sPath);
	return file.GetPath();
}

wxString RessourceItem::GetFilename()
{
	wxFileName file(m_sPath);
	return file.GetFullName();
}

wxString RessourceItem::GetExt()
{
	wxFileName file(m_sPath);
	return file.GetExt();
}

RessourceItem::RessourceType RessourceItem::GetType()
{
	return m_iType;
}

wxImage& RessourceItem::GetThumb()
{
	return m_iThumb;
}

wxColour RessourceItem::GetTypeColour()
{
	if (m_iType==RessourceItem::RT_Folder) // Folder
		return wxColour(192,192,64);
	else if (m_iType==RessourceItem::RT_Image) // Image
		return wxColour(225,32,0);
	
	// Other
	return wxColour(128,176,225);
}

int RessourceItem::GetInfoCount()
{
	if (GetExt().Lower()=="itx")
		return 5;
	else
		return 0;
}

wxString RessourceItem::GetInfo(int i)
{
	if (GetExt().Lower()=="itx")
	{
		Format::IImageITX itx;
		itx.Load(m_sPath.c_str());
		wxString format;
		switch (itx.GetFormat())
		{
		case Format::IImageITX::ITXF_RGBA:
			format="RGBA";
			break;
		case Format::IImageITX::ITXF_DXT1:
			format="DXT1";
			break;
		case Format::IImageITX::ITXF_DXT1a:
			format="DXT1a";
			break;
		case Format::IImageITX::ITXF_DXT1nm:
			format="DXT1nm";
			break;
		case Format::IImageITX::ITXF_DXT3:
			format="DXT3";
			break;
		case Format::IImageITX::ITXF_DXT5:
			format="DXT5";
			break;
		case Format::IImageITX::ITXF_DXT5nm:
			format="DXT5nm";
			break;
		case Format::IImageITX::ITXF_ATI1:
			format="ATI1";
			break;
		case Format::IImageITX::ITXF_ATI2_3Dc:
			format="ATI2/3Dc";
			break;
		};
		if (i==0)
			return wxString::Format("Width : %d",itx.GetWidth());
		else if (i==1)
			return wxString::Format("Height : %d",itx.GetHeight());
		else if (i==2)
			return wxString::Format("Format : %s",format);
		/*else if (i==3)
			return wxString::Format("Format : %s",infos.OrigFile);
		else if (i==4)
			return wxString::Format("Format : %d",infos.OrigFile.Len());*/
	}
	return "";
}

wxImage RessourceItem::_CreateThumb(wxString fileStr)
{
	wxFileName file(fileStr);
	if (file.GetExt().Lower()=="itx")
	{
		wxImage image;
		Initial::Format::IImageITX itx;
		itx.Load(fileStr.c_str());
		IImageToWxImage(itx.GetImage(),image);
		return image;
	}else{
		//Get Image formats handler array
		wxArrayString m_sExts;
		wxList& handlers = wxImage::GetHandlers();
		wxList::iterator node;
		for ( node = handlers.begin(); node != handlers.end(); node++ )
		{
			wxImageHandler *handler = (wxImageHandler*)*node;

			wxString ext_lo = handler->GetExtension();
			m_sExts.Add(ext_lo);			
		}

		for (unsigned int i=0;i<m_sExts.Count();i++)
		{
			if (file.GetExt().Lower()==m_sExts[i])
			{
				return wxImage(fileStr);
				break;
			}
		}
	}
	return wxImage();
}

//----------------------------------------------
// LoadingThread
//----------------------------------------------

RessourcePanel::LoadingThread::LoadingThread(RessourcePanel *panel)
: wxThread(wxTHREAD_JOINABLE)
{
	m_pPanel=panel;
}

void *RessourcePanel::LoadingThread::Entry()
{
	if (m_pPanel)
	{
		while (1)
		{
			m_sLoadingAcess.Wait();
			bool Found=false;
			
			for (unsigned int i=m_pPanel->m_iPreviewPriorityItem;i<m_pPanel->m_aRessources.Count();i++)
			{
				if (m_pPanel->m_aRessources[i]->m_bThumbOk==false)
				{
					Found=true;
				}
			}

			unsigned int i=0;
			if (Found)
				i=m_pPanel->m_iPreviewPriorityItem;
			Found=false;

			for (;i<m_pPanel->m_aRessources.Count();i++)
			{
				if (m_pPanel->m_aRessources[i]->m_bThumbOk==false)
				{
					RessourceItem *res = m_pPanel->m_aRessources[i];
					wxString file = m_pPanel->m_aRessources[i]->m_sPath;
					m_sLoadingAcess.Post();	

					wxImage image = RessourceItem::_CreateThumb(file);	

					m_sLoadingAcess.Wait();		
					if (m_pPanel->m_aRessources[i]==res)
					{
						if (image.IsOk())
							m_pPanel->m_aRessources[i]->m_iThumb=image;
						m_pPanel->m_aRessources[i]->m_bThumbOk=true;
					}

					Found=true;
					break;
				}
			}
			m_sLoadingAcess.Post();
			if (Found==false)
				::Sleep(250);
			else
				::Sleep(50);
		}
	}
	return NULL;
}

//----------------------------------------------
// RessourcePanel
//----------------------------------------------

wxSemaphore RessourcePanel::m_sLoadingAcess;
bool RessourcePanel::m_bLoadingAcessInit=false;

const wxEventType SelectedRessourceEvent = wxNewEventType();

BEGIN_EVENT_TABLE(RessourcePanel, wxScrolledWindow)
    EVT_PAINT(RessourcePanel::OnPaint)
	EVT_ERASE_BACKGROUND(RessourcePanel::OnEraseBackground)

	EVT_MOUSE_EVENTS(RessourcePanel::OnMouseEvent)
	EVT_KEY_UP(RessourcePanel::OnKeyUp)
	EVT_KEY_DOWN(RessourcePanel::OnKeyDown)

	EVT_TIMER(RPID_TIMER,RessourcePanel::OnTimer)
	EVT_TIMER(RPID_TIMERMOUSE,RessourcePanel::OnTimerMouse)
END_EVENT_TABLE()

RessourcePanel::RessourcePanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, unsigned int flags)
: wxScrolledWindow(parent,winid,pos,size)
{
	m_iFlags=flags;
	m_fZoom=1.0;
	m_fZoomTarget=m_fZoom;
	m_iItemSize=150;
	m_iItemSpace=15;
	m_iItemTextSize=14*2;

	m_bKeyShift=false;
	m_bKeyControl=false;

	m_bMouseMiddle=false;
	m_bMouseLeft=false;
	m_bMouseLeftDrag=0;

	m_pLoadingThread=NULL;
	m_bLoadingAcessInstance=false;
	m_iPreviewPriorityItem=0;

	m_iRessourceType=RessourceItem::RT_Image|RessourceItem::RT_All;

	if (m_bLoadingAcessInit==false)
	{
		m_sLoadingAcess.Post();
		m_bLoadingAcessInit=true;
	}

	Timer.SetOwner(this,RPID_TIMER);
	TimerMouse.SetOwner(this,RPID_TIMERMOUSE);
	Timer.Start(500);
	
	//SetDirectory("c:\\textures\\");
	RefreshRessourcesList();
}

RessourcePanel::~RessourcePanel()
{
	if (m_pLoadingThread)
	{
		//m_pLoadingThread->Delete();
		m_pLoadingThread->Kill();
		delete m_pLoadingThread;
	}

	Wait();
	_Clear();
	Post();
}

void RessourcePanel::Post()
{
	if (m_bLoadingAcessInstance==true)
	{
		m_bLoadingAcessInstance=false;
		m_sLoadingAcess.Post();
	}
}

void RessourcePanel::Wait()
{
	if (m_bLoadingAcessInstance==false)
	{
		m_bLoadingAcessInstance=true;
		m_sLoadingAcess.Wait();
	}
}

void RessourcePanel::_Clear()
{
	for (unsigned int i=0;i<m_aRessources.Count();i++)
		if (m_aRessources[i])
			delete m_aRessources[i];

	m_aRessources.Clear();

	_ClearSelection();
}

void RessourcePanel::_ClearSelection()
{
	m_aSelectedItems.Clear();
}

void RessourcePanel::SetDirectory(wxString dir)
{
	m_FS.ChangePathTo(dir,true);
	RefreshRessourcesList();
}

void RessourcePanel::SetRessourceType(RessourceItem::RessourceType type)
{
	m_iRessourceType=type;
}

void RessourcePanel::RefreshRessourcesList()
{
	Wait();
	_Clear();

	wxArrayString m_sExts;
	wxList& handlers = wxImage::GetHandlers();
	wxList::iterator node;
	for ( node = handlers.begin(); node != handlers.end(); node++ )
	{
		wxImageHandler *handler = (wxImageHandler*)*node;

		wxString ext_lo = handler->GetExtension();
		m_sExts.Add(ext_lo);			
	}

	RessourceItem *item=NULL;

	item = new RessourceItem;
	item->m_sPath="..";
	item->m_iType=RessourceItem::RT_Folder;
	item->m_bThumbOk=false;
	m_aRessources.Add(item);
	
	wxString fileStr = m_FS.FindFirst("*",wxDIR);
	while(fileStr!="")
	{
		item = new RessourceItem;
		wxFileName file(fileStr);
		item->m_sPath=fileStr;
		//item->m_sPath=file.GetFullName();
		item->m_iType=RessourceItem::RT_Folder;
		item->m_bThumbOk=false;
		m_aRessources.Add(item);
		fileStr=m_FS.FindNext();
	}

	fileStr = m_FS.FindFirst("*",wxFILE);
	while(fileStr!="")
	{
		item = new RessourceItem;
		wxFileName file(fileStr);
		item->m_sPath=fileStr;
		//item->m_sPath=file.GetFullName();
		item->m_iType=RessourceItem::RT_Other;
		item->m_bThumbOk=false;
		if (file.GetExt().Lower()=="itx")
		{
			item->m_iType=RessourceItem::RT_Image;
		}else{
			for (unsigned int i=0;i<m_sExts.Count();i++)
			{
				if (file.GetExt().Lower()==m_sExts[i])
				{
					item->m_iType=RessourceItem::RT_Image;
					break;
				}
			}
		}
		if (m_iRessourceType&item->m_iType)	
			m_aRessources.Add(item);
		else
			delete item;
		fileStr=m_FS.FindNext();
	}

	if (!m_pLoadingThread)
	{
		m_pLoadingThread = new LoadingThread(this);
		if (m_pLoadingThread && m_pLoadingThread->Create() == wxTHREAD_NO_ERROR)
		{
			m_pLoadingThread->SetPriority(25);
			m_pLoadingThread->Run();
		}
	}

	Post();
}

wxRessourceItemArray RessourcePanel::GetSelection()
{
	return m_aSelectedItems;
}

void RessourcePanel::OnPaint(wxPaintEvent& event)
{
	Wait();

	wxBufferedPaintDC dc(this);

	dc.SetBackground(wxBrush(wxColor(64,64,64), wxSOLID));
    dc.Clear();

	wxFont font = dc.GetFont();
	font.SetPointSize(m_iItemTextSize*m_fZoom/2.0);
	dc.SetFont(font);
	dc.SetTextForeground(wxColour(255,255,255));

	int ItemByLine = (GetSize().x)/m_fZoom/(m_iItemSize+m_iItemSpace);
	if (ItemByLine==0)
		ItemByLine=1;

	int ColumnPosItem=0;
	int RowPosItem=0;

	int ScrollVertical = GetScrollPos(wxVERTICAL);

	wxString TriDot="...";
	long TriDotSize;
	dc.GetTextExtent(TriDot,&TriDotSize,0);

	// On calcul le premier élément à afficher
	int StartItem=0;

	//StartItem=ItemByLine*((ScrollVertical+m_fZoom*m_iItemSpace)/(m_iItemSize+m_iItemTextSize+m_iItemSpace)*m_fZoom);
	RowPosItem=StartItem/ItemByLine;
	//StartItem+=ItemByLine;

	for (unsigned int i=StartItem;i<m_aRessources.Count();i++)
	{
		if (m_aRessources[i])
		{
			bool Selected=false;
			for (unsigned int j=0;j<m_aSelectedItems.Count();j++)
			{
				if (m_aRessources[i]==m_aSelectedItems[j])
				{
					Selected=true;
					//wxLogMessage("Selected %s",m_aRessources[i]->m_sPath);
					break;
				}
			}

			int XPos = m_fZoom*(m_iItemSpace+ColumnPosItem*(m_iItemSize+m_iItemSpace));
			int YPos = m_fZoom*(m_iItemSpace+RowPosItem*(m_iItemSize+m_iItemTextSize+m_iItemSpace))-ScrollVertical;

			if (YPos>GetSize().y) // on evite d'afficher ceux qui sont invisible
				break;
			
			wxImage rescale;
			if (m_aRessources[i]->m_iThumbTemp.GetWidth()!=m_iItemSize*m_fZoom-8*m_fZoom)
			{
			if (m_aRessources[i]->m_iThumbTemp.GetWidth()<m_iItemSize*m_fZoom-8*m_fZoom)
			rescale = wxImage(m_aRessources[i]->m_iThumb);
			else
			rescale = wxImage(m_aRessources[i]->m_iThumbTemp);

			rescale.Rescale(m_iItemSize*m_fZoom-8*m_fZoom,m_iItemSize*m_fZoom-8*m_fZoom);	
			m_aRessources[i]->m_iThumbTemp=rescale;
			}
			dc.DrawBitmap(m_aRessources[i]->m_iThumbTemp,XPos+m_fZoom*4,YPos+m_fZoom*4);

			if (Selected)
			{
				dc.SetBrush(wxBrush(wxColour(0,0,0),wxTRANSPARENT));
				dc.SetPen(wxPen(wxColour(255,225,0),8*m_fZoom));
				dc.DrawRectangle(XPos,YPos,m_iItemSize*m_fZoom,m_iItemSize*m_fZoom);
			}else{
				wxColour color=m_aRessources[i]->GetTypeColour();

				wxColour colorInter(color.Red()/2.0,color.Green()/2.0,color.Blue()/2.0);

				int borderSize=4*m_fZoom;
				if (borderSize==0)
					borderSize=1;

				dc.GradientFillLinear(wxRect(XPos,YPos,m_iItemSize*m_fZoom,borderSize),color,colorInter,wxEAST);
				dc.GradientFillLinear(wxRect(XPos,YPos,borderSize,m_iItemSize*m_fZoom),color,colorInter,wxSOUTH);
				dc.GradientFillLinear(wxRect(XPos,YPos+m_iItemSize*m_fZoom-borderSize,m_iItemSize*m_fZoom,borderSize),colorInter,wxColour(0,0,0),wxEAST);
				dc.GradientFillLinear(wxRect(XPos+m_iItemSize*m_fZoom-borderSize,YPos,borderSize,m_iItemSize*m_fZoom),colorInter,wxColour(0,0,0),wxSOUTH);
			}

			long TextSizeX;
			wxString Text = m_aRessources[i]->GetFilename();
			dc.GetTextExtent(Text,&TextSizeX,NULL);

			if (TextSizeX>m_iItemSize*m_fZoom)
			{
				bool Ok=false;
				for (size_t j=Text.Len()-TriDot.Len();j>=0;j--)
				{
					Text=Text.Left(j);
					dc.GetTextExtent(Text,&TextSizeX,NULL);
					if (TextSizeX+TriDotSize<=m_iItemSize*m_fZoom)
					{
						Ok=true;
						break;
					}
				}
				Text+=TriDot;
				if (Ok==false)
					Text="";
				dc.GetTextExtent(Text,&TextSizeX,NULL);
			}

			dc.DrawText(Text,XPos+(m_fZoom*m_iItemSize-TextSizeX)/2.0,YPos+m_fZoom*m_iItemSize);
			
			ColumnPosItem++;
			if (ColumnPosItem>=ItemByLine)
			{
				ColumnPosItem=0;
				RowPosItem++;
			}
		}
	}

	//Draw area selection
	if (m_bMouseLeftDrag==1)
	{
#if wxUSE_GRAPHICS_CONTEXT == 1
		wxGraphicsContext *dc2 = wxGraphicsContext::Create(*((wxBufferedPaintDC*)&dc));
		if  (dc2)
		{
			dc2->SetPen(wxPen(wxColour(109,182,255),1,wxSHORT_DASH));
			dc2->SetBrush(wxBrush(wxColour(182,226,255,64),wxSOLID));
			int x,y,w,h;
			x=m_i2MousePosStart.x;
			y=m_i2MousePosStart.y;
			w=m_i2MousePos.x-m_i2MousePosStart.x;
			h=m_i2MousePos.y-m_i2MousePosStart.y;
			if (w<0)
			{
				x=x+w;
				w=-w;
			}
			if (h<0)
			{
				y=y+h;
				h=-h;
			}
			dc2->DrawRectangle(x,y,w,h);
			//dc2->DrawBitmap();
			delete dc2;
		}
#else
		dc.SetPen(wxPen(wxColour(109,182,255),1,wxSHORT_DASH));
		dc.SetBrush(wxBrush(wxColour(182,226,255,64),wxTRANSPARENT));
		dc.DrawRectangle(m_i2MousePosStart.x,m_i2MousePosStart.y,m_i2MousePos.x-m_i2MousePosStart.x,m_i2MousePos.y-m_i2MousePosStart.y);
#endif
	}

	if (GetTickCount()-m_iLastMove>=500)
	{
		RessourceItem *res=NULL;
		ColumnPosItem=0;
		RowPosItem=0;
		for (unsigned int i=0;i<m_aRessources.Count();i++)
		{
			if (m_aRessources[i])
			{
				int XPos = m_fZoom*(m_iItemSpace+ColumnPosItem*(m_iItemSize+m_iItemSpace));
				int YPos = m_fZoom*(m_iItemSpace+RowPosItem*(m_iItemSize+m_iItemTextSize+m_iItemSpace))-ScrollVertical;

				if (XPos<=m_i2MousePos.x && XPos+m_fZoom*m_iItemSize>=m_i2MousePos.x &&
					YPos<=m_i2MousePos.y && YPos+m_fZoom*m_iItemSize>=m_i2MousePos.y)
				{
					res=m_aRessources[i];
					break;
				}

				ColumnPosItem++;
				if (ColumnPosItem>=ItemByLine)
				{
					ColumnPosItem=0;
					RowPosItem++;
				}
			}
		}
		//Info box
		if (res)
		{
			wxColour color=res->GetTypeColour();
			wxColour colorInter(color.Red()/2.0,color.Green()/2.0,color.Blue()/2.0);
			int PosX=m_i2MousePos.x;
			int PosY=m_i2MousePos.y+20;
			int BWidth=120;
			int BHeight=25 + res->GetInfoCount()*20;

			font.SetPointSize(12);
			dc.SetFont(font);
			dc.SetTextForeground(wxColour(255,255,255));

			long TextSizeX;			
			for (int i=0;i<res->GetInfoCount();i++)
			{
				dc.GetTextExtent(res->GetInfo(i),&TextSizeX,NULL);
				if (BWidth<TextSizeX)
					BWidth=TextSizeX;
			}
			dc.GetTextExtent(res->GetFilename(),&TextSizeX,NULL);
			if (BWidth<TextSizeX)
				BWidth=TextSizeX;
			BWidth+=12;

			if (PosX+BWidth>GetSize().x-20) //-20 : scrollbar
				PosX=GetSize().x-BWidth-20;
			
			dc.SetBrush(wxBrush(wxColour(0,0,0)));
			dc.SetPen(wxPen(wxColour(0,0,0)));
			dc.DrawRectangle(PosX,PosY,BWidth,BHeight);

			dc.SetBrush(wxBrush(colorInter));
			dc.SetPen(wxPen(colorInter));
			dc.DrawRectangle(PosX,PosY,BWidth,25);

			dc.SetBrush(wxBrush(wxColour(0,0,0,0),wxTRANSPARENT));
			dc.SetPen(wxPen(color,2));
			dc.DrawRoundedRectangle(PosX,PosY,BWidth,BHeight,1);

			dc.DrawText(res->GetFilename(),PosX+6,PosY+3);

			for (int i=0;i<res->GetInfoCount();i++)
			{
				dc.DrawText(res->GetInfo(i),PosX+6,PosY+25+3+i*17);
			}
		}
	}

	int Height = m_fZoom*(m_iItemSpace+ceil(m_aRessources.Count()/(float)ItemByLine)*(m_iItemSize+m_iItemTextSize+m_iItemSpace));
	SetScrollbars(1,1,1,Height,0,GetScrollPos(wxVERTICAL),true);

	Post();
}

void RessourcePanel::OnMouseEvent(wxMouseEvent &event)
{
	Wait();

	if (abs(event.GetPosition().x-m_i2MousePos.x)+abs(event.GetPosition().y-m_i2MousePos.y)>2)
	{
		Refresh();
		m_iLastMove=GetTickCount();
		m_i2MousePos=event.GetPosition();
	}

	int ItemByLine = (GetSize().x)/m_fZoom/(m_iItemSize+m_iItemSpace);
	int ScrollVertical = GetScrollPos(wxVERTICAL);
	int ColumnPosItem=0;
	int RowPosItem=0;
	// On calcul le premier élément à afficher
	int StartItem=0;

	//StartItem=ItemByLine*((ScrollVertical-m_fZoom*m_iItemSpace)/(m_iItemSize+m_iItemTextSize+m_iItemSpace)*m_fZoom);
	//RowPosItem=StartItem/ItemByLine;
	//StartItem+=ItemByLine;

	if (event.ButtonDown())
		SetFocus();

	if (event.ButtonDClick(wxMOUSE_BTN_LEFT))
	{
		wxCoord xMouse, yMouse;
		event.GetPosition(&xMouse,&yMouse);

		for (unsigned int i=StartItem;i<m_aRessources.Count();i++)
		{
			if (m_aRessources[i])
			{
				int XPos = m_fZoom*(m_iItemSpace+ColumnPosItem*(m_iItemSize+m_iItemSpace));
				int YPos = m_fZoom*(m_iItemSpace+RowPosItem*(m_iItemSize+m_iItemTextSize+m_iItemSpace))-ScrollVertical;

				if (XPos<=xMouse && XPos+m_fZoom*m_iItemSize>=xMouse &&
					YPos<=yMouse && YPos+m_fZoom*m_iItemSize>=yMouse)
				{
					if (m_aRessources[i]->m_iType==RessourceItem::RT_Folder)
					{
						SetDirectory(m_FS.GetPath()+m_aRessources[i]->GetFilename());
						RefreshRessourcesList();
						Refresh();					
					}else{
						if (m_iFlags&FLAG_RP_SELECTOR)
						{
							wxCommandEvent MyEvent( SelectedRessourceEvent ); // Keep it simple, don't give a specific event ID
							wxPostEvent(this, MyEvent); // This posts to ourselves: it'll be caught and sent to a different method
						}else
							system(m_aRessources[i]->m_sPath.c_str());
					}
					break;
				}

				ColumnPosItem++;
				if (ColumnPosItem>=ItemByLine)
				{
					ColumnPosItem=0;
					RowPosItem++;
				}
			}
		}
	}else if (event.ButtonIsDown(wxMOUSE_BTN_LEFT))	
	{
		if (m_bMouseLeft==false)
		{
			m_bMouseLeft=true;
			m_i2MousePosStart=event.GetPosition();

			wxCoord xMouse, yMouse;
			event.GetPosition(&xMouse,&yMouse);

			bool Over=false;
			for (unsigned int i=StartItem;i<m_aRessources.Count();i++)
			{
				if (m_aRessources[i])
				{
					int XPos = m_fZoom*(m_iItemSpace+ColumnPosItem*(m_iItemSize+m_iItemSpace));
					int YPos = m_fZoom*(m_iItemSpace+RowPosItem*(m_iItemSize+m_iItemTextSize+m_iItemSpace))-ScrollVertical;

					if (XPos<=xMouse && XPos+m_fZoom*m_iItemSize>=xMouse &&
						YPos<=yMouse && YPos+m_fZoom*m_iItemSize>=yMouse)
						Over=true;

					ColumnPosItem++;
					if (ColumnPosItem>=ItemByLine)
					{
						ColumnPosItem=0;
						RowPosItem++;
					}
				}
			}

			m_bMouseLeftCanDrag=Over;
		}else{
			if (m_bMouseLeftDrag==0)
			{
				wxPoint temp = event.GetPosition()-m_i2MousePosStart;
				if (abs(temp.x)+abs(temp.y)>=2) // On laisse une marge de 2 pixels
				{
					m_bMouseLeftDrag=m_bMouseLeftCanDrag+1;
				}
			}else{ // Selection area				
				Refresh();
			}
		}
	}else if (m_bMouseLeft)
	{
		wxCoord xMouse, yMouse;
		event.GetPosition(&xMouse,&yMouse);
		// Simple clic
		if (m_bMouseLeftDrag==0)
		{
			wxPoint temp = event.GetPosition()-m_i2MousePosStart;
			if (abs(temp.x)+abs(temp.y)<2)
			{
				if (m_bKeyShift==false || !(m_iFlags&FLAG_RP_MULTIPLE_SELECTION))
					_ClearSelection();
				for (unsigned int i=StartItem;i<m_aRessources.Count();i++)
				{
					if (m_aRessources[i])
					{
						int XPos = m_fZoom*(m_iItemSpace+ColumnPosItem*(m_iItemSize+m_iItemSpace));
						int YPos = m_fZoom*(m_iItemSpace+RowPosItem*(m_iItemSize+m_iItemTextSize+m_iItemSpace))-ScrollVertical;

						if (XPos<=xMouse && XPos+m_fZoom*m_iItemSize>=xMouse &&
							YPos<=yMouse && YPos+m_fZoom*m_iItemSize>=yMouse)
						{
							if (m_bKeyShift)
							{
								bool Found=false;
								for (unsigned int j=0;j<m_aSelectedItems.Count();j++)
									if (m_aRessources[i]==m_aSelectedItems[j])
									{
										m_aSelectedItems.Remove(m_aSelectedItems[j]);
										Found=true;
										break;
									}
								if (Found==false)
									m_aSelectedItems.Add(m_aRessources[i]);
							}else
								m_aSelectedItems.Add(m_aRessources[i]);
						}

						ColumnPosItem++;
						if (ColumnPosItem>=ItemByLine)
						{
							ColumnPosItem=0;
							RowPosItem++;
						}
					}
				}
			}
		}else if (m_bMouseLeftDrag==1) // Area selection
		{ 
			if (m_bKeyShift==false || !(m_iFlags&FLAG_RP_MULTIPLE_SELECTION))
				_ClearSelection();
			for (unsigned int i=StartItem;i<m_aRessources.Count();i++)
			{
				if (m_aRessources[i])
				{
					int XPos = m_fZoom*(m_iItemSpace+ColumnPosItem*(m_iItemSize+m_iItemSpace));
					int YPos = m_fZoom*(m_iItemSpace+RowPosItem*(m_iItemSize+m_iItemTextSize+m_iItemSpace))-ScrollVertical;

					int x,y,w,h;
					x=m_i2MousePosStart.x;
					y=m_i2MousePosStart.y;
					w=xMouse-m_i2MousePosStart.x;
					h=yMouse-m_i2MousePosStart.y;

					if (w<0)
					{
						x=x+w;
						w=-w;
					}

					if (h<0)
					{
						y=y+h;
						h=-h;
					}

					if (XPos>=x && XPos+m_fZoom*m_iItemSize<=x+w &&
						YPos>=y && YPos+m_fZoom*m_iItemSize<=y+h)
					{
						if (m_bKeyShift)
						{
							bool Found=false;
							for (unsigned int j=0;j<m_aSelectedItems.Count();j++)
								if (m_aRessources[i]==m_aSelectedItems[j])
								{
									m_aSelectedItems.Remove(m_aSelectedItems[j]);
									Found=true;
									break;
								}
							if (Found==false)
								m_aSelectedItems.Add(m_aRessources[i]);
						}else
							m_aSelectedItems.Add(m_aRessources[i]);
					}

					ColumnPosItem++;
					if (ColumnPosItem>=ItemByLine)
					{
						ColumnPosItem=0;
						RowPosItem++;
					}
				}
			}
		}else{ //Drag
		}

		m_bMouseLeft=false;
		m_bMouseLeftDrag=0;
		Refresh();
	}

	if (event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))	
	{
		if (m_bMouseMiddle==false)
		{
			m_bMouseMiddle=true;
			m_i2MousePosStart=event.GetPosition();
			SetCursor(wxCURSOR_SIZING);
			TimerMouse.Start(30);
		}
	}else if (m_bMouseMiddle)
	{
		TimerMouse.Stop();
		m_bMouseMiddle=false;
		SetCursor(wxCURSOR_ARROW);
	}

	if (event.ButtonDown(wxMOUSE_BTN_RIGHT))
	{
		wxCoord xMouse, yMouse;
		event.GetPosition(&xMouse,&yMouse);

		bool Over=false;
		for (unsigned int i=StartItem;i<m_aRessources.Count();i++)
		{
			if (m_aRessources[i])
			{
				int XPos = m_fZoom*(m_iItemSpace+ColumnPosItem*(m_iItemSize+m_iItemSpace));
				int YPos = m_fZoom*(m_iItemSpace+RowPosItem*(m_iItemSize+m_iItemTextSize+m_iItemSpace))-ScrollVertical;

				if (XPos<=xMouse && XPos+m_fZoom*m_iItemSize>=xMouse &&
					YPos<=yMouse && YPos+m_fZoom*m_iItemSize>=yMouse)
				{
					//Show menu
					Over=true;
					if (m_aSelectedItems.Index(m_aRessources[i])==-1)
					{
						_ClearSelection();
						m_aSelectedItems.Add(m_aRessources[i]);
					}
					ShowMenu(xMouse,yMouse,m_aSelectedItems);
					break;
				}

				ColumnPosItem++;
				if (ColumnPosItem>=ItemByLine)
				{
					ColumnPosItem=0;
					RowPosItem++;
				}
			}
		}

		if (!Over)
		{
			//printf("Show dir menu\n");
		}
	}


	if (event.GetWheelRotation()!=0) // Zoom
	{
		int sign = (abs(event.GetWheelRotation())/event.GetWheelRotation());
		if (m_bKeyControl)
		{			
			m_fZoomTarget = m_fZoomTarget+sign*m_fZoomTarget/10;
			if (m_fZoomTarget<0.1)
				m_fZoomTarget=0.1f;
			if (m_fZoomTarget>10)
				m_fZoomTarget=10;
			Timer.Start(30);
		}else{
			SetScrollPos(wxVERTICAL,GetScrollPos(wxVERTICAL)-20*event.GetWheelRotation()/event.GetWheelDelta(),false);
		}
		Refresh();
	}

	Post();
}

void RessourcePanel::OnKeyUp(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_SHIFT:
		m_bKeyShift=false;
		break;
	case WXK_CONTROL:
		m_bKeyControl=false;
		break;
	}
}

void RessourcePanel::OnKeyDown(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_SHIFT:
		m_bKeyShift=true;
		break;
	case WXK_CONTROL:
		m_bKeyControl=true;
		break;
	}
}

void RessourcePanel::OnTimer(wxTimerEvent &event)
{
	if (m_fZoom!=m_fZoomTarget)
	{
		float val = (m_fZoomTarget-m_fZoom)/10.0;
		if (abs(val)<0.00001)
		{
			val=0.00001*abs(val)/val;
		}
		m_fZoom+=val;
		if ((val<0 && m_fZoom-m_fZoomTarget<0)
			|| (val>0 && m_fZoom-m_fZoomTarget>0) || !(m_iFlags&FLAG_RP_ZOOM_ANIMATION))
		{
			m_fZoom=m_fZoomTarget;
			Timer.Start(500);
		}
	}
	Refresh();
}

void RessourcePanel::OnTimerMouse(wxTimerEvent &event)
{
	SetScrollPos(wxVERTICAL,GetScrollPos(wxVERTICAL)+(m_i2MousePos-m_i2MousePosStart).y/2.0,false);
	Refresh();
};

void RessourcePanel::ShowMenu(int x, int y, wxRessourceItemArray items)
{
	//printf("Show menu\n");	
	wxMenu *ContextMenu = new wxMenu();
	ContextMenu->Append(0,"Menu");

	PopupMenu(ContextMenu,wxPoint(x,y));
	delete ContextMenu;
}
