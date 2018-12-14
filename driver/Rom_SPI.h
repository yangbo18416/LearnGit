#ifndef __Rom_SPI_h__
#define __Rom_SPI_h__

/*此处端口定义为字库芯片通讯端口*/
sbit Rom_CS = P0^5;
sbit Rom_CLK= P0^4;
sbit Rom_SI = P0^3;
sbit Rom_SO = P0^2;

/*发送字节函数。选择发送的地址,选择八位数据传输*/
void ROM_Send(unsigned char add);				 //八位数据的发送
/*接收字节函数,八位一收*/
unsigned char ROM_Read();				      //八位数据的发送
/*汉字存储地址获取*/
void LCD12864_address(unsigned char * prt);

/*发送命令控制，高中低三位字符*/
void Rom_Send_Com(unsigned char add_H,unsigned char add_M, unsigned char add_L);

#endif