
#include "FolderScanThread.h"

#include "ConvertThread.h"

#include "TexturialisatorApp.h"

#include "wx/filesys.h"

//#include "ITXFormat.h"

FolderScanThread *FolderScanThread::m_pInstance=NULL;

FolderScanThread::FolderScanThread(wxString Folder)
{
	m_sFolder=Folder;
	m_pInstance=this;
	m_iConvertCount=0;
}

FolderScanThread::~FolderScanThread()
{
	m_pInstance=NULL;
}

bool FolderScanThread::Start()
{
	if (Create() != wxTHREAD_NO_ERROR )
	{
		wxLogError(wxT("Erreur lors de la création du thread"));
		return false;
	}else
		Run();
	return true;
}

void* FolderScanThread::Entry()
{
	unsigned int StartTime=GetTickCount();
	ConvertThread::LockQueryAcces();
		wxGetApp().GetWindow()->GetGraph()->AddEvent(0,GetTickCount(),0,"Scan folder");

		//_sleep(500);

		ScanDirectory(m_sFolder);
		m_iFilesNumber=ConvertThread::GetQueryCount();

		wxGetApp().GetWindow()->GetGraph()->AddEvent(0,GetTickCount(),0);
	ConvertThread::UnLockQueryAcces();

	wxString text;
	text.Printf("0 / %d",m_iFilesNumber);
	wxGetApp().GetWindow()->GetStatusBar()->SetStatusText(text);

	m_sConvert.Wait();
	while(m_iConvertCount>0)
		m_sConvert.Wait();
	
	text.Printf("%d file converted in %.2f seconds",m_iFilesNumber,(GetTickCount()-StartTime)/1000.0);
	wxGetApp().GetWindow()->GetStatusBar()->SetStatusText(text);
	wxGetApp().GetWindow()->OnConvertFinish();
	wxLogMessage("Finish");

	return NULL;
}

void FolderScanThread::ScanDirectory(wxString& dir)
{
	wxFileSystem fs;
	fs.ChangePathTo(dir);
	wxString fileStr;
	wxString temp;
	wxArrayString Folders;

	fileStr = fs.FindFirst("*",wxFILE);
	while(fileStr!="")
	{
		//printf("%s\n",fileStr.c_str());
		wxFileName file(fileStr);
		
		if (file.GetExt().Lower()=="itx")
		{
			//if (ITXNeedToBeUpdated(fileStr)==1)
				ConvertThread::AddQuery(fileStr,false);
		}
			
		fileStr=fs.FindNext();
	}

	fileStr = fs.FindFirst("*",wxDIR);
	while(fileStr!="")
	{
		fileStr=fileStr+"\\";
		Folders.Add(fileStr);
		fileStr=fs.FindNext();
	}

	for (;Folders.Count()>0;)
	{
		//printf("%s\n",Folders[0].c_str());
		ScanDirectory(Folders[0]);
		Folders.RemoveAt(0);
	}
}

void FolderScanThread::IncrementConvertCount()
{
	//GetInstance()->m_sConvert.Wait();
	if (GetInstance())
	{
		GetInstance()->m_iConvertCount++;
		GetInstance()->m_sConvert.Post();
	}
}

void FolderScanThread::DecrementConvertCount()
{
	//GetInstance()->m_sConvert.Wait();
	if (GetInstance())
	{
		GetInstance()->m_iConvertCount--;
		GetInstance()->m_sConvert.Post();
	}
}
