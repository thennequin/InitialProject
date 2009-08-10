

#ifndef _FOLDERSCANTHREAD_HEADER_
#define _FOLDERSCANTHREAD_HEADER_

#include <wx/wx.h>

class FolderScanThread : public wxThread
{
public:
	 FolderScanThread(wxString Folder);
	 ~FolderScanThread();

	 bool Start();
	 virtual void *Entry();

	 static FolderScanThread *GetInstance() { return m_pInstance; }
	 static void IncrementConvertCount();
	 static void DecrementConvertCount();

	 unsigned int GetQueryCount() { return m_iFilesNumber; }
protected:
	void ScanDirectory(wxString& dir);
protected:
	wxString m_sFolder;
	wxArrayString m_sExts;

	unsigned int m_iFilesNumber;

	static FolderScanThread *m_pInstance;
	wxSemaphore m_sConvert;
	int m_iConvertCount;
};

#endif
