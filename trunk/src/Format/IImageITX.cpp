
#include <nvtt/nvtt.h>
#include <nvimage/DirectDrawSurface.h>
#include <nvimage/ColorBlock.h>
#include <nvimage/BlockDXT.h>
//#include <nvtt/squish/squish.h>
//#include <nvtt/squish/colourblock.h>
//#include <nvimage/BlockDXT.h>

#include <sys/stat.h>

#include "Initial/Format/IImageITX.h"
#include "Initial/Core/IImage.h"
#include "Initial/IO/IFileStream.h"
#include "Initial/Math/IMath.h"


using namespace Initial::Core;
using namespace Initial::IO;

typedef struct stat stat_t;

struct ITXOutputHandler : public nvtt::OutputHandler
{
	ITXOutputHandler(void *data, Initial::Format::IImageITX::ITXOutput *output)
	{
		Init(data,output);
	}

	void Init(void *data, Initial::Format::IImageITX::ITXOutput *output)
	{
		Data=data;
		Output=output;
		firstWrite=false;
		firstWrite=true;
		Total=1;
		Progress=0;
	}

	void SetTotal(unsigned long t)
	{
		Total = t + 128;
		if (Output)
			Output->SetTotal(Total);
	}

	virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel)
	{
		if (firstWrite==false) // Ignore dds header write
			firstWrite=true;
	}
	
	// Output data.
	virtual bool writeData(const void * data, int size)
	{
		if (Data)
			memcpy((char*)Data+Progress,data,size);

		Progress += size;

		if (Output)
			Output->Progress(Progress);

		return true;
	}

	unsigned long Total;
	unsigned long Progress;

	bool firstWrite;
	void *Data;
	Initial::Format::IImageITX::ITXOutput *Output;
};

namespace Initial
{
	namespace Format
	{
		IImageITX::IImageITX()
			//: IRessource("")
		{
			m_pData=NULL;
			m_iWidth=-1;
			m_iHeight=-1;
			m_iMipMapsCount=-1;
		}

		IImageITX::~IImageITX()
		{
			if (m_pData!=NULL)
				delete[] m_pData;
		}

		bool IImageITX::IsOK()
		{
			if (m_pData==NULL)
				return false;
			else 
				return true;
		}

		IImageITX* IImageITX::LoadITX(IString file)
		{
			IImageITX *itx = new IImageITX;
			if (itx->Load(file))
				return itx;
			delete itx;
			return NULL;
		}

		IString IImageITX::GetFilename()
		{
			return m_sFile;
		}

		IString IImageITX::GetName()
		{
			return m_sName;
		}

		int IImageITX::GetWidth()
		{
			return m_iWidth;
		}

		int IImageITX::GetHeight()
		{
			return m_iHeight;
		}

		IImageITX::ITXFormat IImageITX::GetFormat()
		{
			return m_iFormat;
		}

		IString IImageITX::GetOrigFilename()
		{
			return m_sOrigFile;
		}

		time_t IImageITX::GetOrigFileTime()
		{
			return m_tOrigTime;
		}

		void* IImageITX::GetData()
		{
			return m_pData;
		}

		int IImageITX::GetMipMapCount()
		{
			return m_iMipMapsCount;
		}

		void IImageITX::SetOrigFilename(Core::IString filename)
		{
			m_sOrigFile=filename;
			stat_t attrib;			// create a file attribute structure	        
			stat(m_sOrigFile.c_str(), &attrib);		// get the attributes of afile.txt	
			m_tOrigTime=attrib.st_mtime;
		}

		void IImageITX::SetName(Core::IString name)
		{
			m_sName=name;
		}

		IImageITX::ImageType IImageITX::GetInputImageType()
		{
			return m_iType;
		}

		IImageITX::BorderType IImageITX::GetBorderType()
		{
			return m_iBorder;
		}

		bool IImageITX::Save(IString File)
		{
			if (m_pData==NULL)
				return false; // Not valid data

			IFileStream file(File,IFileStream::IOM_WRITE);

			if (!file.IsOk())
				return false; // File error

			unsigned char ITX[3] ={'I','T','X'};
			unsigned char Version=2;
			short NameSize = m_sName.Length();
			unsigned char Options = (m_iType&0x1) | (m_iBorder&0x2)<<1 | (m_iFormat&0xf)<<3 | (m_bZip&0x1)<<7;
			short Width = m_iWidth;
			short Height = m_iHeight;
			short MipMapsCount = m_iMipMapsCount;
			short OrigFileSize = m_sOrigFile.Length();
			time_t Time = m_tOrigTime;
			long Size = m_lSize;

			file.Write(ITX,1,3);
			file.Write(&Version,1,1);
			file.Write(&NameSize,2,1);
			file.Write(m_sName.c_str(),NameSize,1);
			file.Write(&Options,1,1);
			file.Write(&Width,2,1);
			file.Write(&Height,2,1);
			file.Write(&MipMapsCount,2,1);
			file.Write(&OrigFileSize,2,1);
			file.Write(m_sOrigFile.c_str(),OrigFileSize,1);
			file.Write(&Time,8,1);
			file.Write(&Size,4,1);
			file.Write(m_pData,Size,1);
			file.Close();
			return true;
		}

		bool IImageITX::Load(IString file)
		{
			return _Load(file,false);
		}

		bool IImageITX::LoadInfo(IString file)
		{
			return _Load(file,true);
		}

		bool IImageITX::_Load(IString File, bool noData)
		{
			//printf("Read ITX file %s\n",File.c_str());
			unsigned char ITX[3];
			unsigned char version;
			short NameSize;
			IString Name;
			unsigned char options;
			short Width;
			short Height;
			short MipMapsCount;
			short OrigFileSize;
			IString OrigFile;
			time_t Time;
			long Size;

			IFileStream file(File,IFileStream::IOM_READ);
			if (!file.IsOk())
				return false; // No file

			file.Read(ITX,3,1); //char[3] - Header	
			if (!(ITX[0]=='I' && ITX[1]=='T' && ITX[2]=='X'))
				return false; // Bad file format

			file.Read(&version,1,1); //char - Version
			if (version!=1 && version!=2)
				return false; // Bad version

			file.Read(&NameSize,2,1); //short - Name
			Name.AllocBuffer(NameSize);
			file.Read((void*)Name.c_str(),NameSize,1);
			Name[(int)NameSize]='\0';
			file.Read(&options,1,1); //char - Options
			file.Read(&Width,2,1); //Short - Width
			file.Read(&Height,2,1); //Short - Height
			file.Read(&MipMapsCount,2,1); //Short - Mipmaps count
			file.Read(&OrigFileSize,2,1); //Short - Orig file name
			OrigFile.AllocBuffer(OrigFileSize);
			OrigFile[(int)OrigFileSize]='\0';
			
			file.Read((void*)OrigFile.c_str(),OrigFileSize,1);
			file.Read(&Time,8,1);
			file.Read(&Size,4,1);

			if (m_pData!=NULL)
				delete[] m_pData;
			if (noData)
			{
				m_pData=NULL;
			}else{
				m_pData=new char[Size];
				file.Read(m_pData,Size,1);
			}
			file.Close();

			m_sFile=File;
			m_sName=Name;
			m_iWidth=Width;
			m_iHeight=Height;

			m_iType=(ImageType)((options>>0)&0x1);
			m_iBorder=(BorderType)((options>>1)&0x2);
			m_iFormat=(ITXFormat)((options>>3)&0xf);
			if (version==1)
				m_iFormat=(ITXFormat)(m_iFormat+ITXF_RGBA);
			m_bZip=(bool)((options>>7)&0x1);

			m_iMipMapsCount=MipMapsCount;
			m_sOrigFile=OrigFile;
			m_tOrigTime=Time;
			m_lSize=Size;

			//printf("NameSize %d OrigSize %d\n",NameSize,OrigFileSize);
			//printf("Name %s\n",Name.c_str());
			//printf("Orig %s\n",OrigFile.c_str());
			//printf("%d %d %d %d %d %d %d %d\n",Width,Height,MipMapsCount,Size,m_iType,m_iBorder,m_iFormat,m_bZip);

			return true;
		}

		IImage IImageITX::GetImage(int mipmap)
		{
			IImage image;

			if (m_iFormat==ITXF_RGBA)
			{
				image.SetImage(m_iWidth,m_iHeight,IIF_RGBA);
				image.SetData(m_pData);
			}else{
				if (m_iFormat==ITXF_DXT1 || m_iFormat==ITXF_DXT1nm || m_iFormat==ITXF_DXT5nm)
					image.SetImage(m_iWidth,m_iHeight,IIF_RGB);
				else
					image.SetImage(m_iWidth,m_iHeight,IIF_RGBA);

				unsigned char *data = image.GetData();
				nv::ColorBlock block;
				int line=0;
				int col=0;

				for (int i=0;i<((m_iWidth+3)/4) * ((m_iHeight+3)/4);i++)
				{
					if (m_iFormat==ITXF_DXT1 || m_iFormat==ITXF_DXT1nm || m_iFormat==ITXF_DXT1a)
					{
						nv::BlockDXT1 dxBlock;					
						memcpy(&dxBlock,(unsigned char*)m_pData+i*8,8);					
						dxBlock.decodeBlock(&block);
					}
					else if (m_iFormat==ITXF_DXT3)
					{
						nv::BlockDXT3 dxBlock;
						memcpy(&dxBlock,(unsigned char*)m_pData+i*16,16);					
						dxBlock.decodeBlock(&block);
					}
					else if (m_iFormat==ITXF_DXT5 || m_iFormat==ITXF_DXT5nm)
					{
						nv::BlockDXT5 dxBlock;
						memcpy(&dxBlock,(unsigned char*)m_pData+i*16,16);					
						dxBlock.decodeBlock(&block);
					}
					else if (m_iFormat==ITXF_ATI1)
					{
						nv::BlockATI1 dxBlock;
						memcpy(&dxBlock,(unsigned char*)m_pData+i*8,8);					
						dxBlock.decodeBlock(&block);
					}
					else if (m_iFormat==ITXF_ATI2_3Dc)
					{
						nv::BlockATI2 dxBlock;
						memcpy(&dxBlock,(unsigned char*)m_pData+i*16,16);					
						dxBlock.decodeBlock(&block);
					}

					int colorSize=4;
					if (m_iFormat==ITXF_DXT1 || m_iFormat==ITXF_DXT1nm || m_iFormat==ITXF_DXT5nm)
						colorSize=3;
					
					for (int x=0;x<4;x++)
					{
						for (int y=0;y<4;y++)
						{
							nv::Color32 color = block.color(x,y);
							int pos = colorSize*(col+m_iWidth*line*4+m_iWidth*y+x);
							if (m_iFormat==ITXF_DXT5nm)
							{
								float r = /*(color.r/255.0)*/(color.a/255.0)*2.0-1.0;
								float g = (color.g/255.0)*2.0-1.0;
								float b = 1.0/Q_rsqrt(1.0-r*r-g*g);
								r=(r+1.0)*0.5;
								g=(g+1.0)*0.5;
								b=(b+1.0)*0.5;
								data[pos+0]=r*255;//r
								data[pos+1]=g*255;//g
								data[pos+2]=b*255;//b
							}else{
								data[pos+0]=color.r;//r
								data[pos+1]=color.g;//g
								data[pos+2]=color.b;//b
								if (colorSize==4)
									data[pos+3]=color.a;//a	
							}								
						}
					}

					col+=4;
					if (col>=m_iWidth)
					{
						col=0;
						line++;
					}					
				}
			}
			
			return image;

			return IImage();
		}

		bool IImageITX::GPUAccelerationAvailable()
		{
			nvtt::Compressor compressor;
			compressor.enableCudaAcceleration(true);
			return compressor.isCudaAccelerationEnabled();
		}

		bool IImageITX::CreateFromImage(IImage& image, ITXParameter params)
		{
			if (image.GetFormat()!=IIF_NONE)
			{
				//m_sFile=image.GetFilename();
				nvtt::Compressor compressor;
				nvtt::InputOptions inputOptions;
				nvtt::OutputOptions outputOptions;
				nvtt::CompressionOptions compressionOptions;

				//Get image Size
				m_iWidth=image.GetWidth();
				m_iHeight=image.GetHeight();

				m_iType=params.type;
				m_iBorder=params.border;
				m_iFormat=params.format;
				m_bZip=params.zipped & 0;

				// Convert image to RGBA buffer
				unsigned char * data = new unsigned char[m_iWidth*m_iHeight*4];
				for (int x=0;x<image.GetWidth();x++)
				{
					for (int y=0;y<image.GetHeight();y++)
					{
						// Convert RGB to BGR
						if (image.GetFormat()==IIF_RGBA) //RGB
						{
							data[4*(y*image.GetWidth()+x)+0]=image.GetData()[4*(y*image.GetWidth()+x)+2]; // Blue
							data[4*(y*image.GetWidth()+x)+1]=image.GetData()[4*(y*image.GetWidth()+x)+1]; // Green
							data[4*(y*image.GetWidth()+x)+2]=image.GetData()[4*(y*image.GetWidth()+x)+0]; // Red
							data[4*(y*image.GetWidth()+x)+3]=image.GetData()[4*(y*image.GetWidth()+x)+3]; // Red
						}else if (image.GetFormat()==IIF_RGB) //RGB
						{
							data[4*(y*image.GetWidth()+x)+0]=image.GetData()[3*(y*image.GetWidth()+x)+2]; // Blue
							data[4*(y*image.GetWidth()+x)+1]=image.GetData()[3*(y*image.GetWidth()+x)+1]; // Green
							data[4*(y*image.GetWidth()+x)+2]=image.GetData()[3*(y*image.GetWidth()+x)+0]; // Red
							data[4*(y*image.GetWidth()+x)+3]=255;
						}else{ //Grey
							data[4*(y*image.GetWidth()+x)+0]=image.GetData()[y*image.GetWidth()+x];
							data[4*(y*image.GetWidth()+x)+1]=image.GetData()[y*image.GetWidth()+x];
							data[4*(y*image.GetWidth()+x)+2]=image.GetData()[y*image.GetWidth()+x];
							data[4*(y*image.GetWidth()+x)+3]=255;
						}
					}
				}
				//Set texture data
				inputOptions.setTextureLayout(nvtt::TextureType_2D, image.GetWidth(), image.GetHeight());
				inputOptions.setMipmapData(data, image.GetWidth(), image.GetHeight());
				delete[] data;

				//SetOrigFilename(image.GetFilename());

				//Set output filename
				/*wxFileName filename(image.GetFilename());
				filename.ClearExt();				
				//Set Name
				SetName(filename.GetFullName());*/			

				// CUDA
				compressor.enableCudaAcceleration(params.GPUacce);

				//Mipmaps
				inputOptions.setMipmapGeneration(params.mipmaps);
				
				if (params.mipmaps)
				{
					m_iMipMapsCount=0;
					int _width=image.GetWidth()>image.GetHeight()?image.GetWidth():image.GetHeight();
					while(_width>1)
					{
						_width=_width/2;
						m_iMipMapsCount++;
					}
					//printf("MipMapsCount %d\n",m_iMipMapsCount);
				}else
					m_iMipMapsCount=0;

				//Image type
				inputOptions.setConvertToNormalMap(false);
				if (params.type==IT_COLOR) // Color
				{
					inputOptions.setNormalMap(false);		
					inputOptions.setGamma(2.2f, 2.2f);
					inputOptions.setNormalizeMipmaps(false);
				}else{ // Normal map
					inputOptions.setNormalMap(true);		
					inputOptions.setGamma(1.0f, 1.0f);
					inputOptions.setNormalizeMipmaps(true);
				}	

				//Border
				nvtt::WrapMode wrap=(nvtt::WrapMode)(nvtt::WrapMode_Clamp+params.border);
				inputOptions.setWrapMode(wrap);

				//Quality
				compressionOptions.setQuality((nvtt::Quality)((int)nvtt::Quality_Fastest + params.quality));
				
				//Output format
				nvtt::Format format=nvtt::Format_RGBA;
				switch (params.format)
				{
				case ITXF_DXT1: // DXT1
					format=nvtt::Format_DXT1;
					break;
				case ITXF_DXT1nm: // DXT1nm
					format=nvtt::Format_DXT1;
					compressionOptions.setColorWeights(1, 1, 0);
					break;
				case ITXF_DXT1a: // DXT1a
					format=nvtt::Format_DXT1a;
					break;
				case ITXF_DXT3: // DXT3
					format=nvtt::Format_DXT3;
					break;
				case ITXF_DXT5: // DXT5
					format=nvtt::Format_DXT5;
					break;
				case ITXF_DXT5nm: // DXT5nm
					format=nvtt::Format_DXT5n;
					break;
				case ITXF_ATI1: // ATI1
					format=nvtt::Format_BC4;
					break;
				case ITXF_ATI2_3Dc: // ATI2/3Dc
					format=nvtt::Format_BC5;
					break;
				};

				compressionOptions.setFormat(format);

				m_lSize = compressor.estimateSize(inputOptions, compressionOptions);
				//printf("Size %d\n",m_lSize);
				
				//Image type (1bit) + BorderType (1bit) + DXT Format (4bits) + Zipped Data (1bit) + Not used (1bit)
				unsigned char options = params.type<<0 | params.border<<1 | params.format<<3 | params.zipped<<7;
				//printf("Options %d\n",options);
				//printf("Image type %d\n",(options>>0)&0x1);
				//printf("Border Type %d\n",(options>>1)&0x2);
				//printf("DXT Format %d\n",(options>>3)&0xf);
				//printf("Zipped %d\n",(options>>7)&0x1);

				if (m_pData!=NULL)
					delete[] m_pData;
				m_pData=new char[m_lSize];

				ITXOutputHandler outputHandler(m_pData,params.output);
				outputHandler.SetTotal(m_lSize);

				outputOptions.setOutputHandler(&outputHandler);
				outputOptions.setOutputHeader(false);

				return compressor.process(inputOptions, compressionOptions, outputOptions);				
			}
			return false;
		}

		bool IImageITX::CreateFromImage(IString file, ITXParameter params)
		{
			IImage image(file);
			bool res = CreateFromImage(image,params);
			SetOrigFilename(file);
			return res;
		}

		int IImageITX::UpdateITX(Core::IString file, ITXOutput *output, OutputQuality quality, bool GPUacce, bool force)
		{
			IImageITX itx;
			if (itx.LoadInfo(file))
			{
				//Get input file last modified time
				struct stat attrib;			// create a file attribute structure	        
				stat(itx.GetOrigFilename().c_str(), &attrib);		// get the attributes of afile.txt	
				time_t Time=attrib.st_mtime;

				if (Time!=itx.GetOrigFileTime() || force)
				{
					IImage image;
					if (image.Load(itx.GetOrigFilename()))
					{
						IImageITX itxOut;

						IImageITX::ITXParameter params;
						params.border=itx.m_iBorder;
						params.format=itx.m_iFormat;
						params.GPUacce=GPUacce;
						params.mipmaps=itx.m_iMipMapsCount>0?true:false;
						params.output=output;
						params.quality=quality;
						params.type=itx.m_iType;
						params.zipped=itx.m_bZip;

						bool OK = itxOut.CreateFromImage(image,params) ? 0 : 3;
						itxOut.SetOrigFilename(itx.GetOrigFilename());
						itxOut.Save(file);

						return OK;
					}
					return 3; // No file or valid original image file
				}
				return 1; // No need update
			}
			return 2; // No file or valid ITX file
		}
	}
}