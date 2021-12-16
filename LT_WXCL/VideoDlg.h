#pragma once
#include "VideoPlayWnd.h"
#include "MyButton.h"
#include "afxwin.h"


// CVideoDlg �Ի���

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoDlg();

	BOOL FullScreenFlag;
	CRect m_videoPlayRects[32]; //�������16��������Ƶ���ڵ�λ��
	CRect m_SingleWndRec;
	void InitVideoPlayRect(); //��ʼ��16��λ��Ԫ��
	CVideoPlayWnd* m_videoPlayWnd[32];
	char CurrentBox;
	char CurrentPage;

    // ��ǰ�Ƿ�ȫ��
    bool fullscreened = false;
    // ȫ����������
    int fullscreen_id = 0;
    // �ı䴰�ڲ���
	int ChangeWndRects(bool isSingle, int pos); 

    // �Ի�������
	enum { IDD = IDD_DIALOG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnMyUserFullScreen(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBa();
	afx_msg void OnBnClickedButtonBb();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	CBrush m_brush;
	CMyButton m_button[4];
public:
	CMyButton m_BTurn;
	afx_msg void OnBnClickedButtonTurn();
};
