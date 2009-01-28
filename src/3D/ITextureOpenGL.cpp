//---------------------------------------------
//                Initial Engine
//             Texture source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/OpenGLDrv/ITextureManagerOpenGL.h"

//
//#include "Texture.h"
//#include "libgfl.h"
//
//wxArrayTexture Texture::Textures;
//
//Texture_t *Texture::TexturesList[MAX_TEXTURE];
//
///*for (int i=0;i<MAX_TEXTURE;i++)
//{
//	Texture::TexturesList[i]=NULL;
//}*/
//
//Texture_t *Texture::LoadTexture(wxString filename,bool forceReload, wxString path)
//{
//	bool Exist=false;
//	int ExistId=0;
//	Texture_t *NewTex;
//
//	for (unsigned int i=0;i<Textures.Count();i++)
//	{
//		if (Textures[i]->Filename==filename)
//		{
//			Exist=true;
//			ExistId=i;
//			break;
//		}
//	}
//
//	if (Exist && forceReload==false)
//	{
//		return Textures[ExistId];
//	}else if (Exist && forceReload==true)
//	{
//		NewTex=Textures[ExistId];
//	}else{
//		NewTex = new Texture_t;
//		NewTex->Filename=filename;
//		Textures.Add(NewTex);
//		glGenTextures(1, &(NewTex->TextureId));	// Create The Texture
//	}
//
//	//Load Texture
//
//	gflLibraryInit();
//	GFL_LOAD_PARAMS load_option; 
//	GFL_BITMAP *bitmap; 
//	GFL_FILE_INFORMATION file_info; 
//	GFL_ERROR error; 
//
//	gflGetDefaultLoadParams( &load_option ); 
//	//load_option.ColorModel = GFL_RGBA; 
//	
//	error = gflLoadBitmap( filename, &bitmap, &load_option, &file_info ); 
//	if ( error )
//	{
//		//DisplayError( filename, error ); 
//		wxLogMessage("Error : %s\n%d",filename,error);
//		return 0;
//	}else
//	{	
//		//wxLogMessage("Size %d %d",bitmap->Width,bitmap->Height);
//		//return NewTex->TextureId;
//		NewTex->Thumb = new wxImage(bitmap->Width,bitmap->Height,bitmap->Data);
//		
//
//		glBindTexture(GL_TEXTURE_2D, NewTex->TextureId);
//
//		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
//		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//
//		int CPP=0;
//		bool Error=false;
//		if (bitmap->ComponentsPerPixel==3)
//			CPP=GL_RGB;
//		else if (bitmap->ComponentsPerPixel==4)
//			CPP=GL_RGBA;
//		else
//			Error=true;
//
//		if (Error==false)
//		{
//			glTexImage2D(GL_TEXTURE_2D, GL_RGBA, 3, bitmap->Width, bitmap->Height, 0, CPP, GL_UNSIGNED_BYTE, bitmap->Data);
//			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bitmap->Width, bitmap->Height, CPP, GL_UNSIGNED_BYTE, bitmap->Data);
//			gflFreeBitmap( bitmap );
//			gflLibraryExit();
//			glBindTexture(GL_TEXTURE_2D, NULL);
//			return NewTex;
//		}
//		return 0;
//	}
//}