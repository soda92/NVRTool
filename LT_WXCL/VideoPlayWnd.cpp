// VideoPlayWnd.cpp : ʵ���ļ�
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

// CVideoPlayWnd ��Ϣ�������

/*
*@��������OnPaint()
*@���ã�CVideoPlayWnd���WM_PAINT��Ϣ������������Ϊ��ɫ����������λ����һ����ǩ�����źš�
*@����:LPVOID p,���������̵߳�ʱ�򣬰�������thisָ�봫������
*@����ֵ��UINT,û������
*/
void CVideoPlayWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rt;
	this->GetClientRect(&rt);
	dc.BitBlt(1, 1, rt.right, rt.bottom, NULL, 0, 0, BLACKNESS);//��ʾʹĿ�����������ɫȡ��//����
	m_showText->CenterWindow();
	/*CRect rr(rt.CenterPoint() , rt.BottomRight());
	dc.DrawText("������������������" ,&rr , WHITENESS|DT_SINGLELINE|DT_VCENTER|DT_CENTER );*/
}

/*
*@��������OnLButtonDblClk()
*@���ã�CVideoPlayWnd���˫���¼�������������ʵ��˫��ʱ�����͸������ڽ�����󻯡�
*@����:UINT nFlags, CPoint point˫��ʱ����Ҫ����������
*@����ֵ����
*/
void CVideoPlayWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*if (WndIndex > 4)
		return;*/
	::SendMessageA(GetParent()->m_hWnd, MY_USER_FULL_SCREEN, NULL, WndIndex);
	//CStatic::OnLButtonDblClk(nFlags, point);	
}


/*
*@��������OnCreate()
*@���ã�CVideoPlayWnd��Ĵ��ڴ�������
*@����:LPCREATESTRUCT lpCreateStructΪWM_CREATE��Ҫ�Ĳ���
*@����ֵ��������
*/
int CVideoPlayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_showText->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 120, 20), this, 9666);

	CHARFORMAT cf = {0};
	cf.dwEffects = 1;
	cf.crTextColor = RGB(255, 0, 0);
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwEffects = 0;
	cf.dwMask = CFM_COLOR;
	m_showText->SetDefaultCharFormat(cf);
	LONG lStyle = GetWindowLong(m_showText->m_hWnd, GWL_EXSTYLE);	// ����CRichEditCtrl�Ĵ��ڷ��
	lStyle |= WS_EX_TRANSPARENT;	// CRichEditCtrl�Ĵ��ڷ�����͸������
	SetWindowLong(m_showText->m_hWnd, GWL_EXSTYLE, lStyle);	// ����CRichEditCtrl���´��ڷ��
	m_showText->SetWindowTextA("���ź�");
	m_showText->EnableWindow(false);
	m_showText->CenterWindow();
	return 0;
}

/*
*@��������setShowText()
*@���ã��ı䲥�Ŵ���������ʾ������
*@����:CString strText���Ŵ�����Ҫ��ʾ������
*@����ֵ����
*/
void CVideoPlayWnd::setShowText(CString strText)
{
	m_showText->SetWindowTextA(strText);
}
