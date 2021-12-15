// VideoPlayWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoPlayWnd.h"

IMPLEMENT_DYNAMIC(CMyEdit, CRichEditCtrl)

CMyEdit::CMyEdit()
{
}

BEGIN_MESSAGE_MAP(CMyEdit, CRichEditCtrl)

END_MESSAGE_MAP()

// CVideoPlayWnd

IMPLEMENT_DYNAMIC(CVideoPlayWnd, CStatic)

CVideoPlayWnd::CVideoPlayWnd()
{
}

CVideoPlayWnd::~CVideoPlayWnd()
{
}


BEGIN_MESSAGE_MAP(CVideoPlayWnd, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CVideoPlayWnd 消息处理程序

/*
*@函数名：OnPaint()
*@作用：CVideoPlayWnd类的WM_PAINT消息处理函数，设置为黑色背景，中心位置有一个标签“无信号”
*@参数:LPVOID p,用来启动线程的时候，把类对象的this指针传过来，
*@返回值：UINT,没有作用
*/
void CVideoPlayWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rt;
	this->GetClientRect(&rt);
	dc.BitBlt(1, 1, rt.right, rt.bottom, NULL, 0, 0, BLACKNESS);//表示使目标矩形区域颜色取反//画黑
	m_showText->CenterWindow();
	/*CRect rr(rt.CenterPoint() , rt.BottomRight());
	dc.DrawText("嘻嘻嘻哈哈啊哈哈哈" ,&rr , WHITENESS|DT_SINGLELINE|DT_VCENTER|DT_CENTER );*/
}

/*
*@函数名：OnLButtonDblClk()
*@作用：CVideoPlayWnd类的双击事件处理函数，用来实现双击时，发送给父窗口将其最大化”
*@参数:UINT nFlags, CPoint point双击时间需要的两个参数
*@返回值：无
*/
void CVideoPlayWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*if (WndIndex > 4)
		return;*/
	::SendMessageA(GetParent()->m_hWnd, MY_USER_FULL_SCREEN, NULL, WndIndex);
	//CStatic::OnLButtonDblClk(nFlags, point);	
}


/*
*@函数名：OnCreate()
*@作用：CVideoPlayWnd类的窗口创建函数
*@参数:LPCREATESTRUCT lpCreateStruct为WM_CREATE需要的参数
*@返回值：无意义
*/
int CVideoPlayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_showText->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 120, 20), this, 9666);

	CHARFORMAT cf = {0};
	cf.dwEffects = 1;
	cf.crTextColor = RGB(255, 0, 0);
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwEffects = 0;
	cf.dwMask = CFM_COLOR;
	m_showText->SetDefaultCharFormat(cf);
	LONG lStyle = GetWindowLong(m_showText->m_hWnd, GWL_EXSTYLE);	// 保存CRichEditCtrl的窗口风格
	lStyle |= WS_EX_TRANSPARENT;	// CRichEditCtrl的窗口风格加入透明背景
	SetWindowLong(m_showText->m_hWnd, GWL_EXSTYLE, lStyle);	// 重设CRichEditCtrl的新窗口风格
	m_showText->SetWindowTextA("无信号");
	m_showText->EnableWindow(false);
	m_showText->CenterWindow();
	return 0;
}

/*
*@函数名：setShowText()
*@作用：改变播放窗口上面显示的文字
*@参数:CString strText播放窗口想要显示的文字
*@返回值：无
*/
void CVideoPlayWnd::setShowText(CString strText)
{
	m_showText->SetWindowTextA(strText);
}
