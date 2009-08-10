
#include "Initial/Core/Image/TGA.h"

#include "Initial/IO/IFileStream.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::IO;

#pragma pack(push, 1)
/* tga header */
typedef struct
{
	unsigned char   id_lenght;          /* size of image id */
	unsigned char   colormap_type;      /* 1 is has a colormap */
	unsigned char   image_type;         /* compression type */

	short   cm_first_entry;				/* colormap origin */
	short   cm_length;					/* colormap length */
	unsigned char   cm_size;            /* colormap size */

	short   x_origin;					/* bottom left x coord origin */
	short   y_origin;					/* bottom left y coord origin */

	short   width;						/* picture width (in pixels) */
	short   height;						/* picture height (in pixels) */

	unsigned char   pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
	unsigned char   image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */

} tga_header_t;
#pragma pack(pop)

void GetTextureInfo (tga_header_t *header,IImage *image)
{
	switch (header->image_type)
	{
	case 3:  /* grayscale 8 bits */
	case 11: /* grayscale 8 bits (RLE) */
		image->SetImage(header->width,header->height,IIF_GREY);
		break;
	case 1:  /* 8 bits color index */
	case 2:  /* BGR 16-24-32 bits */
	case 9:  /* 8 bits color index (RLE) */
	case 10: /* BGR 16-24-32 bits (RLE) */
		/* 8 bits and 16 bits images will be converted to 24 bits */
		if (header->pixel_depth <= 24)
			image->SetImage(header->width,header->height,IIF_RGB);
		else /* 32 bits */
			image->SetImage(header->width,header->height,IIF_RGBA);
		break;
	}
}


void ReadTGA8bits (IFileStream& fp, unsigned char *colormap, IImage *image)
{
	int i;
	unsigned char color;

	for (i = 0; i < image->GetWidth() * image->GetHeight(); ++i)
	{
		/* read index color byte */
		color = fp.GetChar();

		/* convert to RGB 24 bits */
		image->GetData()[(i * 3) + 2] = colormap[(color * 3) + 0];
		image->GetData()[(i * 3) + 1] = colormap[(color * 3) + 1];
		image->GetData()[(i * 3) + 0] = colormap[(color * 3) + 2];
	}
}


void ReadTGA16bits (IFileStream& fp, IImage *image)
{
	int i;
	unsigned short color;

	for (i = 0; i < image->GetWidth() * image->GetHeight(); ++i)
	{
		/* read color word */
		color = fp.GetChar() + (fp.GetChar() << 8);

		/* convert BGR to RGB */
		image->GetData()[(i * 3) + 0] = (unsigned char)(((color & 0x7C00) >> 10) << 3);
		image->GetData()[(i * 3) + 1] = (unsigned char)(((color & 0x03E0) >>  5) << 3);
		image->GetData()[(i * 3) + 2] = (unsigned char)(((color & 0x001F) >>  0) << 3);
	}
}


void ReadTGA24bits (IFileStream& fp, IImage *image)
{
	int i;

	for (i = 0; i < image->GetWidth() * image->GetHeight(); ++i)
	{
		/* read and convert BGR to RGB */
		image->GetData()[(i * 3) + 2] = fp.GetChar();
		image->GetData()[(i * 3) + 1] = fp.GetChar();
		image->GetData()[(i * 3) + 0] = fp.GetChar();
	}
}


void ReadTGA32bits (IFileStream& fp, IImage *image)
{
	int i;

	for (i = 0; i < image->GetWidth() * image->GetHeight(); ++i)
	{
		/* read and convert BGRA to RGBA */
		image->GetData()[(i * 4) + 2] = fp.GetChar();
		image->GetData()[(i * 4) + 1] = fp.GetChar();
		image->GetData()[(i * 4) + 0] = fp.GetChar();
		image->GetData()[(i * 4) + 3] = fp.GetChar();
	}
}


void ReadTGAgray8bits (IFileStream& fp, IImage *image)
{
	int i;

	for (i = 0; i < image->GetWidth() * image->GetHeight(); ++i)
	{
		/* read grayscale color byte */
		image->GetData()[i] = fp.GetChar();
	}
}


void ReadTGAgray16bits (IFileStream& fp, IImage *image)
{
	int i;

	for (i = 0; i < image->GetWidth() * image->GetHeight(); ++i)
	{
		/* read grayscale color + alpha channel bytes */
		//image->GetData()[(i * 2) + 0] = fp.GetChar();
		//image->GetData()[(i * 2) + 1] = fp.GetChar();
	}
}


void ReadTGA8bitsRLE (IFileStream& fp, unsigned char *colormap, IImage *image)
{
	int i, size;
	unsigned char color;
	unsigned char packet_header;
	unsigned char *ptr = image->GetData();

	while (ptr < image->GetData() + (image->GetWidth() * image->GetHeight()) * 3)
	{
		/* read first byte */
		packet_header = fp.GetChar();
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			/* run-length packet */
			color = fp.GetChar();

			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[0] = colormap[(color * 3) + 2];
				ptr[1] = colormap[(color * 3) + 1];
				ptr[2] = colormap[(color * 3) + 0];
			}
		}
		else
		{
			/* non run-length packet */
			for (i = 0; i < size; ++i, ptr += 3)
			{
				color = fp.GetChar();

				ptr[0] = colormap[(color * 3) + 2];
				ptr[1] = colormap[(color * 3) + 1];
				ptr[2] = colormap[(color * 3) + 0];
			}
		}
	}
}


void ReadTGA16bitsRLE (IFileStream& fp, IImage *image)
{
	int i, size;
	unsigned short color;
	unsigned char packet_header;
	unsigned char *ptr = image->GetData();

	while (ptr < image->GetData() + (image->GetWidth() * image->GetHeight()) * 3)
	{
		/* read first byte */
		packet_header = fp.GetChar();
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			/* run-length packet */
			color = fp.GetChar() + (fp.GetChar() << 8);

			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[0] = (unsigned char)(((color & 0x7C00) >> 10) << 3);
				ptr[1] = (unsigned char)(((color & 0x03E0) >>  5) << 3);
				ptr[2] = (unsigned char)(((color & 0x001F) >>  0) << 3);
			}
		}
		else
		{
			/* non run-length packet */
			for (i = 0; i < size; ++i, ptr += 3)
			{
				color = fp.GetChar() + (fp.GetChar() << 8);

				ptr[0] = (unsigned char)(((color & 0x7C00) >> 10) << 3);
				ptr[1] = (unsigned char)(((color & 0x03E0) >>  5) << 3);
				ptr[2] = (unsigned char)(((color & 0x001F) >>  0) << 3);
			}
		}
	}
}


void ReadTGA24bitsRLE (IFileStream& fp, IImage *image)
{
	int i, size;
	unsigned char rgb[3];
	unsigned char packet_header;
	unsigned char *ptr = image->GetData();

	while (ptr < image->GetData() + (image->GetWidth() * image->GetHeight()) * 3)
	{
		/* read first byte */
		packet_header = fp.GetChar();
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			/* run-length packet */
			fp.Read(rgb, sizeof (unsigned char), 3);

			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[0] = rgb[2];
				ptr[1] = rgb[1];
				ptr[2] = rgb[0];
			}
		}
		else
		{
			/* non run-length packet */
			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[2] = fp.GetChar();
				ptr[1] = fp.GetChar();
				ptr[0] = fp.GetChar();
			}
		}
	}
}


void ReadTGA32bitsRLE (IFileStream& fp, IImage *image)
{
	int i, size;
	unsigned char rgba[4];
	unsigned char packet_header;
	unsigned char *ptr = image->GetData();

	while (ptr < image->GetData() + (image->GetWidth() * image->GetHeight()) * 4)
	{
		/* read first byte */
		packet_header = fp.GetChar();
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			/* run-length packet */
			fp.Read(rgba, sizeof (unsigned char), 4);

			for (i = 0; i < size; ++i, ptr += 4)
			{
				ptr[0] = rgba[2];
				ptr[1] = rgba[1];
				ptr[2] = rgba[0];
				ptr[3] = rgba[3];
			}
		}
		else
		{
			/* non run-length packet */
			for (i = 0; i < size; ++i, ptr += 4)
			{
				ptr[2] = fp.GetChar();
				ptr[1] = fp.GetChar();
				ptr[0] = fp.GetChar();
				ptr[3] = fp.GetChar();
			}
		}
	}
}


void ReadTGAgray8bitsRLE (IFileStream& fp, IImage *image)
{
	int i, size;
	unsigned char color;
	unsigned char packet_header;
	unsigned char *ptr = image->GetData();

	while (ptr < image->GetData() + (image->GetWidth() * image->GetHeight()))
	{
		/* read first byte */
		packet_header = fp.GetChar();
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			/* run-length packet */
			color = fp.GetChar();

			for (i = 0; i < size; ++i, ptr++)
				*ptr = color;
		}
		else
		{
			/* non run-length packet */
			for (i = 0; i < size; ++i, ptr++)
				*ptr = fp.GetChar();
		}
	}
}


void ReadTGAgray16bitsRLE (IFileStream& fp, IImage *image)
{
	int i, size;
	unsigned char color, alpha;
	unsigned char packet_header;
	unsigned char *ptr = image->GetData();

	while (ptr < image->GetData() + (image->GetWidth() * image->GetHeight()) * 2)
	{
		/* read first byte */
		packet_header = fp.GetChar();
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			/* run-length packet */
			color = fp.GetChar();
			alpha = fp.GetChar();

			for (i = 0; i < size; ++i, ptr += 2)
			{
				ptr[0] = color;
				ptr[1] = alpha;
			}
		}
		else
		{
			/* non run-length packet */
			for (i = 0; i < size; ++i, ptr += 2)
			{
				ptr[0] = fp.GetChar();
				ptr[1] = fp.GetChar();
			}
		}
	}
}

namespace Initial
{
	namespace Image
	{
		bool ReadTGA(IString filename, IImage *image)
		{
			if (image)
			{
				IFileStream fp;
				tga_header_t header;
				unsigned char *colormap = NULL;

				fp.Open(filename, IFileStream::IOM_READ);
				if (!fp.IsOk())
				{
					//fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
					return false;
				}

				bool OK=true;

				/* read header */
				fp.Read(&header, sizeof (tga_header_t), 1);
				GetTextureInfo (&header, image);
				fp.Seek(header.id_lenght,IFileStream::ISM_CURRENT);

				/* read color map */
				if (header.colormap_type)
				{
					/* NOTE: color map is stored in BGR format */
					colormap = (unsigned char *)malloc (sizeof (unsigned char)
						* header.cm_length * (header.cm_size >> 3));
					fp.Read(colormap, sizeof (unsigned char), header.cm_length
						* (header.cm_size >> 3));
				}

				/* read image data */
				switch (header.image_type)
				{
				case 0:
					/* no data */
					break;

				case 1:
					/* uncompressed 8 bits color index */
					ReadTGA8bits (fp, colormap, image);
					break;

				case 2:
					/* uncompressed 16-24-32 bits */
					switch (header.pixel_depth)
					{
					case 16:
						ReadTGA16bits (fp, image);
						break;

					case 24:
						ReadTGA24bits (fp, image);
						break;

					case 32:
						ReadTGA32bits (fp, image);
						break;
					}

					break;

				case 3:
					/* uncompressed 8 or 16 bits grayscale */
					if (header.pixel_depth == 8)
						ReadTGAgray8bits (fp, image);
					else /* 16 */
						ReadTGAgray16bits (fp, image);

					break;

				case 9:
					/* RLE compressed 8 bits color index */
					ReadTGA8bitsRLE (fp, colormap, image);
					break;

				case 10:
					/* RLE compressed 16-24-32 bits */
					switch (header.pixel_depth)
					{
					case 16:
						ReadTGA16bitsRLE (fp, image);
						break;

					case 24:
						ReadTGA24bitsRLE (fp, image);
						break;

					case 32:
						ReadTGA32bitsRLE (fp, image);
						break;
					}

					break;

				case 11:
					/* RLE compressed 8 or 16 bits grayscale */
					if (header.pixel_depth == 8)
						ReadTGAgray8bitsRLE (fp, image);
					else /* 16 */
						ReadTGAgray16bitsRLE (fp, image);

					break;

				default:
					/* image type is not correct */
					fprintf (stderr, "error: unknown TGA image type %i!\n", header.image_type);
					OK=false;
					break;
				}

				/* no longer need colormap data */
				if (colormap)
					free(colormap);

				fp.Close();
				return OK;
			}
			return false;
		}

		bool SaveTGA(Core::IString filename, Core::IImage *image)
		{
			if (image)
			{
				IFileStream fp(filename,IFileStream::IOM_WRITE);
				tga_header_t header;
				unsigned char *colormap = NULL;

				if (!fp.IsOk())
				{
					//fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
					return false;
				}

				bool OK=true;

				header.cm_first_entry=0;
				header.cm_length=0;
				header.cm_size=0;
				header.colormap_type=0;
				header.id_lenght=0;
				header.image_type=2;
				header.x_origin=0;
				header.y_origin=0;
				header.width=image->GetWidth();
				header.height=image->GetHeight();
				
				if (image->GetFormat()==IIF_RGB)
				{
					header.pixel_depth=24;
					header.image_descriptor=0x00;
				}else{
					header.pixel_depth=32;
					header.image_descriptor=0x80;
				}
				fp.Write(&header, sizeof (tga_header_t), 1);

				unsigned char color[4];
				for (int y=image->GetHeight()-1;y>=0;y--)
				{
					for (int x=0;x<image->GetWidth();x++)	
					{
						if (image->GetFormat()==IIF_RGB)
						{
							color[2]=image->GetData()[(y*image->GetWidth()+x)*3];
							color[1]=image->GetData()[(y*image->GetWidth()+x)*3+1];
							color[0]=image->GetData()[(y*image->GetWidth()+x)*3+2];
							fp.Write(color,1,3);
						}else if (image->GetFormat()==IIF_RGBA)
						{
							color[2]=image->GetData()[(y*image->GetWidth()+x)*4];
							color[1]=image->GetData()[(y*image->GetWidth()+x)*4+1];
							color[0]=image->GetData()[(y*image->GetWidth()+x)*4+2];
							color[3]=image->GetData()[(y*image->GetWidth()+x)*4+3];
							fp.Write(color,1,4);
						}						
					}
				}
				fp.Close();
				return true;
			}
			return false;
		}
	}
}
