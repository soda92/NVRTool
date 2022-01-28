// FireMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "FireMsgDlg.h"
#include "LT_LCWB_1ADlg.h"

#include "FireData.h"
#include "FireMsgView.h"


// CFireMsgDlg 对话框

BOOL FireRecFlag = FALSE;
unsigned char FireToCh[9];//防火探头和IPC的对应

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


// CFireMsgDlg 消息处理程序


BOOL CFireMsgDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    FireMsgView::DialogCreated();

    // TODO:  在此添加额外的初始化
    m_brush.CreateSolidBrush(RGB(0, 0, 0));

    m_FireListB.SetBkColor(RGB(0, 0, 0));
    m_FireListB.SetTextColor(RGB(255, 255, 255));

    m_FireListB.SetColTextColor(1, RGB(0, 255, 0));
    m_FireListB.SetColTextColor(2, RGB(0, 255, 0));

    for (int i = 0; i < 4; i++)
        f_fire_tex[i] = "";

    InitList();

    //串口初始化
    auto FireCom = http_get("/conf/FireCom").c_str();

    if (FireComInit((char*)FireCom) != -1)
    {
        //向EF板卡发送TAX箱消息，然后接收EF板卡回复，并广播
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_FireData, this, 0, NULL);
    }

    for (int i = 0; i < 4; i++)
    {
        lastDataTime[i] = 0;
    }
    SetTimer(201, 1000, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CFireMsgDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
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
    //字体大小
    int size = 25;
    m_FireListB.SetFontHW(size, static_cast<int>(size * 0.5));
    m_FireListB.SetHeaderFontHW(size, static_cast<int>(size * 0.4));

    //行高
    m_FireListB.SetRowHeight(38);
    m_FireListB.SetHeaderHeight(2);

    DWORD dwStyle = m_FireListB.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）	
    m_FireListB.SetExtendedStyle(dwStyle); //设置扩展风格

    m_FireListB.InsertColumn(0, "  设备名称", LVCFMT_LEFT, 200);
    m_FireListB.InsertColumn(1, "  类型", LVCFMT_LEFT, 120);
    m_FireListB.InsertColumn(2, "  状态", LVCFMT_LEFT, 120);
    m_FireListB.InsertColumn(3, "供应商", LVCFMT_LEFT, 127 - 10 );

    for (int i = 0; i < 8; i++)
    {
        char a[20] = "";
        //itoa(i+1,a,10);
        sprintf_s(a, "%s探头%d", (i > 3 ? "B节" : "A节"), (i > 3 ? i - 3 : i + 1));
        m_FireListB.InsertItem(i, a);

        m_FireListB.SetItemText(i, 1, "未知");
        m_FireListB.SetItemText(i, 2, "未知");
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
                            m_FireListB.SetItemText(i + (5 * pos), 1, "总线开路");
                        }

                    }
                }
                else if (buf[6] == 0x02)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "总线短路");
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < 5/*32*/; i++)//最大32个探头
                {

                    switch ((buf[7 + i] & 0x07))
                    {
                    case 0://未安装
                        //m_FireList.SetItemText(i,1,"未安装");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "未安装");
                        }
                        continue;
                        break;
                    case 1://表示烟温复合（兼容烟感）
                        //m_FireList.SetItemText(i,1,"烟感");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "烟感");
                        }
                        break;
                    case 2://表示高温
                        //m_FireList.SetItemText(i,1,"高温");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "高温");
                        }
                        break;
                    case 3://表示火焰
                        //m_FireList.SetItemText(i,1,"火焰");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "火焰");
                        }
                        break;
                    case 4://表示感温电缆（兼容火焰）
                        //m_FireList.SetItemText(i,1,"火焰");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "火焰");
                        }
                        break;
                    case 5://表示烟感
                        //m_FireList.SetItemText(i,1,"烟感");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "烟感");
                        }
                        break;
                    default:
                        //m_FireList.SetItemText(i,1,"未知");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 1, "未知");
                        }
                        break;
                    }

                    switch (((buf[7 + i] & 0x38) >> 3))
                    {
                    case 0://离线
                        //m_FireList.SetItemText(i,3,"离线");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "离线");
                        }
                        continue;
                        break;
                    case 1://正常
                        //m_FireList.SetItemText(i,3,"正常");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "正常");
                        }
                        break;
                    case 2://故障
                        //m_FireList.SetItemText(i,3,"故障");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "故障");
                        }
                        break;
                    case 3://污染
                        //m_FireList.SetItemText(i,3,"污染");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "污染");
                        }
                        break;
                    case 4://报警
                        //m_FireList.SetItemText(i,3,"报警");
                        //m_FireList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
                        //联动					
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "报警");
                            m_FireListB.SetItemState(i + (5 * pos), LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
                        }
                        break;
                    default:
                        //m_FireList.SetItemText(i,3,"未知");
                        if (pos < 2)
                        {
                            m_FireListB.SetItemText(i + (5 * pos), 2, "未知");
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
    //Train=0 本车A，1 本车B，2 他车A， 3 他车B

    CListCtrlCl* m_FireList;
    int startnum = 0;			//A、B节标志

    switch (Train) {
    case 0://本车
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
        lastDataTime[Train] = 0;	//计时器归零

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
            //防火
            if (buf[5] != 0)
            {
                if (buf[5] == 0x01)
                {
                    //FireStr = "总线开路";m_FireList.SetItemText(i,1,"未知");
                    for (int i = startnum; i < startnum + 4; i++)
                    {
                        m_FireList->SetItemText(i, 1, "总线开路");
                        m_FireList->SetItemTextColor(2, i, RGB(255, 0, 0));
                    }
                }
                else if (buf[5] == 0x02)
                {
                    //FireStr = "总线短路";
                    for (int i = startnum; i < startnum + 4; i++)
                    {
                        m_FireList->SetItemText(i, 1, "总线短路");
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
                    case 0://未安装
                        m_FireList->SetItemText(startnum + i, 1, "未安装");
                        continue;
                        break;
                    case 1://表示烟温复合（兼容烟感）
                        m_FireList->SetItemText(startnum + i, 1, "烟感");
                        break;
                    case 2://表示高温
                        m_FireList->SetItemText(startnum + i, 1, "高温");
                        break;
                    case 3://表示火焰
                        m_FireList->SetItemText(startnum + i, 1, "火焰");
                        break;
                    case 4://表示感温电缆（兼容火焰）
                        m_FireList->SetItemText(startnum + i, 1, "火焰");
                        break;
                    case 5://表示烟感
                        m_FireList->SetItemText(startnum + i, 1, "烟感");
                        break;
                    default:
                        m_FireList->SetItemText(startnum + i, 1, "未知");
                        break;
                    }

                    switch (((buf[6 + i] & 0x38) >> 3))
                    {
                    case 0://离线
                        m_FireList->SetItemText(startnum + i, 2, "离线");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 255, 255));
                        continue;
                        break;
                    case 1://正常
                        m_FireList->SetItemText(startnum + i, 2, "正常");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(0, 255, 0));
                        break;
                    case 2://故障
                        m_FireList->SetItemText(startnum + i, 2, "故障");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 0, 0));
                        break;
                    case 3://污染
                        m_FireList->SetItemText(startnum + i, 2, "污染");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 255, 0));
                        break;
                    case 4://报警
                        if (tmp_tex == "")
                            tmp_tex = (0 == Train) ? "A节探头" : "B节探头";
                        else
                            tmp_tex += " ";
                        ss.Format("%d", i + 1);
                        tmp_tex += ss;

                        m_FireList->SetItemText(startnum + i, 2, "报警");
                        m_FireList->SetItemTextColor(2, startnum + i, RGB(255, 0, 0));
                        m_FireList->SetItemState(startnum + i, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
                        //联动
                        if ((FireToCh[i] - 1) >= 0)
                        {
                            theApp.m_FireCH[(FireToCh[i] - 1)] = 1;
                        }
                        WarFlag = true;
                        break;
                    default:
                        m_FireList->SetItemText(startnum + i, 2, "未知");
                        m_FireList->SetItemTextColor(2, i, RGB(255, 0, 0));
                        break;
                    }
                }

                //防火报警状态
                if (WarFlag)
                {
                    theApp.m_FireWarFlag = 1;
                    TRACE("m_FireWarFlag = 1\n");
                    if (tmp_tex != "") {
                        tmp_tex += "报警";
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
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    if (nIDEvent == 201)
    {
        for (int i = 0; i < 4; i++)
        {
            if (lastDataTime[i] > 15)	//当超过15秒没有收到该节车厢数据时，初始化该节车厢列表
            {
                //初始化列表
                if (i < 2)
                {
                    for (int j = i * 4; j < i * 4 + 4; j++)		//本车
                    {
                        m_FireListB.SetItemText(j, 1, "无");
                        m_FireListB.SetItemText(j, 2, "无");
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

    // TODO:  在此更改 DC 的任何特性

    if (nCtlColor == CTLCOLOR_DLG)      //对话框颜色  
        return m_brush;

    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  
        hbr = (HBRUSH)m_brush;
    }
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}


void CFireMsgDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码

    //AB节 字
    CFont font1, * oldfont;
    int textSize = 200, ox = 40, oy = 20;

    dc.SetTextColor(RGB(255, 255, 255));										//设置字体颜色
    font1.CreatePointFont(textSize, _T("黑体"), &dc);						//设置字体

    oldfont = dc.SelectObject(&font1);										//选择该字体进入PDC
    dc.SetBkMode(TRANSPARENT);
    // 不为绘图消息调用 CDialogEx::OnPaint()
}
