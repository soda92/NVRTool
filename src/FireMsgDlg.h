#pragma once
#include "afxcmn.h"
#include "MyButton.h"
#include "afxwin.h"
#include "ListCtrlCl.h"
#include "resource.h"

// CFireMsgDlg �Ի���

class CFireMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFireMsgDlg)

public:
	CFireMsgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFireMsgDlg();

	int InitList();
	int FireDec(unsigned char* buf, int len, char pos);//pos 0-��A 1-��B 2-��A 3-��B 

	int SendFireMsg();
	int StopWarFun();

	int FireDataAnalyse(unsigned char* buf, int len, int Train, BOOL SendFlag = FALSE);

	// �Ի�������
	enum { IDD = IDD_DIALOG_FIRE };
	int num;

	//�������ݽ��ռ�ʱ
	int lastDataTime[4];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlCl m_FireListB;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBrush m_brush;

public:
	afx_msg void OnPaint();

	CString f_fire_tex[4];
};
