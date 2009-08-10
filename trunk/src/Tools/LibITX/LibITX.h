// LibITX.h

#pragma once

#include "Initial/Config.h"
#include "Initial/Format/IImageITX.h"

using namespace System;
using namespace Initial;
using namespace Initial::Format;

namespace LibITX
{

	/*INITIAL_EXPORT*/ public ref class ITXImage /*: public Initial::Format::IImageITX*/
	{
	public:
		ITXImage();

		bool		IsOK();

		const char*		GetFilename();
		const char*		GetName();
		int			GetWidth();
		int			GetHeight();
		IImageITX::DXTFormat	GetFormat();
		const char*		GetOrigFilename();
		time_t		GetOrigFileTime();
		void*		GetData();
		int			GetMipMapCount();

		IImageITX::ImageType	GetInputImageType();
		IImageITX::BorderType	GetBorderType();

		void SetOrigFilename(const char*);
		void SetName(const char*);

		bool GPUAccelerationAvailable();

		//bool CreateFromImage(Core::IImage& image, ITXParameter params);
		//bool CreateFromImage(Core::IString file, ITXParameter params);

		/*
		Update ITX file if is necessary

		return value : 
			0 - if file is updated
			1 - no need update
			2 - no input file found or invalid file
			3 - no original file found
		*/
		//static int UpdateITX(Core::IString file, ITXOutput *output=NULL, OutputQuality quality=OQ_NORMAL, bool GPUaccu=true, bool force=false);

		Core::IImage GetImage(int mipmap);

		bool Save(const char* file);
		bool Load(const char* file);
		bool LoadInfo(const char* file);
		
	protected:
		//IImageITX m_ITX;
		void *m_ITX;
	};
}
