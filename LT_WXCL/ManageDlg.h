#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrlCl.h"

// CManageDlg 对话框

class CManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManageDlg)

public:
	CManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CManageDlg();

	int InitList();
	int SetList();

	int m_RecordCallBack(LONG nPort,char* error);

	int SetIPCState();
	int SetHDDState();
	int FindAndDeleteRecord(CString Path);
	CString FindDir(char* HddPath);
	BOOL IsHDD(char* Path);


// 对话框数据
	enum { IDD = IDD_DIALOG_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int num;
	CListCtrlCl m_IPCStateList;
	CListCtrlCl m_HDDStateList;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBrush m_brush;
	CFont newFont1;
public:
	CStatic m_static_device;
	CStatic m_static_hard;

	char RecordFlag[20];//录像保存标志
};
