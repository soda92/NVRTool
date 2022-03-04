// ListCtrlCl.cpp : 实现文件
//

#include "stdafx.h"
#include "ListCtrlCl.h"
#include "resource.h"

// CListCtrlCl

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};
// CListCtrlCl
IMPLEMENT_DYNAMIC(CListCtrlCl, CListCtrl)
CListCtrlCl::CListCtrlCl()
	: m_nRowHeight(0)
	, m_fontHeight(12)
	, m_fontWith(0)
{
	m_color = RGB(0, 0, 0);
}
CListCtrlCl::~CListCtrlCl()
{
	stColor* ptemp = NULL;
	while (m_ptrListCol.GetCount() > 0)
	{
		ptemp = (stColor*)(m_ptrListCol.RemoveHead());
		if (NULL != ptemp)
		{
			delete ptemp;
			ptemp = NULL;
		}
	}
	while (m_ptrListItem.GetCount() > 0)
	{
		ptemp = (stColor*)(m_ptrListItem.RemoveHead());
		if (NULL != ptemp)
		{
			delete ptemp;
			ptemp = NULL;
		}
	}
	while (m_colTextColor.GetCount() > 0)
	{
		ptemp = (stColor*)(m_colTextColor.RemoveHead());
		if (NULL != ptemp)
		{
			delete ptemp;
			ptemp = NULL;
		}
	}
	while (m_ItemTextColor.GetCount() > 0)
	{
		ptemp = (stColor*)(m_ItemTextColor.RemoveHead());
		if (NULL != ptemp)
		{
			delete ptemp;
			ptemp = NULL;
		}
	}
}
BEGIN_MESSAGE_MAP(CListCtrlCl, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
// CListCtrlCl 消息处理程序
void CListCtrlCl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, LVS_OWNERDRAWFIXED);
	ModifyStyle(0, LVS_REPORT);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());

}
void CListCtrlCl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	TCHAR lpBuffer[256] = {0};
	LV_ITEM lvi = {0};
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = lpDrawItemStruct->itemID;
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));
	LV_COLUMN lvc, lvcprev;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;
	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rtClient;
	GetClientRect(&rtClient);
	for (int nCol = 0; GetColumn(nCol, &lvc); nCol++)
	{
		if (nCol > 0)
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol - 1, &lvcprev);
			lpDrawItemStruct->rcItem.left += lvcprev.cx;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left;
		}
		CRect rcItem;
		if (!GetSubItemRect(lpDrawItemStruct->itemID, nCol, LVIR_LABEL, rcItem))
			continue;
		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;
		if (nCol == 0)
		{
			rcTemp.left -= 2;
		}
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			pDC->FillSolidRect(&rcTemp, GetSysColor(COLOR_HIGHLIGHT));
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			COLORREF color;
			color = GetBkColor();
			pDC->FillSolidRect(rcTemp, color);
			if (FindColColor(nCol, color))
			{
				pDC->FillSolidRect(rcTemp, color);
			}
			if (FindItemColor(nCol, lpDrawItemStruct->itemID, color))
			{
				pDC->FillSolidRect(rcTemp, color);
			}
		}
		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		UINT   uFormat = DT_CENTER;

		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0, ofst / 2);
		pDC->SetTextColor(m_color);
		COLORREF color;
		if (FindColTextColor(nCol, color))
		{
			pDC->SetTextColor(color);
		}
		if (FindItemTextColor(nCol, lpDrawItemStruct->itemID, color))
		{
			pDC->SetTextColor(color);
		}
		CFont nFont, * nOldFont;
		nFont.CreateFont(m_fontHeight, m_fontWith, 0, 0, 0, FALSE, FALSE, 0, 0, 0, 0, 0, 0, _TEXT("黑体"));//创建字体
		nOldFont = pDC->SelectObject(&nFont);
		DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer),
			&rcItem, uFormat);//工程为unicode 应该为wcslen（lpBuffer)

		pDC->SelectStockObject(SYSTEM_FONT);
	}
}

void CListCtrlCl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight > 0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}
int CListCtrlCl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	if (nFormat == LVCFMT_LEFT)
	{
		m_Header.m_Format = m_Header.m_Format + _T("0");
	}
	else if (nFormat == LVCFMT_CENTER)
	{
		m_Header.m_Format = m_Header.m_Format + _T("1");
	}
	else if (nFormat == LVCFMT_RIGHT)
	{
		m_Header.m_Format = m_Header.m_Format + _T("2");
	}
	else
	{
		m_Header.m_Format = m_Header.m_Format + _T("1");
	}
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}
// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CListCtrlCl::SetHeaderBKColor(int R, int G1, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G1;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}
//设置表头高度
void CListCtrlCl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_Height = Height;
}
bool CListCtrlCl::FindColColor(int col, COLORREF& color) //查找列颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListCol.GetHeadPosition(); pos != NULL;)
	{
		stColor* pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor->nCol == col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemColor(int col, int row, COLORREF& color) //查找颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListItem.GetHeadPosition(); pos != NULL;)
	{
		stColor* pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor->nCol == col && pColor->nRow == row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetColColor(int col, COLORREF color) //设置列颜色
{
	stColor* pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_ptrListCol.AddTail(pColor);
}

void CListCtrlCl::SetItemColor(int col, int row, COLORREF color) //设置格子颜色
{
	stColor* pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ptrListItem.AddTail(pColor);

}
void CListCtrlCl::SetRowHeight(int nHeight) // 设置行高
{
	m_nRowHeight = nHeight;
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp = {0};
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}
void CListCtrlCl::SetHeaderFontHW(int nHeight, int nWith) //设置头部字体宽和高
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}
void CListCtrlCl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_color = color;
}
void CListCtrlCl::SetTextColor(COLORREF cr)  //设置字体颜色
{
	m_color = cr;
}
void CListCtrlCl::SetFontHW(int nHeight, int nWith) //设置字体高和宽
{
	m_fontHeight = nHeight;
	m_fontWith = nWith;
}
void CListCtrlCl::SetColTextColor(int col, COLORREF color)
{
	stColor* pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_colTextColor.AddTail(pColor);
}
bool CListCtrlCl::FindColTextColor(int col, COLORREF& color)
{
	int flag = 0;
	for (POSITION pos = m_colTextColor.GetHeadPosition(); pos != NULL;)
	{
		stColor* pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor->nCol == col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemTextColor(int col, int row, COLORREF& color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition(); pos != NULL;)
	{
		stColor* pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nCol == col && pColor->nRow == row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}

void CListCtrlCl::SetItemTextColor(int col, int row, COLORREF color)
{
	stColor* pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ItemTextColor.AddTail(pColor);
}



// CListCtrlCl 消息处理程序


int CListCtrlCl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
	return 0;
}



HBRUSH CListCtrlCl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

    HBRUSH hBrush = NULL;
    CBrush brush;
    brush.CreateSolidBrush(RGB(0, 0, 0));
    switch (nCtlColor)
    {
    case CTLCOLOR_DLG:
        // just return a not NULL brush handle
        return hbr;
        break;

    case CTLCOLOR_SCROLLBAR:
    {
        pDC->SetBkColor(RGB(255,255,255));
        HBRUSH b = CreateSolidBrush(RGB(0,0,0));
        return b;
    }
    case CTLCOLOR_STATIC:
    {
        // set text color, transparent back node then
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);
        // return a not NULL brush handle
        hBrush = (HBRUSH)brush;
    }
    break;
    default:
        // do the default processing
        return hbr;
        break;
    }
    return hBrush;

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
