#include "stdafx.h"

#include "game.h"

/////无限刷御魂、觉醒 不影响其他程序体验
//// 1. 点击挑战
//// 2. 点击准备
//// 3. 点击结束
//// 4. 跳转到 1

DWORD WINAPI ShuaTu_YuHun()
{
	HBITMAP ZiTu = NULL;
	int Status = Challenge;

	while (true)
	{
		switch (Status)
		{
		case Challenge:
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Challenge.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (Dodge_Handling(ZiTu))
			{
				Status = Ready;
			}
			
			break;
		case Ready:
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Ready.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (Dodge_Handling(ZiTu))
			{
				Status = Judgment_ends;
			}
			
			break;
		case Judgment_ends:
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Judgment_ends.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (Dodge_Handling(ZiTu))
			{
				Status = Challenge;
			}
			Sleep(500);
			momvoerClock();
			break;
		default:
			break;
		}
		Sleep(3000);
	}

}

DWORD WINAPI ShuaTu_Guanka();

void Thread_YuHun(void)
{
	static int Is_Start = 1;
	if (Is_Start)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShuaTu_YuHun, nullptr, 0, 0);	//创建游戏外挂线程
		Is_Start = 0;
	}
	else
	{
		MessageBoxW(NULL, L"游戏已开始", NULL, MB_OK);
	}

}
