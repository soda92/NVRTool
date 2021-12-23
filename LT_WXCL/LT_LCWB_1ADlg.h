
// LT_LCWB_1ADlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "VideoDlg.h"
#include "ManageDlg.h"
#include "FireMsgDlg.h"

#include "MyButton.h"
#include "MyTabCtrl.h"
#include "LogDlg.h"

#include "TaxData.h"



// CLT_WXCLDlg 对话框
class CLT_LCWB_1ADlg : public CDialogEx
{
	// 构造
public:
	CLT_LCWB_1ADlg(CWnd* pParent = NULL);	// 标准构造函数

	CVideoDlg m_VideoDlg;
	CManageDlg m_ManageDlg;
	CFireMsgDlg m_FireMsgDlg;
    LogDlg m_logDlg;


	unsigned char OsdIndex;

	LONG lUserID[32];
	LONG lRealPlayHandle[32];
    char ip[200][32];

	CBrush m_brush;
	CFont newFont;



	int VideoOSDSet(long* pUid, char* Speed, char* Mileage, char* CheCi,
        char* CheHao, char pos, char* SiJiHao);
	int TimeCFG();

	TAXDATA TaxData;
	BOOL TaxStat;

	//////////////////////////////////////////////////////////////////////////
	int BROADCASTInit();
	SOCKET BRecUdp;
	int FireUdpInit();

	// 对话框数据
	enum { IDD = IDD_LT_WXCL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyTabCtrl m_TabCtrl;
	CMyButton stop_warn;

	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg void OnBnClickedStopWarn();
    virtual void OnCancel();
    virtual void OnOK();

    // 判断FireMsgDlg是否已经退出
    bool firemsgdlg_exited = false;
};
