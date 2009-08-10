
#include "MainWindowPhazer.h"

#include "Initial/Core/IList.h"

#include "wx/filename.h" // filename support
//#include "wx/dirdlg.h" // filename support

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;

BEGIN_EVENT_TABLE(MainWindowPhazer, wxFrame)
	/*EVT_BUTTON(MW_OPEN_FILE,MainWindowPhazer::OnOpenFile)
	EVT_BUTTON(MW_AUTO,MainWindowPhazer::OnAuto)
	EVT_BUTTON(MW_CONVERT,MainWindowPhazer::OnConvert)
	EVT_RADIOBOX(wxID_ANY,OnRadio)

	EVT_BUTTON(MW_OPEN_FOLDER,MainWindowPhazer::OnOpenFolder)
	EVT_BUTTON(MW_START,MainWindowPhazer::OnStart)
	EVT_BUTTON(MW_REBUILD,MainWindowPhazer::OnStart)*/

	EVT_MENU(wxID_ANY,OnMenu)
END_EVENT_TABLE()

MainWindowPhazer::MainWindowPhazer(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame*) NULL, -1, title, pos, size,wxDEFAULT_FRAME_STYLE/*&~wxRESIZE_BORDER*/)
{
	m_pMaterial=NULL;

	m_mgr.SetManagedWindow(this);
	m_mgr.SetEvtHandlerEnabled(true);

	m_pMenuBar = new wxMenuBar;
	m_pFileMenu = new wxMenu;
	m_pFileMenu->Append(MW_MENU_OPEN_FILE,"Open IMA");
	m_pFileMenu->Append(MW_MENU_SAVE_FILE,"Save IMA");
	m_pMenuBar->Append(m_pFileMenu,"File");
	m_pRenderMenu = new wxMenu;
	m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_DEFAULT,"Default");
	m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_ALBEDO,"Albedo");
	m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_NORMALMAP,"Normal map");
	m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_EMISSIVE,"Emissive");
	m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_SPECULAR,"Specular");
	m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_DEPTH,"Depth");
	m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_BLOOM,"Bloom");
	m_pMenuBar->Append(m_pRenderMenu,"Render");

	SetMenuBar(m_pMenuBar);

	IList<IString> myList;
	myList.PushFront(IString("Test"));
	myList.PushBack(IString("Test2"));
	myList.PushBack(IString("Test3"));

	//IList<IString>::ListNode *node=myList.First();
	
	//for (IList<IString>::ListNode *node=myList.First();node;node=node->GetNext())
	for (IList<IString>::Iterator iterator=myList.Ite();iterator;iterator++)
	{
		printf("%s\n",iterator.GetData().c_str());
	}
	//for (myList::)

	IString test="test";

	printf("%d \n",test=="test1");

	m_pProp = new ObjectProperty(this);
	if (m_pProp)
	{
		m_pProp->SetSize(300,150);
		m_mgr.AddPane(m_pProp,wxAuiPaneInfo().
			Name(wxT("Properties")).Caption(wxT("Properties")).
			Left().Bottom().BestSize(300,150).
			CloseButton(false).MaximizeButton(true).
			Dockable(true));
	}

	m_pGraph = new MaterialGraph(this);
	if (m_pGraph)
	{
		m_mgr.AddPane(m_pGraph,wxAuiPaneInfo().
			Name(wxT("Graph")).Caption(wxT("Graph")).
			Center().
			CloseButton(false).MaximizeButton(true).
			Dockable(true));

		m_pGraph->LinkProperty(m_pProp);
	}

	m_pInitial = new InitialPanel(this,wxID_ANY,wxPoint(0,0),wxSize(640,480));
	if (m_pInitial)
	{
		m_mgr.AddPane(m_pInitial,wxAuiPaneInfo().
			Name(wxT("Preview")).Caption(wxT("Preview")).
			Left().
			CloseButton(false).MaximizeButton(true).
			Dockable(true));

		Initial::IDevice *m_pDevice = m_pInitial->GetDevice();

		//m_pDevice->GetRenderDriver()->SetOutputTexture(1);

		INodeLight *light=NULL;
		light = (INodeLight*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeLight");
		if (light)
		{
			//light->SetColor(IColor(0.6f,0.6f,0.48f));
			light->SetRadius(500);
			light->SetPosition(-10,10,10);
			//light->SetProjectedTexture(m_pInitial->GetDevice()->GetRessourceManager()->LoadTexture("brick.itx"));
		}
		printf("Light %d\n",light);

		Initial::IMesh *_3ds;
		_3ds = I3DLoad3DS::Load3DS("marcus.3ds",m_pDevice->GetRenderDriver());
		//_3ds = I3DLoad3DS::Load3DS("car.3ds",m_pDevice->GetRenderDriver());
		//_3ds = I3DLoad3DS::Load3DS("teapot.3ds",m_pDevice->GetRenderDriver());

		m_pMaterial = m_pDevice->GetRessourceManager()->CreateNewMaterial("Phazer");
		//m_pMaterial = m_pDevice->GetRessourceManager()->LoadMaterial("materials/testV2.ima");

		if (_3ds)
		{
			_3ds->SetScale(0.02);
			_3ds->RotateOrigX(DEG_TO_RAD(-90));
			//_3ds->RotateOrigZ(DEG_TO_RAD(180));
			_3ds->Translate(0,0,-3);
			//_3ds->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/marcus.ima"));
			//_3ds->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/testV2.ima"));
			_3ds->SetMaterial(m_pMaterial);			
		}

		m_pGraph->LinkMaterial(m_pMaterial);

		/*for (UINT i=0;i<IObject::ObjectsClass.Count();i++)
		{
			printf("%s\n",IObject::ObjectsClass[i]->GetName());
		}*/
	}

	m_pStatusBar = new wxStatusBar(this,wxID_ANY,0);
	if (m_pStatusBar)
	{
		SetStatusBar(m_pStatusBar);
	}

	m_mgr.Update();

	Center();
	CreateMenu(); 
	//SetIcon(wxIcon("IDI_ICON"));
}

MainWindowPhazer::~MainWindowPhazer()
{
	m_mgr.UnInit();
}

void MainWindowPhazer::OnMenu(wxCommandEvent& event)
{
	if (event.GetId()>=MW_MENU_RENDER_DEFAULT && event.GetId()<=MW_MENU_RENDER_BLOOM)
	{
		m_pInitial->GetDevice()->GetRenderDriver()->SetOutputTexture(event.GetId()-MW_MENU_RENDER_DEFAULT);
	}

	if (event.GetId()==MW_MENU_OPEN_FILE)
	{
		if (m_pMaterial)
		{		
			m_pInitial->Stop();
			wxFileDialog dialog(this, wxT("Select material file to open"), wxT(""), wxT(""), "Initial material (*.IMA)|*.ima", wxOPEN);
			if (dialog.ShowModal() == wxID_OK )	
			{
				m_pGraph->LinkMaterial(NULL);
				if (!m_pMaterial->LoadV2(dialog.GetPath().c_str()))
				{
					wxLogError("Error on material loading");					
				}
				m_pGraph->LinkMaterial(m_pMaterial);
			}
			m_pInitial->Start();
		}
	}

	if (event.GetId()==MW_MENU_SAVE_FILE)
	{
		if (m_pMaterial)
		{
			m_pInitial->Stop();
			wxFileDialog dialog(this, wxT("Select material file to save"), wxT(""), wxT(""), "Initial material (*.IMA)|*.ima", wxSAVE);
			if (dialog.ShowModal() == wxID_OK )		
			{
				if (!m_pMaterial->SaveV2(dialog.GetPath().c_str()))
					wxLogError("Error on material saving");
			}
			m_pInitial->Start();
		}
	}

	if (event.GetId()>=MW_MENU_RENDER_DEFAULT && event.GetId()<=MW_MENU_RENDER_BLOOM)
	{
		m_pInitial->GetDevice()->GetRenderDriver()->SetOutputTexture(event.GetId()-MW_MENU_RENDER_DEFAULT);
	}
}
