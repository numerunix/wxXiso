/***************************************************************
 * Name:      wxXisoMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Giulio Sorrentino (gsorre84@gmail.com)
 * Created:   2015-11-16
 * Copyright: Giulio Sorrentino ()
 * License:   GPL V3
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "wxXisoMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


wxXisoFrame::wxXisoFrame(wxFrame *frame)
    : GUIFrame(frame)
{
    versione=wxT("0.4");
    paginaWeb=wxT("http://numerone.altervista.org/blog/2013/06/wxxiso/");
    fileSelezionato=false;
    dirSelezionata=false;
    config=new wxConfig(wxT("wxXiso"));
    config->Read(wxT("terminal"), &terminal, 1);
    config->Read(wxT("path"), &path, wxT("./"));
    config->Read(wxT("processi"), &numeroProcessi, 1);
    if (!config->Read(wxT("aggiornamentiAutomatici"), &aggiornamenti))
        aggiornamenti=false;
  	client.SetHeader(_T("Content-type"), _T("text/html; charset=utf-8"));
    client.SetTimeout(10);
    if (aggiornamenti) {
        wxString s;
        try {
            if (Aggiornamenti(s)) {
                wxMessageBox(wxT("Sono presenti aggiornamenti, il programma aprira' la pagina per il download."), wxT("Informazioni"), wxICON_INFORMATION);
                wxLaunchDefaultBrowser(paginaWeb);
            }
        } catch (std::domain_error e) {
            wxMessageBox(wxString(e.what()), wxT("Errore"), wxOK|wxICON_ERROR);
        }
    }
}

wxXisoFrame::~wxXisoFrame()
{
    config->Write(wxT("terminal"), terminal);
    config->Write(wxT("path"), path);
    config->Write(wxT("processi"), numeroProcessi);
    config->Write(wxT("aggiornamentiAutomatici"), aggiornamenti);
    delete config;
}

void wxXisoFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void wxXisoFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void wxXisoFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxAboutDialogInfo info;
    info.SetName(_("wxXiso"));
    info.SetVersion(versione);
    info.SetDescription(_("Batch GUI per il programma extract-xiso. Permette di scompattare più iso per volta"));
    info.SetCopyright(_T("(C) 2015 Giulio Sorrentino <gsorre84@gmail.com>"));
    info.SetLicence(_T("GPL v3 o, a tua discrezione, qualsiasi versione successiva."));
    info.SetWebSite(_T("http://numerone.altervista.org"));
    wxAboutBox(info);
}


void wxXisoFrame::OnSelezionaIso(wxCommandEvent &evt) {
    wxDirDialog *dirDialog=new wxDirDialog(this, _("Seleziona una directory"));
    if (dirDialog->ShowModal()==wxID_OK) {
        m_staticText2->SetLabel(dirDialog->GetPath());
    }
    fileSelezionato=true;
    m_button3->Enable(fileSelezionato && dirSelezionata);
}

void wxXisoFrame::OnSelezionaDir(wxCommandEvent &evt) {
    wxDirDialog *dirDialog=new wxDirDialog(this, _("Seleziona una directory"));
    if (dirDialog->ShowModal()==wxID_OK) {
        m_staticText4->SetLabel(dirDialog->GetPath());
    }
    dirSelezionata=true;
    m_button3->Enable(fileSelezionato && dirSelezionata);
}

void wxXisoFrame::OnOk(wxCommandEvent & evt) {
    wxDir dir(m_staticText2->GetLabel());
    wxString f, d, console, errore=wxEmptyString;
    wxArrayString as;
    bool cont;
    size_t i;
    if (!dir.IsOpened()) {
        wxMessageBox(wxT("Directory non aperta"), wxT("Errore"), wxICON_ERROR);
        return;
    }
    cont=dir.GetFirst(&f, wxT("*.iso"));
    if (!cont) {
        wxMessageBox(wxT("La directory non contiene files .iso. Selezionare la directory esatta."), wxT("Attenzione"), wxICON_INFORMATION);
        return;
    }
    if (!wxFileExists(path+wxT("extract-xiso"))) {
        wxMessageBox(wxT("File extract-xiso non trovato. Indicare la path nelle opzioni"), wxT("Errore"), wxICON_ERROR);
        return;
    }
    while (cont) {
        d=m_staticText4->GetLabel()+wxFileName::GetPathSeparator()+f.Left(f.Len()-4);
        if (wxDirExists(d)) {
            errore=errore+f+wxT("\n");
            cont=dir.GetNext(&f);
            continue;
        }
        switch(terminal) {
            case 0: console=wxT("xterm -e"); break;
            case 1: console=wxT("konsole --noclose -e"); break;
        }
        as.Add(console+ wxT(" \"")+path+wxT("extract-xiso\" -x -s -d \"")+d+wxT("\" \"")+m_staticText2->GetLabel()+wxFileName::GetPathSeparator()+f+wxT("\" "));
        cont=dir.GetNext(&f);
    }
    wxString a;
    if (errore != wxEmptyString)
        wxMessageBox(wxT("Ignoro i file ")+errore+wxT("perché le relative cartelle esistono già."), wxT("Attenzione"), wxICON_ASTERISK);
    for (i=0; i<as.GetCount(); i++) {
        SetStatusText(wxT("Estrazione dell'iso ")
                          +wxString::Format(wxT("%zd"),i+1)
                          +wxT(" di ")
                          +wxString::Format(wxT("%zd"),as.GetCount()));
        wxExecute(as.Item(i), wxEXEC_ASYNC);
        if (((i+1) % numeroProcessi==0 || numeroProcessi==1) && i<as.GetCount()-1)
            wxMessageBox(wxT("Messaggio di blocco, premere ok per continuare."), wxT("Informazione"), wxICON_INFORMATION);
    }
    SetStatusText(wxT("Operazione completata"));
}

void wxXisoFrame::OnOpzioni( wxCommandEvent& event ) {
    wxXisoOptions *d = new wxXisoOptions(this, terminal, path, numeroProcessi, aggiornamenti);
    d->ShowModal();
    if (d->getOk()) {
        terminal=d->GetTerminal();
        path=d->GetPath();
        numeroProcessi=d->getNumeroProcessi();
        aggiornamenti=d->getAggiornamenti();
    }
}

bool wxXisoFrame::Aggiornamenti(wxString &nuovaVersione) throw (std::domain_error) {
    bool aggiornamenti=false;
    if (client.Connect(wxT("numerone.altervista.org"))) {
        if (client.GetError() == wxPROTO_NOERR)
        {
            wxInputStream *stream = client.GetInputStream(_T("/wxxiso.html"));
            if (stream==wxNullPtr)
                throw std::domain_error("Si e' verificato un errore nel recuperare le informazioni. Riprovare piu' tardi");
            else {
                wxStringOutputStream out_stream(&nuovaVersione);
                stream->Read(out_stream);
                nuovaVersione=nuovaVersione.RemoveLast();
                aggiornamenti=!nuovaVersione.IsSameAs(versione);
                delete stream;
            }
        }
        client.Close();
    } else {
        wxMessageBox(_("Impossibile connettersi. Riprovare piu' tardi."), _("Errore"), wxICON_ERROR);
    }
    return aggiornamenti;
}

void wxXisoFrame::onAggiornamenti(wxCommandEvent& evt) {
    wxString versioneOttenuta;
    try {
        if (!Aggiornamenti(versioneOttenuta))
            wxMessageBox(wxT("Stai usando l'ultima versione di wxXiso"), wxT("Complimenti"), wxICON_INFORMATION);
        else if (wxMessageBox(wxT("La versione che stai usando e' la ")+versione+wxT(". Sul sito e' presente la ")+versioneOttenuta+wxT(".\nVuoi aprire il browser per scaricarlo ora?"), wxT("Aggiornamento disponibile"), wxICON_INFORMATION | wxYES_NO)==wxYES)
                wxLaunchDefaultBrowser(paginaWeb);
    } catch (std::domain_error e) {
        wxMessageBox(wxString(e.what()), wxT("Errore"), wxOK|wxICON_ERROR);
    }
}

void wxXisoFrame::onSitoWeb(wxCommandEvent &evt) {
    wxLaunchDefaultBrowser(wxT("http://numerone.altervista.org"));
}
