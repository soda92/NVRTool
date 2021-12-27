// DownloadDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "afxdialogex.h"
#include "DownloadDialog.h"

#include <filesystem>


// DownloadDialog 对话框

IMPLEMENT_DYNAMIC(DownloadDialog, CDialogEx)

DownloadDialog::DownloadDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DOWNLOAD, pParent)
{

}

DownloadDialog::~DownloadDialog()
{
}

void DownloadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DOWNLOAD_TITLE, m_title);
	DDX_Control(pDX, IDC_STATIC_DOWNLOAD_DETAIL, m_detail);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_progress);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD_CLOSE, m_close);
}


BEGIN_MESSAGE_MAP(DownloadDialog, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_CLOSE, &DownloadDialog::OnBnClickedButtonDownloadClose)
END_MESSAGE_MAP()


// DownloadDialog 消息处理程序


BOOL DownloadDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_brush.CreateSolidBrush(RGB(0, 0, 0));
    m_close.SetBkColor(RGB(0, 0, 0));
    m_close.SetForeColor(RGB(255, 255, 255));
    m_font_title.CreatePointFont(200, "宋体");
    m_font_detail.CreatePointFont(150, "Arial");
    m_font_close.CreatePointFont(170, "宋体");

    m_title.SetFont(&m_font_title);
    m_detail.SetFont(&m_font_detail);
    m_close.SetFont(&m_font_close);

    SetWindowPos(&this->wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    COLORREF clrBar = RGB(255,255,255); // the bar color
    m_progress.SendMessage(PBM_SETBARCOLOR, 0, (LPARAM)clrBar);
    COLORREF clrBg = RGB(0, 0, 0); // the background color
    m_progress.SendMessage(PBM_SETBKCOLOR, 0, (LPARAM)clrBg);

    m_close.ShowWindow(SW_HIDE);

    m_progress.SetRange(0, paths.size() - 1);
    int i = 0;
    for (const auto& [src, dest] : paths) {
        CString text{ src.c_str() };
        m_detail.SetWindowText(text);
        std::filesystem::copy(
            std::filesystem::path(src),
            std::filesystem::path(dest)
        );
        i += 1;
        m_progress.SetPos(i);
    }

    m_close.ShowWindow(SW_SHOW);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH DownloadDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_DLG) {
        return m_brush;
    }

    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);
        hbr = (HBRUSH)m_brush;
    }

    return hbr;
}


void DownloadDialog::OnBnClickedButtonDownloadClose()
{
    CDialogEx::OnOK();
}


void DownloadDialog::OnOK()
{
    //CDialogEx::OnOK();
}


void DownloadDialog::OnCancel()
{
    //CDialogEx::OnCancel();
}
