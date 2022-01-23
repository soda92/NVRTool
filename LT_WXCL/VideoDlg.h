#pragma once

#include "stdafx.h"
#include "VideoPlayWnd.h"
#include "MyButton.h"
#include "resource.h"


// CVideoDlg �Ի���

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoDlg();

	BOOL FullScreenFlag;
	CRect m_videoPlayRects[32]; //�������32��������Ƶ���ڵ�λ��
	CRect m_SingleWndRec;
	void InitVideoPlayRect(); //��ʼ��32��λ��Ԫ��
	CVideoPlayWnd* m_videoPlayWnd[32];
	char CurrentBox;
	char CurrentPage;

    // ��ǰ�Ƿ�ȫ��
    bool fullscreened = false;
    // ȫ����������
    int fullscreen_id = 0;
	/// <summary>
	/// �ı䴰�ڲ���
	/// </summary>
	/// <param name="is_normal">true �����Ĵ���״̬ false ���״̬</param>
	/// <param name="pos">����id A��Ϊ��1~8��-1��B��Ϊ��9~16��-1</param>
	/// <returns>0</returns>
	int ChangeWndRects(bool is_normal, int pos); 

    // �Ի�������
	enum { IDD = IDD_DIALOG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnMyUserFullScreen(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    // ����A
	afx_msg void OnBnClickedButtonBa();
    // ����B
	afx_msg void OnBnClickedButtonBb();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	CBrush m_brush;
	CMyButton m_button[4];
public:
	CMyButton m_BTurn;
    // ��ҳ��ť
	afx_msg void OnBnClickedButtonTurn();
};
