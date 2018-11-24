// JumpingGame.cpp : ����Ӧ�ó������ڵ㡣
//

#include "JumpingGame.h"

using namespace std;


#pragma region ȫ�ֱ���

#define MAX_LOADSTRING			100		

// ȫ�ֱ���: 
HINSTANCE hInst; // ��ǰ����ʵ��
WCHAR szTitle[MAX_LOADSTRING]; // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING]; // ����������


// TODO: �ڴ��������ȫ�ֱ���

HBITMAP bmp_Background; //����ͼ����Դ
HBITMAP bmp_StartButton; //��ʼ��ťͼ����Դ
HBITMAP bmp_Hero; //����ͼ����Դ
HBITMAP bmp_HelpButton;//������ťͼ����Դ
HBITMAP bmp_QuitButton;
HBITMAP bmp_Brick1;
HBITMAP bmp_Brick2;

Stage* currentStage = NULL; 
//��ǰ����״̬��ָ�룬��ʼ��ָ��NULL������Ӧ��ָ������ΪStage�Ľṹ��
Hero* theHero = NULL; 
//����״̬��ָ�룬��ʼ��ָ��NULL������Ӧ��ָ������ΪHero�Ľṹ��
vector<Button*> buttons; 
//���ȿɱ�Ľṹ��ָ�����飨vector����ÿ��ָ��ָ��һ������ΪButton�Ľṹ��
vector<Brick*> bricks;

int mouseX = 0;
int mouseY = 0;
//��ʼ�����λ��Ϊ��Ļ���Ͻ�
bool mouseDown = false;
//Ĭ�����û�а���
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
//Ĭ�ϼ���ʲô����û�а���
bool FrameCollision = false;
double const PI = acos(double(-1));//�����
#pragma endregion


#pragma region Win32������(������)



// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
	/*WinMain�൱����������
	hInstance represents the actual instance of an application,
	hPrevInstance is a handle to a previous instance,
	lpCmdLine represents the command-line arguments,
	nCmdShow is a value that tells how the window should be displayed.*/
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	//���������޹صĲ������Է�ֹ����������warnings

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JUMPINGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	/*LoadString������to copy the string to a destination buffer*/

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPINGGAME));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindow(szWindowClass, szTitle,
	   WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // ���ô�����ʽ�����ɸı��С���������,
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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		// ��ʼ����Ϸ����
		InitGame(hWnd, wParam, lParam);
		break;
	case WM_KEYDOWN:
		// ���̰����¼�
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// �����ɿ��¼�
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// ����ƶ��¼�
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		// �����������¼�
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// �������ɿ��¼�
		LButtonUp(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		// ��ʱ���¼�
		if(currentStage!=NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
		break;
    case WM_PAINT:
		// ��ͼ
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


#pragma region �¼�������

// ��ʼ����Ϸ���庯��
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	//����ͼ����Դ
	bmp_Background = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BG));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_Hero = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HERO));
	//bmp_HelpButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP));
	bmp_QuitButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_QUIT));
	
	//��Ӱ�ť

	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT, 300, 150);
	buttons.push_back(startButton);
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton, BUTTON_HELP_WIDTH, BUTTON_HELP_HEIGHT, 300, 230);
	buttons.push_back(helpButton);
	Button* quitButton = CreateButton(BUTTON_QUITGAME, bmp_QuitButton, BUTTON_QUITGAME_WIDTH, BUTTON_QUITGAME_HEIGHT, 300, 350);
	buttons.push_back(quitButton);

	//��ʼ����ʼ����
	InitStage(hWnd, STAGE_STARTMENU);

	//��ʼ������ʱ��
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}

// ���̰����¼�������
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

// �����ɿ��¼�������
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

// ����ƶ��¼�������
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// �����������¼�������
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
					//TODO���жϽ����ĸ��ؿ�
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

// �������ɿ��¼�������
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

//������߿����ײ���
void FrameCollisionDetection(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	
}

// ��ʱ���¼�������
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateHero(hWnd);

	//ˢ����ʾ
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion


#pragma region ������Ϸ״̬������

//TODO: �����Ϸ��Ҫ�ĸ��ຯ��

// ��Ӱ�ť����
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

//���ש�麯��
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

// ������Ǻ���
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

// ��ʼ����Ϸ��������
void InitStage(HWND hWnd, int stageID)
{
	// ��ʼ������ʵ��
	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	if (stageID == STAGE_STARTMENU) {
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		//��ʾ��ʼ����İ�ť
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
	else if(stageID== STAGE_1 ) //TODO����Ӷ����Ϸ����
	{
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;

		//��ʾ��Ϸ����İ�ť
		for(int i=0;i<buttons.size();i++)
		{
			Button* button = buttons[i];
			if (false) //TODO��������Ϸ������Ҫ�İ�ť
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}

		// ��ʼ������
		if (theHero != NULL) delete theHero;
		theHero = CreateHero(bmp_Hero, 400, 300, true);

	}
	else if (stageID == STAGE_2)
	{
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;
	}
	//ˢ����ʾ
	InvalidateRect(hWnd, NULL, FALSE);
}


// ˢ������״̬����
void UpdateHero(HWND hWnd)
{
	if (theHero != NULL) 
	{
		//y�����ٶ�
		theHero->vy += HERO_ACCELERATION*0.5;
		if (keyUpDown && !keyDownDown && theHero->vy > -1 * HERO_MAXSPEED)
			theHero->vy -= HERO_ACCELERATION;
		/*else if (!keyUpDown && keyDownDown && theHero->vy < HERO_MAXSPEED)
			theHero->vy += HERO_ACCELERATION;
		else if (theHero->vy > 0)
			theHero->vy -= HERO_ACCELERATION;
		else if (theHero->vy < 0)
			theHero->vy += HERO_ACCELERATION;*/

		//x�����ٶ�
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

// �������ǵĵ�ǰ֡��
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


#pragma region ��ͼ����
// ��ͼ����
void Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//��ʼ������
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// ���Ʊ���������
	SelectObject(hdc_loadBmp, currentStage->bg);
	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	// ����������������ݵ�����
	if (currentStage->stageID == STAGE_STARTMENU) {

	}
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_1) //TODO����Ӷ����Ϸ����
	{
		// �������ǵ�����
		SelectObject(hdc_loadBmp, theHero->img);
		TransparentBlt(
			hdc_memBuffer, theHero->x, theHero->y,
			HERO_SIZE_X, HERO_SIZE_Y,
			hdc_loadBmp, 0, HERO_SIZE_Y * theHero->frame, HERO_SIZE_X, HERO_SIZE_Y,
			RGB(255, 255, 255)
		);
	}


	// ���ư�ť������

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
	
	
	// ������е���Ϣ���Ƶ���Ļ��
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// ������Դ��ռ���ڴ棨�ǳ���Ҫ��
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// ��������
	EndPaint(hWnd, &ps);
}
#pragma endregion
