/***************************************************************
 * Name:      wxXisoApp.cpp
 * Purpose:   Code for Application Class
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

#include "wxXisoApp.h"
#include "wxXisoMain.h"

IMPLEMENT_APP(wxXisoApp);

bool wxXisoApp::OnInit()
{
    wxXisoFrame* frame = new wxXisoFrame(0L);

    frame->Show();

    return true;
}
