#pragma once

#include "stdafx.h"
#include "VideoPlayWnd.h"
#include "MyButton.h"
#include "resource.h"


// CVideoDlg 对话框

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoDlg();

	BOOL FullScreenFlag;
	CRect m_videoPlayRects[32]; //用来存放32个播放视频窗口的位置
	CRect m_SingleWndRec;
	void InitVideoPlayRect(); //初始化32个位置元素
	CVideoPlayWnd* m_videoPlayWnd[32];
	char CurrentBox;
	char CurrentPage;

    // 当前是否全屏
    bool fullscreened = false;
    // 全屏窗口索引
    int fullscreen_id = 0;
	/// <summary>
	/// 改变窗口布局
	/// </summary>
	/// <param name="is_normal">true 正常四窗口状态 false 最大化状态</param>
	/// <param name="pos">窗口id A节为（1~8）-1，B节为（9~16）-1</param>
	/// <returns>0</returns>
	int ChangeWndRects(bool is_normal, int pos); 

    // 对话框数据
	enum { IDD = IDD_DIALOG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnMyUserFullScreen(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    // 本机A
	afx_msg void OnBnClickedButtonBa();
    // 本机B
	afx_msg void OnBnClickedButtonBb();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	CBrush m_brush;
	CMyButton m_button[4];
public:
	CMyButton m_BTurn;
    // 翻页按钮
	afx_msg void OnBnClickedButtonTurn();
};
