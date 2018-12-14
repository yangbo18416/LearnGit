#include<reg52.h>
#include<intrins.H>
#include "Rom_SPI.h"
#include "delay.h"

/*发送字节函数。选择发送的地址,选择八位数据传输*/
void ROM_Send(unsigned char add)				 //八位数据的发送
{
   unsigned char i;
   Rom_CS=0;										 //初始化
   Rom_CLK=0;
   for(i=0;i<8;i++)
   	 {
	 	 if(add&0X80)
		 	Rom_SI=1;
		 else
		 	Rom_SI=0;
		 Rom_CLK=0;
		 Rom_CLK=1;
		 add<<=1;
	 }										   //这里不能给给Rom_CS拉高，依据时序图，得保持低电平到数据读取完毕
}

/*接收字节函数,八位一收*/

unsigned char ROM_Read()				      //八位数据的发送
{
   unsigned char i,dat=1;;
   Rom_CLK=1;
   for(i=0;i<8;i++)
   	 {
		 Rom_CLK=0;
	 	 if(Rom_SO)
		 	dat=dat<<1|0x01;
		 else
		 	dat=dat<<1|0x00;
		 Rom_CLK=1;
	 }
	return dat;
}

/*发送命令控制，高中低三位字符*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L)
{
	  Rom_CS=1;
		ROM_Send(0x03);
		ROM_Send(add_H);
		ROM_Send(add_M);
		ROM_Send(add_L);
}


