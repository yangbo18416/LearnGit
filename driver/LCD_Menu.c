/*包含头文件*/
#include<reg52.h>
#include "math.h"
#include "stdio.h"
#include "ziku.h"
#include "LCD_Menu.h"
#include "delay.h"
#include "LCD12864.h"

#define KSJZ 3									//定义快速加减变量值						
static unsigned int func_index;
static unsigned int old_index;

extern xdata Menu_table table[50];
unsigned char RED_LIGHT,GREEN_LIGHT,BLUE_LIGHT,WHITE_LIGHT,Occupy,STROB;
unsigned int DMX_add;
unsigned int Cal_red_factor ;
unsigned int Cal_green_factor ;
unsigned int Cal_blue_factor ;
unsigned int Cal_white_factor ;
unsigned char TJ_Flag=0;						//该参数用来控制加减操作是对菜单进行操作还是对数值进行增减
unsigned char AT_Num= 0;						//AT参数调节
unsigned char PR_Num= 0;						//PR参数调节
void (*current_operation_index)(void);

/*
**程序名称: 按键处理程序
**输入参数: 无
**输出参数：
**说    明：获取对应的按键值
*/
void Key_Process(void)
	{
      volatile unsigned char KEY=0x00; 
		  if((P1&0X0F)!=0X0F)				/*如果按键，则说明有按键按下了*/
			{
				delayMs(10);						//延时10毫秒去抖
				if((P1&0X0F)!=0X0F)		//延时后按键仍然按下，说明按键有效
					{
						KEY=(P1&0X0F);		//获取按键值
						while((P1&0X0F)!=0X0F)
							{
								if(TJ_Flag!=0)			//如果是菜单加减，则跳出循环，不运行快加
									{
										KEY=(~KEY)&0X0F;
										delayMs(60);		//此处加延时，可以达到减慢快速加的速度
										if(KEY==0X02)														//加按键
											{
												Key_Up();
											}
										if(KEY==0X04)														//减按键
											{
												Key_Down();
											}
							}
								else
									continue;																//此处需要利用continue语句，继续等待循环
								(*current_operation_index)();			     //刷新显示											
//											;
											/*可放入延时等待中需要执行的命令*/
							}
					}
			}
		 KEY=(~KEY)&0X0F;													//通过这样的处理，对应的按键将会显示高电平
		 if(KEY==0X01)														//主菜单按键--返回键
				{
					Key_Menu();
				}
		 if(KEY==0X02)														//加按键
				{
					Key_Up();
				}
		 if(KEY==0X04)														//减按键
				{
					Key_Down();
				}
		 if(KEY==0X08)														//确认按键
				{
					Key_OK();													
				}
		current_operation_index=table[func_index].current_operation;//送入对应的执行函数
		(*current_operation_index)();	
			
	}
/*
**程序名称: 定时器初始化程序
**输入参数: 无
**输出参数：无
**说    明：定时器处理程序，完成按键定时扫描/调用显示程序
*/
void Time0_Init(void)
	{
			TMOD |= 0X01;							//定时器0，工作在模式2，16位工作模式
		  TH0 =(65535-5000)%256;		//定时器2ms进一次中断
		  TL0 =(65535-5000)/256;
			ET0 =1;										//打开定时器中断使能
			EA  =1;										//打开总中断开关
			TR0	=1;										//打开定时器
	}
/*
**程序名称: 定时器中断处理程序
**输入参数: 无
**输出参数：无
**说    明：定时器处理程序，完成按键定时扫描/调用显示程序
*/
void Time0_Interrupt(void) interrupt 1
	{
			TH0 =(65535-5000)%256;		//定时器2ms进一次中断
		  TL0 =(65535-5000)/256;	
		  Key_Process();
	}	
	
//返回-菜单按键
void Key_Menu(void)
	{
		func_index=table[func_index].Home;
		LCD12864_CLR();
		TJ_Flag=0;
	}
//确认按键
void Key_OK(void)
	{
		old_index=func_index;								//按确认键时，先存储索引状态
		func_index=table[func_index].enter;
		LCD12864_CLR();
		TJ_Flag=0;
	}
//按键加操作
void Key_Up(void)
	{
		if(TJ_Flag)													//若进入需要调节数值的菜单，则进行增减
			{
					if(old_index==3)							//若进入参数调节参数时选择为DMX调节
						{
							 if(DMX_add<511)
									DMX_add++;
							 else
									DMX_add=0;
						}	
				    if(old_index==7)							//若进入参数调节参数时选择为红色
						{
							 if(RED_LIGHT<254)
									RED_LIGHT++;
							 else
									RED_LIGHT=0;
						}
					if(old_index==8)							//若进入参数调节参数时选择为Green
						{
							 if(GREEN_LIGHT<254)
									GREEN_LIGHT++;
							 else
									GREEN_LIGHT=0;
						}
					if(old_index==9)							//若进入参数调节参数时选择为Blue
						{
							 if(BLUE_LIGHT<254)
									BLUE_LIGHT++;
							 else
									BLUE_LIGHT=0;
						}
					if(old_index==10)							//若进入参数调节参数时选择为White
						{
							 if(WHITE_LIGHT<254)
									WHITE_LIGHT++;
							 else
									WHITE_LIGHT=0;
						}	
					if(old_index==11)							//若进入参数调节参数时选择为Strob
						{
							 if(STROB<25)
									STROB++;
							 else
									STROB=0;
						}
					if(old_index==12)							//若进入参数调节参数时选择为Strob
						{
							 if(Occupy<99)
									Occupy++;
							 else
									Occupy=1;
						}
					if(old_index==27)							//若进入参数调节参数时选择为AT
						{
							 if(AT_Num<10)
									AT_Num++;
							 else
									AT_Num=1;
						}
					if(old_index==28)							//若进入参数调节参数时选择为PR
						{
							 if(PR_Num<10)
									PR_Num++;
							 else
									PR_Num=1;
						}
					if(old_index==45)							//若进入参数调节参数时选择为CAL_RED
						{
							 if(Cal_red_factor<255)
									Cal_red_factor++;
							 else
									Cal_red_factor=0;
						}
					if(old_index==46)							//若进入参数调节参数时选择为CAL_GREEN
						{
							 if(Cal_green_factor<255)
									Cal_green_factor++;
							 else
									Cal_green_factor=0;
						}	
					if(old_index==47)							//若进入参数调节参数时选择为CAL_BLUE
						{
							 if(Cal_blue_factor<255)
									Cal_blue_factor++;
							 else
									Cal_blue_factor=0;
						}
					if(old_index==48)							//若进入参数调节参数时选择为CAL_White
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
//按键减操作
void Key_Down(void)
	{
		if(TJ_Flag)
			{
					if(old_index==3)							//若进入参数调节参数时选择为Strob
						{
							 if(DMX_add>0)
									DMX_add--;
							 else
									DMX_add=512;
						}
					if(old_index==7)							//若进入参数调节参数时选择为红色
						{
							 if(RED_LIGHT>0)
									RED_LIGHT--;
							 else
									RED_LIGHT=255;
						}
					if(old_index==8)							//若进入参数调节参数时选择为Green
						{
							 if(GREEN_LIGHT>0)
									GREEN_LIGHT--;
							 else
									GREEN_LIGHT=255;
						}
					if(old_index==9)							//若进入参数调节参数时选择为Blue
						{
							 if(BLUE_LIGHT>0)
									BLUE_LIGHT--;
							 else
									BLUE_LIGHT=255;
						}
					if(old_index==10)							//若进入参数调节参数时选择为White
						{
							 if(WHITE_LIGHT>0)
									WHITE_LIGHT--;
							 else
									WHITE_LIGHT=255;
						}	
					if(old_index==11)							//若进入参数调节参数时选择为Strob
						{
							 if(STROB>0)
									STROB--;
							 else
									STROB=25;
						}
					if(old_index==12)							//若进入参数调节参数时选择为Occupy
						{
							 if(Occupy>1)
									Occupy--;
							 else
									Occupy=99;
						}
					if(old_index==27)							//若进入参数调节参数时选择为AT
						{
							 if(AT_Num>1)
									AT_Num--;
							 else
									AT_Num=10;
						}
					if(old_index==28)							//若进入参数调节参数时选择为PR
						{
							 if(PR_Num>1)
									PR_Num--;
							 else
									PR_Num=10;
						}
					if(old_index==45)							//若进入参数调节参数时选择为CAL_RED
						{
							 if(Cal_red_factor>0)
									Cal_red_factor--;
							 else
									Cal_red_factor=255;
						}
					if(old_index==46)							//若进入参数调节参数时选择为CAL_GREEN
						{
							 if(Cal_green_factor>0)
									Cal_green_factor--;
							 else
									Cal_green_factor=255;
						}	
					if(old_index==47)							//若进入参数调节参数时选择为CAL_BLUE
						{
							 if(Cal_blue_factor>0)
									Cal_blue_factor--;
							 else
									Cal_blue_factor=255;
						}
					if(old_index==48)							//若进入参数调节参数时选择为CAL_white
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
**程序名称: 开始菜单显示
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_STAT(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}
/*
**程序名称: 自走模式
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/		
void Menu_AUTO(void)
	{		
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index].text,0);
			LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
	}	
/*
**程序名称: 工作模式
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/		
void Menu_RUN(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}
/*
**程序名称: DMX地址调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_DMX(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}	
/*
**程序名称: 预设模式
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_PERS(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}
/*
**程序名称: 灯具设置
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/
void Menu_SET(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}	
/*
**程序名称: 偏光调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_CAL2(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index].text,0);
	}
/*
**程序名称: STAT-RED调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/
void Menu_Red(void)
	{
		  LCD12864_GB2312_display(1,1, table[func_index].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}	
/*
**程序名称: STAT-Green调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_Green(void)
	{		
		  LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
	}
/*
**程序名称: STAT-Blue调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/		
void Menu_Blue(void)
	{	
		 	LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}	
/*
**程序名称: STAT-White调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/		
void Menu_White(void)
	{	
		 	LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**程序名称: STAT-Strob调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/		
void Menu_Strob(void)
	{		
		  LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**程序名称: STAT-Occupy调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/
void Menu_Occupy(void)
	{
		  LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);	
	}	
/*
**程序名称: SET-UPLD调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_UPLD(void)
	{
		  table[49].Home=func_index;						//进入OK后，返回时仍然返回到UPLD内
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}
/*
**程序名称: SET-RESET调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
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
**程序名称: SET-ID调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_ID(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}
/*
**程序名称: SET-RGBW调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_RGBW(void)
	{
		  table[49].Home=old_index;																	//更新确认键的返回值		
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}	
/*
**程序名称: SET-POW调节
**输入参数: 无
**输出参数：无
**说    明：调节功率
*/	
void Menu_POW(void)
	{
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**程序名称: SET-DIM调节
**输入参数: 无
**输出参数：无
**说    明：调光曲线选择
*/	
void Menu_DIM(void)
	{
		  table[49].Home=old_index;																	//更新确认键的返回值
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**程序名称: SET-CLA2-RGBW调节
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息
*/	
void Menu_CRGB(void)
	{
		  table[49].Home=old_index;	
		  LCD12864_GB2312_display(1,1, table[func_index].text,0);
	}
/*
**程序名称: SET-DIM-OFF调节
**输入参数: 无
**输出参数：无
**说    明：关闭调节DIM
*/
void Menu_OFF(void)
	{
			table[49].Home=func_index;																	//更新确认键的返回值				
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+2].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+3].text,1);
	}
/*
**程序名称: SET-DIM-DIM1调节
**输入参数: 无
**输出参数：无
**说    明：DIM1
*/	
void Menu_DIM1(void)
	{
		  table[49].Home=func_index;																	//更新确认键的返回值		
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(5,1, table[func_index+1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+2].text,1);
	}	
/*
**程序名称: SET-DIM-DIM2调节
**输入参数: 无
**输出参数：无
**说    明：DIM2
*/		
void Menu_DIM2(void)
	{
		  table[49].Home=func_index;																	//更新确认键的返回值		
			LCD12864_GB2312_display(1,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(5,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(7,1, table[func_index+1].text,1);
	}
/*
**程序名称: SET-DIM-DIM3调节
**输入参数: 无
**输出参数：无
**说    明：DIM13
*/		
void Menu_DIM3(void)
	{
		  table[49].Home=func_index;																	//更新确认键的返回值		
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**程序名称: SET-DIM-DIM4调节
**输入参数: 无
**输出参数：无
**说    明：DIM4
*/		
void Menu_DIM4(void)
	{
		  table[49].Home=func_index;																	//更新确认键的返回值		
			LCD12864_GB2312_display(1,1, table[func_index-3].text,1);
			LCD12864_GB2312_display(3,1, table[func_index-2].text,1);
			LCD12864_GB2312_display(5,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(7,1, table[func_index+0].text,0);
	}
/*
**程序名称: 参数调节函数
**输入参数: 无
**输出参数：无
**说    明：对RGBW Strob调节+ AUTO 模式调节+偏光调节
*/		
void Menu_NumSet(void)
	{	
		unsigned char temp[5];												//用来存储数据变量		
/*根据传递过来的索引参数进行查询，该进入哪个程序*/	
		table[49].Home=old_index;											//更新确认键后，按返回按键的地址信息
		table[25].Home=old_index;											//更新确认键后，按返回按键的地址信息
		TJ_Flag=1;
		switch (old_index)
			{
				case 7:													//红色调光
					{
						 sprintf((char *)temp,"%03d",(unsigned int)RED_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 8:													//绿色调光
					{
						 sprintf((char *)temp,"%03d",(unsigned int)GREEN_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 9:													//蓝色调光
					{
						 sprintf((char *)temp,"%03d",(unsigned int)BLUE_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 10:													//白色调光
					{
						 sprintf((char *)temp,"%03d",(unsigned int)WHITE_LIGHT);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 11:													//闪光调光
					{
						 sprintf((char *)temp,"%03d",(unsigned int)STROB);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 12:													//闪光调光
					{
						 sprintf((char *)temp,"%03d",(unsigned int)Occupy);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
/*开始模式调节 AT.01-AT.10 PR.01-PR.10*/					
				case 27:													//AT数值调整
					{
						 sprintf((char *)temp,"%02d",(unsigned int)AT_Num);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(1,33, temp,1);			//在同一行内显示
						 break;
					}	
				case 28:													//PR数值调整
					{
						 sprintf((char *)temp,"%02d",(unsigned int)PR_Num);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(1,33, temp,1);			//在同一行内显示
						 break;
					}
/*开始调节偏光*/
				case 45:													//红色偏光
					{
						 sprintf((char *)temp,"%03d",(unsigned int) Cal_red_factor);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 46:													//绿色偏光
					{
						 sprintf((char *)temp,"%03d",(unsigned int) Cal_green_factor);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 47:													//蓝色偏光
					{
						 sprintf((char *)temp,"%03d",(unsigned int) Cal_blue_factor);
						 LCD12864_GB2312_display(1,1, table[old_index].text,1);
						 LCD12864_GB2312_display(3,1, temp,1);
						 break;
					}
				case 48:													//蓝色偏光
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
**程序名称: DMX-DMX_Setting
**输入参数: 无
**输出参数：无
**说    明：DMX地址调节
*/		
void DMX_Setting(void)
	{
		unsigned char temp[5];												//用来存储数据变量	
		table[49].Home=old_index;											//更新确认键后，按返回按键的地址信息
		TJ_Flag=1;
		sprintf((char *)temp,"%03d",(unsigned int)DMX_add);
		LCD12864_GB2312_display(1,1, table[func_index].text,1);
		LCD12864_GB2312_display(3,1, temp,1);
	
	}
/*
**程序名称: AUTO-AT菜单
**输入参数: 无
**输出参数：无
**说    明：该菜单可提供数据调整功能
*/	
void Auto_AT(void)
	{
			table[49].Home=old_index;																	//更新确认键的返回值
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
	}
/*
**程序名称: AUTO-PR菜单
**输入参数: 无
**输出参数：无
**说    明：该菜单可提供数据调整功能
*/		
void Auto_PR(void)
	{
			table[49].Home=old_index;																	//更新确认键的返回值
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
	}
/*
**程序名称: PERS菜单选项设置
**输入参数: 无
**输出参数：无
**说    明：该函数通过判断index参数来显示菜单项
*/
void PERS_SET(void)
	{
/*
		这里和其他函数处理稍有不同，因为中间省去了一个中间环节，因此将OK键的返回值，设置为之前进入序列号，此处为当前索引号
		比如颜色函数，因为有中间函数过度，所以不能采取此方式。这个在调试过程中可以感觉到不同
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
			TJ_Flag = 0;																								//调节菜单
			table[49].Home=func_index;																	//更新确认键的返回值
			LCD12864_GB2312_display(1,1, table[func_index+0].text,0);
			LCD12864_GB2312_display(3,1, table[func_index+1].text,1);
	}
void SET_SLAV(void)
	{
			TJ_Flag = 0;																								//调节菜单
			table[49].Home=func_index;																	//更新确认键的返回值
			LCD12864_GB2312_display(1,1, table[func_index-1].text,1);
			LCD12864_GB2312_display(3,1, table[func_index+0].text,0);
	}
/*
**程序名称: SET-ID 开关
**输入参数: 无
**输出参数：无
**说    明：该函数通过判断index参数来显示菜单项
*/	
void ID_SET(void)
	{
		 TJ_Flag = 0;																								//调节菜单
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
**程序名称: CAL2- RGBW 色温调节开关
**输入参数: 无
**输出参数：无
**说    明：该函数通过判断index参数来显示菜单项
*/				
void RGBW_SET(void)
	{
		TJ_Flag = 0;																								//调节菜单
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
**程序名称: SET-POW 功率调节
**输入参数: 无
**输出参数：无
**说    明：该函数通过判断index参数来显示菜单项
*/					
void POW_SET(void)
	{
		 TJ_Flag = 0;																								//调节菜单
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
		 TJ_Flag = 0;																								//调节菜单
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
**程序名称: OK确认键
**输入参数: 无
**输出参数：无
**说    明：显示相关提示信息，并将相关数值进行更新
*/		
void Menu_OK(void)
	{
		TJ_Flag=0;
		/*开始对传递过来的OK值进行更新操作*/
	
		/*结束传递过来的OK值进行更新*/
	/*更新状态值*/
//	    switch (old_index)
//	    	{
//	    	 	//RESET选项
//	    	 	case 14:
//	    	 		{
//	    	 			Sys_Reset();			//调用复位函数
//	    	 			break;
//	    	 		}
//	    	 	//调光曲线调节
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
//	    	   //自走模式更新
//				case 27:			//AT模式
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
//				case 28:			//PR模式
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
//	    			//运行模式设置  RUN_DMX RUN_SLAV
//	    		case 29:
//	    			{
//	    				break;
//	    			}
//	    		case 30:
//	    			{
//	    				break;
//	    			}	
//	    		//通道设置
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
//				//ID 设置开关
//	    		case 39:
//	    			{
//	    				break;
//	    			}
//	    		case 40:
//	    			{
//	    				break;
//	    			}
//	    		//偏光RGB开关					
//	    		case 41:
//	    			{
//						break;
//	    			}
//	    		case 42:
//	    			{
//						break;
//	    			}
//	    		//功率调节
//	    		case 43:
//	    			{
//	    				break;
//	    			}
//	    		case 44:
//	    			{
//	    				break;
//	    			}
//	    		//偏光调节，更新标志位
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
	/*结束状态值更新*/		 	
		LCD12864_GB2312_display(2,1, table[func_index].text,1);	
	}
//复位函数
void Sys_Reset(void)
	{
//  //出厂设置，地址＝1；
//			DMX_add =1;
//			DMX_HL[0] = (DMX_add >> 8) & 0X00000003;
//			DMX_HL[1] = DMX_add & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x01,DMX_HL,2);
//			Delay_ms(1);
//			
//			//此处可以选择建立一临时变量用来存储相关参数
//			Menu_Flag = 6 ;   //DIM
//			DMX_HL[0] = (Menu_Flag >> 8) & 0X00000003;
//			DMX_HL[1] = Menu_Flag & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x03,DMX_HL,2);
//			Delay_ms(1);
//			
//			Menu_Flag =2;   //通道设置
//			Channel = HVS;
//			DMX_HL[0] = (Menu_Flag>>8) & 0X00000003;
//			DMX_HL[1] =Menu_Flag & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x05,DMX_HL,2);
//			Delay_ms(1);
//			
//         	//亮度调节
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
//			Cal_red_factor  =255;   //高色温偏光
//			DMX_HL[0] = (Cal_red_factor >> 8) & 0X00000003;
//			DMX_HL[1] = Cal_red_factor & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x13,DMX_HL,2);
//			Delay_ms(1);
//			
//			Cal_green_factor  =255;   //高色温偏光
//			DMX_HL[0] = (Cal_green_factor >> 8) & 0X00000003;
//			DMX_HL[1] = Cal_green_factor & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x15,DMX_HL,2);
//			Delay_ms(1);
//			
//			Cal_blue_factor  =255;   //高色温偏光
//			DMX_HL[0] = (Cal_blue_factor >> 8) & 0X00000003;
//			DMX_HL[1] = Cal_blue_factor & 0X000000FF;
//			I2C_WriteNByte(LC64,TWO_BYTE_SUBA,0x17,DMX_HL,2);
//			Delay_ms(1);
//			
//			Cal_white_factor  =255;   //高色温偏光
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
	
	