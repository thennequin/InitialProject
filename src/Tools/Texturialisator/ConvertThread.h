

#ifndef _CONVERTTHREAD_HEADER_
#define _CONVERTTHREAD_HEADER_

#include <wx/wx.h>

struct ConvertQuery
{
	unsigned int Id;
	wxString File;
};

WX_DEFINE_ARRAY(ConvertQuery*,wxConvertQueryArray);

class ConvertThread;
WX_DEFINE_ARRAY(ConvertThread*,wxConvertThreadArray);

class ConvertThread : public wxThread
{
public:
	 ConvertThread(int ThreadId, bool cuda, int quality, bool force);
	~ConvertThread();

	 void Start();
	 virtual void *Entry();
public:
	static void Init();
	static void AddQuery(wxString file,bool access=true);
	static void ClearQuery();
	static unsigned int GetQueryCount();

	static void LockQueryAcces();
	static void UnLockQueryAcces();
	static wxConvertThreadArray& GetInstances() { return m_pInstances; }
protected:
	int m_iThreadId;

	static wxSemaphore m_sQueryAcces;
	static bool m_bQueryInit;

	static wxConvertQueryArray m_aQuery;
	static unsigned int m_iNextQueryId;
	bool m_bCUDA;
	int m_iQuality;
	bool m_bForce;

	static wxConvertThreadArray m_pInstances;
};

#endif