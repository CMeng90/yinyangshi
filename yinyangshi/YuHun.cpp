#include "stdafx.h"

#include "game.h"

/////����ˢ���ꡢ���� ��Ӱ��������������
//// 1. �����ս
//// 2. ���׼��
//// 3. �������
//// 4. ��ת�� 1

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
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShuaTu_YuHun, nullptr, 0, 0);	//������Ϸ����߳�
		Is_Start = 0;
	}
	else
	{
		MessageBoxW(NULL, L"��Ϸ�ѿ�ʼ", NULL, MB_OK);
	}

}
