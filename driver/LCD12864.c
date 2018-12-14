/*
��������:LCD12864�Ļ���Ӧ��
����ԭ��:ͨ����ȡ�����ַ���Ϣ��Ȼ�������LCD12864
����Ҫ��:1��LCD12864MINI�Ĵ�����ͨ��SPIͨѶ���д���ģ���˴˴�Ҫ�˽�SPIЭ����÷�
		 2�������ֺ����ݵĶ�ȡ��д��
		 3���˿ڵĿ���
		 4����ַ�����Ķ��壬��������ڳ������ʱ�����ڽ�����ַ�ı�������Ϊ�޷������ͣ����¶�ȡ��ĸʱ������Ϊ�Ǹ�ʱ��Ļ���ַ�Ѿ�������65535��
		 5�����ȼ������⣬�ڵ�һ�ε���ʱ��������ô�����õ���ֵ�����ԣ�ԭ�������ڽ���address&0xFF0000>>4�Ĳ���ʱ���Ǽ������ˡ�������λ�����ȼ������룬���Գ���
		 6��������ǿ��һ�㣬��ʱ��ȡ��ַ���Ƕγ���ŵ�VC���棬û�п��������ܹ�������ȷֵ�������ڸõ���ά64λ�ģ�����int��long����4���ֽڡ�����Ƭ����
		 	��int��long �������в��ģ����ԲŻ��VC��û���⣬��Ƭ���������������
��дʱ��:2013-09-24
*/

#include<reg52.h>
#include "LCD12864.h"
#include "delay.h"
#include "ziku.h"

unsigned long int address=0X00000000;



/*�ֿⷢ���ֽں�����ѡ���͵ĵ�ַ,ѡ���λ���ݴ���*/
void ROM_Send(unsigned char add)				 //��λ���ݵķ��ͣ����룬����������
{
   unsigned char i;
   Rom_CS=0;										 //��ʼ����ѡ���ֿ�
   for(i=0;i<8;i++)
   	 {
			 if(add&0X80)
				 LCD_SI=1;							//ROMд�����ݣ����ߺ�LCD�ĸ���
			 else
				 LCD_SI=0;							//ROMд1
			 LCD_CLK=0;								//ʱ���ź�����
			 LCD_CLK=1;								//ʱ���ź����ߣ��γ������أ�����д��
			 add<<=1;
	 }										   			//���ﲻ�ܸ���Rom_CS���ߣ�����ʱ��ͼ���ñ��ֵ͵�ƽ�����ݶ�ȡ���
}

/*�ֿ�����ֽں���,��λһ��*/

unsigned char ROM_Read()				      //��λ���ݵķ���,��ȡ���½������
{
   unsigned char i,dat=1;
   LCD_CLK=1;
   for(i=0;i<8;i++)
   	 {
			 LCD_CLK=0;										 //�γ��½��أ����ݶ���
			 if(Rom_SO)										 //�ж������Ƿ�Ϊ1�����͸�λ
				dat=dat<<1|0x01;
			 else
				dat=dat<<1|0x00;
			 LCD_CLK=1;										//���ߣ�׼����һ�����Ͳ���
	 }
	return dat;
}

/*�ֿⷢ��������ƣ����е���λ�ַ�*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L)
{
	  Rom_CS=0;											//���ͣ�׼�������ȡ����
		ROM_Send(0x03);								//����ָ�03��׼����ȡ����
		ROM_Send(add_H);
		ROM_Send(add_M);
		ROM_Send(add_L);
}

/*Һ��оƬдָ��*/
void LCD12864_com(unsigned char com)
{
	unsigned char i;
	LCD_DC=0;					  //��������Ϊָ��  ��DC=1---����  DC=0---ָ��)
	LCD_CS=0;					  //SPI����д��CSΪ�ͣ�������д��
  for(i=0;i<8;i++)
   	 {
			 if(com&0X80)
				 LCD_SI=1;							//ROMд�����ݣ����ߺ�LCD�ĸ���
			 else
				 LCD_SI=0;							//ROMд1
			 LCD_CLK=0;								//ʱ���ź�����
			 LCD_CLK=1;								//ʱ���ź����ߣ��γ������أ�����д��
			 com<<=1;									//�ȷ��͸�λ D7-->D0
		 }										   			
	LCD_CS=1;											//д������ϣ�Ƭѡ����
}

/*Һ��оƬд����*/
void LCD12864_dat(unsigned char dat)
{
	unsigned char i;
	LCD_DC=1;					  //��������Ϊ����  ��DC=1---����  DC=0---ָ��)
	LCD_CS=0;					  //SPIд���ݣ�Ƭѡ����

  for(i=0;i<8;i++)
   	 {
			 if(dat&0X80)
				 LCD_SI=1;							//��LCD����оƬд����
			 else
				 LCD_SI=0;							//д1
			 LCD_CLK=0;								//ʱ���ź�����
			 LCD_CLK=1;								//ʱ���ź����ߣ��γ������أ�����д��
			 dat<<=1;									//�ȷ��͸�λ D7-->D0
		 }										   			
	LCD_CS=1;											//д������ϣ�Ƭѡ����
}

/*Һ����������*/
void LCD12864_CLR()
{
	unsigned char i,j;			  //�������е�����ʾ����д0
	LCD_CS=0;					        //Ԥ������
	Rom_CS = 1;	
	for(i=0;i<9;i++)			  //��������ʾ������LCD12864�ǰ�ҳ�����ֵģ��ܹ�128�У�����datasheet����ʾ��132�У�64�У�����һ��д8��bit�����Ϊ8ҳ
	  {
	  	LCD12864_com(0xB0+i);	  //����д��OXB0+i��ԭ�������ڣ�����ҳ�Ŀ����ָ���λ�ǹ̶��ģ�����λ��ֵ�Ϳ�����ҳ
   	  LCD12864_com(0x10);  	  //�е�ַ�ĸ���λ����Ȼ������Բ���д��ѭ���ڣ���ΪҺ��оƬ���Զ���1�ģ�������ָ�Ĭ�ϼ�1��
	    LCD12864_com(0X00);     //�е�ַ�ĵ���λ
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
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�,flag--ѡ���Ƿ�ת��ʾ*/
void LCD12864_16X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag)
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
			    if(flag)
						LCD12864_dat(*prt);
					else
						LCD12864_dat(~(*prt));
				prt++;						//���ｫָ���Լӣ���ҳ��ַΪ1ʱ����׼����ʾ�ֵ��°벿�ˣ�ָ���ֵ������ϴ�������
			}
	  
	  }
	LCD_CS=1;
}

/*Һ��8*16��8���Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_8X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag)
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
			    if(flag)
						LCD12864_dat(*prt);
					else
						LCD12864_dat(~(*prt));
				prt++;						//���ｫָ���Լӣ���ҳ��ַΪ1ʱ����׼����ʾ�ֵ��°벿�ˣ�ָ���ֵ������ϴ�������
			}
	  
	  }
	LCD_CS=1;
}

/*Һ��5*7��5���Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_5X7(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag)
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
			    if(flag)
						LCD12864_dat(*prt);
					else
						LCD12864_dat(~(*prt));
					prt++;						//���ｫָ���Լӣ���ҳ��ַΪ1ʱ����׼����ʾ�ֵ��°벿�ˣ�ָ���ֵ������ϴ�������
			}
	  
	  }
	LCD_CS=1;
}

/*Һ����ȡGB2312�ֿ���ʾ����*/
void LCD12864_GB2312_display(unsigned char page,unsigned char column, unsigned char *prt,unsigned char flag)
{
	unsigned char add_H,add_M,add_L;		 				//���������м��������յ�ַ����
  unsigned char datatemp[32];
	unsigned char i=0,fontsize;				               //fontsizeΪ32�������16*16�����Ϊ16,�����8*16
	unsigned long BaseAdd=0;							   //BaseAdd--������ַ��GB2312��ASSIC����ʼλ�ò�һ��,����ע�⣬������ַӦ�ö���Ϊ�����ͣ�����
														   //�ں��ڴ����ַ�����ʱ�ͻ����
	while(*prt!=0)
	 {
		if(((*prt>=0xb0) &&(*prt<=0xf7))&&(*(prt+1)>=0xa1))
		  {
				BaseAdd=0x00;
		/*������壨GB2312���������ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
				address =(*prt-0xB0) * 94;
				address+=(*(prt+1)-0xA1)+846;
				address=( unsigned long )address*32+BaseAdd;
				fontsize=32;
				prt+=2;

		  }
		else if((*prt>=0xA1) &&(*prt<=0xA3)&&(*(prt+1)>=0xA1))
		  {
				BaseAdd=0x00;
		/*������壨GB2312��15x16����ַ����ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
		/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
		/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
				address =(*prt-0xa1) * 94;
				address+=(*(prt+1)-0xA1);
				address=( unsigned long )address*32+BaseAdd;
		  	fontsize=32;
				prt+=2;
		  }
		else if((*prt>=0x20) &&(*prt<=0x7e))				   //������ַ�����ֻ��8λ
		  {
				BaseAdd=0x3cf80;
				address = (*prt- 0x20);
				address = (unsigned long)(address*16);
				address = (unsigned long)(address+BaseAdd);			
				fontsize=16;
				++prt;
		  }
		add_H=(unsigned char)((address&0xff0000)>>16);			  //����ע�����ȼ������⣬����������ȼ�����λ�����ͣ�������Ǽ����ţ��������λ
		add_M=(unsigned char)((address&0xff00)>>8);
		add_L=(unsigned char)(address&0xff);

		Rom_Send_Com(add_H,add_M,add_L);		  				  //�ͳ���ַ����
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

/*Һ����ȡ5x7�ֿ���ʾ����*/
void LCD12864_5X7_display(unsigned char page,unsigned char column, unsigned char *prt,unsigned char flag)
{
 	unsigned char add_H,add_M,add_L;		 //���������м��������յ�ַ����
  unsigned char datatemp[8];
	unsigned char i=0,MSB=0,LSB=0;			//MSB--�ַ�����߰�λ�� LSB--�ַ������8λ,fontsize�������Ƶ��ò�ͬ����ʾ����
											//fontsizeΪ32�������16*16�����Ϊ16,�����8*16
	unsigned long BaseAdd=0;				//BaseAdd--������ַ��GB2312��ASSIC����ʼλ�ò�һ��
  for(i=0;i<8;i++)						//�����ֿ���պ���Ҫ��������һ�ε����ݻᱣ����������
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

		Rom_Send_Com(add_H,add_M,add_L);		  //�ͳ���ַ����
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

/*Һ��оƬ��ʼ��*/
void LCD12864_init()
{
	LCD_CS=0;									//LCDƬѡ�ź�
	Rom_CS = 1;								//�ֿ�ROMƬѡ�ź�
/*Ŀǰʹ�õ�оƬ�Դ��ϵ��Զ���λ����
	LCD_RES = 0;							//LCD��λ�ź�����
	delayMs(10);							//��ʱ10ms
	LCD_RES = 1��							//��λ���
*/ 
	delayMs(50);							//��ʱ20ms
	LCD12864_com(0xAE);				//�ر�LCD��ʾ
	
	LCD12864_com(0xD5);				//������ʾʱ��duty
	LCD12864_com(0x80);				//���þ���Ƶ��
	
	LCD12864_com(0xA8);				//����COM����·�� ���ܼ�comΪ64·��0-63�������ʾcomȫ������
	LCD12864_com(0x3F);
	
	LCD12864_com(0x20);				//���ô洢��ַģʽ--ҳ��ַģʽ
	LCD12864_com(0x02);
	
	LCD12864_com(0xD3);				//����ˮƽλ��Ϊ0�������ô�0�п�ʼ��ʾ
	LCD12864_com(0x00);
	
	LCD12864_com(0x40);				//������ʾ��ʼ��Ϊ0��
	

	LCD12864_com(0xA1);			//���ض�λ column address 0 is mapped to SEG0
/*������������ʵ�ֵ�Ч����ɨ����ϵ��£�ɨ��*/
	LCD12864_com(0xC8);			//��ɨ��ģʽ����COM63-COM00
	
	LCD12864_com(0xDA);
	LCD12864_com(0x12);	
	
	LCD12864_com(0x81);			//�Աȶ�����
	LCD12864_com(0XCF);

	LCD12864_com(0XD9);			//Ԥ���ʱ������
	LCD12864_com(0X22);			//Ĭ��ֵ

	LCD12864_com(0XDB);			//COM�˿�ȡ��ѡ���ѹ����
	LCD12864_com(0X20);
	
	LCD12864_com(0XA4);			//����ʾ
	LCD12864_com(0XCF);			//��ʾ��ʽ-����/��ת
	
	LCD12864_com(0x8D);			//Set Charge Pump
	LCD12864_com(0x14);
	
	LCD12864_com(0XAF);			//����ʾ
	
	delayMs(50);						//��ʱ100ms
	
	LCD_CS=1;

}