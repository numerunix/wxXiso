/***************************************************************
 * Name:      wxXisoMain.h
 * Purpose:   Defines Application Frame
 * Author:    Giulio Sorrentino (gsorre84@gmail.com)
 * Created:   2015-11-16
 * Copyright: Giulio Sorrentino ()
 * License:   GPL V3
 **************************************************************/

#ifndef WXXISOMAIN_H
#define WXXISOMAIN_H



#include "wxXisoApp.h"
#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/config.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/msgdlg.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>
#include <stdexcept>
#include "GUIFrame.h"
#include "wxXisoOptions.h"

class wxXisoFrame: public GUIFrame
{
    public:
        wxXisoFrame(wxFrame *frame);
        ~wxXisoFrame();
    private:
        bool fileSelezionato, dirSelezionata, aggiornamenti;
        int terminal;
        long numeroProcessi;
        wxString path,
                 versione,
                 paginaWeb;

        wxConfig *config;
        wxHTTP client;
        virtual void OnClose(wxCloseEvent& event);
  		virtual void OnOpzioni( wxCommandEvent& event );
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnSelezionaIso(wxCommandEvent &evt);
        virtual void OnSelezionaDir(wxCommandEvent &evt);
        virtual void OnOk(wxCommandEvent & evt);
 //       virtual void onAggiornamenti(wxCommandEvent& evt);
        virtual void onSitoWeb(wxCommandEvent& evt);
      //  bool Aggiornamenti(wxString &nuovaVersione) throw (std::domain_error);
};

#endif // WXXISOMAIN_H
