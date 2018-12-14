/*
程序名称:LCD12864的基本应用
程序原理:通过提取内置字符信息，然后输出到LCD12864
程序要点:1、LCD12864MINI的传输室通过SPI通讯进行传输的，因此此处要了解SPI协议的用法
		 2、控制字和数据的读取与写入
		 3、端口的控制
		 4、地址变量的定义，比如个人在程序调试时，由于将基地址的变量定义为无符号整型，导致读取字母时出错，因为那个时候的基地址已经超过了65535了
		 5、优先级的问题，在第一次调试时，无论怎么处理，得到的值都不对，原因是由于进行address&0xFF0000>>4的操作时忘记加括号了。由于移位的优先级高于与，所以出错
		 6、这里再强调一点，当时将取地址的那段程序放到VC里面，没有看出错误，能够返回正确值，是由于该电脑维64位的，所以int和long都是4个字节。而单片机里
		 	的int和long 类型是有差别的，所以才会出VC里没问题，单片机里出问题的情况。
编写时间:2013-09-24
*/

#include<reg52.h>
#include "LCD12864.h"
#include "delay.h"
#include "ziku.h"

unsigned long int address=0X00000000;



/*字库发送字节函数。选择发送的地址,选择八位数据传输*/
void ROM_Send(unsigned char add)				 //八位数据的发送，送入，上升沿锁存
{
   unsigned char i;
   Rom_CS=0;										 //初始化，选择字库
   for(i=0;i<8;i++)
   	 {
			 if(add&0X80)
				 LCD_SI=1;							//ROM写入数据，该线和LCD的复用
			 else
				 LCD_SI=0;							//ROM写1
			 LCD_CLK=0;								//时钟信号拉低
			 LCD_CLK=1;								//时钟信号拉高，形成上升沿，数据写入
			 add<<=1;
	 }										   			//这里不能给给Rom_CS拉高，依据时序图，得保持低电平到数据读取完毕
}

/*字库接收字节函数,八位一收*/

unsigned char ROM_Read()				      //八位数据的发送,读取，下降沿输出
{
   unsigned char i,dat=1;
   LCD_CLK=1;
   for(i=0;i<8;i++)
   	 {
			 LCD_CLK=0;										 //形成下降沿，数据读出
			 if(Rom_SO)										 //判断数据是否为1，先送高位
				dat=dat<<1|0x01;
			 else
				dat=dat<<1|0x00;
			 LCD_CLK=1;										//拉高，准备下一次拉低操作
	 }
	return dat;
}

/*字库发送命令控制，高中低三位字符*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L)
{
	  Rom_CS=0;											//拉低，准备送入读取命令
		ROM_Send(0x03);								//发送指令，03，准备读取数据
		ROM_Send(add_H);
		ROM_Send(add_M);
		ROM_Send(add_L);
}

/*液晶芯片写指令*/
void LCD12864_com(unsigned char com)
{
	unsigned char i;
	LCD_DC=0;					  //数据类型为指令  （DC=1---数据  DC=0---指令)
	LCD_CS=0;					  //SPI数据写，CS为低，上升沿写入
  for(i=0;i<8;i++)
   	 {
			 if(com&0X80)
				 LCD_SI=1;							//ROM写入数据，该线和LCD的复用
			 else
				 LCD_SI=0;							//ROM写1
			 LCD_CLK=0;								//时钟信号拉低
			 LCD_CLK=1;								//时钟信号拉高，形成上升沿，数据写入
			 com<<=1;									//先发送高位 D7-->D0
		 }										   			
	LCD_CS=1;											//写数据完毕，片选拉高
}

/*液晶芯片写数据*/
void LCD12864_dat(unsigned char dat)
{
	unsigned char i;
	LCD_DC=1;					  //数据类型为数据  （DC=1---数据  DC=0---指令)
	LCD_CS=0;					  //SPI写数据，片选拉低

  for(i=0;i<8;i++)
   	 {
			 if(dat&0X80)
				 LCD_SI=1;							//朝LCD控制芯片写数据
			 else
				 LCD_SI=0;							//写1
			 LCD_CLK=0;								//时钟信号拉低
			 LCD_CLK=1;								//时钟信号拉高，形成上升沿，数据写入
			 dat<<=1;									//先发送高位 D7-->D0
		 }										   			
	LCD_CS=1;											//写数据完毕，片选拉高
}

/*液晶清屏操作*/
void LCD12864_CLR()
{
	unsigned char i,j;			  //逐行逐列的往显示区域写0
	LCD_CS=0;					        //预先拉高
	Rom_CS = 1;	
	for(i=0;i<9;i++)			  //控制行显示，由于LCD12864是按页来划分的，总共128列，不过datasheet里显示有132列，64行，由于一次写8个bit，因此为8页
	  {
	  	LCD12864_com(0xB0+i);	  //这里写成OXB0+i的原因是由于，控制页的控制字高四位是固定的，低四位的值就控制着页
   	  LCD12864_com(0x10);  	  //列地址的高四位，当然这里可以不用写在循环内，因为液晶芯片会自动加1的（配合相关指令，默认加1）
	    LCD12864_com(0X00);     //列地址的低四位
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
/*page--显示页，column--显示列初始， prt--自编字库,flag--选择是否翻转显示*/
void LCD12864_16X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag)
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
			    if(flag)
						LCD12864_dat(*prt);
					else
						LCD12864_dat(~(*prt));
				prt++;						//这里将指正自加，到页地址为1时，即准备显示字的下半部了，指针的值会接着上次再自增
			}
	  
	  }
	LCD_CS=1;
}

/*液晶8*16（8宽）自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_8X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag)
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
			    if(flag)
						LCD12864_dat(*prt);
					else
						LCD12864_dat(~(*prt));
				prt++;						//这里将指正自加，到页地址为1时，即准备显示字的下半部了，指针的值会接着上次再自增
			}
	  
	  }
	LCD_CS=1;
}

/*液晶5*7（5宽）自编字体输出操作*/
/*page--显示页，column--显示列初始， prt--自编字库*/
void LCD12864_5X7(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag)
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
			    if(flag)
						LCD12864_dat(*prt);
					else
						LCD12864_dat(~(*prt));
					prt++;						//这里将指正自加，到页地址为1时，即准备显示字的下半部了，指针的值会接着上次再自增
			}
	  
	  }
	LCD_CS=1;
}

/*液晶调取GB2312字库显示程序*/
void LCD12864_GB2312_display(unsigned char page,unsigned char column, unsigned char *prt,unsigned char flag)
{
	unsigned char add_H,add_M,add_L;		 				//定义三个中间量来接收地址数据
  unsigned char datatemp[32];
	unsigned char i=0,fontsize;				               //fontsize为32，则调用16*16，如果为16,则调用8*16
	unsigned long BaseAdd=0;							   //BaseAdd--基础地址，GB2312和ASSIC的起始位置不一样,这里注意，基础地址应该定义为长整型，否则
														   //在后期处理字符数据时就会出错。
	while(*prt!=0)
	 {
		if(((*prt>=0xb0) &&(*prt<=0xf7))&&(*(prt+1)>=0xa1))
		  {
				BaseAdd=0x00;
		/*国标简体（GB2312）汉字在字库IC中的地址由以下公式来计算：*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
				address =(*prt-0xB0) * 94;
				address+=(*(prt+1)-0xA1)+846;
				address=( unsigned long )address*32+BaseAdd;
				fontsize=32;
				prt+=2;

		  }
		else if((*prt>=0xA1) &&(*prt<=0xA3)&&(*(prt+1)>=0xA1))
		  {
				BaseAdd=0x00;
		/*国标简体（GB2312）15x16点的字符在字库IC中的地址由以下公式来计算：*/
		/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
		/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
				address =(*prt-0xa1) * 94;
				address+=(*(prt+1)-0xA1);
				address=( unsigned long )address*32+BaseAdd;
		  	fontsize=32;
				prt+=2;
		  }
		else if((*prt>=0x20) &&(*prt<=0x7e))				   //这里的字符内码只有8位
		  {
				BaseAdd=0x3cf80;
				address = (*prt- 0x20);
				address = (unsigned long)(address*16);
				address = (unsigned long)(address+BaseAdd);			
				fontsize=16;
				++prt;
		  }
		add_H=(unsigned char)((address&0xff0000)>>16);			  //这里注意优先级的问题，与操作的优先级比移位操作低，如果忘记加括号，则会先移位
		add_M=(unsigned char)((address&0xff00)>>8);
		add_L=(unsigned char)(address&0xff);

		Rom_Send_Com(add_H,add_M,add_L);		  				  //送出地址数据
	  for(i=0;i<fontsize;i++)
		datatemp[i]=ROM_Read();
	  Rom_CS=1;
	  LCD_CLK=1;
		if (fontsize==32)
			{
			  LCD12864_16X16(page,column,datatemp,flag);
			  column=column+16;
			}
		if (fontsize==16)
			{
			  LCD12864_8X16(page,column,datatemp,flag);
			  column=column+8;
			}
	}
}

/*液晶调取5x7字库显示程序*/
void LCD12864_5X7_display(unsigned char page,unsigned char column, unsigned char *prt,unsigned char flag)
{
 	unsigned char add_H,add_M,add_L;		 //定义三个中间量来接收地址数据
  unsigned char datatemp[8];
	unsigned char i=0,MSB=0,LSB=0;			//MSB--字符内码高八位， LSB--字符内码低8位,fontsize用来控制调用不同的显示程序
											//fontsize为32，则调用16*16，如果为16,则调用8*16
	unsigned long BaseAdd=0;				//BaseAdd--基础地址，GB2312和ASSIC的起始位置不一样
  for(i=0;i<8;i++)						//这里字库清空很重要，否则上一次的数据会保存在数据里
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

		Rom_Send_Com(add_H,add_M,add_L);		  //送出地址数据
		for(i=0;i<8;i++)
			{
				datatemp[i]=ROM_Read();
			}
		Rom_CS=1;
		LCD_CLK=1;
	  LCD12864_5X7(page,column,datatemp,flag);
	  column=column+6;
	}
}

/*液晶芯片初始化*/
void LCD12864_init()
{
	LCD_CS=0;									//LCD片选信号
	Rom_CS = 1;								//字库ROM片选信号
/*目前使用的芯片自带上电自动复位功能
	LCD_RES = 0;							//LCD复位信号拉低
	delayMs(10);							//延时10ms
	LCD_RES = 1；							//复位完毕
*/ 
	delayMs(50);							//延时20ms
	LCD12864_com(0xAE);				//关闭LCD显示
	
	LCD12864_com(0xD5);				//设置显示时钟duty
	LCD12864_com(0x80);				//设置晶振频率
	
	LCD12864_com(0xA8);				//设置COM驱动路数 ，总计com为64路，0-63，这里表示com全部驱动
	LCD12864_com(0x3F);
	
	LCD12864_com(0x20);				//设置存储地址模式--页地址模式
	LCD12864_com(0x02);
	
	LCD12864_com(0xD3);				//设置水平位移为0，即设置从0行开始显示
	LCD12864_com(0x00);
	
	LCD12864_com(0x40);				//设置显示起始行为0行
	

	LCD12864_com(0xA1);			//列重定位 column address 0 is mapped to SEG0
/*以下三条命令实现的效果是扫描从上到下，扫描*/
	LCD12864_com(0xC8);			//行扫描模式，从COM63-COM00
	
	LCD12864_com(0xDA);
	LCD12864_com(0x12);	
	
	LCD12864_com(0x81);			//对比度设置
	LCD12864_com(0XCF);

	LCD12864_com(0XD9);			//预充电时间设置
	LCD12864_com(0X22);			//默认值

	LCD12864_com(0XDB);			//COM端口取消选择电压设置
	LCD12864_com(0X20);
	
	LCD12864_com(0XA4);			//打开显示
	LCD12864_com(0XCF);			//显示方式-正常/翻转
	
	LCD12864_com(0x8D);			//Set Charge Pump
	LCD12864_com(0x14);
	
	LCD12864_com(0XAF);			//打开显示
	
	delayMs(50);						//延时100ms
	
	LCD_CS=1;

}