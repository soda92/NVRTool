#pragma once


//WORD m_Conday = 0;									// �����ж�ʱ���Ƿ����ı���


int LdleDataComInit(char* com);
UINT Thread_LdleDataAnalysis(LPVOID lParam);

int LDFM_SendLdleDataMsg(unsigned char* buf, int len);
