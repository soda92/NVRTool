#include "FireMsgView.h"
#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"

namespace FireMsgView {

    void DialogCreated() {
        CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)theApp.m_pMainWnd;
        dlg->firemsgdlg_exited = false;
    }

    void DialogExited()
    {
        CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)theApp.m_pMainWnd;
        if (dlg != nullptr) {
            dlg->firemsgdlg_exited = true;
        }
    }

}
