/*
程序名称:LCD12864的基本应用
程序原理:通过提取内置字符信息，然后输出到LCD12864
程序要点:1、LCD12864MINI的传输室通过SPI通讯进行传输的，因此此处要了解SPI协议的用法
		 2、控制字和数据的读取与写入
		 3、端口的控制
编写时间:2013-09-24
*/

#include<reg52.h>
#include "delay.h"

/*延时MS ，每一次延时，大概约1ms，当然这个是在晶振为12MHz的情况下*/
void delayMs(unsigned char i)
{
	while(--i)
		{
			delayUs(245);
			delayUs(245);		
		}
}

void delayUs(unsigned char i)
{
	while(--i);
}
//延时1s
void delayTs(unsigned char i)
{
	while(i--)
		{
	  	   delayMs(250);
		   delayMs(250);
		   delayMs(250);
		   delayMs(250);
		}
}