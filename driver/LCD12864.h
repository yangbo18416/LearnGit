/*
��������:LCD12864�Ļ���Ӧ��
����ԭ��:ͨ����ȡ�����ַ���Ϣ��Ȼ�������LCD12864
����Ҫ��:1��LCD12864MINI�Ĵ�����ͨ��SPIͨѶ���д���ģ���˴˴�Ҫ�˽�SPIЭ����÷�
				 2�������ֺ����ݵĶ�ȡ��д��
				 3���˿ڵĿ���
				 4��LCD��ʾ���Ʋ�ȡ����SPI���ƣ���˿��Խ����SPI_Rom��������
��дʱ��:2015-09-25
*/
#ifndef __LCD12864_h__
#define __LCD12864_h__

/*�˴��˿ڶ���ΪLCD12864��ʾ�˿�*/
sbit LCD_CLK = P2^0;						//LCD�Լ��ֿ�оƬ����ʱ�Ӷ˿�
sbit LCD_SI  = P2^1;						//LCD�Լ��ֿ�оƬ���õ�MOSI�ܽţ������оƬ��������
sbit LCD_DC  = P2^2;						//LCD����оƬ������/���� ѡ�� 0--��� 1--����
sbit LCD_CS	 = P2^3;						//LCDƬѡ�ź�
sbit Rom_SO	 = P2^4;						//Rom���������
sbit Rom_CS	 = P2^5;						//�ֿ�оƬƬѡ�ź�

///*�ֿ�оƬ������*/
//unsigned char Read_Rom(unsigned char);

/*�����ֽں�����ѡ���͵ĵ�ַ,ѡ���λ���ݴ���*/
void ROM_Send(unsigned char add);				 //��λ���ݵķ���
/*�����ֽں���,��λһ��*/
unsigned char ROM_Read();				      //��λ���ݵķ���
/*���ִ洢��ַ��ȡ*/

void LCD12864_address(unsigned char * prt);

/*����������ƣ����е���λ�ַ�*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L);
/*Һ��оƬдָ��*/
void LCD12864_com(unsigned char com);

/*Һ��оƬд����*/
void LCD12864_dat(unsigned char dat);

/*Һ��оƬ��ʼ��*/
void LCD12864_init();

/*Һ����������*/
void LCD12864_CLR();

/*Һ��16*16�Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_16X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*Һ��8*16��8���Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_8X16(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*Һ��5*7��5���Ա������������*/
/*page--��ʾҳ��column--��ʾ�г�ʼ�� prt--�Ա��ֿ�*/
void LCD12864_5X7(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*Һ����GB2312ȡ�ֿ���ʾ����*/
void LCD12864_GB2312_display(unsigned char page,unsigned char column,unsigned char *prt,unsigned char flag);

/*Һ����ȡ5x7�ֿ���ʾ����*/
void LCD12864_5X7_display(unsigned char page,unsigned char column, unsigned char *prt,unsigned char flag);


#endif