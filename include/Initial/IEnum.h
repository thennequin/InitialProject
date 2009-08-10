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
		IVDT_OPENGL,

		IVDT_MAX
	};

	enum DeviceExt
	{
		IEXT_TEXTURE_2D=0,
		IEXT_DEPTH_TEST,
		IEXT_BLEND,
		IEXT_DEPTH,
		IEXT_COLOR,
		IEXT_STENCIL_TEST,

		IEXT_MAX
	};

	enum VideoBuffer
	{
		IVB_COLOR=0,
		IVB_DEPTH,
		IVB_STENCIL,

		IVB_MAX
	};

	enum TextureFormat
	{
		ITF_RGB=1,
		ITF_RGBA,
		ITF_RED,
		ITF_GREEN,
		ITF_BLUE,
		ITF_ALPHA,
		ITF_DEPTH,

		ITF_RGB32F,
		ITF_RGBA32F,
		ITF_RGB16F,
		ITF_RGBA16F,
		ITF_DEPTH24,
		ITF_DEPTH32,

		ITF_MAX              
	};

	enum ImageFormat
	{
		IIF_NONE=0,
		IIF_GREY=1,
		IIF_RGB=3,
		IIF_RGBA=4,

		IIF_MAX
	};

	enum RenderFlags
	{
		IRF_ALWAYS_RENDER				= 1<<1,
		IRF_SHOW_BOUNDING_BOX			= 1<<2,
		IRF_SHOW_CHILDREN_BOUNDING_BOX	= 1<<3,
		IRF_SHOW_NORMAL					= 1<<4,
		IRF_WIREFRAME					= 1<<5,
		IRF_WIREFRAME_ONLY				= 1<<6,
		IRF_WHITE_WIREFRAME				= 1<<7,
		IRF_SHADOW_MAP_RENDER			= 1<<8,

		IRF_MAX
	};

	enum MaterialType
	{
		IMT_MAX	
	};

	enum BlendMode
	{
		IBM_ZERO=0,
		IBM_ONE,
		IBM_SRC_COLOR,
		IBM_ONE_MINUS_SRC_COLOR,
		IBM_SRC_ALPHA,
		IBM_ONE_MINUS_SRC_ALPHA,
		IBM_DST_ALPHA,
		IBM_ONE_MINUS_DST_ALPHA,
		IBM_DST_COLOR,
		IBM_ONE_MINUS_DST_COLOR,
		IBM_SRC_ALPHA_SATURATE
	};

	enum AlphaFunc
	{
		IAF_NEVER=0,
		IAF_LESS,
		IAF_EQUAL,
		IAF_LEQUAL,
		IAF_GREATER,
		IAF_NOTEQUAL,
		IAF_GEQUAL,
		IAF_ALWAYS,
		
		IAF_MAX
	};

	enum StencilOp
	{
		ISO_KEEP=0,
		ISO_REPLACE,
		ISO_INCR,
		ISO_DECR,

		ISO_MAX
	};

	enum FrontFaceDirection
	{
		IFFD_CW=0,
		IFFD_CCW,

		IFFD_MAX
	};

	enum VBOBufferType
	{
		IVBOBT_VERTEX,
		IVBOBT_NORMALS,
		IVBOBT_COORD,
		IVBOBT_TANGENT,
		IVBOBT_INDEX
	};

	enum VBOMode
	{
		IVBOM_STREAM,
		IVBOM_DYNAMIC,
		IVBOM_STATIC
	};

	enum PropertyType
	{
		IPT_BOOL,
		IPT_INTEGER,
		IPT_FLOAT,
		IPT_VEC2,
		IPT_VEC3,
		IPT_COLOR,
		IPT_STRING,
		IPT_CHOICE,
		//IPT_TEXTURE,
		IPT_RESSOURCE_ITX, // Texture
		IPT_RESSOURCE_IMA, // Material
		IPT_RESSOURCE_IEM, // Animated mesh
	};

	enum KeyCode
	{
		IKC_BACK=1,
		IKC_TAB,
		IKC_RETURN,
		IKC_ESCAPE,
		IKC_SPACE,
		IKC_DELETE,

		//IKC_CLEAR,
		IKC_SHIFT,
		IKC_ALT,
		IKC_CONTROL,
		//IKC_MENU,
		IKC_PAUSE,
		IKC_CAPITAL,
		IKC_END,
		IKC_HOME,

		IKC_LEFT,
		IKC_UP,
		IKC_RIGHT,
		IKC_DOWN,

		IKC_SELECT,
		IKC_PRINT,
		IKC_EXECUTE,
		IKC_SNAPSHOT,
		IKC_INSERT,
		IKC_HELP,

		IKC_NUMPAD0,
		IKC_NUMPAD1,
		IKC_NUMPAD2,
		IKC_NUMPAD3,
		IKC_NUMPAD4,
		IKC_NUMPAD5,
		IKC_NUMPAD6,
		IKC_NUMPAD7,
		IKC_NUMPAD8,
		IKC_NUMPAD9,
		IKC_MULTIPLY,
		IKC_ADD,
		IKC_SEPARATOR,
		IKC_SUBTRACT,
		IKC_DECIMAL,
		IKC_DIVIDE,

		IKC_F1,
		IKC_F2,
		IKC_F3,
		IKC_F4,
		IKC_F5,
		IKC_F6,
		IKC_F7,
		IKC_F8,
		IKC_F9,
		IKC_F10,
		IKC_F11,
		IKC_F12,
		IKC_F13,
		IKC_F14,
		IKC_F15,
		IKC_F16,
		IKC_F17,
		IKC_F18,
		IKC_F19,
		IKC_F20,
		IKC_F21,
		IKC_F22,
		IKC_F23,
		IKC_F24,

		IKC_NUMLOCK,
		IKC_SCROLL,
		IKC_PAGEUP,
		IKC_PAGEDOWN,

		/*IKC_NUMPAD_END,
		IKC_NUMPAD_BEGIN,
		IKC_NUMPAD_INSERT,
		IKC_NUMPAD_DELETE,
		IKC_NUMPAD_EQUAL,
		IKC_NUMPAD_MULTIPLY,
		IKC_NUMPAD_ADD,
		IKC_NUMPAD_SEPARATOR,
		IKC_NUMPAD_SUBTRACT,
		IKC_NUMPAD_DECIMAL,
		IKC_NUMPAD_DIVIDE,*/
	};
}

#endif