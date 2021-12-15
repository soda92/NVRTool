#pragma once


//WORD m_Conday = 0;									// 用来判断时间是否变更的变量


int LdleDataComInit(char* com);
UINT Thread_LdleDataAnalysis(LPVOID lParam);

int LDFM_SendLdleDataMsg(unsigned char* buf, int len);
