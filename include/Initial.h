//---------------------------------------------
//                Initial Engine
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INITIAL_HEADER_
#define _INITIAL_HEADER_

#include		"Config.h"
#include		"IEnum.h"

#include		"Core/IString.h"
#include		"Core/IConfigINI.h"

#include		"Core/IMath.h"
#include		"Core/IMatrix.h"

#include		"Core/IColor.h"

#include		"Video/IMaterial.h"
#	include		"Video/ITexture.h"
#	include		"Video/ITextureVideo.h"
#	include		"Video/IShader.h"

#include		"Video/IRenderDriver.h"

#include		"3D/I3DObject.h"
#	include		"3D/IBBox.h"
#	include		"3D/ITriangle.h"
#		include "3D/IVertex.h"
#		include "3D/INormal.h"
#		include "3D/ICoord.h"
#	include		"3D/ILine.h"

#include		"3D/Loader/I3DLoad3DS.h"
	
#include		"3D/Camera/ICamera.h"
#	include		"3D/Camera/ICameraFree.h"
#	include		"3D/Camera/ICameraFPS.h"

#include		"Core/IShaderExpression.h"

#include		"Core/IDevice.h"
#include		"Core/IDeviceWin32.h"

// OpenGL Render
#include		"Video/OpenGLDrv/IRenderDriverOpenGL.h"
#include		"Video/OpenGLDrv/ITextureManagerOpenGL.h"
#include		"Video/OpenGLDrv/IShaderManagerOpenGL.h"

#include		"GUI/IFontManager.h"
#	include		"GUI/IFont.h"

#include		"Core/ITriangulator.h"
#include		"3D/IPolygon.h"

namespace Initial
{
	IRenderDriver *CreateDevice(VideoDriverType type,int Width,int Height,int BitDepth, bool Fullscreen, IString Title, int AntiAliasing=0, bool UseShader=true);
}

#endif