/*����ͷ�ļ�*/
#include<reg52.h>
#include "math.h"
#include "stdio.h"
#include "ziku.h"
#include "LCD_Menu.h"
#include "delay.h"
#include "LCD12864.h"

#define KSJZ 3									//������ټӼ�����ֵ						
static unsigned int func_index;
static unsigned int old_index;

extern xdata Menu_table table[50];
unsigned char RED_LIGHT,GREEN_LIGHT,BLUE_LIGHT,WHITE_LIGHT,Occupy,STROB;
unsigned int DMX_add;
unsigned int Cal_red_factor ;
unsigned int Cal_green_factor ;
unsigned int Cal_blue_factor ;
unsigned int Cal_white_factor ;
unsigned char TJ_Flag=0;						//�ò����������ƼӼ������ǶԲ˵����в������Ƕ���ֵ��������
unsigned char AT_Num= 0;						//AT��������
unsigned char PR_Num= 0;						//PR��������
void (*current_operation_index)(void);

/*
**��������: �����������
**�������: ��
**���������
**˵    ������ȡ��Ӧ�İ���ֵ
*/
void Key_Process(void)
	{
      volatile unsigned char KEY=0x00; 
		  if((P1&0X0F)!=0X0F)				/*�����������˵���а���������*/
			{
				delayMs(10);						//��ʱ10����ȥ��
				if((P1&0X0F)!=0X0F)		//��ʱ�󰴼���Ȼ���£�˵��������Ч
					{
						KEY=(P1&0X0F);		//��ȡ����ֵ
						while((P1&0X0F)!=0X0F)
							{
								if(TJ_Flag!=0)			//����ǲ˵��Ӽ���������ѭ���������п��
									{
										KEY=(~KEY)&0X0F;
										delayMs(60);		//�˴�����ʱ�����Դﵽ�������ټӵ��ٶ�
										if(KEY==0X02)														//�Ӱ���
											{
												Key_Up();
											}
										if(KEY==0X04)														//������
											{
												Key_Down();
											}
							}
								else
									continue;																//�˴���Ҫ����continue��䣬�����ȴ�ѭ��
								(*current_operation_index)();			     //ˢ����ʾ											
//											;
											/*�ɷ�����ʱ�ȴ�����Ҫִ�е�����*/
							}
					}
			}
		 KEY=(~KEY)&0X0F;													//ͨ�������Ĵ�����Ӧ�İ���������ʾ�ߵ�ƽ
		 if(KEY==0X01)														//���˵�����--���ؼ�
				{
					Key_Menu();
				}
		 if(KEY==0X02)														//�Ӱ���
				{
					Key_Up();
				}
		 if(KEY==0X04)														//������
				{
					Key_Down();
				}
		 if(KEY==0X08)														//ȷ�ϰ���
				{
					Key_OK();													
				}
		current_operation_index=table[func_index].current_operation;//�����Ӧ��ִ�к���
		(*current_operation_index)();	
			
	}
/*
**��������: ��ʱ����ʼ������
**�������: ��
**�����������
**˵    ������ʱ�����������ɰ�����ʱɨ��/������ʾ����
*/
void Time0_Init(void)
	{
			TMOD |= 0X01;							//��ʱ��0��������ģʽ2��16λ����ģʽ
		  TH0 =(65535-5000)%256;		//��ʱ��2ms��һ���ж�
		  TL0 =(65535-5000)/256;
			ET0 =1;										//�򿪶�ʱ���ж�ʹ��
			EA  =1;										//�����жϿ���
			TR0	=1;										//�򿪶�ʱ��
	}
/*
**��������: ��ʱ���жϴ������
**�������: ��
**�����������
**˵    ������ʱ�����������ɰ�����ʱɨ��/������ʾ����
*/
void Time0_Interrupt(void) interrupt 1
	{
			TH0 =(65535-5000)%256;		//��ʱ��2ms��һ���ж�
		  TL0 =(65535-5000)/256;	
		  Key_Process();
	}	
	
//����-�˵�����
void Key_Menu(void)
	{
		func_index=table[func_index].Home;
		LCD12864_CLR();
		TJ_Flag=0;
	}
//ȷ�ϰ���
void Key_OK(void)
	{
		old_index=func_index;								//��ȷ�ϼ�ʱ���ȴ洢����״̬
		func_index=table[func_index].enter;
		LCD12864_CLR();
		TJ_Flag=0;
	}
//�����Ӳ���
void Key_Up(void)
	{
		if(TJ_Flag)													//��������Ҫ������ֵ�Ĳ˵������������
			{
					if(old_index==3)							//������������ڲ���ʱѡ��ΪDMX����
						{
							 if(DMX_add<511)
									DMX_add++;
							 else
									DMX_add=0;
						}	
				    if(old_index==7)							//������������ڲ���ʱѡ��Ϊ��ɫ
						{
							 if(RED_LIGHT<254)
									RED_LIGHT++;
							 else
									RED_LIGHT=0;
						}
					if(old_index==8)							//������������ڲ���ʱѡ��ΪGreen
						{
							 if(GREEN_LIGHT<254)
									GREEN_LIGHT++;
							 else
									GREEN_LIGHT=0;
						}
					if(old_index==9)							//������������ڲ���ʱѡ��ΪBlue
						{
							 if(BLUE_LIGHT<254)
									BLUE_LIGHT++;
							 else
									BLUE_LIGHT=0;
						}
					if(old_index==10)							//������������ڲ���ʱѡ��ΪWhite
						{
							 if(WHITE_LIGHT<254)
									WHITE_LIGHT++;
							 else
									WHITE_LIGHT=0;
						}	
					if(old_index==11)							//������������ڲ���ʱѡ��ΪStrob
						{
							 if(STROB<25)
									STROB++;
							 else
									STROB=0;
						}
					if(old_index==12)							//������������ڲ���ʱѡ��ΪStrob
						{
							 if(Occupy<99)
									Occupy++;
							 else
									Occupy=1;
						}
					if(old_index==27)							//������������ڲ���ʱѡ��ΪAT
						{
							 if(AT_Num<10)
									AT_Num++;
							 else
									AT_Num=1;
						}
					if(old_index==28)							//������������ڲ���ʱѡ��ΪPR
						{
							 if(PR_Num<10)
									PR_Num++;
							 else
									PR_Num=1;
						}
					if(old_index==45)							//������������ڲ���ʱѡ��ΪCAL_RED
						{
							 if(Cal_red_factor<255)
									Cal_red_factor++;
							 else
									Cal_red_factor=0;
						}
					if(old_index==46)							//������������ڲ���ʱѡ��ΪCAL_GREEN
						{
							 if(Cal_green_factor<255)
									Cal_green_factor++;
							 else
									Cal_green_factor=0;
						}	
					if(old_index==47)							//������������ڲ���ʱѡ��ΪCAL_BLUE
						{
							 if(Cal_blue_factor<255)
									Cal_blue_factor++;
							 else
									Cal_blue_factor=0;
						}
					if(old_index==48)							//������������ڲ���ʱѡ��ΪCAL_White
						{
							 if(Cal_white_factor<255)
									Cal_white_factor++;
							 else
									Cal_white_factor=0;
						}													
			}
		else
			{
				func_index=table[func_index].up;
			}	
	}
//����������
void Key_Down(void)
	{
		if(TJ_Flag)
			{
					if(old_index==3)							//������������ڲ���ʱѡ��ΪStrob
						{
							 if(DMX_add>0)
									DMX_add--;
							 else
									DMX_add=512;
						}
					if(old_index==7)							//������������ڲ���ʱѡ��Ϊ��ɫ
						{
							 if(RED_LIGHT>0)
									RED_LIGHT--;
							 else
									RED_LIGHT=255;
						}
					if(old_index==8)							//������������ڲ���ʱѡ��ΪGreen
						{
							 if(GREEN_LIGHT>0)
									GREEN_LIGHT--;
							 else
									GREEN_LIGHT=255;
						}
					if(old_index==9)							//������������ڲ���ʱѡ��ΪBlue
						{
							 if(BLUE_LIGHT>0)
									BLUE_LIGHT--;
							 else
									BLUE_LIGHT=255;
						}
					if(old_index==10)							//������������ڲ���ʱѡ��ΪWhite
						{
							 if(WHITE_LIGHT>0)
									WHITE_LIGHT--;
							 else
									WHITE_LIGHT=255;
						}	
					if(old_index==11)							//������������ڲ���ʱѡ��ΪStrob
						{
							 if(STROB>0)
									STROB--;
							 else
									STROB=25;
						}
					if(old_index==12)							//������������ڲ���ʱѡ��ΪOccupy
						{
							 if(Occupy>1)
									Occupy--;
							 else
									Occupy=99;
						}
					if(old_index==27)							//������������ڲ���ʱѡ��ΪAT
						{
							 if(AT_Num>1)
									AT_Num--;
							 else
									AT_Num=10;
						}
					if(old_index==28)							//������������ڲ���ʱѡ��ΪPR
						{
							 if(PR_Num>1)
									PR_Num--;
							 else
									PR_Num=10;
						}
					if(old_index==45)							//������������ڲ���ʱѡ��ΪCAL_RED
						{
							 if(Cal_red_factor>0)
									Cal_red_factor--;
							 else
									Cal_red_factor=255;
						}
					if(old_index==46)							//������������ڲ���ʱѡ��ΪCAL_GREEN
						{
							 if(Cal_green_factor>0)
									Cal_green_factor--;
							 else
									Cal_green_factor=255;
						}	
					if(old_index==47)							//������������ڲ���ʱѡ��ΪCAL_BLUE
						{
							 if(Cal_blue_factor>0)
									Cal_blue_factor--;
							 else
									Cal_blue_factor=255;
						}
					if(old_index==48)							//������������ڲ���ʱѡ��ΪCAL_white
						{
							 if(Cal_white_factor>0)
									Cal_white_factor--;
							 else
									Cal_white_factor=255;
						}														
			}
		else
			{
				func_index=table[func_index].down;
			}	
	}
/*
**��������: ��ʼ�˵���ʾ
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_STAT(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}
/*
**��������: ����ģʽ
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/		
void Menu_AUTO(void)
	{		
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index].text,0);
			LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
	}	
/*
**��������: ����ģʽ
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/		
void Menu_RUN(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}
/*
**��������: DMX��ַ����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_DMX(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}	
/*
**��������: Ԥ��ģʽ
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_PERS(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}
/*
**��������: �ƾ�����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/
void Menu_SET(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}	
/*
**��������: ƫ�����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_CAL2(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}
/*
**��������: STAT-RED����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/
void Menu_Red(void)
	{
		  LCD12864_GB2312_display(1,1, table[func_index].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}	
/*
**��������: STAT-Green����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_Green(void)
	{		
		  LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
	}
/*
**��������: STAT-Blue����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/		
void Menu_Blue(void)
	{	
		 	LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}	
/*
**��������: STAT-White����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/		
void Menu_White(void)
	{	
		 	LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**��������: STAT-Strob����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/		
void Menu_Strob(void)
	{		
		  LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**��������: STAT-Occupy����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/
void Menu_Occupy(void)
	{
		  LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);	
	}	
/*
**��������: SET-UPLD����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_UPLD(void)
	{
		  table[49].Home=func_index;						//����OK�󣬷���ʱ��Ȼ���ص�UPLD��
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}
/*
**��������: SET-RESET����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/
void Menu_REST(void)
	{
		  table[49].Home=func_index;
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
	}	
/*
**��������: SET-ID����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_ID(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}
/*
**��������: SET-RGBW����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_RGBW(void)
	{
		  table[49].Home=old_index;																	//����ȷ�ϼ��ķ���ֵ		
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}	
/*
**��������: SET-POW����
**�������: ��
**�����������
**˵    �������ڹ���
*/	
void Menu_POW(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**��������: SET-DIM����
**�������: ��
**�����������
**˵    ������������ѡ��
*/	
void Menu_DIM(void)
	{
		  table[49].Home=old_index;																	//����ȷ�ϼ��ķ���ֵ
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**��������: SET-CLA2-RGBW����
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ
*/	
void Menu_CRGB(void)
	{
		  table[49].Home=old_index;	
		  LCD12864_GB2312_display(1,1, table[func_index].text,0);
	}
/*
**��������: SET-DIM-OFF����
**�������: ��
**�����������
**˵    �����رյ���DIM
*/
void Menu_OFF(void)
	{
			table[49].Home=func_index;																	//����ȷ�ϼ��ķ���ֵ				
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}
/*
**��������: SET-DIM-DIM1����
**�������: ��
**�����������
**˵    ����DIM1
*/	
void Menu_DIM1(void)
	{
		  table[49].Home=func_index;																	//����ȷ�ϼ��ķ���ֵ		
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
	}	
/*
**��������: SET-DIM-DIM2����
**�������: ��
**�����������
**˵    ����DIM2
*/		
void Menu_DIM2(void)
	{
		  table[49].Home=func_index;																	//����ȷ�ϼ��ķ���ֵ		
			LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}
/*
**��������: SET-DIM-DIM3����
**�������: ��
**�����������
**˵    ����DIM13
*/		
void Menu_DIM3(void)
	{
		  table[49].Home=func_index;																	//����ȷ�ϼ��ķ���ֵ		
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**��������: SET-DIM-DIM4����
**�������: ��
**�����������
**˵    ����DIM4
*/		
void Menu_DIM4(void)
	{
		  table[49].Home=func_index;																	//����ȷ�ϼ��ķ���ֵ		
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**��������: �������ں���
**�������: ��
**�����������
**˵    ������RGBW Strob����+ AUTO ģʽ����+ƫ�����
*/		
void Menu_NumSet(void)
	{	
		unsigned char temp[5];												//�����洢���ݱ���		
/*���ݴ��ݹ����������������в�ѯ���ý����ĸ�����*/	
		table[49].Home=old_index;											//����ȷ�ϼ��󣬰����ذ����ĵ�ַ��Ϣ
		table[25].Home=old_index;											//����ȷ�ϼ��󣬰����ذ����ĵ�ַ��Ϣ
		TJ_Flag=1;
		switch (old_index)
			{
				case 7:													//��ɫ����
					{
						 sprintf((char *)temp,"%03d",(unsigned int)RED_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 8:													//��ɫ����
					{
						 sprintf((char *)temp,"%03d",(unsigned int)GREEN_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 9:													//��ɫ����
					{
						 sprintf((char *)temp,"%03d",(unsigned int)BLUE_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 10:													//��ɫ����
					{
						 sprintf((char *)temp,"%03d",(unsigned int)WHITE_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 11:													//�������
					{
						 sprintf((char *)temp,"%03d",(unsigned int)STROB);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 12:													//�������
					{
						 sprintf((char *)temp,"%03d",(unsigned int)Occupy);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
/*��ʼģʽ���� AT.01-AT.10 PR.01-PR.10*/					
				case 27:													//AT��ֵ����
					{
						 sprintf((char *)temp,"%02d",(unsigned int)AT_Num);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(1,33, temp,1);			//��ͬһ������ʾ
						 break;
					}	
				case 28:													//PR��ֵ����
					{
						 sprintf((char *)temp,"%02d",(unsigned int)PR_Num);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(1,33, temp,1);			//��ͬһ������ʾ
						 break;
					}
/*��ʼ����ƫ��*/
				case 45:													//��ɫƫ��
					{
						 sprintf((char *)temp,"%03d",(unsigned int) Cal_red_factor);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 46:													//��ɫƫ��
					{
						 sprintf((char *)temp,"%03d",(unsigned int) Cal_green_factor);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 47:													//��ɫƫ��
					{
						 sprintf((char *)temp,"%03d",(unsigned int) Cal_blue_factor);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 48:													//��ɫƫ��
					{
						 sprintf((char *)temp,"%03d",(unsigned int) Cal_white_factor);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}										
        default:
					{
						break;
					}
			}	
	}
/*
**��������: DMX-DMX_Setting
**�������: ��
**�����������
**˵    ����DMX��ַ����
*/		
void DMX_Setting(void)
	{
		unsigned char temp[5];												//�����洢���ݱ���	
		table[49].Home=old_index;											//����ȷ�ϼ��󣬰����ذ����ĵ�ַ��Ϣ
		TJ_Flag=1;
		sprintf((char *)temp,"%03d",(unsigned int)DMX_add);
		LCD12864_GB2312_display(1,1, table[func_index].text,1);
		LCD12864_GB2312_display(3,1, temp,1);
	
	}
/*
**��������: AUTO-AT�˵�
**�������: ��
**�����������
**˵    �����ò˵����ṩ���ݵ�������
*/	
void Auto_AT(void)
	{
			table[49].Home=old_index;																	//����ȷ�ϼ��ķ���ֵ
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
	}
/*
**��������: AUTO-PR�˵�
**�������: ��
**�����������
**˵    �����ò˵����ṩ���ݵ�������
*/		
void Auto_PR(void)
	{
			table[49].Home=old_index;																	//����ȷ�ϼ��ķ���ֵ
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
	}
/*
**��������: PERS�˵�ѡ������
**�������: ��
**�����������
**˵    �����ú���ͨ���ж�index��������ʾ�˵���
*/
void PERS_SET(void)
	{
/*
		��������������������в�ͬ����Ϊ�м�ʡȥ��һ���м价�ڣ���˽�OK���ķ���ֵ������Ϊ֮ǰ�������кţ��˴�Ϊ��ǰ������
		������ɫ��������Ϊ���м亯�����ȣ����Բ��ܲ�ȡ�˷�ʽ������ڵ��Թ����п��Ըо�����ͬ
*/		
		 table[49].Home=func_index;		
		 switch (func_index)
				{
			 case 31:
					{
							LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
							LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
							LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
							LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
						  break;
					}
			 case 32:
					{
							LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
							LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
							LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
							LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
						  break;
					}
			 case 33:
					{
							LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
							LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
							LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
							LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
						  break;
					}
			 case 34:
					{
							LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
							LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
							LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
							LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
						  break;
					}	
			 case 35:
					{
							LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
							LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
							LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
							LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
						  break;
					}	
			 case 36:
					{
							LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
							LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
							LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
							LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
						  break;
					}	
			 case 37:
					{
							LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
							LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
							LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
							LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
						  break;
					}
			 case 38:
					{
							LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
							LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
							LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
							LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
						  break;
					}	
			 default : break;
				}
	}	
void SET_DMX(void)
	{
			TJ_Flag = 0;																								//���ڲ˵�
			table[49].Home=func_index;																	//����ȷ�ϼ��ķ���ֵ
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
	}
void SET_SLAV(void)
	{
			TJ_Flag = 0;																								//���ڲ˵�
			table[49].Home=func_index;																	//����ȷ�ϼ��ķ���ֵ
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
	}
/*
**��������: SET-ID ����
**�������: ��
**�����������
**˵    �����ú���ͨ���ж�index��������ʾ�˵���
*/	
void ID_SET(void)
	{
		 TJ_Flag = 0;																								//���ڲ˵�
		 table[49].Home=func_index;		
		 switch (func_index)
				{
					 case 39:
							{
									LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
									LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
									break;
							}
					 case 40:
							{
									LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
									LCD12864_GB2312_display(3,1, table[func_index+0].text,0);	
									break;
							}
					 default: break;
				}
	}
/*
**��������: CAL2- RGBW ɫ�µ��ڿ���
**�������: ��
**�����������
**˵    �����ú���ͨ���ж�index��������ʾ�˵���
*/				
void RGBW_SET(void)
	{
		TJ_Flag = 0;																								//���ڲ˵�
		table[49].Home=func_index;		
		 switch (func_index)
				{
					 case 41:
							{
									LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
									LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
									break;
							}
					 case 42:
							{
									LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
									LCD12864_GB2312_display(3,1, table[func_index+0].text,0);	
									break;
							}
					 default: break;
				}
	}
/*
**��������: SET-POW ���ʵ���
**�������: ��
**�����������
**˵    �����ú���ͨ���ж�index��������ʾ�˵���
*/					
void POW_SET(void)
	{
		 TJ_Flag = 0;																								//���ڲ˵�
		 table[49].Home=func_index;		
		 switch (func_index)
				{
					 case 43:
							{
									LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
									LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
									break;
							}
					 case 44:
							{
									LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
									LCD12864_GB2312_display(3,1, table[func_index+0].text,0);	
									break;
							}
					 default: break;
				}
	}	
void RGBW_Color(void)
	{
		 TJ_Flag = 0;																								//���ڲ˵�
		 table[49].Home=func_index;		
		 switch (func_index)
				{
					 case 45:
							{
									LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
									LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
									LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
									LCD12864_GB2312_display(7,1, table[func_index+3].text,1);								
									break;
							}
					 case 46:
							{
									LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
									LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
									LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
									LCD12864_GB2312_display(7,1, table[func_index+2].text,1);										
									break;
							}
					 case 47:
							{
									LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
									LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
									LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
									LCD12864_GB2312_display(7,1, table[func_index+1].text,1);										
									break;
							}
					 case 48:
							{
									LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
									LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
									LCD12864_GB2312_display(5,1, table[func_index-1].text,1);	
									LCD12864_GB2312_display(7,1, table[func_index+0].text,0);										
									break;
							}						
					 default: break;
				}
	}		
	
/*
**��������: OKȷ�ϼ�
**�������: ��
**�����������
**˵    ������ʾ�����ʾ��Ϣ�����������ֵ���и���
*/		
void Menu_OK(void)
	{
		TJ_Flag=0;
		/*��ʼ�Դ��ݹ�����OKֵ���и��²���*/
	
		/*�������ݹ�����OKֵ���и���*/
	/*����״ֵ̬*/
//	    switch (old_index)
//	    	{
//	    	 	//RESETѡ��
//	    	 	case 14:
//	    	 		{
//	    	 			Sys_Reset();			//���ø�λ����
//	    	 			break;
//	    	 		}
//	    	 	//�������ߵ���
//	    		case 20:
//	    			{
//	    				break;
//	    			}
//	    		case 21:
//	    			{
//	    				break;
//	    			}
//	    		case 22:
//	    			{
//	    				break;
//	    			}
//	    		case 23:
//	    			{
//	    				break;
//	    			}
//	    		case 24:
//	    			{
//	    				break;
//	    			}
//	    
//	    	   //����ģʽ����
//				case 27:			//ATģʽ
//	    			{
//	    				if(AT_Num == 1)
//	    					{
//	    					}
//	    				if(AT_Num == 2)
//	    					{
//	    					}
//	    				if(AT_Num == 3)
//	    					{
//	    					}
//	    				if(AT_Num == 4)
//	    					{
//	    					}	    						    					
//	    				break;
//	    			}
//				case 28:			//PRģʽ
//	    			{
//	    				if(PR_Num == 1)
//	    					{
//	    					}
//	    				if(PR_Num == 2)
//	    					{
//	    					}
//	    				if(PR_Num == 3)
//	    					{
//	    					}    						    					
//	    				break;
//	    			}
//	    			//����ģʽ����  RUN_DMX RUN_SLAV
//	    		case 29:
//	    			{
//	    				break;
//	    			}
//	    		case 30:
//	    			{
//	    				break;
//	    			}	
//	    		//ͨ������
//	    		case 31:
//	    			{
//			      		break;
//	    			}
//	    		case 32:
//	    			{
//			      		break;
//	    			}
//	    		case 33:
//	    			{
//			      		break;
//	    			}
//	    		case 34:
//	    			{
//			      		break;
//	    			}
//	    		case 35:
//	    			{
//			      		break;
//	    			}
//	    		case 36:
//	    			{
//			      		break;
//	    			}	    				    				    			
//	    		case 37:
//	    			{
//	    				 break;
//	    			}
//	    		case 38:
//	    			{
//			      		break;
//	    			}
//	    			
//				//ID ���ÿ���
//	    		case 39:
//	    			{
//	    				break;
//	    			}
//	    		case 40:
//	    			{
//	    				break;
//	    			}
//	    		//ƫ��RGB����					
//	    		case 41:
//	    			{
//						break;
//	    			}
//	    		case 42:
//	    			{
//						break;
//	    			}
//	    		//���ʵ���
//	    		case 43:
//	    			{
//	    				break;
//	    			}
//	    		case 44:
//	    			{
//	    				break;
//	    			}
//	    		//ƫ����ڣ����±�־λ
//	    		case 45:
//	    			{
//	    				break;
//	    			}
//	    		case 46:
//	    			{
//	    				break;
//	    			}
//	    		case 47:
//	    			{
//	    				break;
//	    			}
//	    		case 48:
//	    			{
//	    				break;
//	    			}
//	    		default :break;	    			    				    				    	   
//	    				    			
//	    	}			     
//	    
	/*����״ֵ̬����*/		 	
		LCD12864_GB2312_display(2,1, table[func_index].text,1);	
	}
//��λ����
void Sys_Reset(void)
	{
//  //�������ã���ַ��1��
//			DMX_add =1;
//			DMX_HL[0] = (DMX_add >> 8) & 0X00000003;
//			DMX_HL[1] = DMX_add & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x01,DMX_HL,2);
//			Delay_ms(1);
//			
//			//�˴�����ѡ����һ��ʱ���������洢��ز���
//			Menu_Flag = 6 ;   //DIM
//			DMX_HL[0] = (Menu_Flag >> 8) & 0X00000003;
//			DMX_HL[1] = Menu_Flag & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x03,DMX_HL,2);
//			Delay_ms(1);
//			
//			Menu_Flag =2;   //ͨ������
//			Channel = HVS;
//			DMX_HL[0] = (Menu_Flag>>8) & 0X00000003;
//			DMX_HL[1] =Menu_Flag & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x05,DMX_HL,2);
//			Delay_ms(1);
//			
//         	//���ȵ���
//			RED_LIGHT = 0;
//			GREEN_LIGHT = 0;
//			BLUE_LIGHT = 0;
//			WHITE_LIGHT = 0;
//			Occupy = 0;
//			STROB = 0;
//			//Cal_LOW_factor = 255;				
//		    I2C_TEMP[0] = RED_LIGHT;
//			I2C_TEMP[1] = GREEN_LIGHT;
//			I2C_TEMP[2] = BLUE_LIGHT;
//			I2C_TEMP[3] = WHITE_LIGHT;
//			I2C_TEMP[4] = Occupy;
//			I2C_TEMP[5] = STROB;				
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x07,I2C_TEMP,12);
//			Delay_ms(2);
//			
//			Cal_red_factor  =255;   //��ɫ��ƫ��
//			DMX_HL[0] = (Cal_red_factor >> 8) & 0X00000003;
//			DMX_HL[1] = Cal_red_factor & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x13,DMX_HL,2);
//			Delay_ms(1);
//			
//			Cal_green_factor  =255;   //��ɫ��ƫ��
//			DMX_HL[0] = (Cal_green_factor >> 8) & 0X00000003;
//			DMX_HL[1] = Cal_green_factor & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x15,DMX_HL,2);
//			Delay_ms(1);
//			
//			Cal_blue_factor  =255;   //��ɫ��ƫ��
//			DMX_HL[0] = (Cal_blue_factor >> 8) & 0X00000003;
//			DMX_HL[1] = Cal_blue_factor & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x17,DMX_HL,2);
//			Delay_ms(1);
//			
//			Cal_white_factor  =255;   //��ɫ��ƫ��
//			DMX_HL[0] = (Cal_white_factor >> 8) & 0X00000003;
//			DMX_HL[1] = Cal_white_factor & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x19,DMX_HL,2);
//			Delay_ms(1);
//			
//			RGB_ONOFF =0;  
//			DMX_HL[0] = (RGB_ONOFF  >> 8) & 0X00000003;
//			DMX_HL[1] = RGB_ONOFF  & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x1b,DMX_HL,2);
//			Delay_ms(1);
//			
//			STAT_EN  =0;  
//			DMX_HL[0] = (STAT_EN   >> 8) & 0X00000003;
//			DMX_HL[1] = STAT_EN   & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x1d,DMX_HL,2);
//			Delay_ms(1);
//			//
//             Menu_Flag = OK;
//             MENU_L3 = REST; 	
	}
	
	