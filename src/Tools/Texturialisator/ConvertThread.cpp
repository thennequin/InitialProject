
#include "ConvertThread.h"
#include "FolderScanThread.h"

#include "TexturialisatorApp.h"

#include "wx/filename.h"

#include "Initial/Format/IImageITX.h"

using namespace Initial;
using namespace Initial::Format;

wxSemaphore ConvertThread::m_sQueryAcces;
bool ConvertThread::m_bQueryInit=false;
wxConvertQueryArray ConvertThread::m_aQuery;
unsigned int ConvertThread::m_iNextQueryId=1;
wxConvertThreadArray ConvertThread::m_pInstances;

class MyITXOutput : public IImageITX::ITXOutput
{
public:
	virtual void Progress(int current)
	{
		//wxGetApp().GetWindow()->GetGauge()->SetValue(current/Size);
		wxGetApp().GetWindow()->GetGraph()->SetEvent(BuildId,Size,current);
	}

	virtual void SetTotal(int totalSize)
	{
		Size=totalSize;
	}
	long Size;
	int BuildId;
};

ConvertThread::ConvertThread(int ThreadId, bool cuda, int quality, bool force)
{
	m_iThreadId=ThreadId;
	m_bCUDA=cuda;
	m_iQuality=quality;
	m_bForce=force;
	Init();
	m_pInstances.Add(this);
	
	FolderScanThread::IncrementConvertCount();
}

ConvertThread::~ConvertThread()
{
	m_pInstances.Remove(this);
	
	FolderScanThread::DecrementConvertCount();
}

void ConvertThread::Init()
{
	if (m_bQueryInit==false)
	{
		m_bQueryInit=true;
		m_sQueryAcces.Post();
	}
}

void ConvertThread::Start()
{
	if (Create() != wxTHREAD_NO_ERROR )
	{
		wxLogError(wxT("Erreur lors de la création du thread n°%d"),m_iThreadId);
	}else{
		SetPriority(25);
		Run();
	}
}

void* ConvertThread::Entry()
{
	Init();
	m_sQueryAcces.Wait();
	while (m_aQuery.Count()>0)
	{
			ConvertQuery *query=m_aQuery[0];
			m_aQuery.RemoveAt(0);
		m_sQueryAcces.Post();

		if (query)
		{
			//Convert

			MyITXOutput myoutput;
			IImageITX itxFile;
			wxFileName file(query->File);
			int OK = itxFile.LoadInfo(query->File.c_str());
			//wxGetApp().GetWindow()->GetGraph()->AddEvent(query->Id,GetTickCount(),m_iThreadId-1,itxFile.GetName().c_str());
			wxGetApp().GetWindow()->GetGraph()->AddEvent(query->Id,GetTickCount(),m_iThreadId-1,file.GetFullName());
			if (OK)
			{
				myoutput.BuildId=query->Id;
				OK = IImageITX::UpdateITX(query->File.c_str(),&myoutput,(IImageITX::OutputQuality)(IImageITX::OQ_FAST+m_iQuality),m_bCUDA,m_bForce);
				//OK = UpdateITX(query->File,m_bCUDA,m_iQuality,query->Id,m_bForce);
				if (OK)
				{
					//Sleep(50);
				}
			}
			
			if (OK==0)
				wxGetApp().GetWindow()->GetGraph()->AddEvent(query->Id,GetTickCount(),1); // OK
			else if (OK==1)
				wxGetApp().GetWindow()->GetGraph()->AddEvent(query->Id,GetTickCount(),2); // No Need Update
			else
				wxGetApp().GetWindow()->GetGraph()->AddEvent(query->Id,GetTickCount(),3); // Error
			
			
			if (FolderScanThread::GetInstance())
			{
				int FilesOk = FolderScanThread::GetInstance()->GetQueryCount()-GetQueryCount();
				wxString text;
				text.Printf("%d / %d",FilesOk,FolderScanThread::GetInstance()->GetQueryCount());
				wxGetApp().GetWindow()->GetStatusBar()->SetStatusText(text);
				wxString detail;
				if (OK==0)
				{
					wxString format;
					wxString type;
					if (itxFile.GetInputImageType()==IImageITX::IT_NORMAL)
						type="Normal map";
					else
						type="Color";
					switch (itxFile.GetFormat())
					{
					case IImageITX::ITXF_RGBA:
						format="RGBA";
						break;
					case IImageITX::ITXF_DXT1:
						format="DXT1";
						break;
					case IImageITX::ITXF_DXT1a:
						format="DXT1a";
						break;
					case IImageITX::ITXF_DXT1nm:
						format="DXT1nm";
						break;
					case IImageITX::ITXF_DXT3:
						format="DXT3";
						break;
					case IImageITX::ITXF_DXT5:
						format="DXT5";
						break;
					case IImageITX::ITXF_DXT5nm:
						format="DXT5nm";
						break;
					case IImageITX::ITXF_ATI1:
						format="ATI1";
						break;
					case IImageITX::ITXF_ATI2_3Dc:
						format="ATI2/3Dc";
						break;
					};
					detail.Printf("\n\tOriginal file : %s\n\tWidth : %d\n\tHeight : %d\n\tFormat : %s\n\tType : %s",itxFile.GetOrigFilename().c_str(),itxFile.GetWidth(),itxFile.GetHeight(),format,type);
				}else if (OK==1)
					detail="\n\tNot need update";
				else
					detail="\n\tError";
				wxGetApp().GetWindow()->AddDetailText(query->File+detail+"\n");
			}
		}
		m_sQueryAcces.Wait();
	}
	m_sQueryAcces.Post();

	return NULL;
}

void ConvertThread::AddQuery(wxString file,bool access)
{
	Init();
	if (access)
	m_sQueryAcces.Wait();
		ConvertQuery *query = new ConvertQuery;
		query->Id=m_iNextQueryId++;
		query->File=file;
		m_aQuery.Add(query);
	if (access)
	m_sQueryAcces.Post();
}

void ConvertThread::ClearQuery()
{
	Init();
	m_sQueryAcces.Wait();
		for (unsigned int i=0;i<m_aQuery.Count();i++)
		{
			delete m_aQuery[i];
		}
		m_aQuery.Clear();
	m_sQueryAcces.Post();
}

unsigned int ConvertThread::GetQueryCount()
{
	//m_sQueryAcces.Wait();
	unsigned int count=(unsigned int)m_aQuery.Count();
	//m_sQueryAcces.Post();
	return count;
}

void ConvertThread::LockQueryAcces()
{
	Init();
	m_sQueryAcces.Wait();
}

void ConvertThread::UnLockQueryAcces()
{
	Init();
	m_sQueryAcces.Post();
}
