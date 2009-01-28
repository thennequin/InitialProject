#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>
//#include <gl/gl.h>


#include "Initial.h"
//#include "OpenGL/IRenderDriverOpengl.h"
#include "Thread/IThread.h"

using namespace Initial;
using namespace Initial::GUI;
using namespace Initial::ShaderExpression;


#define PASS printf("\n");

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
	printf("test %d\n",!str);

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

I3DObject *MakeCube( IRenderDriver *device )
{
	I3DObject *cube = new I3DObject(NULL);

	if (cube)
	{
		ITriangle *pol;

		pol=new ITriangle;
		/*pol->SetColor(0,IColor(1,0,0));
		pol->SetColor(1,IColor(0,1,0));
		pol->SetColor(2,IColor(0,0,1));*/
		pol->SetVertex(0,IVertex(-0.5,-0.5,-0.5));
		pol->SetVertex(1,IVertex(0.5,-0.5,-0.5));
		pol->SetVertex(2,IVertex(-0.5,0.5,-0.5));
		pol->SetCoord(0,ICoord(1,1));
		pol->SetCoord(1,ICoord(0,1));
		pol->SetCoord(2,ICoord(1,0));
		//pol->SetMaterial(IMaterial::LoadMaterial(device,"brick.ima"));
		cube->AddPolygon(*pol);

		pol=new ITriangle;
		/*pol->SetColor(0,IColor(1,0,0));
		pol->SetColor(1,IColor(0,1,0));
		pol->SetColor(2,IColor(0,0,1));*/
		pol->SetVertex(0,IVertex(0.5,0.5,-0.5));
		pol->SetVertex(1,IVertex(0.5,-0.5,-0.5));
		pol->SetVertex(2,IVertex(-0.5,0.5,-0.5));
		pol->SetCoord(0,ICoord(0,0));
		pol->SetCoord(1,ICoord(0,1));
		pol->SetCoord(2,ICoord(1,0));
		//pol->SetMaterial(IMaterial::LoadMaterial(device,"brick.ima"));
		cube->AddPolygon(*pol);
		cube->SetMaterial(IMaterial::LoadMaterial(device,"brick.ima"));
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

int main(int argn, char *argv[])
{
	MyThread thread1(0);
	/*MyThread thread2(1);
	MyThread thread3(2);
	MyThread thread4(3);
	mutex.Lock();
	{		
		//IMutexLocker lock(mutex);
		thread1.Run();
		thread2.Run();
		thread3.Run();
		thread4.Run();
	}
	mutex.Unlock();

	_sleep(1500);
	
	system("PAUSE");
	exit(0);*/

	//string();
	matrix();

	IArray<IString> Array;
	Array.Add(IString("test"));
	Array.Add(IString("test2"));
	Array.Add(IString("test3"));
	Array.Add(IString("test4"));

	for (int i=0;i<10;i++)
		Array.Add("test"+IString(i+5));

	Array.Delete(1);
	Array[2]="test5";

	for (int i=0;i<Array.Count();i++)
	{
		printf("%s\n",Array[i].c_str());
	}
	printf("%s\n",Array[2].c_str());
	printf("%s\n",Array[3].c_str());
	printf("%s\n",Array[1].c_str());
	printf("%s\n",Array[1].c_str());

	//printf("tan 45 = %f\ntan 90 = %f\n",tan(45),tan(90));

	ISEVec2 A("A");
	ISEVec2 B("B");
	ISEVec3 Ok("Ok");
	ISEVec3 Else("Else");
	ISEVec3 res = If(A,B,"<",Ok,Else);

	printf("If = %s\n",res.GetGLSLString().c_str());

	system("PAUSE");
	//exit(0);

	IConfigINI ini("config.ini");
	ini.Load();
	int Width = ini.ReadIntValue("Width",640);
	int Height = ini.ReadIntValue("Height",480);
	int Bits = ini.ReadIntValue("Bits",32);
	int Fullscreen = ini.ReadIntValue("Fullscreen",0);
	int AntiAliasing = ini.ReadIntValue("AntiAliasing",8);
	int UseShader = ini.ReadIntValue("UseShader",1);
	ini.Save();	

	IRenderDriver* device = CreateDevice(VDT_OPENGL,Width,Height,Bits,Fullscreen,"Initial Engine",AntiAliasing,UseShader);
	if (!device)
	{
		system("PAUSE");
		return 1;
	}
	device->SetMouseLock(false);
	device->SetShowMouse(false);
	//device->SetShowMouse(true);

	//device->LoadTexture("brick_normal.png");
	
	ITexture *destTex = device->GetTextureManager()->CreateTexture(1024,1024);
	ITexture *destTex2 = device->GetTextureManager()->CreateTexture(1024,1024);
	IMaterial *destMat = IMaterial::LoadMaterialFromTexture(device,destTex);
	destMat->SetTexture(destTex2,1);

	ITexture *videoTex = device->GetTextureManager()->LoadVideo("test.bik");
	//ITexture *videoTex = device->GetTextureManager()->LoadTexture("WETSUIT.jpg");
	//videoTex->UpdateImage();
	//destMat->SetTexture(videoTex,0);

	ICameraFPS camera(90,device->GetWidth()/(float)device->GetHeight());
	//ICameraFree camera2(90,device->GetWidth()/(float)device->GetHeight());	
	ICameraFPS camera2(90,1); // Camera du render to texture avec un aspect ratio de 1
	//camera.Move(2,0,0);
	//camera2.Move(-2,0,0);
	//device->SetCamera(&camera);
	//device->SetCamera(NULL);

	IMaterial *mat = IMaterial::LoadMaterial(device,"brick.ima");

	I3DObject *_3ds=NULL;
	//_3ds = I3DLoad3DS::Load3DS("chair_01.3DS",device);
	_3ds = I3DLoad3DS::Load3DS("car.3ds",device);
	//_3ds = I3DLoad3DS::Load3DS("lara01.3DS",device);
	//_3ds = I3DLoad3DS::Load3DS("M1_Abrams.3DS",device);
	//_3ds = I3DLoad3DS::Load3DS("TIEf3DS8.3ds",device);
	//_3ds = I3DLoad3DS::Load3DS("dreezle.3ds",device);
	//_3ds = I3DLoad3DS::Load3DS("ElephantBody.3ds",device);
	//_3ds = I3DLoad3DS::Load3DS("Projet_tut_IUT.3ds",device);
	//_3ds = I3DLoad3DS::Load3DS("COCACOLA.3DS",device);
	//_3ds = I3DLoad3DS::Load3DS("Untitled.3ds",device);	
	
	if (_3ds)
	{
		//_3ds->SetFlags(I3DObject::RF_ALWAYS_RENDER,false);
		_3ds->SetFlags(RF_WIREFRAME,true);
		//_3ds->SetFlags(I3DObject::RF_WIREFRAME_ONLY,true);
		//_3ds->SetFlags(I3DObject::RF_SHOW_NORMAL,true);	
		_3ds->SetFlags(RF_SHOW_BOUNDING_BOX,true);
		_3ds->SetFlags(RF_SHOW_CHILDREN_BOUNDING_BOX,true);		
		_3ds->SetScale(0.2);
		_3ds->RotateOrigX(DEG_TO_RAD(-90));
		_3ds->Translate(-3,0,0);
		//_3ds->SetColor(IColor(1,1,0));
		_3ds->SetMaterial(IMaterial::LoadMaterial(device,"car.ima"));
		//_3ds->SetMaterial(destMat);
	}

	I3DObject *cube=NULL;
	cube = MakeCube(device);
	if (cube)
	{
		cube->SetDevice(device);
		//cube->SetFlags(RF_ALWAYS_RENDER,false);
		cube->SetFlags(RF_WIREFRAME,true);
		//cube->SetFlags(RF_WIREFRAME_ONLY,true);
		//cube->SetFlags(RF_WHITE_WIREFRAME,false);
		//cube->SetFlags(RF_SHOW_NORMAL,true);
		//cube->SetFlags(RF_SHOW_BOUNDING_BOX,true);
		
		//cube->TranslateOrig(0,0,-10);
		cube->SetScale(3);

		cube->SetMaterial(destMat);
		
		//cube->RotateOrigX(-50);
		cube->RotateZ(DEG_TO_RAD(180));
	}

	I3DObject *cube2=NULL;
	cube2 = MakeCube(device);
	if (cube2)
	{
		cube2->SetDevice(device);
		cube2->SetFlags(RF_WIREFRAME,true);
		cube2->TranslateOrig(5,0,0);
		cube2->SetScale(3);
		cube2->GetMaterial()->SetTexture(videoTex,0);
	}

	GUI::IFontManager fontManager;
	IFont* font=NULL;
	IFont* font2=NULL;
	fontManager.SetRenderDevice(device);
	font2=fontManager.LoadFont("DS-DIGI.TTF",2);
	//font2=fontManager.LoadFont("african.ttf",2);
	//font2=fontManager.LoadFont("ALGER.TTF");
	//font2=fontManager.LoadFont("SLNTHLN.TTF");
	//font2=fontManager.LoadFont("SFOR.ttf");
	//font2=fontManager.LoadFont("BRADHITC.TTF");
	font=fontManager.LoadFont("fable2.ttf",2);
	if (font==NULL)
		font=fontManager.LoadFont("c:/windows/fonts/arial.ttf",2);
	if (font2==NULL)
		font2=fontManager.LoadFont("c:/windows/fonts/comic.ttf",2);
	

	IArray<IPolygon> contours;
	contours.Add(IPolygon());
	contours[0].AddPoint(IVertex(0,0));
	contours[0].AddPoint(IVertex(1,0));
	contours[0].AddPoint(IVertex(1,1));
	contours[0].AddPoint(IVertex(0,1));
	contours[0].AddPoint(IVertex(-0.5,0.5));

	contours.Add(IPolygon());
	contours[1].AddPoint(IVertex(0.25,0.25));
	contours[1].AddPoint(IVertex(0.75,0.25));
	contours[1].AddPoint(IVertex(0.75,0.75));
	contours[1].AddPoint(IVertex(0.25,0.75));

	contours.Add(IPolygon());
	contours[2].AddPoint(IVertex(0.875,0.5));
	contours[2].AddPoint(IVertex(0.875,0.75));
	contours[2].AddPoint(IVertex(1.5,0.75));
	contours[2].AddPoint(IVertex(1.5,0.5));

	ITriangulator triangulate(contours);
	IArray<ITriangle> result;
	//triangulate.Triangulate(result);

	printf("Contours count %d\nResult %d",contours.Count(),result.Count());
	/*for (int i=0;i<contours.Count();i++)
	{
		contours[i]->Clear(true);
	}
	contours.Clear(true);*/

	float zoom=90;
	bool exit=false;
	int speed=1;
	unsigned long Time = GetTickCount();
	unsigned long Begin = Time; 
	float FPS=0;
	ICamera *selectedcamera=NULL;
	while (device->UpdateDevice() && exit==false)
	{
		//videoTex->UpdateImage();

		if (device->KeyPress(VK_ESCAPE))
			exit=true;
		if (device->KeyPress(VK_F1))
		{
			//device->SetCamera(NULL);
			device->SetMouseLock(false);
			device->SetShowMouse(true);
		}
		else if (device->KeyPress(VK_F2))
		{
			device->SetMouseLock(true);
			device->SetShowMouse(false);
			device->UpdateDevice();
			//device->SetCamera(&camera);		
			selectedcamera=&camera;
		}
		else if (device->KeyPress(VK_F3))
		{
			device->SetMouseLock(true);
			device->SetShowMouse(false);
			device->UpdateDevice();
			//device->SetCamera(&camera2);		
			selectedcamera=&camera2;
		}

		if (device->KeyPress(VK_F4))
			device->SetAntialising(1);
		else if (device->KeyPress(VK_F5))
			device->SetAntialising(0);

		if (device->KeyPress(VK_F6) && cube)
			cube->SetFlags(RF_ALWAYS_RENDER,!(cube->GetFlag(RF_ALWAYS_RENDER)));

		if (device->KeyPress(VK_F7) && _3ds)
			_3ds->SetFlags(RF_ALWAYS_RENDER,!(_3ds->GetFlag(RF_ALWAYS_RENDER)));

		if (device->MousePress(0))
		{
			//printf("press\n");
			zoom-=1;
			if (zoom<40)
				zoom=40;
		}else{
			zoom+=1;
			if (zoom>90)
				zoom=90;
		}

		//ICamera *cam = device->GetCamera();
		if (selectedcamera)
		{
			if (device->KeyDown(VK_SHIFT))
				speed=2;
			else
				speed=1;

			if (device->KeyDown(VK_UP))
				selectedcamera->Move(0,0,0.1*speed);
			if (device->KeyDown(VK_DOWN))
				selectedcamera->Move(0,0,-0.1*speed);
			if (device->KeyDown(VK_LEFT))
				selectedcamera->Move(0.1*speed,0,0);
			if (device->KeyDown(VK_RIGHT))
				selectedcamera->Move(-0.1*speed,0,0);

			if (device->KeyDown(VK_NUMPAD0))
				selectedcamera->Move(0,0.1*speed,0);
			if (device->KeyDown(VK_CONTROL))
				selectedcamera->Move(0,-0.1*speed,0);

			selectedcamera->RotateY(device->GetMouseAccX()/100.0);
			selectedcamera->RotateX(device->GetMouseAccY()/100.0);

			selectedcamera->SetFov(zoom);
		}

		device->BeginRender();
			device->SetCamera(&camera2);
			device->Render();
			fontManager.RenderText(font,"Texte dans un RenderToTexture");
		device->EndRender(destTex);

		device->BeginRender();
			device->SetCamera(&camera);
			/*device->_DrawLine(IVertex(),IVertex(1,0,0),IColor(1,0,0));
			device->_DrawLine(IVertex(),IVertex(0,1,0),IColor(0,1,0));
			device->_DrawLine(IVertex(),IVertex(0,0,1),IColor(0,0,1));*/
			//cube->RotateOrigX(DEG_TO_RAD(0.25));
			//cube->TranslateOrig(0.1,0,0);
			
			//cube->RotateOrigZ(DEG_TO_RAD(0.25));
			device->Render();

			device->UseMaterial(NULL);
			for (int i=0;i<result.Count();i++)
			{
				device->_StartPolyDraw(false);
					device->_DrawPolygon(&result[i]);
				device->_EndPolyDraw();
			}
			for (int i=0;i<result.Count();i++)
			{
				device->_SetColor(IColor(1,0,0));
				device->_DrawLine(result[i].GetVertex()[0],result[i].GetVertex()[1]);
				device->_SetColor(IColor(0,1,0));
				device->_DrawLine(result[i].GetVertex()[1],result[i].GetVertex()[2]);
				device->_SetColor(IColor(0,0,1));
				device->_DrawLine(result[i].GetVertex()[2],result[i].GetVertex()[0]);
			}

			IString str;

			device->_SetColor(IColor(1,0,0));
			const float Quality=50.0;
			for (int i=0;i<Quality;i++)
			{
				/*const double x=0.085;
				const double y=0.915;*/
				const double x=0.085;
				const double y=0.915;
				/*device->_DrawLine(	RenderConicBezier(IVertex(1,0,1),IVertex(1,0,1),IVertex(1,1,1),i/Quality),
									RenderConicBezier(IVertex(1,0,1),IVertex(1,0,1),IVertex(1,1,1),(i+1)/Quality));*/
				device->_DrawLine(	RenderConicBezier(IVertex(0,0,1),IVertex(1,1,1),IVertex(x,y,1),i/Quality),
									RenderConicBezier(IVertex(0,0,1),IVertex(1,1,1),IVertex(x,y,1),(i+1)/Quality));
			}

			device->_SetColor(IColor(0,1,0));
			for (int i=0;i<Quality;i++)
			{
				device->_DrawLine(	IVertex(1)+IVertex(cos((i/Quality)*I_PIE/2+I_PIE/2),
									sin((i/Quality)*I_PIE/2+I_PIE/2),1),
									IVertex(1)+IVertex(cos(((i+1)/Quality)*I_PIE/2+I_PIE/2),
									sin(((i+1)/Quality)*I_PIE/2+I_PIE/2),1));
			}

			IFontDrawParam param;
			param.m_bOutline=true;
			//param.m_cColor=IColor(1,1,1);
			param.m_cOutlineColor=IColor(0,0,0);
			//param.m_fOutlineSize=2;
			str.Printf("\n\n\n\n\n\nFable 2\nBonjour je suis Thibault\nCréateur de ce	programme\n^$ù*=)&é_çà'é-('");
			fontManager.RenderText(font,str,param);

			param.m_cColor=IColor(0,128.0/255,0);
			//param.m_cOutlineColor=IColor(0,92.0/255,0);
			param.m_cOutlineColor=IColor();
			param.m_cShadowColor=IColor(0,32.0/255,0,0.5);
			param.m_fOutlineSize=2;
			param.m_fSize=1.5;
			//param.m_bFixedSpace=true;
			param.m_fFixedSpaceWidth=0.4;
			//str.Printf("%d polygons\n%d lines\n%0.1f FPS",device->PolygonDrawsCount(),device->LineDrawsCount(),FPS);
			float Speed=1;
			float Start=(GetTickCount()*Speed-Begin*Speed)/1000.0;
			int hour = Start/3600;
			int min = Start/60 - hour*60;
			int sec = Start - min*60 - hour*3600;
			int ms = (Start - sec - min*60 - hour*3600)*100;
			str.Printf("%02d:%02d:%02d:%02d\n%d polygons\n%d lines\n%0.1f FPS",hour, min, sec, ms, device->PolygonDrawsCount(),device->LineDrawsCount(),FPS);
			fontManager.RenderText(font2,str,param);

		device->EndRender();
		
		unsigned long Time2=GetTickCount();
		FPS=(FPS+1000/(Time2-Time+1))/2.0;
		//printf("fps %f\n",1000.0/(Time2-Time+1));
		//if (1000.0/(Time2-Time+1)>60)
			//_sleep(1000/100.0-(Time2-Time+1));
		Time=Time2;
	}
}