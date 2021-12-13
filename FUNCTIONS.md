# 函数说明

## FireMsg

AA AA 4F 00 01 [A/B] TaxDataBuf [SUM]

发送到COM2

## 停止报警

AA AA 10 02 01 12 01

发送到COM2

## 接收线程

读入1024到RecBuf

FF 05 04 0A(0B)(由theApp.local决定) + RecBuf

发送到255.255.255.255:8000

## 电源数据

每5s发送：`0x55 88 99`到广播地址

## UdpBroadcastRecv

从广播地址接收至RecBuf[1024]

- FF 01: 防火
- FF 02: 采集盒
- FF 03: 校时
- FF 04: 中断报警
- FF 05: 空转（未使用）

## Thread_WXCL_FireData

从COM2（FireCom）读取到RecBuf[256]

FF 01 04 0A(0B) 发送到广播

## 其他

tPort: COM1: Tax
pPort: COM3: Power
