// VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "VideoDlg.h"
#include "afxdialogex.h"


// CVideoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
{
	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i] = new CVideoPlayWnd;//实例化12个播放窗口
	}

	FullScreenFlag = FALSE;
	CurrentBox = 0;
	CurrentPage = 0;
}

CVideoDlg::~CVideoDlg()
{
	for (int i = 0; i < 32; i++)
	{
		delete m_videoPlayWnd[i];
		m_videoPlayWnd[i] = NULL;
	}
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_BA, m_button[0]);
	DDX_Control(pDX, IDC_BUTTON_BB, m_button[1]);
	DDX_Control(pDX, IDC_BUTTON_TA, m_button[2]);
	DDX_Control(pDX, IDC_BUTTON_TB, m_button[3]);
	DDX_Control(pDX, IDC_BUTTON_TURN, m_BTurn);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BA, &CVideoDlg::OnBnClickedButtonBa)
	ON_BN_CLICKED(IDC_BUTTON_BB, &CVideoDlg::OnBnClickedButtonBb)
	ON_BN_CLICKED(IDC_BUTTON_TA, &CVideoDlg::OnBnClickedButtonTa)
	ON_BN_CLICKED(IDC_BUTTON_TB, &CVideoDlg::OnBnClickedButtonTb)
	ON_MESSAGE(MY_USER_FULL_SCREEN, &CVideoDlg::OnMyUserFullScreen)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TURN, &CVideoDlg::OnBnClickedButtonTurn)
END_MESSAGE_MAP()


// CVideoDlg 消息处理程序


BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_brush.CreateSolidBrush(RGB(0, 0, 0));
	for (int i = 0; i < 4; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
		m_button[i].SetForeColor(RGB(255, 255, 255));
	}
	m_BTurn.SetBkColor(RGB(0, 0, 0));
	m_BTurn.SetForeColor(RGB(255, 255, 255));

	InitVideoPlayRect();

	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i]->Create(" ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_NOTIFY, m_videoPlayRects[i], this, 6000 + i);
		m_videoPlayWnd[i]->WndIndex = i;
		/*CString t;
		t.Format("%d",i);
		m_videoPlayWnd[i]->setShowText(t);*/
		if (i >= 4)
		{
			m_videoPlayWnd[i]->ShowWindow(SW_HIDE);
		}
	}

	if (theApp.Local[1] == 'A')
		OnBnClickedButtonBa();
	else
		OnBnClickedButtonBb();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CVideoDlg::InitVideoPlayRect()
{
	CRect bigRect(0, 0, SCREEN_X, SCREEN_Y / 40 * 38 - 10);
	int WidthTemp = 35;
	int width = bigRect.Width();//高度，
	int height = bigRect.Height();//宽度
	int screenBeginX = bigRect.TopLeft().x;//左上角坐标x
	int screenBeginY = bigRect.TopLeft().y;//左上角坐标y
	int screenEndX = bigRect.BottomRight().x;//右下角坐标x
	int screenEndY = bigRect.BottomRight().y - WidthTemp;//右下角坐标y
	int lm = 1;

	//////////////////////////////////////////////////////////////////////////
	//按钮位置
	GetDlgItem(IDC_BUTTON_BA)->MoveWindow(0, screenEndY, 100, 45);
	GetDlgItem(IDC_BUTTON_BB)->MoveWindow(100, screenEndY, 100, 45);
	GetDlgItem(IDC_BUTTON_TA)->MoveWindow(200, screenEndY, 100, 45);
	GetDlgItem(IDC_BUTTON_TB)->MoveWindow(300, screenEndY, 100, 45);

	GetDlgItem(IDC_BUTTON_TRUN)->MoveWindow(450, screenEndY, 100, 45);

	//GetDlgItem(IDC_BUTTON_TA)->ShowWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_TB)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC)->MoveWindow(600, screenEndY + 5, 120, 20);
	//////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < 8; i++)
	{
		m_videoPlayRects[4 * i] = CRect(0, 0, screenEndX / 4 * 2 - 2 * lm, screenEndY / 4 * 2 - lm);//1

		m_videoPlayRects[4 * i + 1] = CRect(screenEndX / 4 * 2 + lm, 0, screenEndX / 4 * 4 - lm, screenEndY / 4 * 2 - lm);//2

		m_videoPlayRects[4 * i + 2] = CRect(0, screenEndY / 4 * 2 + lm, screenEndX / 4 * 2 - 2 * lm, screenEndY / 4 * 4);//3

		m_videoPlayRects[4 * i + 3] = CRect(screenEndX / 4 * 2 + lm, screenEndY / 4 * 2 + lm, screenEndX / 4 * 4 - lm, screenEndY / 4 * 4);//4
	}


	//全屏
	m_SingleWndRec = CRect(0, 0, screenEndX / 4 * 4 - lm, screenEndY / 4 * 4);
}

void CVideoDlg::OnBnClickedButtonBa()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < 4; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[0].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC, "当前：本机A");
	CurrentPage = 0;
	FullScreenFlag = FALSE;
	CurrentBox = 0;

	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i]->MoveWindow(&m_videoPlayRects[i]);
	}

	for (int i = 0; i < 32; i++)
	{
		if ((i <= 8 * CurrentBox + 3 && i >= 8 * CurrentBox))
		{
			m_videoPlayWnd[i]->ShowWindow(SW_SHOW);
		}
		else
			m_videoPlayWnd[i]->ShowWindow(SW_HIDE);
	}
}


void CVideoDlg::OnBnClickedButtonBb()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < 4; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[1].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC, "当前：本机B");

	FullScreenFlag = FALSE;
	CurrentBox = 1;
	CurrentPage = 0;
	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i]->MoveWindow(&m_videoPlayRects[i]);
	}

	for (int i = 0; i < 32; i++)
	{
		if ((i <= 8 * CurrentBox + 3 && i >= 8 * CurrentBox))
		{
			m_videoPlayWnd[i]->ShowWindow(SW_SHOW);
		}
		else
			m_videoPlayWnd[i]->ShowWindow(SW_HIDE);
	}
}


void CVideoDlg::OnBnClickedButtonTa()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < 4; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[2].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC, "当前：补机A");
	CurrentPage = 0;
	FullScreenFlag = FALSE;
	CurrentBox = 2;

	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i]->MoveWindow(&m_videoPlayRects[i]);
	}

	for (int i = 0; i < 32; i++)
	{
		if ((i <= 8 * CurrentBox + 3 && i >= 8 * CurrentBox))
		{
			m_videoPlayWnd[i]->ShowWindow(SW_SHOW);
		}
		else
			m_videoPlayWnd[i]->ShowWindow(SW_HIDE);
	}
}


void CVideoDlg::OnBnClickedButtonTb()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < 4; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[3].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC, "当前：补机B");
	CurrentPage = 0;
	FullScreenFlag = FALSE;
	CurrentBox = 3;

	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i]->MoveWindow(&m_videoPlayRects[i]);
	}

	for (int i = 0; i < 32; i++)
	{
		if ((i <= 8 * CurrentBox + 3 && i >= 8 * CurrentBox))
		{
			m_videoPlayWnd[i]->ShowWindow(SW_SHOW);
		}
		else
			m_videoPlayWnd[i]->ShowWindow(SW_HIDE);
	}
}

afx_msg LRESULT CVideoDlg::OnMyUserFullScreen(WPARAM wParam, LPARAM lParam)
{
	/*char buf[20] = "";
	sprintf_s(buf,"index= %d",lParam);
	MessageBox(buf);*/
	TRACE("index= %d \n", lParam);

	ChangeWndRects(FullScreenFlag, lParam);

	FullScreenFlag = ~FullScreenFlag;

	return 0;
}

int CVideoDlg::ChangeWndRects(bool isSingle, int pos)
{

	if (!isSingle)
	{
		for (int i = 0; i < 32; i++)
		{
			if (i == pos)
			{
				m_videoPlayWnd[i]->MoveWindow(&m_SingleWndRec);
				m_videoPlayWnd[i]->ShowWindow(TRUE);
			}
			else
			{
				m_videoPlayWnd[i]->ShowWindow(FALSE);
			}
		}
	}
	else
	{
		for (int i = 0; i < 32; i++)
		{
			m_videoPlayWnd[i]->MoveWindow(&m_videoPlayRects[i]);
			if (i <= 4 * (pos / 4) + 3 && i >= 4 * (pos / 4))
			{
				m_videoPlayWnd[i]->ShowWindow(TRUE);
			}
			else
				m_videoPlayWnd[i]->ShowWindow(FALSE);
		}
	}
	return 0;
}

HBRUSH CVideoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)      //对话框颜色  
		return   m_brush;       //返加刷子

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  
		hbr = (HBRUSH)m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CVideoDlg::OnBnClickedButtonTurn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FullScreenFlag)
		return;

	if (++CurrentPage > 1)
		CurrentPage = 0;

	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i]->MoveWindow(&m_videoPlayRects[i]);
		if (i <= 8 * CurrentBox + 3 + 4 * CurrentPage && i >= 8 * CurrentBox + 4 * CurrentPage)
		{
			m_videoPlayWnd[i]->ShowWindow(TRUE);
		}
		else
			m_videoPlayWnd[i]->ShowWindow(FALSE);
	}


}
