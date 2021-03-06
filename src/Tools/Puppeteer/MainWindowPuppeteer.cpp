
#include "MainWindowPuppeteer.h"

#include "Initial/Node/INodeText.h"

#include "wx/filename.h" // filename support
//#include "wx/dirdlg.h" // filename support

#include <vector>

BEGIN_EVENT_TABLE(MainWindowPuppeteer, wxFrame)
	/*EVT_BUTTON(MW_OPEN_FILE,MainWindowPuppeteer::OnOpenFile)
	EVT_BUTTON(MW_AUTO,MainWindowPuppeteer::OnAuto)
	EVT_BUTTON(MW_CONVERT,MainWindowPuppeteer::OnConvert)
	EVT_RADIOBOX(wxID_ANY,OnRadio)

	EVT_BUTTON(MW_OPEN_FOLDER,MainWindowPuppeteer::OnOpenFolder)
	EVT_BUTTON(MW_START,MainWindowPuppeteer::OnStart)
	EVT_BUTTON(MW_REBUILD,MainWindowPuppeteer::OnStart)*/

	EVT_MENU(wxID_ANY,OnMenu)
END_EVENT_TABLE()

using namespace Initial;
using namespace Initial::Core;

MainWindowPuppeteer::MainWindowPuppeteer(const wxString& title, const wxPoint& pos, const wxSize& size)
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

	m_pTree = new wxTreeCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(250,-1));
	if (m_pTree)
	{
		m_mgr.AddPane(m_pTree,wxAuiPaneInfo().
			Name(wxT("IEM file")).Caption(wxT("IEM file")).
			Left().
			CloseButton(false).MaximizeButton(true).
			Dockable(true));

		ClearTree();
	}

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

	std::vector<std::vector<Initial::Core::IVector3D>> test, test2;
	std::vector<Initial::Core::IVector3D> val;
	Initial::Core::IVector3D val2;
	test.push_back(val);
	test.back().push_back(val2);
	test.back().push_back(val2);
	test2=test;

	printf("%d\n",test.size());
	printf("%d\n",test.back().size());


	/*Initial::Core::IList<Initial::Core::IList<Initial::Core::IVector3D>> test, test2;
	test.PushBack(Initial::Core::IList<Initial::Core::IVector3D>());
	test.Last()->GetData().PushBack(Initial::Core::IVector3D());
	test2=test;*/

	//m_pInitial=NULL;
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
		light->SetProjectedTexture(m_pDevice->GetRessourceManager()->LoadTexture("brick.itx"));

		/*light = (INodeLight*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeLight");
		light->SetColor(IColor(0.0,1.0,0.0));
		light->SetRadius(10.0);
		light->SetPosition(0,2,0);

		light = (INodeLight*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeLight");
		light->SetColor(IColor(0.0,0.0,1.0));
		light->SetRadius(10.0);
		light->SetPosition(2,2,0);*/

		light = (INodeLight*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeLight");
		//light->SetColor(IColor(0.6f,0.6f,0.48f));
		light->SetRadius(1000.0);
		light->SetPosition(-10,10,10);
		light->SetProjectedTexture(m_pDevice->GetRessourceManager()->LoadTexture("brick.itx"));

		//m_pLight=light;

		/*for (int i=0;i<20;i++)
		{
		light = new INodeLight(m_pDevice);
		light->SetColor(IColor((rand()%1000)/1000.0,(rand()%1000)/1000.0,(rand()%1000)/1000.0));
		light->SetRadius(2.0);
		light->SetPosition(IVector3D((rand()%10)/1.0,(rand()%10)/1.0,(rand()%10)/1.0)+IVector3D(-10,-5,-5));
		m_pDevice->GetSceneManager()->AddNode(light);
		}*/

		//Initial::ICameraFPS *m_pCamera = new ICameraFPS(90,m_pDevice->GetWidth()/(float)m_pDevice->GetHeight());
		//m_pCamera->Move(0,0,-2);

		/*animObject = new IAnimatedMesh(m_pDevice->GetRenderDriver(),"Models/Skeleton/skeleton_head.cmf");
		//animObject->LoadAnimation("Models/skeleton/skeleton_jog.caf");
		animObject->SetScale(0.02f);
		animObject->RotateX(DEG_TO_RAD(-90));
		animObject->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("Materials/skeleton.ima"));
		*/
		//m_pDevice->GetRenderDriver()->GetTextureManager()->SetTextureData(;

		Initial::IMesh *_3ds;
		Initial::IMesh *_3ds2;
		Initial::IMesh *_3ds3;
		_3ds=NULL;
		_3ds2=NULL;
		_3ds3=NULL;
		_3ds = I3DLoad::Load("marcus.3ds",m_pDevice->GetRenderDriver());
		_3ds2 = I3DLoad::Load("car.3ds",m_pDevice->GetRenderDriver());
		//_3ds3 = I3DLoad::Load("teapot.3ds",m_pDevice->GetRenderDriver());
		//_3ds3 = I3DLoad::Load("ElephantBody.3ds",m_pDevice->GetRenderDriver());
		//_3ds3 = I3DLoad::Load("TIEf3DS8.3ds",m_pDevice->GetRenderDriver());
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
			_3ds2->SetScale(0.1f);
			_3ds2->RotateOrigX(DEG_TO_RAD(-90));
			_3ds2->Translate(-6,0,0);
			//_3ds->SetColor(IColor(1,1,0));
			_3ds2->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/car.ima"));
		}	

		if (_3ds3)
		{
			//_3ds3->CalcNormals();
			//_3ds3->SetScale(3);
			_3ds3->SetScale(0.03f);
			_3ds3->RotateOrigX(DEG_TO_RAD(-90));
			_3ds3->Translate(-12,0,0);
			//_3ds->SetColor(IColor(1,1,0));
			_3ds3->SetMaterial(m_pDevice->GetRessourceManager()->LoadMaterial("materials/elephant.ima"));
		}

		//m_pDevice->GetRessourceManager()->LoadMaterial("materials/elephant.ima")->LoadV2("testv2.ima");

		INodeText *text = (INodeText*)m_pDevice->GetSceneManager()->AddNodeByClass("INodeText");
		if (text)
		{
			printf("NodeText created\n");
			text->SetText("Super message de la mort\nHihihi!!!");
			text->SetText("<--\n\tInitial engine\t0.1\n\tBy CameleonTH\n-->");
			Initial::GUI::IFontDrawParam& params=text->GetParams();
			//params.m_bOutline=false;
			params.m_fOutlineSize=2.0;
			//params.m_fSize=2.0;
			params.m_cOutlineColor.Set(1.0,0,0,1.0);
			params.m_bShadow=false;
			//params.m_bSolid=false;
		}

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

MainWindowPuppeteer::~MainWindowPuppeteer()
{
	m_mgr.UnInit();
}

void MainWindowPuppeteer::OnMenu(wxCommandEvent& event)
{
	if (event.GetId()>=MW_MENU_RENDER_DEFAULT && event.GetId()<=MW_MENU_RENDER_BLOOM)
	{
		if (m_pInitial)
			m_pInitial->GetDevice()->GetRenderDriver()->SetOutputTexture(event.GetId()-MW_MENU_RENDER_DEFAULT);
	}
}

void MainWindowPuppeteer::ClearTree()
{
	if (m_pTree)
	{
		m_pTree->DeleteAllItems();
		wxTreeItemId root = m_pTree->AddRoot("Root");
		m_pTree->AppendItem(root,"Mesh");
		m_pTree->AppendItem(root,"Bones");
		m_pTree->ExpandAll();
	}
}
