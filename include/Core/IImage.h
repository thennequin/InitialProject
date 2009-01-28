//---------------------------------------------
//                Initial Engine
//              Image loader Module
//
//            By Thibault HENNEQUIN
//              January 12th  2008
//---------------------------------------------

#ifndef _IIMAGE_HEADER_
#define _IIMAGE_HEADER_

#include <windows.h>
#include <libgfl.h>
#include "IEnum.h"
#include "Core/IString.h"

namespace Initial
{
	
	class IImage
	{
	public:
		IImage();
		IImage(IString filename);
		~IImage();

		void* GetData();
		ImageFormat GetFormat();
		int GetWidth();
		int GetHeight();

		void SetImage(int width, int height, ImageFormat format=IF_RGB);
		void SetData(void *data);

	protected:
		void _Init();
	protected:
		void *m_pData;

		int			m_iWidth;
		int			m_iHeight;
		ImageFormat	m_iFormat;
	};

}

#endif