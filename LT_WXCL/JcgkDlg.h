#pragma once
#include "afxcmn.h"

#include "MyMeter.h"
#include "MyButton.h"
#include "afxwin.h"
#include <cstdlib>

// CJcgkDlg 对话框

class CJcgkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJcgkDlg)

public:
	CJcgkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJcgkDlg();
	CStatic lable[8][5];
	void ProCtrlInit();
	void SetProValue(char num, int value);

	CButton button[13];
	void SetButton();

	void SetButtonWar(char num, BOOL WFlag);

	void FlushAll();
	float CH[11][2];
	CString itemName[8];
	CString itemUnit[8];

	char CurrentPage;//

	CBrush m_brush;

	char MeterShowFlag;//0-显示柱状图 1-显示压力表
	//////////////////////////////////////////////////////////////////////////

	HANDLE CPort;
	int CJMKComInit(char* com);
	int CJMK_SendMsg(char Flag);//1-状态 2-表
	int CJMK_DecFun(unsigned int data, float VMax, float Max);
	int CJMK_MsgDec(unsigned char* buf, int len, int Train, char Flag);
	char buttonFlagTemp[13];

	void ShowZhuZT(bool flag);
	//////////////////////////////////////////////////////////////////////////

	MyMeter* m_Meter[4];
	int MeterInit();
	unsigned int MeterValue[4];
	int CJMKUdpInit();
	SOCKET CJMKRecUdpSoc;
	int CJMK_MeterSet(int No, unsigned int num);

	//////////////////////////////////////////////////////////////////////////
// 对话框数据
	enum { IDD = IDD_DIALOG_JCGK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_pro1;
	CProgressCtrl m_pro2;
	CProgressCtrl m_pro3;
	CProgressCtrl m_pro4;
	CProgressCtrl m_pro5;
	CProgressCtrl m_pro6;
	CProgressCtrl m_pro7;
	CProgressCtrl m_pro8;
	afx_msg void OnBnClickedBca();
	afx_msg void OnBnClickedBcb();
	afx_msg void OnBnClickedTca();
	afx_msg void OnBnClickedTcb();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	int lastDataTime[8];

	CFont newFont, newFont1;

	CMyButton m_button[5];

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_static_Train;
};
