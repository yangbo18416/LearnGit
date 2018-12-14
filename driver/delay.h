/*
程序名称:LCD12864的基本应用
程序原理:通过提取内置字符信息，然后输出到LCD12864
程序要点:1、LCD12864MINI的传输室通过SPI通讯进行传输的，因此此处要了解SPI协议的用法
		 2、控制字和数据的读取与写入
		 3、端口的控制
编写时间:2013-09-24
*/

#ifndef __delay_h__
#define __delay_h__
void delayMs(unsigned char i);
void delayUs(unsigned char i);
//延时1s
void delayTs(unsigned char i);
#endif