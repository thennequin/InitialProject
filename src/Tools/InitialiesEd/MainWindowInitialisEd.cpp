
#include "MainWindowInitialisEd.h"

#include "Initial/Node/INodeText.h"

#include "wx/filename.h" // filename support
//#include "wx/dirdlg.h" // filename support

#include <vector>

BEGIN_EVENT_TABLE(MainWindowInitialisEd, wxFrame)
	EVT_CLOSE(MainWindowInitialisEd::OnClose)

	//Menu file
	EVT_MENU(MW_MENU_OPEN_FILE, MainWindowInitialisEd::OnMenu)
	EVT_MENU(MW_MENU_SAVE_FILE, MainWindowInitialisEd::OnMenu)
	//Menu render
	EVT_MENU(MW_MENU_RENDER_DEFAULT, MainWindowInitialisEd::OnMenu)
	EVT_MENU(MW_MENU_RENDER_ALBEDO, MainWindowInitialisEd::OnMenu)
	EVT_MENU(MW_MENU_RENDER_NORMALMAP, MainWindowInitialisEd::OnMenu)
	EVT_MENU(MW_MENU_RENDER_EMISSIVE, MainWindowInitialisEd::OnMenu)
	EVT_MENU(MW_MENU_RENDER_SPECULAR, MainWindowInitialisEd::OnMenu)
	EVT_MENU(MW_MENU_RENDER_DEPTH, MainWindowInitialisEd::OnMenu)
	EVT_MENU(MW_MENU_RENDER_BLOOM, MainWindowInitialisEd::OnMenu)

	//Toolbar
	EVT_MENU(MW_TB_SELECT, MainWindowInitialisEd::OnTool)
	EVT_MENU(MW_TB_MOVE, MainWindowInitialisEd::OnTool)
	EVT_MENU(MW_TB_ROTATE, MainWindowInitialisEd::OnTool)
	EVT_MENU(MW_TB_SCALE, MainWindowInitialisEd::OnTool)
	//wxEVT_COMMAND_CHOICE_SELECTED
	EVT_CHOICE(MW_TB_TRANS_MODE, MainWindowInitialisEd::OnTrandModeChoice)
END_EVENT_TABLE()

using namespace Initial;
using namespace Initial::Core;

MainWindowInitialisEd::MainWindowInitialisEd(const wxString& title, const wxPoint& pos, const wxSize& size)
 : wxFrame((wxFrame*) NULL, -1, title, pos, size,wxDEFAULT_FRAME_STYLE/*&~wxRESIZE_BORDER*/)
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetEvtHandlerEnabled(true);

	m_pMenuBar = new wxMenuBar;
	m_pFileMenu = new wxMenu;
		m_pFileMenu->Append(MW_MENU_OPEN_FILE,"Open IEM");
		m_pFileMenu->Append(MW_MENU_SAVE_FILE,"Save IEM");
	m_pMenuBar->Append(m_pFileMenu,"File");
	m_pRenderMenu = new wxMenu;
		m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_DEFAULT,"Default");
		m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_ALBEDO,"Albedo");
		m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_NORMALMAP,"Normal map");
		m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_EMISSIVE,"Emissive");
		m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_SPECULAR,"Specular");
		m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_DEPTH,"Depth");
		m_pRenderMenu->AppendRadioItem(MW_MENU_RENDER_BLOOM,"Bloom");
		m_pRenderMenu->AppendSeparator();
		m_pRenderMenu->AppendCheckItem(wxID_ANY,"Show Bounding box");
		m_pRenderMenu->AppendCheckItem(wxID_ANY,"Show bones");
	m_pMenuBar->Append(m_pRenderMenu,"Render");

	SetMenuBar(m_pMenuBar);

	m_pToolbar = new wxAuiToolBar(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxAUI_TB_OVERFLOW);
	if (m_pToolbar)
	{
		m_pToolbar->SetToolBitmapSize(wxSize(22,22));
		//m_pToolbar->AddTool(0,"Open",wxBitmap("res/open.bmp",wxBITMAP_TYPE_BMP));
		//m_pToolbar->AddTool(0,"Save",wxBitmap("res/save.bmp",wxBITMAP_TYPE_BMP));
		m_pToolbar->AddTool(MW_TB_SELECT,"Select",wxBitmap("res/TBSelect.bmp",wxBITMAP_TYPE_BMP),wxEmptyString,wxITEM_CHECK);
		m_pToolbar->AddTool(MW_TB_MOVE,"Move",wxBitmap("res/TBMove.bmp",wxBITMAP_TYPE_BMP),wxEmptyString,wxITEM_CHECK);
		m_pToolbar->AddTool(MW_TB_ROTATE,"Rotate",wxBitmap("res/TBRotate.bmp",wxBITMAP_TYPE_BMP),wxEmptyString,wxITEM_CHECK);
		m_pToolbar->AddTool(MW_TB_SCALE,"Scale",wxBitmap("res/TBScale.bmp",wxBITMAP_TYPE_BMP),wxEmptyString,wxITEM_CHECK);
		wxArrayString choices;
		choices.Add("World");
		choices.Add("Local");
		choices.Add("View");
		m_pToolbarMode = new wxChoice(m_pToolbar,MW_TB_TRANS_MODE,wxDefaultPosition,wxSize(100,-1),choices);
		m_pToolbarMode->SetSelection(0);
		m_pToolbar->AddControl(m_pToolbarMode);
		m_pToolbar->AddSeparator();
		m_pToolbar->AddTool(MW_TB_MAGNET,"Magnet",wxBitmap("res/TBMagnet.bmp",wxBITMAP_TYPE_BMP),wxEmptyString,wxITEM_CHECK);
		m_pToolbar->Realize();
		m_pToolbar->ToggleTool(MW_TB_SELECT,true);		

		m_mgr.AddPane(m_pToolbar,wxAuiPaneInfo().
			Name(wxT("Toolbar")).Caption(wxT("Toolbar")).
			ToolbarPane().Top().
			CloseButton(false).MaximizeButton(true).
			Dockable(true));
	}

	m_pProp = new ObjectProperty(this);
	if (m_pProp)
	{
		m_pProp->SetSize(300,150);
		m_mgr.AddPane(m_pProp,wxAuiPaneInfo().
			Name(wxT("Properties")).Caption(wxT("Properties")).
			Bottom().BestSize(300,150).
			CloseButton(false).MaximizeButton(true).
			Dockable(true));
	}

	m_pInitial = new InitialPanel(this,wxID_ANY,wxPoint(0,0),wxSize(640,480));
	if (m_pInitial)
	{
		m_mgr.AddPane(m_pInitial,wxAuiPaneInfo().
			Name(wxT("Preview")).Caption(wxT("Preview")).
			Center().
			CloseButton(false).MaximizeButton(true).
			Dockable(true));

		Initial::IDevice *m_pDevice = m_pInitial->GetDevice();

		INodeLight *light=NULL;

		light = (INodeLight*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeLight");
		light->SetColor(IColor(1.0,0.0,0.0));
		light->SetRadius(10.0);
		light->SetPosition(-5.8,2.6,0.7);	
		light->SetName("RedLight");
		//light->SetProjectedTexture(m_pDevice->GetRessourceManager()->LoadTexture("brick.itx"));

		light = (INodeLight*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeLight");
		light->SetRadius(1000.0);
		light->SetPosition(-10,10,10);
		light->SetName("Sun");
		//light->SetProjectedTexture(m_pDevice->GetRessourceManager()->LoadTexture("brick.itx"));

		Initial::IMesh *_3ds;
		Initial::IMesh *_3ds2;
		Initial::IMesh *_3ds3;
		_3ds=NULL;
		_3ds2=NULL;
		_3ds3=NULL;
		_3ds = I3DLoad::Load("marcus.3ds",m_pDevice->GetRenderDriver());
		//_3ds2 = I3DLoad::Load("car.3ds",m_pDevice->GetRenderDriver());
		_3ds2 = I3DLoad::Load("cube.3ds",m_pDevice->GetRenderDriver());
		//_3ds3 = I3DLoad::Load("teapot.3ds",m_pDevice->GetRenderDriver());
		//_3ds3 = I3DLoad::Load("ElephantBody.3ds",m_pDevice->GetRenderDriver());
		_3ds3 = I3DLoad::Load("TIEf3DS8.3ds",m_pDevice->GetRenderDriver());
		//_3ds3 = I3DLoad::Load("cube.3ds",m_pDevice->GetRenderDriver());


		if (_3ds)
		{
			//_3ds->CalcNormals();
			//_3ds->SetFlags(IMesh::RF_ALWAYS_RENDER,false);
			//_3ds->SetFlags(RF_WIREFRAME,true);
			//_3ds->SetFlags(IMesh::RF_WIREFRAME_ONLY,true);
			//_3ds->SetFlags(RF_SHOW_NORMAL,true);	
			//_3ds->SetFlags(RF_SHOW_BOUNDING_BOX,true);
			//_3ds->SetFlags(RF_SHOW_CHILDREN_BOUNDING_BOX,true);		
			_3ds->SetScale(0.02f);
			_3ds->RotateOrigX(DEG_TO_RAD(-90));
			//_3ds->RotateOrigZ(DEG_TO_RAD(180));
			_3ds->Translate(-3,0,0);
			//_3ds->SetColor(IColor(1,1,0));

			_3ds->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/marcus.ima"));
			if (_3ds->GetChild(1))
				_3ds->GetChild(1)->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/marcus-hair.ima"));
			if (_3ds->GetChild(2))
				_3ds->GetChild(2)->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/marcus-hair.ima"));
		}

		if (_3ds2)
		{
			//_3ds->CalcNormals();
			//_3ds2->SetScale(0.1f);
			_3ds2->SetScale(2);
			_3ds2->RotateOrigX(DEG_TO_RAD(-90));
			_3ds2->Translate(-6,0,0);
			//_3ds->SetColor(IColor(1,1,0));
			_3ds2->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/car.ima"));
		}	

		if (_3ds3)
		{
			//_3ds3->CalcNormals();
			//_3ds3->SetScale(3);
			_3ds3->SetScale(0.2f);
			_3ds3->RotateOrigX(DEG_TO_RAD(-90));
			_3ds3->Translate(-12,0,0);
			//_3ds->SetColor(IColor(1,1,0));
			_3ds3->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/tie.ima"));
		}

		//m_pDevice->GetRessourceManager()->LoadMaterial("materials/elephant.ima")->LoadV2("testv2.ima");

		INodeText *text = (INodeText*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeText");
		if (text)
		{
			text->SetText("InitialisEd 0.1");
			Initial::GUI::IFontDrawParam& params=text->GetParams();
			params.m_fOutlineSize=2.0;
			params.m_fSize=5.0;
			params.m_cOutlineColor.Set(1.0,0,0,1.0);
			//params.m_bShadow=false;
		}

		/*for (UINT i=0;i<IObject::ObjectsClass.Count();i++)
		{
			printf("%s\n",IObject::ObjectsClass[i]->GetName());
		}*/
	}

	m_pNodeTree = new NodeTree(this);
	if (m_pNodeTree)
	{
		m_pNodeTree->SetSize(300,150);
		m_mgr.AddPane(m_pNodeTree,wxAuiPaneInfo().
			Name(wxT("Tree")).Caption(wxT("Tree")).
			Left().BestSize(300,150).
			CloseButton(false).MaximizeButton(true).
			Dockable(true));

		if (m_pInitial && m_pInitial->GetDevice())
			m_pNodeTree->LinkNodeManager(m_pInitial->GetDevice()->GetSceneManager());
		m_pNodeTree->LinkProperty(m_pProp);
		m_pNodeTree->RefreshList();
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

MainWindowInitialisEd::~MainWindowInitialisEd()
{
	m_mgr.UnInit();
}

void MainWindowInitialisEd::OnClose(wxCloseEvent& event)
{
	if (m_pInitial)
		m_pInitial->Stop();
	exit(0);
}

void MainWindowInitialisEd::OnMenu(wxCommandEvent& event)
{
	if (event.GetId()>=MW_MENU_RENDER_DEFAULT && event.GetId()<=MW_MENU_RENDER_BLOOM)
	{
		if (m_pInitial)
			m_pInitial->GetDevice()->GetRenderDriver()->SetOutputTexture(event.GetId()-MW_MENU_RENDER_DEFAULT);
	}
}

void MainWindowInitialisEd::OnTool(wxCommandEvent& event)
{
	printf("OnTool\n");
	if (event.GetId()>=MW_TB_SELECT && event.GetId()<=MW_TB_SCALE)
	{
		m_pToolbar->ToggleTool(MW_TB_SELECT, event.GetId()==MW_TB_SELECT?true:false);
		m_pToolbar->ToggleTool(MW_TB_MOVE, event.GetId()==MW_TB_MOVE?true:false);
		m_pToolbar->ToggleTool(MW_TB_ROTATE, event.GetId()==MW_TB_ROTATE?true:false);
		m_pToolbar->ToggleTool(MW_TB_SCALE, event.GetId()==MW_TB_SCALE?true:false);

		m_pInitial->SetTool((InitialPanel::ToolType)(InitialPanel::TT_SELECT+event.GetId()-MW_TB_SELECT));
	}
}

void MainWindowInitialisEd::OnTrandModeChoice(wxCommandEvent& event)
{
	printf("%d\n",event.GetInt());
	if (m_pInitial)
		m_pInitial->SetTransMode((InitialPanel::TransMode)event.GetInt());
}
