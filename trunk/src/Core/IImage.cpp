//---------------------------------------------
//                Initial Engine
//              Image loader Module
//
//            By Thibault HENNEQUIN
//              January 12th  2008
//---------------------------------------------

#define _IIMAGE_COMPILE_

#include <windows.h>
#include <libgfl.h>

#include "Initial/ILogger.h"
#include "Initial/Core/IImage.h"

#include "Initial/Core/Image/TGA.h"

namespace Initial
{
	namespace Core
	{
		void IImage::_Init()
		{
			m_iWidth=0;
			m_iHeight=0;
			m_iFormat=IIF_NONE;
			m_pData=NULL;
		}

		IImage::IImage()
		{
			_Init();
		}

		IImage::IImage(IImage& copy)
		{
			_Init();
			*this=copy;
		}

		IImage::IImage(IString filename)
		{
			_Init();
			Load(filename);
		}

		IImage::~IImage()
		{
			if (m_pData)
				free(m_pData);
		}

		unsigned char* IImage::GetData()
		{
			return (unsigned char*)m_pData;
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

		bool IImage::Load(IString filename)
		{
			gflLibraryInit();
			GFL_LOAD_PARAMS load_option; 
			GFL_BITMAP *bitmap; 
			GFL_FILE_INFORMATION file_info; 
			GFL_ERROR error; 

			gflGetDefaultLoadParams( &load_option ); 
			//load_option.ColorModel = GFL_RGBA; 
			
			error = gflLoadBitmap( filename.c_str(), &bitmap, &load_option, &file_info ); 
			if ( error )
			{
				error = gflLoadBitmap( ("Textures/"+filename).c_str(), &bitmap, &load_option, &file_info ); 
			}
			
			if ( error )
			{
				printf("Pas de fichier trouver\n");	
			}else
			{
				//GFL_SAVE_PARAMS save_option; 
				//gflGetDefaultSaveParams(&save_option);
				//gflSaveBitmap("c:/test.bmp",bitmap,&save_option);

				int CPP=0;
				bool Error=false;
				if (bitmap->ComponentsPerPixel==3)
					m_iFormat=IIF_RGB;
				else if (bitmap->ComponentsPerPixel==4)
					m_iFormat=IIF_RGBA;
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
				return true;
			}
			gflLibraryExit();
			return false;
		}

		void IImage::Save(IString filename)
		{
			Image::SaveTGA(filename,this);
		}

		void IImage::operator =(IImage &copy)
		{
			SetImage(copy.m_iWidth,copy.m_iHeight,copy.m_iFormat);
			SetData(copy.m_pData);
		}
	}
}
