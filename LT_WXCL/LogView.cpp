#include "stdafx.h"

#include "log.h"
#include "LogDlg.h"

namespace LogView {
    void Update(void* p_logDlg) {
        LogDlg* m_logDlg = (LogDlg*)p_logDlg;
        auto len = logn::logs.size();
        for (size_t i = 0; i < len; i++) {
            logn::log _log = logn::logs[i];
            int nItem;
            // translation
            if (_log.message == "system exit") {
                nItem = m_logDlg->log_list.InsertItem(0, _TEXT("系统退出"));
            }
            else if (_log.message == "system startup") {
                nItem = m_logDlg->log_list.InsertItem(0, _TEXT("系统启动"));
            }
            else {
                nItem = m_logDlg->log_list.InsertItem(0, _TEXT("未知事件"));
            }
            m_logDlg->log_list.SetItemText(nItem, 1, CString(_log.time.c_str()));
        }
    }
}
