
#include "stdafx.h"

#include "game.h"

/////无限刷图 不影响其他程序体验
//// 1. 选择关卡 （坐标为500，250）
//// 2. 点击探索
//// 3. 寻找小怪
//// 4. 当无小怪时，寻找BOSS 或小怪
//// 5. 寻找结束纸人
//// 6. 挑战到 1

DWORD WINAPI ShuaTu_Guanka()
{
	HBITMAP ZiTu = NULL;
	int Status = Find_Level;

	while (true)
	{
		switch (Status)
		{
			//选择关卡 每次都在中心位置，故直接发送坐标点
		case Find_Level:
			momvoerClock(500, 250);		

			Sleep(100);
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//tansuo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (Dodge_Handling(ZiTu))
			{
				Status = Mobs;
			}
			break;
			//寻找小怪
		case Mobs:
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Mobs.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
				Status = Mobs;
			}
			break;
		default:
			break;
		}

		Sleep(3000);
	}

}