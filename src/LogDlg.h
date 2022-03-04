#pragma once
#include "afxdialogex.h"
#include "ListCtrlCl.h"


// LogDlg 对话框

class LogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LogDlg)

public:
	LogDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LogDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CBrush m_brush;
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();

    afx_msg void OnSize(UINT nType, int cx, int cy);

    // 列表
    CListCtrlCl log_list;

    // 用于OnSize
    int num = 0;
};
