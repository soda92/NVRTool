#pragma once

#include <string>
#include "afxwin.h"
//#include "LT_WXCLDlg.h"
using std::string;


#define UPATH ":/LSKZ/"
#define FILEPATH "E:\\LSKZ\\"

// CRacingDlg �Ի���

class CRacingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRacingDlg)

public:
	CRacingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRacingDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_RACING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//���U���������ļ��������ļ�
	static DWORD WINAPI thread_copyDataToU(LPVOID lpParameter);

	//ʶ���ļ��е���Ϣ���������ڶ�
	bool copyToU();

	void set_Upath(string UP);

	CStatic* m_static_text[24];
	CStatic* m_static_value[24];   //0-5����A�� 6-11����B�� 12-17����A�� 18-23����B


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
	int static_value_handle1;//��̬�ı�m_static_value[24]�� ��ʼID
	int lastDataTime[4];

public:
	afx_msg void OnPaint();
	CStatic m_static_B;
	CStatic m_static_T;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CBrush m_brush;
};
