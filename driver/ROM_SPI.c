#include<reg52.h>
#include<intrins.H>
#include "Rom_SPI.h"
#include "delay.h"

/*�����ֽں�����ѡ���͵ĵ�ַ,ѡ���λ���ݴ���*/
void ROM_Send(unsigned char add)				 //��λ���ݵķ���
{
   unsigned char i;
   Rom_CS=0;										 //��ʼ��
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
	 }										   //���ﲻ�ܸ���Rom_CS���ߣ�����ʱ��ͼ���ñ��ֵ͵�ƽ�����ݶ�ȡ���
}

/*�����ֽں���,��λһ��*/

unsigned char ROM_Read()				      //��λ���ݵķ���
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

/*����������ƣ����е���λ�ַ�*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L)
{
	  Rom_CS=1;
		ROM_Send(0x03);
		ROM_Send(add_H);
		ROM_Send(add_M);
		ROM_Send(add_L);
}


