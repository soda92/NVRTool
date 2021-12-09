/*
	测量表对象：用于绘制测量表，如压力表，气压表，时速表，电压表，电流表等
	包括：表盘，指针，和信息列表三部分
	MyMeter类的使用：
		1、有三个构造函数可以使用，参数最多的是自定义表类型，最少的是使用默认值快速绘表，另外一个是使用已有的三种样式快速绘表
		2、可以在声明变量后使用各种set_函数设置表属性
		3、设置颜色的时候需要对应区域
		4、当前测量值（即指针的指向）需要在画表的时候传递，否则默认指向0
	注：在设置量程和分割分数的时候 分割分数要能被量程整除，根据量程设定合理的分割分数
*/

#pragma once

typedef struct
{
	int size;
	COLORREF color;
}PEN;

class MyMeter
{
public:
	virtual ~MyMeter();

	MyMeter(int ox_, int oy_, int R_, int max_, int cut_, CString unit_, CString name_, CString name1_, int num1_); //构造函数
	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,测量单位unit,测量项目名name，项目1名，项目1值

	MyMeter(int ox_, int oy_, int R_, int mode_);//样式构造函数 设置位置和样式
	//参数：CDC指针，圆心坐标x，圆心坐标y，表样式mode（0默认，1样式一，2样式二，3样式三）

	MyMeter();//默认值构造函数  使用默认值

	void DrawMeter(CDC *pDC_, double METER_VALUE);		//画表（CDC指针，当前测量值）
	void DrawMeter(CDC *pDC_);							//画表 （测量值为0）

	void set_site(int ox,int oy);						//设置 中心点坐标（x坐标，y坐标）
	void set_R(int R);									//设置 半径R
	void set_range(int max,int cut,CString unit);		//设置 量程、量程分割份数、测量单位
	void set_item(CString name,CString name1,int num1);	//设置 测量值项目名、左1项目名、左1项目值
	void set_pen(int num, int size, COLORREF color);	//设置 各板块颜色和粗细  板块号、粗细、颜色  0矩形1圆弧2小刻度3大刻度4指针5指针根
	void set_wordcolor(COLORREF color);					//设置 字的颜色
	void set_numcolor(COLORREF color);					//设置 字的颜色
	void set_handtype(int hand_type);					//设置 指针类型 1直线型，2三角形
	void set_wordsize(int size);						//设置 字体大小
	void set_rect(int width,int height);				//设置 矩形长宽
	void set_redarea(int red_area);						//设置 警戒值

	void use_mode(int num);								//选择 样式
private:
	CDC *pDC;
	//默认值400, 240, 200, 2500, 5, _T("kPa"),_T("压力值"),_T("表号"),1024
	int ox;							//表盘圆心x坐标
	int oy;							//表盘圆心y坐标
	int R;							//表盘半径
	int max;						//表的最大量程
	int cut;						//量程分成几份（最好能被最大量程整除） 表盘的最小刻度值=max/cut/10
	CString unit;					//测量单位
	CString name;					//测量项目名);	//参数：CDC指针,圆心x坐标,圆心y坐标,半径R,最大量程max,量程份数cut,当前值METER_VALUE,单位unit,项目名name
	CString name1;					//左边方框 项目名
	int num1;						//左边项目 数值
	COLORREF num_color ;			//表刻度数颜色
	COLORREF word_color;			//字颜色
	int word_size ;					//字体大小	注：以十分之一点 为单位 例：120值 等于12像素高
	int hand_type;					//指针类型 1直线，2三角形
	int rect_width;					//信息表宽
	int rect_height ;				//信息表高
	double METER_VALUE;				//测量值
	int mode;						//使用样式 0默认 样式1 2 3
	int red_area;					//警戒区域 0为无

	//画笔
	PEN pen[6];

private:
	void DrawString(int x, int y, CString str1, int size, COLORREF color);
	void DrawRect(int x,int y, int width,int height);
};

/*
													》》》》使用示例《《《《
	//MyMeter mymeter0();			//默认样式
	//mymeter0.DrawMeter(pDC);

	//直接调用已有样式
	MyMeter mymeter1(200,240,1);	//样式一
	mymeter1.DrawMeter(pDC);

	MyMeter mymeter2(500,240,2);	//样式二
	mymeter2.DrawMeter(pDC);

	MyMeter mymeter3(800, 240, 3);	//样式二
	mymeter3.DrawMeter(pDC);

	//自定义样式
	
	MyMeter mymeter1(200, 240, 130, 2400, 8, _T("kPa"),_T("压力值"),_T("表号"),1001);
	mymeter1.set_pen(4,2,RGB(255,0,0));				//设置 指针宽度为2，颜色为红色
	mymeter1.set_rect(120,40);						//设置 信息列表长和宽
	// mymeter1.set_wordsize(180);					//设置 信息列表字体大小
	mymeter1.DrawMeter(pDC,rand() % 2400);			//画图（参数：当前测量值）

	MyMeter mymeter2(500, 240, 130, 1000, 10, _T("kPa"), _T("压力值"), _T("表号"), 1002);
	mymeter2.set_pen(3,2,RGB(0,255,0));				//设置 大刻度颜色为绿色
	mymeter2.DrawMeter(pDC,rand() % 1000);			

	MyMeter mymeter3(800, 240, 130, 500, 5, _T("kPa"), _T("压力值"), _T("表号"), 1003);
	mymeter3.set_pen(0, 1, RGB(0, 0, 255));			//设置 信息列表矩形边框粗细为1，颜色为蓝
	mymeter3.set_pen(1, 30, RGB(45, 171,224 ));		//设置 表盘圆弧粗细为30，颜色为RGB(45, 171,224 )
	mymeter3.set_pen(2, 1, RGB(255, 255, 255));		//设置 小刻度粗细为1，颜色为白
	mymeter3.set_pen(3, 1, RGB(255, 255, 255));		//设置 大刻度粗细为1，颜色为白
	mymeter3.set_pen(4, 5, RGB(0, 0, 0));			//设置 指针粗细为5，颜色为黑
	mymeter3.set_handtype(2);						//设置 指针类型为三角形
	mymeter3.DrawMeter(pDC,rand() % 500);
	
*/



