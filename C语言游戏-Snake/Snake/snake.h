#ifndef _SNAKE_H
#define _SNAKE_H

/**********����ȫ���ĺ���**********/
void gotoxy(int x, int y);   //��������
int color(int c);   //������ɫ
void welcom();    //��ʼ����
void createmap();  //���Ƶ�ͼ
void intisnake();  //���ߣ���ʼ��
void notice();    //С��ʾ
void initfood();  //����ʳ��
int lfde_1();   //�ж��Ƿ�ҧ���Լ�
void lfde_2();  //�ж��Ƿ�ײǽ
void speedup();  //����F1
void speeddn();  //����F2
void ctsnake();  //�ߵ��ƶ�
void keyboarcontrol();  //���̵Ŀ���
void explat();  //����
void endgame();  //������Ϸ����
void choose();  //�������ѡ��
void olg();  //��������
void olg_2();  //
void ssss();  //��Ƥ

#endif