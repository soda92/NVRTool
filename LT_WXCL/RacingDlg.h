#pragma once

#include <string>
#include "afxwin.h"
//#include "LT_WXCLDlg.h"
using std::string;


#define UPATH ":/LSKZ/"
#define FILEPATH "E:\\LSKZ\\"

// CRacingDlg 对话框

class CRacingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRacingDlg)

public:
	CRacingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRacingDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_RACING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//检测U盘中配置文件，拷贝文件
	static DWORD WINAPI thread_copyDataToU(LPVOID lpParameter);

	//识别文件中的信息，返回日期段
	bool copyToU();

	void set_Upath(string UP);

	CStatic* m_static_text[24];
	CStatic* m_static_value[24];   //0-5本车A， 6-11本车B， 12-17他车A， 18-23他车B


	CFont newFont;
	CFont newFont1;

	int SendFireMsg();

	bool isCopy(string path, string date, string days);
	int Copy_File(string date, string days, string Upath);

	int LdleMsgAnalysis(unsigned char* buf, int len, int Train, BOOL SendFlag = FALSE);

	void saveData(unsigned char* buf);

	CString strFilePath;
	CString strTrainNum;

private:
	string Upath;
	int static_value_handle1;//静态文本m_static_value[24]的 起始ID
	int lastDataTime[4];

public:
	afx_msg void OnPaint();
	CStatic m_static_B;
	CStatic m_static_T;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CBrush m_brush;
};
