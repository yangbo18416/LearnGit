#ifndef __ziku_h__
#define __ziku_h__
/*--  ����:  ��  --*/
/*--  ����16;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
typedef struct
{
	unsigned int current;//��ǰ״̬������
	unsigned int  down; //����
	unsigned int  up;	//����
	unsigned int  enter; //ȷ��
	unsigned int  Home; //�˳�
	unsigned char *text;	//��Ӧ�Ĳ˵���ʾ����
	void (*current_operation)(void); //��ǰ״̬Ӧ��ִ�еĲ���
} Menu_table;	
#endif