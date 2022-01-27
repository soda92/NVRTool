#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrlCl.h"
#include "resource.h"
#include <vector>

// CManageDlg �Ի���

class CManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManageDlg)

public:
	CManageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CManageDlg();

	int InitList();
	int SetList();

	int m_RecordCallBack(LONG nPort, char* error);

	int SetIPCState();
	int SetHDDState();

    char OnlineDev[6] = {0}; // �����豸
	int FindAndDeleteRecord(CString Path);
	CString FindDir(char* HddPath);
	BOOL IsHDD(char* Path);


	// �Ի�������
	enum { IDD = IDD_DIALOG_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int num;
	CListCtrlCl m_IPCStateList;
	CListCtrlCl m_HDDStateList;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBrush m_brush;
	CFont newFont1;
public:
	CStatic m_static_device;
	CStatic m_static_hard;
    CStatic program_version;

	char RecordFlag[20];//¼�񱣴��־

    int StartURecord(char* uPath);
    bool URecordFlag; // U��ת����
    bool URecordStatus[6] = { false }; // ͨ��U�̱���״̬

    TCHAR szRootPathName[MAX_PATH] = ""; // U��Ŀ¼
};
