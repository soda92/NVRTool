# ����˵��

## FireMsg

AA AA 4F 00 01 [A/B] TaxDataBuf [SUM]

���͵�COM2

## ֹͣ����

AA AA 10 02 01 12 01

���͵�COM2

## �����߳�

����1024��RecBuf

FF 05 04 0A(0B)(��theApp.local����) + RecBuf

���͵�255.255.255.255:8000

## ��Դ����

ÿ5s���ͣ�`0x55 88 99`���㲥��ַ

## UdpBroadcastRecv

�ӹ㲥��ַ������RecBuf[1024]

- FF 01: ����
- FF 02: �ɼ���
- FF 03: Уʱ
- FF 04: �жϱ���
- FF 05: ��ת��δʹ�ã�

## Thread_WXCL_FireData

��COM2��FireCom����ȡ��RecBuf[256]

FF 01 04 0A(0B) ���͵��㲥

## ����

tPort: COM1: Tax
pPort: COM3: Power
