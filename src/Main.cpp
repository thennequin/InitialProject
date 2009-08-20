
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>
//#include <gl/gl.h>


#include "Initial/Initial.h"
//#include "OpenGL/IRenderDriverOpengl.h"
#include "Initial/Thread/IThread.h"

#include "Initial/IObject.h"

#include "Initial/Format/IMeshIEM.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::IO;
using namespace Initial::Video;
using namespace Initial::GUI;
using namespace Initial::ShaderExpression;


#define PASS printf("\n");

void testShader();

void string()
{
	IString str("test");
	printf("%s\n",str.c_str());
	str="test2";
	printf("%s\n",str.c_str());
	str+=" test3 ";
	printf("%s\n",str.c_str());
	str+=12;
	printf("%s\n",str.c_str());
	str+=13;
	printf("%s\n",str.c_str());
	IString str2=str;

	str.Printf("%s %d %f %s %c",str.c_str(),1,5.2,"tryc",'t');
	printf("\n");
	printf("%s\n%s\n",str.c_str(),str2.c_str());
	//printf("test %d\n",!str);

	str="this is a sparta test";
	int pos = str.Find("sparta");
	printf("%s %d\n",str.c_str(),pos);

	printf("\nBefore\n\n");
	str="super mega chouette ";
	pos = str.Find(" ");
	str2=str.Before(" ");
	IString str3=str.After(" ");
	
	printf("%s %d\n",str.c_str(),pos);
	printf("%s\n",str2.c_str());
	printf("%s\n%c\n",str3.c_str(),str3[0]);
}

void matrix()
{
	//IMatrix mat(3);
	//IMatrix mat2(3);
	//IMatrix mat3;
	//mat2.MakeIdentity();
	//mat.SetValue(3*3,
	//1.0,0.0,5.0,
	//0.0,1.0,0.0,
	//0.0,1.0,1.0);
	//mat[0][1]=5;
	//
	//printf("A\n");
	//mat.Print();
	//printf("\nB\n");
	//mat2.Print();
	//printf("\nAdd\n");

	////mat.SetValue(0,2,10);
	//mat2.Add(mat,mat2);
	//mat2.Print();
	//printf("\nDot\n");
	//mat2.Dot(mat,mat2);
	//mat2.Print();

	//printf("\ntest\n");
	//IMatrix test(3,3);
	//test.MakeZero();
	//test[2][2]=10;
	//test.Print();

	//IMatrix x23(2,3);
	//IMatrix x32(3,2);
	//IMatrix res;

	//printf("\ntest2\n");
	//x23.SetValue(2*3,
	//	1.0,2.0,
	//	3.0,4.0,
	//	5.0,6.0);

	//x32.SetValue(3*2,
	//	1.0,2.0,3.0,
	//	4.0,5.0,6.0);

	////.Print();
	////res=(x23*x32)+test;
	//x23.Dot(x32,res);

	//printf("\nx23\n");
	//x23.Print();
	//printf("\nx32\n");
	//x32.Print();
	//printf("\nDot\n");
	//res.Print();

	//printf("\n");
	//IMatrix col(res.GetLine(0));
	//col.Print();

	float det,det2;
	IMatrix detm33(3,3);	
	detm33.SetValue(3*3,
		-2.0,2.0,-3.0,
		-1.0,1.0,-6.0,
		0.0,5.0,6.0);
	det = detm33.Det();
	det2 = detm33.Det(true);
	printf("\n");
	detm33.Print();
	printf("normal det33 %f %f\n",det,det2);

	IMatrix detm44(4,4);
	detm44.SetValue(4*4,
		6.0,23.0,7.0,1.0,
		-1.0,5.0,-2.0,2.0,
		5.0,6.0,4.0,3.0,
		9.0,8.0,-1.0,6.0);

	det = detm44.Det();
	det2 = detm44.Det(true);
PASS
	detm44.Print();
PASS
	detm44.Transpose().Print();
PASS
	printf("normal det44 %f %f\n",det,det2);
	IMatrix gauss = detm44.GaussJordan(true);
	gauss.Print();
	
	IMatrix inv;
	detm44.Inv(inv);
	printf("Inv\n");
	inv.Print();

	IMatrix initial=detm44*inv;
	printf("A*A-1=I\n");
	initial.Print();

PASS
PASS

	printf("Resolution d'une equation Ax=B par la methode de la pseudo inverse\nA\n");
	IMatrix A(3,3);
	IMatrix B(1,3);
	A.SetValue(3*3,
		1.0,0.0,-2.0,
		0.0,3.0,1.0,
		1.0,1.0,1.0);
	B.SetValue(3,
		5.0,7.0,4.0);

	A.Print();
	printf("\nB\n");
	B.Print();

	printf("\nx\n");

	IMatrix AT,temp,temp2,temp2Inv,res;
	AT=A.Transpose();
	temp=AT*B;
	temp2=AT*A;
	temp2.Inv(temp2Inv);
	res=temp2Inv*temp;
	res.Print();

}

IMesh *MakeCube( IRenderDriver *device )
{
	IMesh *cube = new IMesh(NULL);

	if (cube)
	{
		ITriangle *pol;

		pol=new ITriangle;
		pol->SetVertex(0,IVector3D(-0.5,-0.5,-0.5));
		pol->SetVertex(1,IVector3D(0.5,-0.5,-0.5));
		pol->SetVertex(2,IVector3D(-0.5,0.5,-0.5));
		pol->SetCoord(0,IVector2D(1,1));
		pol->SetCoord(1,IVector2D(0,1));
		pol->SetCoord(2,IVector2D(1,0));
		pol->SetNormal(0,IVector3D(0,0,1));
		pol->SetNormal(1,IVector3D(0,0,1));
		pol->SetNormal(2,IVector3D(0,0,1));
		//pol->SetMaterial(IMaterial::LoadMaterial(device,"materials/brick.image"));
		cube->AddPolygon(*pol);

		pol=new ITriangle;
		pol->SetVertex(0,IVector3D(0.5,0.5,-0.5));
		pol->SetVertex(1,IVector3D(0.5,-0.5,-0.5));
		pol->SetVertex(2,IVector3D(-0.5,0.5,-0.5));
		pol->SetCoord(0,IVector2D(0,0));
		pol->SetCoord(1,IVector2D(0,1));
		pol->SetCoord(2,IVector2D(1,0));
		pol->SetNormal(0,IVector3D(0,0,1));
		pol->SetNormal(1,IVector3D(0,0,1));
		pol->SetNormal(2,IVector3D(0,0,1));
		pol->GetNormals()[0].Normalize();
		//pol->SetMaterial(IMaterial::LoadMaterial(device,"materials/brick.image"));
		cube->AddPolygon(*pol);

		//cube->SetMaterial(IMaterial::LoadMaterial(device,"materials/brick.image"));
	}

	cube->CalculateBBox();
	return cube;
}

IMutex mutex;

class MyThread : public IThread
{
public:
	MyThread(int id)
	{
		str="test";
		str+=id;
	}

protected:
	virtual void ThreadFunc()
	{
		printf("MyThread Test %s\n",str.c_str());

		while (1)
		{
			//IMutexLocker lock(mutex);
			mutex.Lock();
			printf("Lock\n");
			_sleep(500+rand()%100);
			printf("	MyThread hohoho %s\n",str.c_str());
			printf("Unlock\n");
			mutex.Unlock();
		}
	}
protected:
	IString str;
	IArray<MyThread> arraytest;
};

int main2();

int main(int argn, char *argv[])
{
	long time = GetTickCount();

	XML::IXMLParser xmlreader2("config.xml");
	xmlreader2.Parse();

	printf("%f\n",(GetTickCount()-time)/1000.0);
	system("PAUSE");
	/*{
		IDevice *device = Initial::CreateDevice(IVDT_OPENGL,640,480,32,0,"test");
		while(device->Run())
			device->GetRenderDriver()->SwapBuffer();
	return 0;
	}*/
	IString full = GetFullPath("test0.txt");
	printf("%s\n",full.c_str());

	//IRessourceManager manager;
	//manager.Run();

	main2();
	return 0;

	//system("PAUSE");

	//return 0;

	//Format::IImageITX image;
	//int resultas = image.Load("Textures/brick.itx");
	//printf("%d\n",resultas);
	//resultas = image.Save("Textures/brick2.itx");
	//printf("%d\n",resultas);
	//resultas = image.Load("Textures/brick2.itx");
	////system("PAUSE");

	//Format::IMeshIEM iem,iem2,iem3;
	//iem.SetName("Skeleton");
	//iem.SetSkeleton("Models/Skeleton/skeleton.csf");
	//iem.AddMesh("Models/Skeleton/skeleton_calf_left.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_calf_left.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_calf_right.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_chest.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_foot_left.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_foot_right.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_hair.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_hand_left.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_hand_right.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_head.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_lowerarm_left.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_lowerarm_right.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_neck.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_pelvis.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_spine_lower.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_spine_upper.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_thigh_left.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_thigh_right.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_upperarm_left.cmf");
	//iem.AddMesh("Models/Skeleton/skeleton_upperarm_right.cmf");
	//iem.SetMaterial(0,"test.ima");
	//iem.SetMaterial(1,"test_head.ima");
	//printf("--------Write\n");
	//iem.SaveIEM("skeleton.iem");

	///*iem3.SetName("Paladin");
	//iem3.SetSkeleton("Models/Paladin/Paladin.csf");
	//iem3.AddMesh("Models/Paladin/paladin_cape.cmf");
	//iem3.AddMesh("Models/Paladin/paladin_body.cmf");
	//iem3.AddMesh("Models/Paladin/paladin_loincloth.cmf");
	//iem3.SaveIEM("paladin.iem");*/

	//printf("--------Read\n");
	//iem2.LoadIEM("skeleton.iem");
	//iem2.SaveIEM("skeleton2.iem");
	//system("PAUSE");
	////return 0;

	//{
	//	ISmartPtr<IObject> monPointeur,monPointeur2;
	//	monPointeur=new IObject();
	//	monPointeur2=monPointeur;
	//	printf("%d\n",(void*)monPointeur);
	//	printf("%d\n",(void*)monPointeur2);
	//	system("PAUSE");
	//	delete monPointeur2;
	//	printf("%d\n",(void*)monPointeur);
	//	printf("%d\n",(void*)monPointeur2);
	//}
	//system("PAUSE");
	//return 0;

	{
		ISEVec2 Pos/* = InputIsConnect(0) ? GetInputExpr(0).xy() : TextureCoord().xy()*/;
		ISEVec2 Origin = /*InputIsConnect(1) ? GetInputExpr(1).xy() :*/ ISEVec2(0.5,0.5);
		ISEFloat angle = /*InputIsConnect(2) ? GetInputExpr(2).x() :*/ 0.0;
		ISEFloat cos = Cos(angle);
		ISEFloat sin = Sin(angle);
		ISEFloat test = -sin;
		ISEVec2 RowX(cos,-sin);
		ISEVec2 RowY(sin,cos);
		ISEVec2 BaseCoord = Pos;
		ISEFloat Arg1 = Dot(RowX,(BaseCoord-Origin));
		ISEFloat Arg2 = Dot(RowY,(BaseCoord-Origin));
		ISEVec2 Rotator = ISEVec2(Arg1,Arg2)+Origin;
		printf("%s\n",Rotator.GetGLSLString().c_str());
	}

	//MyThread thread1(0);
	///*MyThread thread2(1);
	//MyThread thread3(2);
	//MyThread thread4(3);
	//mutex.Lock();
	//{		
	//	//IMutexLocker lock(mutex);
	//	thread1.Run();
	//	thread2.Run();
	//	thread3.Run();
	//	thread4.Run();
	//}
	//mutex.Unlock();

	//_sleep(1500);
	//
	//system("PAUSE");
	//exit(0);*/

	////string();
	//matrix();

	//IArray<IString> Array;
	//Array.Add(IString("test"));
	//Array.Add(IString("test2"));
	//Array.Add(IString("test3"));
	//Array.Add(IString("test4"));

	//for (int i=0;i<10;i++)
	//	Array.Add("test"+IString(i+5));

	//Array.Delete(1);
	//Array[2]="test5";

	//for (int i=0;i<Array.Count();i++)
	//{
	//	printf("%s\n",Array[i].c_str());
	//}
	//printf("%s\n",Array[2].c_str());
	//printf("%s\n",Array[3].c_str());
	//printf("%s\n",Array[1].c_str());
	//printf("%s\n",Array[1].c_str());

	////printf("tan 45 = %f\ntan 90 = %f\n",tan(45),tan(90));

	//ISEVec2 A("A");
	//ISEVec2 B("B");
	//ISEVec3 Ok("Ok");
	//ISEVec3 Else("Else");
	//ISEVec3 res = If(A,B,"<",Ok,Else);

	//printf("If = %s\n",res.GetGLSLString().c_str());

	////testShader();

	//system("PAUSE");
	//exit(0);

	XML::IXMLParser xmlreader("test.xml");
	xmlreader.Parse();
	
	XML::IXMLNode* node = xmlreader.GetFirstNode();
	IString str="-------------------\n";
	while(node)
	{			
		str+=node->print();
		node=node->GetNext();
	}		
	//printf(str.c_str());

	IConfigINI ini("config.ini");
	ini.Load();
	int Width = ini.ReadIntValue("Width",640);
	int Height = ini.ReadIntValue("Height",480);
	int Bits = ini.ReadIntValue("Bits",32);
	int Fullscreen = ini.ReadIntValue("Fullscreen",0);
	int AntiAliasing = ini.ReadIntValue("AntiAliasing",8);
	int UseShader = ini.ReadIntValue("UseShader",1);
	int UsePP = ini.ReadIntValue("UsePostProcess",1);
	ini.Save();	

	//IRenderDriver* device= CreateDevice(IVDT_OPENGL,Width,Height,Bits,Fullscreen,"Initial Engine",AntiAliasing,UseShader);
	//if (!device)
	//{
	//	system("PAUSE");
	//	return 1;
	//}
	//device->SetMouseLock(false);
	//device->SetShowMouse(false);
	////device->SetShowMouse(true);

	////device->LoadTexture("brick_normal.png");

	////Frame Buffer
	//ITexture *colorBuffer = device->GetTextureManager()->CreateTexture(1024,1024);
	//IFrameBuffer *frameBuffer = new IFrameBuffer(device);
	//
	//ITexture *destTex = device->GetTextureManager()->CreateTexture(1024,1024);
	//ITexture *destTex2 = device->GetTextureManager()->CreateTexture(1024,1024);
	//IMaterial *destMat = IMaterial::LoadMaterialFromTexture(device,destTex);
	//destMat->SetTexture(destTex2,1);

	//ITexture *videoTex = device->GetTextureManager()->LoadVideo("test.bik");
	////ITexture *videoTex = device->GetTextureManager()->LoadTexture("WETSUIT.jpg");
	////videoTex->UpdateImage();
	////destMat->SetTexture(videoTex,0);

	//ICameraFPS camera(90,device->GetWidth()/(float)device->GetHeight());
	////ICameraFree camera2(90,device->GetWidth()/(float)device->GetHeight());	
	//ICameraFPS camera2(90,1); // Camera du render to texture avec un aspect ratio de 1
	////camera.Move(2,0,0);
	////camera2.Move(-2,0,0);
	////device->SetCamera(&camera);
	////device->SetCamera(NULL);
	//ILight *lights[4];
	//
	//lights[0] = new ILight(IColor(0.5,0,0),IVector3D(-2,2,0),10,device);
	//lights[1] = new ILight(IColor(0,0.5,0),IVector3D(0,2,0),10,device);
	//lights[2] = new ILight(IColor(0,0,0.5),IVector3D(2,2,0),10,device);

	///*new ILight(IColor(1,1,0),IVector3D(0,2,-2),1);
	//new ILight(IColor(0,1,1),IVector3D(-2,0,2),1);
	//new ILight(IColor(1,0,1),IVector3D(2,5,2),1);
	//new ILight(IColor(1,0,0),IVector3D(-7,-1,1),1);
	//new ILight(IColor(0,0,1),IVector3D(-7,3,-1),5);*/

	////for (int i=0;i<100;i++)
	//	//new ILight(IColor((rand()%1000)/1000.0,(rand()%1000)/1000.0,(rand()%1000)/1000.0),IVector3D((rand()%10)/1.0,(rand()%10)/1.0,(rand()%10)/1.0),5,device);
	////lights[3] = new ILight(IColor(1,1,1),IVector3D(0,0,0),10,device);

	//lights[3] = new ILight(IColor(0.8,0.8,0.64),IVector3D(-10,10,10),10000,device); // Sun

	//lights[0]->m_bGodRay=false;
	//lights[1]->m_bGodRay=false;
	//lights[2]->m_bGodRay=false;
	//lights[3]->m_bGodRay=true;
	//ILight *mylight = lights[3];

	//IMaterial *mat = IMaterial::LoadMaterial(device,"materials/brick.image");
	////IMaterial *mat = IMaterial::LoadMaterial(device,"light.ima");

	//IMesh *_3ds=NULL;
	//IMesh *_3ds2=NULL;
	////_3ds = I3DLoad::Load("chair_01.3DS",device);
	////_3ds = I3DLoad::Load("teapot.3ds",device);
	////_3ds = I3DLoad::Load("car.3ds",device);
	////_3ds = I3DLoad::Load("hebe.3ds",device);
	////_3ds = I3DLoad::Load("lara01.3DS",device);
	////_3ds = I3DLoad::Load("M1_Abrams.3DS",device);
	////_3ds = I3DLoad::Load("TIEf3DS8.3ds",device);
	////_3ds = I3DLoad::Load("dreezle.3ds",device);
	////_3ds = I3DLoad::Load("ElephantBody.3ds",device);
	////_3ds = I3DLoad::Load("Projet_tut_IUT.3ds",device);
	////_3ds = I3DLoad::Load("COCACOLA.3DS",device);
	////_3ds = I3DLoad::Load("Untitled.3ds",device);	
	//_3ds = I3DLoad::Load("marcus.3ds",device);
	//_3ds2 = I3DLoad::Load("car.3ds",device);
	//
	//if (_3ds)
	//{
	//	//_3ds->CalcNormals();
	//	//_3ds->SetFlags(IMesh::RF_ALWAYS_RENDER,false);
	//	//_3ds->SetFlags(RF_WIREFRAME,true);
	//	//_3ds->SetFlags(IMesh::RF_WIREFRAME_ONLY,true);
	//	//_3ds->SetFlags(RF_SHOW_NORMAL,true);	
	//	//_3ds->SetFlags(RF_SHOW_BOUNDING_BOX,true);
	//	//_3ds->SetFlags(RF_SHOW_CHILDREN_BOUNDING_BOX,true);		
	//	_3ds->SetScale(0.02);
	//	_3ds->RotateOrigX(DEG_TO_RAD(-90));
	//	//_3ds->RotateOrigZ(DEG_TO_RAD(180));
	//	_3ds->Translate(-3,0,0);
	//	//_3ds->SetColor(IColor(1,1,0));
	//	_3ds->SetMaterial(IMaterial::LoadMaterial(device,"materials/marcus.ima"));
	//	if (_3ds->GetChild(1))
	//		_3ds->GetChild(1)->SetMaterial(IMaterial::LoadMaterial(device,"materials/marcus-hair.ima"));
	//	if (_3ds->GetChild(2))
	//		_3ds->GetChild(2)->SetMaterial(IMaterial::LoadMaterial(device,"materials/marcus-hair.ima"));
	//}

	//if (_3ds2)
	//{
	//	//_3ds->CalcNormals();
	//	_3ds2->SetScale(0.1);
	//	_3ds2->RotateOrigX(DEG_TO_RAD(-90));
	//	_3ds2->Translate(-6,0,0);
	//	//_3ds->SetColor(IColor(1,1,0));
	//	_3ds2->SetMaterial(IMaterial::LoadMaterial(device,"materials/car.ima"));
	//}

	//IMesh *cube=NULL;
	//cube = MakeCube(device);
	//if (cube)
	//{
	//	//cube->CalcNormals();
	//	cube->SetDevice(device);
	//	//cube->SetFlags(RF_ALWAYS_RENDER,false);
	//	//cube->SetFlags(RF_WIREFRAME,true);
	//	//cube->SetFlags(RF_WIREFRAME_ONLY,true);
	//	//cube->SetFlags(RF_WHITE_WIREFRAME,false);
	//	//cube->SetFlags(RF_SHOW_NORMAL,true);
	//	//cube->SetFlags(RF_SHOW_BOUNDING_BOX,true);
	//	
	//	//cube->TranslateOrig(0,0,-10);
	//	cube->SetScale(3);

	//	cube->SetMaterial(destMat);
	//	
	//	//cube->RotateOrigX(-50);
	//	cube->RotateZ(DEG_TO_RAD(180));
	//}

	//IMesh *cube2=NULL;
	//cube2 = MakeCube(device);
	//if (cube2)
	//{
	//	//cube2->CalcNormals();
	//	cube2->SetDevice(device);
	//	cube2->SetFlags(IRF_WIREFRAME,true);
	//	cube2->SetFlags(IRF_SHOW_NORMAL,true);
	//	cube2->TranslateOrig(5,0,0);
	//	cube2->SetScale(3);
	//	cube2->SetMaterial(mat);
	//	/*if (cube2->GetMaterial())
	//		cube2->GetMaterial()->SetTexture(videoTex,0);*/
	//}

	//IAnimatedMesh *animObject = new IAnimatedMesh(device,"Models/Skeleton/skeleton_head.cmf"/*,"Models/Skeleton/skeleton.csf","Models/Skeleton/skeleton_jog.caf"*/);
	////IAnimatedMesh *animObject = new IAnimatedMesh(device,"Models/paladin/paladin_body.cmf","Models/paladin/paladin.csf","Models/paladin/paladin_jog.caf");
	//animObject->LoadAnimation("Models/skeleton/skeleton_jog.caf");
	//animObject->SetScale(0.02);
	//animObject->RotateX(DEG_TO_RAD(-90));
	//animObject->SetMaterial(mat);

	//GUI::IFontManager fontManager;
	//IFont* font=NULL;
	//IFont* font2=NULL;
	//fontManager.SetRenderDevice(device);
	////font2=fontManager.LoadFont("DS-DIGI.TTF",1);
	////font2=fontManager.LoadFont("african.ttf",2);
	////font2=fontManager.LoadFont("ALGER.TTF");
	////font2=fontManager.LoadFont("SLNTHLN.TTF");
	////font2=fontManager.LoadFont("SFOR.ttf");
	////font2=fontManager.LoadFont("BRADHITC.TTF");
	////font=fontManager.LoadFont("fable2.ttf",2);
	///*if (font==NULL)
	//	font=fontManager.LoadFont("c:/windows/fonts/arial.ttf",2);
	//if (font2==NULL)
	//	font2=fontManager.LoadFont("c:/windows/fonts/comic.ttf",2);*/
	//

	//IArray<IPolygon> contours;
	//contours.Add(IPolygon());
	//contours[0].AddPoint(IVector3D(0,0));
	//contours[0].AddPoint(IVector3D(1,0));
	//contours[0].AddPoint(IVector3D(1,1));
	//contours[0].AddPoint(IVector3D(0,1));
	//contours[0].AddPoint(IVector3D(-0.5,0.5));

	//contours.Add(IPolygon());
	//contours[1].AddPoint(IVector3D(0.25,0.25));
	//contours[1].AddPoint(IVector3D(0.75,0.25));
	//contours[1].AddPoint(IVector3D(0.75,0.75));
	//contours[1].AddPoint(IVector3D(0.25,0.75));

	//contours.Add(IPolygon());
	//contours[2].AddPoint(IVector3D(0.875,0.5));
	//contours[2].AddPoint(IVector3D(0.875,0.75));
	//contours[2].AddPoint(IVector3D(1.5,0.75));
	//contours[2].AddPoint(IVector3D(1.5,0.5));

	//ITriangulator triangulate(contours);
	//IArray<ITriangle> result;
	////triangulate.Triangulate(result);

	//printf("Contours count %d\nResult %d",contours.Count(),result.Count());
	///*for (int i=0;i<contours.Count();i++)
	//{
	//	contours[i]->Clear(true);
	//}
	//contours.Clear(true);*/

	//float zoom=90;
	//bool exit=false;
	//int speed=1;
	//unsigned long Time = GetTickCount();
	//unsigned long Begin = Time; 
	//float FPS=0;
	//ICamera *selectedcamera=NULL;
	//while (device->UpdateDevice() && exit==false)
	//{
	//	//Update light position;
	//	//lights[3]->SetPosition(IVector3D(100*sin(GetTickCount()/5000.0)-4,10,100*cos(GetTickCount()/2000.0)));
	//	//mylight->SetPosition(IVector3D(5*sin(GetTickCount()/1000.0)-4,2*cos(GetTickCount()/1000.0),5*cos(GetTickCount()/500.0)));

	//	float speed=0.02;
	//	if (device->KeyDown(VK_NUMPAD4))
	//		mylight->SetPosition(mylight->GetPosition()+IVector3D(-speed,0,0));
	//	if (device->KeyDown(VK_NUMPAD6))
	//		mylight->SetPosition(mylight->GetPosition()+IVector3D(speed,0,0));
	//	if (device->KeyDown(VK_NUMPAD9))
	//		mylight->SetPosition(mylight->GetPosition()+IVector3D(0,speed,0));
	//	if (device->KeyDown(VK_NUMPAD3))
	//		mylight->SetPosition(mylight->GetPosition()+IVector3D(0,-speed,0));
	//	if (device->KeyDown(VK_NUMPAD8))
	//		mylight->SetPosition(mylight->GetPosition()+IVector3D(0,0,-speed));
	//	if (device->KeyDown(VK_NUMPAD2))
	//		mylight->SetPosition(mylight->GetPosition()+IVector3D(0,0,speed));

	//	if (device->KeyPress(VK_DIVIDE))
	//		mylight=lights[0];
	//	if (device->KeyPress(VK_MULTIPLY))
	//		mylight=lights[1];
	//	if (device->KeyPress(VK_SUBTRACT))
	//		mylight=lights[2];
	//	if (device->KeyPress(VK_ADD))
	//		mylight=lights[3];
	//	//mylight->SetPosition(IVector3D(0,0,5*cos(GetTickCount()/1000.0)));

	//	//videoTex->UpdateImage();

	//	if (device->KeyPress(VK_ESCAPE))
	//		exit=true;
	//	if (device->KeyPress(VK_F1))
	//	{
	//		//device->SetCamera(NULL);
	//		device->SetMouseLock(false);
	//		device->SetShowMouse(true);
	//	}
	//	else if (device->KeyPress(VK_F2))
	//	{
	//		device->SetMouseLock(true);
	//		device->SetShowMouse(false);
	//		device->UpdateDevice();
	//		//device->SetCamera(&camera);		
	//		selectedcamera=&camera;
	//	}
	//	else if (device->KeyPress(VK_F3))
	//	{
	//		device->SetMouseLock(true);
	//		device->SetShowMouse(false);
	//		device->UpdateDevice();
	//		//device->SetCamera(&camera2);		
	//		selectedcamera=&camera2;
	//	}

	//	if (device->KeyPress(VK_F4))
	//		device->SetAntialising(1);
	//	else if (device->KeyPress(VK_F5))
	//		device->SetAntialising(0);

	//	if (device->KeyPress(VK_F6) && cube)
	//		cube->SetFlags(IRF_ALWAYS_RENDER,!(cube->GetFlag(IRF_ALWAYS_RENDER)));

	//	if (device->KeyPress(VK_F7) && _3ds)
	//		_3ds->SetFlags(IRF_ALWAYS_RENDER,!(_3ds->GetFlag(IRF_ALWAYS_RENDER)));

	//	if (device->MousePress(0))
	//	{
	//		//printf("press\n");
	//		zoom-=1;
	//		if (zoom<40)
	//			zoom=40;
	//	}else{
	//		zoom+=1;
	//		if (zoom>90)
	//			zoom=90;
	//	}

	//	//ICamera *cam = device->GetCamera();
	//	if (selectedcamera)
	//	{
	//		if (device->KeyDown(VK_SHIFT))
	//			speed=2;
	//		else
	//			speed=1;

	//		if (device->KeyDown(VK_UP))
	//			selectedcamera->Move(0,0,0.1*speed);
	//		if (device->KeyDown(VK_DOWN))
	//			selectedcamera->Move(0,0,-0.1*speed);
	//		if (device->KeyDown(VK_LEFT))
	//			selectedcamera->Move(0.1*speed,0,0);
	//		if (device->KeyDown(VK_RIGHT))
	//			selectedcamera->Move(-0.1*speed,0,0);

	//		if (device->KeyDown(VK_NUMPAD0))
	//			selectedcamera->Move(0,0.1*speed,0);
	//		if (device->KeyDown(VK_CONTROL))
	//			selectedcamera->Move(0,-0.1*speed,0);

	//		selectedcamera->RotateY(device->GetMouseAccX()/100.0);
	//		selectedcamera->RotateX(device->GetMouseAccY()/100.0);

	//		selectedcamera->SetFov(zoom);
	//	}

	//	/*device->BeginRender();
	//		device->SetCamera(&camera2);
	//		device->Render();
	//		//fontManager.RenderText(font,"Texte dans un RenderToTexture");
	//	device->EndRender(destTex);*/

	//	//device->BeginRender(0,true);
	//	device->BeginRender();
	//		device->SetCamera(&camera);
	//		/*device->_DrawLine(IVector3D(),IVector3D(1,0,0),IColor(1,0,0));
	//		device->_DrawLine(IVector3D(),IVector3D(0,1,0),IColor(0,1,0));
	//		device->_DrawLine(IVector3D(),IVector3D(0,0,1),IColor(0,0,1));*/
	//		//cube->RotateOrigX(DEG_TO_RAD(0.25));
	//		//cube->TranslateOrig(0.1,0,0);
	//		
	//		//cube->RotateOrigZ(DEG_TO_RAD(0.25));
	//		device->Render(NULL);

	//		animObject->Render(device);

	//		device->UseMaterial(NULL);
	//		for (int i=0;i<result.Count();i++)
	//		{
	//			device->_StartTriangleDraw(false);
	//				device->_DrawTriangle(&result[i]);
	//			device->_EndTriangleDraw();
	//		}
	//		for (int i=0;i<result.Count();i++)
	//		{
	//			device->_SetColor(IColor(1,0,0));
	//			device->_DrawLine(result[i].GetVertex()[0],result[i].GetVertex()[1]);
	//			device->_SetColor(IColor(0,1,0));
	//			device->_DrawLine(result[i].GetVertex()[1],result[i].GetVertex()[2]);
	//			device->_SetColor(IColor(0,0,1));
	//			device->_DrawLine(result[i].GetVertex()[2],result[i].GetVertex()[0]);
	//		}

	//		IString str;

	//		IBBox box(-0.1,-0.1,-0.1,0.1,0.1,0.1);
	//		box.Render(device);
	//		device->_SetColor(IColor(1,0,0));
	//		const float Quality=50.0;
	//		for (int i=0;i<Quality;i++)
	//		{
	//			/*const double x=0.085;
	//			const double y=0.915;*/
	//			const double x=0.085;
	//			const double y=0.915;
	//			/*device->_DrawLine(	RenderConicBezier(IVector3D(1,0,1),IVector3D(1,0,1),IVector3D(1,1,1),i/Quality),
	//								RenderConicBezier(IVector3D(1,0,1),IVector3D(1,0,1),IVector3D(1,1,1),(i+1)/Quality));*/
	//			device->_DrawLine(	RenderConicBezier(IVector3D(0,0,1),IVector3D(1,1,1),IVector3D(x,y,1),i/Quality),
	//								RenderConicBezier(IVector3D(0,0,1),IVector3D(1,1,1),IVector3D(x,y,1),(i+1)/Quality));
	//		}

	//		device->_SetColor(IColor(0,1,0));
	//		for (int i=0;i<Quality;i++)
	//		{
	//			device->_DrawLine(	IVector3D(1)+IVector3D(cos((i/Quality)*I_PIE/2+I_PIE/2),
	//								sin((i/Quality)*I_PIE/2+I_PIE/2),1),
	//								IVector3D(1)+IVector3D(cos(((i+1)/Quality)*I_PIE/2+I_PIE/2),
	//								sin(((i+1)/Quality)*I_PIE/2+I_PIE/2),1));
	//		}

	//		IBBox lightbox;
	//		lightbox.SetDimension(IVector3D(0.1,0.1,0.1));
	//		lightbox.SetCenter(lights[0]->GetPosition());
	//		device->_SetColor(IColor(1,0,0));
	//		lightbox.Render(device);

	//		lightbox.SetCenter(lights[1]->GetPosition());
	//		device->_SetColor(IColor(0,1,0));
	//		lightbox.Render(device);

	//		lightbox.SetCenter(lights[2]->GetPosition());
	//		device->_SetColor(IColor(0,0,1));
	//		lightbox.Render(device);

	//		lightbox.SetCenter(lights[3]->GetPosition());
	//		device->_SetColor(IColor(1,1,1));
	//		lightbox.Render(device);

	//		IFontDrawParam param;
	//		param.m_bOutline=true;
	//		//param.m_cColor=IColor(1,1,1);
	//		param.m_cOutlineColor=IColor(0,0,0);
	//		//param.m_fOutlineSize=2;
	//		str.Printf("\n\n\n\n\n\nFable 2\nBonjour je suis Thibault\nCréateur de ce	programme\n^$ù*=)&é_çà'é-('");
	//		//fontManager.RenderText(font,str,param);

	//		param.m_cColor=IColor(0,128.0/255,0);
	//		//param.m_cOutlineColor=IColor(0,92.0/255,0);
	//		param.m_cOutlineColor=IColor();
	//		param.m_cShadowColor=IColor(0,32.0/255,0,0.5);
	//		param.m_fOutlineSize=2;
	//		param.m_fSize=1.5;
	//		//param.m_bFixedSpace=true;
	//		param.m_fFixedSpaceWidth=0.4;
	//		//str.Printf("%d polygons\n%d lines\n%0.1f FPS",device->PolygonDrawsCount(),device->LineDrawsCount(),FPS);
	//		float Speed=1;
	//		float Start=(GetTickCount()*Speed-Begin*Speed)/1000.0;
	//		int hour = Start/3600;
	//		int min = Start/60 - hour*60;
	//		int sec = Start - min*60 - hour*3600;
	//		int ms = (Start - sec - min*60 - hour*3600)*100;
	//		str.Printf("%02d:%02d:%02d:%02d\n%d polygons\n%d lines\n%0.1f FPS",hour, min, sec, ms, device->PolygonDrawsCount(),device->LineDrawsCount(),FPS);
	//		fontManager.RenderText(font2,str,param);
	//	device->EndRender();

	//	//device->PostProcess(IMaterial::LoadMaterial(device,"postprocess.ima"));
	//	/*if (UsePP)
	//		device->PostProcess(NULL);*/

	//	device->SwapBuffer();

	//	unsigned long Time2=GetTickCount();
	//	FPS=(FPS+1000/(Time2-Time+1))/2.0;
	//	//printf("fps %f\n",1000.0/(Time2-Time+1));
	//	//if (1000.0/(Time2-Time+1)>60)
	//		//_sleep(1000/100.0-(Time2-Time+1));
	//	Time=Time2;
	//}
}

int main2()
{	
	IConfigINI ini("config.ini");
	ini.Load();
	int Width = ini.ReadIntValue("Width",640);
	int Height = ini.ReadIntValue("Height",480);
	int Bits = ini.ReadIntValue("Bits",32);
	int Fullscreen = ini.ReadIntValue("Fullscreen",0);
	int AntiAliasing = ini.ReadIntValue("AntiAliasing",8);
	int UseShader = ini.ReadIntValue("UseShader",1);
	int UsePP = ini.ReadIntValue("UsePostProcess",1);
	if (ini.NeedToBeSave())
		ini.Save();	

	IDevice *device = Initial::CreateDevice(IVDT_OPENGL,Width,Height,Bits,Fullscreen,"Test");

	if (!device)
		return 1;

	//INode::GetStaticClass()->CreateInstance();
	printf("%s\n",INode::GetStaticClass()->GetName());
	printf("%d\n",INode::GetStaticClass()->IsKindOf(INode::GetStaticClass()));

	printf("Class count %d\n",IObject::ObjectsClass.Count());
	for (int i=0;i<IObject::ObjectsClass.Count();i++)
		if (IObject::ObjectsClass[i])
			printf("%s\n",IObject::ObjectsClass[i]->GetName());

	GUI::IFontManager fontManager;
	IFont* font=NULL;
	IFont* font2=NULL;
	fontManager.SetDevice(device);
	font2=fontManager.LoadFont("DS-DIGI.TTF",1);
	//font2=fontManager.LoadFont("african.ttf",2);
	//font2=fontManager.LoadFont("ALGER.TTF");
	//font2=fontManager.LoadFont("SLNTHLN.TTF");
	//font2=fontManager.LoadFont("SFOR.ttf");
	//font2=fontManager.LoadFont("BRADHITC.TTF");
	//font=fontManager.LoadFont("fable2.ttf",2);
	/*if (font==NULL)
		font=fontManager.LoadFont("c:/windows/fonts/arial.ttf",1);
	if (font2==NULL)
		font2=fontManager.LoadFont("c:/windows/fonts/comic.ttf",1);*/
	

	device->SetMouseLock(true);
	device->SetShowMouse(false);

	IRenderDriver *render = device->GetRenderDriver();

	//render->Resize(800,600);

	ICameraFPS camera(90,device->GetWidth()/(float)device->GetHeight());
	camera.Move(0,0,-2);
	//ICameraFree camera2(90,device->GetWidth()/(float)device->GetHeight());	

	ILight *lights[4];
	
	lights[0] = new ILight(IColor(0.5,0,0),IVector3D(-5.8,2.6,0.7),10,render);
	lights[1] = new ILight(IColor(0,0.5,0),IVector3D(0,2,0),10,render);
	lights[2] = new ILight(IColor(0,0,0.5),IVector3D(2,2,0),10,render);

	lights[3] = new ILight(IColor(0.8,0.8,0.64),IVector3D(-10,10,10),1,render); // Sun

	for (int i=0;i<10;i++)
		new ILight(IColor((rand()%1000)/1000.0,(rand()%1000)/1000.0,(rand()%1000)/1000.0),IVector3D((rand()%10)/1.0,(rand()%10)/1.0,(rand()%10)/1.0),5,render);


	ILight *mylight = lights[0];

	//IAnimatedMesh *animObject = new IAnimatedMesh(render,"Models/Skeleton/skeleton_head.cmf"/*,"Models/Skeleton/skeleton.csf","Models/Skeleton/skeleton_jog.caf"*/);
	////animObject->LoadAnimation("Models/skeleton/skeleton_jog.caf");
	//animObject->SetScale(0.02);
	//animObject->RotateX(DEG_TO_RAD(-90));
	//animObject->SetMaterial(IMaterial::LoadMaterial(render,"Materials/skeleton.ima"));

	IMesh *_3ds=NULL;
	IMesh *_3ds2=NULL;
	_3ds = I3DLoad::Load("marcus.3ds",render);
	_3ds2 = I3DLoad::Load("car.3ds",render);
	
	if (_3ds)
	{
		//_3ds->CalcNormals();
		//_3ds->SetFlags(IMesh::RF_ALWAYS_RENDER,false);
		//_3ds->SetFlags(RF_WIREFRAME,true);
		//_3ds->SetFlags(IMesh::RF_WIREFRAME_ONLY,true);
		//_3ds->SetFlags(RF_SHOW_NORMAL,true);	
		//_3ds->SetFlags(RF_SHOW_BOUNDING_BOX,true);
		//_3ds->SetFlags(RF_SHOW_CHILDREN_BOUNDING_BOX,true);		
		_3ds->SetScale(0.02);
		_3ds->RotateOrigX(DEG_TO_RAD(-90));
		//_3ds->RotateOrigZ(DEG_TO_RAD(180));
		_3ds->Translate(-3,0,0);
		//_3ds->SetColor(IColor(1,1,0));
		
		_3ds->SetMaterial(device->GetRessourceManager()->LoadMaterial("materials/marcus.ima"));
		if (_3ds->GetChild(1))
			_3ds->GetChild(1)->SetMaterial(device->GetRessourceManager()->LoadMaterial("materials/marcus-hair.ima"));
		if (_3ds->GetChild(2))
			_3ds->GetChild(2)->SetMaterial(device->GetRessourceManager()->LoadMaterial("materials/marcus-hair.ima"));
	}

	if (_3ds2)
	{
		//_3ds->CalcNormals();
		_3ds2->SetScale(0.1);
		_3ds2->RotateOrigX(DEG_TO_RAD(-90));
		_3ds2->Translate(-6,0,0);
		//_3ds->SetColor(IColor(1,1,0));
		_3ds2->SetMaterial(device->GetRessourceManager()->LoadMaterial("materials/car.ima"));
	}

	bool exit=false;
	int speed=1;
	while(device->Run() && exit==false)
	{
		//Light
		float speed=0.02;
		if (device->KeyDown(VK_NUMPAD4))
		{
			ILogger::LogDebug("orig %f %f %f\n",mylight->GetPosition().GetX(),mylight->GetPosition().GetY(),mylight->GetPosition().GetZ());
			mylight->SetPosition(mylight->GetPosition()+IVector3D(-speed,0,0));
			ILogger::LogDebug("new1 %f %f %f\n",mylight->GetPosition().GetX(),mylight->GetPosition().GetY(),mylight->GetPosition().GetZ());
			//ILogger::LogDebug("new2 %f %f %f\n",mylight->GetPosition().GetX(),mylight->GetPosition().GetY(),mylight->GetPosition().GetZ());
		}
		if (device->KeyDown(VK_NUMPAD6))
			mylight->SetPosition(mylight->GetPosition()+IVector3D(speed,0,0));
		if (device->KeyDown(VK_NUMPAD9))
			mylight->SetPosition(mylight->GetPosition()+IVector3D(0,speed,0));
		if (device->KeyDown(VK_NUMPAD3))
			mylight->SetPosition(mylight->GetPosition()+IVector3D(0,-speed,0));
		if (device->KeyDown(VK_NUMPAD8))
			mylight->SetPosition(mylight->GetPosition()+IVector3D(0,0,-speed));
		if (device->KeyDown(VK_NUMPAD2))
			mylight->SetPosition(mylight->GetPosition()+IVector3D(0,0,speed));

		if (device->KeyPress(VK_DIVIDE))
			mylight=lights[0];
		if (device->KeyPress(VK_MULTIPLY))
			mylight=lights[1];
		if (device->KeyPress(VK_SUBTRACT))
			mylight=lights[2];
		if (device->KeyPress(VK_ADD))
			mylight=lights[3];


		//Camera
		speed=1;
		if (device->KeyDown(VK_UP))
			camera.Move(0,0,0.1*speed);
		if (device->KeyDown(VK_DOWN))
			camera.Move(0,0,-0.1*speed);
		if (device->KeyDown(VK_LEFT))
			camera.Move(0.1*speed,0,0);
		if (device->KeyDown(VK_RIGHT))
			camera.Move(-0.1*speed,0,0);

		if (device->KeyDown(VK_NUMPAD0))
			camera.Move(0,0.1*speed,0);
		if (device->KeyDown(VK_CONTROL))
			camera.Move(0,-0.1*speed,0);

		camera.RotateY(device->GetMouseX()/100.0);
		camera.RotateX(device->GetMouseY()/100.0);
		//ILogger::LogWarning("%d %d\n",device->GetMouseX(),device->GetMouseY());

		render->BeginRender();
		render->SetCamera(&camera);

		render->Render(NULL,NULL);

		//animObject->Render(render);

		IBBox lightbox;
		lightbox.SetDimension(IVector3D(0.1,0.1,0.1));
		lightbox.SetCenter(lights[0]->GetPosition());
		render->_SetColor(IColor(1,0,0));
		lightbox.Render(render);

		lightbox.SetCenter(lights[1]->GetPosition());
		render->_SetColor(IColor(0,1,0));
		lightbox.Render(render);

		lightbox.SetCenter(lights[2]->GetPosition());
		render->_SetColor(IColor(0,0,1));
		lightbox.Render(render);

		lightbox.SetCenter(lights[3]->GetPosition());
		render->_SetColor(IColor(1,1,1));
		lightbox.Render(render);

		render->EndRender();

		if (device->KeyDown(VK_ESCAPE))
			exit=true;

		render->SwapBuffer();
	}

	delete device;
	return 0;
}