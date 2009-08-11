#include "gflC.h"

#define DIR "../data/"

void PrintInfo( GFLC_FORMAT &format )
{
	const char *ext; 

	printf("Index : %d ", (int )format.getIndex()); 
	printf("%s -", format.getName()); 
	printf("Read %3s -", format.isReadable() ? "Yes" : "No"); 
	printf("Write %3s \n", format.isWritable() ? "Yes" : "No"); 

	printf("  "); 
	ext = format.getExtensionFirst(); 
	while (ext)
	{
		printf("%s ", ext); 
		ext = format.getExtensionNext(); 
	}
	printf("\n"); 
}

void GetInfo( char *filename )
{
	const char 
		* origin; 
	GFLC_FILE_INFORMATION 
		info; 

	if ( info.getFromFile( filename ) != GFL_NO_ERROR )
		return; 
	
  switch (info.getOrigin())
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
	printf( "    Size               : %d bytes\n", info.getFileSize());
	printf( "    Format             : %s (%s - %d)\n", info.getDescription(), info.getNameOfFormat(), (int )info.getIndexOfFormat()); 
	printf( "    Compression        : %s\n", info.getCompressionDescription()); 
	printf( "    Width              : %d\n", info.getWidth());
	printf( "    Height             : %d\n", info.getHeight());
	printf( "    ComponentsPerPixel : %d\n", (int )info.getComponentsPerPixel());
	printf( "    BitsPerComponent   : %d\n", (int )info.getBitsPerComponent());
	printf( "    Depth              : %d\n", (int )info.getNumberOfBits());
	printf( "    # colors           : %lu\n", 1UL<<(info.getNumberOfBits() >= 24 ? 24 : info.getNumberOfBits())); 
	printf( "    Orientation        : %s\n", origin); 
	printf( "    Color Model        : %s\n", info.getLabelForColorModel()); 
	if ( info.getXdpi() )
		printf( "    Xdpi               : %d\n", info.getXdpi()); 
	else
		printf( "    Xdpi               : Not set\n"); 
	if ( info.getYdpi() )
		printf( "    Ydpi               : %d\n", info.getYdpi()); 
	else
		printf( "    Ydpi               : Not set\n"); 
	printf( "    Page(s)           : %d\n", info.getNumberOfImages()); 
	/*if ( info.HasComment() )
		printf( "    Comment         : %s \n", info.getComment() ); */
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
	GFLC_LOAD_PARAMS 
		load_option; 
	GFLC_BITMAP 
		bitmap; 
	GFLC_FILE_INFORMATION
		info; 
	GFL_ERROR 
		error; 
	
	error = bitmap.loadFromFile(filename, load_option, info); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLC_SAVE_PARAMS 
			save_option;
		
		save_option.setFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.setFormat(output_index == -1 ? info.getIndexOfFormat() : output_index); 
		
		error = bitmap.saveIntoFile(dstfilename, save_option); 
		if ( error )
			DisplayError( dstfilename, error ); 

		GFLC_BITMAP bitmap2(bitmap); 
		bitmap2.clear(); 
	}
}

/*
 * Load a picture preview, and save it 
 */
void Test2File( char *filename, char *dstfilename, int output_index )
{
	GFLC_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.loadThumbnailFromFile( filename, 64, 64 ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLC_SAVE_PARAMS
			save_option; 
		
		save_option.setFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.setFormat(output_index); 
		
		error = bitmap.saveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
	}
}

/*
 * Load a picture, resize and save it 
 */
void Test3File( char *filename, char *dstfilename, int output_index )
{
	GFLC_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.loadFromFile( filename ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLC_SAVE_PARAMS 
			save_option; 
		
		if ( error = bitmap.resize( 160, 120, GFL_RESIZE_BILINEAR, 0 ) )
			DisplayError( NULL, error ); 
		else
		{
			save_option.setFlags(GFL_SAVE_REPLACE_EXTENSION); 
			save_option.setFormat(output_index); 
			error = bitmap.saveIntoFile( dstfilename, save_option ); 
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
	GFLC_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.loadFromFile( filename ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLC_SAVE_PARAMS 
			save_option; 

		save_option.setFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.setFormat(output_index); 
		save_option.setQuality(quality); 
		error = bitmap.saveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
	}
}

/*
 * Load a picture, change his color depth and save it
 */
void Test5File( char *filename, char *dstfilename, int output_index, GFL_MODE mode, GFL_MODE_PARAMS option )
{
	GFLC_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 

	error = bitmap.loadFromFile( filename ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLC_SAVE_PARAMS 
			save_option; 
		
		bitmap.changeColorDepth( mode, option ); 
		
		if (bitmap.hasColorMap())
		{
			for (int i=0; i<bitmap.getColorMap().getNumberOfColors(); ++i)
				cout << "RGB: " << i << " " << (GFL_UINT32) bitmap.getColorMap().getRedAt(i) << " " << (GFL_UINT32) bitmap.getColorMap().getGreenAt(i)<< " " << (GFL_UINT32) bitmap.getColorMap().getBlueAt(i) << endl; 
		}
		save_option.setFlags(GFL_SAVE_REPLACE_EXTENSION); 
		save_option.setFormat(output_index); 
		error = bitmap.saveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
	}
}

/*
 * Load a picture (in Bottom-Left origin), and save it
 */
void Test6File( char *filename, char *dstfilename )
{
	GFLC_LOAD_PARAMS
		load_option; 
	GFLC_FILE_INFORMATION
		file_info; 
	GFLC_BITMAP 
		bitmap; 
	GFL_ERROR 
		error; 
	
	load_option.setOrigin(GFL_BOTTOM_LEFT); 
	error = bitmap.loadFromFile( filename, load_option, file_info ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFLC_SAVE_PARAMS 
			save_option; 
		
		save_option.setFormat(file_info.getIndexOfFormat()); 
		save_option.setCompression(GFL_RLE); 
		error = bitmap.saveIntoFile( dstfilename, save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
		
		bitmap.clear(); 
	}
}

/*
 * Create a picture, and save it
 */
void Test7File( char *format_name )
{
	GFLC_BITMAP 
		bitmap(GFL_RGB, 256, 256); 
	
	for (int i=0; i<bitmap.getHeight(); ++i)
		for (int j=0; j<bitmap.getWidth(); ++j)
			bitmap.setPixel(i, j, GFLC_COLOR(i,j,i+j)); 
	
	bitmap.saveIntoFile( DIR"test20.jpg", format_name ); 
/*
	bitmap.negative(); 
	bitmap.saveIntoFile( DIR"test21.jpg", format_name ); 
	bitmap.negative(); 
*/
	bitmap.rotate(45); 
	bitmap.saveIntoFile( DIR"test22.jpg", format_name ); 
/*
	bitmap.brightness(50); 
	bitmap.saveIntoFile( DIR"test23.jpg", format_name ); 

	bitmap.contrast(50); 
	bitmap.saveIntoFile( DIR"test24.jpg", format_name ); 

	bitmap.gamma(1.2); 
	bitmap.saveIntoFile( DIR"test25.jpg", format_name ); 

	bitmap.sharpen(96); 
	bitmap.saveIntoFile( DIR"test26.jpg", format_name ); 

	bitmap.blur(96); 
	bitmap.saveIntoFile( DIR"test27.jpg", format_name ); 
*/
}

/*
 * Create a picture, make drawing and save it
 */
#ifdef __GFLC_DRAW_FUNCTIONS__
void Test8File( void )
{
	GFLC_BITMAP 
		bitmap; 
		
	bitmap.Create(GFL_RGB, 256, 256); 
	bitmap.Color(GFLC_COLOR(255, 255, 255)); 
	bitmap.FillColor(GFLC_COLOR(255, 0, 0)); 

	bitmap.DrawCircle(150, 150, 20); 

	bitmap.LineWidth(4); 
	bitmap.DrawFillRectangle(20, 20, 100, 50); 

	bitmap.SaveIntoFile( DIR"test30.jpg", "jpeg" ); 
}
#endif

void go(void)
{
	GFLC_LIBRARY::initialise(); 
	GFLC_LIBRARY::enableLZW(GFL_TRUE); 
	
	GFLC_FORMAT
		format; 
	GFL_ERROR
		ret; 

	format.findByName("jpeg"); 
	PrintInfo(format); 
	format.findByFilename("toto.tif"); 
	PrintInfo(format); 
	format.findByIndex(10); 
	PrintInfo(format); 
	
	ret = GFLC_FORMAT::findFirst(format); 
	while ( ret == GFL_NO_ERROR )
	{
		ret = GFLC_FORMAT::findNext(format); 
		PrintInfo(format); 
	}

	GetInfo( DIR"test1" ); 
	TestFile( DIR"test1", DIR"test1.bmp", -1 ); 
	GetInfo( DIR"test1.bmp" ); 
	TestFile( DIR"test2", DIR"test2.bmp", -1 ); 
	GetInfo( DIR"test2.bmp" ); 
	
	GetInfo( DIR"test3" ); 
	TestFile( DIR"test3", DIR"test3.bmp", GFLC_FORMAT("jpeg").getIndex() ); 
	GetInfo( DIR"test2.bmp" ); 
	TestFile( DIR"test4", DIR"test.bmp", -1 ); 
	
	Test2File( DIR"test2", DIR"test4.bmp", GFLC_FORMAT("bmp").getIndex() ); 
	GetInfo( DIR"test4.bmp" ); 
	
	Test3File( DIR"test2", DIR"test5.bmp", GFLC_FORMAT("bmp").getIndex() ); 
	GetInfo( DIR"test5.bmp" ); 
	
	Test4File( DIR"test2", DIR"test6.jpg", GFLC_FORMAT("jpeg").getIndex(), 5 ); 
	Test4File( DIR"test2", DIR"test7.jpg", GFLC_FORMAT("jpeg").getIndex(), 80 ); 
	
	Test5File( DIR"test2", DIR"test10.gif", GFLC_FORMAT("gif").getIndex(), GFL_MODE_TO_BINARY, GFL_MODE_FLOYD_STEINBERG ); 
	Test5File( DIR"test2", DIR"test11.gif", GFLC_FORMAT("gif").getIndex(), GFL_MODE_TO_64GREY, GFL_MODE_PATTERN_DITHER ); 
	Test5File( DIR"test2", DIR"test12.gif", GFLC_FORMAT("gif").getIndex(), GFL_MODE_TO_64GREY, GFL_MODE_ADAPTIVE ); 
	Test5File( DIR"test2", DIR"test13.gif", GFLC_FORMAT("gif").getIndex(), GFL_MODE_TO_216COLORS, GFL_MODE_PATTERN_DITHER ); 
	Test5File( DIR"test2", DIR"test14.gif", GFLC_FORMAT("gif").getIndex(), GFL_MODE_TO_216COLORS, GFL_MODE_ADAPTIVE ); 
	Test5File( DIR"test10.gif", DIR"test15.jpg", GFLC_FORMAT("jpeg").getIndex(), GFL_MODE_TO_RGB, 0 ); 
	
	Test6File( DIR"test1", DIR"test16.png" ); 

	Test7File( "jpeg" ); 

#ifdef __GFLC_DRAW_FUNCTIONS__
	Test8File(); 
#endif

	GFLC_LIBRARY::exit(); 
}

main()
{
	try {
		go(); 
	}
	catch(const GFLC_EXCEPTION msg)
	{
		cout << "Error... file:" << msg.getFile() << "  line:" << msg.getLine() << "  => " << msg.getError() << endl; 
	}
	return 0; 
}
