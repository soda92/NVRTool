/*
	������������ڻ��Ʋ�������ѹ������ѹ��ʱ�ٱ���ѹ���������
	���������̣�ָ�룬����Ϣ�б�������
	MyMeter���ʹ�ã�
		1�����������캯������ʹ�ã������������Զ�������ͣ����ٵ���ʹ��Ĭ��ֵ���ٻ������һ����ʹ�����е�������ʽ���ٻ��
		2������������������ʹ�ø���set_�������ñ�����
		3��������ɫ��ʱ����Ҫ��Ӧ����
		4����ǰ����ֵ����ָ���ָ����Ҫ�ڻ����ʱ�򴫵ݣ�����Ĭ��ָ��0
	ע�����������̺ͷָ������ʱ�� �ָ����Ҫ�ܱ��������������������趨����ķָ����
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

	MyMeter(int ox_, int oy_, int R_, int max_, int cut_, CString unit_, CString name_, CString name1_, int num1_); //���캯��
	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,������λunit,������Ŀ��name����Ŀ1������Ŀ1ֵ

	MyMeter(int ox_, int oy_, int R_, int mode_);//��ʽ���캯�� ����λ�ú���ʽ
	//������CDCָ�룬Բ������x��Բ������y������ʽmode��0Ĭ�ϣ�1��ʽһ��2��ʽ����3��ʽ����

	MyMeter();//Ĭ��ֵ���캯��  ʹ��Ĭ��ֵ

	void DrawMeter(CDC *pDC_, double METER_VALUE);		//����CDCָ�룬��ǰ����ֵ��
	void DrawMeter(CDC *pDC_);							//���� ������ֵΪ0��

	void set_site(int ox,int oy);						//���� ���ĵ����꣨x���꣬y���꣩
	void set_R(int R);									//���� �뾶R
	void set_range(int max,int cut,CString unit);		//���� ���̡����̷ָ������������λ
	void set_item(CString name,CString name1,int num1);	//���� ����ֵ��Ŀ������1��Ŀ������1��Ŀֵ
	void set_pen(int num, int size, COLORREF color);	//���� �������ɫ�ʹ�ϸ  ���š���ϸ����ɫ  0����1Բ��2С�̶�3��̶�4ָ��5ָ���
	void set_wordcolor(COLORREF color);					//���� �ֵ���ɫ
	void set_numcolor(COLORREF color);					//���� �ֵ���ɫ
	void set_handtype(int hand_type);					//���� ָ������ 1ֱ���ͣ�2������
	void set_wordsize(int size);						//���� �����С
	void set_rect(int width,int height);				//���� ���γ���
	void set_redarea(int red_area);						//���� ����ֵ

	void use_mode(int num);								//ѡ�� ��ʽ
private:
	CDC *pDC;
	//Ĭ��ֵ400, 240, 200, 2500, 5, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1024
	int ox;							//����Բ��x����
	int oy;							//����Բ��y����
	int R;							//���̰뾶
	int max;						//����������
	int cut;						//���̷ֳɼ��ݣ�����ܱ�������������� ���̵���С�̶�ֵ=max/cut/10
	CString unit;					//������λ
	CString name;					//������Ŀ��);	//������CDCָ��,Բ��x����,Բ��y����,�뾶R,�������max,���̷���cut,��ǰֵMETER_VALUE,��λunit,��Ŀ��name
	CString name1;					//��߷��� ��Ŀ��
	int num1;						//�����Ŀ ��ֵ
	COLORREF num_color ;			//��̶�����ɫ
	COLORREF word_color;			//����ɫ
	int word_size ;					//�����С	ע����ʮ��֮һ�� Ϊ��λ ����120ֵ ����12���ظ�
	int hand_type;					//ָ������ 1ֱ�ߣ�2������
	int rect_width;					//��Ϣ���
	int rect_height ;				//��Ϣ���
	double METER_VALUE;				//����ֵ
	int mode;						//ʹ����ʽ 0Ĭ�� ��ʽ1 2 3
	int red_area;					//�������� 0Ϊ��

	//����
	PEN pen[6];

private:
	void DrawString(int x, int y, CString str1, int size, COLORREF color);
	void DrawRect(int x,int y, int width,int height);
};

/*
													��������ʹ��ʾ����������
	//MyMeter mymeter0();			//Ĭ����ʽ
	//mymeter0.DrawMeter(pDC);

	//ֱ�ӵ���������ʽ
	MyMeter mymeter1(200,240,1);	//��ʽһ
	mymeter1.DrawMeter(pDC);

	MyMeter mymeter2(500,240,2);	//��ʽ��
	mymeter2.DrawMeter(pDC);

	MyMeter mymeter3(800, 240, 3);	//��ʽ��
	mymeter3.DrawMeter(pDC);

	//�Զ�����ʽ
	
	MyMeter mymeter1(200, 240, 130, 2400, 8, _T("kPa"),_T("ѹ��ֵ"),_T("���"),1001);
	mymeter1.set_pen(4,2,RGB(255,0,0));				//���� ָ����Ϊ2����ɫΪ��ɫ
	mymeter1.set_rect(120,40);						//���� ��Ϣ�б��Ϳ�
	// mymeter1.set_wordsize(180);					//���� ��Ϣ�б������С
	mymeter1.DrawMeter(pDC,rand() % 2400);			//��ͼ����������ǰ����ֵ��

	MyMeter mymeter2(500, 240, 130, 1000, 10, _T("kPa"), _T("ѹ��ֵ"), _T("���"), 1002);
	mymeter2.set_pen(3,2,RGB(0,255,0));				//���� ��̶���ɫΪ��ɫ
	mymeter2.DrawMeter(pDC,rand() % 1000);			

	MyMeter mymeter3(800, 240, 130, 500, 5, _T("kPa"), _T("ѹ��ֵ"), _T("���"), 1003);
	mymeter3.set_pen(0, 1, RGB(0, 0, 255));			//���� ��Ϣ�б���α߿��ϸΪ1����ɫΪ��
	mymeter3.set_pen(1, 30, RGB(45, 171,224 ));		//���� ����Բ����ϸΪ30����ɫΪRGB(45, 171,224 )
	mymeter3.set_pen(2, 1, RGB(255, 255, 255));		//���� С�̶ȴ�ϸΪ1����ɫΪ��
	mymeter3.set_pen(3, 1, RGB(255, 255, 255));		//���� ��̶ȴ�ϸΪ1����ɫΪ��
	mymeter3.set_pen(4, 5, RGB(0, 0, 0));			//���� ָ���ϸΪ5����ɫΪ��
	mymeter3.set_handtype(2);						//���� ָ������Ϊ������
	mymeter3.DrawMeter(pDC,rand() % 500);
	
*/



