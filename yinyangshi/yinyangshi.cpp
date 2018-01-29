// yinyangshi.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <stdio.h> 
#include "yinyangshi.h"

#include "game.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND hWnd;		//主窗口句柄				

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_YINYANGSHI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YINYANGSHI));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YINYANGSHI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_YINYANGSHI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}
HWND hwndEDIT;
void SAVE_chuli();
void Thread_YuHun(void);

BOOL BitmapToBitmap32(HBITMAP   hBitmap1, HBITMAP hBitmap2);

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int   cxChar, cyChar;

	//HBITMAP ZiTu1 = (HBITMAP)LoadImage(NULL, L"image//tmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//HBITMAP ZiTu2 = (HBITMAP)LoadImage(NULL, L"image//Mobs.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	cxChar = sizeof(BITMAPINFOHEADER);

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hwndEDIT = CreateWindow(TEXT("Edit"),
			TEXT("开始挂机"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
			cxChar, cyChar * 10,
			30 * cxChar, 7 * cyChar / 4,
			hWnd, NULL,
			((LPCREATESTRUCT)lParam)->hInstance, NULL);

			//初始化游戏外部接口1
			if (Game_Init() != true)
			{
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}

		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_32771:
				MessageBoxW(NULL, L"探索开始", NULL, MB_OK);

				Game_Start();
				//Thread_YuHun();			
				//BitmapToBitmap32(ZiTu1, ZiTu2);

				break;
			case ID_32772:
				MessageBoxW(NULL, L"御魂觉醒开始", NULL, MB_OK);
				Thread_YuHun();
				break;
			case ID_32773:
				SAVE_chuli();
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		Game_Exit();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



void SAVE_chuli()
{
	static int Save_type = 0;
	char str[100];
	char * split = ",";
	char * p;
	char *next_token = nullptr;
	int x, y, nWidth, nHeight;

	if (Save_type == 0)
	{
		Save_Bmp();
		MessageBoxW(NULL, L"还需要输入保存的坐标值，再保存一次", NULL, MB_OK);
		Save_type = 1;
	}
	else
	{
		GetWindowTextA(hwndEDIT, str, 100);
		if ((p = strtok_s(str, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &x);
		}
		if ((p = strtok_s(NULL, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &y);
		}
		if ((p = strtok_s(NULL, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &nWidth);
		}
		if ((p = strtok_s(NULL, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &nHeight);
		}
		else
		{
			MessageBoxW(NULL, L"填写大小坐标不对，重来", NULL, MB_OK);
		}

		Save_Bmp(x, y, nWidth, nHeight, L"image//save2.bmp");
		MessageBoxW(NULL, L"保存成功", NULL, MB_OK);

		Save_type = 0;
	}
}