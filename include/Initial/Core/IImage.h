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
#include "Initial/IEnum.h"
#include "Initial/Core/IString.h"

namespace Initial
{
	namespace Core
	{
		class IImage
		{
		public:
			IImage();
			IImage(IImage& copy);
			IImage(IString filename);
			~IImage();

			unsigned char* GetData();
			ImageFormat GetFormat();
			int GetWidth();
			int GetHeight();

			void SetImage(int width, int height, ImageFormat format=IIF_RGB);
			void SetData(void *data);

			bool Load(Core::IString filename);
			void Save(Core::IString filename);

			void operator =(IImage &copy);

		protected:
			void _Init();
		protected:
			void *m_pData;

			int			m_iWidth;
			int			m_iHeight;
			ImageFormat	m_iFormat;
		};
	}
}

#endif