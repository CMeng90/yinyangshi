
#include "stdafx.h"

#include "game.h"

/////����ˢͼ ��Ӱ��������������
//// 1. ѡ��ؿ� ������Ϊ500��250��
//// 2. ���̽��
//// 3. Ѱ��С��
//// 4. ����С��ʱ��Ѱ��BOSS ��С��
//// 5. Ѱ�ҽ���ֽ��
//// 6. ��ս�� 1

DWORD WINAPI ShuaTu_Guanka()
{
	HBITMAP ZiTu = NULL;
	int Status = Find_Level;

	while (true)
	{
		switch (Status)
		{
			//ѡ��ؿ� ÿ�ζ�������λ�ã���ֱ�ӷ��������
		case Find_Level:
			momvoerClock(500, 250);		

			Sleep(100);
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//tansuo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (Dodge_Handling(ZiTu))
			{
				Status = Mobs;
			}
			break;
			//Ѱ��С��
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