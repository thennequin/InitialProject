
#ifndef _RESSOURCEPANEL_HEADER_
#define _RESSOURCEPANEL_HEADER_

#include <wx/wx.h>
#include <wx/filesys.h>
#include <wx/dragimag.h>

//Flags
enum
{
	FLAG_RP_MULTIPLE_SELECTION = 1<<1,
	FLAG_RP_SELECTOR = 1<<2,
	FLAG_RP_ZOOM_ANIMATION = 1<<3,
};

class RessourceItem
{
	friend class RessourcePanel;
public:
	enum RessourceType
	{
		RT_Folder = 0,
		RT_Image = 1<<0,
		RT_Material = 1<<1,
		RT_Mesh = 1<<2,
		RT_Other = 1<<3,		
		
		RT_All = RT_Image | RT_Material | RT_Mesh | RT_Other
	};
public:
	wxString GetPath();
	wxString GetFilename();
	wxString GetExt();
	RessourceType GetType();
	wxImage& GetThumb();
	wxColour GetTypeColour();
	wxString GetInfo(int i);
	int GetInfoCount();
protected:
	static wxImage _CreateThumb(wxString file);
protected:
	wxString m_sPath;
	RessourceType m_iType;
	wxImage m_iThumb;
	wxImage m_iThumbTemp;
	bool m_bThumbOk;
};

WX_DEFINE_ARRAY(RessourceItem*,wxRessourceItemArray);

//DECLARE_EVENT_TYPE(wxEVT_RP_ITEM_SELECTED, 1051+1)
extern const wxEventType SelectedRessourceEvent;

class RessourcePanel : public wxScrolledWindow
{
	class LoadingThread : public wxThread
	{
	public:
		LoadingThread(RessourcePanel *panel);
		virtual void *Entry();
	protected:
		RessourcePanel *m_pPanel;
	};

	friend class LoadingThread;

	enum
	{
		RPID_TIMER = wxID_HIGHEST+1,
		RPID_TIMERMOUSE
	};
	DECLARE_EVENT_TABLE()
public:
public:
	RessourcePanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
			unsigned int flags = FLAG_RP_MULTIPLE_SELECTION/*|FLAG_RP_ZOOM_ANIMATION*/);
	~RessourcePanel();

	void SetDirectory(wxString dir);
	void SetRessourceType(RessourceItem::RessourceType type = RessourceItem::RT_All);
	void RefreshRessourcesList();

	wxRessourceItemArray GetSelection();

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event) { } // Evite le scintillement

	void OnMouseEvent(wxMouseEvent &event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);

	void OnTimer(wxTimerEvent &event);
	void OnTimerMouse(wxTimerEvent &event);

	void ShowMenu(int x, int y, wxRessourceItemArray items);
protected:
	void _Clear();
	void _ClearSelection();

	void Post();
	void Wait();
protected:
	unsigned int m_iFlags;
	wxTimer Timer;
	wxTimer TimerMouse;

	wxFileSystem m_FS;
	wxRessourceItemArray m_aRessources;
	int m_iRessourceType;

	float m_fZoom;
	float m_fZoomTarget;
	int m_iItemSize;
	int m_iItemSpace;
	int m_iItemTextSize;

	bool m_bKeyShift;
	bool m_bKeyControl;

	wxPoint m_i2MousePosStart;
	wxPoint m_i2MousePos;
	bool m_bMouseMiddle;
	bool m_bMouseLeft;
	int m_bMouseLeftDrag; //0=Nothing, 1=Area selection, 2=Drag
	bool m_bMouseLeftCanDrag;

	long m_iLastMove;

	wxRessourceItemArray m_aSelectedItems;

	static wxSemaphore m_sLoadingAcess;
	static bool m_bLoadingAcessInit;

	bool m_bLoadingAcessInstance;
	unsigned int m_iPreviewPriorityItem;

	LoadingThread *m_pLoadingThread;
};

#endif