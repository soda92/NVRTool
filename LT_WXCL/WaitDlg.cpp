// WaitDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "WaitDlg.h"
#include "afxdialogex.h"

char Num = 30;
// CWaitDlg �Ի���

IMPLEMENT_DYNAMIC(CWaitDlg, CDialogEx)

CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWaitDlg::IDD, pParent)
{

}

CWaitDlg::~CWaitDlg()
{
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWaitDlg ��Ϣ�������


BOOL CWaitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_brush.CreateSolidBrush(RGB(0, 0, 0));

	MoveWindow(0, 0, SCREEN_X, SCREEN_Y);
	CenterWindow();

	GetDlgItem(IDC_STATIC_TXT)->MoveWindow(SCREEN_X * 2 / 7, SCREEN_Y / 2, SCREEN_X * 3 / 5, SCREEN_Y / 3);
	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CWaitDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	char buf[200] = "";
	sprintf_s(buf, "ϵͳ���ݼ����С����� %dS", Num);
	SetDlgItemText(IDC_STATIC_TXT, buf);
	if (--Num < 0)
	{
		KillTimer(1);
		OnOK();
	}
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CWaitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_DLG)      //�Ի�����ɫ  
		return   m_brush;       // ���ػ���

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
		hbr = (HBRUSH)m_brush;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
