// JumpingGame.cpp : 定义应用程序的入口点。
//

#include "JumpingGame.h"

using namespace std;


#pragma region 全局变量

#define MAX_LOADSTRING			100		

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名


// TODO: 在此添加其它全局变量

HBITMAP bmp_Background; //背景图像资源
HBITMAP bmp_StartButton; //开始按钮图像资源
HBITMAP bmp_Hero; //主角图像资源
HBITMAP bmp_HelpButton;//帮助按钮图像资源
HBITMAP bmp_QuitButton;
HBITMAP bmp_Brick1;
HBITMAP bmp_Brick2;

Stage* currentStage = NULL; 
//当前场景状态，指针，初始化指向NULL，后续应当指向类型为Stage的结构体
Hero* theHero = NULL; 
//主角状态，指针，初始化指向NULL，后续应当指向类型为Hero的结构体
vector<Button*> buttons; 
//长度可变的结构体指针数组（vector），每个指针指向一个类型为Button的结构体
vector<Brick*> bricks;

int mouseX = 0;
int mouseY = 0;
//初始化鼠标位置为屏幕左上角
bool mouseDown = false;
//默认鼠标没有按下
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
//默认键盘什么键都没有按下
bool FrameCollision = false;
double const PI = acos(double(-1));//定义π
#pragma endregion


#pragma region Win32程序框架(主函数)



// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
	/*WinMain相当于主函数，
	hInstance represents the actual instance of an application,
	hPrevInstance is a handle to a previous instance,
	lpCmdLine represents the command-line arguments,
	nCmdShow is a value that tells how the window should be displayed.*/
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	//声明可能无关的参数，以防止编译器产生warnings

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JUMPINGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	/*LoadString函数：to copy the string to a destination buffer*/

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPINGGAME));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JUMPINGGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindow(szWindowClass, szTitle,
	   WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化,
	   CW_USEDEFAULT, 
	   CW_USEDEFAULT,
	   WINDOW_WIDTH,
	   WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		// 初始化游戏窗体
		InitGame(hWnd, wParam, lParam);
		break;
	case WM_KEYDOWN:
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		// 鼠标左键按下事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		// 定时器事件
		if(currentStage!=NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
		break;
    case WM_PAINT:
		// 绘图
		Paint(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


#pragma endregion


#pragma region 事件处理函数

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	//加载图像资源
	bmp_Background = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BG));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_Hero = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HERO));
	//bmp_HelpButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP));
	bmp_QuitButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_QUIT));
	
	//添加按钮

	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT, 300, 150);
	buttons.push_back(startButton);
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton, BUTTON_HELP_WIDTH, BUTTON_HELP_HEIGHT, 300, 230);
	buttons.push_back(helpButton);
	Button* quitButton = CreateButton(BUTTON_QUITGAME, bmp_QuitButton, BUTTON_QUITGAME_WIDTH, BUTTON_QUITGAME_HEIGHT, 300, 350);
	buttons.push_back(quitButton);

	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = true;
		break;
	case VK_DOWN:
		keyDownDown = true;
		break;
	case VK_LEFT:
		keyLeftDown = true;
		break;
	case VK_RIGHT:
		keyRightDown = true;
		break;
	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// TODO
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = false;
		break;
	case VK_DOWN:
		keyDownDown = false;
		break;
	case VK_LEFT:
		keyLeftDown = false;
		break;
	case VK_RIGHT:
		keyRightDown = false;
		break;
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	for(int i=0;i<buttons.size();i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				switch (button->buttonID) {
				case BUTTON_STARTGAME:
				{
					//TODO：判断进入哪个关卡
					InitStage(hWnd, STAGE_1);
				}
				break;
				case BUTTON_HELP:
				{
					//turn to help page...
				}
				break;
				case BUTTON_QUITGAME:
				{
					//end game!
				}
				break;
				}
			}
		}
	}

}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

//主角与边框的碰撞检测
void FrameCollisionDetection(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateHero(hWnd);

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion


#pragma region 其它游戏状态处理函数

//TODO: 添加游戏需要的更多函数

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

//添加砖块函数
Brick* CreateBrick(int brickID, HBITMAP img, int x, int y, int size)
{
	Brick* brick = new Brick();
	brick->img = img;
	brick->x = x;
	brick->y = y;
	brick->size = size;
	brick->visible = false;
	brick->danger = false;
	return brick;
}

// 添加主角函数
Hero* CreateHero(HBITMAP img, int x, int y, bool health)
{
	Hero* hero = new Hero();
	hero->img = img;
	hero->x = x;
	hero->y = y;
	hero->frame = 0;
	hero->vx = 0;
	hero->vy = 0;
	hero->health = health;
	return hero;
}

// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例
	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	if (stageID == STAGE_STARTMENU) {
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		//显示开始界面的按钮
		for(int i=0;i<buttons.size();i++)
		{
			Button* button = buttons[i];
			if (button->buttonID >= BUTTON_STARTGAME&&button->buttonID<=BUTTON_QUITGAME) 
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}
	}
	else if(stageID== STAGE_1 ) //TODO：添加多个游戏场景
	{
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;

		//显示游戏界面的按钮
		for(int i=0;i<buttons.size();i++)
		{
			Button* button = buttons[i];
			if (false) //TODO：加载游戏界面需要的按钮
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}

		// 初始化主角
		if (theHero != NULL) delete theHero;
		theHero = CreateHero(bmp_Hero, 400, 300, true);

	}
	else if (stageID == STAGE_2)
	{
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;
	}
	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


// 刷新主角状态函数
void UpdateHero(HWND hWnd)
{
	if (theHero != NULL) 
	{
		//y方向速度
		theHero->vy += HERO_ACCELERATION*0.5;
		if (keyUpDown && !keyDownDown && theHero->vy > -1 * HERO_MAXSPEED)
			theHero->vy -= HERO_ACCELERATION;
		/*else if (!keyUpDown && keyDownDown && theHero->vy < HERO_MAXSPEED)
			theHero->vy += HERO_ACCELERATION;
		else if (theHero->vy > 0)
			theHero->vy -= HERO_ACCELERATION;
		else if (theHero->vy < 0)
			theHero->vy += HERO_ACCELERATION;*/

		//x方向速度
		if (keyLeftDown && !keyRightDown && theHero->vx > -1 * HERO_MAXSPEED)
			theHero->vx -= HERO_ACCELERATION*2;
		else if (!keyLeftDown && keyRightDown && theHero->vx < HERO_MAXSPEED)
			theHero->vx += HERO_ACCELERATION*2;
		else if (theHero->vx > 0)
			theHero->vx -= HERO_ACCELERATION*2;
		else if (theHero->vx < 0)
			theHero->vx += HERO_ACCELERATION*2;

		theHero->x += (int)(theHero->vx);
		theHero->y += (int)(theHero->vy);

		int centerX = theHero->x + HERO_SIZE_X / 2;
		int centerY = theHero->y + HERO_SIZE_Y / 2;

		double dirX = mouseX - centerX;
		double dirY = mouseY - centerY;
		theHero->frame = GetHeroFrame(dirX, dirY);
	}
}

// 计算主角的当前帧数
int GetHeroFrame(double dirX, double dirY) 
{
	double dirLen = sqrt(dirX * dirX + dirY * dirY);
	if (dirLen == 0)return 0;
	double cos = -dirY / dirLen;
	double arc = acos(cos);
	if (dirX < 0)arc = 2 * PI - arc;

	int frame = (int)(arc / (2 * PI / HERO_FRAME_NUMBER));
	if (frame == HERO_FRAME_NUMBER)
		frame = 0;

	if(frame < HERO_FRAME_NUMBER / 2)
		frame = HERO_FRAME_NUMBER / 2 - frame - 1;

	return frame;
}



#pragma endregion


#pragma region 绘图函数
// 绘图函数
void Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	SelectObject(hdc_loadBmp, currentStage->bg);
	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	// 按场景分类绘制内容到缓存
	if (currentStage->stageID == STAGE_STARTMENU) {

	}
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_1) //TODO：添加多个游戏场景
	{
		// 绘制主角到缓存
		SelectObject(hdc_loadBmp, theHero->img);
		TransparentBlt(
			hdc_memBuffer, theHero->x, theHero->y,
			HERO_SIZE_X, HERO_SIZE_Y,
			hdc_loadBmp, 0, HERO_SIZE_Y * theHero->frame, HERO_SIZE_X, HERO_SIZE_Y,
			RGB(255, 255, 255)
		);
	}


	// 绘制按钮到缓存

	for(int i=0;i<buttons.size();i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(255, 255, 255)
			);
		}
	}
	
	
	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}
#pragma endregion
