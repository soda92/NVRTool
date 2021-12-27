#pragma once
#include "afxdialogex.h"
#include "MyButton.h"

#include <vector>
#include <array>
#include <string>


// DownloadDialog 对话框

class DownloadDialog : public CDialogEx
{
	DECLARE_DYNAMIC(DownloadDialog)

public:
	DownloadDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DownloadDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DOWNLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CStatic m_title;
    CStatic m_detail;
    CProgressCtrl m_progress;
    CMyButton m_close;
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
    CBrush m_brush;
    CFont m_font_title;
    CFont m_font_detail;
    CFont m_font_close;
public:
    afx_msg void OnBnClickedButtonDownloadClose();
    virtual void OnOK();
    virtual void OnCancel();

    std::vector<std::array<std::string, 2>> paths;
};
