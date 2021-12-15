#pragma once

#define MY_USER_FULL_SCREEN WM_USER+8
/*
*@类名：CMyEdit
*@作用：定义播放窗口显示的黑色的屏幕上面的一个标签，标签带一“无信号”字样
*@参数:
*@返回值：
*/
class CMyEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CMyEdit)

public:
	CMyEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
};

// CVideoPlayWnd
/*
*@类名：CVideoPlayWnd()
*@作用：播放窗口的黑色屏幕
*/
class CVideoPlayWnd : public CStatic//自定义一个类，用来做播放器窗口
{
	DECLARE_DYNAMIC(CVideoPlayWnd)

public:
	CVideoPlayWnd();
	virtual ~CVideoPlayWnd();
	CSize m_size;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void setShowText(CString strText);
	int WndIndex;

public:
	CMyEdit* m_showText;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


