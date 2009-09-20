
#include "RessourceDialog.h"

BEGIN_EVENT_TABLE(RessourceDialog, wxDialog)
	EVT_COMMAND(wxID_ANY, SelectedRessourceEvent, RessourceDialog::OnSelected)
END_EVENT_TABLE()

wxString RelativeToPath(wxString filepath, wxString file)
{
	int pos = filepath.Last('/');
	int pos2 = filepath.Last('\\');
	if (pos2>pos)
		pos=pos2;

	wxString path = filepath.Left(pos2);
	if (file.Find(path)!=-1)
		file = file.Right(file.Length()-path.Length()-1);

	return file;
}

RessourceDialog::RessourceDialog(wxWindow *parent)
: wxDialog(parent,wxID_ANY,"Ressource Explorer",wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
	m_pPanel = new RessourcePanel(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,/*FLAG_RP_ZOOM_ANIMATION|*/FLAG_RP_SELECTOR);
	
	SetSize(800,600);
	Center();
}

RessourcePanel* RessourceDialog::GetRessourcePanel()
{
	return m_pPanel;
}

void RessourceDialog::OnSelected(wxCommandEvent& event)
{
	//m_aSelected
	m_aSelected.Clear();
	wxRessourceItemArray& list = m_pPanel->GetSelection();
	
	//char buffer[2048];
	//GetCurrentDirectory(2048,buffer);

	for (int i=0;i<list.Count();i++)
	{
		//printf("%s\n",list[i]->GetPath().c_str());
		//printf("%s\n",list[i]->GetFilename().c_str());
		m_aSelected.Add(wxString(list[i]->GetPath()+"\\"+list[i]->GetFilename()));
		//wxString relativePath = RelativeToPath(buffer,list[i]->GetPath()+"\\"+list[i]->GetFilename());
		//m_aSelected.Add(wxString(relativePath.c_str()));		
	}
	EndModal(1);
}

wxArrayString& RessourceDialog::GetSelected()
{
	return m_aSelected;
}
