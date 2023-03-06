#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<Windows.h>  //API调控
#include<stdlib.h>
#include<conio.h>  //接受键盘输入输出
#include<time.h> //加速
//#include<easyx.h>
//#include<graphics.h>
#include"snake.h"

//蛇的状态
#define up 1
#define down 2
#define  right 3
#define   left  4

/********定义全局变量*********/
typedef struct _snake {//蛇身的一个结点
	//蛇的坐标
	int x;
	int y;
	struct _snake *next;     //下一个蛇身的位置
}snake;    //创建蛇的结点
int score = 0, add = 5;				   //初始化得分和每次加的分
int fors, forstime = 200;			  //蛇前进的状态，每次运行的时间
snake *head, *food;				    //蛇头指针  食物指针
snake *p;						  //临时指针，用来遍历
int end = 0;				    //判断游戏结束情况
HANDLE hOut;				   //控制台句柄   使用系统工具的凭证/*

/**********主函数************/
int main(void) {

	welcom();
	keyboarcontrol();
	endgame();

	_getch();
	return 0;
}

//设置光标坐标
void gotoxy(int x, int y) {

	COORD pos;  //表示一个字符在控制台屏幕上的坐标
	pos.X=x;
	pos.Y=y;   //GetStdHandle函数获取一个指向特定标准设备的句柄，包括标准输入，标准输出和标准错误。
	//STD_OUTPUT_HANDLE正是代表标准输出（也就是显示屏）的宏 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

//设置颜色
int color(int c) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
	return 0;
}

//开始界面
void welcom (){
	int n;

	ssss();
	gotoxy(47, 3);//功能是将光标移动到指定位置 在特定的位置打印输出
	color(11);
	printf("欢 迎 来 到");
	gotoxy(44, 5);
	color(12);
	printf("贪 吃 蛇 大 作 战");
	gotoxy(30, 25);
	printf("1、开始游戏");
	gotoxy(45, 25);
	printf("2、游戏说明");
	gotoxy(60, 25);
	printf("3、退出游戏");
	gotoxy(75, 25);
	printf("4、更多:有待开发...");
	gotoxy(45, 20);
	color(3);
	printf("请选择：1,2,3");
	color(15);
	scanf("%d", &n);
	switch (n) {
	case 1:
		system("cls");//  清屏函数  调用DOS命令
			createmap();   //绘制地图
			intisnake();      //初始化蛇身
			initfood();        //创建食物
			keyboarcontrol();   //控制键盘按键
		break;

	case 2:
		explat();           //游戏说明
		break;

	case 3:
		exit(0);// 退出游戏
		break;

	default:
		color(12);
		gotoxy(45, 30);
		printf("请输入1-3的数字");
		_getch();       //输入任意键继续
		system("cls");   //清屏
		welcom();   //重新来到开始界面

	}

}

//绘制地图
void createmap() { //绘制地图

	//initgraph(680, 500);//初始化200*200像素的绘图窗口
	//setlinecolor(BLUE);   //设置背景颜色
	int i, j;
	for (i = 0; i < 59; i += 2) {   //打印上下的框  的总数    一个框占字符是2，所以要每次加2
		gotoxy(i, 0);
		color(6);            //框的颜色
		printf("■");

		gotoxy(i, 25);            //控制上下框的距离
		printf("■");
	}
	for (j = 0; j < 25; j++) { // 打印左右的框  的总数
		gotoxy(0, j);
		color(6);
		printf("■");

		gotoxy(58, j);
		printf("■");
	}

	for (i = 2; i < 58; i+=2) {   //绘制中间的网格
		for (j = 1; j < 25; j++) {
			gotoxy(i, j);            //用ij来控制填充的坐标
			color(8);
			printf("■");

		}
		
	}

}

//游戏的得分和小提示
void notice() {

	gotoxy(80, 8);
	color(14);
	printf("您目前的得分：%d",score);

	gotoxy(84, 12);
	printf("注意啦！");
	gotoxy(82, 14);
	printf("1.每个食物得分是：%d", add);
	gotoxy(82, 16);
	printf("2.不能穿墙，不能咬到自己");
	gotoxy(82,18);
	printf("3.用↑ ↓ ← →分别控制蛇的移动方向");
	gotoxy(82, 20);
	printf("4.F1 是加速键，F2 是减速键");
	gotoxy(82, 22);
	printf("5.空格键:暂停游戏");
	gotoxy(82, 24);
	printf("6.ESC:退出游戏");

}

//画蛇了  先初始化
void intisnake (){
	//<创建链表>
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));   // 从蛇尾开始 //使用头插法，以x，y设定开始的位置
	tail->x = 24;    //蛇一开始的位置是（24，10）;
	tail->y = 5;

	tail->next = NULL;   //蛇的下一个设为空
	for (i = 1; i <=2; i++) {   //设置蛇的长度为3
	
		//注意这是在循环
		head = (snake*)malloc(sizeof(snake)); //开辟空间 初始化蛇头   
		head->next = tail;             //蛇头的下个一位是蛇尾   
		head->x = 24 + 2 * i;               //设置蛇头的位置   往x轴走 2字符一个蛇结点
		head->y = 5;
		tail = head;                  //蛇头和蛇尾在一个结点    重复循环    //建议画图理解链表
	}									//以便让head蛇头再次循环时指向tial蛇尾

	//<输出链表>
	while (tail!=NULL) { //从头到尾输出蛇
		color(11);
		gotoxy(tail->x, tail->y);   //每个结点的蛇身都是不一样的
		printf("●");           //输出蛇身   
		tail = tail->next;   //一直输出直到tail遇到NULL
	}

}

//蛇皮
void ssss() {
	color(5);
	gotoxy(38, 8);
	printf("       □□□□□□□□\n");
	gotoxy(38, 9);
	printf("       □            □\n");
	gotoxy(38, 10);
	printf("       □            □\n");
	gotoxy(38, 11);
	printf("       □            □\n");
	gotoxy(38, 12);
	printf("       □            □\n");
	gotoxy(38, 13);
	printf("       □      ★    □\n");
	gotoxy(38, 14);
	printf("       □             \n");
	gotoxy(38, 15);
	printf("       □             \n");
	gotoxy(38, 16);
	printf("       □             \n");
	gotoxy(38, 17);
	printf("       □             \n");
	gotoxy(38, 18);
	printf("       ''              \n");

} 

//生成食物
void initfood() {

	snake *food_1;//定义食物xy和下一个食物位置
	srand((unsigned)time(NULL));// 初始化随机数函数  //精确到秒
	food_1 = (snake*)malloc(sizeof(snake));    //初始化食物

	while ((food_1->x % 2) != 0)	{//保证其为偶数，使得食物能与蛇头对其，然后食物会出现在网格线上  每个蛇结点是两个字符
		food_1->x = rand() % 51+2;      //食物随机出现   //注意x，y不能超过墙壁边界
	}
	//所做的参数都是为了让食物更好的出现
	food_1->y = rand() % 24+1;          
	p = head;   //让p指针指向蛇头

	while (p->next != NULL) { //判断蛇头（首结点）

		if (p->x == food_1->x&&p->y == food_1->y) {  //判断蛇的身体和食物是否重合
			free(food_1);					//如果蛇身和食物重合，那么释放食物指针
			initfood();						//然后重新出现食物
		}
		p = p->next;//移动指针
	}

	//输出食物
	gotoxy(food_1->x, food_1->y); //食物的坐标
	food = food_1;           //food保存数据为下面的调用使用   
	color(2);
	printf("★");

}


//判断是否咬到自己
int lfde_1 (){
	snake *s;       //定义s为蛇身上的一个结点
	s = head->next;  //s是蛇头以外身上的结点
	while (s!=NULL) {

		if (s->x == head->x&&s->y == head->y) {  //如果s和蛇身上的结点重合
			end = 2;   //返回第2种死亡情况
			return 1;  //返回1
		}
		s = s->next;
	}
	return 0;  //返回0
	}

//查看蛇是否撞墙
void lfde_2() {
	//如果蛇头撞到了墙壁
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 24) {  //查看蛇是否走到了墙壁的边界
		end = 1;   //返回第一个种情况
		endgame(); //出现游戏结束的画面
	}

}

// F1 加速
void speedup() {
	
	if (forstime >= 50)   //如果时间间隔大于50
	{
	forstime =forstime- 10;  //   时间间隔减去10
		add =add+2;        //得分在原来的基础上加2
	}

}

// F2 减速
void speeddn() {
	if (forstime < 350)   //如果时间间隔小于350
	{
		forstime =forstime+30;   //时间间隔加30
		add =add-2;    //得分在原来的基础上减2
	}


}

//蛇的移动  
///Central
void ctsnake(){//蛇前进,上up,下down,左left,右right

	//这里只是表示移动一次
	snake*nextd;
	lfde_2();//查看移动是否撞墙
	nextd = (snake*)malloc(sizeof(snake)); //为下一步开辟蛇头
	if (fors == up) {
		nextd->x = head->x;  //找到蛇头     //向上前进时，x坐标不动，y坐标-1
		nextd->y = head->y-1;         //向上前进时，x坐标不动，y坐标-1
		nextd->next = head;      //链接的作用  nextd变成蛇头了 head反而是第二节了  所以蛇头要指向第二节
		head = nextd;         //移动完了之后，nextd就是蛇头 所以head要重新获取蛇头
		p = head;           //指针q指向蛇头
	
	 if (nextd->x == food->x&&nextd->y == food->y) {//如果下一个有食物 下一个位置的坐标和食物的坐标相同
		while (p->next!=NULL) {
			gotoxy(p->x, p->y);	
			color(11);
			printf("●");        //把星星替换蛇身的形状
			p = p->next;//然后继续输出其他的蛇身
		}
		score =score+add;
		speedup();  //加速
		initfood();   //重新生成食物

	 }  
	      else {//没有食物的情况
			while (p->next->next!=NULL) {//输出移动后的蛇身后并找到蛇尾
			gotoxy(p->x, p->y);
			color(11);
			printf("●"); //蛇继续往前走
			p = p->next;  //输出蛇身
		  }
			//在此之前蛇尾的下一位的地方还没有被打印释放内存
			//所以上面的循环找的是蛇尾的下一位是不是NULL
		gotoxy(p->next->x, p->next->y);   //经过上面的循环，q指向蛇尾，蛇尾的下一位，就是蛇走过去的位置
		color(8);
		printf("■");        //这就中间的墙壁
		free(p->next);  //进行输出■之后，释放指向下一位的指针
		p->next = NULL;   //指针下一位指向空
	}

	}

	if (fors == down) { //蛇向下时  
		nextd->x = head->x;  //找到蛇头    
		nextd->y = head->y + 1;         //向下前进时，x坐标不动，y坐标+1
		nextd->next = head;
		head = nextd;         //
		p = head;           //指针q指向蛇头

		if (nextd->x == food->x&&nextd->y == food->y) {//如果下一个有食物 下一个位置的坐标和食物的坐标相同
			while (p != NULL) {
				gotoxy(p->x, p->y);
				color(11);
				printf("●");        //替换蛇身的形状
				p = p->next;//然后继续输出其他的蛇身
			}
			score = score + add;
			speedup();  //加速
			initfood();   //重新生成食物
		}
		else {//没有食物的情况
			while (p->next->next != NULL) {
				gotoxy(p->x, p->y);
				color(11);
				printf("●"); //蛇继续往前走
				p = p->next;  //输出蛇身
			}
			gotoxy(p->next->x, p->next->y);   //经过上面的循环，q指向蛇尾，蛇尾的下一位，就是蛇走过去的位置
			color(8);
			printf("■");        //这就中间的墙壁
			free(p->next);  //进行输出■之后，释放指向下一位的指针
			p->next = NULL;   //指针下一位指向空
		}

		}

	if (fors == left) { //蛇向左时
		nextd->x = head->x-2;  //找到蛇头    
		nextd->y = head->y;         //向左前进时，x坐标向左移动-2，y坐标不动
		nextd->next = head;
		head = nextd;         //
		p = head;           //指针q指向蛇头

		if (nextd->x == food->x&&nextd->y == food->y) {//如果下一个有食物 下一个位置的坐标和食物的坐标相同
			while (p != NULL) {
				gotoxy(p->x, p->y);
				color(11);
				printf("●");        //替换蛇身的形状
				p = p->next;//然后继续输出其他的蛇身
			}
			score = score + add;
			speedup();  //加速
			initfood();   //重新生成食物
		}
		else {//没有食物的情况
			while (p->next->next != NULL) {
				gotoxy(p->x, p->y);
				color(11);
				printf("●"); //蛇继续往前走
				p = p->next;  //输出蛇身
			}
			gotoxy(p->next->x, p->next->y);   //经过上面的循环，q指向蛇尾，蛇尾的下一位，就是蛇走过去的位置
			color(8);
			printf("■");        //这就中间的墙壁
			free(p->next);  //进行输出■之后，释放指向下一位的指针
			p->next = NULL;   //指针下一位指向空
		}

	}

	if (fors == right) { //蛇向右时时
		nextd->x = head->x+2;  //找到蛇头     
		nextd->y = head->y;        //向右前进时，x坐标向右移动+2，y坐标不动
		nextd->next = head;
		head = nextd;         //
		p = head;           //指针q指向蛇头

		if (nextd->x == food->x&&nextd->y == food->y) {//如果下一个有食物 下一个位置的坐标和食物的坐标相同
			while (p != NULL) {
				gotoxy(p->x, p->y);
				color(11);
				printf("●");        //替换蛇身的形状
				p = p->next;//然后继续输出其他的蛇身
			}
			score = score + add;
			speedup();  //加速
			initfood();   //重新生成食物
		}
		else {//没有食物的情况
			while (p->next->next != NULL) {
				gotoxy(p->x, p->y);
				color(11);
				printf("●"); //蛇继续往前走
				p = p->next;  //输出蛇身
			}
			gotoxy(p->next->x, p->next->y);   //经过上面的循环，q指向蛇尾，蛇尾的下一位，就是蛇走过去的位置
			color(8);
			printf("■");        //这就中间的墙壁
			free(p->next);  //进行输出■之后，释放指向下一位的指针
			p->next = NULL;   //指针下一位指向空
		}
	}

	if (lfde_1()==1) {
		end = 2;
		endgame(); //游戏结束画面
	}
}

	//键盘控制
void keyboarcontrol () {

		fors = right;  //初始化蛇是向右走

		while (1) {//无限循环

			notice();  //小提示
			//向上走的时候使按向下走没作用
			if (GetAsyncKeyState(VK_UP) && fors != down) {//GetAsyncKeyState函数用来判断函数调用时指定虚拟键的状态
				//VK_UP等于↑
				fors = up; //如果蛇不是向下前进的时候，按上键，执行向上前进操作
			}
			else if (GetAsyncKeyState(VK_DOWN) && fors != up) {
				//VK_UP等于↓
				fors = down; // 如果蛇不是向下前进的时候，按下键，执行向上前进操作
			}
			else if (GetAsyncKeyState(VK_LEFT) && fors != right) {
				//VK_UP等于←
				fors = left;
			}
			else if (GetAsyncKeyState(VK_RIGHT) && fors != left) {
				//VK_UP等于→
				fors = right;
			}
			if (GetAsyncKeyState(VK_SPACE)) {//按暂停键，执行pause暂停函数
				while (1) {
					Sleep(300);//sleep()函数，头文件#include <unistd.h>另进程暂停，知道达到里面设定的参数的时间。
				
				if (GetAsyncKeyState(VK_SPACE)) {//再次按空格键就出去
					break;
				}
				}
			}
			else if(GetAsyncKeyState(VK_ESCAPE)){//按esc键，直接到结束界面
				end=3;//第三种情况
				exit(0);
				break;
			}
			else if (GetAsyncKeyState(VK_F1)) {  //F1加速
				speedup();
			}
			else if (GetAsyncKeyState(VK_F2)) { //F2减速
				speeddn();
			}

			Sleep(forstime);  //设置Sleep函数
			ctsnake();  //蛇移动
		}

	}

	//游戏说明
void explat() {
		system("cls");//清屏
		olg_2();
		gotoxy(45, 10);
		printf("注意啦！");
		gotoxy(38, 12);
		printf("1.每个食物得分是：%d", add);
		gotoxy(38, 14);
		printf("2.不能穿墙，不能咬到自己");
		gotoxy(38, 16);
		printf("3.用↑ ↓ ← →分别控制蛇的移动方向");
		gotoxy(38, 18);
		printf("4.F1 是加速键，F2 是减速键,每次吃到食物都会加速加分");
		gotoxy(38, 20);
		printf("5.空格键:暂停游戏");
		gotoxy(38, 22);
		printf("6.ESC:退出游戏");
		_getch();   //按任意键返回主界面
		system("cls");   //清屏
		welcom();            //主界面
	}

	//结束游戏
void endgame() {
	
		system("cls");   //清屏

		if (end == 1) {
			olg();
			gotoxy(45, 11);
			color(11);
			printf("GAME OVER!");
			gotoxy(45, 13);
			printf("死亡原因：撞墙");
		}
	    else if (end == 3) {
			olg();
			gotoxy(45, 9);
			color(12);
			printf("已结束游戏。");
			gotoxy(45, 12);
			printf("死亡原因：退出");
		}
		else if (end == 2) {
			olg();
			gotoxy(45, 11);
			color(11);
			printf("GAME OVER!");
			gotoxy(45, 13);
			printf("死亡原因：咬到自己");
        }

		gotoxy(45, 16);
		color(12);
		printf("你的得分是: %d", score);
		choose();  //游戏失败后的选择
	
	}

	//游戏失败后的选择
void choose() {
		int n;
		gotoxy(27,25);
		printf("Continue ---->1");
		gotoxy(56, 25);
		printf("Exit ---->2");
		gotoxy(45, 18);
		printf("请选择:");
		scanf("%d", &n);

		switch (n) {

		case 1:
			system("cls");   //清屏
			score = 0;     //分数归零
			forstime = 200;   //设定初始速度
			add = 0;       //初始化分数为零
			welcom();     //调出开始界面
			break;

		case 2:
			exit(0); //推出游戏
			break;
		default:
			gotoxy(45, 34);
			printf("输入有误 重新输入");
			system("cls");
			system("pause >nul");
			endgame();  //游戏结束画面
			choose();   //游戏结束后选择
			break;
		}
	}

	//失败后的画面
void olg() {
		system("cls");
		int i;
		gotoxy(54, 5);
		printf(".-.-");

		gotoxy(52, 4);
		printf("\\\\\\\\\|///");

		gotoxy(50, 5);
		printf("\\\\");

		gotoxy(60, 5);
		printf("//");


		gotoxy(40, 7);
		color(7);
		printf("oOOo");


		gotoxy(65, 7);
		color(7);
		printf("oOOo");

		gotoxy(54, 7);
		printf("(__) ");
	
		gotoxy(51, 6);
		printf("( ");

		gotoxy(60, 6);
		printf(") ");

		gotoxy(54, 6);
		color(9);
		printf(".@.@");

		gotoxy(17, 7);
		color(2);
		printf("+----------------------");

		gotoxy(44, 7);
		color(2);
		printf("----------");

		gotoxy(58, 7);
		color(2);
		printf("-------");

		for (i =8; i < 22; i++) {

			gotoxy(17, i);
			printf("|");
			gotoxy(89, i);
			printf("|");
		}

		gotoxy(69, 7);
		printf("--------------------+");



		gotoxy(18, 21);
		color(14);
		printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆");

		gotoxy(17, 22);
		color(2);
		printf("+-----------------------------------------------------------------------+");
	}

void olg_2() {
	system("cls");
	int i;
	gotoxy(50, 5);
	printf("------------");

	gotoxy(52, 2);
	printf("-------");

	gotoxy(52, 2);
	printf("|");

	gotoxy(52, 3);
	printf("|");

	gotoxy(52, 4);
	printf("|");

	gotoxy(59, 2);
	printf("|");

	gotoxy(59, 3);
	printf("|");

	gotoxy(59, 4);
	printf("|");

	gotoxy(40, 7);
	color(7);
	printf("oOOo");


	gotoxy(65, 7);
	color(7);
	printf("oOOo");

	gotoxy(55, 7);
	printf("| |");
	gotoxy(55, 8);
	printf("|_|");


	gotoxy(51, 6);
	printf("( ");

	gotoxy(60, 6);
	printf(") ");

	gotoxy(55, 6);
	color(9);
	printf("> <");

	gotoxy(17, 7);
	color(2);
	printf("+----------------------");

	gotoxy(44, 7);
	color(2);
	printf("----------");

	gotoxy(58, 7);
	color(2);
	printf("-------");

	for (i = 8; i < 26; i++) {

		gotoxy(17, i);
		printf("|");
		gotoxy(89, i);
		printf("|");
	}

	gotoxy(69, 7);
	printf("--------------------+");



	gotoxy(18, 25);
	color(14);
	printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆");

	gotoxy(17, 26);
	color(2);
	printf("+-----------------------------------------------------------------------+");
}
/*764行代码   2020年4月21日*/