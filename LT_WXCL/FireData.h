//FireData.h
/*	
	防火数据
	内容：串口初始化、防火数据解析线程、消息发送线程	
*/


#pragma once

int WXCL_FireComInit(char* com);
UINT Thread_WXCL_FireData(LPVOID lParam);

int WXCL_SendMsg(unsigned char* buf,int len);

//