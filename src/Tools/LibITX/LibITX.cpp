// Il s'agit du fichier DLL principal.

#include "stdafx.h"

#include "LibITX.h"

namespace LibITX
{
	ITXImage::ITXImage()
	{
		m_ITX = (void*)(new IImageITX());
	}

	bool ITXImage::IsOK()
	{
		return true;
	}

	const char* ITXImage::GetFilename()
	{
		return "";
	}

	const char* ITXImage::GetName()
	{
		return "";
	}

	int ITXImage::GetWidth()
	{
		return 1;
	}

	int ITXImage::GetHeight()
	{
		return 1;
	}

	IImageITX::DXTFormat ITXImage::GetFormat()
	{
		return IImageITX::DXTF_ATI1;
	}

	const char* ITXImage::GetOrigFilename()
	{
		return "";
	}

	time_t ITXImage::GetOrigFileTime()
	{
		return 1;
	}

	void* ITXImage::GetData()
	{
		return NULL;
	}

	int ITXImage::GetMipMapCount()
	{
		return 0;
	}

	IImageITX::ImageType ITXImage::GetInputImageType()
	{
		return IImageITX::IT_COLOR;
	}

	IImageITX::BorderType ITXImage::GetBorderType()
	{
		return IImageITX::BT_CLAMP;
	}

	void ITXImage::SetOrigFilename(const char* filename)
	{
	}

	void ITXImage::SetName(const char* name)
	{
	}

	bool ITXImage::GPUAccelerationAvailable()
	{
		return true;
	}

	Core::IImage ITXImage::GetImage(int mipmap)
	{
		return Core::IImage();
	}

	bool ITXImage::Save(const char* file)
	{
		return true;
	}

	bool ITXImage::Load(std::string file)
	{
		return true;
	}

	bool ITXImage::LoadInfo(const char* file)
	{
		return true;
	}
}