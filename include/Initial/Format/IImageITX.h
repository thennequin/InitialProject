//---------------------------------------------
//                Initial Engine
//             Render driver Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IITXIMAGE_HEADER_
#define _IITXIMAGE_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IImage.h"
//#include "Initial/IRessource.h"

namespace Initial
{
	namespace Format
	{
		class IImageITX //: public IRessource
		{
		public:
			class ITXOutput
			{
			public:
				virtual void Progress(int current)=0;
				virtual void SetTotal(int totalSize)=0;
			};
		public:
			enum ImageType
			{
				IT_COLOR,
				IT_NORMAL,
			};

			enum BorderType
			{
				BT_CLAMP=0,
				BT_REPEAT,
				BT_MIRROR,
			};

			enum ITXFormat
			{
				ITXF_GREY=0,
				ITXF_AL, // Alpha and Grey
				ITXF_RGB,
				ITXF_RGBA,
				ITXF_DXT1,
				ITXF_DXT1nm,
				ITXF_DXT1a,
				ITXF_DXT3,
				ITXF_DXT5,
				ITXF_DXT5nm,
				ITXF_ATI1,
				ITXF_ATI2_3Dc,				
			};

			enum OutputQuality
			{
				OQ_FAST,
				OQ_NORMAL,
				OQ_PRODUCTION,
				OQ_HIGHEST,
			};

			struct ITXParameter
			{
				ITXParameter()
				{
					type=IT_COLOR;
					border=BT_REPEAT;
					format=ITXF_DXT5;
					mipmaps=true;
					quality=OQ_NORMAL;
					GPUacce=true;
					output=NULL;
					zipped=false;
				}

				ImageType type;
				BorderType border;
				ITXFormat format;
				bool mipmaps;
				OutputQuality quality;
				bool GPUacce;
				ITXOutput *output;
				bool zipped; // Supported in V2
			};
		public:
			static IImageITX *LoadITX(Core::IString file);
			~IImageITX();

			bool		IsOK();

			Core::IString		GetFilename();
			Core::IString		GetName();
			int			GetWidth();
			int			GetHeight();
			ITXFormat	GetFormat();
			Core::IString		GetOrigFilename();
			time_t		GetOrigFileTime();
			void*		GetData();
			int			GetMipMapCount();

			ImageType	GetInputImageType();
			BorderType	GetBorderType();

			void SetOrigFilename(Core::IString filename);
			void SetName(Core::IString name);

			bool GPUAccelerationAvailable();

			bool CreateFromImage(Core::IImage& image, ITXParameter params);
			bool CreateFromImage(Core::IString file, ITXParameter params);

			/*
			Update ITX file if is necessary

			return value : 
				0 - if file is updated
				1 - no need update
				2 - no input file found or invalid file
				3 - no original file found
			*/
			static int UpdateITX(Core::IString file, ITXOutput *output=NULL, OutputQuality quality=OQ_NORMAL, bool GPUaccu=true, bool force=false);

			Core::IImage GetImage(int mipmap=0);

			virtual bool Save(Core::IString file);
			virtual bool Load(Core::IString file);
			virtual bool LoadInfo(Core::IString file);

		//protected:
			IImageITX();
		protected:
			bool _Load(Core::IString file, bool nodata);
		protected:
			Core::IString		m_sFile;
			Core::IString		m_sName;
			int			m_iWidth;
			int			m_iHeight;

			Core::IString		m_sOrigFile;
			time_t		m_tOrigTime;

			//Options
			ImageType	m_iType;
			BorderType	m_iBorder;
			ITXFormat	m_iFormat;
			bool		m_bZip;

			int			m_iMipMapsCount;
			long		m_lSize;
			void*		m_pData;
		};
	}
};

#endif