#pragma once

/*TODO LIST

1.hero��brick����ײ���????
2.hero�ͱ߿����ײ���
3.����ͼƬ��Դ��button��hero��bg��brick��
4.�����ƶ����Σ���ô��������Ծ�������ƶ���
5.stage�л����ӿ�ʼ��������һ�أ�
*/


#pragma region ͷ�ļ�����

// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// ��Դͷ�ļ�
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <vector>
#include <math.h>


#pragma endregion


#pragma region �궨��

#define WINDOW_WIDTH			800		//��Ϸ���ڿ��
#define WINDOW_HEIGHT			600		//��Ϸ���ڸ߶�
#define WINDOW_TITLEBARHEIGHT	32		//�������߶�

#define STAGE_STARTMENU			0		//��ʼ�����ID
#define STAGE_1					1		//��һ����Ϸ������ID
#define STAGE_2                 2

#define HERO_SIZE_X				36		//���ǵĿ��
#define HERO_SIZE_Y				38		//���ǵĸ߶�
#define HERO_FRAME_NUMBER		8		//���ǵĶ���֡����
#define HERO_ACCELERATION		0.3		//���Ǽ��ٶ�
#define HERO_MAXSPEED			3.0		//��������ٶ�

#define BUTTON_STARTGAME			1001	//��ʼ��Ϸ��ťID
#define BUTTON_STARTGAME_WIDTH		147		//��ʼ��Ϸ��ť���
#define BUTTON_STARTGAME_HEIGHT		60		//��ʼ��Ϸ��ť�߶�

#define BUTTON_HELP                1002
#define BUTTON_HELP_WIDTH          163
#define BUTTON_HELP_HEIGHT         153

#define BUTTON_QUITGAME             1003
#define BUTTON_QUITGAME_WIDTH       154
#define BUTTON_QUITGAME_HEIGHT      119

#define TIMER_GAMETIMER				1	//��Ϸ��Ĭ�ϼ�ʱ��ID
#define TIMER_GAMETIMER_ELAPSE		10	//Ĭ�ϼ�ʱ��ˢ�¼���ĺ�����


#pragma endregion


#pragma region �ṹ������

// �����ṹ��
struct Stage
{
	int stageID;		//�������
	HBITMAP bg;			//����ͼƬ
	int timeCountDown;	//��Ϸʱ�䵹��ʱ
	bool timerOn;		//��ʱ���Ƿ����У���Ϸ�Ƿ���ͣ��

};


// ��ť�ṹ��
struct Button
{
	int buttonID;	//��ť���
	bool visible;	//��ť�Ƿ�ɼ�
	HBITMAP img;	//ͼƬ
	int x;			//����x
	int y;			//����y
	int width;		//���
	int height;		//�߶�
};

// ���ǽṹ��
struct Hero
{
	HBITMAP img;	//ͼƬ
	int frame;		//��ǰ��ʾ��֡��
	int x;			//����x
	int y;			//����y
	double vx;		//�ٶ�x
	double vy;		//�ٶ�y
	bool health;	//����ֵ
};

struct Brick 
{
	int BrickID;
	bool visible;
	HBITMAP img;//ͼƬ
	int x;//����x
	int y;//����y
	int size;//�߳�
	bool danger;//�Ƿ�Ϊ����ש��
};

struct Block {
	int x, y;
	int nwidth, nheight; // ����
};

struct Map
{
	vector<Block> blocks;
	int startingPos[2];

};
//TODO: �����Ϸ��Ҫ�ĸ��������ݣ��ӵ���ש�顢���ˡ���Ļ�ȣ�


#pragma endregion


#pragma region �¼�����������


// ��ʼ����Ϸ���庯��
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ���̰����¼�������
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����ɿ��¼�������
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ����ƶ��¼�������
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����������¼�������
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �������ɿ��¼�������
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

//������߿���ײ��⺯��
void FrameCollisionDetection(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ��ʱ���¼�������
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region ������Ϸ״̬����������

// ��Ӱ�ť����
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

//���ש�麯��
Brick* CreateBrick(HBITMAP img, int x, int y, int size, bool danger);

// ������Ǻ���
Hero* CreateHero(HBITMAP img, int x, int y, bool health);

// ��ʼ����������
void InitStage(HWND hWnd, int stageID);

// ˢ������״̬����
void UpdateHero(HWND hWnd);

// �������ǵĵ�ǰ֡��
int GetHeroFrame(double dirX, double dirY);

//TODO: �����Ϸ��Ҫ�ĸ��ຯ��

#pragma endregion


#pragma region ��ͼ��������

// ��ͼ����
void Paint(HWND hWnd);

#pragma endregion