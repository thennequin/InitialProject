//---------------------------------------------
//                Initial Engine
//          Render Device source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/IRenderDriver.h"
#include "Video/OpenGLDrv/IRenderDriverOpenGL.h"

#include "3D/I3DObject.h"

using namespace Initial;

IRenderDriver* IRenderDriver::CreateDevice(VideoDriverType type,int Width,int Height,int BitDepth, bool Fullscreen, IString Title, int AntiaAliasing, bool UseShader)
{
	switch (type)
	{
	case VDT_OPENGL:
		IRenderDriver *device = new IRenderDriverOpenGL(Width,Height,BitDepth,Fullscreen,Title,AntiaAliasing,UseShader);
		return device;
	};

	return NULL;
}


IRenderDriver::IRenderDriver(int Width,int Height,int BitDepth, bool Fullscreen,IString Title, int AntiaAliasing, bool UseShader)
{
	m_iWidth = Width;
	m_iHeight = Height;
	m_iBitDepth = BitDepth;
	m_bFullscreen = Fullscreen;
	m_sTitle=Title;
	m_pCamera=NULL;
	m_bInRender=false;
	m_bLockMouse=true;
	m_bShowMouse=true;
	m_pCurrentMaterial=NULL;
	m_iAntiAliasing=AntiaAliasing;
	m_bAntiAliasingEnable=true;
	m_bUseShader=UseShader;
	m_pScreenTexture=NULL;
	m_pDepthTexture=NULL;
	if (m_iWidth<640)
		m_iWidth=640;

	if (m_iHeight<480)
		m_iHeight=480;

	if (m_iBitDepth<16)
		m_iBitDepth=16;
}

void IRenderDriver::Render()
{
	_SetColor(IColor());
	for (int i=0;i<I3DObject::m_aObjects.Count();i++)
	{
		if (I3DObject::m_aObjects[i] &&
			I3DObject::m_aObjects[i]->GetFlag(RF_ALWAYS_RENDER) &&
			I3DObject::m_aObjects[i]->GetParent()==NULL)
			I3DObject::m_aObjects[i]->Render();
	}
}

ITextureManager* IRenderDriver::GetTextureManager()
{
	return m_pTextureManager;
}

IShaderManager* IRenderDriver::GetShaderManager()
{
	return m_pShaderManager;
}

void IRenderDriver::SetCamera(ICamera *camera)
{
	m_pCamera=camera;
	if (camera && m_bInRender)
	{
		camera->UpdateView(this);
	}
}

int IRenderDriver::GetMouseAccX()
{
	return m_iMouseAccX;
}

int IRenderDriver::GetMouseAccY()
{
	return m_iMouseAccY;
}

void IRenderDriver::SetMouseLock(bool lock)
{
	m_bLockMouse=lock;
}

bool IRenderDriver::GetMouseLock()
{
	return m_bLockMouse;
}

void IRenderDriver::SetShowMouse(bool show)
{
	m_bShowMouse=show;
}

bool IRenderDriver::GetShowMouse()
{
	return m_bShowMouse;
}

int IRenderDriver::GetWidth()
{
	return m_iWidth;
}

int IRenderDriver::GetHeight()
{
	return m_iHeight;
}

unsigned int IRenderDriver::PolygonDrawsCount()
{
	return m_iPolygonCount;
}

unsigned int IRenderDriver::LineDrawsCount()
{
	return m_iLineCount;
}

void IRenderDriver::UseMaterial(IMaterial *mat)
{
	if (m_pCurrentMaterial!=mat)
	{		
		if (mat)
		{
			if (mat->GetParent());
				UseMaterial(mat->GetParent());
			
			if (mat->GetParent()==NULL || mat->GetParent()->GetShader()==NULL)
			{
				IShader *shader = mat->GetShader();
				if (m_pShaderManager)
				{
					m_pShaderManager->UseShader(shader);
					m_pShaderManager->SetParameterFloat(shader,"Time",GetTickCount()/1000.0);
				}
			}
			
			// Bind Texture
			for (int i=0;i<8;i++)
				if (mat->TextureIsDefine(i) || mat->GetParent()==NULL)
					m_pTextureManager->BindTexture(mat->GetTexture(i),i);
		}else{
			if (m_pShaderManager)
				m_pShaderManager->UseShader(NULL);
			for (int i=0;i<8;i++)
				m_pTextureManager->BindTexture(NULL,i);
		}
		m_pCurrentMaterial=mat;
	}
}
