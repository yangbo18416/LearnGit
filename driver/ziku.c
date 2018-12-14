#include<reg52.h>
#include "ziku.h"
#include "LCD_Menu.h"
xdata Menu_table  table[50]=
{
// Index     
// | DOWN 
// | | UP 
// | | | ENTEL 
// | | | | Home 
// | | | | | ���ܺ��� 
// | | | | |     | 
  {0,6,1,7,0,"STAT",(*Menu_STAT)}, //��һ��STAT
	{1,0,2,27,0,"AUTO",(*Menu_AUTO)}, //��һ��AUTO
	{2,1,3,29,0,"RUN ",(*Menu_RUN)},  //��һ��RUN			
	{3,2,4,26,0,"DMX ",(*Menu_DMX)},  //��һ��DXM
	{4,3,5,31,0,"PERS",(*Menu_PERS)}, //��һ��PERS			
	{5,4,6,13,0,"SET ",(*Menu_SET)},  //��һ��SET
	{6,5,0,45,0,"CAL2",(*Menu_CAL2)}, //��һ��CAL2
//STAT	
	{7,12,8,25,0,"Red   ",(*Menu_Red)},  //�ڶ���Red
	{8,7,9,25,0,"Green ",(*Menu_Green)},//�ڶ���Green
	{9,8,10,25,0,"Blue  ",(*Menu_Blue)}, //�ڶ���Blue
	{10,9,11,25,0,"White ",(*Menu_White)},//�ڶ���White
	{11,10,12,25,0,"Strob ",(*Menu_Strob)},//�ڶ���Strob
	{12,11,7,25,0,"Occupy",(*Menu_Occupy)},//�ڶ���Occupy ����ռ�ձ�
//SET	
	{13,18,14,49,5,"UPLD",(*Menu_UPLD)},  //�ڶ���UPLD--SET
	{14,13,15,49,5,"REST",(*Menu_REST)},  //�ڶ���REST---SET
	{15,14,16,39,5,"ID  ",(*Menu_ID)}, 		//�ڶ���ID-------SET
	{16,15,17,41,5,"RGBW",(*Menu_RGBW)},	//�ڶ���RGBW---SET
	{17,16,18,43,5,"POW ",(*Menu_POW)},		//�ڶ���POW-----SET	
	{18,17,13,20,5,"DIM ",(*Menu_DIM)},		//�ڶ���DIM-----SET
//CAL2--�˺�����ʱδʹ��	
	{19,19,19,45,6,"RGBW",(*Menu_CRGB)},	//�ڶ���RGBW--CAL2	
//DIM
	{20,24,21,49,18,"OFF ",(*Menu_OFF)},  //������OFF
	{21,20,22,49,18,"DIM1",(*Menu_DIM1)},//������DIM1
	{22,21,23,49,18,"DIM2",(*Menu_DIM2)},//������DIM2
	{23,22,24,49,18,"DIM3",(*Menu_DIM3)},//������DIM3
	{24,23,20,49,18,"DIM4",(*Menu_DIM4)},//������DIM4

//��ֵ��������
	{25,25,25,49,7,"Menu_NumSet",(*Menu_NumSet)},//��ֵ��������
//DMX���ڲ˵�
	{26,26,26,49,3,"DMX_Address",(*DMX_Setting)},
// AUTO �Ӳ˵�
	{27,28,28,25,1,"AT",(*Auto_AT)},				//����ĵ����������ӳ������
	{28,27,27,25,1,"PR",(*Auto_PR)},
// RUN �Ӳ˵�
	{29,30,30,49,2,"DMX",(*SET_DMX)},
	{30,29,29,49,2,"SLAV",(*SET_SLAV)},
//SET-PERS �Ӳ˵�
	{31,38,32,49,4,"TOUR ",(*PERS_SET)},
	{32,31,33,49,4,"Arc.1",(*PERS_SET)},
	{33,32,34,49,4,"Ar1.d",(*PERS_SET)},
	{34,33,35,49,4,"Arc.2",(*PERS_SET)},
	{35,34,36,49,4,"Ar2.d",(*PERS_SET)},
	{36,35,37,49,4,"Ar2.s",(*PERS_SET)},
	{37,36,38,49,4,"HV   ",(*PERS_SET)},
	{38,37,31,49,4,"HSV  ",(*PERS_SET)},

//SET-ID �Ӳ˵�	
	{39,40,40,49,15,"ID_ON ",(*ID_SET)},
	{40,39,39,49,15,"ID_OFF",(*ID_SET)},
//SET-RGBW �Ӳ˵�
	{41,42,42,49,16,"RGBW_ON ",(*RGBW_SET)},
	{42,41,41,49,16,"RGBW_OFF",(*RGBW_SET)},	
//SET-POW �Ӳ˵�
	{43,44,44,49,17,"POW_HIGH",(*POW_SET)},
	{44,43,43,49,17,"POW_NORM",(*POW_SET)},	
//CAL2-RGBW
	{45,48,46,25,6,"CAL_Red  ",(*RGBW_Color)},
	{46,45,47,25,6,"CAL_Green",(*RGBW_Color)},
	{47,46,48,25,6,"CAL_Blue ",(*RGBW_Color)},	
	{48,47,45,25,6,"CAL_White",(*RGBW_Color)},	
/*Reserved*/	
//ȷ�ϰ���
	{49,49,49,49,0,"Setting Confrim!",(*Menu_OK)},  //������ȷ�ϰ���


	
};	