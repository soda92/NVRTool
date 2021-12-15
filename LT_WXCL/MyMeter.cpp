#include "stdafx.h"
#include "MyMeter.h"
#include "math.h"

MyMeter::~MyMeter()
{
}

MyMeter::MyMeter(int ox_, int oy_, int R_, int max_, int cut_, CString unit_, CString name_, CString name1_, int num1_)
//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,测量单位unit,测量项目名name，项目1名，项目1值
{
	//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
	ox = ox_;						//表盘圆心x坐标
	oy = oy_;						//表盘圆心y坐标
	R = R_;							//表盘半径
	max = max_;						//表的最大量程
	cut = cut_;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
	unit = unit_;					//测量单位
	name = name_;					//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
	name1 = name1_;					//左边方框 项目名
	num1 = num1_;					//左边项目 数值

	num_color = RGB(0, 0, 0);		//表刻度数颜色
	word_color = RGB(0, 0, 0);		//字颜色
	word_size = 0.9 * R;				//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
	hand_type = 1;					//指针类型 1直线，2三角形
	rect_width = 0.6 * R;			//信息表宽
	rect_height = 0.25 * R;			//信息表高
	METER_VALUE = 0;				//测量值
	mode = 0;						//使用样式 0默认 样式1 2 3
	red_area = 0;					//警戒区域 0为无

	//画笔
	pen[0].size = 3;				//0 矩形
	pen[0].color = RGB(0, 0, 0);

	pen[1].size = 2;				//1 圆弧
	pen[1].color = RGB(0, 0, 0);

	pen[2].size = 1;				//2 小刻度
	pen[2].color = RGB(0, 0, 0);

	pen[3].size = 2;				//3 大刻度
	pen[3].color = RGB(0, 0, 0);

	pen[4].size = R / 100;			//4 指针
	pen[4].color = RGB(255, 0, 0);

	pen[5].size = 10;				//5	指针根
	pen[5].color = RGB(255, 0, 0);

}

MyMeter::MyMeter(int ox_, int oy_, int R_, int mode_)//参数：CDC指针，圆心坐标x，圆心坐标y，表样式mode（0默认，1样式一，2样式二，3样式三）
{
	//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
	ox = ox_;						//表盘圆心x坐标
	oy = oy_;						//表盘圆心y坐标
	R = R_;							//表盘半径
	max = 2500;						//表的最大量程
	cut = 5;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
	unit = _T("kPa");				//测量单位
	name = _T("压力值");			//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
	name1 = _T("表号");				//左边方框 项目名
	num1 = 1024;					//左边项目 数值

	num_color = RGB(0, 0, 0);		//表刻度数颜色
	word_color = RGB(0, 0, 0);		//字颜色
	word_size = 0.9 * R;				//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
	hand_type = 1;					//指针类型 1直线，2三角形
	rect_width = 0.6 * R;			//信息表宽
	rect_height = 0.25 * R;			//信息表高
	METER_VALUE = 0;				//测量值
	mode = 0;						//使用样式 0默认 样式1 2 3
	red_area = 0;					//警戒区域 0为无

	//画笔
	pen[0].size = 3;				//0 矩形
	pen[0].color = RGB(0, 0, 0);

	pen[1].size = 2;				//1 圆弧
	pen[1].color = RGB(0, 0, 0);

	pen[2].size = 1;				//2 小刻度
	pen[2].color = RGB(0, 0, 0);

	pen[3].size = 2;				//3 大刻度
	pen[3].color = RGB(0, 0, 0);

	pen[4].size = R / 100;			//4 指针
	pen[4].color = RGB(255, 0, 0);

	pen[5].size = 10;				//5	指针根
	pen[5].color = RGB(255, 0, 0);

	use_mode(mode_);
}

MyMeter::MyMeter()//默认值构造函数  使用默认样式
{
	//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
	ox = 400;						//表盘圆心x坐标
	oy = 240;						//表盘圆心y坐标
	R = 200;						//表盘半径
	max = 2500;						//表的最大量程
	cut = 5;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
	unit = _T("kPa");				//测量单位
	name = _T("压力值");			//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
	name1 = _T("表号");				//左边方框 项目名
	num1 = 1024;					//左边项目 数值

	num_color = RGB(0, 0, 0);		//表刻度数颜色
	word_color = RGB(0, 0, 0);		//字颜色
	word_size = 0.9 * R;				//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
	hand_type = 1;					//指针类型 1直线，2三角形
	rect_width = 0.6 * R;			//信息表宽
	rect_height = 0.25 * R;			//信息表高
	METER_VALUE = 0;				//测量值
	mode = 0;						//使用样式 0默认 样式1 2 3
	red_area = 0;					//警戒区域 0为无

	//画笔
	pen[0].size = 3;				//0 矩形
	pen[0].color = RGB(0, 0, 0);

	pen[1].size = 2;				//1 圆弧
	pen[1].color = RGB(0, 0, 0);

	pen[2].size = 1;				//2 小刻度
	pen[2].color = RGB(0, 0, 0);

	pen[3].size = 2;				//3 大刻度
	pen[3].color = RGB(0, 0, 0);

	pen[4].size = R / 100;			//4 指针
	pen[4].color = RGB(255, 0, 0);

	pen[5].size = 10;				//5	指针根
	pen[5].color = RGB(255, 0, 0);
}

void MyMeter::DrawMeter(CDC* pDC_, double METER_VALUE)	//参数：测量值
{
	// TODO: 在此处添加实现代码.
	pDC = pDC_;
	CDC* mpDC = pDC;

	//画笔数组
	CPen* m_pen[6];
	/*= {
			{PS_SOLID, pen[0].size, pen[0].color},		//0 矩形画笔
			{PS_SOLID, pen[1].size, pen[1].color},		//1 圆弧画笔
			{PS_SOLID, pen[2].size, pen[2].color},		//2 小刻度
			{PS_SOLID, pen[3].size, pen[3].color},		//3 大刻度
			{PS_SOLID, pen[4].size, pen[4].color},		//4 指针
			{PS_SOLID, pen[5].size, pen[5].color},		//5	指针根
	};*/

	for (int i = 0; i < 6; i++)
	{
		m_pen[i] = new CPen(PS_SOLID, pen[i].size, pen[i].color);
	}

	//表参数
	int xStart = ox;				//圆心x
	int yStart = oy;				//圆心y
	double r = R;					//表盘半径
	double PI = 3.1415926;			//圆周率

	double MAX = max;					//量程
	double cut_num = cut;				//表盘分割份数 
	double all_num = cut_num * 10;		//总刻度数
	double lenth = 0.08 * r;				//刻度长度
	double MIN = MAX / 10 / cut_num;	//最刻度值

	//警戒区域
	int red = red_area / MAX * all_num;
	CPen redpen1(PS_SOLID, pen[1].size, RGB(255, 0, 0));
	CPen redpen2(PS_SOLID, pen[2].size, RGB(255, 0, 0));
	CPen redpen3(PS_SOLID, pen[3].size, RGB(255, 0, 0));

	//绘制 圆弧
	mpDC->SelectObject(m_pen[1]);																//选择画笔
	mpDC->MoveTo(xStart + (r - pen[1].size / 3) / 2, yStart + (r - pen[1].size / 3) * 0.86602540);	//起始点
	mpDC->AngleArc(xStart, yStart, (r - pen[1].size / 3), -60, 300);							//画弧
	if (red_area)
	{
		mpDC->SelectObject(&redpen1);																	//选择画笔
		mpDC->MoveTo(xStart + (r - pen[1].size / 3) / 2, yStart + (r - pen[1].size / 3) * 0.86602540);	//起始点
		mpDC->AngleArc(xStart, yStart, (r - pen[1].size / 3), -60, 300 * (1 - red_area / MAX));			//画红弧
	}

	//画刻度  和 数值
	int x, y;														//临时变量，存放坐标
	CFont font;
	CSize size;														//字符串大小												
	CString value_num;												//刻度值字符串
	VERIFY(font.CreatePointFont(1.4 * r, _T("黑体"), mpDC));				//设置字体
	CFont* oldfont = mpDC->SelectObject(&font);						//选择该字体进入PDC
	mpDC->SetBkMode(TRANSPARENT);									//设置字体背景透明
	mpDC->SetTextColor(num_color);									//设置字体颜色

	for (int i = 0; i <= all_num; i++) {
		double angle = 330 - (double)i * 300 / all_num;				//计算第i个刻度的角度  = 刻度盘0刻度的角度330 - 第i个刻度与0刻度的夹角
		double Radians = angle * PI / 180;							//计算弧度 = 角度 * Pi / 180
		if (i % 5 == 0) {											/*每隔5个刻度线加长一根*/
			if (red_area != 0 && i >= red)
			{
				mpDC->SelectObject(&redpen3);						//选择红画笔
			}
			else
			{
				mpDC->SelectObject(m_pen[3]);						//选择画笔
			}
			mpDC->MoveTo(xStart + r * sin(Radians), yStart + r * cos(Radians));
			mpDC->LineTo(xStart + (r - 2 * lenth) * sin(Radians), yStart + (r - 2 * lenth) * cos(Radians));
		}
		else {
			if (red_area != 0 && i >= red)
			{
				mpDC->SelectObject(&redpen2);								//选择红画笔
			}
			else
			{
				mpDC->SelectObject(m_pen[2]);								//选择画笔
			}
			mpDC->MoveTo(xStart + r * sin(Radians), yStart + r * cos(Radians));
			mpDC->LineTo(xStart + (r - lenth) * sin(Radians), yStart + (r - lenth) * cos(Radians));
		}
		if (i % 10 == 0)													/*每隔10个刻度显示一个刻度*/
		{
			if (red_area != 0 && i >= red)
			{
				mpDC->SetTextColor(RGB(255, 0, 0));							//设置字体颜色为红
			}
			value_num.Format(_T("%d"), i * (int)MIN);
			size = mpDC->GetTextExtent(value_num, value_num.GetLength());	//计算字符串长和高
			x = xStart - (size.cx / 2) + (int)((double)(r * 70 / 100) * sin(Radians));
			y = yStart - (size.cy / 2) + (int)((double)(r * 75 / 100) * cos(Radians));
			mpDC->TextOut(x, y, value_num);
		}
	}
	mpDC->SetTextColor(num_color);												//设置字体颜色
	mpDC->SelectObject(oldfont);												//还原字体

	//画指针	(需要知道当前测量值 METER_VALUE)
	double angle = 330 - (METER_VALUE * 300 / MAX);								//计算角度
	double Radians = angle * PI / 180;											//计算弧度     弧度=角度*Pi/180
	mpDC->SelectObject(m_pen[4]);	//选择画笔
	double sin1 = sin(Radians);
	double cos1 = cos(Radians);
	int r2;
	CRgn rgn2;
	CBrush brush, * oldbrush = NULL;
	CPoint pt[3];
	CRect* rect1 = NULL;
	CRect* rect2 = NULL;
	CRect* rect3 = NULL;
	CRect* rect4 = NULL;
	CRect* rect5 = NULL;

	switch (hand_type)
	{
	case 1:	//直线型指针
	{	mpDC->MoveTo(xStart + (r * 0.8) * sin1, yStart + (r * 0.8) * cos1);		//指针起始点
	mpDC->LineTo(xStart, yStart);											//画指针
	//指针根
	r2 = 0.02 * r;															//小圆尺寸
	mpDC->SelectObject(m_pen[5]);											//选择画笔
	rect1 = new CRect(xStart + r2, yStart + r2, xStart - r2, yStart - r2);	//设置椭圆的限定矩形（左上角坐标，右下角坐标）
	mpDC->Ellipse(*rect1);													//画椭圆
	break;
	}
	case 2:	//三角形指针
	{
		r2 = 0.08 * r;//小圆尺寸
		pt[0].x = (int)(xStart + ((r - pen[4].size) * 0.8) * sin1);
		pt[0].y = (int)(yStart + ((r - pen[4].size) * 0.8) * cos1);
		pt[1].x = (int)(xStart + cos1 * r2);
		pt[1].y = (int)(yStart - sin1 * r2);
		pt[2].x = (int)(xStart - cos1 * r2);
		pt[2].y = (int)(yStart + sin1 * r2);

		mpDC->Polygon(pt, 3);
		rgn2.CreatePolygonRgn(pt, 3, ALTERNATE/*WINDING*/);						//创建区域         
		brush.CreateSolidBrush(pen[4].color);									//创建画刷        
		mpDC->FillRgn(&rgn2, &brush);											//填充区域

		//指针根
		oldbrush = mpDC->SelectObject(&brush);									//选择新画刷，并存储旧画刷
		rect1 = new CRect(xStart + r2, yStart + r2, xStart - r2, yStart - r2);	//设置椭圆的限定矩形（左上角坐标，右下角坐标）
		mpDC->Ellipse(rect1);													//画椭圆
		mpDC->SelectObject(oldbrush);											//返回旧画刷
		break;
	}
	default:
		break;
	}



	//表盘中央 测量单位显示
	size = mpDC->GetTextExtent(unit, unit.GetLength());							//计算字符串长和高
	if (mode != 4)mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.3, unit);				//显示 单位
	CString string1, string2;
	CFont font1;
	CFont font2;
	int rectwith;				//宽
	int recthigh;				//高
	int rect_y;					//上边界距圆心高度差	
	int rectx;
	TEXTMETRIC tm;				//字符串宽度长度信息

	switch (mode)
	{
	case 0:
	case 3:
	case 1:
	{
		//矩形参数
		rectwith = rect_width;				//宽
		recthigh = rect_height;				//高
		rect_y = 1.2 * (int)r;				//上边界距圆心高度差	

		//绘制矩形
		mpDC->SelectObject(m_pen[0]);																//选择画笔
		rect2 = new CRect(xStart - rectwith, yStart + rect_y, xStart, yStart + rect_y + recthigh);	//左矩形 (左上角坐标，右下角坐标)
		rect3 = new CRect(xStart, yStart + rect_y, xStart + rectwith, yStart + rect_y + recthigh);	//右
		mpDC->Rectangle(rect2);																		//绘制
		mpDC->Rectangle(rect3);

		//信息显示
		mpDC->SetTextColor(word_color);												//设置字体颜色
		font1.CreatePointFont((int)(word_size), _T("黑体"), mpDC);					//设置字体
		oldfont = mpDC->SelectObject(&font1);										//选择该字体进入PDC
		string1.Format(_T("%d"), (int)METER_VALUE);									//数值转化成字符串
		string2.Format(_T("%d"), num1);

		mpDC->TextOut(xStart - rect_width + 10, yStart + r, name1);					//显示 左边项目名
		mpDC->TextOut(xStart - rect_width + 10, yStart + 1.25 * r, string2);			//显示 左边项目值
		mpDC->TextOut(xStart + 10, yStart + 1.25 * r, string1);						//显示 当前值  value_num字符串（左上角x坐标，左上角y坐标，字符串）
		mpDC->TextOut(xStart + 10, yStart + r, name);								//显示 测量项目名

		mpDC->SelectObject(oldfont);												//还原字体

		break;
	}
	case 2:
	{
		rectx = 1.1 * r;
		mpDC->SelectObject(m_pen[0]);														//选择画笔
		mpDC->MoveTo(xStart - rectx, yStart + 1.3 * r);										//上边框
		mpDC->LineTo(xStart - rectx, yStart - 1.1 * r - 1);
		mpDC->LineTo(xStart + rectx - 1, yStart - 1.1 * r);
		mpDC->LineTo(xStart + rectx - 1, yStart + 1.3 * r);
		rect2 = new CRect(xStart - rectx, yStart + r, xStart - 0.2 * r, yStart + 1.3 * r);			//1左
		rect3 = new CRect(xStart - 0.2 * r, yStart + r, xStart + 1.1 * r, yStart + 1.3 * r);			//1右
		rect4 = new CRect(xStart - rectx, yStart + 1.3 * r - 1, xStart - 0.2 * r, yStart + 1.6 * r);//2左
		rect5 = new CRect(xStart - 0.2 * r, yStart + 1.3 * r - 1, xStart + 1.1 * r, yStart + 1.6 * r);//2右

		mpDC->Rectangle(rect2);																//绘制
		mpDC->Rectangle(rect3);
		mpDC->Rectangle(rect4);
		mpDC->Rectangle(rect5);

		//信息显示
		mpDC->SetTextColor(word_color);												//设置字体颜色
		font2.CreatePointFont((int)(word_size), _T("黑体"), mpDC);					//设置字体
		oldfont = mpDC->SelectObject(&font2);										//选择该字体进入PDC
		string1.Format(_T("%d"), (int)METER_VALUE);									//数值转化成字符串
		string2.Format(_T("%d"), num1);

		mpDC->TextOut(xStart - 0.9 * rectx, yStart + 1.1 * r, name1);					//显示 1左 项目名
		mpDC->TextOut(xStart, yStart + 1.1 * r, string2);							//显示 1右 项目值
		mpDC->TextOut(xStart - 0.9 * rectx, yStart + 1.4 * r, name);					//显示 2左 测量项目名
		mpDC->TextOut(xStart, yStart + 1.4 * r, string1);							//显示 2右 当前值  value_num字符串（左上角x坐标，左上角y坐标，字符串）

		mpDC->SelectObject(oldfont);												//还原字体

		break;
	}
	case 4:
	{
		//信息显示
		mpDC->SetTextColor(word_color);												//设置字体颜色
		font1.CreatePointFont((int)(word_size), _T("黑体"), mpDC);					//设置字体
		oldfont = mpDC->SelectObject(&font1);										//选择该字体进入PDC
		string1.Format(_T("%d"), (int)METER_VALUE);									//数值转化成字符串

		size = mpDC->GetTextExtent(unit, unit.GetLength());							//计算字符串长和高
		mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.4, unit);				//显示 单位

		mpDC->SetTextColor(RGB(0, 0, 255));											//设置测量值颜色
		size = mpDC->GetTextExtent(string1, string1.GetLength());					//计算字符串长和高
		mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.2, string1);				//显示 当前值  value_num字符串（左上角x坐标，左上角y坐标，字符串）
		mpDC->SetTextColor(word_color);

		size = mpDC->GetTextExtent(name, name.GetLength());							//计算字符串长和高
		mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.8, name);					//显示 测量项目名


		mpDC->SelectObject(oldfont);												//还原字体
		break;
	}
	}


	//释放资源
	for (int i = 0; i < 6; i++)
	{
		m_pen[i]->DeleteObject();
		delete m_pen[i];
	}
	redpen1.DeleteObject();
	redpen2.DeleteObject();
	redpen3.DeleteObject();

	font.DeleteObject();
	oldfont->DeleteObject();
	rgn2.DeleteObject();
	brush.DeleteObject();
	delete rect1;
	delete rect2;
	delete rect3;
	delete rect4;
	delete rect5;
	font1.DeleteObject();
	font2.DeleteObject();

	return;
}

void MyMeter::DrawMeter(CDC* pDC_)
{
	DrawMeter(pDC_, 0);
}

void MyMeter::set_site(int ox, int oy)
{
	this->ox = ox;
	this->oy = oy;
}

void MyMeter::set_R(int R)
{
	this->R = R;
}

void MyMeter::set_range(int max, int cut, CString unit)
{
	this->max = max;
	this->cut = cut;
	this->unit = unit;
}

void MyMeter::set_item(CString name, CString name1, int num1)
{
	this->name = name;
	this->name1 = name1;
	this->num1 = num1;
}

void MyMeter::set_pen(int num, int size, COLORREF color)
{
	this->pen[num].size = size;
	this->pen[num].color = color;
}

void MyMeter::set_wordcolor(COLORREF color)
{
	this->word_color = color;
}

void MyMeter::set_numcolor(COLORREF color)
{
	this->num_color = color;
}

void MyMeter::set_handtype(int hand_type)
{
	this->hand_type = hand_type;
}

void MyMeter::set_wordsize(int size)
{
	this->word_size = size;
}

void MyMeter::set_rect(int width, int height)
{
	this->rect_width = width;
	this->rect_height = height;
}

void MyMeter::set_redarea(int red_area)
{
	this->red_area = red_area;
}

void MyMeter::use_mode(int num)
{
	mode = num;
	switch (num)
	{
	case 1:		//样式一
	{
		//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
		max = 1400;						//表的最大量程
		cut = 7;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
		unit = _T("kPa");				//测量单位
		name = _T("压力值");			//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
		name1 = _T("压力表");			//左边方框 项目名
		num1 = 1001;					//左边项目 数值
		num_color = RGB(0, 0, 0);		//表刻度数颜色
		word_color = RGB(0, 0, 0);		//字颜色
		word_size = 1.1 * R;				//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
		hand_type = 1;					//指针类型 1直线，2三角形
		rect_width = R;					//信息表宽
		rect_height = 0.3 * R;			//信息表高
		METER_VALUE = 0;				//测量值

		//画笔
		pen[0].size = 3;				//0 矩形
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = 2;				//1 圆弧
		pen[1].color = RGB(0, 0, 0);

		pen[2].size = 1;				//2 小刻度
		pen[2].color = RGB(0, 0, 0);

		pen[3].size = 2;				//3 大刻度
		pen[3].color = RGB(0, 0, 0);

		pen[4].size = R / 50;			//4 指针
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	指针根
		pen[5].color = RGB(255, 0, 0);

		//DrawString(150, 50, _T("样式一"), 180, RGB(0,0,255));			//测试用

		break;
	}
	case 2:		//样式二
	{
		//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
		max = 1400;						//表的最大量程
		cut = 7;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
		unit = _T("kPa");				//测量单位
		name = _T("压力值");			//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
		name1 = _T("压力表");			//左边方框 项目名
		num1 = 1002;					//左边项目 数值
		num_color = RGB(0, 0, 0);		//表刻度数颜色
		word_color = RGB(0, 0, 0);		//字颜色
		word_size = 1.1 * R;				//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
		hand_type = 2;					//指针类型 1直线，2三角形
		METER_VALUE = 0;				//测量值

		//画笔
		pen[0].size = 2;				//0 矩形
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = R / 5;				//1 圆弧
		pen[1].color = RGB(45, 171, 224);

		pen[2].size = 1;				//2 小刻度
		pen[2].color = RGB(255, 255, 255);

		pen[3].size = 2;				//3 大刻度
		pen[3].color = RGB(255, 255, 255);

		pen[4].size = R / 50;			//4 指针
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	指针根
		pen[5].color = RGB(255, 0, 0);

		//pen[0] = { 2, RGB(0, 0, 0) };			//0 矩形
		//pen[1] = { 30, RGB(45, 171, 224) };	//1 圆弧
		//pen[2] = { 1, RGB(255, 255, 255) };	//2 小刻度
		//pen[3] = { 2, RGB(255, 255, 255) };	//3 大刻度
		//pen[4] = { 5, RGB(255, 0, 0) };		//4 指针
		//pen[5] = { 10, RGB(255, 0, 0) };		//5	指针根

		//DrawString(450, 50, _T("样式二"), 180, RGB(0, 0, 255));			//测试用
		break;
	}
	case 3:		//样式三
	{
		//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
		max = 300;						//表的最大量程
		cut = 15;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
		unit = _T("km/h");				//测量单位
		name = _T("当前时速");			//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
		name1 = _T("车号");				//左边方框 项目名
		num1 = 1003;					//左边项目 数值
		num_color = RGB(0, 0, 0);		//表刻度数颜色
		word_color = RGB(0, 0, 0);		//字颜色
		word_size = 1.1 * R;				//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
		hand_type = 1;					//指针类型 1直线，2三角形
		rect_width = R;					//信息表宽
		rect_height = 0.3 * R;				//信息表高
		METER_VALUE = 0;				//测量值
		red_area = 200;					//警戒值

		//画笔
		pen[0].size = 3;				//0 矩形
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = 2;				//1 圆弧
		pen[1].color = RGB(0, 0, 0);

		pen[2].size = 1;				//2 小刻度
		pen[2].color = RGB(0, 0, 0);

		pen[3].size = 2;				//3 大刻度
		pen[3].color = RGB(0, 0, 0);

		pen[4].size = R / 50;			//4 指针
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	指针根
		pen[5].color = RGB(255, 0, 0);

		//DrawString(750, 50, _T("样式三"), 180, RGB(0, 0, 255));			//测试用	

		break;
	}
	case 4:
	{
		//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
		max = 1400;						//表的最大量程
		cut = 7;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
		unit = _T("kPa");				//测量单位
		name = _T("压力值");					//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
		num_color = RGB(0, 0, 0);		//表刻度数颜色
		word_color = RGB(0, 0, 0);		//字颜色
		word_size = 1.1 * R;				//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
		hand_type = 1;					//指针类型 1直线，2三角形
		rect_width = R;					//信息表宽
		rect_height = 0.3 * R;			//信息表高
		METER_VALUE = 0;				//测量值

		//画笔
		pen[0].size = 3;				//0 矩形
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = 2;				//1 圆弧
		pen[1].color = RGB(0, 0, 0);

		pen[2].size = 1;				//2 小刻度
		pen[2].color = RGB(0, 0, 0);

		pen[3].size = 2;				//3 大刻度
		pen[3].color = RGB(0, 0, 0);

		pen[4].size = R / 50;			//4 指针
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	指针根
		pen[5].color = RGB(255, 0, 0);

		//DrawString(150, 50, _T("样式四"), 180, RGB(0,0,255));			//测试用

		break;
	}
	default:
		break;
	}
}

void MyMeter::DrawString(int x, int y, CString str1, int size, COLORREF color)
{
	CFont font1;
	pDC->SetTextColor(color);											//设置字体颜色
	font1.CreatePointFont(size, _T("黑体"), pDC);						//设置字体
	CFont* oldfont1 = pDC->SelectObject(&font1);						//选择该字体进入PDC
	pDC->TextOut(x, y, str1);
	pDC->SelectObject(oldfont1);										//还原字体
}

void MyMeter::DrawRect(int x, int y, int width, int height)
{
	//绘制矩形
	pDC->SelectObject(&pen[0]);											//选择画笔
	CRect rect1(x, y, x + width, y + height);								//左矩形 (左上角坐标，右下角坐标)
	pDC->Rectangle(&rect1);												//绘制
}
