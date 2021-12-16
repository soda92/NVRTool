#pragma once
#include "VideoPlayWnd.h"
#include "MyButton.h"
#include "afxwin.h"


// CVideoDlg 对话框

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoDlg();

	BOOL FullScreenFlag;
	CRect m_videoPlayRects[32]; //用来存放16个播放视频窗口的位置
	CRect m_SingleWndRec;
	void InitVideoPlayRect(); //初始化16个位置元素
	CVideoPlayWnd* m_videoPlayWnd[32];
	char CurrentBox;
	char CurrentPage;

    // 当前是否全屏
    bool fullscreened = false;
    // 全屏窗口索引
    int fullscreen_id = 0;
    // 改变窗口布局
	int ChangeWndRects(bool isSingle, int pos); 

    // 对话框数据
	enum { IDD = IDD_DIALOG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
