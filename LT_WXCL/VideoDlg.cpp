// VideoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "LT_LCWB_1ADlg.h"


// CVideoDlg �Ի���

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
{
	for (int i = 0; i < 32; i++)
	{
		m_videoPlayWnd[i] = new CVideoPlayWnd; //ʵ����12�����Ŵ���
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
	DDX_Control(pDX, IDC_BUTTON_TURN, m_BTurn);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BA, &CVideoDlg::OnBnClickedButtonBa)
	ON_BN_CLICKED(IDC_BUTTON_BB, &CVideoDlg::OnBnClickedButtonBb)
	ON_MESSAGE(MY_USER_FULL_SCREEN, &CVideoDlg::OnMyUserFullScreen)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TURN, &CVideoDlg::OnBnClickedButtonTurn)
END_MESSAGE_MAP()


// CVideoDlg ��Ϣ�������


BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
		m_videoPlayWnd[i]->Create(" ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_NOTIFY,
			m_videoPlayRects[i], this, 6000 + i);
		m_videoPlayWnd[i]->WndIndex = i;

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
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CVideoDlg::InitVideoPlayRect()
{
	CRect bigRect(0, 0, SCREEN_X, SCREEN_Y / 40 * 38 - 10);
	int WidthTemp = 35;
	int width = bigRect.Width(); //�߶ȣ�
	int height = bigRect.Height(); //���
	int screenBeginX = bigRect.TopLeft().x; //���Ͻ�����x
	int screenBeginY = bigRect.TopLeft().y; //���Ͻ�����y
	int screenEndX = bigRect.BottomRight().x; //���½�����x
	int screenEndY = bigRect.BottomRight().y - WidthTemp; //���½�����y
	int lm = 1;

	//////////////////////////////////////////////////////////////////////////
	//��ťλ��
	GetDlgItem(IDC_BUTTON_BA)->MoveWindow(0, screenEndY, 100, 45);
	GetDlgItem(IDC_BUTTON_BB)->MoveWindow(100, screenEndY, 100, 45);

	GetDlgItem(IDC_BUTTON_TURN)->MoveWindow(450, screenEndY, 100, 45);

	GetDlgItem(IDC_STATIC)->MoveWindow(600, screenEndY + 5, 120, 20);
	//////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < 8; i++)
	{
		m_videoPlayRects[4 * i] = CRect(
			0, 0,
			screenEndX / 4 * 2 - 2 * lm, screenEndY / 4 * 2 - lm); //1
		m_videoPlayRects[4 * i + 1] = CRect(
			screenEndX / 4 * 2 + lm, 0,
			screenEndX / 4 * 4 - lm, screenEndY / 4 * 2 - lm); //2
		m_videoPlayRects[4 * i + 2] = CRect(
			0, screenEndY / 4 * 2 + lm,
			screenEndX / 4 * 2 - 2 * lm, screenEndY / 4 * 4); //3
		m_videoPlayRects[4 * i + 3] = CRect(
			screenEndX / 4 * 2 + lm, screenEndY / 4 * 2 + lm,
			screenEndX / 4 * 4 - lm, screenEndY / 4 * 4); //4
	}

	//ȫ��
	m_SingleWndRec = CRect(0, 0, screenEndX / 4 * 4 - lm, screenEndY / 4 * 4);
}

void CVideoDlg::OnBnClickedButtonBa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < 4; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[0].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC, "��ǰ������A");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < 4; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[1].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC, "��ǰ������B");

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

afx_msg LRESULT CVideoDlg::OnMyUserFullScreen(WPARAM wParam, LPARAM lParam)
{

	TRACE("index= %d \n", lParam);

	ChangeWndRects(FullScreenFlag, lParam);

    fullscreen_id = static_cast<int>(lParam);
	FullScreenFlag = ~FullScreenFlag;

	return 0;
}

int CVideoDlg::ChangeWndRects(bool isSingle, int pos)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)theApp.m_pMainWnd;
	if (!isSingle)
	{
        // ��ֹ�ظ�����
        if (fullscreened && fullscreen_id == pos) {
            return 0;
        }
		for (int i = 0; i < 32; i++)
		{
			if (i == pos)
			{
				m_videoPlayWnd[i]->MoveWindow(&m_SingleWndRec);
                
				m_videoPlayWnd[i]->ShowWindow(TRUE);
                if (!fullscreened) {
                    fullscreened = true;
                }
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
        fullscreened = false;
	}
	return 0;
}

void CVideoDlg::OnBnClickedButtonTurn()
{
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


HBRUSH CVideoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
		return   m_brush;

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
        // ���Ծ�̬�ؼ��ı�����ɫ����
		pDC->SetBkMode(TRANSPARENT); 
		hbr = (HBRUSH)m_brush;
	}

	return hbr;
}

