#include "FireMsgView.h"
#include "stdafx.h"
#include "FireVideo.h"
#include "FireVideoDlg.h"

namespace FireMsgView {

    void DialogCreated() {
        CFireVideoDlg* dlg = (CFireVideoDlg*)theApp.m_pMainWnd;
        dlg->firemsgdlg_exited = false;
    }

    void DialogExited()
    {
        CFireVideoDlg* dlg = (CFireVideoDlg*)theApp.m_pMainWnd;
        if (dlg != nullptr) {
            dlg->firemsgdlg_exited = true;
        }
    }

}
