#pragma once


// CWaitDlg �Ի���

class CWaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaitDlg)

public:
	CWaitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaitDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    int Num = 30;

private:
	CBrush m_brush;
};
