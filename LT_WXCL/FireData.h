//FireData.h
/*	
	��������
	���ݣ����ڳ�ʼ�����������ݽ����̡߳���Ϣ�����߳�	
*/


#pragma once

int WXCL_FireComInit(char* com);
UINT Thread_WXCL_FireData(LPVOID lParam);

int WXCL_SendMsg(unsigned char* buf,int len);

//