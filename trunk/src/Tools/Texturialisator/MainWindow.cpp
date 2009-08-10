
#include "MainWindow.h"

#include "wx/filename.h" // filename support
//#include "wx/dirdlg.h" // filename support

//#include <nvcore/StrLib.h>
//#include <nvcore/StdStream.h>

//#include <nvimage/Image.h>
//#include <nvimage/DirectDrawSurface.h>

//#include "GaugeOutputHandler.h"

#include "Initial/Initial.h"

#include "FolderScanThread.h"
#include "ConvertThread.h"

#include "RessourcePanel.h"
#include "FlowLib/FlowGraph.h"
#include "MaterialGraph.h"

#include "Initial/Video/IMaterialExpression.h"

//#include "ITXFormat.h"

const int Control_Height = 20;

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_BUTTON(MW_OPEN_FILE,MainWindow::OnOpenFile)
	EVT_BUTTON(MW_AUTO,MainWindow::OnAuto)
	EVT_BUTTON(MW_CONVERT,MainWindow::OnConvert)
	EVT_RADIOBOX(wxID_ANY,OnRadio)

	EVT_BUTTON(MW_OPEN_FOLDER,MainWindow::OnOpenFolder)
	EVT_BUTTON(MW_START,MainWindow::OnStart)
	EVT_BUTTON(MW_REBUILD,MainWindow::OnStart)
END_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
 : wxFrame((wxFrame*) NULL, -1, title, pos, size,wxDEFAULT_FRAME_STYLE/*&~wxRESIZE_BORDER*/)
{
	m_pNotebook = new wxNotebook(this,wxID_ANY);
	
	//Single file
		wxString ImageType_str[2];
		ImageType_str[0] = "Color";
		ImageType_str[1] = "Normal";

		wxString BorderType_str[3];
		BorderType_str[0] = "Repeat";
		BorderType_str[1] = "Clamp";
		BorderType_str[2] = "Mirror";

		wxString OutputQuality_str[4];
		OutputQuality_str[0] = "Fast";
		OutputQuality_str[1] = "Normal";
		OutputQuality_str[2] = "Production";
		OutputQuality_str[3] = "Heighest";

		wxString OutputFormat_str[9];
		OutputFormat_str[0] = "RGBA (no compressed)";
		OutputFormat_str[1] = "DXT1 (RGB)";
		OutputFormat_str[2] = "DXT1nm (normal map)";
		OutputFormat_str[3] = "DXT1a (RGBA, binary alpha)";
		OutputFormat_str[4] = "DXT3";
		OutputFormat_str[5] = "DXT5 (RGBA)";
		OutputFormat_str[6] = "DXT5nm (normal map with alpha)";
		OutputFormat_str[7] = "ATI1";
		OutputFormat_str[8] = "ATI2/3Dc";

	m_pPageSingle = new wxPanel(m_pNotebook);
	m_pNotebook->AddPage(m_pPageSingle,"Single file",true);

		m_pTC_InputFile = new wxTextCtrl(m_pPageSingle,wxID_ANY,wxEmptyString,wxPoint(5,10+Control_Height*0),wxSize(300,-1),wxTE_READONLY);
		m_pTC_InputFile->SetValue("c:\\marcus.tga");

		m_pB_OpenFile = new wxButton(m_pPageSingle,MW_OPEN_FILE,"Open image",wxPoint(5+300+5,10+Control_Height*0),wxSize(100,-1));

		m_pB_Start = new wxButton(m_pPageSingle,MW_CONVERT,"Convert",wxPoint(5+300+5,10+Control_Height*1.5),wxSize(100,Control_Height*2));

		m_pB_Auto = new wxButton(m_pPageSingle,MW_AUTO,"Auto",wxPoint(5+195+5,10+Control_Height*1.5),wxSize(100,Control_Height*1));
		
		m_pCB_CUDA = new wxCheckBox(m_pPageSingle,wxID_ANY,"Use  GPU acceleration (CUDA)",wxPoint(5,10+Control_Height*1.5));
		/*nvtt::Compressor compressor;
		compressor.enableCudaAcceleration(true);
		if (!compressor.isCudaAccelerationEnabled())
			m_pCB_CUDA->Disable();*/

		m_pCB_Mipmaps = new wxCheckBox(m_pPageSingle,wxID_ANY,"No mipmaps",wxPoint(5,10+Control_Height*2.5));

		int Pos=Control_Height*3.5;
		int height=Control_Height*(2+1);
		m_pRB_ImageType = new wxRadioBox(m_pPageSingle,wxID_ANY,"Image type",wxPoint(5,10+Pos),wxSize(100,height),2,ImageType_str,0,wxVERTICAL);

		height=Control_Height*(2+1);
		m_pRB_BorderType = new wxRadioBox(m_pPageSingle,wxID_ANY,"Border type",wxPoint(5+5+100,10+Pos),wxSize(130,height),3,BorderType_str,2,wxVERTICAL);

		m_pRB_OutputQuality = new wxRadioBox(m_pPageSingle,wxID_ANY,"Quality",wxPoint(5+5+100+5+100+30,10+Pos),wxSize(160,height),4,OutputQuality_str,2,wxVERTICAL);
		Pos+=height;

		height=Control_Height*(5+1);
		m_pRB_OutputType = new wxRadioBox(m_pPageSingle,wxID_ANY,"Output format",wxPoint(5,10+Pos),wxSize(400,height),9,OutputFormat_str,5,wxVERTICAL);
		m_pRB_OutputType->Select(1); //DXT1 by default
		Pos+=height+5;

		m_pGauge = new wxGauge(m_pPageSingle,wxID_ANY,100,wxPoint(5,10+Pos),wxSize(400,20));

	//Multi file
	m_pPageMulti = new wxPanel(m_pNotebook);
	m_pNotebook->AddPage(m_pPageMulti,"Update files",false);

		m_pPanelFolder = new wxPanel(m_pPageMulti,0,0,400,5+22+5+20+5+60+5+40+5);
		m_pTC_InputFolder = new wxTextCtrl(m_pPanelFolder,wxID_ANY,wxEmptyString,wxPoint(5,5),wxSize(200,-1),wxTE_READONLY);
		m_pB_OpenFolder = new wxButton(m_pPanelFolder,MW_OPEN_FOLDER,"Open folder",wxPoint(5+200+5,5),wxSize(100,-1));

		m_pCB_CUDAMult = new wxCheckBox(m_pPanelFolder,wxID_ANY,"Use  GPU acceleration (CUDA)",wxPoint(5,5+22+5));
		//if (!compressor.isCudaAccelerationEnabled())
		//	m_pCB_CUDAMult->Disable();

		//m_pCB_MipmapsMult = new wxCheckBox(m_pPanelFolder,wxID_ANY,"No mipmaps",wxPoint(200+5,5+22+5));

		m_pRB_OutputQualityMult = new wxRadioBox(m_pPanelFolder,wxID_ANY,"Quality",wxPoint(5,5+22+5+20+5),wxSize(190,60),4,OutputQuality_str,2,wxVERTICAL);

		new wxStaticText(m_pPanelFolder,wxID_ANY,"Agent count",wxPoint(5+190+5,5+22+5+20+5+5));
		m_pSC_ThreadCount = new wxSpinCtrl(m_pPanelFolder,wxID_ANY,"2",wxPoint(5+190+5,5+22+5+20+5+25),wxSize(100,-1),4096,1,100);

		m_pB_Starts = new wxButton(m_pPanelFolder,MW_START,"Start",wxPoint(5,5+22+5+20+5+60+5),wxSize(180,40));
		m_pB_Rebuild = new wxButton(m_pPanelFolder,MW_REBUILD,"Rebuild",wxPoint(5+185,5+22+5+20+5+60+5),wxSize(180,40));

		m_pNoteBookGraph = new wxNotebook(m_pPageMulti,wxID_ANY,wxDefaultPosition,wxSize(400,200),wxNB_LEFT);
		m_pGraph = new GraphWidget(m_pNoteBookGraph,wxID_ANY);
		m_pTCDetail = new wxTextCtrl(m_pNoteBookGraph,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY|wxTE_MULTILINE);
		m_pNoteBookGraph->AddPage(m_pGraph,"Graph");
		m_pNoteBookGraph->AddPage(m_pTCDetail,"Detail");

		RessourcePanel *ressources = new RessourcePanel(m_pNoteBookGraph);
		m_pNoteBookGraph->AddPage(ressources,"Ressources");

		MaterialGraph *graph = new MaterialGraph(m_pNoteBookGraph);
		//FlowGraph *graph = new FlowGraph(m_pNoteBookGraph);
		m_pNoteBookGraph->AddPage(graph,"Graph");

	wxBoxSizer *sizerMulti = new wxBoxSizer(wxVERTICAL);
	sizerMulti->Add(m_pPanelFolder);
	sizerMulti->Add(m_pNoteBookGraph,1,wxEXPAND);

	m_sDetailAccess.Post();
	
	m_pPageMulti->SetSizer(sizerMulti);

	m_pStatusBar = new wxStatusBar(this,wxID_ANY,0);
	if (m_pStatusBar)
	{
		SetStatusBar(m_pStatusBar);
	}
}

MainWindow::~MainWindow()
{

}

void MainWindow::OnOpenFile(wxCommandEvent& event)
{
	wxFileDialog *dialog = new wxFileDialog(this, wxT("Select image file"), wxT(""), wxT(""), GetImageFormat(), wxOPEN);
	if (dialog)
	{
		dialog->ShowModal();
		m_pTC_InputFile->SetValue(dialog->GetPath());
	}
}

void MainWindow::OnAuto(wxCommandEvent& event)
{
	wxImage image(m_pTC_InputFile->GetValue());
	if (!image.IsOk())
	{
		wxLogMessage("The file is not a supported image type.");
		return;
	}

	int alphaType=0; // 0 = opaque, 1 = binary, 2 = gradient
	bool normalMap=false;

	//wxLogMessage("start");
	if (image.HasAlpha())
	{
		alphaType=1;
	}

	for (int x=0;x<image.GetWidth() && (alphaType==1 || normalMap==true);x++)
	{
		for (int y=0;y<image.GetHeight() && (alphaType==1 || normalMap==true);y++)
		{
			if (image.HasAlpha())
			{
				unsigned char alpha = image.GetAlpha()[y*image.GetWidth()+x];
				if (!(alpha==0 || alpha==255))
					alphaType=2;
			}

			/*float r = image.GetData()[3*(y*image.GetWidth()+x)+0];
			float g = image.GetData()[3*(y*image.GetWidth()+x)+1];
			float b = image.GetData()[3*(y*image.GetWidth()+x)+2];
			//if (r==g && r==b && g==b && (r==0 || r==255))
			{
				r=(r-128)*2.0/255.0;
				g=(g-128)*2.0/255.0;
				b=(b-128)*2.0/255.0;
				float val=sqrt(r*r+g*g+b*b);
				printf("%f\n",val);

				//if (val/(256.0*256.0)<0.75)
				//if (val<0.95)
					//normalMap=false;
			}*/
		}
	}

	int sel=1;
	if (alphaType==0)
	{
		if (normalMap)
			sel=2;
		else
			sel=1;
	}else if (alphaType==1)
	{
		if (normalMap)
			sel=3;
		else
			sel=3;
	}else{
		if (normalMap)
			sel=6;
		else
			sel=5;
	}

	m_pRB_ImageType->SetSelection(normalMap);
	m_pRB_OutputType->SetSelection(sel);
}

void MainWindow::OnConvert(wxCommandEvent& event)
{
	wxFileName filename(m_pTC_InputFile->GetValue());
	filename.ClearExt();
	wxString outputFile = filename.GetFullPath()+".itx";

	/*switch (m_pRB_OutputType->GetSelection())
	{
	case 1: // DXT1
		format=nvtt::Format_DXT1;
		break;
	case 2: // DXT1nm
		format=nvtt::Format_DXT1;
		compressionOptions.setColorWeights(1, 1, 0);
		break;
	case 3: // DXT1a
		format=nvtt::Format_DXT1a;
		break;
	case 4: // DXT3
		format=nvtt::Format_DXT3;
		break;
	case 5: // DXT5
		format=nvtt::Format_DXT5;
		break;
	case 6: // DXT5nm
		format=nvtt::Format_DXT5n;
		break;
	case 7: // ATI1
		format=nvtt::Format_BC4;
		break;
	case 8: // ATI2/3Dc
		format=nvtt::Format_BC5;
		break;
	};*/

	Initial::Format::IImageITX itx;
	Initial::Format::IImageITX::ITXParameter params;
	params.border = (Initial::Format::IImageITX::BorderType)(Initial::Format::IImageITX::BT_CLAMP + m_pRB_BorderType->GetSelection());
	params.format;
	params.GPUacce = m_pCB_CUDA->GetValue();
	params.mipmaps = m_pCB_Mipmaps->GetValue();
	params.output;
	params.quality;
	params.type;
	params.zipped=false;

	m_pCB_CUDA->GetValue();
	m_pCB_Mipmaps->GetValue();
	m_pRB_ImageType->GetSelection();
	m_pRB_BorderType->GetSelection();
	m_pRB_OutputQuality->GetSelection();
	m_pRB_OutputType->GetSelection();

	if (itx.CreateFromImage(m_pTC_InputFile->GetValue().c_str(),params))
	{
		itx.Save(outputFile.c_str());
		wxLogMessage("File converted");
	}else
		wxLogMessage("Error");
	/*if (ConvertToITX(m_pTC_InputFile->GetValue(),outputFile,(ImageType)(IT_COLOR+m_pRB_ImageType->GetSelection()),(BorderType)(BT_MIRROR+m_pRB_BorderType->GetSelection()),(DXTFormat)m_pRB_OutputType->GetSelection(),!m_pCB_Mipmaps->GetValue(),m_pCB_CUDA->GetValue(),false,m_pRB_OutputQuality->GetSelection()))
		wxLogMessage("File converted");
	else
		wxLogMessage("Error");*/
}

void MainWindow::OnRadio(wxCommandEvent& event)
{
	/*nvtt::Compressor compressor;
	nvtt::InputOptions inputOptions;
	nvtt::OutputOptions outputOptions;
	nvtt::CompressionOptions compressionOptions;
	wxString outputFile;*/

	/*if (UpdateSettings(compressor,inputOptions,outputOptions,compressionOptions,outputFile,false))
	{
		int size = compressor.estimateSize(inputOptions, compressionOptions);

		wxString text;
		text.Printf("Estimate size : %d bytes",size);
		m_pStatusBar->SetStatusText(text);
	}*/
}

void MainWindow::OnOpenFolder(wxCommandEvent& event)
{
	wxDirDialog *dialog = new wxDirDialog(this, wxT("Select folder"),m_pTC_InputFolder->GetValue());
	if (dialog)
	{
		dialog->ShowModal();
		m_pTC_InputFolder->SetValue(dialog->GetPath()+"\\");
	}
}

void MainWindow::OnStart(wxCommandEvent& event)
{
	m_pB_Starts->SetLabel("Stop");
	int quality=m_pRB_OutputQualityMult->GetSelection();
	//bool mipmaps=!m_pCB_MipmapsMult->GetValue();
	bool cuda=m_pCB_CUDAMult->GetValue();
	m_pGraph->Clear();
	m_pGraph->ResetStartTime();
	m_pTCDetail->SetValue("");
	FolderScanThread *scanThread = new FolderScanThread(m_pTC_InputFolder->GetValue());
	if (scanThread->Start())
	{
		for (int i=1;i<=m_pSC_ThreadCount->GetValue();i++)
		{
			ConvertThread *thread = new ConvertThread(i,cuda,quality,event.GetId()==MW_REBUILD);
			thread->Start();
		}
	}else
		wxLogError("Erreur lors de la création des thread");
}

wxString MainWindow::GetImageFormat()
{
	wxString str;
	wxString formats;

    // TODO: This section may require locking (using global).

    wxList& handlers = wxImage::GetHandlers();

    wxList::iterator node;

    // Let's iterate over the image handler list.
    //for ( wxList::Node *node = handlers.GetFirst(); node; node = node->GetNext() )
    for ( node = handlers.begin(); node != handlers.end(); node++ )
    {
        wxImageHandler *handler = (wxImageHandler*)*node;

        wxString ext_lo = handler->GetExtension();
        wxString ext_up = ext_lo.Upper();

        str.append( ext_up );
        str.append( wxT(" files (*.") );
        str.append( ext_up );
        str.append( wxT(")|*.") );
        str.append( ext_lo );
		if (node != handlers.end())
			str.append( wxT("|") );
		formats.append( "*."+ext_lo + ";");
    }

	str=wxT("All images format (*.*)|"+formats)+"|"+str;
	//str="*.*|*.*";

	return str;
}

/*bool MainWindow::UpdateSettings(nvtt::Compressor& compressor, nvtt::InputOptions& inputOptions, nvtt::OutputOptions& outputOptions, nvtt::CompressionOptions& compressionOptions,wxString& outputfile,bool verbose)
{
	wxImage image(m_pTC_InputFile->GetValue());
	if (!image.IsOk())
	{
		if (verbose)
			wxLogMessage("The file is not a supported image type.");
		return false;
	}

	outputfile=m_pTC_InputFile->GetValue()+".dds";
	wxFileName filename(m_pTC_InputFile->GetValue());
	filename.ClearExt();
	outputfile=filename.GetFullPath()+".dds";
	//outputOptions.setFileName(m_pTC_InputFile->GetValue()+".dds");

	// CUDA
	compressor.enableCudaAcceleration(m_pCB_CUDA->GetValue());

	//Mipmaps
	inputOptions.setMipmapGeneration(!m_pCB_Mipmaps->GetValue());

	//Image type
	inputOptions.setConvertToNormalMap(false);
	if (m_pRB_ImageType->GetSelection()==0) // Color
	{
		inputOptions.setNormalMap(false);		
		inputOptions.setGamma(2.2f, 2.2f);
		inputOptions.setNormalizeMipmaps(false);
	}else{ // Normal map
		inputOptions.setNormalMap(true);		
		inputOptions.setGamma(1.0f, 1.0f);
		inputOptions.setNormalizeMipmaps(true);
	}	

	//Border
	nvtt::WrapMode wrap=nvtt::WrapMode_Repeat;
	if (m_pRB_BorderType->GetSelection()==1)
		wrap=nvtt::WrapMode_Clamp;
	inputOptions.setWrapMode(wrap);

	//Qaulity
	compressionOptions.setQuality((nvtt::Quality)((int)nvtt::Quality_Fastest + m_pRB_OutputQuality->GetSelection()));
	
	//Output format
	nvtt::Format format=nvtt::Format_RGB;
	switch (m_pRB_OutputType->GetSelection())
	{
	case 1: // DXT1
		format=nvtt::Format_DXT1;
		break;
	case 2: // DXT1nm
		format=nvtt::Format_DXT1;
		compressionOptions.setColorWeights(1, 1, 0);
		break;
	case 3: // DXT1a
		format=nvtt::Format_DXT1a;
		break;
	case 4: // DXT3
		format=nvtt::Format_DXT3;
		break;
	case 5: // DXT5
		format=nvtt::Format_DXT5;
		break;
	case 6: // DXT5nm
		format=nvtt::Format_DXT5n;
		break;
	case 7: // ATI1
		format=nvtt::Format_BC4;
		break;
	case 8: // ATI2/3Dc
		format=nvtt::Format_BC5;
		break;
	};

	compressionOptions.setFormat(format);

	return true;
}*/


void MainWindow::AddDetailText(wxString detail)
{
	m_sDetailAccess.Wait();
	m_pTCDetail->SetValue(m_pTCDetail->GetValue()+detail);
	m_sDetailAccess.Post();
}

void MainWindow::OnConvertFinish()
{
	m_pB_Starts->SetLabel("Start");
}
