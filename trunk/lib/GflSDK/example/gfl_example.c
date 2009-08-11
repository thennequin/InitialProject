#include <stdio.h>

#include "libgfl.h"
#include "libgfle.h"

#define DIR "../data/"

#ifndef max
#define max(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#endif

void GetInfo( char *filename )
{
	char * origin; 
	int depth, i; 
	GFL_FILE_INFORMATION info; 

	if ( gflGetFileInformation( filename, -1, &info ) != GFL_NO_ERROR )
		return; 

  switch(info.Origin)
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
  
	depth = info.ComponentsPerPixel * info.BitsPerComponent; 
	printf( "\n%s : Success\n", filename); 
	printf( "    Size               : %d bytes\n", info.FileSize);
	printf( "    Format             : %s (%s - %d)\n", info.Description, info.FormatName, (int )info.FormatIndex); 
	printf( "    Compression        : %s\n", info.CompressionDescription ); 
	printf( "    Width              : %d\n", info.Width);
	printf( "    Height             : %d\n", info.Height);
	printf( "    ComponentsPerPixel : %d\n", (int )info.ComponentsPerPixel);
	printf( "    BitsPerComponent   : %d\n", (int )info.BitsPerComponent);
	printf( "    Depth              : %d\n", depth);
	printf( "    # colors           : %lu\n", 1UL<<(depth >= 24 ? 24 : depth)); 
	printf( "    Orientation        : %s\n", origin); 
	printf( "    Color Model        : %s\n", gflGetLabelForColorModel(info.ColorModel)); 
	if ( info.Xdpi )
		printf( "    Xdpi               : %d\n", info.Xdpi); 
	else
		printf( "    Xdpi               : Not set\n"); 
	if ( info.Ydpi )
		printf( "    Ydpi               : %d\n", info.Ydpi); 
	else
		printf( "    Ydpi               : Not set\n"); 
	printf( "    Page(s)            : %d\n", info.NumberOfImages); 
	/*if ( info.Comment > 0 )
		printf( "    Comment         : %s\n", info.Comment ); */

	gflFreeFileInformation( &info ); 
}

void DisplayError( const char *filename, GFL_ERROR error )
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
	GFL_LOAD_PARAMS load_option; 
	GFL_BITMAP *bitmap; 
	GFL_FILE_INFORMATION file_info; 
	GFL_ERROR error; 

	gflGetDefaultLoadParams( &load_option ); 
/*load_option.ColorModel = GFL_ABGR; 
load_option.Origin = GFL_BOTTOM_LEFT; */
	error = gflLoadBitmap( filename, &bitmap, &load_option, &file_info ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 

		gflGetDefaultSaveParams( &save_option ); 
		
		save_option.Flags = GFL_SAVE_REPLACE_EXTENSION; 
		save_option.FormatIndex = output_index == -1 ? file_info.FormatIndex : output_index; 
		error = gflSaveBitmap( dstfilename, bitmap, &save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
		
		gflFreeBitmap( bitmap ); 
	}
}

/*
 * Load a picture preview, and save it 
 */
void Test2File( char *filename, char *dstfilename, int output_index )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_BITMAP *bitmap; 
	GFL_ERROR error; 

	gflGetDefaultPreviewParams( &load_option ); 
	error = gflLoadPreview( filename, 64, 64, &bitmap, &load_option, NULL ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 

		gflGetDefaultSaveParams( &save_option ); 

		save_option.Flags = GFL_SAVE_REPLACE_EXTENSION; 
		save_option.FormatIndex = output_index; 
		error = gflSaveBitmap( dstfilename, bitmap, &save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
		
		gflFreeBitmap( bitmap ); 
	}
}

/*
 * Load a picture, resize and save it 
 */
void Test3File( char *filename, char *dstfilename, int output_index )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_BITMAP *bitmap, *bitmap_dst; 
	GFL_ERROR error; 

	gflGetDefaultLoadParams( &load_option ); 
	error = gflLoadBitmap( filename, &bitmap, &load_option, NULL ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 

		if ( error = gflResize( bitmap, &bitmap_dst, 160, 120, GFL_RESIZE_BILINEAR, 0 ) )
			DisplayError( NULL, error ); 
		else
		{
			gflGetDefaultSaveParams( &save_option ); 

			save_option.Flags = GFL_SAVE_REPLACE_EXTENSION; 
			save_option.FormatIndex = output_index; 
			error = gflSaveBitmap( dstfilename, bitmap_dst, &save_option ); 
			if ( error )
				DisplayError( dstfilename, error ); 

			gflFreeBitmap( bitmap_dst ); 
		}

		gflFreeBitmap( bitmap ); 
	}
}

/*
 * Load a picture, save it in jpeg
 */
void Test4File( char *filename, char *dstfilename, int output_index, GFL_INT32 quality )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_BITMAP *bitmap; 
	GFL_ERROR error; 

	gflGetDefaultLoadParams( &load_option ); 
	error = gflLoadBitmap( filename, &bitmap, &load_option, NULL ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 

		gflGetDefaultSaveParams( &save_option ); 

		save_option.Flags = GFL_SAVE_REPLACE_EXTENSION; 
		save_option.FormatIndex = output_index; 
		save_option.Quality = quality; 
		error = gflSaveBitmap( dstfilename, bitmap, &save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 

		gflFreeBitmap( bitmap ); 
	}
}

/*
 * Load a picture, change his color depth and save it
 */
void Test5File( char *filename, char *dstfilename, int output_index, GFL_MODE mode, GFL_MODE_PARAMS option )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_BITMAP *bitmap; 
	GFL_ERROR error; 

	gflGetDefaultLoadParams( &load_option ); 
	error = gflLoadBitmap( filename, &bitmap, &load_option, NULL ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 
		
		gflChangeColorDepth( bitmap, NULL, mode, option ); 

		gflGetDefaultSaveParams( &save_option ); 

		save_option.Flags = GFL_SAVE_REPLACE_EXTENSION; 
		save_option.FormatIndex = output_index; 
		error = gflSaveBitmap( dstfilename, bitmap, &save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
		
		gflFreeBitmap( bitmap ); 
	}
}

/*
 * Load a picture (in Bottom-Left origin), and save it
 */
void Test6File( char *filename, char *dstfilename )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_FILE_INFORMATION file_info; 
	GFL_BITMAP *bitmap; 
	GFL_ERROR error; 

	gflGetDefaultLoadParams( &load_option ); 
	load_option.Origin = GFL_BOTTOM_LEFT; 
	error = gflLoadBitmap( filename, &bitmap, &load_option, &file_info ); 
/*bitmap->Origin = GFL_TOP_LEFT; */
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 
		
		gflGetDefaultSaveParams( &save_option ); 
		
		save_option.FormatIndex = file_info.FormatIndex; 
		error = gflSaveBitmap( dstfilename, bitmap, &save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 
		
		gflFreeBitmap( bitmap ); 
	}
}

/*
 * Create multi-page file
 */
void Test8File( char *dstfilename, int count, char *filename[] )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_SAVE_PARAMS save_option; 
	GFL_FILE_INFORMATION file_info; 
	GFL_BITMAP *bitmap; 
	GFL_ERROR error; 
	GFL_HANDLE handle; 
	int i; 

	gflGetDefaultSaveParams( &save_option ); 
	save_option.FormatIndex = gflGetFormatIndexByName("tiff"); 
	if ( error = gflFileCreate( &handle, dstfilename, count, &save_option ) )
	{
		DisplayError( dstfilename, error ); 
		return; 
	}

	for ( i=0; i<count; i++ )
	{
		gflGetDefaultLoadParams( &load_option ); 
		error = gflLoadBitmap( filename[i], &bitmap, &load_option, &file_info ); 
		if ( error )
			DisplayError( filename[i], error ); 
		else
		{
			gflFileAddPicture( handle, bitmap ); 
			gflFreeBitmap( bitmap ); 
		}
	}

	gflFileClose( handle ); 
}

/*
 * Load a picture with own callback
 */
typedef struct {
		GFL_UINT8 * data; 
		GFL_UINT32 index; 
		GFL_UINT32 length; 
	} MY_DATA; 

GFL_UINT32 GFLAPI myReadFunction(GFL_HANDLE handle, void *buffer, GFL_UINT32 size)
{
	MY_DATA *ptr = (MY_DATA *)handle; 
	if ( ptr->index+size >= ptr->length )
		size = ptr->length - ptr->index; 

	memcpy( buffer, ptr->data+ptr->index, size ); 
	ptr->index += size; 

	return size; 
}

GFL_UINT32 GFLAPI myTellFunction(GFL_HANDLE handle)
{
	MY_DATA *ptr = (MY_DATA *)handle; 
	return ptr->index; 
}

GFL_UINT32 GFLAPI mySeekFunction(GFL_HANDLE handle, GFL_INT32 offset, GFL_INT32 origin)
{
	MY_DATA *ptr = (MY_DATA *)handle; 
	if ( origin == SEEK_CUR )
		offset += ptr->index; 
	else
	if ( origin == SEEK_END )
		offset = ptr->length-ptr->index; 
	
	if ( offset > ptr->length )
		return (GFL_UINT32 )-1; 
		
	ptr->index = offset; 
	return ptr->index; 
}

GFL_UINT32 GFLAPI myWriteFunction2(GFL_HANDLE handle, const void *buffer, GFL_UINT32 size)
{
	return fwrite( buffer, 1, size, (FILE *)handle ); 
}

GFL_UINT32 GFLAPI myTellFunction2(GFL_HANDLE handle)
{
	return ftell( (FILE *)handle ); 
}

GFL_UINT32 GFLAPI mySeekFunction2(GFL_HANDLE handle, GFL_INT32 offset, GFL_INT32 origin)
{
	return fseek( (FILE *)handle, offset, origin ); 
}

void Test7File( char *filename, char *dstfilename )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_FILE_INFORMATION file_info; 
	GFL_BITMAP *bitmap; 
	GFL_ERROR error; 
	MY_DATA my_data; 
	FILE *fp; 

	fp = fopen( filename, "rb" ); 
	if ( fp == NULL )
		return; 

	fseek( fp, 0, SEEK_END ); 
	my_data.length = ftell( fp ); 
	fseek( fp, 0, SEEK_SET ); 
	my_data.data = (GFL_UINT8 *)malloc( my_data.length ); 
	my_data.index = 0; 
	fread( my_data.data, my_data.length, 1, fp ); 
	fclose( fp ); 

	gflGetDefaultLoadParams( &load_option ); 
	load_option.Callbacks.Read = myReadFunction; 
	load_option.Callbacks.Tell = myTellFunction; 
	load_option.Callbacks.Seek = mySeekFunction; 
	/*load_option.FormatIndex = gflGetFormatIndexByName("gif"); */
	error = gflLoadBitmapFromHandle( (GFL_HANDLE )&my_data, &bitmap, &load_option, &file_info ); 
	if (! error)
	{
		GFL_BITMAP *bitmap_dst; 
		gflChangeColorDepth( bitmap, &bitmap_dst, GFL_MODE_TO_RGB, 0); 
		gflFreeBitmap(bitmap); 
		bitmap = bitmap_dst; 
	}

	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 
		FILE *fp; 
		
		if ( fp = fopen( dstfilename, "wb" ) )
		{
			gflGetDefaultSaveParams( &save_option ); 
			save_option.Callbacks.Write = myWriteFunction2; 
			save_option.Callbacks.Tell = myTellFunction2; 
			save_option.Callbacks.Seek = mySeekFunction2; 
			save_option.FormatIndex = gflGetFormatIndexByName("png"); 
			bitmap->TransparentIndex = 8; 
			error = gflSaveBitmapIntoHandle( fp, bitmap, &save_option ); 
			if ( error )
				DisplayError( dstfilename, error ); 

			fclose( fp ); 
		}
		
		gflFreeBitmap( bitmap ); 
	}
	
	free( my_data.data ); 
}

typedef struct {
		GFL_INT32 currentPosition; 
		GFL_INT32 currentSize; 
		GFL_INT32 currentAllocatedSize; 
		GFL_UINT8 *data; 
	} MY_DATA2; 

GFL_UINT32 GFLAPI myWriteFunction3(GFL_HANDLE handle, const void *buffer, GFL_UINT32 size)
{
	MY_DATA2 *my_data = (MY_DATA2 *)handle; 

	if ( my_data->currentPosition+size >= my_data->currentAllocatedSize )
	{
		my_data->currentAllocatedSize += max(16384, size); 
		if ( my_data->data == NULL )
			my_data->data = (GFL_UINT8 *)gflMemoryAlloc( my_data->currentAllocatedSize ); 
		else
			my_data->data = (GFL_UINT8 *)gflMemoryRealloc( my_data->data, my_data->currentAllocatedSize ); 

		if ( my_data->data == NULL )
			return 0; 
	}
	memcpy( my_data->data+my_data->currentPosition, buffer, size ); 
	my_data->currentPosition += size; 
	my_data->currentSize += size; 

	return size; 
}

GFL_UINT32 GFLAPI myTellFunction3(GFL_HANDLE handle)
{
	MY_DATA2 *my_data = (MY_DATA2 *)handle; 
	return my_data->currentPosition; 
}

GFL_UINT32 GFLAPI mySeekFunction3(GFL_HANDLE handle, GFL_INT32 offset, GFL_INT32 origin)
{
	MY_DATA2 *my_data = (MY_DATA2 *)handle; 
	if ( offset >= my_data->currentSize )
		return (GFL_UINT32 )-1; 
	
	my_data->currentPosition = offset; 
	return 0; 
}

void Test9File( char *filename, char *dstfilename )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_FILE_INFORMATION file_info; 
	GFL_BITMAP *bitmap; 
	GFL_ERROR error; 
	MY_DATA2 my_data; 

	gflGetDefaultLoadParams( &load_option ); 
	error = gflLoadBitmap( filename, &bitmap, &load_option, &file_info ); 
	if ( error )
		DisplayError( filename, error ); 
	else
	{
		GFL_SAVE_PARAMS save_option; 
		FILE *fp; 
		
		my_data.currentAllocatedSize = my_data.currentPosition = my_data.currentSize = 0; 
		my_data.data = NULL; 

		gflGetDefaultSaveParams( &save_option ); 
		save_option.Callbacks.Write = myWriteFunction3; 
		save_option.Callbacks.Tell = myTellFunction3; 
		save_option.Callbacks.Seek = mySeekFunction3; 
		save_option.FormatIndex = gflGetFormatIndexByName("png"); 
		error = gflSaveBitmapIntoHandle( &my_data, bitmap, &save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 

		if ( fp = fopen( dstfilename, "wb" ) )
		{
			fwrite( my_data.data, my_data.currentSize, 1, fp ); 
			fclose( fp ); 
		}
		
		gflFreeBitmap( bitmap ); 
	}
	
	gflMemoryFree( my_data.data ); 
}

/*
 * Merge 4 picture into one
 */
void Test10File( char *filename[], char *dstfilename, GFL_INT32 index )
{
	GFL_LOAD_PARAMS load_option; 
	GFL_FILE_INFORMATION file_info; 
	GFL_ERROR error; 
	GFL_BITMAP * bitmap_table[4], * new_bitmap; 
	GFL_UINT32 opacity[4] = { 40, 25, 15, 20 }; 
	GFL_POINT points[4] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} }; 
	GFL_SAVE_PARAMS save_option; 
	GFL_INT32 i; 

	for ( i=0; i<4; i++ )
	{
		gflGetDefaultLoadParams( &load_option ); 
		error = gflLoadBitmap( filename[i], &bitmap_table[i], &load_option, &file_info ); 
		if ( error )
		{
			DisplayError( filename[i], error ); 
			return; 
		}
	}

	error = gflMerge( bitmap_table, points, opacity, 4, &new_bitmap ); 
	for ( i=0; i<4; i++ )
		gflFreeBitmap(bitmap_table[i]); 
	
	if (! error )
	{
		gflGetDefaultSaveParams( &save_option ); 
		save_option.FormatIndex = index; 

		error = gflSaveBitmap( dstfilename, new_bitmap, &save_option ); 
		if ( error )
			DisplayError( dstfilename, error ); 

		gflFreeBitmap(new_bitmap); 
	}
}

main()
{
	int i, j; 
	GFL_FORMAT_INFORMATION format_info; 
	char *filename_table[4]; 

	gflSetPluginsPathname("..\\plugins\\"); 
	gflLibraryInit(); 
	printf("==> %d \n", (int)gflGetNumberOfFormat()); 

	gflEnableLZW( GFL_TRUE ); 

	gflGetFormatInformationByName( "jpeg", &format_info ); 
	printf("Format <%s> \n", format_info.Description ); 
	
	for ( i=0; i<gflGetNumberOfFormat(); i++ )
	{
		gflGetFormatInformationByIndex( i, &format_info ); 

		printf("Format <%s> \n", format_info.Description ); 
		printf("       <%s> \n", format_info.Name ); 
		printf("       <%d> \n", (int )format_info.Index ); 
		printf("       <%x> \n", (unsigned int )format_info.Status ); 

		printf("       "); 
		for ( j=0; j<format_info.NumberOfExtension; j++ )
			printf(" .%s ", format_info.Extension[j] ); 
		printf("\n\n"); 
	}

	GetInfo( DIR"test1" ); 
	TestFile( DIR"test1", DIR"test1.bmp", gflGetFormatIndexByName("bmp") ); 
	GetInfo( DIR"test1.bmp" ); 
	TestFile( DIR"test2", DIR"test2.bmp", -1 ); 
	GetInfo( DIR"test2.bmp" ); 
	TestFile( DIR"test3", DIR"test3.bmp", gflGetFormatIndexByName("jpeg") ); 
	GetInfo( DIR"test2.bmp" ); 
	TestFile( DIR"test4", DIR"test.bmp", -1 ); 

	Test2File( DIR"test2", DIR"test4.bmp", gflGetFormatIndexByName("bmp") ); 
	GetInfo( DIR"test4.bmp" ); 

	Test3File( DIR"test2", DIR"test5.bmp", gflGetFormatIndexByName("bmp") ); 
	GetInfo( DIR"test5.bmp" ); 
	Test3File( DIR"test2", DIR"test5.tif", gflGetFormatIndexByName("tiff") ); 
	
	Test4File( DIR"test2", DIR"test6.jpg", gflGetFormatIndexByName("jpeg"), 5 ); 
	Test4File( DIR"test2", DIR"test7.jpg", gflGetFormatIndexByName("jpeg"), 80 ); 
	
	Test5File( DIR"test2", DIR"test10.gif", gflGetFormatIndexByName("gif"), GFL_MODE_TO_BINARY, GFL_MODE_FLOYD_STEINBERG ); 
	Test5File( DIR"test2", DIR"test11.gif", gflGetFormatIndexByName("gif"), GFL_MODE_TO_64GREY, GFL_MODE_PATTERN_DITHER ); 
	Test5File( DIR"test2", DIR"test12.gif", gflGetFormatIndexByName("gif"), GFL_MODE_TO_64GREY, GFL_MODE_ADAPTIVE ); 
	Test5File( DIR"test2", DIR"test13.gif", gflGetFormatIndexByName("gif"), GFL_MODE_TO_216COLORS, GFL_MODE_PATTERN_DITHER ); 
	Test5File( DIR"test2", DIR"test14.gif", gflGetFormatIndexByName("gif"), GFL_MODE_TO_216COLORS, GFL_MODE_ADAPTIVE ); 
	
	Test5File( DIR"test10.gif", DIR"test15.jpg", gflGetFormatIndexByName("jpeg"), GFL_MODE_TO_RGB, 0 ); 
	
	Test6File( DIR"test1", DIR"test16.png" ); 
	
	Test7File( DIR"test1", DIR"test17.png" ); 
	Test9File( DIR"test1", DIR"test18.png" ); 

	filename_table[0] = DIR"test1"; 
	filename_table[1] = DIR"test2"; 
	filename_table[2] = DIR"test3"; 
	Test8File( DIR"test1.tif", 3, filename_table ); 

	filename_table[0] = DIR"img0.tif"; 
	filename_table[1] = DIR"img1.tif"; 
	filename_table[2] = DIR"img2.tif"; 
	filename_table[3] = DIR"img3.tif"; 
	Test10File( filename_table, DIR"test20.png", gflGetFormatIndexByName("png") ); 

	gflLibraryExit(); 
}
