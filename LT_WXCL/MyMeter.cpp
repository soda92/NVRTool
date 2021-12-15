#include "stdafx.h"
#include "MyMeter.h"
#include "math.h"

MyMeter::~MyMeter()
{
}

MyMeter::MyMeter(int ox_, int oy_, int R_, int max_, int cut_, CString unit_, CString name_, CString name1_, int num1_)
//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,������λunit,������Ŀ��name����Ŀ1������Ŀ1ֵ
{
	//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
	ox = ox_;						//����Բ��x����
	oy = oy_;						//����Բ��y����
	R = R_;							//���̰뾶
	max = max_;						//����������
	cut = cut_;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
	unit = unit_;					//������λ
	name = name_;					//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
	name1 = name1_;					//��߷��� ��Ŀ��
	num1 = num1_;					//�����Ŀ ��ֵ

	num_color = RGB(0, 0, 0);		//��̶�����ɫ
	word_color = RGB(0, 0, 0);		//����ɫ
	word_size = 0.9 * R;				//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
	hand_type = 1;					//ָ������ 1ֱ�ߣ�2������
	rect_width = 0.6 * R;			//��Ϣ���
	rect_height = 0.25 * R;			//��Ϣ���
	METER_VALUE = 0;				//����ֵ
	mode = 0;						//ʹ����ʽ 0Ĭ�� ��ʽ1 2 3
	red_area = 0;					//�������� 0Ϊ��

	//����
	pen[0].size = 3;				//0 ����
	pen[0].color = RGB(0, 0, 0);

	pen[1].size = 2;				//1 Բ��
	pen[1].color = RGB(0, 0, 0);

	pen[2].size = 1;				//2 С�̶�
	pen[2].color = RGB(0, 0, 0);

	pen[3].size = 2;				//3 ��̶�
	pen[3].color = RGB(0, 0, 0);

	pen[4].size = R / 100;			//4 ָ��
	pen[4].color = RGB(255, 0, 0);

	pen[5].size = 10;				//5	ָ���
	pen[5].color = RGB(255, 0, 0);

}

MyMeter::MyMeter(int ox_, int oy_, int R_, int mode_)//������CDCָ�룬Բ������x��Բ������y������ʽmode��0Ĭ�ϣ�1��ʽһ��2��ʽ����3��ʽ����
{
	//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
	ox = ox_;						//����Բ��x����
	oy = oy_;						//����Բ��y����
	R = R_;							//���̰뾶
	max = 2500;						//����������
	cut = 5;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
	unit = _T("kPa");				//������λ
	name = _T("ѹ��ֵ");			//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
	name1 = _T("���");				//��߷��� ��Ŀ��
	num1 = 1024;					//�����Ŀ ��ֵ

	num_color = RGB(0, 0, 0);		//��̶�����ɫ
	word_color = RGB(0, 0, 0);		//����ɫ
	word_size = 0.9 * R;				//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
	hand_type = 1;					//ָ������ 1ֱ�ߣ�2������
	rect_width = 0.6 * R;			//��Ϣ���
	rect_height = 0.25 * R;			//��Ϣ���
	METER_VALUE = 0;				//����ֵ
	mode = 0;						//ʹ����ʽ 0Ĭ�� ��ʽ1 2 3
	red_area = 0;					//�������� 0Ϊ��

	//����
	pen[0].size = 3;				//0 ����
	pen[0].color = RGB(0, 0, 0);

	pen[1].size = 2;				//1 Բ��
	pen[1].color = RGB(0, 0, 0);

	pen[2].size = 1;				//2 С�̶�
	pen[2].color = RGB(0, 0, 0);

	pen[3].size = 2;				//3 ��̶�
	pen[3].color = RGB(0, 0, 0);

	pen[4].size = R / 100;			//4 ָ��
	pen[4].color = RGB(255, 0, 0);

	pen[5].size = 10;				//5	ָ���
	pen[5].color = RGB(255, 0, 0);

	use_mode(mode_);
}

MyMeter::MyMeter()//Ĭ��ֵ���캯��  ʹ��Ĭ����ʽ
{
	//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
	ox = 400;						//����Բ��x����
	oy = 240;						//����Բ��y����
	R = 200;						//���̰뾶
	max = 2500;						//����������
	cut = 5;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
	unit = _T("kPa");				//������λ
	name = _T("ѹ��ֵ");			//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
	name1 = _T("���");				//��߷��� ��Ŀ��
	num1 = 1024;					//�����Ŀ ��ֵ

	num_color = RGB(0, 0, 0);		//��̶�����ɫ
	word_color = RGB(0, 0, 0);		//����ɫ
	word_size = 0.9 * R;				//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
	hand_type = 1;					//ָ������ 1ֱ�ߣ�2������
	rect_width = 0.6 * R;			//��Ϣ���
	rect_height = 0.25 * R;			//��Ϣ���
	METER_VALUE = 0;				//����ֵ
	mode = 0;						//ʹ����ʽ 0Ĭ�� ��ʽ1 2 3
	red_area = 0;					//�������� 0Ϊ��

	//����
	pen[0].size = 3;				//0 ����
	pen[0].color = RGB(0, 0, 0);

	pen[1].size = 2;				//1 Բ��
	pen[1].color = RGB(0, 0, 0);

	pen[2].size = 1;				//2 С�̶�
	pen[2].color = RGB(0, 0, 0);

	pen[3].size = 2;				//3 ��̶�
	pen[3].color = RGB(0, 0, 0);

	pen[4].size = R / 100;			//4 ָ��
	pen[4].color = RGB(255, 0, 0);

	pen[5].size = 10;				//5	ָ���
	pen[5].color = RGB(255, 0, 0);
}

void MyMeter::DrawMeter(CDC* pDC_, double METER_VALUE)	//����������ֵ
{
	// TODO: �ڴ˴����ʵ�ִ���.
	pDC = pDC_;
	CDC* mpDC = pDC;

	//��������
	CPen* m_pen[6];
	/*= {
			{PS_SOLID, pen[0].size, pen[0].color},		//0 ���λ���
			{PS_SOLID, pen[1].size, pen[1].color},		//1 Բ������
			{PS_SOLID, pen[2].size, pen[2].color},		//2 С�̶�
			{PS_SOLID, pen[3].size, pen[3].color},		//3 ��̶�
			{PS_SOLID, pen[4].size, pen[4].color},		//4 ָ��
			{PS_SOLID, pen[5].size, pen[5].color},		//5	ָ���
	};*/

	for (int i = 0; i < 6; i++)
	{
		m_pen[i] = new CPen(PS_SOLID, pen[i].size, pen[i].color);
	}

	//�����
	int xStart = ox;				//Բ��x
	int yStart = oy;				//Բ��y
	double r = R;					//���̰뾶
	double PI = 3.1415926;			//Բ����

	double MAX = max;					//����
	double cut_num = cut;				//���̷ָ���� 
	double all_num = cut_num * 10;		//�̶ܿ���
	double lenth = 0.08 * r;				//�̶ȳ���
	double MIN = MAX / 10 / cut_num;	//��̶�ֵ

	//��������
	int red = red_area / MAX * all_num;
	CPen redpen1(PS_SOLID, pen[1].size, RGB(255, 0, 0));
	CPen redpen2(PS_SOLID, pen[2].size, RGB(255, 0, 0));
	CPen redpen3(PS_SOLID, pen[3].size, RGB(255, 0, 0));

	//���� Բ��
	mpDC->SelectObject(m_pen[1]);																//ѡ�񻭱�
	mpDC->MoveTo(xStart + (r - pen[1].size / 3) / 2, yStart + (r - pen[1].size / 3) * 0.86602540);	//��ʼ��
	mpDC->AngleArc(xStart, yStart, (r - pen[1].size / 3), -60, 300);							//����
	if (red_area)
	{
		mpDC->SelectObject(&redpen1);																	//ѡ�񻭱�
		mpDC->MoveTo(xStart + (r - pen[1].size / 3) / 2, yStart + (r - pen[1].size / 3) * 0.86602540);	//��ʼ��
		mpDC->AngleArc(xStart, yStart, (r - pen[1].size / 3), -60, 300 * (1 - red_area / MAX));			//���컡
	}

	//���̶�  �� ��ֵ
	int x, y;														//��ʱ�������������
	CFont font;
	CSize size;														//�ַ�����С												
	CString value_num;												//�̶�ֵ�ַ���
	VERIFY(font.CreatePointFont(1.4 * r, _T("����"), mpDC));				//��������
	CFont* oldfont = mpDC->SelectObject(&font);						//ѡ����������PDC
	mpDC->SetBkMode(TRANSPARENT);									//�������屳��͸��
	mpDC->SetTextColor(num_color);									//����������ɫ

	for (int i = 0; i <= all_num; i++) {
		double angle = 330 - (double)i * 300 / all_num;				//�����i���̶ȵĽǶ�  = �̶���0�̶ȵĽǶ�330 - ��i���̶���0�̶ȵļн�
		double Radians = angle * PI / 180;							//���㻡�� = �Ƕ� * Pi / 180
		if (i % 5 == 0) {											/*ÿ��5���̶��߼ӳ�һ��*/
			if (red_area != 0 && i >= red)
			{
				mpDC->SelectObject(&redpen3);						//ѡ��컭��
			}
			else
			{
				mpDC->SelectObject(m_pen[3]);						//ѡ�񻭱�
			}
			mpDC->MoveTo(xStart + r * sin(Radians), yStart + r * cos(Radians));
			mpDC->LineTo(xStart + (r - 2 * lenth) * sin(Radians), yStart + (r - 2 * lenth) * cos(Radians));
		}
		else {
			if (red_area != 0 && i >= red)
			{
				mpDC->SelectObject(&redpen2);								//ѡ��컭��
			}
			else
			{
				mpDC->SelectObject(m_pen[2]);								//ѡ�񻭱�
			}
			mpDC->MoveTo(xStart + r * sin(Radians), yStart + r * cos(Radians));
			mpDC->LineTo(xStart + (r - lenth) * sin(Radians), yStart + (r - lenth) * cos(Radians));
		}
		if (i % 10 == 0)													/*ÿ��10���̶���ʾһ���̶�*/
		{
			if (red_area != 0 && i >= red)
			{
				mpDC->SetTextColor(RGB(255, 0, 0));							//����������ɫΪ��
			}
			value_num.Format(_T("%d"), i * (int)MIN);
			size = mpDC->GetTextExtent(value_num, value_num.GetLength());	//�����ַ������͸�
			x = xStart - (size.cx / 2) + (int)((double)(r * 70 / 100) * sin(Radians));
			y = yStart - (size.cy / 2) + (int)((double)(r * 75 / 100) * cos(Radians));
			mpDC->TextOut(x, y, value_num);
		}
	}
	mpDC->SetTextColor(num_color);												//����������ɫ
	mpDC->SelectObject(oldfont);												//��ԭ����

	//��ָ��	(��Ҫ֪����ǰ����ֵ METER_VALUE)
	double angle = 330 - (METER_VALUE * 300 / MAX);								//����Ƕ�
	double Radians = angle * PI / 180;											//���㻡��     ����=�Ƕ�*Pi/180
	mpDC->SelectObject(m_pen[4]);	//ѡ�񻭱�
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
	case 1:	//ֱ����ָ��
	{	mpDC->MoveTo(xStart + (r * 0.8) * sin1, yStart + (r * 0.8) * cos1);		//ָ����ʼ��
	mpDC->LineTo(xStart, yStart);											//��ָ��
	//ָ���
	r2 = 0.02 * r;															//СԲ�ߴ�
	mpDC->SelectObject(m_pen[5]);											//ѡ�񻭱�
	rect1 = new CRect(xStart + r2, yStart + r2, xStart - r2, yStart - r2);	//������Բ���޶����Σ����Ͻ����꣬���½����꣩
	mpDC->Ellipse(*rect1);													//����Բ
	break;
	}
	case 2:	//������ָ��
	{
		r2 = 0.08 * r;//СԲ�ߴ�
		pt[0].x = (int)(xStart + ((r - pen[4].size) * 0.8) * sin1);
		pt[0].y = (int)(yStart + ((r - pen[4].size) * 0.8) * cos1);
		pt[1].x = (int)(xStart + cos1 * r2);
		pt[1].y = (int)(yStart - sin1 * r2);
		pt[2].x = (int)(xStart - cos1 * r2);
		pt[2].y = (int)(yStart + sin1 * r2);

		mpDC->Polygon(pt, 3);
		rgn2.CreatePolygonRgn(pt, 3, ALTERNATE/*WINDING*/);						//��������         
		brush.CreateSolidBrush(pen[4].color);									//������ˢ        
		mpDC->FillRgn(&rgn2, &brush);											//�������

		//ָ���
		oldbrush = mpDC->SelectObject(&brush);									//ѡ���»�ˢ�����洢�ɻ�ˢ
		rect1 = new CRect(xStart + r2, yStart + r2, xStart - r2, yStart - r2);	//������Բ���޶����Σ����Ͻ����꣬���½����꣩
		mpDC->Ellipse(rect1);													//����Բ
		mpDC->SelectObject(oldbrush);											//���ؾɻ�ˢ
		break;
	}
	default:
		break;
	}



	//�������� ������λ��ʾ
	size = mpDC->GetTextExtent(unit, unit.GetLength());							//�����ַ������͸�
	if (mode != 4)mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.3, unit);				//��ʾ ��λ
	CString string1, string2;
	CFont font1;
	CFont font2;
	int rectwith;				//��
	int recthigh;				//��
	int rect_y;					//�ϱ߽��Բ�ĸ߶Ȳ�	
	int rectx;
	TEXTMETRIC tm;				//�ַ�����ȳ�����Ϣ

	switch (mode)
	{
	case 0:
	case 3:
	case 1:
	{
		//���β���
		rectwith = rect_width;				//��
		recthigh = rect_height;				//��
		rect_y = 1.2 * (int)r;				//�ϱ߽��Բ�ĸ߶Ȳ�	

		//���ƾ���
		mpDC->SelectObject(m_pen[0]);																//ѡ�񻭱�
		rect2 = new CRect(xStart - rectwith, yStart + rect_y, xStart, yStart + rect_y + recthigh);	//����� (���Ͻ����꣬���½�����)
		rect3 = new CRect(xStart, yStart + rect_y, xStart + rectwith, yStart + rect_y + recthigh);	//��
		mpDC->Rectangle(rect2);																		//����
		mpDC->Rectangle(rect3);

		//��Ϣ��ʾ
		mpDC->SetTextColor(word_color);												//����������ɫ
		font1.CreatePointFont((int)(word_size), _T("����"), mpDC);					//��������
		oldfont = mpDC->SelectObject(&font1);										//ѡ����������PDC
		string1.Format(_T("%d"), (int)METER_VALUE);									//��ֵת�����ַ���
		string2.Format(_T("%d"), num1);

		mpDC->TextOut(xStart - rect_width + 10, yStart + r, name1);					//��ʾ �����Ŀ��
		mpDC->TextOut(xStart - rect_width + 10, yStart + 1.25 * r, string2);			//��ʾ �����Ŀֵ
		mpDC->TextOut(xStart + 10, yStart + 1.25 * r, string1);						//��ʾ ��ǰֵ  value_num�ַ��������Ͻ�x���꣬���Ͻ�y���꣬�ַ�����
		mpDC->TextOut(xStart + 10, yStart + r, name);								//��ʾ ������Ŀ��

		mpDC->SelectObject(oldfont);												//��ԭ����

		break;
	}
	case 2:
	{
		rectx = 1.1 * r;
		mpDC->SelectObject(m_pen[0]);														//ѡ�񻭱�
		mpDC->MoveTo(xStart - rectx, yStart + 1.3 * r);										//�ϱ߿�
		mpDC->LineTo(xStart - rectx, yStart - 1.1 * r - 1);
		mpDC->LineTo(xStart + rectx - 1, yStart - 1.1 * r);
		mpDC->LineTo(xStart + rectx - 1, yStart + 1.3 * r);
		rect2 = new CRect(xStart - rectx, yStart + r, xStart - 0.2 * r, yStart + 1.3 * r);			//1��
		rect3 = new CRect(xStart - 0.2 * r, yStart + r, xStart + 1.1 * r, yStart + 1.3 * r);			//1��
		rect4 = new CRect(xStart - rectx, yStart + 1.3 * r - 1, xStart - 0.2 * r, yStart + 1.6 * r);//2��
		rect5 = new CRect(xStart - 0.2 * r, yStart + 1.3 * r - 1, xStart + 1.1 * r, yStart + 1.6 * r);//2��

		mpDC->Rectangle(rect2);																//����
		mpDC->Rectangle(rect3);
		mpDC->Rectangle(rect4);
		mpDC->Rectangle(rect5);

		//��Ϣ��ʾ
		mpDC->SetTextColor(word_color);												//����������ɫ
		font2.CreatePointFont((int)(word_size), _T("����"), mpDC);					//��������
		oldfont = mpDC->SelectObject(&font2);										//ѡ����������PDC
		string1.Format(_T("%d"), (int)METER_VALUE);									//��ֵת�����ַ���
		string2.Format(_T("%d"), num1);

		mpDC->TextOut(xStart - 0.9 * rectx, yStart + 1.1 * r, name1);					//��ʾ 1�� ��Ŀ��
		mpDC->TextOut(xStart, yStart + 1.1 * r, string2);							//��ʾ 1�� ��Ŀֵ
		mpDC->TextOut(xStart - 0.9 * rectx, yStart + 1.4 * r, name);					//��ʾ 2�� ������Ŀ��
		mpDC->TextOut(xStart, yStart + 1.4 * r, string1);							//��ʾ 2�� ��ǰֵ  value_num�ַ��������Ͻ�x���꣬���Ͻ�y���꣬�ַ�����

		mpDC->SelectObject(oldfont);												//��ԭ����

		break;
	}
	case 4:
	{
		//��Ϣ��ʾ
		mpDC->SetTextColor(word_color);												//����������ɫ
		font1.CreatePointFont((int)(word_size), _T("����"), mpDC);					//��������
		oldfont = mpDC->SelectObject(&font1);										//ѡ����������PDC
		string1.Format(_T("%d"), (int)METER_VALUE);									//��ֵת�����ַ���

		size = mpDC->GetTextExtent(unit, unit.GetLength());							//�����ַ������͸�
		mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.4, unit);				//��ʾ ��λ

		mpDC->SetTextColor(RGB(0, 0, 255));											//���ò���ֵ��ɫ
		size = mpDC->GetTextExtent(string1, string1.GetLength());					//�����ַ������͸�
		mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.2, string1);				//��ʾ ��ǰֵ  value_num�ַ��������Ͻ�x���꣬���Ͻ�y���꣬�ַ�����
		mpDC->SetTextColor(word_color);

		size = mpDC->GetTextExtent(name, name.GetLength());							//�����ַ������͸�
		mpDC->TextOut(xStart - size.cx * 0.5, yStart + r * 0.8, name);					//��ʾ ������Ŀ��


		mpDC->SelectObject(oldfont);												//��ԭ����
		break;
	}
	}


	//�ͷ���Դ
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
	case 1:		//��ʽһ
	{
		//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
		max = 1400;						//����������
		cut = 7;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
		unit = _T("kPa");				//������λ
		name = _T("ѹ��ֵ");			//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
		name1 = _T("ѹ����");			//��߷��� ��Ŀ��
		num1 = 1001;					//�����Ŀ ��ֵ
		num_color = RGB(0, 0, 0);		//��̶�����ɫ
		word_color = RGB(0, 0, 0);		//����ɫ
		word_size = 1.1 * R;				//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
		hand_type = 1;					//ָ������ 1ֱ�ߣ�2������
		rect_width = R;					//��Ϣ���
		rect_height = 0.3 * R;			//��Ϣ���
		METER_VALUE = 0;				//����ֵ

		//����
		pen[0].size = 3;				//0 ����
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = 2;				//1 Բ��
		pen[1].color = RGB(0, 0, 0);

		pen[2].size = 1;				//2 С�̶�
		pen[2].color = RGB(0, 0, 0);

		pen[3].size = 2;				//3 ��̶�
		pen[3].color = RGB(0, 0, 0);

		pen[4].size = R / 50;			//4 ָ��
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	ָ���
		pen[5].color = RGB(255, 0, 0);

		//DrawString(150, 50, _T("��ʽһ"), 180, RGB(0,0,255));			//������

		break;
	}
	case 2:		//��ʽ��
	{
		//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
		max = 1400;						//����������
		cut = 7;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
		unit = _T("kPa");				//������λ
		name = _T("ѹ��ֵ");			//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
		name1 = _T("ѹ����");			//��߷��� ��Ŀ��
		num1 = 1002;					//�����Ŀ ��ֵ
		num_color = RGB(0, 0, 0);		//��̶�����ɫ
		word_color = RGB(0, 0, 0);		//����ɫ
		word_size = 1.1 * R;				//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
		hand_type = 2;					//ָ������ 1ֱ�ߣ�2������
		METER_VALUE = 0;				//����ֵ

		//����
		pen[0].size = 2;				//0 ����
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = R / 5;				//1 Բ��
		pen[1].color = RGB(45, 171, 224);

		pen[2].size = 1;				//2 С�̶�
		pen[2].color = RGB(255, 255, 255);

		pen[3].size = 2;				//3 ��̶�
		pen[3].color = RGB(255, 255, 255);

		pen[4].size = R / 50;			//4 ָ��
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	ָ���
		pen[5].color = RGB(255, 0, 0);

		//pen[0] = { 2, RGB(0, 0, 0) };			//0 ����
		//pen[1] = { 30, RGB(45, 171, 224) };	//1 Բ��
		//pen[2] = { 1, RGB(255, 255, 255) };	//2 С�̶�
		//pen[3] = { 2, RGB(255, 255, 255) };	//3 ��̶�
		//pen[4] = { 5, RGB(255, 0, 0) };		//4 ָ��
		//pen[5] = { 10, RGB(255, 0, 0) };		//5	ָ���

		//DrawString(450, 50, _T("��ʽ��"), 180, RGB(0, 0, 255));			//������
		break;
	}
	case 3:		//��ʽ��
	{
		//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
		max = 300;						//����������
		cut = 15;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
		unit = _T("km/h");				//������λ
		name = _T("��ǰʱ��");			//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
		name1 = _T("����");				//��߷��� ��Ŀ��
		num1 = 1003;					//�����Ŀ ��ֵ
		num_color = RGB(0, 0, 0);		//��̶�����ɫ
		word_color = RGB(0, 0, 0);		//����ɫ
		word_size = 1.1 * R;				//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
		hand_type = 1;					//ָ������ 1ֱ�ߣ�2������
		rect_width = R;					//��Ϣ���
		rect_height = 0.3 * R;				//��Ϣ���
		METER_VALUE = 0;				//����ֵ
		red_area = 200;					//����ֵ

		//����
		pen[0].size = 3;				//0 ����
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = 2;				//1 Բ��
		pen[1].color = RGB(0, 0, 0);

		pen[2].size = 1;				//2 С�̶�
		pen[2].color = RGB(0, 0, 0);

		pen[3].size = 2;				//3 ��̶�
		pen[3].color = RGB(0, 0, 0);

		pen[4].size = R / 50;			//4 ָ��
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	ָ���
		pen[5].color = RGB(255, 0, 0);

		//DrawString(750, 50, _T("��ʽ��"), 180, RGB(0, 0, 255));			//������	

		break;
	}
	case 4:
	{
		//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
		max = 1400;						//����������
		cut = 7;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
		unit = _T("kPa");				//������λ
		name = _T("ѹ��ֵ");					//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
		num_color = RGB(0, 0, 0);		//��̶�����ɫ
		word_color = RGB(0, 0, 0);		//����ɫ
		word_size = 1.1 * R;				//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
		hand_type = 1;					//ָ������ 1ֱ�ߣ�2������
		rect_width = R;					//��Ϣ���
		rect_height = 0.3 * R;			//��Ϣ���
		METER_VALUE = 0;				//����ֵ

		//����
		pen[0].size = 3;				//0 ����
		pen[0].color = RGB(0, 0, 0);

		pen[1].size = 2;				//1 Բ��
		pen[1].color = RGB(0, 0, 0);

		pen[2].size = 1;				//2 С�̶�
		pen[2].color = RGB(0, 0, 0);

		pen[3].size = 2;				//3 ��̶�
		pen[3].color = RGB(0, 0, 0);

		pen[4].size = R / 50;			//4 ָ��
		pen[4].color = RGB(255, 0, 0);

		pen[5].size = 10;				//5	ָ���
		pen[5].color = RGB(255, 0, 0);

		//DrawString(150, 50, _T("��ʽ��"), 180, RGB(0,0,255));			//������

		break;
	}
	default:
		break;
	}
}

void MyMeter::DrawString(int x, int y, CString str1, int size, COLORREF color)
{
	CFont font1;
	pDC->SetTextColor(color);											//����������ɫ
	font1.CreatePointFont(size, _T("����"), pDC);						//��������
	CFont* oldfont1 = pDC->SelectObject(&font1);						//ѡ����������PDC
	pDC->TextOut(x, y, str1);
	pDC->SelectObject(oldfont1);										//��ԭ����
}

void MyMeter::DrawRect(int x, int y, int width, int height)
{
	//���ƾ���
	pDC->SelectObject(&pen[0]);											//ѡ�񻭱�
	CRect rect1(x, y, x + width, y + height);								//����� (���Ͻ����꣬���½�����)
	pDC->Rectangle(&rect1);												//����
}
