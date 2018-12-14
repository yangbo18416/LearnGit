/*
��������:LCD12864�Ļ���Ӧ��
����ԭ��:ͨ����ȡ�����ַ���Ϣ��Ȼ�������LCD12864
����Ҫ��:1��LCD12864MINI�Ĵ�����ͨ��SPIͨѶ���д���ģ���˴˴�Ҫ�˽�SPIЭ����÷�
		 2�������ֺ����ݵĶ�ȡ��д��
		 3���˿ڵĿ���
��дʱ��:2013-09-24
*/

#include<reg52.h>
#include "LCD12864.h"
#include "Rom_SPI.h"
#include "delay.h"

long int address=0X00000000;
unsigned char add_H,add_M,add_L;		 //���������м��������յ�ַ����

///*�ֿ�оƬ������*/
//unsigned char Read_Rom(unsigned char)
//{
//  return 0x00;
//}


/*Һ��оƬдָ��*/
void LCD12864_com(unsigned char com)
{
	unsigned char i;
	LCD_RS=0;					  //��������Ϊָ��  ��RS=1---����  RS=2---ָ��)
	LCD_CS=0;					  //�������ݶ���CSΪ��
	for(i=0;i<8;i++)
		{
		 	LCD_CLK=0;
			if((bit)(com&0x80))
				LCD_SDA=1;
			else
				LCD_SDA=0;
			LCD_CLK=1;			  //�����������ش��룬���ݶ˿���ǰ׼��������������
			com=com<<1;			  //ÿ������һλ���ݺ󣬿���������һλ
				
		}
	LCD_CS=0;
}

/*Һ��оƬд����*/
void LCD12864_dat(unsigned char dat)
{
	unsigned char i;
	LCD_RS=1;					  //��������Ϊ����  ��RS=1---����  RS=2---ָ��)
	LCD_CS=0;					  //�������ݶ���CSΪ��
	for(i=0;i<8;i++)
		{
		 	LCD_CLK=0;
			if((bit)(dat&0x80))
				LCD_SDA=1;
			else
				LCD_SDA=0;
			LCD_CLK=1;			  //�����������ش��룬���ݶ˿���ǰ׼��������������
			dat=dat<<1;			  //ÿ������һλ���ݺ󣬿���������һλ
				
		}
   LCD_CS=1;
}

/*Һ����������*/
void LCD12864_CLR()
{
	unsigned char i,j;			  //�������е�����ʾ����д0
	LCD_CS=1;					  //Ԥ������
	Rom_CS = 1;	
	for(i=0;i<9;i++)			  //��������ʾ������LCD12864�ǰ�ҳ�����ֵģ��ܹ�128�У�����datasheet����ʾ��132�У�64�У�����һ��д8��bit�����Ϊ8ҳ
	  {
	  	LCD12864_com(0xB0+i);	  //����д��OXB0+i��ԭ�������ڣ�����ҳ�Ŀ����ָ���λ�ǹ̶��ģ�����λ��ֵ�Ϳ�����ҳ
   	    LCD12864_com(0x10);  	  //�е�ַ�ĸ���λ����Ȼ������Բ���д��ѭ���ڣ���ΪҺ��оƬ���Զ���1�ģ�������ָ�Ĭ�ϼ�1��
	    LCD12864_com(0X00);       //�е�ַ�ĵ���λ
	  	for(j=0;j<133;j++)
		   {
//		   	  LCD12864_com(0x10+(j>>4));  //�е�ַ�ĸ���λ����Ȼ������Բ���д��ѭ���ڣ���ΪҺ��оƬ���Զ���1�ģ�������ָ�Ĭ�ϼ�1��
//			  LCD12864_com(0X00+j&0x0f);  //�е�ַ�ĵ���λ
			  LCD12864_dat(0X00);
		   }	  
	  }
	LCD_CS=1;
}
/*Һ��16*16(16��)�Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_16X16(unsigned char page,unsigned char column,unsigned char *prt)
{
	unsigned char i,j;			  			//i����page��j����column
	LCD_CS=1;
	Rom_CS=1;
	column=column-1;						//ѡ���һ�п�ʼ��ʾ����������Ϊ1����ʵ����ָ���Ǵ�0��ʼ��
	page=page-1;							//ͬ��
	for(i=0;i<2;i++)
	  {
	      LCD12864_com(0XB0+(page+i));		//д��ҳ��ַ
		  LCD12864_com(0x10+(column>>4));	//д���е�ַ����λ
		  LCD12864_com(0x00+column&0X0F);
		  for(j=0;j<16;j++)					//����ֻд16λ������Ϊ�ֿ�ֻռ16�У��������ǰ16����������ֵ��ϰ벿
		  	{
			    LCD12864_dat(*prt);
				prt++;						//���ｫָ���Լӣ���ҳ��ַΪ1ʱ����׼����ʾ�ֵ��°벿�ˣ�ָ���ֵ������ϴ�������
			}
	  
	  }
	LCD_CS=1;
}

/*Һ��8*16��8���Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_8X16(unsigned char page,unsigned char column,unsigned char *prt)
{
	unsigned char i,j;			  			//i����page��j����column
	LCD_CS=1;
	Rom_CS=1;
	column=column-1;						//ѡ���һ�п�ʼ��ʾ����������Ϊ1����ʵ����ָ���Ǵ�0��ʼ��
	page=page-1;							//ͬ��
	for(i=0;i<2;i++)
	  {
	      LCD12864_com(0XB0+(page+i));		//д��ҳ��ַ
		  LCD12864_com(0x10+(column>>4));	//д���е�ַ����λ
		  LCD12864_com(0x00+column&0X0F);
		  for(j=0;j<8;j++)					//����ֻд8λ������Ϊ�ֿ�ֻռ8�У��������ǰ8����������ֵ��ϰ벿
		  	{
			    LCD12864_dat(*prt);
				prt++;						//���ｫָ���Լӣ���ҳ��ַΪ1ʱ����׼����ʾ�ֵ��°벿�ˣ�ָ���ֵ������ϴ�������
			}
	  
	  }
	LCD_CS=1;
}

/*Һ��5*7��5���Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_5X7(unsigned char page,unsigned char column,unsigned char *prt)
{
	unsigned char i,j;			  			//i����page��j����column
	LCD_CS=1;
	Rom_CS=1;
	column=column-1;						//ѡ���һ�п�ʼ��ʾ����������Ϊ1����ʵ����ָ���Ǵ�0��ʼ��
	page=page-1;							//ͬ��
	for(i=0;i<1;i++)
	  {
	      LCD12864_com(0XB0+(page+i));		//д��ҳ��ַ
		  LCD12864_com(0x10+(column>>4));	//д���е�ַ����λ
		  LCD12864_com(0x00+column&0X0F);
		  for(j=0;j<5;j++)					//����ֻд5λ������Ϊ�ֿ�ֻռ5��
		  	{
			    LCD12864_dat(*prt);
				prt++;						//���ｫָ���Լӣ���ҳ��ַΪ1ʱ����׼����ʾ�ֵ��°벿�ˣ�ָ���ֵ������ϴ�������
			}
	  
	  }
	LCD_CS=1;
}


/*Һ����ȡGB2312�ֿ���ʾ����*/
void LCD12864_GB2312_display(unsigned char page,unsigned char column, unsigned char *prt)
{
    unsigned char datatemp[32];
	unsigned char i=0,MSB=0,LSB=0,fontsize;				   //MSB--�ַ�����߰�λ�� LSB--�ַ������8λ,fontsize�������Ƶ��ò�ͬ����ʾ����
														   //fontsizeΪ32�������16*16�����Ϊ16,�����8*16
	unsigned int BaseAdd=0;								   //BaseAdd--������ַ��GB2312��ASSIC����ʼλ�ò�һ��
    for(i=0;i<32;i++)									   //�����ֿ���պ���Ҫ��������һ�ε����ݻᱣ����������
		datatemp[i]=0;
	i=0;
	while(*prt!=0)
	 {
		/*������壨GB2312���������ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
		MSB=*prt;										   //��ȡ��ַ���ַ�����߰�λ
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
		else if((MSB>=0x20) &&(MSB<=0x7e))				   //������ַ�����ֻ��8λ
		  {
				BaseAdd=0x3cf80;
				address = (MSB- 0x20);
				address = (unsigned long)(address*16);
				address = (unsigned long)(address+BaseAdd);			
				fontsize=16;
		  }
		prt++;									  //���ж����������λ1�λ���0�κ�������ƶ�1λ��׼��������һ�ε�ַ
		add_H=(unsigned char)((address&0xff0000)>>16);			  //����ע�����ȼ������⣬����������ȼ�����λ�����ͣ�������Ǽ����ţ��������λ
		add_M=(unsigned char)((address&0xff00)>>8);
		add_L=(unsigned char)(address&0xff);

		Rom_Send_Com(add_H,add_M,add_L);		  //�ͳ���ַ����
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

/*Һ����ȡ5x7�ֿ���ʾ����*/
void LCD12864_5X7_display(unsigned char page,unsigned char column, unsigned char *prt)
{
    unsigned char datatemp[8];
	unsigned char i=0,MSB=0,LSB=0;				   //MSB--�ַ�����߰�λ�� LSB--�ַ������8λ,fontsize�������Ƶ��ò�ͬ����ʾ����
														   //fontsizeΪ32�������16*16�����Ϊ16,�����8*16
	unsigned int BaseAdd=0;								   //BaseAdd--������ַ��GB2312��ASSIC����ʼλ�ò�һ��
    for(i=0;i<8;i++)									   //�����ֿ���պ���Ҫ��������һ�ε����ݻᱣ����������
		datatemp[i]=0;
	i=0;
	while(*prt!=0)
	 {
		/*������壨GB2312���������ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
		MSB=*prt;										   //��ȡ��ַ���ַ�����߰�λ
		if(((MSB>=0x20) &&(MSB<=0x7e)))
		  {
				BaseAdd=0x3bfc0;
				address = (MSB- 0x20);
				address = (unsigned long)(address*8);
				address = (unsigned long)(address+BaseAdd);			
				prt++;
		  }
		add_H=(unsigned char)((address&0xff0000)>>16);			  //����ע�����ȼ������⣬����������ȼ�����λ�����ͣ�������Ǽ����ţ��������λ
		add_M=(unsigned char)((address&0xff00)>>8);
		add_L=(unsigned char)(address&0xff);

//		Rom_Send_Com(add_H,add_M,add_L);		  //�ͳ���ַ����
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

/*Һ��оƬ��ʼ��*/
void LCD12864_init()
{
	LCD_CS=0;
	Rom_CS = 1;
	LCD_RST=0;                //�͵�ƽ��λ
	delayMs(10);
	LCD_RST=1;		          //��λ���
	delayMs(10);  
	
	LCD12864_com(0xE2);		  //�����λ
	delayMs(5);      

	LCD12864_com(0x2F);		  //���õ�Դ����
	delayMs(5);				  //��ʱ5ms

	LCD12864_com(0x81);		  //���öԱȣ�����Ǳ���ģ����ֵ��Ҫ���ϵĵ������Դﵽ��õ���ʾЧ��
	LCD12864_com(0x20);		  //����ֵΪ45���ù�����Ҫ�������������

	LCD12864_com(0xC8);		  //������ɨ�跽ʽ�����ϵ���

	LCD12864_com(0xA0);		  //������ɨ�跽ʽ����������

	LCD12864_com(0x40);		  //�ӵ�һ�п�ʼ��ʾ��ʵ���Ͽ��Ƶ��ǹ��������
	
	LCD12864_com(0xA2);	      //1/9ƫѹ�ȣ�bias��

	LCD12864_com(0xAF);		  //����ʾ
	
	LCD_CS=1;

}