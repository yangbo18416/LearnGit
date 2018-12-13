/*
程序名称:LCD12864的提取字库数据的应用
程序原理:通过提取内置字符信息，然后输出到LCD12864
程序要点:1、LCD12864MINI的传输室通过SPI通讯进行传输的，因此此处要了解SPI协议的用法
				 2、控制字和数据的读取与写入
				 3 、端口的控制

程序bug: 1、无法显示汉字“数”
编写时间:2013-09-24
*/
#include<reg52.h>
#include<math.h>
#include<intrins.h>
#include "stdio.h"
#include "LCD12864.h"
#include "LCD_Menu.h"
#include "ziku.h"
void main()
{
	LCD12864_init();
	LCD12864_CLR();
	Time0_Init();
	while(1)
	  {

	  }
}