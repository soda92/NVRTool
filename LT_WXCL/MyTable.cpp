// MyTable.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTable.h"


// CMyTable

IMPLEMENT_DYNAMIC(CMyTable, CTabCtrl)

CMyTable::CMyTable()
{
	m_fontHeight = 25;
	m_fontWith = 11;
	// 	m_R = 120;
	// 	m_G = 60;
	// 	m_B = 58;
	m_R = 60;
	m_G = 180;
	m_B = 58;
	m_Gradient = 5;
}

CMyTable::~CMyTable()
{
}


BEGIN_MESSAGE_MAP(CMyTable, CTabCtrl)
	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()



// CMyTable 消息处理程序




void CMyTable::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//创建画刷
	//CBrush      cbr;
	//cbr.CreateSolidBrush(RGB(0,0,0));

	//获取选项卡文字内容
	char        szTabText[100];
	TC_ITEM     tci;
	memset(szTabText, '\0', sizeof(szTabText));
	tci.mask = TCIF_TEXT;
	tci.pszText = szTabText;
	tci.cchTextMax = sizeof(szTabText) - 1;
	GetItem(lpDrawItemStruct->itemID, &tci);


	if (lpDrawItemStruct->itemState)
	{
		CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);

		CRect tRect;
		GetItemRect(lpDrawItemStruct->itemID, &tRect);//得到Item的尺寸
		int R = m_R, G = m_G, B = m_B;
		CRect nRect(tRect);//拷贝尺寸到新的容器中
		//画分割线 
		CBrush _brush;
		_brush.CreateSolidBrush(RGB(R - 15, G - 15, B - 15));
		CRect m_rect(tRect);
		m_rect.right = nRect.left++;
		dc->FillRect(&m_rect, &_brush); //填充背景 
		_brush.DeleteObject();

		for (int j = tRect.top; j <= tRect.bottom; j++)
		{
			nRect.bottom = nRect.top + 1;
			CBrush _brush;
			_brush.CreateSolidBrush(RGB(R, G, B));//创建画刷 
			dc->FillRect(&nRect, &_brush); //填充背景 
			_brush.DeleteObject(); //释放画刷 
			R -= m_Gradient; G -= m_Gradient; B -= m_Gradient;
			if (R < 0)R = 0;
			if (G < 0)G = 0;
			if (B < 0)B = 0;
			nRect.top = nRect.bottom;
		}
		dc->SetBkMode(TRANSPARENT);

		dc->SetTextColor(RGB(255, 255, 255));
		CFont nFont, * nOldFont;
		nFont.CreateFont(m_fontHeight, m_fontWith, 0, 0, 0, FALSE, FALSE, 0, 0, 0, 0, 0, 0, _TEXT("黑体"));//创建字体 
		nOldFont = dc->SelectObject(&nFont);
		RECT rc;
		rc = lpDrawItemStruct->rcItem;
		rc.top += 12;
		dc->DrawText(tci.pszText, lstrlen(tci.pszText), &rc, DT_CENTER | DT_VCENTER);

	}
	else
	{
		CBrush      cbr;
		cbr.CreateSolidBrush(RGB(0, 0, 0));
		//填充选项卡背景
		CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);
		dc->FillRect(&lpDrawItemStruct->rcItem, &cbr);
		//绘制选项卡文字
		dc->SetBkColor(RGB(0, 0, 0));
		dc->SetTextColor(RGB(255, 255, 255));
		CFont nFont, * nOldFont;
		nFont.CreateFont(m_fontHeight, m_fontWith, 0, 0, 0, FALSE, FALSE, 0, 0, 0, 0, 0, 0, _TEXT("黑体"));//创建字体 
		nOldFont = dc->SelectObject(&nFont);
		RECT rc;
		rc = lpDrawItemStruct->rcItem;
		rc.top += 12;
		dc->DrawText(tci.pszText, lstrlen(tci.pszText), &rc, DT_CENTER | DT_VCENTER);
	}
}


BOOL CMyTable::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect   rect;
	GetClientRect(&rect);

	//创建画刷
	CBrush   brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));

	//填充控件背景
	pDC->FillRect(&rect, &brush);

	//return CTabCtrl::OnEraseBkgnd(pDC);
	return true;
}
