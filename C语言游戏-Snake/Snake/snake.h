#ifndef _SNAKE_H
#define _SNAKE_H

/**********声明全部的函数**********/
void gotoxy(int x, int y);   //设置坐标
int color(int c);   //字体颜色
void welcom();    //开始界面
void createmap();  //绘制地图
void intisnake();  //画蛇，初始化
void notice();    //小提示
void initfood();  //生成食物
int lfde_1();   //判断是否咬到自己
void lfde_2();  //判断是否撞墙
void speedup();  //加速F1
void speeddn();  //减速F2
void ctsnake();  //蛇的移动
void keyboarcontrol();  //键盘的控制
void explat();  //帮助
void endgame();  //结束游戏窗口
void choose();  //结束后的选择
void olg();  //结束画面
void olg_2();  //
void ssss();  //蛇皮

#endif