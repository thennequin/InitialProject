
#ifndef _MAINWINDOW_HEADER_
#define _MAINWINDOW_HEADER_

#include <wx/wx.h>

#include "wx/checkbox.h"
#include "wx/radiobox.h"
#include "wx/textctrl.h"
#include "wx/bookctrl.h"
#include "wx/spinctrl.h"

#include "../Frames/GraphWidget.h"

class MainWindow : public wxFrame
{
	DECLARE_EVENT_TABLE()

	enum
	{
		//Single File
		MW_OPEN_FILE = wxID_HIGHEST+1,
		MW_AUTO,
		MW_CONVERT,

		//Multi file
		MW_OPEN_FOLDER,
		MW_START,
		MW_REBUILD,
	};
public:
	MainWindow(const wxString& title, const wxPoint& Pos=wxDefaultPosition, const wxSize& size=wxSize(430,350+400));
	~MainWindow();	

	void OnOpenFile(wxCommandEvent& event);
	void OnAuto(wxCommandEvent& event);
	void OnConvert(wxCommandEvent& event);
	void OnRadio(wxCommandEvent& event);

	void OnOpenFolder(wxCommandEvent& event);
	void OnStart(wxCommandEvent& event);

	void OnConvertFinish();

	wxString GetImageFormat();

	GraphWidget *GetGraph() { return m_pGraph; }

	wxGauge		*GetGauge() { return m_pGauge; }

	void AddDetailText(wxString detail);

	//bool UpdateSettings(nvtt::Compressor& compressor, nvtt::InputOptions& inputOptions, nvtt::OutputOptions& outputOptions, nvtt::CompressionOptions& compressionOptions,wxString& outputfile,bool verbose);
protected:
	wxNotebook	*m_pNotebook;

	wxPanel		*m_pPageSingle;
		wxTextCtrl	*m_pTC_InputFile;
		wxButton	*m_pB_OpenFile;
		wxButton	*m_pB_Start;
		wxButton	*m_pB_Auto;
		wxCheckBox	*m_pCB_CUDA;
		wxCheckBox	*m_pCB_Mipmaps;
		wxRadioBox	*m_pRB_ImageType;
		wxRadioBox	*m_pRB_BorderType;
		wxRadioBox	*m_pRB_OutputQuality;
		wxRadioBox	*m_pRB_OutputType;
		wxGauge		*m_pGauge;
		wxStatusBar	*m_pStatusBar;

	wxPanel		*m_pPageMulti;
		wxPanel		*m_pPanelFolder;
			wxTextCtrl	*m_pTC_InputFolder;
			wxButton	*m_pB_OpenFolder;
			wxRadioBox	*m_pRB_OutputQualityMult;
			wxCheckBox	*m_pCB_CUDAMult;
			wxCheckBox	*m_pCB_MipmapsMult;
			wxButton	*m_pB_Starts;
			wxButton	*m_pB_Rebuild;
			wxSpinCtrl	*m_pSC_ThreadCount;
		wxNotebook	*m_pNoteBookGraph;
			GraphWidget *m_pGraph;
			wxTextCtrl	*m_pTCDetail;

	wxSemaphore m_sDetailAccess;
};

#endif