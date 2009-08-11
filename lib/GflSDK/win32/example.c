#include <windows.h>
#include "libgfl.h"
#include "libgfle.h"
#include "resource.h"
#include <commctrl.h>

HINSTANCE hInstance; 

#define __USE_HBITMAP__

#ifdef __USE_HBITMAP__
static HBITMAP ThehBitmap = NULL; 
#else
static GFL_BITMAP *TheBitmap=NULL; 
#endif

static int SortItem( const void *elmt1, const void *elmt2 )
  {
  GFL_FORMAT_INFORMATION *format1=(GFL_FORMAT_INFORMATION *)elmt1, *format2=(GFL_FORMAT_INFORMATION *)elmt2;

		if ( format1->NumberOfExtension && ! format2->NumberOfExtension )
			return -1; 
		if (! format1->NumberOfExtension && format2->NumberOfExtension )
			return 1; 
		if (! format1->NumberOfExtension && ! format2->NumberOfExtension )
			return strcmp( format1->Description, format2->Description ); 

    return strcmp( format1->Extension[0], format2->Extension[0] ); 
  }

static int SortItem2( const void *elmt1, const void *elmt2 )
  {
  GFL_FORMAT_INFORMATION *format1=(GFL_FORMAT_INFORMATION *)elmt1, *format2=(GFL_FORMAT_INFORMATION *)elmt2;

		return strcmp( format1->Description, format2->Description ); 
  }

static void FormatsListCreate( HWND hwnd )
	{
	LV_COLUMN pcol; 
  LV_ITEM lvi; 
	int i, j; 
	GFL_FORMAT_INFORMATION * list; 
	char extension[128]; 

		pcol.mask = LVCF_TEXT; 
		pcol.fmt = LVCFMT_LEFT;
		pcol.pszText = "Name";
		pcol.cchTextMax = strlen(pcol.pszText);
		pcol.iSubItem = 0; 
		ListView_InsertColumn( hwnd, 0, &pcol );  
		
		pcol.pszText = "Readable";
		pcol.cchTextMax = strlen(pcol.pszText);
		ListView_InsertColumn( hwnd, 1, &pcol );  
		
		pcol.pszText = "Writable";
		pcol.cchTextMax = strlen(pcol.pszText);
		ListView_InsertColumn( hwnd, 2, &pcol );  
		
		pcol.pszText = "Extension";
		pcol.cchTextMax = strlen(pcol.pszText);
		ListView_InsertColumn( hwnd, 3, &pcol ); 
		
		list = malloc( gflGetNumberOfFormat() * sizeof(GFL_FORMAT_INFORMATION) ); 
		for ( i=0; i<gflGetNumberOfFormat(); i++ )
			gflGetFormatInformationByIndex( i, &list[i] ); 
		qsort( list, gflGetNumberOfFormat(), sizeof(GFL_FORMAT_INFORMATION), SortItem2 ); 

		for ( i=0; i<gflGetNumberOfFormat(); i++ )
		{
			extension[0] = '\0'; 
			for ( j=0; j<list[i].NumberOfExtension; j++ )
			{
				strcat( extension, list[i].Extension[j] ); 
				if ( j != list[i].NumberOfExtension-1 )
					strcat( extension, "," ); 
			}
			
      lvi.mask = LVIF_TEXT;
      lvi.iItem = i;
      lvi.iSubItem = 0; 
      lvi.pszText = list[i].Description; 
      ListView_InsertItem( hwnd, &lvi );
      
      ListView_SetItemText( hwnd, i, 1, list[i].Status & GFL_READ ? "Yes" : "No" ); 
      ListView_SetItemText( hwnd, i, 2, list[i].Status & GFL_WRITE ? "Yes" : "No" ); 
      ListView_SetItemText( hwnd, i, 3, extension ); 
		}
		
    ListView_SetColumnWidth( hwnd, 0, LVSCW_AUTOSIZE );
    ListView_SetColumnWidth( hwnd, 1, LVSCW_AUTOSIZE_USEHEADER );
    ListView_SetColumnWidth( hwnd, 2, LVSCW_AUTOSIZE_USEHEADER );
    ListView_SetColumnWidth( hwnd, 3, LVSCW_AUTOSIZE );
	  ListView_SetExtendedListViewStyle( hwnd, LVS_EX_FULLROWSELECT ); 

		free( list ); 
	}

static BOOL CALLBACK HelpDlgProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
  {
		switch( msg )
		{
		case WM_INITDIALOG:
			FormatsListCreate( GetDlgItem( hwnd, IDC_LIST2 ) ); 
			break; 

		case WM_COMMAND:
			if ( LOWORD(wparam) == IDOK
				|| LOWORD(wparam) == IDCANCEL )
			{
				EndDialog( hwnd, TRUE );
				return TRUE; 
			}
		}
		
    return FALSE; 
  }

static void OpenFormatsDialog( HWND hwnd )
	{
		DialogBox( hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, HelpDlgProc ); 
	}

static int OpenFileDialog( HWND hwnd, char *filename )
	{
  OPENFILENAME ofname;
	GFL_FORMAT_INFORMATION * list; 
  char *filter, *string, *ptr;
	int i, k, ret; 

		filter = malloc( 16384 ); 
		list = malloc( gflGetNumberOfFormat() * sizeof(GFL_FORMAT_INFORMATION) ); 
		
    for ( i=0; i<gflGetNumberOfFormat(); i++ )
      gflGetFormatInformationByIndex( i, &list[i] );
		
    qsort( list, gflGetNumberOfFormat(), sizeof(GFL_FORMAT_INFORMATION), SortItem );
    
		ptr = filter; 
    strcpy( ptr, "All files" ); 
    ptr += strlen( ptr ); 
    *ptr++ = '\0'; 
    strcpy( ptr, "*.*" ); 
    ptr += strlen( ptr ); 
		*ptr++ = '\0'; 
    for ( i=0; i<gflGetNumberOfFormat(); i++)
    {
			if (! ( list[i].Status & GFL_READ ) )
        continue;
      
			if ( list[i].NumberOfExtension )
			{
				strcpy( ptr, list[i].Extension[0] ); 
				strupr( ptr ); 
				strcat( ptr, " - " ); 
			}
			else
				*ptr = '\0'; 
      strcat( ptr, list[i].Description ); 
      ptr += strlen( ptr ); 
      *ptr = '\0'; 
			++ptr; 
			
      if ( list[i].NumberOfExtension )
      {
        for ( k=0; k<list[i].NumberOfExtension; k++ )
        {
          strcpy( ptr, "*." ); 
          strcat( ptr, list[i].Extension[k] );
          strcat( ptr, ";" ); 
          
          ptr += strlen( ptr );
        }
        ptr--; 
      }
      else
      {
        strcpy( ptr, "*.*" ); 
        ptr += strlen(  "*.*" ); 
      }
      *ptr++ = '\0'; 
    }
    *ptr++ = '\0'; 
    *ptr++ = '\0';
    
    string = calloc( 1, 10000 );
    
    memset( &ofname, 0, sizeof(OPENFILENAME) ); 
    ofname.lStructSize = sizeof(OPENFILENAME);
    ofname.hwndOwner = hwnd;
    ofname.lpstrFilter = filter; 
    ofname.nFilterIndex = 0; 
    ofname.lpstrFile = string;
    ofname.nMaxFile = 10000;
    ofname.lpstrInitialDir = NULL; 
    ofname.lpstrTitle = "Open picture...";
    ofname.hInstance = hInstance; 
    ofname.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING; 
    
		ret = FALSE; 
    if ( GetOpenFileName( &ofname ) )
    {
			strcpy( filename, string ); 
      ret = TRUE;
    }
    
		free( filter ); 
		free( list ); 
		free( string ); 

    return ret; 
	}

static void GetDIBFromBitmap( GFL_BITMAP *bitmap, LPBITMAPINFOHEADER * pbitmap_info, unsigned char **data )
	{
	int i, j, bytes_per_line; 
	unsigned char *ptr_src, *ptr_dst; 
	LPBITMAPINFOHEADER bitmap_info; 
	RGBQUAD *colormap; 

		*data = NULL; 
		*pbitmap_info = NULL; 

		bitmap_info = *pbitmap_info = calloc( 1, sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) ); 
		colormap = (unsigned char *)bitmap_info + sizeof(BITMAPINFOHEADER); 
		
		bitmap_info->biSize   = sizeof(BITMAPINFOHEADER); 
		bitmap_info->biWidth  = bitmap->Width; 
		bitmap_info->biHeight = bitmap->Height; 
		bitmap_info->biPlanes = 1; 
		
		if ( bitmap->Type == GFL_BINARY )
		{
			bytes_per_line = ((bitmap->Width + 31) / 32) * 4; 
			bitmap_info->biClrUsed = 2;
			bitmap_info->biBitCount = 1; 
		}
		else
		if ( bitmap->Type == GFL_GREY
			|| bitmap->Type == GFL_COLORS )
		{
			bytes_per_line = (bitmap->Width + 3) & ~3; 
			bitmap_info->biClrUsed = 256;
			bitmap_info->biBitCount = 8; 
		}
		else
		{
			bytes_per_line = (bitmap->Width * 3 + 3) & ~3; 
			bitmap_info->biClrUsed = 0;
			bitmap_info->biBitCount = 24; 
		}
    bitmap_info->biCompression = BI_RGB; 
    bitmap_info->biSizeImage = bytes_per_line * bitmap->Height; 
    bitmap_info->biClrImportant = 0; 
		
		if ( bitmap_info->biClrUsed > 0 )
		{
			if ( bitmap->Type == GFL_BINARY )
			{
				colormap[0].rgbRed   = 0; 
        colormap[0].rgbGreen = 0; 
        colormap[0].rgbBlue  = 0;

				colormap[0].rgbRed   = 255; 
        colormap[0].rgbGreen = 255; 
        colormap[0].rgbBlue  = 255;
			}
			else
			if ( bitmap->Type == GFL_GREY )
			{
        for ( i=0; i<bitmap->ColorUsed; i++ )
        {
					colormap[i].rgbRed   = (i * 255) / (bitmap->ColorUsed-1); 
					colormap[i].rgbGreen = colormap[i].rgbRed; 
					colormap[i].rgbBlue  = colormap[i].rgbRed; 
				}
			}
			else
			{
        for ( i=0; i<bitmap->ColorUsed; i++ )
        {
					colormap[i].rgbRed   = bitmap->ColorMap->Red[i]; 
					colormap[i].rgbGreen = bitmap->ColorMap->Green[i]; 
					colormap[i].rgbBlue  = bitmap->ColorMap->Blue[i]; 
				}
			}
		}

		/*
		if ( bitmap->Type != GFL_BINARY
			&& bitmap->Type != GFL_GREY
			&& bitmap->Type != GFL_COLORS )
			return; 
		
		*data = malloc( bitmap_info->biSizeImage ); 
		
		for ( i=0; i<bitmap->Height; i++ )
		{
			ptr_src = bitmap->Data + i * bitmap->BytesPerLine; 
			ptr_dst = *data + i * bytes_per_line; 

			if ( bitmap->Type == GFL_BINARY
				|| bitmap->Type == GFL_GREY )
			{
				for ( j=0; j<bitmap->Width; j++ )
				{
					*ptr_dst++ = *ptr_src; 
					*ptr_dst++ = *ptr_src; 
					*ptr_dst++ = *ptr_src++; 
				}
			}
			else
			if ( bitmap->Type == GFL_COLORS )
			{
				for ( j=0; j<bitmap->Width; j++ )
				{
					*ptr_dst++ = bitmap->ColorMap->Blue[ *ptr_src ]; 
					*ptr_dst++ = bitmap->ColorMap->Green[ *ptr_src ]; 
					*ptr_dst++ = bitmap->ColorMap->Red[ *ptr_src++ ]; 
				}
			}
			else
			{
				for ( j=0; j<bitmap->Width; j++ )
				{
					*ptr_dst++ = ptr_src[2]; 
					*ptr_dst++ = ptr_src[1]; 
					*ptr_dst++ = ptr_src[0]; 
					ptr_src += bitmap->BytesPerPixel; 
				}
			}
		}
		*/
	}

static void LoadTheBitmap( HWND hwnd, const char *filename )
	{
	GFL_LOAD_PARAMS load_params; 

#ifdef __USE_HBITMAP__
	if ( ThehBitmap )
		DeleteObject( ThehBitmap ); 
#else
		if ( TheBitmap )
			gflFreeBitmap( TheBitmap ); 
		TheBitmap = NULL; 
#endif
		
		gflGetDefaultLoadParams( &load_params ); 
		//load_params.Flags |= GFL_LOAD_SKIP_ALPHA; 
		load_params.Origin = GFL_BOTTOM_LEFT; 
		load_params.ColorModel = GFL_BGR; 
		//load_params.LinePadding = 4; 
#ifdef __USE_HBITMAP__
		if ( gflLoadBitmapIntoDDB( filename, &ThehBitmap, &load_params, NULL ) )
#else
		if ( gflLoadBitmap( filename, &TheBitmap, &load_params, NULL ) )
#endif
			MessageBox( hwnd, "Error loading file...", "Error", MB_OK ); 
		else
		{
			/*
			HANDLE hBmp = NULL;

			gflCon
			vertBitmapIntoDIB(TheBitmap, &hBmp);
			gflFreeBitmap(TheBitmap);
			TheBitmap = NULL;

			gflConvertDIBIntoBitmap(hBmp, &TheBitmap);
			GlobalFree(hBmp);
			*/

			InvalidateRect( hwnd, NULL, TRUE ); 
		}
	}

static LRESULT APIENTRY MainWindowProc( HWND hwnd, UINT msg, UINT wparam, LONG lparam )
  {
		switch (msg)
		{
		case WM_COMMAND:
			switch( LOWORD(wparam) )
			{
			case ID_EXIT:
				SendMessage( hwnd, WM_DESTROY, 0, 0 ); 
				break; 
			
			case ID_OPEN:
				{
				char filename[MAX_PATH]; 

					if ( OpenFileDialog( hwnd, filename ) )
						LoadTheBitmap( hwnd, filename ); 
				}
				break; 
			
			case ID_ABOUT_FORMATS:
				OpenFormatsDialog( hwnd ); 
				break; 
			}
			break; 

		case WM_PAINT:
			{
			PAINTSTRUCT ps;
			HDC hDC; 
			
				BeginPaint( hwnd, &ps ); 
				hDC = GetDC( hwnd ); 
				
#ifdef __USE_HBITMAP__
				if ( ThehBitmap )
				{
					HDC hMemoryDC; 
					HBITMAP hOldBitmap; 
					BITMAP bm; 

					hMemoryDC = CreateCompatibleDC( hDC ); 
					hOldBitmap = SelectObject( hMemoryDC, ThehBitmap ); 

					GetObject( ThehBitmap, sizeof(BITMAP), &bm ); 

					StretchBlt( hDC, 
						0, 0, bm.bmWidth, bm.bmHeight, 
						hMemoryDC, 
						0, 0, bm.bmWidth, bm.bmHeight, 
						SRCCOPY ); 

					SelectObject( hMemoryDC, hOldBitmap ); 
					DeleteDC( hMemoryDC ); 
				}
#else
				if ( TheBitmap )
				/*
				{
				HBITMAP hBitmap; 
				HDC hOldDC, hMemoryDC; 
				HBRUSH hBrush, hOldBrush; 
				RECT rect; 

					gflConvertBitmapIntoDDB( TheBitmap, &hBitmap ); 

					hMemoryDC = CreateCompatibleDC( hDC ); 
					hOldDC = SelectObject( hMemoryDC, hBitmap ); 
					
					BitBlt( 
						hDC, 
						0, 0, TheBitmap->Width, TheBitmap->Height, 
						hMemoryDC, 
						0, 0, 
						SRCCOPY ); 
					
					SelectObject( hMemoryDC, hOldDC ); 
					DeleteDC( hMemoryDC ); 
					DeleteObject( hBitmap ); 
				}
				*/
				{
				unsigned char *dib_data; 
				LPBITMAPINFOHEADER dib_info; 
				
					GetDIBFromBitmap( TheBitmap, &dib_info, &dib_data ); 
					
					StretchDIBits( hDC, 
						0, 0, TheBitmap->Width, TheBitmap->Height, 
						0, 0, TheBitmap->Width, TheBitmap->Height, 
						dib_data ? dib_data : TheBitmap->Data, (BITMAPINFO *)dib_info, DIB_RGB_COLORS, SRCCOPY ); 
					
					if ( dib_data )
						free( dib_data ); 
					if ( dib_info )
						free( dib_info ); 
				}
#endif
				
				ReleaseDC( hwnd, hDC ); 
				EndPaint( hwnd, &ps ); 
			}
			break; 

		case WM_DROPFILES:
			{
			HDROP hDrop = (HANDLE )wparam; 
			char string[MAX_PATH]; 

				DragQueryFile( hDrop, 0, string, sizeof(string) ); 
				LoadTheBitmap( hwnd, string ); 
				DragFinish( hDrop ); 
				return FALSE; 
			}
			break; 

		case WM_DESTROY:
			PostQuitMessage(TRUE); 
			return 0; 
		}

		return DefWindowProc( hwnd, msg, wparam, lparam ); 
	}

static HWND CreateMainWindow( void )
  {
  WNDCLASS  wc;
  BOOL      rc;
  HWND      hwnd; 

    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC )MainWindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0; 
    wc.hInstance     = hInstance; 
    wc.hIcon         = LoadIcon( NULL, IDI_APPLICATION ); 
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH )(COLOR_3DFACE + 1); 
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1); 
    wc.lpszClassName = "MainClass"; 
		
    rc = RegisterClass( &wc );
    if ( !rc )
    {
      MessageBox ( NULL, "RegisterClass() failed", "Error", MB_OK );
      return ( NULL );
    }
		
    hwnd = CreateWindowEx(
        0, 
        "MainClass",
        "GFL SDK example",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
        NULL,
        NULL,
        hInstance,
        NULL
        );
    
    if ( !hwnd )
    {
      MessageBox ( NULL, "WindowMainCreate() failed", "Error", MB_OK ); 
      return ( NULL );
    }
    
		DragAcceptFiles( hwnd, TRUE ); 

    return hwnd; 
  }

int APIENTRY WinMain( HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow )
  {
	MSG msg; 
	HWND hwnd; 

#ifdef _DEBUG
    AllocConsole();
    freopen( "conin$", "r", stdin );
    freopen( "conout$", "w", stdout );
    freopen( "conout$", "w", stderr );
#endif

		gflSetPluginsPathname("..\\plugins\\"); 
		gflLibraryInit(); 
		gflEnableLZW( GFL_TRUE ); 

		hInstance = hCurrentInst; 
		hwnd = CreateMainWindow(); 
		ShowWindow( hwnd, SW_SHOWDEFAULT );
		
    while ( GetMessage( &msg, NULL, 0, 0 ) )
    {
			TranslateMessage( &msg ); 
			DispatchMessage( &msg ); 
		}
		
#ifdef __USE_HBITMAP__
		if ( ThehBitmap )
			DeleteObject( ThehBitmap ); 
#else
		if ( TheBitmap )
			gflFreeBitmap( TheBitmap ); 
#endif

		gflLibraryExit(); 

#ifdef _DEBUG
    FreeConsole(); 
#endif
    return msg.wParam; 
	}
