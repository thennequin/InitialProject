//---------------------------------------------
//                Initial Engine
//              Image loader Module
//
//            By Thibault HENNEQUIN
//              January 12th  2008
//---------------------------------------------

#include <windows.h>
#include "Core/IImage.h"

using namespace Initial;

void IImage::_Init()
{
	m_iWidth=0;
	m_iHeight=0;
	m_iFormat=IF_NONE;
	m_pData=NULL;
}

IImage::IImage()
{
	_Init();
}

IImage::IImage(IString filename)
{
	_Init();
	gflLibraryInit();
	GFL_LOAD_PARAMS load_option; 
	GFL_BITMAP *bitmap; 
	GFL_FILE_INFORMATION file_info; 
	GFL_ERROR error; 

	gflGetDefaultLoadParams( &load_option ); 
	//load_option.ColorModel = GFL_RGBA; 
	
	error = gflLoadBitmap( filename, &bitmap, &load_option, &file_info ); 
	if ( error )
	{
		
	}else
	{
		int CPP=0;
		bool Error=false;
		if (bitmap->ComponentsPerPixel==3)
			m_iFormat=IF_RGB;
		else if (bitmap->ComponentsPerPixel==4)
			m_iFormat=IF_RGBA;
		else{
			printf("Format non supporte\n");
			Error=true;
		}

		if (Error==false)
		{
			m_iWidth=bitmap->Width;
			m_iHeight=bitmap->Height;
			m_pData=malloc(m_iWidth*m_iHeight*m_iFormat);
			memcpy(m_pData,bitmap->Data,m_iWidth*m_iHeight*m_iFormat);
		}
		gflFreeBitmap( bitmap );
	}
	gflLibraryExit();
}

IImage::~IImage()
{
	if (m_pData)
		free(m_pData);
}

void* IImage::GetData()
{
	return m_pData;
}

ImageFormat IImage::GetFormat()
{
	return m_iFormat;
}

int IImage::GetWidth()
{
	return m_iWidth;
}

int IImage::GetHeight()
{
	return m_iHeight;
}

void IImage::SetImage(int width, int height, ImageFormat format)
{
	m_iWidth=width;
	m_iHeight=height;
	m_iFormat=format;
	if (m_pData)
		free(m_pData);	
	m_pData=malloc(m_iWidth*m_iHeight*m_iFormat);
}

void IImage::SetData(void *data)
{
	if (m_pData && data)
	{
		memcpy(m_pData,data,m_iWidth*m_iHeight*m_iFormat);
	}
}