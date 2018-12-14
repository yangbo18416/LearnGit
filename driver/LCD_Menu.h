#ifndef __LCD_Menu_H__
#define __LCD_Menu_H__
/*定义按键接口*/
sbit MENU = P1^0;		//定义按键菜单键
sbit UP		= P1^1;		//定义加键
sbit DOWN = P1^2;		//定义减键
sbit OK		= P1^3;		//定义确定键

void Key_Process(void);
void Key_Menu(void);		//返回键
void Key_Up(void);			//加按键
void Key_Down(void);		//减按键
void Key_OK(void);			//OK按键
void Time0_Init(void);
void Menu_STAT(void);
void Menu_AUTO(void);
void Menu_RUN(void);
void Menu_DMX(void);
void Menu_PERS(void);
void Menu_SET(void);
void Menu_CAL2(void);
void Menu_Red(void);
void Menu_Green(void);
void Menu_Blue(void);
void Menu_White(void);
void Menu_Strob(void);
void Menu_UPLD(void);
void Menu_REST(void);
void Menu_ID(void);
void Menu_RGBW(void);
void Menu_POW(void);
void Menu_DIM(void);
void Menu_CRGB(void);
void Menu_OFF(void);
void Menu_DIM1(void);
void Menu_DIM2(void);
void Menu_DIM3(void);
void Menu_DIM4(void);
void Menu_NumSet(void);
void Menu_Occupy(void);
void DMX_Setting(void);
void Menu_OK(void);
void Reserved(void);
void Auto_AT(void);
void Auto_PR(void);
void SET_DMX(void);
void SET_SLAV(void);
void PERS_SET(void);
void RGBW_Color(void);
void POW_SET(void);
void RGBW_SET(void);
void ID_SET(void);
void Sys_Reset(void);

#endif