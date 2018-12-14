/*
程序名称:LCD12864的基本应用
程序原理:通过提取内置字符信息，然后输出到LCD12864
程序要点:1、LCD12864MINI的传输室通过SPI通讯进行传输的，因此此处要了解SPI协议的用法
		 2、控制字和数据的读取与写入
		 3、端口的控制
编写时间:2013-09-24
*/

#include<reg52.h>
#include "LCD12864.h"
#include "Rom_SPI.h"
#include "delay.h"

long int address=0X00000000;
unsigned char add_H,add_M,add_L;		 //定义三个中间量来接收地址数据

///*字库芯片读数据*/
//unsigned char Read_Rom(unsigned char)
//{
//  return 0x00;
//}


/*液晶芯片写指令*/
void LCD12864_com(unsigned char com)
{
	unsigned char i;
	LCD_RS=0;					  //数据类型为指令  （RS=1---数据  RS=2---指令)
	LCD_CS=0;					  //串行数据读，CS为低
	for(i=0;i<8;i++)
		{
		 	LCD_CLK=0;
			if((bit)(com&0x80))
				LCD_SDA=1;
			else
				LCD_SDA=0;
			LCD_CLK=1;			  //数据在上升沿传入，数据端口提前准备待传出的数据
			com=com<<1;			  //每传输完一位数据后，控制字左移一位
				
		}
	LCD_CS=0;
}

/*液晶芯片写数据*/
void LCD12864_dat(unsigned char dat)
{
	unsigned char i;
	LCD_RS=1;					  //数据类型为数据  （RS=1---数据  RS=2---指令)
	LCD_CS=0;					  //串行数据读，CS为低
	for(i=0;i<8;i++)
		{
		 	LCD_CLK=0;
			if((bit)(dat&0x80))
				LCD_SDA=1;
			else
				LCD_SDA=0;
			LCD_CLK=1;			  //数据在上升沿传入，数据端口提前准备待传出的数据
			dat=dat<<1;			  //每传输完一位数据后，控制字左移一位
				
		}
   LCD_CS=1;
}

/*液晶清屏操作*/
void LCD12864_CLR()
{
	unsigned char i,j;			  //逐行逐列的往显示区域写0
	LCD_CS=1;					  //预先拉高
	Rom_CS = 1;	
	for(i=0;i<9;i++)			  //控制行显示，由于LCD12864是按页来划分的，总共128列，不过datasheet里显示有132列，64行，由于一次写8个bit，因此为8页
	  {
	  	LCD12864_com(0xB0+i);	  //这里写成OXB0+i的原因是由于，控制页的控制字高四位是固定的，低四位的值就控制着页
   	    LCD12864_com(0x10);  	  //列地址的高四位，当然这里可以不用写在循环内，因为液晶芯片会自动加1的（配合相关指令，默认加1）
	    LCD12864_com(0X00);       //列地址的低四位
	  	for(j=0;j<133;j++)
		   {
//		   	  LCD12864_com(0x10+(j>>4));  //列地址的高四位，当然这里可以不用写在循环内，因为液晶芯片会自动加1的（配合相关指令，默认加1）
//			  LCD12864_com(0X00+j&0x0f);  //列地址的低四位
			  LCD12864_dat(0X00);
		   }	  
	  }
	LCD_CS=1;
}
/*液晶16*16(16宽)自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_16X16(unsigned char page,unsigned char column,unsigned char *prt)
{
	unsigned char i,j;			  			//i控制page，j控制column
	LCD_CS=1;
	Rom_CS=1;
	column=column-1;						//选择第一列开始显示，数字输入为1，但实际上指令是从0开始的
	page=page-1;							//同理
	for(i=0;i<2;i++)
	  {
	      LCD12864_com(0XB0+(page+i));		//写入页地址
		  LCD12864_com(0x10+(column>>4));	//写入列地址高四位
		  LCD12864_com(0x00+column&0X0F);
		  for(j=0;j<16;j++)					//这里只写16位，是因为字宽只占16列，且数组的前16个就组成了字的上半部
		  	{
			    LCD12864_dat(*prt);
				prt++;						//这里将指正自加，到页地址为1时，即准备显示字的下半部了，指针的值会接着上次再自增
			}
	  
	  }
	LCD_CS=1;
}

/*液晶8*16（8宽）自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_8X16(unsigned char page,unsigned char column,unsigned char *prt)
{
	unsigned char i,j;			  			//i控制page，j控制column
	LCD_CS=1;
	Rom_CS=1;
	column=column-1;						//选择第一列开始显示，数字输入为1，但实际上指令是从0开始的
	page=page-1;							//同理
	for(i=0;i<2;i++)
	  {
	      LCD12864_com(0XB0+(page+i));		//写入页地址
		  LCD12864_com(0x10+(column>>4));	//写入列地址高四位
		  LCD12864_com(0x00+column&0X0F);
		  for(j=0;j<8;j++)					//这里只写8位，是因为字宽只占8列，且数组的前8个就组成了字的上半部
		  	{
			    LCD12864_dat(*prt);
				prt++;						//这里将指正自加，到页地址为1时，即准备显示字的下半部了，指针的值会接着上次再自增
			}
	  
	  }
	LCD_CS=1;
}

/*液晶5*7（5宽）自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_5X7(unsigned char page,unsigned char column,unsigned char *prt)
{
	unsigned char i,j;			  			//i控制page，j控制column
	LCD_CS=1;
	Rom_CS=1;
	column=column-1;						//选择第一列开始显示，数字输入为1，但实际上指令是从0开始的
	page=page-1;							//同理
	for(i=0;i<1;i++)
	  {
	      LCD12864_com(0XB0+(page+i));		//写入页地址
		  LCD12864_com(0x10+(column>>4));	//写入列地址高四位
		  LCD12864_com(0x00+column&0X0F);
		  for(j=0;j<5;j++)					//这里只写5位，是因为字宽只占5列
		  	{
			    LCD12864_dat(*prt);
				prt++;						//这里将指正自加，到页地址为1时，即准备显示字的下半部了，指针的值会接着上次再自增
			}
	  
	  }
	LCD_CS=1;
}


/*液晶调取GB2312字库显示程序*/
void LCD12864_GB2312_display(unsigned char page,unsigned char column, unsigned char *prt)
{
    unsigned char datatemp[32];
	unsigned char i=0,MSB=0,LSB=0,fontsize;				   //MSB--字符内码高八位， LSB--字符内码低8位,fontsize用来控制调用不同的显示程序
														   //fontsize为32，则调用16*16，如果为16,则调用8*16
	unsigned int BaseAdd=0;								   //BaseAdd--基础地址，GB2312和ASSIC的起始位置不一样
    for(i=0;i<32;i++)									   //这里字库清空很重要，否则上一次的数据会保存在数据里
		datatemp[i]=0;
	i=0;
	while(*prt!=0)
	 {
		/*国标简体（GB2312）汉字在字库IC中的地址由以下公式来计算：*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
		MSB=*prt;										   //获取地址，字符内码高八位
		LSB=*(prt+1);
		if(((MSB>=0xb0) &&(MSB<=0xf7))&&(LSB>=0xa1))
		  {
				BaseAdd=0x00;
				address =(MSB-0xB0) * 94;
				address+=(LSB-0xA1+846);
				address=( unsigned long )address*32+BaseAdd;
				fontsize=32;
				prt++;
		  }
		else if((MSB>=0xA1) &&(MSB<=0xA3)&&(LSB>=0xA1))
		  {
				
				BaseAdd=0x00;
				address =(MSB-0xB0) * 94;
				address+=(LSB-0xA1);
				address=( unsigned long )address*32+BaseAdd;
		  		fontsize=32;
				prt++;
		  }
		else if((MSB>=0x20) &&(MSB<=0x7e))				   //这里的字符内码只有8位
		  {
				BaseAdd=0x3cf80;
				address = (MSB- 0x20);
				address = (unsigned long)(address*16);
				address = (unsigned long)(address+BaseAdd);			
				fontsize=16;
		  }
		prt++;									  //在判断语句里，完成移位1次或者0次后，再向后移动1位，准备接受下一次地址
		add_H=(unsigned char)((address&0xff0000)>>16);			  //这里注意优先级的问题，与操作的优先级比移位操作低，如果忘记加括号，则会先移位
		add_M=(unsigned char)((address&0xff00)>>8);
		add_L=(unsigned char)(address&0xff);

		Rom_Send_Com(add_H,add_M,add_L);		  //送出地址数据
//		Rom_Send_Com(0x03,0xD1,0xf0);
		for(i=0;i<fontsize;i++)
			{
				datatemp[i]=ROM_Read();
			}
//		Rom_CS=1;
//		Rom_CLK=1;
		if (fontsize==32)
			{
			  LCD12864_16X16(page,column,datatemp);
			  column=column+16;
			}
		if (fontsize==16)
			{
			  LCD12864_8X16(page,column,datatemp);
			  column=column+8;
			}
	}
}

/*液晶调取5x7字库显示程序*/
void LCD12864_5X7_display(unsigned char page,unsigned char column, unsigned char *prt)
{
    unsigned char datatemp[8];
	unsigned char i=0,MSB=0,LSB=0;				   //MSB--字符内码高八位， LSB--字符内码低8位,fontsize用来控制调用不同的显示程序
														   //fontsize为32，则调用16*16，如果为16,则调用8*16
	unsigned int BaseAdd=0;								   //BaseAdd--基础地址，GB2312和ASSIC的起始位置不一样
    for(i=0;i<8;i++)									   //这里字库清空很重要，否则上一次的数据会保存在数据里
		datatemp[i]=0;
	i=0;
	while(*prt!=0)
	 {
		/*国标简体（GB2312）汉字在字库IC中的地址由以下公式来计算：*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
		MSB=*prt;										   //获取地址，字符内码高八位
		if(((MSB>=0x20) &&(MSB<=0x7e)))
		  {
				BaseAdd=0x3bfc0;
				address = (MSB- 0x20);
				address = (unsigned long)(address*8);
				address = (unsigned long)(address+BaseAdd);			
				prt++;
		  }
		add_H=(unsigned char)((address&0xff0000)>>16);			  //这里注意优先级的问题，与操作的优先级比移位操作低，如果忘记加括号，则会先移位
		add_M=(unsigned char)((address&0xff00)>>8);
		add_L=(unsigned char)(address&0xff);

//		Rom_Send_Com(add_H,add_M,add_L);		  //送出地址数据
		Rom_Send_Com(0x03,0xde,0xe8);
		for(i=0;i<8;i++)
			{
				datatemp[i]=ROM_Read();
			}
//		Rom_CS=1;
//		Rom_CLK=1;
	    LCD12864_5X7(page,column,datatemp);
	    column=column+6;
	}
}

/*液晶芯片初始化*/
void LCD12864_init()
{
	LCD_CS=0;
	Rom_CS = 1;
	LCD_RST=0;                //低电平复位
	delayMs(10);
	LCD_RST=1;		          //复位完毕
	delayMs(10);  
	
	LCD12864_com(0xE2);		  //软件复位
	delayMs(5);      

	LCD12864_com(0x2F);		  //设置电源控制
	delayMs(5);				  //延时5ms

	LCD12864_com(0x81);		  //设置对比，这个是必须的，这个值需要不断的调整，以达到最好的显示效果
	LCD12864_com(0x20);		  //设置值为45，该过程需要两条语句配合完成

	LCD12864_com(0xC8);		  //设置行扫描方式，从上到下

	LCD12864_com(0xA0);		  //设置列扫描方式，从左至右

	LCD12864_com(0x40);		  //从第一行开始显示（实际上控制的是滚动行命令）
	
	LCD12864_com(0xA2);	      //1/9偏压比（bias）

	LCD12864_com(0xAF);		  //打开显示
	
	LCD_CS=1;

}