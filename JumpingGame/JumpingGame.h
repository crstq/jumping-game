#pragma once

/*TODO LIST

1.hero和brick的碰撞检测????
2.hero和边框的碰撞检测
3.各种图片资源（button、hero、bg、brick）
4.主角移动调参（怎么才能像跳跃而不是移动）
5.stage切换（从开始界面进入第一关）
*/


#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")


// TODO:  在此处引用程序需要的其他头文件
#include <vector>
#include <math.h>


#pragma endregion


#pragma region 宏定义

#define WINDOW_WIDTH			800		//游戏窗口宽度
#define WINDOW_HEIGHT			600		//游戏窗口高度
#define WINDOW_TITLEBARHEIGHT	32		//标题栏高度

#define STAGE_STARTMENU			0		//开始画面的ID
#define STAGE_1					1		//第一个游戏场景的ID
#define STAGE_2                 2

#define HERO_SIZE_X				36		//主角的宽度
#define HERO_SIZE_Y				38		//主角的高度
#define HERO_FRAME_NUMBER		8		//主角的动画帧数量
#define HERO_ACCELERATION		0.3		//主角加速度
#define HERO_MAXSPEED			3.0		//主角最大速度

#define BUTTON_STARTGAME			1001	//开始游戏按钮ID
#define BUTTON_STARTGAME_WIDTH		147		//开始游戏按钮宽度
#define BUTTON_STARTGAME_HEIGHT		60		//开始游戏按钮高度

#define BUTTON_HELP                1002
#define BUTTON_HELP_WIDTH          163
#define BUTTON_HELP_HEIGHT         153

#define BUTTON_QUITGAME             1003
#define BUTTON_QUITGAME_WIDTH       154
#define BUTTON_QUITGAME_HEIGHT      119

#define TIMER_GAMETIMER				1	//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		10	//默认计时器刷新间隔的毫秒数


#pragma endregion


#pragma region 结构体声明

// 场景结构体
struct Stage
{
	int stageID;		//场景编号
	HBITMAP bg;			//背景图片
	int timeCountDown;	//游戏时间倒计时
	bool timerOn;		//计时器是否运行（游戏是否被暂停）

};


// 按钮结构体
struct Button
{
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

// 主角结构体
struct Hero
{
	HBITMAP img;	//图片
	int frame;		//当前显示的帧数
	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	bool health;	//生命值
};

struct Brick 
{
	int BrickID;
	bool visible;
	HBITMAP img;//图片
	int x;//坐标x
	int y;//坐标y
	int size;//边长
	bool danger;//是否为陷阱砖块
};

struct Block {
	int x, y;
	int nwidth, nheight; // 个数
};

struct Map
{
	vector<Block> blocks;
	int startingPos[2];

};
//TODO: 添加游戏需要的更多种数据（子弹、砖块、敌人、弹幕等）


#pragma endregion


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

//主角与边框碰撞检测函数
void FrameCollisionDetection(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region 其它游戏状态处理函数声明

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

//添加砖块函数
Brick* CreateBrick(HBITMAP img, int x, int y, int size, bool danger);

// 添加主角函数
Hero* CreateHero(HBITMAP img, int x, int y, bool health);

// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

// 刷新主角状态函数
void UpdateHero(HWND hWnd);

// 计算主角的当前帧数
int GetHeroFrame(double dirX, double dirY);

//TODO: 添加游戏需要的更多函数

#pragma endregion


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

#pragma endregion