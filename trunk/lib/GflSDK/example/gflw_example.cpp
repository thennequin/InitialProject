#include "gflw.h"

#define DIR "../data/"

void PrintInfo( GFLW_FORMAT &format )
{
	const char *ext; 

	printf("Index : %d ", (int )format.GetIndex()); 
	printf("%s -", format.GetName()); 
	printf("Read %3s -", format.IsReadable() ? "Yes" : "No"); 
	printf("Write %3s \n", format.IsWritable() ? "Yes" : "No"); 

	printf("  "); 
	ext = format.GetExtensionFirst(); 
	while (ext)
	{
		printf("%s ", ext); 
		ext = format.GetExtensionNext(); 
	}
	printf("\n"); 
}

void GetInfo( char *filename )
{
	const char 
		* origin; 
	GFLW_FILE_INFORMATION 
		info; 

	if ( info.GetFromFile( filename ) != GFL_NO_ERROR )
		return; 
	
  switch (info.GetOrigin())
  {
  case GFL_BOTTOM_LEFT:
    origin = "Bottom Left"; 
    break; 
  case GFL_TOP_RIGHT:
		origin = "Top Right"; 
		break; 
	case GFL_BOTTOM_RIGHT:
		origin = "Bottom Right"; 
		break; 
	default:
		origin = "Top Left"; 
	}
  
	printf( "\n%s : Success\n", filename); 
	printf( "    Size               : %d bytes\n", info.GetFileSize());
	printf( "    Format             : %s (%s - %d)\n", info.GetDescription(), info.GetNameOfFormat(), (int )info.GetIndexOfFormat()); 
	printf( "    Compression        : %s\n", info.GetCompressionDescription()); 
	printf( "    Width              : %d\n", info.GetWidth());
	printf( "    Height             : %d\n", info.GetHeight());
	printf( "    ComponentsPerPixel : %d\n", (int )info.GetComponentsPerPixel());
	printf( "    BitsPerComponent   : %d\n", (int )info.GetBitsPerComponent());
	printf( "    Depth              : %d\n", (int )info.GetNumberOfBits());
	printf( "    # colors           : %lu\n", 1UL<<(info.GetNumberOfBits() >= 24 ? 24 : info.GetNumberOfBits())); 
	printf( "    Orientation        : %s\n", origin); 
	printf( "    Color Model        : %s\n", info.GetLabelForColorModel()); 
	if ( info.GetXdpi() )
		printf( "    Xdpi               : %d\n", info.GetXdpi()); 
	else
		printf( "    Xdpi               : Not set\n"); 
	if ( info.GetYdpi() )
		printf( "    Ydpi               : %d\n", info.GetYdpi()); 
	else
		printf( "    Ydpi               : Not set\n"); 
	printf( "    Page(s)           : %d\n", info.GetNumberOfImages()); 
	/*if ( info.HasComment() )
		printf( "    Comment         : %s \n", info.GetComment() ); */
}

void DisplayError( char *filename, GFL_ERROR error )
{
	const char * str = gflGetErrorString( error ); 
	
	if ( filename )
		printf( "\nGflError : <%s> => %s\n", filename, str ); 
	else
		printf( "\nGflError :%s\n", str ); 
}

/*
 * Load a picture, and save it 
 */
void TestFile( char *filename, char *dstfilename, int output_index )
{
	GFLW_LOAD_PARAMS 
		load_option; 
	GFLW_BITMAP 
		bitmap; 
	GFLW_FILE_INFORMATION
		info; 
	GFL_ERROR 
		error; 
	
	error = bitmap.LoadFromFile(filename, load_option, info); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLW_SAVE_PARAMS 
			save_option;
		
		save_option.SetFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.SetFormat(output_index == -1 ? info.GetIndexOfFormat() : output_index); 
		
		error = bitmap.SaveIntoFile(dstfilename, save_option); 
		if ( error )
			DisplayError( dstfilename, error ); 

		GFLW_BITMAP bitmap2(bitmap); 
		bitmap2.Empty(); 
	}
}

/*
 * Load a picture preview, and save it 
 */
void Test2File( char *filename, char *dstfilename, int output_index )
{
	GFLW_LOAD_PARAMS 
		load_option; 
	GFLW_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.LoadPreviewFromFile( filename, 64, 64, load_option ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLW_SAVE_PARAMS
			save_option; 
		
		save_option.SetFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.SetFormat(output_index); 
		
		error = bitmap.SaveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
	}
}

/*
 * Load a picture, resize and save it 
 */
void Test3File( char *filename, char *dstfilename, int output_index )
{
	GFLW_LOAD_PARAMS 
		load_option; 
	GFLW_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.LoadFromFile( filename, load_option ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLW_SAVE_PARAMS 
			save_option; 
		
		if ( error = bitmap.Resize( 160, 120, GFL_RESIZE_BILINEAR, 0 ) )
			DisplayError( NULL, error ); 
		else
		{
			save_option.SetFlags(GFL_SAVE_REPLACE_EXTENSION); 
			save_option.SetFormat(output_index); 
			error = bitmap.SaveIntoFile( dstfilename, save_option ); 
			if ( error )
				DisplayError( dstfilename, error ); 
		}
	}
}

/*
 * Load a picture, save it in jpeg
 */
void Test4File( char *filename, char *dstfilename, int output_index, GFL_INT32 quality )
{
	GFLW_LOAD_PARAMS 
		load_option; 
	GFLW_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.LoadFromFile( filename, load_option ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLW_SAVE_PARAMS 
			save_option; 

		save_option.SetFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.SetFormat(output_index); 
		save_option.SetQuality(quality); 
		error = bitmap.SaveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
	}
}

/*
 * Load a picture, change his color depth and save it
 */
void Test5File( char *filename, char *dstfilename, int output_index, GFL_MODE mode, GFL_MODE_PARAMS option )
{
	GFLW_LOAD_PARAMS
		load_option; 
	GFLW_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.LoadFromFile( filename, load_option ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLW_SAVE_PARAMS 
			save_option; 
		
		bitmap.ChangeColorDepth( mode, option ); 
		
		if (bitmap.HasColorMap())
		{
			for (int i=0; i<bitmap.GetColorMap().GetNumberOfColors(); ++i)
				cout << "RGB: " << i << " " << (GFL_UINT32) bitmap.GetColorMap().GetRedAt(i) << " " << (GFL_UINT32) bitmap.GetColorMap().GetGreenAt(i)<< " " << (GFL_UINT32) bitmap.GetColorMap().GetBlueAt(i) << endl; 
		}
		save_option.SetFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.SetFormat(output_index); 
		error = bitmap.SaveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
	}
}

/*
 * Load a picture (in Bottom-Left origin), and save it
 */
void Test6File( char *filename, char *dstfilename )
{
	GFLW_LOAD_PARAMS
		load_option; 
	GFLW_FILE_INFORMATION
		file_info; 
	GFLW_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 
	
	load_option.SetOrigin(GFL_BOTTOM_LEFT); 
	error = bitmap.LoadFromFile( filename, load_option, file_info ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLW_SAVE_PARAMS 
			save_option; 
		
		save_option.SetFormat(file_info.GetIndexOfFormat()); 
		save_option.SetCompression(GFL_RLE); 
		error = bitmap.SaveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
		
		bitmap.Empty(); 
	}
}

/*
 * Create a picture, and save it
 */
void Test7File( char *format_name )
{
	GFLW_BITMAP 
		bitmap; 
	GFLW_SAVE_PARAMS 
		save_option; 
		
	bitmap.Allocate(GFL_RGB, 256, 256); 

	GFL_UINT8
		* ptr = (GFL_UINT8 *) bitmap.GetDataPointer(); 
	for (int i=0; i<bitmap.GetHeight(); ++i)
		for (int j=0; j<bitmap.GetWidth(); ++j)
		{
			*ptr++ = i; 
			*ptr++ = j; 
			*ptr++ = i+j; 
		}

	GFLW_FORMAT
		format(format_name); 
	save_option.SetFormat(format); 
	bitmap.SaveIntoFile( DIR"test20.jpg", save_option ); 
/*
	bitmap.Negative(); 
	bitmap.SaveIntoFile( DIR"test21.jpg", save_option ); 
	bitmap.Negative(); 
*/
	bitmap.Rotate(45); 
	bitmap.SaveIntoFile( DIR"test22.jpg", save_option ); 
/*
	bitmap.Brightness(50); 
	bitmap.SaveIntoFile( DIR"test23.jpg", save_option ); 

	bitmap.Contrast(50); 
	bitmap.SaveIntoFile( DIR"test24.jpg", save_option ); 

	bitmap.Gamma(1.2); 
	bitmap.SaveIntoFile( DIR"test25.jpg", save_option ); 

	bitmap.Sharpen(96); 
	bitmap.SaveIntoFile( DIR"test26.jpg", save_option ); 

	bitmap.Blur(96); 
	bitmap.SaveIntoFile( DIR"test27.jpg", save_option ); 
*/
}

/*
 * Create a picture, make drawing and save it
 */
void Test8File( void )
{
	GFLW_BITMAP 
		bitmap; 
		
	bitmap.Allocate(GFL_RGB, 256, 256); 
	bitmap.SetColor(GFLW_COLOR(255, 255, 255)); 
	bitmap.SetFillColor(GFLW_COLOR(255, 0, 0)); 

	bitmap.DrawCircle(150, 150, 20); 

	bitmap.SetLineWidth(4); 
	bitmap.DrawFillRectangle(20, 20, 100, 50); 

	GFLW_FORMAT
		format("jpeg"); 
	GFLW_SAVE_PARAMS 
		save_option; 
	save_option.SetFormat(format); 
	bitmap.SaveIntoFile( DIR"test30.jpg", save_option ); 
}

void go(void)
{
	GFLW_LIBRARY::Initialise(); 
	GFLW_LIBRARY::EnableLZW(GFL_TRUE); 
	
	GFLW_FORMAT
		format; 
	GFL_ERROR
		ret; 

	format.FindByName("jpeg"); 
	PrintInfo(format); 
	format.FindByFilename("toto.tif"); 
	PrintInfo(format); 
	format.FindByIndex(10); 
	PrintInfo(format); 
	
	ret = GFLW_FORMAT::FindFirst(format); 
	while ( ret == GFL_NO_ERROR )
	{
		ret = GFLW_FORMAT::FindNext(format); 
		PrintInfo(format); 
	}

	GetInfo( DIR"test1" ); 
	TestFile( DIR"test1", DIR"test1.bmp", -1 ); 
	GetInfo( DIR"test1.bmp" ); 
	TestFile( DIR"test2", DIR"test2.bmp", -1 ); 
	GetInfo( DIR"test2.bmp" ); 
	
	GetInfo( DIR"test3" ); 
	TestFile( DIR"test3", DIR"test3.bmp", GFLW_FORMAT("jpeg").GetIndex() ); 
	GetInfo( DIR"test2.bmp" ); 
	TestFile( DIR"test4", DIR"test.bmp", -1 ); 
	
	Test2File( DIR"test2", DIR"test4.bmp", GFLW_FORMAT("bmp").GetIndex() ); 
	GetInfo( DIR"test4.bmp" ); 
	
	Test3File( DIR"test2", DIR"test5.bmp", GFLW_FORMAT("bmp").GetIndex() ); 
	GetInfo( DIR"test5.bmp" ); 
	
	Test4File( DIR"test2", DIR"test6.jpg", GFLW_FORMAT("jpeg").GetIndex(), 5 ); 
	Test4File( DIR"test2", DIR"test7.jpg", GFLW_FORMAT("jpeg").GetIndex(), 80 ); 
	
	Test5File( DIR"test2", DIR"test10.gif", GFLW_FORMAT("gif").GetIndex(), GFL_MODE_TO_BINARY, GFL_MODE_FLOYD_STEINBERG ); 
	Test5File( DIR"test2", DIR"test11.gif", GFLW_FORMAT("gif").GetIndex(), GFL_MODE_TO_64GREY, GFL_MODE_PATTERN_DITHER ); 
	Test5File( DIR"test2", DIR"test12.gif", GFLW_FORMAT("gif").GetIndex(), GFL_MODE_TO_64GREY, GFL_MODE_ADAPTIVE ); 
	Test5File( DIR"test2", DIR"test13.gif", GFLW_FORMAT("gif").GetIndex(), GFL_MODE_TO_216COLORS, GFL_MODE_PATTERN_DITHER ); 
	Test5File( DIR"test2", DIR"test14.gif", GFLW_FORMAT("gif").GetIndex(), GFL_MODE_TO_216COLORS, GFL_MODE_ADAPTIVE ); 
	Test5File( DIR"test10.gif", DIR"test15.jpg", GFLW_FORMAT("jpeg").GetIndex(), GFL_MODE_TO_RGB, 0 ); 
	
	Test6File( DIR"test1", DIR"test16.png" ); 

	Test7File( "jpeg" ); 

	Test8File(); 

	GFLW_LIBRARY::Exit(); 
}

main()
{
	try {
		go(); 
	}
	catch(const GFLW_EXCEPTION msg)
	{
		cout << "Error... file:" << msg.GetFile() << "  line:" << msg.GetLine() << "  => " << msg.GetError() << endl; 
	}
	return 0; 
}
