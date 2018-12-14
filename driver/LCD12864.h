/*
程序名称:LCD12864的基本应用
程序原理:通过提取内置字符信息，然后输出到LCD12864
程序要点:1、LCD12864MINI的传输室通过SPI通讯进行传输的，因此此处要了解SPI协议的用法
				 2、控制字和数据的读取与写入
				 3、端口的控制
				 4、LCD显示控制采取的是SPI控制，因此可以将其和SPI_Rom进行整合
编写时间:2015-09-25
*/
#ifndef __LCD12864_h__
#define __LCD12864_h__

/*此处端口定义为LCD12864显示端口*/
sbit LCD_CLK = P2^0;						//LCD以及字库芯片公用时钟端口
sbit LCD_SI  = P2^1;						//LCD以及字库芯片公用的MOSI管脚，向控制芯片传输数据
sbit LCD_DC  = P2^2;						//LCD控制芯片，数据/命令 选择 0--命令， 1--数据
sbit LCD_CS	 = P2^3;						//LCD片选信号
sbit Rom_SO	 = P2^4;						//Rom的数据输出
sbit Rom_CS	 = P2^5;						//字库芯片片选信号

///*字库芯片读数据*/
//unsigned char Read_Rom(unsigned char);

/*发送字节函数。选择发送的地址,选择八位数据传输*/
void ROM_Send(unsigned char add);				 //八位数据的发送
/*接收字节函数,八位一收*/
unsigned char ROM_Read();				      //八位数据的发送
/*汉字存储地址获取*/

void LCD12864_address(unsigned char * prt);

/*发送命令控制，高中低三位字符*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L);
/*液晶芯片写指令*/
void LCD12864_com(unsigned char com);

/*液晶芯片写数据*/
void LCD12864_dat(unsigned char dat);

/*液晶芯片初始化*/
void LCD12864_init();

/*液晶清屏操作*/
void LCD12864_CLR();

/*液晶16*16自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_16X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*液晶8*16（8宽）自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_8X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*液晶5*7（5宽）自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_5X7(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*液晶调GB2312取字库显示程序*/
void LCD12864_GB2312_display(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*液晶调取5x7字库显示程序*/
void LCD12864_5X7_display(unsigned char page,unsigned char column, unsigned char *prt,unsigned char flag);


#endif