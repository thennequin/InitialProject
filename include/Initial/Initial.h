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

//#include		"IMemoryManager.h"
#include		"ILogger.h"
#include		"IObject.h"
#include		"IProperty.h"
#include		"ISmartPtr.h"
#include		"IRessource.h"
#include		"IRessourceManager.h"
#include		"ISceneManager.h"
#include		"IDevice.h"
#include		"IDeviceWin32.h"

#include		"Core/IArray.h"
#include		"Core/IList.h"
#include		"Core/IString.h"
#include		"Core/IUtil.h"
#include		"Core/IColor.h"
#include		"Core/IVector3D.h"
#include		"Core/IVector3D.h"

#include		"IO/IFileStream.h"
#include		"IO/IConfigINI.h"
#include		"IO/IXMLParser.h"

#include		"Math/IMath.h"
#include		"Math/IMatrix.h"

#include		"Format/IImageITX.h"
#include		"Format/IMeshIEM.h"

#include		"3D/IMesh.h"
#	include		"3D/IBBox.h"
#	include		"3D/ITriangle.h"
#	include		"3D/ILine.h"
#include		"3D/ILight.h"
#include		"3D/ITriangulator.h"
#include		"3D/IPolygon.h"

#include		"3D/IAnimatedMesh.h"

#include		"3D/Loader/I3DLoad3DS.h"
	
#include		"3D/Camera/ICamera.h"
#	include		"3D/Camera/ICameraFree.h"
#	include		"3D/Camera/ICameraFPS.h"

#include		"Node/INode.h"
#include		"Node/INodeLight.h"
#include		"Node/INodeSkybox.h"
#include		"Node/INode3DObject.h"
#include		"Node/INode3DAnimatedMesh.h"

#include		"GUI/IFontManager.h"
#	include		"GUI/IFont.h"

#include		"Video/IRenderDriver.h"
#include		"Video/IShaderExpression.h"
#include		"Video/IMaterial.h"
#	include		"Video/ITexture.h"
#	include		"Video/ITextureVideo.h"
#	include		"Video/IShader.h"

// OpenGL Render
#include		"Video/OpenGLDrv/IRenderDriverOpenGL.h"
#include		"Video/OpenGLDrv/ITextureManagerOpenGL.h"
#include		"Video/OpenGLDrv/IShaderManagerOpenGL.h"

namespace Initial
{
	IDevice *CreateDevice(VideoDriverType type,int Width,int Height,int BitDepth, bool Fullscreen, Core::IString Title);

	IDevice *CreateDeviceEx(VideoDriverType type, int BitDepth, void *WinId);
}

#endif