#ifndef __ziku_h__
#define __ziku_h__
/*--  文字:  我  --*/
/*--  宋体16;  此字体下对应的点阵为：宽x高=16x16   --*/
typedef struct
{
	unsigned int current;//当前状态索引号
	unsigned int  down; //向上
	unsigned int  up;	//向下
	unsigned int  enter; //确定
	unsigned int  Home; //退出
	unsigned char *text;	//对应的菜单显示名称
	void (*current_operation)(void); //当前状态应该执行的操作
} Menu_table;	
#endif