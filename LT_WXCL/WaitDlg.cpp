// WaitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "WaitDlg.h"
#include "afxdialogex.h"

char Num = 30;
// CWaitDlg 对话框

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


// CWaitDlg 消息处理程序


BOOL CWaitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_brush.CreateSolidBrush(RGB(0, 0, 0));

	MoveWindow(0, 0, SCREEN_X, SCREEN_Y);
	CenterWindow();

	GetDlgItem(IDC_STATIC_TXT)->MoveWindow(SCREEN_X * 2 / 7, SCREEN_Y / 2, SCREEN_X * 3 / 5, SCREEN_Y / 3);
	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CWaitDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	char buf[200] = "";
	sprintf_s(buf, "系统数据加载中。。。 %dS", Num);
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

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)      //对话框颜色  
		return   m_brush;       // 返回画笔

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  
		hbr = (HBRUSH)m_brush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
