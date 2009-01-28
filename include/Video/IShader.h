//---------------------------------------------
//                Initial Engine
//                Shader  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ISHADER_HEADER_
#define _ISHADER_HEADER_

#include "Core/IString.h"
#include "Video/ITexture.h"
#include "Video/IShaderManager.h"

namespace Initial
{
	class IShaderManager;

	class IShader
	{
		friend class IShaderManager;
	public:		
		~IShader();

		IString GetFragmentCode();
		IString GetVertexCode();

		void SetFragmentCode(IString Code);
		void SetVertexCode(IString Code);

		int GetProgram();

		//Compile Shader
		bool IsCompiled();
		bool Compile();
		IString ReturnError();
	protected:
		IShader(IShaderManager *manager, IString vertex, IString fragment);
	protected:
		IString m_sShaderName;

		// Shader Code
		IString m_sFragmentCode;
		IString m_sVertexCode;

		// Shader
		int m_iFragmentShader;
		int m_iVertexShader;
		int m_iProgram;

		IShaderManager *m_pShaderManager;
	};

}

#endif