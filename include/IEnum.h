//---------------------------------------------
//                Initial Engine
//                 Enum  Module
//
//            By Thibault HENNEQUIN
//              January 16 2009

#ifndef _IENUM_HEADER_
#define _IENUM_HEADER_


namespace Initial
{

	enum VideoDriverType
	{
		VDT_OPENGL,
		VDT_MAX
	};

	enum DeviceExt
	{
		EXT_TEXTURE_2D,
		EXT_DEPTH_TEST,
		EXT_BLEND,
		EXT_DEPTH,
		EXT_MAX
	};

	enum TextureFormat
	{
		TF_RGB=1,
		TF_RGBA,
		TF_RED,
		TF_GREEN,
		TF_BLUE,
		TF_ALPHA,
		TF_DEPTH,
		TF_MAX              
	};

	enum ImageFormat
	{
		IF_NONE=0,
		IF_RGB=3,
		IF_RGBA=4
	};

	enum RenderFlags
	{
		RF_ALWAYS_RENDER				= 1<<1,
		RF_SHOW_BOUNDING_BOX			= 1<<2,
		RF_SHOW_CHILDREN_BOUNDING_BOX	= 1<<3,
		RF_SHOW_NORMAL					= 1<<4,
		RF_WIREFRAME					= 1<<5,
		RF_WIREFRAME_ONLY				= 1<<6,
		RF_WHITE_WIREFRAME				= 1<<7,

		RF_MAX
	};


}

#endif