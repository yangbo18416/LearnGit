/*
��������:LCD12864�Ļ���Ӧ��
����ԭ��:ͨ����ȡ�����ַ���Ϣ��Ȼ�������LCD12864
����Ҫ��:1��LCD12864MINI�Ĵ�����ͨ��SPIͨѶ���д���ģ���˴˴�Ҫ�˽�SPIЭ����÷�
		 2�������ֺ����ݵĶ�ȡ��д��
		 3���˿ڵĿ���
��дʱ��:2013-09-24
*/

#include<reg52.h>
#include "delay.h"

/*��ʱMS ��ÿһ����ʱ�����Լ1ms����Ȼ������ھ���Ϊ12MHz�������*/
void delayMs(unsigned char i)
{
	while(--i)
		{
			delayUs(245);
			delayUs(245);		
		}
}

void delayUs(unsigned char i)
{
	while(--i);
}
//��ʱ1s
void delayTs(unsigned char i)
{
	while(i--)
		{
	  	   delayMs(250);
		   delayMs(250);
		   delayMs(250);
		   delayMs(250);
		}
}