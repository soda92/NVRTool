#include "stdafx.h"

#include "log.h"
#include "LogDlg.h"
#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"

#include "ManageView.h"

namespace LogView {
    void Update() {
        CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)theApp.m_pMainWnd;

        dlg->m_logDlg.log_list.LockWindowUpdate();
        dlg->m_logDlg.log_list.SetRedraw(false);
        dlg->m_logDlg.log_list.DeleteAllItems();
        dlg->m_logDlg.log_list.UnlockWindowUpdate();

        auto len = logn::logs.size();
        for (size_t i = 0; i < len; i++) {
            logn::log _log = logn::logs[i];
            int nItem;
            
            // translation
            if (_log.message == "system exit") {
                nItem = dlg->m_logDlg.log_list.InsertItem(0, _TEXT("系统退出"));
            }
            else if (_log.message == "system startup") {
                nItem = dlg->m_logDlg.log_list.InsertItem(0, _TEXT("系统启动"));
            }
            else if (_log.message.substr(0, 4) == "fire") {
                char AB = _log.message.at(4);
                char num = _log.message.at(5);
                CString str;
                str.Format(_TEXT("%c节探头%c报警"), AB, num);
                nItem = dlg->m_logDlg.log_list.InsertItem(0, str);
            }
            else if (_log.message.substr(0, 3) == "cam") {
                auto IPCNum_str = _log.message.substr(4, 2);
                auto IPCNum = std::stoi(IPCNum_str);
                auto message = IPCName[IPCNum];
                CString str;
                str.Format(_TEXT("IPC%d[%s]连接失败"), IPCNum+1, message);
                nItem = dlg->m_logDlg.log_list.InsertItem(0, str);
            }
            else {
                nItem = dlg->m_logDlg.log_list.InsertItem(0, _TEXT("未知事件"));
            }
            dlg->m_logDlg.log_list.SetItemText(nItem, 1, CString(_log.time.c_str()));
        }

        dlg->m_logDlg.log_list.SetRedraw(true);
    }
}
