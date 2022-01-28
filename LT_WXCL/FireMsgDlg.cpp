// FireMsgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "FireMsgDlg.h"
#include "LT_LCWB_1ADlg.h"

#include "FireData.h"
#include "FireMsgView.h"


// CFireMsgDlg �Ի���

BOOL FireRecFlag = FALSE;
unsigned char FireToCh[9];//����̽ͷ��IPC�Ķ�Ӧ

IMPLEMENT_DYNAMIC(CFireMsgDlg, CDialogEx)

CFireMsgDlg::CFireMsgDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CFireMsgDlg::IDD, pParent)
{
    num = 0;
}

CFireMsgDlg::~CFireMsgDlg()
{
    FireMsgView::DialogExited();
}

void CFireMsgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_FIRE, m_FireListB);
}


BEGIN_MESSAGE_MAP(CFireMsgDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CFireMsgDlg ��Ϣ�������


BOOL CFireMsgDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    FireMsgView::DialogCreated();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_brush.CreateSolidBrush(RGB(0, 0, 0));

    m_FireListB.SetBkColor(RGB(0, 0, 0));
    m_FireListB.SetTextColor(RGB(255, 255, 255));

    m_FireListB.SetColTextColor(1, RGB(0, 255, 0));
    m_FireListB.SetColTextColor(2, RGB(0, 255, 0));

    for (int i = 0; i < 4; i++)
        f_fire_tex[i] = "";

    InitList();

    //���ڳ�ʼ��
    auto FireCom = http_get("/conf/FireCom").c_str();

    if (FireComInit((char*)FireCom) != -1)
    {
        //��EF�忨����TAX����Ϣ��Ȼ�����EF�忨�ظ������㲥
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_FireData, this, 0, NULL);
    }

    for (int i = 0; i < 4; i++)
    {
        lastDataTime[i] = 0;
    }
    SetTimer(201, 1000, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CFireMsgDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO: �ڴ˴������Ϣ����������
    if (num++ > 0)
    {
        CRect rc;
        this->GetClientRect(&rc);
        int x0 = rc.right * 0 + 100,
            y0 = static_cast<int>(rc.bottom * 0.08) + 20,
            width = static_cast<int>(rc.right * 0.5 * 1.4),
            height = static_cast<int>(rc.bottom * 1.1 * 0.71 - 18);

        m_FireListB.MoveWindow(x0, y0, width, height);

    }
}

int CFireMsgDlg::InitList()
{
    //�����С
    int size = 25;
    m_FireListB.SetFontHW(size, static_cast<int>(size * 0.5));
    m_FireListB.SetHeaderFontHW(size, static_cast<int>(size * 0.4));

    //�и�
    m_FireListB.SetRowHeight(38);
    m_FireListB.SetHeaderHeight(2);

    DWORD dwStyle = m_FireListB.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��	
    m_FireListB.SetExtendedStyle(dwStyle); //������չ���

    m_FireListB.InsertColumn(0, "  �豸����", LVCFMT_LEFT, 200);
    m_FireListB.InsertColumn(1, "  ����", LVCFMT_LEFT, 120);
    m_FireListB.InsertColumn(2, "  ״̬", LVCFMT_LEFT, 120);
    m_FireListB.InsertColumn(3, "��Ӧ��", LVCFMT_LEFT, 127 - 10 );

    for (int i = 0; i < 8; i++)
    {
        char a[20] = "";
        //itoa(i+1,a,10);
        sprintf_s(a, "%s̽ͷ%d", (i > 3 ? "B��" : "A��"), (i > 3 ? i - 3 : i + 1));
        m_FireListB.InsertItem(i, a);

        m_FireListB.SetItemText(i, 1, "δ֪");
        m_FireListB.SetItemText(i, 2, "δ֪");
        m_FireListB.SetItemText(i, 3, "LTDW");
    }

    return 0;
}

int CFireMsgDlg::FireDec(unsigned char* buf, int len, char pos)
{
    if (buf[0] == 0xAA && buf[1] == 0xAA)
    {
        if (buf[2] == 0x28 && buf[4] == 0x22)
        {
            if (buf[6] != 0)
            {
                if (buf[6] == 0x01)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "���߿�·");
                        }

                    }
                }
                else if (buf[6] == 0x02)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "���߶�·");
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < 5/*32*/; i++)//���32��̽ͷ
                {

                    switch ((buf[7 + i] & 0x07))
                    {
                    case 0://δ��װ
                        //m_FireList.SetItemText(i,1,"δ��װ");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "δ��װ");
                        }
                        continue;
                        break;
                    case 1://��ʾ���¸��ϣ������̸У�
                        //m_FireList.SetItemText(i,1,"�̸�");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "�̸�");
                        }
                        break;
                    case 2://��ʾ����
                        //m_FireList.SetItemText(i,1,"����");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "����");
                        }
                        break;
                    case 3://��ʾ����
                        //m_FireList.SetItemText(i,1,"����");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "����");
                        }
                        break;
                    case 4://��ʾ���µ��£����ݻ��棩
                        //m_FireList.SetItemText(i,1,"����");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "����");
                        }
                        break;
                    case 5://��ʾ�̸�
                        //m_FireList.SetItemText(i,1,"�̸�");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "�̸�");
                        }
                        break;
                    default:
                        //m_FireList.SetItemText(i,1,"δ֪");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "δ֪");
                        }
                        break;
                    }

                    switch (((buf[7 + i] & 0x38) >> 3))
                    {
                    case 0://����
                        //m_FireList.SetItemText(i,3,"����");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "����");
                        }
                        continue;
                        break;
                    case 1://����
                        //m_FireList.SetItemText(i,3,"����");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "����");
                        }
                        break;
                    case 2://����
                        //m_FireList.SetItemText(i,3,"����");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "����");
                        }
                        break;
                    case 3://��Ⱦ
                        //m_FireList.SetItemText(i,3,"��Ⱦ");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "��Ⱦ");
                        }
                        break;
                    case 4://����
                        //m_FireList.SetItemText(i,3,"����");
                        //m_FireList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
                        //����					
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "����");
                            m_FireListB.SetItemState(i + (5 * pos), LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
                        }
                        break;
                    default:
                        //m_FireList.SetItemText(i,3,"δ֪");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "δ֪");
                        }
                        break;
                    }
                }
            }
        }
    }

    return 0;
}

int CFireMsgDlg::SendFireMsg()
{

    unsigned char SendBuf[100] = "";
    memset(SendBuf, 0, 100);
    SendBuf[0] = 0xAA;
    SendBuf[1] = 0xAA;
    SendBuf[2] = 0x4F;

    SendBuf[4] = 0x01;
    if (((CLT_LCWB_1ADlg*)theApp.pMainDlg)->TaxStat)
    {
        SendBuf[5] = 0x11;

        memcpy(&SendBuf[6], ((CLT_LCWB_1ADlg*)theApp.pMainDlg)->TaxData.TAXDataBuf, 72);
    }
    else
        SendBuf[5] = 0x22;

    for (int i = 0; i < 78; i++)
    {
        SendBuf[78] += SendBuf[i];
    }

    return SendMsg(SendBuf, 79);
}

int CFireMsgDlg::FireDataAnalyse(unsigned char* buf, int len, int Train, BOOL SendFlag /*= FALSE*/)
{
    //Train=0 ����A��1 ����B��2 ����A�� 3 ����B

    CListCtrlCl* m_FireList;
    int startnum = 0;			//A��B�ڱ�־

    switch (Train) {
    case 0://����
        startnum = Train * 4;
        m_FireList = &m_FireListB;
        break;
    case 1:
        startnum = Train * 4;
        m_FireList = &m_FireListB;
        break;
    default:
        break;
    }


    if (buf[0] == 0xAA && buf[1] == 0xAA)
    {
        lastDataTime[Train] = 0;	//��ʱ������

        short plen = buf[2] | buf[3] << 8;
        if (plen != 0x2A)
        {
            return -1;
        }

        unsigned char src = 0;
        for (int i = 0; i < 41; i++)
        {
            src += buf[i];
        }

        if (src != buf[41])
        {
            return -1;
        }

        m_FireList->m_ItemTextColor.RemoveAll();
        FireRecFlag = TRUE;
        if (buf[4] == 1)
        {
            //����
            if (buf[5] != 0)
            {
                if (buf[5] == 0x01)
                {
                    //FireStr = "���߿�·";m_FireList.SetItemText(i,1,"δ֪");
                    for (int i = startnum; i < startnum + 4; i++)
                    {
                        m_FireList->SetItemText(i, 1, "���߿�·");
                        m_FireList->SetItemTextColor(2, i, RGB(255, 0, 0));
                    }
                }
                else if (buf[5] == 0x02)
                {
                    //FireStr = "���߶�·";
                    for (int i = startnum; i < startnum + 4; i++)
                    {
                        m_FireList->SetItemText(i, 1, "���߶�·");
                        m_FireList->SetItemTextColor(2, i, RGB(255, 0, 0));
                    }
                }
            }
            else
            {
                bool WarFlag = false;
                CString tmp_tex = "", ss = "";
                for (int i = 0; i < 4; i++)
                {
                    m_FireList->SetItemState(i, 0, LVIS_SELECTED);
                    switch ((buf[6 + i] & 0x07))
                    {
                    case 0://δ��װ
                        m_FireList->SetItemText(startnum + i, 1, "δ��װ");
                        continue;
                        break;
                    case 1://��ʾ���¸��ϣ������̸У�
                        m_FireList->SetItemText(startnum + i, 1, "�̸�");
                        break;
                    case 2://��ʾ����
                        m_FireList->SetItemText(startnum + i, 1, "����");
                        break;
                    case 3://��ʾ����
                        m_FireList->SetItemText(startnum + i, 1, "����");
                        break;
                    case 4://��ʾ���µ��£����ݻ��棩
                        m_FireList->SetItemText(startnum + i, 1, "����");
                        break;
                    case 5://��ʾ�̸�
                        m_FireList->SetItemText(startnum + i, 1, "�̸�");
                        break;
                    default:
                        m_FireList->SetItemText(startnum + i, 1, "δ֪");
                        break;
                    }

                    switch (((buf[6 + i] & 0x38) >> 3))
                    {
                    case 0://����
                        m_FireList->SetItemText(startnum + i, 2, "����");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 255, 255));
                        continue;
                        break;
                    case 1://����
                        m_FireList->SetItemText(startnum + i, 2, "����");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(0, 255, 0));
                        break;
                    case 2://����
                        m_FireList->SetItemText(startnum + i, 2, "����");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 0, 0));
                        break;
                    case 3://��Ⱦ
                        m_FireList->SetItemText(startnum + i, 2, "��Ⱦ");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 255, 0));
                        break;
                    case 4://����
                        if (tmp_tex == "")
                            tmp_tex = (0 == Train) ? "A��̽ͷ" : "B��̽ͷ";
                        else
                            tmp_tex += " ";
                        ss.Format("%d", i + 1);
                        tmp_tex += ss;

                        m_FireList->SetItemText(startnum + i, 2, "����");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 0, 0));
                        m_FireList->SetItemState(startnum + i, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
                        //����
                        if ((FireToCh[i] - 1) >= 0)
                        {
                            theApp.m_FireCH[(FireToCh[i] - 1)] = 1;
                        }
                        WarFlag = true;
                        break;
                    default:
                        m_FireList->SetItemText(startnum + i, 2, "δ֪");
                        m_FireList->SetItemTextColor(2, i, RGB(255, 0, 0));
                        break;
                    }
                }

                //���𱨾�״̬
                if (WarFlag)
                {
                    theApp.m_FireWarFlag = 1;
                    TRACE("m_FireWarFlag = 1\n");
                    if (tmp_tex != "") {
                        tmp_tex += "����";
                        f_fire_tex[Train] = tmp_tex;
                    }
                }
                else
                {
                    f_fire_tex[Train] = "";
                    memset(theApp.m_FireCH, 0, sizeof(theApp.m_FireCH));
                    if (theApp.m_FireWarFlag == 1)
                    {
                        theApp.m_FireWarFlag = 2;
                        TRACE("m_FireWarFlag = 2\n");
                    }

                }
            }
            return 0;
        }
    }
    return -1;
}

int CFireMsgDlg::StopWarFun()
{
    unsigned char SendBuf[16] = "";

    SendBuf[0] = 0xAA;
    SendBuf[1] = 0xAA;
    SendBuf[2] = 0x10;
    SendBuf[4] = 0x02;
    SendBuf[5] = 0x01;
    SendBuf[6] = 0x12;
    SendBuf[7] = 0x01;

    for (int i = 0; i < 15; i++)
    {
        SendBuf[15] += SendBuf[i];
    }
    SendMsg(SendBuf, 16);
    return 0;
}

void CFireMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    if (nIDEvent == 201)
    {
        for (int i = 0; i < 4; i++)
        {
            if (lastDataTime[i] > 15)	//������15��û���յ��ýڳ�������ʱ����ʼ���ýڳ����б�
            {
                //��ʼ���б�
                if (i < 2)
                {
                    for (int j = i * 4; j < i * 4 + 4; j++)		//����
                    {
                        m_FireListB.SetItemText(j, 1, "��");
                        m_FireListB.SetItemText(j, 2, "��");
                        m_FireListB.SetItemText(j, 3, "LTDW");
                    }
                }
                lastDataTime[i] = 0;
            }
            lastDataTime[i]++;
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CFireMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����

    if (nCtlColor == CTLCOLOR_DLG)      //�Ի�����ɫ  
        return m_brush;

    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
        hbr = (HBRUSH)m_brush;
    }
    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}


void CFireMsgDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������

    //AB�� ��
    CFont font1, * oldfont;
    int textSize = 200, ox = 40, oy = 20;

    dc.SetTextColor(RGB(255, 255, 255));										//����������ɫ
    font1.CreatePointFont(textSize, _T("����"), &dc);						//��������

    oldfont = dc.SelectObject(&font1);										//ѡ����������PDC
    dc.SetBkMode(TRANSPARENT);
    // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
