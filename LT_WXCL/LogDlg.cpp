// LogDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Logdlg.h"
#include "resource.h"
#include "log.h"
#include "LogView.h"


// LogDlg 对话框

IMPLEMENT_DYNAMIC(LogDlg, CDialogEx)

LogDlg::LogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOG, pParent)
{

}

LogDlg::~LogDlg()
{
}

void LogDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_LOG, log_list);
}


BEGIN_MESSAGE_MAP(LogDlg, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// LogDlg 消息处理程序


HBRUSH LogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

    HBRUSH hBrush = NULL;
    switch (nCtlColor)
    {
    case CTLCOLOR_DLG:
        // just return a not NULL brush handle
        hBrush = (HBRUSH)m_brush;
        break;
    case CTLCOLOR_STATIC:
    {
        // set text color, transparent back node then 
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);
        // return a not NULL brush handle
        hBrush = (HBRUSH)m_brush;
    }
    break;
    default:
        // do the default processing
        hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }
    return hBrush;
 
}


BOOL LogDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化


    // 黑色笔刷，用于填充背景
    this->m_brush.CreateSolidBrush(RGB(0, 0, 0));

    this->log_list.SetBkColor(RGB(0, 0, 0));
    this->log_list.SetTextColor(RGB(255, 255, 255));

    this->log_list.SetHeaderHeight(2);
    // 行高
    this->log_list.SetRowHeight(38);

    DWORD dwStyle = this->log_list.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    this->log_list.SetExtendedStyle(dwStyle); //设置扩展风格

    int size = 25;
    this->log_list.SetFontHW(size, static_cast<int>(size * 0.5));
    this->log_list.SetHeaderFontHW(size, static_cast<int>(size * 0.5));

    this->log_list.InsertColumn(0, "事件", LVCFMT_CENTER, 278);
    this->log_list.InsertColumn(1, "时间", LVCFMT_CENTER, 277);

#ifdef DEBUG
  /*  logn::load();
    logn::log x = { "11",1,"2" };
    logn::addlog(x);
    logn::save();*/
#endif // DEBUG

    // 初始化日志记录
    //logn::update();
    logn::load();
    LogView::Update(static_cast<void*>(this));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void LogDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码

    int x = 100;
    int y = 65;
    int width = 562;
    int height = 421;

    if (num++ > 0) {
        this->log_list.MoveWindow(x,y,width, height);
    }
   
}
