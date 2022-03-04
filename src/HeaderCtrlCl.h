#pragma once


// CHeaderCtrlCl

#pragma once


class CHeaderCtrlCl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderCtrlCl)

public:
	CHeaderCtrlCl();
	virtual ~CHeaderCtrlCl();

protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnPaint();

	CStringArray m_HChar;

	CString m_Format;

public:

	int m_R;

	int m_G;

	int m_B;
	int m_Gradient;	// 画立体背景，渐变系数

	float m_Height;  //表头高度，这是倍数,

	int m_fontHeight; //字体高度

	int m_fontWith;   //字体宽度

	COLORREF m_color;

	LRESULT OnLayout(WPARAM wParam, LPARAM lParam);

};
