#pragma once
#include "stdafx.h"

// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	int m_fontHeight;	// 字体高度
	int m_fontWith;         // 字体宽度

	int m_R;

	int m_G;

	int m_B;
	int m_Gradient;

};


