#ifndef __Rom_SPI_h__
#define __Rom_SPI_h__

/*�˴��˿ڶ���Ϊ�ֿ�оƬͨѶ�˿�*/
sbit Rom_CS = P0^5;
sbit Rom_CLK= P0^4;
sbit Rom_SI = P0^3;
sbit Rom_SO = P0^2;

/*�����ֽں�����ѡ���͵ĵ�ַ,ѡ���λ���ݴ���*/
void ROM_Send(unsigned char add);				 //��λ���ݵķ���
/*�����ֽں���,��λһ��*/
unsigned char ROM_Read();				      //��λ���ݵķ���
/*���ִ洢��ַ��ȡ*/
void LCD12864_address(unsigned char * prt);

/*����������ƣ����е���λ�ַ�*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L);

#endif