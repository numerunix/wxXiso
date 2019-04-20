#ifndef WXXISOOPTIONS_H
#define WXXISOOPTIONS_H

#include <wx/msgdlg.h>

#include "GUIFrame.h"
#include "wx/dirdlg.h"

class wxXisoOptions : public MyDialog1
{
    public:
        wxXisoOptions(wxWindow *parent, int id, wxString path, long numeroProcessi, bool aggiornamenti);
        virtual ~wxXisoOptions();
        int GetTerminal();
        long getNumeroProcessi();
        wxString GetPath();
		bool getAggiornamenti();
    private:
        int terminal;
        wxString path, numeroProcessiStr;
        long numeroProcessi;
        bool aggiornamenti;
  		virtual void OnSelect( wxCommandEvent& event ) ;
		virtual void OnOK( wxCommandEvent& event );

};

#endif // WXXISOOPTIONS_H
