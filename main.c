/*
��������:LCD12864����ȡ�ֿ����ݵ�Ӧ��
����ԭ��:ͨ����ȡ�����ַ���Ϣ��Ȼ�������LCD12864
����Ҫ��:1��LCD12864MINI�Ĵ�����ͨ��SPIͨѶ���д���ģ���˴˴�Ҫ�˽�SPIЭ����÷�
				 2�������ֺ����ݵĶ�ȡ��д��
				 3 ���˿ڵĿ���

����bug: 1���޷���ʾ���֡�����
��дʱ��:2013-09-24
*/
#include<reg52.h>
#include<math.h>
#include<intrins.h>
#include "stdio.h"
#include "LCD12864.h"
#include "LCD_Menu.h"
#include "ziku.h"
void main()
{
	LCD12864_init();
	LCD12864_CLR();
	Time0_Init();
	while(1)
	  {

	  }
}