#pragma once

#define MY_USER_FULL_SCREEN WM_USER+8
/*
*@������CMyEdit
*@���ã����岥�Ŵ�����ʾ�ĺ�ɫ����Ļ�����һ����ǩ����ǩ��һ�����źš�����
*@����:
*@����ֵ��
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
*@������CVideoPlayWnd()
*@���ã����Ŵ��ڵĺ�ɫ��Ļ
*/
class CVideoPlayWnd : public CStatic//�Զ���һ���࣬����������������
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


