//FireData.h
/*
	��������
	���ݣ����ڳ�ʼ�����������ݽ����̡߳���Ϣ�����߳�
*/


#pragma once

int FireComInit(char* com);
UINT Thread_FireData(LPVOID lParam);

int SendMsg(unsigned char* buf, int len);
