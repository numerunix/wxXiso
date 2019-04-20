#include "wxXisoOptions.h"

wxXisoOptions::wxXisoOptions(wxWindow *parent, int id, wxString path, long numeroProcessi, bool aggiornamenti) : MyDialog1(parent)
{
    terminal=id;
    m_choice1->SetSelection(terminal);
    this->path=path;
    m_textCtrl1->SetValue(this->path);
    numeroProcessiStr<<numeroProcessi;
    m_textCtrl3->SetValue(numeroProcessiStr);
    m_checkBox1->SetValue(aggiornamenti);
    //ctor
}

wxXisoOptions::~wxXisoOptions()
{
    //dtor
}

void wxXisoOptions::OnSelect( wxCommandEvent& event ) {
   wxDirDialog *dirDialog=new wxDirDialog(this, _("Seleziona una directory"));
    if (dirDialog->ShowModal()==wxID_OK) {
        m_textCtrl1->SetValue(dirDialog->GetPath());
        m_textCtrl3->SetValue(numeroProcessiStr);
    }
}

int wxXisoOptions::GetTerminal() {
    return terminal;
}

wxString wxXisoOptions::GetPath() {
    return path;
}

void wxXisoOptions::OnOK( wxCommandEvent& event ) {
    terminal=m_choice1->GetSelection();
    aggiornamenti=m_checkBox1->GetValue();
    numeroProcessiStr=m_textCtrl3->GetValue();
    if (!numeroProcessiStr.ToLong(&numeroProcessi))
        wxMessageBox(wxT("Il valore inserito come numero di processi non è un numero intero."), wxT("Errore"), wxICON_ERROR);
    else if (numeroProcessi < 1 || numeroProcessi> 10)
        wxMessageBox(wxT("Il valore inserito come numero di processi non è tra 1 e 10"), wxT("Errore"), wxICON_ERROR);
    else {
        ok=true;
        Close();
    }
}


long wxXisoOptions::getNumeroProcessi() {
    return numeroProcessi;
}

bool wxXisoOptions::getAggiornamenti() {
    return aggiornamenti;
}
