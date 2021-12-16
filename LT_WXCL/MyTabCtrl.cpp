// MyTabCtrl.cpp : 实现文件
//
#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "MyTabCtrl.h"

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)

// CMyTabCtrl
IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{
}

CMyTabCtrl::~CMyTabCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
    ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// CMyTabCtrl 消息处理程序
void CMyTabCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    char        szTabText[100];
    UINT        bkColor;
    CBrush* cbr;
    TC_ITEM     tci;
    CBrush m_brRed(RGB(255, 0, 0)), m_brYellow(RGB(255, 255, 0)), m_brMagenta(RGB(0, 255, 255)), m_brWhite(RGB(0, 0, 0)), m_brBlue(RGB(0, 0, 255));
    switch (lpDrawItemStruct->itemID)
    {
    case 0:
        cbr = &m_brRed;
        bkColor = RED;
        break;
    case 1:
        cbr = &m_brYellow;
        bkColor = YELLOW;
        break;
    case 2:
        cbr = &m_brMagenta;
        bkColor = MAGENTA;
        break;
    case 3:
        cbr = &m_brWhite;
        bkColor = WHITE;
        break;
    case 4:
        cbr = &m_brBlue;
        bkColor = BLUE;
        break;
    }

    memset(szTabText, '\0', sizeof(szTabText));
    tci.mask = TCIF_TEXT;
    tci.pszText = szTabText;
    tci.cchTextMax = sizeof(szTabText) - 1;

    GetItem(lpDrawItemStruct->itemID, &tci);
    CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);
    dc->FillRect(&lpDrawItemStruct->rcItem, cbr);
    dc->SetBkColor(bkColor);

    TextOut(lpDrawItemStruct->hDC,
        lpDrawItemStruct->rcItem.left,
        lpDrawItemStruct->rcItem.top,
        tci.pszText,
        lstrlen(tci.pszText));

    CTabCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
